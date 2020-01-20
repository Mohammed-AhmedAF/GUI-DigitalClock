#include "Std_Types.h"
#include "Macros.h"
#include "DIO_interface.h"
#include "TIMER0_interface.h"
#include "INTERRUPTS_interface.h"
#include "UART_interface.h"
#include "LCD_interface.h"
#include "RTC_interface.h"
#include "EEPROM_INTERNAL_interface.h"
#include "DCLOCK_interface.h"

volatile u32 u32CountTime;
extern volatile u8 u8AlarmFlag;
extern RTC_t rtc;
extern u8 u8HoursAlarm;
extern u8 u8MinutesAlarm;

void vidGetTime(void);
void vidCountOneSecond(void);

void main(void)
{
	/*Initialization of LCD*/	
	LCD_vidInit();
	/*Initialization UART*/
	UART_vidInit();

	/*Initializing RTC*/
	RTC_vidInit();

	INTERRUPTS_vidEnableInterrupt(INTERRUPTS_USART_RXC);
	INTERRUPTS_vidPutISRFunction(INTERRUPTS_USART_RXC,DCLOCK_vidGetTime);

	TIMER0_vidInit(TIMER0_WGM_NORMAL,TIMER0_COM_NORMAL,TIMER0_CLK_1);

	INTERRUPTS_vidEnableInterrupt(INTERRUPTS_TIMER0_OVF);
	INTERRUPTS_vidPutISRFunction(INTERRUPTS_TIMER0_OVF,vidCountOneSecond);

	INTERRUPTS_vidSetGlobalInterruptFlag();

	DIO_vidSetPinDirection(DIO_PORTA,DIO_PIN3,DIO_OUTPUT);

	/*After reset, check alarm flag in case the alarm was set and the microcontroller
	 * has been reset afterwards before the alarm was cleared*/
	DCLOCK_vidCheckAlarmFlag();
	while(1) {			
	}
}

void vidCountOneSecond(void)
{
	u32CountTime++;
	if (u32CountTime == 31500)
	{
		u32CountTime = 0;	
		LCD_vidSendCommand(LCD_RETURN_HOME);
		RTC_vidGetTime(&rtc);
		LCD_vidGoToXY(LCD_XPOS_SHIFT+LCD_XPOS6,LCD_YPOS0);
		LCD_vidWriteNumber(RTC_BCD2DEC(rtc.u8Seconds));
		LCD_vidGoToXY(LCD_XPOS_SHIFT+LCD_XPOS5,LCD_YPOS0);
		LCD_vidWriteCharacter(':');
		LCD_vidGoToXY(LCD_XPOS_SHIFT+LCD_XPOS3,LCD_YPOS0);
		LCD_vidWriteNumber(RTC_BCD2DEC(rtc.u8Minutes));
		LCD_vidGoToXY(LCD_XPOS_SHIFT+LCD_XPOS2,LCD_YPOS0);
		LCD_vidWriteCharacter(':');
		LCD_vidGoToXY(LCD_XPOS_SHIFT+LCD_XPOS0,LCD_YPOS0);
		LCD_vidWriteNumber(RTC_BCD2DEC(rtc.u8Hours));
		/*Check for alarm*/
		if (u8AlarmFlag == DCLOCK_ALARM_SET)
		{
			if ((RTC_BCD2DEC(rtc.u8Hours) == u8HoursAlarm) && (RTC_BCD2DEC(rtc.u8Minutes) == u8MinutesAlarm))
			{
				DIO_vidTogglePin(DIO_PORTA,DIO_PIN3);
			}
		}

	}
}


