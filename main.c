#include "Std_Types.h"
#include "Macros.h"
#include "DIO_interface.h"
#include "TIMER0_interface.h"
#include "INTERRUPTS_interface.h"
#include "UART_interface.h"
#include "LCD_interface.h"
#include "RTC_interface.h"

#define DCLOCK_ALARM_SET 1
#define DCLOCK_ALARM_CLEARED 0

volatile u8 u8index = 0;
volatile u8 u8Byte;
volatile u32 u32CountTime;
u8 u8HoursAlarm;
u8 u8MinutesAlarm;
RTC_t rtc;
volatile u8 u8TimeArray[4];
volatile u8 u8AlarmFlag = DCLOCK_ALARM_CLEARED;

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
	INTERRUPTS_vidPutISRFunction(INTERRUPTS_USART_RXC,vidGetTime);

	TIMER0_vidInit(TIMER0_WGM_NORMAL,TIMER0_COM_NORMAL,TIMER0_CLK_1);

	INTERRUPTS_vidEnableInterrupt(INTERRUPTS_TIMER0_OVF);
	INTERRUPTS_vidPutISRFunction(INTERRUPTS_TIMER0_OVF,vidCountOneSecond);

	INTERRUPTS_vidSetGlobalInterruptFlag();

	DIO_vidSetPinDirection(DIO_PORTA,DIO_PIN3,DIO_OUTPUT);
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

void vidGetTime(void)
{

	u8Byte = UART_u8GetReceivedByte();
	u8TimeArray[u8index] = u8Byte;
	u8index++;
	if (u8index == 4)
	{
		u8index = 0;
		if (u8TimeArray[0] == 'c')
		{
			rtc.u8Seconds = RTC_DEC2BCD(u8TimeArray[3]);
			rtc.u8Minutes = RTC_DEC2BCD(u8TimeArray[2]);
			rtc.u8Hours = RTC_DEC2BCD(u8TimeArray[1]);
			RTC_vidSetTime(&rtc);
	
		}
		else if (u8TimeArray[0] == 'a')
		{

			LCD_vidGoToXY(LCD_XPOS0+LCD_XPOS_SHIFT,LCD_YPOS2);
			LCD_vidWriteNumber(u8TimeArray[1]);
			LCD_vidGoToXY(LCD_XPOS2+LCD_XPOS_SHIFT,LCD_YPOS2);
			LCD_vidWriteCharacter(':');
			LCD_vidGoToXY(LCD_XPOS3+LCD_XPOS_SHIFT,LCD_YPOS2);
			LCD_vidWriteNumber(u8TimeArray[2]);
			/*Store in alarm variables*/
			u8HoursAlarm = u8TimeArray[1];
			u8MinutesAlarm = u8TimeArray[2];
			u8AlarmFlag = DCLOCK_ALARM_SET;

		}

	}
}
