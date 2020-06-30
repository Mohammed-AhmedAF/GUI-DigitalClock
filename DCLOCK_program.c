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
volatile u8 u8Temperature;
RTC_t rtc;
volatile u8 u8MessageArray[DCLOCK_MESSAGESIZE];
volatile u8 u8AlarmFlag = DCLOCK_ALARM_CLEARED;

u8 * u8DaysOfWeek_Arr[7] = {"Mon","Tues","Wed","Thu","Fri","Sat","Sun"};

/*Declaring alarm values struct*/
strctAlarm_t strctAlarm;

/*Checking alarm flag when microcontroller is reset*/
void DCLOCK_vidCheckAlarmFlag(void)
{
	u8AlarmFlag = EEPROM_INTERNAL_u8ReadByte(DCLOCK_ALARM_ADDRESS);
	if (u8AlarmFlag == DCLOCK_ALARM_SET) 
	{
		DCLOCK_vidRetrieveAlarmValues(&strctAlarm);
		LCD_vidGoToXY(LCD_XPOS0,LCD_YPOS1);
		LCD_vidWriteString("Alarm:");
		LCD_vidGoToXY(LCD_XPOS0+LCD_XPOS_SHIFT,LCD_YPOS1);
		LCD_vidWriteNumber(strctAlarm.u8Hour);
		LCD_vidGoToXY(LCD_XPOS2+LCD_XPOS_SHIFT,LCD_YPOS1);
		LCD_vidWriteCharacter(':');
		LCD_vidGoToXY(LCD_XPOS3+LCD_XPOS_SHIFT,LCD_YPOS1);
		LCD_vidWriteNumber(strctAlarm.u8Minute);
	}
}

/*Alarm is cleared by a command from GUI application*/
static void DCLOCK_vidClearAlarmFlag(void)
{
	u8AlarmFlag = DCLOCK_ALARM_CLEARED;
	EEPROM_INTERNAL_vidWriteByte(DCLOCK_ALARM_ADDRESS,u8AlarmFlag);
	/*Clear displayed alarm from the screen*/
	LCD_vidClearLine(LCD_YPOS1);
}

/*Called when UART receive interrupt event happens*/
/**
 * @breif Called by UART receive interrupt.
 *
 *
 */

void DCLOCK_vidGetTime(void)
{

	u8Byte = UART_u8GetReceivedByte();
	u8MessageArray[u8index] = u8Byte;
	u8index++;
	if (u8index == DCLOCK_MESSAGESIZE)
	{
		u8index = 0;
		if (u8MessageArray[0] == 'c')
		{
			rtc.u8Seconds = RTC_DEC2BCD(u8MessageArray[3]);
			rtc.u8Minutes = RTC_DEC2BCD(u8MessageArray[2]);
			rtc.u8Hours = RTC_DEC2BCD(u8MessageArray[1]);
			RTC_vidSetTime(&rtc);
	
		}
		else if (u8MessageArray[0] == 'a')
		{
			/*Store in alarm variables*/
			strctAlarm.u8Hour = u8MessageArray[1];
			strctAlarm.u8Minute = u8MessageArray[2];
			LCD_vidSendCommand(LCD_CLEAR_SCREEN);
			LCD_vidGoToXY(LCD_XPOS0,LCD_YPOS1);
			LCD_vidWriteString("Alarm:");
			LCD_vidGoToXY(LCD_XPOS0+LCD_XPOS_SHIFT,LCD_YPOS1);
			LCD_vidWriteNumber(strctAlarm.u8Hour);
			LCD_vidGoToXY(LCD_XPOS2+LCD_XPOS_SHIFT,LCD_YPOS1);
			LCD_vidWriteCharacter(':');
			LCD_vidGoToXY(LCD_XPOS3+LCD_XPOS_SHIFT,LCD_YPOS1);
			LCD_vidWriteNumber(strctAlarm.u8Minute);
			u8AlarmFlag = DCLOCK_ALARM_SET;
			EEPROM_INTERNAL_vidWriteByte(DCLOCK_ALARM_ADDRESS,DCLOCK_ALARM_SET);
			/*Store alarm values in EEPROM, to retrieve them if system
			 *is reset before alarm is cleared */
			DCLOCK_vidStoreAlarmValues(&strctAlarm);

		}
		/*Set date*/
		else if (u8MessageArray[0] == 'd')
		{
			rtc.u8Days = RTC_DEC2BCD(u8MessageArray[1]);
			rtc.u8Months = RTC_DEC2BCD(u8MessageArray[2]);
			rtc.u8Years = RTC_DEC2BCD(u8MessageArray[3]);
			RTC_vidSetDate(&rtc);
		}
		else if (u8MessageArray[0] == 'w')
		{
			LCD_vidSendCommand(LCD_CLEAR_SCREEN);
			rtc.u8DayOfWeek = RTC_DEC2BCD(u8MessageArray[1]);
			RTC_vidSetDayOfWeek(&rtc);
		}
		/*Clear alarm*/
		else if (u8MessageArray[0] == 'u')
		{
			DCLOCK_vidClearAlarmFlag();
		}
		/*Clear screen*/
		else if (u8MessageArray[0] == 'r')
		{
			DCLOCK_vidResetSystem();
		}
		else if (u8MessageArray[0] == 't')
		{
			u8Temperature = u8MessageArray[1];
			LCD_vidGoToXY(LCD_XPOS10,LCD_YPOS3);
			LCD_vidWriteString("Temp.: ");
			LCD_vidWriteNumber(u8Temperature);
		}

	}
}

/*Called when timer overflow interrupt event happens*/
void DCLOCK_vidCountOneSecond(void)
{
	u32CountTime++;
	if (u32CountTime == 31500)
	{
		u32CountTime = 0;	
		/*Show time*/
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
		/*Show date*/
		LCD_vidGoToXY(LCD_XPOS0+5,LCD_YPOS2);
		RTC_vidGetDate(&rtc);
		LCD_vidWriteNumber(RTC_BCD2DEC(rtc.u8Days));
		LCD_vidWriteCharacter(':');
		LCD_vidWriteNumber(RTC_BCD2DEC(rtc.u8Months));
		LCD_vidWriteCharacter(':');
		LCD_vidWriteNumber(RTC_BCD2DEC(rtc.u8Years)+2000);
		/*Get day of the week*/
		RTC_vidGetDayOfWeek(&rtc);
		LCD_vidGoToXY(LCD_XPOS0,LCD_YPOS2);
		LCD_vidWriteString(u8DaysOfWeek_Arr[RTC_BCD2DEC(rtc.u8DayOfWeek)]);
		/*Check for alarm*/
		if (u8AlarmFlag == DCLOCK_ALARM_SET)
		{
			if ((RTC_BCD2DEC(rtc.u8Hours) == strctAlarm.u8Hour) && (RTC_BCD2DEC(rtc.u8Minutes) == strctAlarm.u8Minute))
			{
				DIO_vidTogglePin(DIO_PORTA,DIO_PIN3);
			}
			/*Handling the case of when the alarm should be cleared by software, not user input*/
			/*The following condition clears the alarm flag after 1 minute*/
			else if ((RTC_BCD2DEC(rtc.u8Hours) == strctAlarm.u8Hour) && (RTC_BCD2DEC(rtc.u8Minutes) > strctAlarm.u8Minute))
			{
				DCLOCK_vidClearAlarmFlag();
			}
		}

	}
}


/**
 * @breif Stores hours and minutes of alarm into internal EEPROM.
 *
 */
void DCLOCK_vidStoreAlarmValues(strctAlarm_t * ptStrctAlarm) 
{
	EEPROM_INTERNAL_vidWriteByte(DCLOCK_ALARM_HOUR_ADDRESS,ptStrctAlarm->u8Hour);
	EEPROM_INTERNAL_vidWriteByte(DCLOCK_ALARM_MINUTE_ADDRESS,ptStrctAlarm->u8Minute);
}

/*
 * @breif Retreives alarm values(minutes, hours), normally after system reset.
 * @param ptStrctAlarm pointer to struct holding minutes and hours.
 *
 */

void DCLOCK_vidRetrieveAlarmValues(strctAlarm_t * ptStrctAlarm) 
{
	ptStrctAlarm->u8Hour = EEPROM_INTERNAL_u8ReadByte(DCLOCK_ALARM_HOUR_ADDRESS);
	ptStrctAlarm->u8Minute = EEPROM_INTERNAL_u8ReadByte(DCLOCK_ALARM_MINUTE_ADDRESS);

}

/*
 * @breif Reset system: just clear the screen
 * @detail Resets the system
 * */

void DCLOCK_vidResetSystem(void)
{
	LCD_vidSendCommand(LCD_CLEAR_SCREEN);
	/*Displaying alarm notification if alarm is set*/
	DCLOCK_vidCheckAlarmFlag();
}
