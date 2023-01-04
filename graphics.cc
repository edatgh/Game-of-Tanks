
/*=INCLUDES=*/
#include "tgincludes.h"

/*=EXTERNALS=*/
extern TGUI tgui;

/*=Model class implementation=*/
void Model::Load(char *filename)
{
    XpmAttributes xpm_attr;
    Pixmap mask;
    Display *display;
    unsigned int sn; // screen number

    display=tgui.display;
    sn=DefaultScreen(display);

    /*=Fill in xpm attributes=*/
    xpm_attr.valuemask=XpmCloseness|XpmReturnPixels|XpmColormap|XpmDepth;
    xpm_attr.closeness=0xffff;
    xpm_attr.colormap=DefaultColormap(display,sn);
    xpm_attr.depth=DefaultDepth(display,sn);

    /*=Read file data=*/
    XpmReadFileToPixmap(display,tgui.root_window,filename,&m_ID,&mask,&xpm_attr);
}

void Model::Draw(int x,int y)
{
    /*=Copy model's image to the offscreen buffer=*/
    XCopyArea(tgui.display,
	      m_ID,
	      tgui.game_offscreen,
	      tgui.gc_normal,
	      0x0,
	      0x0,
	      MODEL_WIDTH,
	      MODEL_HEIGHT,
	      x,
	      y
	);
    tgui.Redraw();
}
