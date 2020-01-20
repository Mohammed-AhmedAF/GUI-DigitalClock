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
	
	/*Timer used to cause an interrupt every second to retrieve current time from RTC*/
	TIMER0_vidInit(TIMER0_WGM_NORMAL,TIMER0_COM_NORMAL,TIMER0_CLK_1);

	INTERRUPTS_vidEnableInterrupt(INTERRUPTS_TIMER0_OVF);
	INTERRUPTS_vidPutISRFunction(INTERRUPTS_TIMER0_OVF,DCLOCK_vidCountOneSecond);

	INTERRUPTS_vidSetGlobalInterruptFlag();

	DIO_vidSetPinDirection(DIO_PORTA,DIO_PIN3,DIO_OUTPUT);

	/*After reset, check alarm flag in case the alarm was set and the microcontroller
	 * has been reset afterwards before the alarm was cleared*/
	DCLOCK_vidCheckAlarmFlag();
	while(1) {			
	}
}




