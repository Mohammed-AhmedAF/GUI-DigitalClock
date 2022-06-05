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
void DCLOCK_vidDisplayTime(void);

#define DCLOCK_ALARM_ADDRESS 10
#define DCLOCK_ALARM_SET 1
#define DCLOCK_ALARM_CLEARED 0
#define DCLOCK_ALARM_HOUR_ADDRESS 0
#define DCLOCK_ALARM_MINUTE_ADDRESS 1
#define DCLOCK_MESSAGESIZE 4
#define DCLOCK_INDEX_COMMAND 0

/*Number of OVFs for 1 second*/
#define DCLOCK_OVF_COUNT_SECOND 31250

#endif
