
#include "tgincludes.h"

/*=EXTERNALS=*/
extern TGUI tgui;
extern Tank *main_tank;

/*=Map object class implementation=*/
MapObject::MapObject(void)
{
    mo_x=mo_y=0x0;
    mo_cx=mo_cy=0x0;
    mo_speed=0x0;
    mo_armor=0x0;
    mo_health=0x0;
    mo_damage=0x0;
    mo_do_clear=0x1;
}

MapObject::~MapObject()
{
    if (mo_do_clear) {
	tgui.ClearRect(mo_x,mo_y,MODEL_WIDTH,MODEL_HEIGHT);
	tgui.Redraw();
    }
}

int MapObject::GetX(void)
{
    return mo_x;
}

int MapObject::GetY(void)
{
    return mo_y;
}

void MapObject::Draw(int x,int y)
{

}

void MapObject::SetSpeed(int speed)
{
    mo_speed=speed;
}

int MapObject::GetSpeed(void)
{
    return mo_speed;
}

void MapObject::SetCXCY(int cx,int cy)
{
    mo_cx=cx;
    mo_cy=cy;
}

int MapObject::GetCX(void)
{
    return mo_cx;
}

int MapObject::GetCY(void)
{
    return mo_cy;
}

void MapObject::Move(void)
{
    // no code
}

void MapObject::OnCollisionHandler(MapObject *with)
{

}

void MapObject::OnDamage(void *who,int damage)
{

}

void MapObject::SetArmor(int armor)
{
    mo_armor=armor;
}

int MapObject::GetArmor(void)
{
    return mo_armor;
}

void MapObject::SetDamage(int damage)
{
    mo_damage=damage;
}

int MapObject::GetDamage(void)
{
    return mo_damage;
}

/*=Tank class implementation=*/
Tank::Tank(int x,int y,int load_models):MapObject()
{
    /*=Set correct direction=*/
    current_model=&tank_up;
    mo_cx=0x0;
    mo_cy=0xffffffff; // up
    nr_bullets=0x0;
    nr_frags=0x0;

    if (!load_models)
	return;

    tank_left.Load("models/tank_left.xpm");
    tank_right.Load("models/tank_right.xpm");
    tank_up.Load("models/tank_up.xpm");
    tank_down.Load("models/tank_down.xpm");
    tank_explosion.Load("models/explosion.xpm");

    mo_x=x;
    mo_y=y;
    Draw(x,y);
}

Tank::~Tank()
{
}

void Tank::Draw(int x,int y)
{
    if (TGCollision(0x1,this,x,y))
	return;

    tgui.ClearRect(mo_x,mo_y,MODEL_WIDTH,MODEL_HEIGHT);
    current_model->Draw(x,y);
    mo_x=x;
    mo_y=y;
}

void Tank::MoveLeft(void)
{
    current_model=&tank_left;

    mo_cx=0xffffffff;
    mo_cy=0x0;

    Move();
}

void Tank::MoveRight(void)
{
    current_model=&tank_right;

    mo_cx=0x1;
    mo_cy=0x0;

    Move();
}

void Tank::MoveUp(void)
{
    current_model=&tank_up;

    mo_cx=0x0;
    mo_cy=0xffffffff;

    Move();
}

void Tank::MoveDown(void)
{
    current_model=&tank_down;

    mo_cx=0x0;
    mo_cy=0x1;

    Move();
}

void Tank::Move(void)
{
    Draw(mo_x+mo_cx*mo_speed,
	 mo_y+mo_cy*mo_speed);
}

void Tank::GetGunXY(int *rx,int *ry)
{
    if (current_model==&tank_left) {
	*rx=mo_x-MODEL_WIDTH;
	*ry=mo_y;
	return;
    }

    if (current_model==&tank_right) {
	*rx=mo_x+MODEL_WIDTH;
	*ry=mo_y;
	return;
    }

    if (current_model==&tank_up) {
	*rx=mo_x;
	*ry=mo_y-MODEL_HEIGHT;
	return;
    }

    if (current_model==&tank_down) {
	*rx=mo_x;
	*ry=mo_y+MODEL_HEIGHT;
	return;
    }
}

void Tank::OnDamage(void *who,int damage)
{
    // no code
}

void Tank::OnCollisionHandler(MapObject *with)
{

}

void Tank::IncNrBullets(void)
{
    nr_bullets++;
}

void Tank::DecNrBullets(void)
{
    if (nr_bullets>0x0)
	nr_bullets--;
}

int Tank::GetNrBullets(void)
{
    return nr_bullets;
}

void Tank::ResetNrFrags(void)
{
    nr_frags=0x0;
}

void Tank::IncNrFrags(void)
{
    nr_frags++;
    tgui.RedrawIndicators();
    tgui.SetArmorIndicator(mo_armor);
    tgui.SetHealthIndicator(mo_health);
    tgui.SetDeathIndicator(nr_frags);
}

int Tank::GetNrFrags(void)
{
    return nr_frags;
}

/*=Main tank class implementation=*/
MainTank::MainTank(int x,int y):Tank(x,y,0x0)
{
    tank_left.Load("models/main_tank_left.xpm");
    tank_right.Load("models/main_tank_right.xpm");
    tank_up.Load("models/main_tank_up.xpm");
    tank_down.Load("models/main_tank_down.xpm");

    /*=Override some base class defaults=*/
    mo_speed=DEFAULT_TANK_SPEED;
    mo_armor=DEFAULT_TANK_ARMOR;
    mo_health=DEFAULT_TANK_HEALTH;

    /*=The indicators are used only by the main tank=*/
    tgui.RedrawIndicators();
    tgui.SetArmorIndicator(mo_armor);
    tgui.SetHealthIndicator(mo_health);
    tgui.SetDeathIndicator(nr_frags);

    mo_x=x;
    mo_y=y;
    Draw(x,y);
}

/*=====================================================*/
/* OnDamage()--This function handles OnDamage event    */
/*             posted to the main game (non-bot) tank. */
/*=====================================================*/
void MainTank::OnDamage(void *who,int damage)
{
    if (mo_armor>0x0)
	mo_armor-=damage;
    else
	mo_health-=damage;

    /*=Set indicators=*/
    tgui.RedrawIndicators();
    tgui.SetArmorIndicator(mo_armor);
    tgui.SetHealthIndicator(mo_health);
    tgui.SetDeathIndicator(nr_frags);
    
    if (mo_health<=0x0)
	TGDeleteGameObject(this);
}

/*=Tank-bot class implementation=*/
TankBot::TankBot(int x,int y):Tank(x,y)
{
    mo_speed=DEFAULT_TANKBOT_SPEED; // bot's speed
    mo_armor=DEFAULT_TANK_ARMOR;
    mo_health=DEFAULT_TANK_HEALTH;
}

/*============================================================*/
/* OnDamage()--Overrides parent's (Tank) function and handles */
/*             damage event for Bot-tanks.                    */
/*============================================================*/
void TankBot::OnDamage(void *who,int damage)
{
    int i;
    Tank *tank; // the tank which has attacked me

    /*=Set armor of the tank after shoot=*/
    mo_armor-=damage;

    /*=Is the tank killed?=*/
    if (mo_armor<=0x0) { // yes

	/*=Draw the explosion inplace the tank=*/
	for (i=0x0;i<0x5;i++) {
	    tank_explosion.Draw(mo_x,mo_y);
	    tgui.Redraw();
	}
	tank=((Bullet *)who)->GetTank(); // get the tank using the bullet
	tank->IncNrFrags(); // increment enemy's number of frags
	TGDeleteBot(this);
	TGAddBots(); // add some new bots to the game
    }
}

void TankBot::OnCollisionHandler(MapObject *with)
{
    if (!TGCollision(0x0,this,mo_x-mo_speed,mo_y)) {
	MoveLeft();
	return;
    }

    if (!TGCollision(0x0,this,mo_x+mo_speed,mo_y)) {
	MoveRight();
	return;
    }

    if (!TGCollision(0x0,this,mo_x,mo_y-mo_speed)) {
	MoveUp();
	return;
    }

    if (!TGCollision(0x0,this,mo_x,mo_y+mo_speed)) {
	MoveDown();
	return;
    }
}

/*=Bullet class implementation=*/
Bullet::Bullet(Tank *t,int x,int y):MapObject()
{
    bullet_model.Load("models/bullet.xpm");
    tank=t; //  this tank created the bullet

    /*=Override parent class defaults=*/
    mo_speed=MODEL_HWIDTH;
    mo_damage=DEFAULT_BULLET_DAMAGE;
    mo_x=x;
    mo_y=y;

    if (TGCollision(0x1,this,x,y))
	mo_do_clear=0x0;
    else
	Draw(x,y);
}

Bullet::~Bullet()
{

}

void Bullet::Draw(int x,int y)
{
    if (TGCollision(0x1,this,x,y))
	return;

    tgui.ClearRect(mo_x,mo_y,MODEL_WIDTH,MODEL_WIDTH);
    bullet_model.Draw(x,y);
    mo_x=x;
    mo_y=y;
}

void Bullet::Move(void)
{
    Draw(mo_x+mo_cx*mo_speed,
	 mo_y+mo_cy*mo_speed);
}

void Bullet::OnCollisionHandler(MapObject *with)
{
    if (with)
	with->OnDamage(this,mo_damage);

    TGDeleteGameBullet(this);
}

void Bullet::SetTank(Tank *t)
{
    tank=t;
}

Tank *Bullet::GetTank(void)
{
    return tank;
}

/*=Tree class implementation=*/
Tree::Tree(int x,int y):MapObject()
{
    tree_model.Load("models/tree.xpm");
    smoke_model.Load("models/smoke.xpm");
    mo_health=DEFAULT_TREE_HEALTH;

    mo_x=x;
    mo_y=y;
    Draw(x,y);
}

void Tree::Draw(int x,int y)
{
    tgui.ClearRect(mo_x,mo_y,MODEL_WIDTH,MODEL_WIDTH);
    tree_model.Draw(x,y);
    mo_x=x;
    mo_y=y;
}

void Tree::OnDamage(void *who,int damage)
{
    int i;

    mo_health-=damage;

    if (mo_health<=0x0) {
	for (i=0x0;i<0xa;i++)
	    smoke_model.Draw(mo_x,mo_y);

	TGDeleteGameObject(this);
    }
}

/*=Wall class implementation=*/
Wall::Wall(int x,int y):MapObject()
{
    wall_model.Load("models/wall.xpm");
    smoke_model.Load("models/smoke.xpm");
    mo_health=DEFAULT_WALL_HEALTH;

    mo_x=x;
    mo_y=y;
    Draw(x,y);
}

void Wall::Draw(int x,int y)
{
    tgui.ClearRect(mo_x,mo_y,MODEL_WIDTH,MODEL_WIDTH);
    wall_model.Draw(x,y);
    mo_x=x;
    mo_y=y;
}

void Wall::OnDamage(void *who,int damage)
{
    int i;

    mo_health-=damage;

    if (mo_health<=0x0) {
	for (i=0x0;i<0xa;i++)
	    smoke_model.Draw(mo_x,mo_y);

	TGDeleteGameObject(this);
    }
}

/*=Iron wall class implementation=*/
IronWall::IronWall(int x,int y):MapObject()
{
    ironwall_model.Load("models/iron_wall.xpm");
    smoke_model.Load("models/smoke.xpm");
    mo_health=DEFAULT_IRONWALL_HEALTH;

    mo_x=x;
    mo_y=y;
    Draw(x,y);
}

void IronWall::Draw(int x,int y)
{
    tgui.ClearRect(mo_x,mo_y,MODEL_WIDTH,MODEL_WIDTH);
    ironwall_model.Draw(x,y);
    mo_x=x;
    mo_y=y;
}

void IronWall::OnDamage(void *who,int damage)
{
    int i;

    mo_health-=damage;

    if (mo_health<=0x0) {
	for (i=0x0;i<0xa;i++)
	    smoke_model.Draw(mo_x,mo_y);

	TGDeleteGameObject(this);
    }
}
