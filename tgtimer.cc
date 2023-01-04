
/*=INCLUDES=*/
#include "tgincludes.h"

/*=EXTERNALS=*/
extern TGUI tgui;

/*=CONFIG=*/
#define MAIN_TIMER_INTERVAL 0x40

/*=GLOBALS=*/
TGTIMER_ID tgt_free_ID;
XtIntervalId main_timer_ID;
TimerListItem *first_tli; // points to the first timer list item
int tgt_paused;

/*=LOCAL FUNCTION PROTOTYPES=*/
void TGResetTimer(void);

/*=X Toolkit-style timer handler=*/
void tgt_main_timer_handler(XtPointer client_data,XtIntervalId *timer)
{
    TimerListItem *p_tli=first_tli;

    while (p_tli) {
	if (p_tli->tgtimer.tgt_cf) {
	    p_tli->tgtimer.tgt_cf=0x0; // mark it as uncomplete
	    p_tli->tgtimer.xt_timer_ID=XtAppAddTimeOut(tgui.app_context,
						       p_tli->tgtimer.tgt_interval,
						       p_tli->tgtimer.tgt_timerproc,
						       p_tli);
	}
	p_tli=p_tli->next_tli;
    }
    TGResetTimer();
}

/*=FUNCTIONS=*/
void TGInitializeTimer(void)
{
    tgt_free_ID=0x0;
    main_timer_ID=0x0;
    first_tli=NULL;
    tgt_paused=0x0;
    TGResetTimer();
}

TGTIMER_ID TGAddTimer(XtTimerCallbackProc timerproc,XtIntervalId interval)
{
    TimerListItem *new_tli;

    /*=Allocate the memory for the new timer list item=*/
    new_tli=(TimerListItem *)malloc(sizeof(TimerListItem));

    /*=Fill it in=*/
    new_tli->tgtimer.tgt_interval=interval;
    new_tli->tgtimer.tgt_timerproc=timerproc;
    new_tli->tgtimer.tgt_ID=tgt_free_ID;
    new_tli->tgtimer.xt_timer_ID=0x0;
    new_tli->tgtimer.tgt_cf=0x1; // by default it is in complete state

    /*=Link it=*/
    new_tli->next_tli=first_tli;
    first_tli=new_tli;

    tgt_free_ID++;

    return tgt_free_ID-0x1;
}

void TGTimerComplete(XtPointer client_data)
{
    ((TimerListItem *)client_data)->tgtimer.tgt_cf=0x1; // set it to "complete" state
}

void TGRemoveTimer(TGTIMER_ID tgtimer_ID)
{
    TimerListItem *tmp1,*tmp2;

    tmp1=tmp2=first_tli;

    while (tmp1) {
	if (tmp1->tgtimer.tgt_ID==tgtimer_ID) {
	    /*=Remove the item from the list=*/
	    while (tmp2) {
		if (tmp2->next_tli==tmp1) {
		    tmp2->next_tli=tmp1->next_tli;
		    break;
		}
		tmp2=tmp2->next_tli;
	    }
	    if (tmp2==NULL)
		first_tli=tmp1->next_tli;

	    /*=Free the memory used by the item=*/
	    free(tmp1);
	    return;
	}
	tmp1=tmp1->next_tli;
    }
}

void TGDestroyTimer(void)
{
    TimerListItem *dead_tli=NULL,*p_tli=first_tli;

    if (!main_timer_ID)
	return;

    XtRemoveTimeOut(main_timer_ID);
    main_timer_ID=0x0;
    tgt_paused=0x1;

    /*=Remove all timer list items=*/
    while (p_tli) {
	XtRemoveTimeOut(p_tli->tgtimer.xt_timer_ID);
	dead_tli=p_tli;
	p_tli=p_tli->next_tli;
	free(dead_tli);
    }
}

void TGPauseTimer(void)
{
    if (main_timer_ID==0x0)
	return;

    XtRemoveTimeOut(main_timer_ID);
    tgt_paused=0x1;
}

void TGContinueTimer(void)
{
    if (main_timer_ID==0x0)
	return;

    TGResetTimer();
    tgt_paused=0x0;
}

/*=LOCAL=*/
void TGResetTimer(void)
{
    /*=Start main timer handler=*/
    main_timer_ID=XtAppAddTimeOut(tgui.app_context,
				  MAIN_TIMER_INTERVAL,
				  tgt_main_timer_handler,
				  NULL);
}
