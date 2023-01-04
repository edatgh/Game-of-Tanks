
#include "tgincludes.h"

/*==========================================*/
/* Tank Game Interface class implementation */
/*==========================================*/

/*=PUBLIC=*/
TGUI::TGUI(void)
{
    gf_width=0x0; // default game field width (in pixels)
    gf_height=0x0; // default game field height (in pixels)
}

void TGUI::Initialize(int *argc,char **argv)
{
    /*=Establish a connection to X display and make all the necessary initializations=*/
    top_level=XtVaOpenApplication(&app_context, // returned application context
				  "TankGame", // class of the application
				  NULL, // options
				  0x0, // number of options
				  argc, // pointer to number of command line arguments
				  argv, // pointer to command line arguments
				  NULL, // resources
				  sessionShellWidgetClass, // class of the application widget
				  NULL);

    /*=Set up local class variables=*/
    display=XtDisplay(top_level); // get display descriptor
    screen=XtScreen(top_level); // get screen descriptor
    depth=DefaultDepthOfScreen(screen); // get defalt depth
    root_window=RootWindowOfScreen(screen); // get root window of the screen
}

void TGUI::CreateMainWindow(Position x,Position y,Dimension w,Dimension h)
{
    /*=Create main window=*/
    main_window=XtVaCreateManagedWidget("main_window", // name of the widget
					boxWidgetClass, // class of the widget
					top_level, // parent of the widged
					NULL);

    /*=Create menu=*/
    CreateMenu();

    /*=Create drawing area=*/
    drawing_area=XtVaCreateManagedWidget("drawing_area", // name
					 coreWidgetClass, // class
					 main_window, // parent
					 XtNx,x, // x-coordinate (position)
					 XtNy,y, // y-coordinate (position)
					 XtNwidth,w, // width of the widget
					 XtNheight,h, // height of the widget
					 NULL);

    gf_width=w;
    gf_height=h;

    InitOffscreen(); // create offscreen buffer
    InitGC(); // create some usefull GCs

    /*=Create standard output window=*/
    CreateStdOut();

    /*=Draw all widgets on the screen=*/
    XtRealizeWidget(top_level);

    /*=Set up all local class variables=*/
    game_field=XtWindow(drawing_area); // get window of drawing area
    msgoutput=XtWindow(stdout_widget);
}

void TGUI::CreateAboutBox(void)
{

}

void TGUI::CreateMenu(void)
{
    /*=Create menu bar itself=*/
    menu_bar=XtVaCreateManagedWidget("menu_bar",
				     boxWidgetClass,
				     main_window,
				     XtNorientation,XtEhorizontal,
				     NULL);

    /*=Create buttons (menu bar items)=*/
    ngame=XtVaCreateManagedWidget("ngame",
				  commandWidgetClass,
				  menu_bar,
				  XtNlabel,"New game",
				  NULL);

    npause=XtVaCreateManagedWidget("npause",
				   commandWidgetClass,
				   menu_bar,
				   XtNlabel,"Pause",
				   NULL);

    nexit=XtVaCreateManagedWidget("nexit",
				  commandWidgetClass,
				  menu_bar,
				  XtNlabel,"Exit game",
				  NULL);

//    noptions=XtVaCreateManagedWidget("noptions",
//				     commandWidgetClass,
//				     menu_bar,
//				     XtNlabel,"Options",
//				     NULL);
//
//    nhelp=XtVaCreateManagedWidget("nhelp",
//				  commandWidgetClass,
//				  menu_bar,
//				  XtNlabel,"Help",
//				  NULL);

    /*=Add all callbacks=*/
    XtAddCallback(ngame,XtNcallback,on_ngame,NULL);
    XtAddCallback(npause,XtNcallback,on_npause,NULL);
    XtAddCallback(nexit,XtNcallback,on_nexit,NULL);
    //XtAddCallback(noptions,XtNcallback,on_noptions,NULL);
    //XtAddCallback(nhelp,XtNcallback,on_nhelp,NULL);
}

void TGUI::CreateStdOut(void)
{
    stdout_widget=XtVaCreateManagedWidget("stdout", // name
					  coreWidgetClass, // class
					  main_window, // parent
					  XtNwidth,gf_width,
					  XtNheight,OUTPUTWINDOWHEIGHT, // height of the widget
					  NULL);
}

void TGUI::ClearStdOut(void)
{
    XFillRectangle(display,
		   output_offscreen,
		   gc_black,
		   0x0,
		   0x0,
		   gf_width,
		   OUTPUTWINDOWHEIGHT);
    UpdateStdOut();
}

void TGUI::PrintMessage(char *text)
{
    ClearStdOut();

    XDrawString(display,
		output_offscreen,
		gc_red,
		0x2,
		0xC,
		text,
		strlen(text));
    UpdateStdOut();
}

void TGUI::PrintStatus(char *text)
{
    int x,y;

    x=gf_width/0x2-strlen(text)*0x7/0x2;
    y=gf_height/0x2;

    if (*text=='\n') {
	y+=0x10;
	text++;
    }

    XDrawString(display,
		game_offscreen,
		gc_red,
		x,
		y,
		text,
		strlen(text));
    Redraw();
}

void TGUI::ResetMenu(void)
{
    XtVaSetValues(npause,XtNlabel,"Pause",NULL);
}

void TGUI::CreateIndicators(void)
{
    armor_win.Load("models/armor_window.xpm");
    health_win.Load("models/health_window.xpm");
    death_win.Load("models/death_window.xpm");
    RedrawIndicators();
}

void TGUI::RedrawIndicators(void)
{
    armor_win.Draw(0x0,gf_height-MODEL_HEIGHT);
    health_win.Draw(MODEL_WIDTH+MODEL_HWIDTH,gf_height-MODEL_HEIGHT);
    death_win.Draw(MODEL_WIDTH*0x3,gf_height-MODEL_HEIGHT);
}

void TGUI::SetArmorIndicator(int val)
{
    char *str;

    if (val<0x0||val>99)
	return;

    str=(char *)malloc(0x3); // two digits+NULL
    sprintf(str,"%d",val);
    XDrawString(display,
		game_offscreen,
		gc_red,
		0xa, // tune it!!!
		gf_height-MODEL_HHEIGHT+0x5, // tune it!!!
		str,
		strlen(str));
}

void TGUI::SetHealthIndicator(int val)
{
    char *str;

    if (val<0x0||val>99)
	return;

    str=(char *)malloc(0x3); // two digits+NULL
    sprintf(str,"%d",val);
    XDrawString(display,
		game_offscreen,
		gc_red,
		MODEL_WIDTH+MODEL_HWIDTH+0xa, // tune it!!!
		gf_height-MODEL_HHEIGHT+0x5, // tune it!!!
		str,
		strlen(str));
}

void TGUI::SetDeathIndicator(int val)
{
    char *str;

    if (val<0x0||val>99)
	return;

    str=(char *)malloc(0x3); // two digits+NULL
    sprintf(str,"%d",val);
    XDrawString(display,
		game_offscreen,
		gc_red,
		MODEL_WIDTH*0x3+0xa, // tune it!!!
		gf_height-MODEL_HHEIGHT+0x5, // tune it!!!
		str,
		strlen(str));
}

void TGUI::UpdateStdOut(void)
{
    XCopyArea(display, // X connection
	      output_offscreen, // source
	      msgoutput, // destination
	      gc_black, // which GC to use
	      0x0, // source x
	      0x0, // source y
	      gf_width, // source width
	      OUTPUTWINDOWHEIGHT, // source height
	      0x0, // destination x
	      0x0 // destination y
	);
}

void TGUI::CreateOptionsBox(void)
{
}

void TGUI::Redraw(void)
{
    /*=Copy offscreen buffer to the game field=*/
    XCopyArea(display, // X connection
	      game_offscreen, // source
	      game_field, // destination
	      gc_normal, // which GC to use
	      0x0, // source x
	      0x0, // source y
	      gf_width, // source width
	      gf_height, // source height
	      0x0, // destination x
	      0x0 // destination y
	);
}

void TGUI::Clear(void)
{
    /*=Clear game field=*/
    XFillRectangle(display, // X connection
		   game_offscreen, // window (drawable)
		   gc_black, // which GC
		   0x0, // x-coordinate
		   0x0, // y-coordinate
		   gf_width, // width
		   gf_height // height
	);
}

void TGUI::ClearRect(int x,int y,int w,int h)
{
    XFillRectangle(display,
		   game_offscreen,
		   gc_black,
		   x,
		   y,
		   w,
		   h);
}

void TGUI::Loop(void)
{
    /*=Handle registered events=*/
    XtAppMainLoop(app_context);
}

void TGUI::SetGameControls(GameControls *p_gamecontrols)
{
    /*=Make local copy of user-defined game controls=*/
    memcpy(&gamecontrols,p_gamecontrols,sizeof(GameControls));
}

void TGUI::SetDefaultGameControls(void)
{
    // no code
}

/*=PRIVATE=*/
void TGUI::InitOffscreen(void)
{
    /*=Create offscreen buffer=*/
    game_offscreen=XCreatePixmap(display, // X connection
				 root_window, // window by which the screen will be determinated
				 gf_width, // width of the offscreen (the same as gf)
				 gf_height, // height of the offscreen (the same as gf)
				 depth // depth of the offscreen buffer
	);

    output_offscreen=XCreatePixmap(display,
				   root_window,
				   gf_width,
				   OUTPUTWINDOWHEIGHT,
				   depth);
}

void TGUI::InitGC(void)
{
    Colormap clmap;
    XColor cl;

    clmap=DefaultColormapOfScreen(screen);
    cl.pixel=WhitePixelOfScreen(screen);
    XQueryColor(display,clmap,&cl);

    /*=Create GC for black filling=*/
    gc_black=XCreateGC(display,root_window,0x0,NULL);

    /*=Create GC for all standard graphic operations=*/
    gc_normal=XCreateGC(display,root_window,0x0,NULL);

    gc_red=XCreateGC(display,root_window,0x0,NULL);

    /*=Fill in GCs=*/
    XSetForeground(display,gc_black,BlackPixelOfScreen(screen)); // black
    XSetBackground(display,gc_black,BlackPixelOfScreen(screen)); // one

    XSetForeground(display,gc_normal,BlackPixelOfScreen(screen)); // and
    XSetBackground(display,gc_normal,WhitePixelOfScreen(screen)); // normal one

    XSetForeground(display,gc_red,cl.red);
    XSetBackground(display,gc_red,BlackPixelOfScreen(screen));
}

void TGUI::InstallEventHandlers(void)
{
    /*=Register exposure event handler=*/
    XtAddEventHandler(drawing_area,
		      ExposureMask,
		      FALSE,
		      ExposureEventHandler,
		      NULL
	);

    /*=Register key press event handler=*/
    XtAddEventHandler(drawing_area,
		      KeyPressMask,
		      FALSE,
		      KeypressEventHandler,
		      NULL
	);
}

void TGUI::UninstallEventHandlers(void)
{
    XtRemoveEventHandler(drawing_area,
			 KeyPressMask,
			 FALSE,
			 KeypressEventHandler,
			 NULL);

    XtRemoveEventHandler(drawing_area,
			 ExposureMask,
			 FALSE,
			 ExposureEventHandler,
			 NULL);
}

void TGUI::Terminate(void)
{
    XtDestroyApplicationContext(app_context);
    exit(0x0);
}
