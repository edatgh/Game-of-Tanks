
#ifndef _TANK__TGUI_H_
#define _TANK__TGUI_H_

/*=INCLUDES=*/
#include "tgincludes.h"

/*=CONSTANTS=*/
#define NR_HORIZONTAL_MODELS 0x14
#define NR_VERTICAL_MODELS 0xF
#define OUTPUTWINDOWHEIGHT 0x10

/*=TYPES=*/
typedef void (*KEYHANDLERPROC)(void *);
typedef struct _gamekeyhandler_t {
    KeySym key;
    KEYHANDLERPROC khp;
} GameKeyHandler;
typedef struct _gamecontrols_t {
    GameKeyHandler tank_ON_MOVE_UP;
    GameKeyHandler tank_ON_MOVE_DOWN;
    GameKeyHandler tank_ON_MOVE_LEFT;
    GameKeyHandler tank_ON_MOVE_RIGHT;
    GameKeyHandler tank_ON_FIRE;
    GameKeyHandler game_ON_ESCAPE;
    /*=...=*/
} GameControls;

/*===========================================*/
/* Tank Game User Interface class definition */
/*===========================================*/
class TGUI {
public: // public class interface
    TGUI(void); // constructor
    void Initialize(int *argc,char **argv);
    void Redraw(void);
    void Clear(void);
    void ClearRect(int x,int y,int w,int h);
    void Loop(void);
    void SetGameControls(GameControls *p_gamecontrols);
    void SetDefaultGameControls(void);
    void PrintMessage(char *text);
    void PrintStatus(char *text);
    void ResetMenu(void);
    void CreateIndicators(void);
    void RedrawIndicators(void);
    void SetArmorIndicator(int val);
    void SetHealthIndicator(int val);
    void SetDeathIndicator(int val);

    /*=MISC. WINDOW CREATION FUNCTIONS=*/
    void CreateMainWindow(Position x,Position y,Dimension w,Dimension h);
    void CreateAboutBox(void);
    void CreateMenu(void);
    void CreateStdOut(void);
    void ClearStdOut(void);
    void UpdateStdOut(void);
    void CreateOptionsBox(void);

    /*=PUBLIC VARIABLES=*/
    XtAppContext app_context; // context of the application
    Display *display; // X Server connection descriptor
    GameControls gamecontrols; // list of game control keys and their handlers
    /*=GC=*/
    GC gc_black,gc_normal,gc_red;
    /*=Menu relative stuff=*/
    Widget menu_bar,ngame,npause,nexit,noptions,nhelp;
    /*=========*/
    Window root_window; // root window of the screen
    Pixmap game_offscreen; // will be mapped on game field
    Pixmap output_offscreen;
    int gf_width,gf_height; // game screen width/height
    void InstallEventHandlers(void);
    void UninstallEventHandlers(void);
    void Terminate(void);

private: // local class interface

/*=======================FUNCTIONS===============================*/
    void InitOffscreen(void);
    void InitGC(void); // creates all necessary GCs

/*=======================VARIABLES===============================*/
    /*=Widgets=*/
    Widget top_level; // top-level widget of the application
    Widget main_window; // main window of the application
    Widget drawing_area; // actually game field widget
    Widget stdout_widget;

    Screen *screen; // screen descriptor on which the application runs
    unsigned int depth; // depth of color of the screen
    Window game_field; // window on which the game action actually appears
    Window msgoutput; // output window

    /*=Models=*/
    Model armor_win,health_win,death_win;
};

/*=EXTERNAL FUNCTIONS=*/
void ExposureEventHandler(Widget w,XtPointer client_data,XEvent *event,Boolean *continue_to_dispatch);
void KeypressEventHandler(Widget w,XtPointer client_data,XEvent *event,Boolean *continue_to_dispatch);
void on_ngame(Widget w,XtPointer client_data,XtPointer call_data);
void on_npause(Widget w,XtPointer client_data,XtPointer call_data);
void on_nexit(Widget w,XtPointer client_data,XtPointer call_data);
void on_noptions(Widget w,XtPointer client_data,XtPointer call_data);
void on_nhelp(Widget w,XtPointer client_data,XtPointer call_data);

#endif
