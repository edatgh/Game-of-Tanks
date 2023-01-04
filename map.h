
#ifndef _TANK__MAP_H_
#define _TANK__MAP_H_

/*=INCLUDES=*/
#include "tgincludes.h"

/*=CONSTATNTS=*/
#define MODEL_WIDTH 0x20
#define MODEL_HEIGHT 0x20
#define MODEL_HWIDTH 0x10
#define MODEL_HHEIGHT 0x10
#define DEFAULT_TANK_SPEED 0x8
#define DEFAULT_TANK_ARMOR 0x3
#define DEFAULT_TANK_HEALTH 0x3
#define DEFAULT_TANK_DAMAGE 0x0
#define DEFAULT_TANKBOT_SPEED 0x4
#define DEFAULT_BULLET_DAMAGE 0x1
#define DEFAULT_TREE_HEALTH 0x1
#define DEFAULT_WALL_HEALTH 0x3
#define DEFAULT_IRONWALL_HEALTH 0x20

/*=Map object class definition=*/
class MapObject {
public:
    MapObject(void);
    virtual ~MapObject();
    virtual int GetX(void);
    virtual int GetY(void);
    virtual void Draw(int x,int y);
    virtual void SetSpeed(int speed);
    virtual int GetSpeed(void);
    virtual void SetCXCY(int cx,int cy);
    virtual int GetCX(void);
    virtual int GetCY(void);
    virtual void Move(void);
    virtual void OnCollisionHandler(MapObject *with);
    virtual void OnDamage(void *who,int damage);
    virtual void SetArmor(int armor);
    virtual int GetArmor(void);
    virtual void SetDamage(int damage);
    virtual int GetDamage(void);
protected:
    int mo_x,mo_y;
    int mo_cx;
    int mo_cy;
    int mo_speed; // in pixels
    int mo_armor;
    int mo_health;
    int mo_damage;
    int mo_do_clear;
};

/*=Tank class definition=*/
class Tank:public MapObject {
public:
    Tank(int x=0x0,int y=0x0,int load_models=0x1);
    ~Tank();
    void Draw(int x,int y);
    void MoveLeft(void);
    void MoveRight(void);
    void MoveUp(void);
    void MoveDown(void);
    void Move(void);
    void GetGunXY(int *rx,int *ry);
    void OnDamage(void *who,int damage);
    void OnCollisionHandler(MapObject *with);
    void IncNrBullets(void);
    void DecNrBullets(void);
    int GetNrBullets(void);
    void ResetNrFrags(void);
    void IncNrFrags(void);
    int GetNrFrags(void);
protected:
    Model tank_left;
    Model tank_right;
    Model tank_up;
    Model tank_down;
    Model tank_explosion;
    Model *current_model;
    int nr_bullets;
    int nr_frags; // number of killed tanks
};

/*=Main tank class definition=*/
class MainTank:public Tank {
public:
    MainTank(int x,int y);
    void OnDamage(void *who,int damage);
private:
};

/*=Tank-bot class definition=*/
class TankBot:public Tank {
public:
    TankBot(int x,int y);
    void OnDamage(void *who,int damage);
    void OnCollisionHandler(MapObject *with);
private:
};

/*=Bullet class definition=*/
class Bullet:public MapObject {
public:
    Bullet(Tank *t,int x,int y);
    ~Bullet();
    void Draw(int x,int y);
    void Move(void);
    void OnCollisionHandler(MapObject *with);
    void SetTank(Tank *t);
    Tank *GetTank(void);
private:
    Model bullet_model;
    Tank *tank; // feedback with the tank which launched the bullet
};

/*=Tree class definition=*/
class Tree:public MapObject {
public:
    Tree(int x,int y);
    void Draw(int x,int y);
    void OnDamage(void *who,int damage);
private:
    Model tree_model;
    Model smoke_model;
};

/*=Wall class definition=*/
class Wall:public MapObject {
public:
    Wall(int x,int y);
    void Draw(int x,int y);
    void OnDamage(void *who,int damage);
private:
    Model wall_model;
    Model smoke_model;
};

/*=Iron wall class definition=*/
class IronWall:public MapObject {
public:
    IronWall(int x,int y);
    void Draw(int x,int y);
    void OnDamage(void *who,int damage);
private:
    Model ironwall_model;
    Model smoke_model;
};

#endif
