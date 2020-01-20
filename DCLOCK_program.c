#include "Std_Types.h"
#include "Macros.h"
#include "DIO_interface.h"
#include "EEPROM_INTERNAL_interface.h"
#include "UART_interface.h"
#include "LCD_interface.h"
#include "RTC_interface.h"
#include "DCLOCK_interface.h"

volatile u32 u32CountTime;
volatile u8 u8index = 0;
volatile u8 u8Byte;
u8 u8HoursAlarm;
u8 u8MinutesAlarm;
RTC_t rtc;
volatile u8 u8TimeArray[4];
volatile u8 u8AlarmFlag = DCLOCK_ALARM_CLEARED;

/*Checking alarm flag when microcontroller is reset*/
void DCLOCK_vidCheckAlarmFlag(void)
{
	u8AlarmFlag = EEPROM_INTERNAL_u8ReadByte(DCLOCK_ALARM_ADDRESS);
}

/*Alarm is cleared by a command from GUI application*/
void DCLOCK_vidClearAlarmFlag(void)
{
	u8AlarmFlag = DCLOCK_ALARM_CLEARED;
	EEPROM_INTERNAL_vidWriteByte(DCLOCK_ALARM_ADDRESS,u8AlarmFlag);
	/*Clear displayed alarm from the screen*/
	LCD_vidSendCommand(LCD_CLEAR_SCREEN);
}

void DCLOCK_vidGetTime(void)
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
			LCD_vidGoToXY(LCD_XPOS0,LCD_YPOS2);
			LCD_vidWriteString("Alarm:");
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
			EEPROM_INTERNAL_vidWriteByte(DCLOCK_ALARM_ADDRESS,DCLOCK_ALARM_SET);

		}
		else if (u8TimeArray[0] == 'u')
		{
			DCLOCK_vidClearAlarmFlag();
		}

	}
}

void DCLOCK_vidCountOneSecond(void)
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
