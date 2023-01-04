
#ifndef _TANK__BOT_H_
#define _TANK__BOT_H_

#define MAX_ALIVE_BOTS 0x3

typedef struct _gamebot_t {
    TankBot *tankbot;
    struct _gamebot_t *next_bot;
} GameBot;

void TGBotInitialize(void);
void TGSetBotCoordinates(int x1,int y1,int x2,int y2,int x3,int y3);
void TGSetMaxBots(int n);
int TGGetNumberOfBots(void);
void TGAddBot(int x,int y);
void TGAddBots(void);
void TGDeleteBot(TankBot *bot);
void TGDestroyBot(void);

#endif
