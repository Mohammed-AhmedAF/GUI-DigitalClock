#ifndef _DCLOCK_INTERFACE_H
#define _DCLOCK_INTERFACE_H

typedef struct {
	u8 u8Hour;
	u8 u8Minute;
} strctAlarm_t;

void DCLOCK_vidCheckAlarmFlag(void);
static void DCLOCK_vidClearAlarmFlag(void);
void DCLOCK_vidGetTime(void);
void DCLOCK_vidCountOneSecond(void);
void DCLOCK_vidStoreAlarmValues(strctAlarm_t *);
void DCLOCK_vidRetrieveAlarmValues(strctAlarm_t *);
void DCLOCK_vidResetSystem(void);

/*Stopwatch*/
void STOPWATCH_vidToggle(void);
void STOPWATCH_vidRun(void);
void STOPWATCH_vidStop(void);

#define DCLOCK_ALARM_ADDRESS 10
#define DCLOCK_ALARM_SET 1
#define DCLOCK_ALARM_CLEARED 0
#define DCLOCK_ALARM_HOUR_ADDRESS 0
#define DCLOCK_ALARM_MINUTE_ADDRESS 1
#define DCLOCK_MESSAGESIZE 4

#endif
