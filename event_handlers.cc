
#include "tgincludes.h"

extern TGUI tgui; // make it visible for this module
extern int tgt_paused;

void ExposureEventHandler(Widget w,XtPointer client_data,XEvent *event,Boolean *continue_to_dispatch)
{
    tgui.Redraw(); // since exposure event has arrived, I have to redraw main game window
}

void KeypressEventHandler(Widget w,XtPointer client_data,XEvent *event,Boolean *continue_to_dispatch)
{
    KeySym keysym;

    /*=Convert key code to key symbol=*/
    keysym=XKeycodeToKeysym(tgui.display,event->xkey.keycode,0x0);

    if (keysym==tgui.gamecontrols.tank_ON_MOVE_UP.key)
	tgui.gamecontrols.tank_ON_MOVE_UP.khp(NULL);

    if (keysym==tgui.gamecontrols.tank_ON_MOVE_DOWN.key)
	tgui.gamecontrols.tank_ON_MOVE_DOWN.khp(NULL);

    if (keysym==tgui.gamecontrols.tank_ON_MOVE_LEFT.key)
	tgui.gamecontrols.tank_ON_MOVE_LEFT.khp(NULL);

    if (keysym==tgui.gamecontrols.tank_ON_MOVE_RIGHT.key)
	tgui.gamecontrols.tank_ON_MOVE_RIGHT.khp(NULL);

    if (keysym==tgui.gamecontrols.tank_ON_FIRE.key)
	tgui.gamecontrols.tank_ON_FIRE.khp(NULL);

    if (keysym==tgui.gamecontrols.game_ON_ESCAPE.key)
	tgui.gamecontrols.game_ON_ESCAPE.khp(NULL);
}

/*=CALLBACK FUNCTIONS=*/
void on_ngame(Widget w,XtPointer client_data,XtPointer call_data)
{
    TGResetGame();
}

void on_npause(Widget w,XtPointer client_data,XtPointer call_data)
{
    if (tgt_paused) {
	XtVaSetValues(w,XtNlabel,"Pause",NULL);
	TGContinueTimer();
    } else {
	XtVaSetValues(w,XtNlabel,"Continue",NULL);
	TGPauseTimer();
    }
}

void on_nexit(Widget w,XtPointer client_data,XtPointer call_data)
{
    TGTerminateGame();
}

void on_noptions(Widget w,XtPointer client_data,XtPointer call_data)
{
    /*=Display options dialog=*/
    printf("Options menu item selected.\n");
}

void on_nhelp(Widget w,XtPointer client_data,XtPointer call_data)
{
    /*=Display help dialog=*/
    printf("Help menu item selected.\n");
}
