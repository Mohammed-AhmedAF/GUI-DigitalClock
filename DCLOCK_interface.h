#ifndef _DCLOCK_INTERFACE_H
#define _DCLOCK_INTERFACE_H
void DCLOCK_vidCheckAlarmFlag(void);
void DCLOCK_vidClearAlarmFlag(void);
void DCLOCK_vidGetTime(void);
void DCLOCK_vidCountOneSecond(void);

#define DCLOCK_ALARM_ADDRESS 10
#define DCLOCK_ALARM_SET 1
#define DCLOCK_ALARM_CLEARED 0

#endif
