
/*=INCLUDES=*/
#include "tgincludes.h"

/*=GLOBALS=*/
TGUI tgui;

/*==============================*/
/* PROGRAM ENTRY POINT          */
/*==============================*/
int main(int argc,char **argv)
{
    GameControls gmc;

    /*=NOTE: for reference see "../include/X11/keysymdef.h"=*/
    gmc.tank_ON_MOVE_UP.key=XK_Up; // set key to which handler must response
    gmc.tank_ON_MOVE_UP.khp=on_up; // set handler itself

    gmc.tank_ON_MOVE_DOWN.key=XK_Down;
    gmc.tank_ON_MOVE_DOWN.khp=on_down;

    gmc.tank_ON_MOVE_LEFT.key=XK_Left;
    gmc.tank_ON_MOVE_LEFT.khp=on_left;

    gmc.tank_ON_MOVE_RIGHT.key=XK_Right;
    gmc.tank_ON_MOVE_RIGHT.khp=on_right;

    gmc.tank_ON_FIRE.key=XK_Home;
    gmc.tank_ON_FIRE.khp=on_fire;

    gmc.game_ON_ESCAPE.key=XK_Escape;
    gmc.game_ON_ESCAPE.khp=on_escape;

    /*=And so on...=*/

    tgui.SetGameControls(&gmc); // now set all those (above) controls

    tgui.Initialize(&argc,argv); // initialize tank game user iterface
    tgui.CreateMainWindow(0x0,
			  0x0,
			  NR_HORIZONTAL_MODELS*MODEL_WIDTH,
			  NR_VERTICAL_MODELS*MODEL_HEIGHT);
    tgui.CreateIndicators();
    TGNewGame();
    tgui.Loop(); // run main application loop

    return 0x0;
}
