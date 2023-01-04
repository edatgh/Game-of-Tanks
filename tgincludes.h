
#ifndef _TANK__TGINCLUDES_H_
#define _TANK__TGINCLUDES_H_

/*===============================INCLUDES=================================*/

#define ABS(x) ((x)>=0x0?(x):-(x))

/*=X Toolkit includes=*/
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>

/*=X Window-specific includes=*/
#include <X11/keysym.h>

/*=Athena includes=*/
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>

/*=Other includes=*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <X11/xpm.h>
#include "graphics.h"
#include "TGUI.h"
#include "tgtimer.h"
#include "map.h"
#include "tank_game.h"
#include "bot.h"
/*========================================================================*/

#endif
