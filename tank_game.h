
#ifndef _TANK__TANK_GAME_H_
#define _TANK__TANK_GAME_H_

/*=INCLUDES=*/
#include "tgincludes.h"

/*=TYPES=*/
typedef struct _gameobject_t {
    MapObject *mapobject;
    struct _gameobject_t *next_go;
} GameObject;

typedef struct _gamebullet_t {
    Bullet *bullet;
    struct _gamebullet_t *next_gb;
} GameBullet;

/*=FUNCTION PROTOTYPES=*/
void TGNewGame(void);
void TGResetGame(void);
void TGInitialize(void);
void TGDestroy(void);
void TGAddGameObject(MapObject *new_mo);
void TGDeleteGameObject(MapObject *mo);
void TGCreateMainTank(int x,int y);
void TGAddGameBullet(Tank *tank);
void TGDeleteGameBullet(Bullet *bullet);
int TGCollision(int do_call_handler,MapObject *mo,int x,int y);
void TGMakeDebugDump(void);
void TGTerminateGame(void);
void TGDeleteAllBullets(void);
void TGDeleteAllGameObjects(void);
void TGLoadMap(const char *fname);
char *TGReadIniString(const char *fname,const char *section,const char *key);
void TGBotsSurrender(void);
void TGTheEnd(int winner);

/*=HANDLERS=*/
void on_fire(void *p);
void on_left(void *p);
void on_right(void *p);
void on_up(void *p);
void on_down(void *p);
void on_escape(void *p);

#endif
