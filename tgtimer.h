
#ifndef _TANK__TGTIMER_H_
#define _TANK__TGTIMER_H_

/*=INCLUDES=*/
#include "tgincludes.h"

/*=TYPES=*/
typedef unsigned int TGTIMER_ID;

typedef struct _tgtimer_t {
    unsigned long int tgt_interval;
    XtTimerCallbackProc tgt_timerproc;
    TGTIMER_ID tgt_ID;
    XtIntervalId xt_timer_ID;
    int tgt_cf; // Completion flag
} TGTimer;

typedef struct _timerlistitem_t {
    TGTimer tgtimer;
    struct _timerlistitem_t *next_tli; // pointer to the next timer list item
} TimerListItem;

/*=FUNCTION PROTOTYPES=*/
void TGInitializeTimer(void);
TGTIMER_ID TGAddTimer(XtTimerCallbackProc timerproc,XtIntervalId interval);
void TGTimerComplete(XtPointer client_data);
void TGRemoveTimer(TGTIMER_ID tgtimer_ID);
void TGPauseTimer(void);
void TGContinueTimer(void);
void TGDestroyTimer(void);

#endif
