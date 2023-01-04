
#include "tgincludes.h"

/*=EXTERNALS=*/
extern TGUI tgui;
extern int nr_game_objects;

/*=GLOBALS=*/
GameBot *first_bot;
int cx1,cx2,cx3,cy1,cy2,cy3;
int nr_bots;
int alive_bots;
int max_bots;

void bot_manager(XtPointer client_data,XtIntervalId *timer)
{
    GameBot *p_bot=first_bot;

    while (p_bot) {
	p_bot->tankbot->Move();
	TGAddGameBullet(p_bot->tankbot);
	p_bot=p_bot->next_bot;
    }
    TGTimerComplete(client_data);
}

void TGBotInitialize(void)
{
    first_bot=NULL;
    cx1=cx2=cx3=cy1=cy2=cy3=0x0;
    nr_bots=max_bots=alive_bots=0x0;
    TGAddTimer(bot_manager,0x20);
}

void TGSetBotCoordinates(int x1,int y1,int x2,int y2,int x3,int y3)
{
    cx1=x1;
    cy1=y1;

    cx2=x2;
    cy2=y2;

    cx3=x3;
    cy3=y3;
}

void TGSetMaxBots(int n)
{
    max_bots=n;
}

int TGGetNumberOfBots(void)
{
    return nr_bots;
}

void TGAddBot(int x,int y)
{
    TankBot *new_tankbot;
    GameBot *new_bot;

    if (nr_bots>=max_bots)
	return;

    new_tankbot=new TankBot(x,y);
    TGAddGameObject(new_tankbot);

    new_bot=(GameBot *)malloc(sizeof(GameBot));
    new_bot->tankbot=new_tankbot;
    new_bot->next_bot=first_bot;
    first_bot=new_bot;

    nr_bots++;
    alive_bots++;

    new_bot->tankbot->MoveDown();
}

void TGAddBots(void)
{
    if (nr_bots+(MAX_ALIVE_BOTS-alive_bots)<=max_bots) {
	switch (MAX_ALIVE_BOTS-alive_bots) {
	    case 0x3:
		TGAddBot(cx1,cy1);
	    case 0x2:
		TGAddBot(cx2,cy2);
	    case 0x1:
		TGAddBot(cx3,cy3);
		break;
	}
    }
    if (alive_bots<=0x0)
	TGBotsSurrender();
}

void TGDeleteBot(TankBot *bot)
{
    GameBot *dead_bot,*prev_bot;

    if (nr_game_objects==0x0)
	return;

    dead_bot=prev_bot=first_bot;

    while (dead_bot) {
	if (dead_bot->tankbot==bot) {
	    if (dead_bot==first_bot)
		first_bot=dead_bot->next_bot;
	    else
		while (prev_bot) {
		    if (prev_bot->next_bot==dead_bot) {
			prev_bot->next_bot=dead_bot->next_bot;
			break;
		    }
		    prev_bot=prev_bot->next_bot;
		}
	    TGDeleteGameObject(bot);
	    free(dead_bot);
	    alive_bots--;
	    return;
	}
	dead_bot=dead_bot->next_bot;
    }
}

/*================================================================*/
/* TGBotDestroy()--Removes all bot list items (bot descriptors).  */
/*================================================================*/
void TGDestroyBot(void)
{
    GameBot *dead_bot=NULL,*p_bot=first_bot;

    if (nr_game_objects==0x0)
	return;

    while (p_bot) {
	TGDeleteGameObject(p_bot->tankbot);
	dead_bot=p_bot;
	p_bot=p_bot->next_bot;
	free(dead_bot);
    }

    nr_bots=alive_bots=0x0;
}
