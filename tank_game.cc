
#include "tgincludes.h"

extern TGUI tgui;

#define BM_DELAY 0x20

/*=GLOBALS=*/
GameObject *first_go;
GameBullet *first_gb;
MainTank *main_tank;
int nr_game_objects;

/*=Bullet manager=*/
void bullet_manager(XtPointer client_data,XtIntervalId *timer)
{
    GameBullet *p_gb=first_gb;

    while (p_gb) {
	p_gb->bullet->Move();
	p_gb=p_gb->next_gb;
    }
    TGTimerComplete(client_data);
}

/*=FUNCTIONS=*/
void TGNewGame(void)
{
    tgui.Clear();
    tgui.ClearStdOut();
    tgui.PrintMessage("Loading...");
    tgui.RedrawIndicators();
    tgui.SetArmorIndicator(0x0);
    tgui.SetHealthIndicator(0x0);
    tgui.SetDeathIndicator(0x0);
    TGInitialize();
    tgui.Redraw();
    tgui.InstallEventHandlers();
    tgui.PrintMessage("New game.");
}

void TGResetGame(void)
{
    /*=Destroy everything=*/
    tgui.PrintMessage("Resetting the game...");
    TGDestroy(); // destroy previous game
    TGNewGame(); // and create new one
    tgui.PrintMessage("Done.");
}

void TGInitialize(void)
{
    first_go=NULL;
    first_gb=NULL;
    main_tank=NULL;
    nr_game_objects=0x0;
    TGInitializeTimer(); // initialize the timer
    TGAddTimer(bullet_manager,BM_DELAY); // start the bullet manager
    TGBotInitialize();
    TGLoadMap("maps/map00.tg");
}

void TGDestroy(void)
{
    tgui.UninstallEventHandlers();
    TGDestroyTimer();
    TGDeleteAllBullets();
    TGDestroyBot();
    TGDeleteAllGameObjects();
    tgui.ResetMenu();
}

void TGAddGameObject(MapObject *new_mo)
{
    GameObject *new_go;

    new_go=(GameObject *)malloc(sizeof(GameObject));
    new_go->mapobject=new_mo;
    new_go->next_go=first_go;
    first_go=new_go;
    nr_game_objects++;
}

void TGDeleteGameObject(MapObject *mo)
{
    GameObject *dead_go,*prev_go;

    if (nr_game_objects==0x0)
	return;

    dead_go=prev_go=first_go;

    if (mo==main_tank) {
	TGTheEnd(0x0);
	return; // return to tank's "OnDamage" handler
    }

    while (dead_go) {
	if (dead_go->mapobject==mo) {
	    if (dead_go==first_go)
		first_go=dead_go->next_go;
	    else
		while (prev_go) {
		    if (prev_go->next_go==dead_go) {
			prev_go->next_go=dead_go->next_go;
			break;
		    }
		    prev_go=prev_go->next_go;
		}
	    delete mo;
	    free(dead_go);
	    nr_game_objects--;
	    break; // stop the search
	}
	dead_go=dead_go->next_go;
    }
}

void TGCreateMainTank(int x,int y)
{
    main_tank=new MainTank(x,y); // create new tank
    TGAddGameObject(main_tank); // register it
}

void TGAddGameBullet(Tank *tank)
{
    GameBullet *new_gb;
    Bullet *new_bullet;
    int mx,my,cx,cy;

    /*=!!NOTE!! CHANGE THIS !!!NOTE!!!=*/
    if (tank->GetNrBullets()!=0x0)
	return;

    tank->IncNrBullets();
    tank->GetGunXY(&mx,&my);

    /*=Determinate the direction of the tank=*/
    cx=tank->GetCX();
    cy=tank->GetCY();

    new_bullet=new Bullet(tank,mx,my);

    new_bullet->SetCXCY(cx,cy);
    TGAddGameObject(new_bullet);

    new_gb=(GameBullet *)malloc(sizeof(GameBullet));
    new_gb->bullet=new_bullet;
    new_gb->next_gb=first_gb;
    first_gb=new_gb;
}

void TGDeleteGameBullet(Bullet *bullet)
{
    GameBullet *dead_gb,*prev_gb;
    Tank *tank;

    if (nr_game_objects==0x0)
	return;

    dead_gb=prev_gb=first_gb;

    while (dead_gb) {
	if (dead_gb->bullet==bullet) {
	    if (dead_gb==first_gb)
		first_gb=dead_gb->next_gb;
	    else
		while (prev_gb) {
		    if (prev_gb->next_gb==dead_gb) {
			prev_gb->next_gb=dead_gb->next_gb;
			break;
		    }
		    prev_gb=prev_gb->next_gb;
		}
	    /*=Decrement number of bullets the tank has=*/
	    tank=bullet->GetTank();
	    tank->DecNrBullets();

	    /*=Completely remove the bullet=*/
	    TGDeleteGameObject(bullet);
	    free(dead_gb);
	    return; // objects found, so stop the search
	}
	dead_gb=dead_gb->next_gb; // continue search
    }
}

int TGCollision(int do_call_handler,MapObject *mo,int x,int y)
{
    GameObject *p_go=first_go;
    int mx,my;

    while (p_go) {
	if (x<=0x0||x+MODEL_WIDTH>=tgui.gf_width||
	    y<=0x0||y+MODEL_HEIGHT*0x2>=tgui.gf_height) {
	    if (do_call_handler)
		mo->OnCollisionHandler(NULL);
	    return 0x1;
	}

	if (p_go->mapobject==mo) {
	    p_go=p_go->next_go;
	    continue;
	}

	mx=p_go->mapobject->GetX();
	my=p_go->mapobject->GetY();

	if (ABS(x-mx)<MODEL_WIDTH&&ABS(y-my)<MODEL_HEIGHT) {
	    if (do_call_handler)
		mo->OnCollisionHandler(p_go->mapobject);
	    return 0x1; // collision detected
	}

	p_go=p_go->next_go;
    }
    return 0x0;
}

void TGMakeDebugDump(void)
{
    char *str;

    str=(char *)malloc(sizeof(char)*0x100);
    sprintf(str,"Total number of game objects: %d",nr_game_objects);
    tgui.PrintMessage(str);
    free(str);
}

void TGTerminateGame(void)
{
    TGDestroy();
    tgui.Terminate();
}

void TGDeleteAllBullets(void)
{
    GameBullet *dead_gb=NULL,*p_gb=first_gb;

    if (nr_game_objects==0x0)
	return;

    while (p_gb) {
	TGDeleteGameObject(p_gb->bullet);
	dead_gb=p_gb;
	p_gb=p_gb->next_gb;
	free(dead_gb);
    }
    nr_game_objects=0x0;
}

void TGDeleteAllGameObjects(void)
{
    GameObject *dead_go=NULL,*p_go=first_go;

    if (nr_game_objects==0x0)
	return;

    while (p_go) {
	delete p_go->mapobject;
	dead_go=p_go;
	p_go=p_go->next_go;
	free(dead_go);
    }
}

void TGLoadMap(const char *fname)
{
    int nr_objs;
    char *res;
    char *req;
    int i;
    Tree *t;
    Wall *w;
    IronWall *iw;
    int x,y;
    int x1,x2,x3,y1,y2,y3;

    req=(char *)malloc(0x20); // pretty enough for request

    /*=At the first load the trees, walls, etc.=*/
    res=TGReadIniString(fname,"Trees","Amount");
    if (res==NULL)
	printf("WARNING: there are no trees defined in the map file!\n");
    else {
	nr_objs=atoi(res);
	free(res);

	for (i=0x0;i<nr_objs;i++) {
	    sprintf(req,"tree%d_x",i+0x1);
	    res=TGReadIniString(fname,"Trees",req);
	    x=atoi(res);
	    free(res);

	    sprintf(req,"tree%d_y",i+0x1);
	    res=TGReadIniString(fname,"Trees",req);
	    y=atoi(res);
	    free(res);

	    t=new Tree(x,y);
	    TGAddGameObject(t);
	}
    }

    res=TGReadIniString(fname,"Walls","Amount");
    if (res==NULL)
	printf("WARNING: there are no walls defined in the map file!\n");
    else {
	nr_objs=atoi(res);
	free(res);

	for (i=0x0;i<nr_objs;i++) {
	    sprintf(req,"wall%d_x",i+0x1);
	    res=TGReadIniString(fname,"Walls",req);
	    x=atoi(res);
	    free(res);

	    sprintf(req,"wall%d_y",i+0x1);
	    res=TGReadIniString(fname,"Walls",req);
	    y=atoi(res);
	    free(res);

	    w=new Wall(x,y);
	    TGAddGameObject(w);
	}
    }

    res=TGReadIniString(fname,"IronWalls","Amount");
    if (res==NULL)
	printf("WARNING: there are no iron walls defined in the map file!\n");
    else {
	nr_objs=atoi(res);
	free(res);

	for (i=0x0;i<nr_objs;i++) {
	    sprintf(req,"ironwall%d_x",i+0x1);
	    res=TGReadIniString(fname,"IronWalls",req);
	    x=atoi(res);
	    free(res);

	    sprintf(req,"ironwall%d_y",i+0x1);
	    res=TGReadIniString(fname,"IronWalls",req);
	    y=atoi(res);
	    free(res);

	    iw=new IronWall(x,y);
	    TGAddGameObject(iw);
	}
    }

    /*=Add bots=*/
    res=TGReadIniString(fname,"Bots","Amount");
    if (res==NULL)
	printf("WARNING: there are no bots defined in the map file!\n");
    else {
	nr_objs=atoi(res);
	TGSetMaxBots(nr_objs);
	free(res);

	res=TGReadIniString(fname,"Bots","x1");
	x1=atoi(res);
	free(res);

	res=TGReadIniString(fname,"Bots","x2");
	x2=atoi(res);
	free(res);

	res=TGReadIniString(fname,"Bots","x3");
	x3=atoi(res);
	free(res);

	res=TGReadIniString(fname,"Bots","y1");
	y1=atoi(res);
	free(res);

	res=TGReadIniString(fname,"Bots","y2");
	y2=atoi(res);
	free(res);

	res=TGReadIniString(fname,"Bots","y3");
	y3=atoi(res);
	free(res);

	TGSetBotCoordinates(x1,y1,x2,y2,x3,y3);
	TGAddBots();
    }

    /*=Add main tank=*/
    res=TGReadIniString(fname,"MainTank","x");
    if (res==NULL)
	printf("WARNING: there is no main tank defined in the map file!\n");
    else {
	x=atoi(res);
	free(res);

	res=TGReadIniString(fname,"MainTank","y");
	y=atoi(res);
	free(res);

	TGCreateMainTank(x,y);
    }
    free(req);
}

char *TGReadIniString(const char *fname,const char *section,const char *key)
{
    FILE *file;
    char *buffer,*ret_buffer=NULL;
    int found_section=0x0,found_key=0x0,off=0x0,len=0x0;
    struct stat statbuf;

    if (!fname)
	return NULL;

    if ((file=fopen(fname,"r"))!=NULL) {
	stat(fname,&statbuf);
	buffer=(char *)malloc(statbuf.st_size);
	fread(buffer,0x1,statbuf.st_size,file);
	while (!found_key&&off<statbuf.st_size) {
	    while ((buffer[off]=='\r'||buffer[off]=='\n'||buffer[off]==' '||buffer[off]=='\t')&&off<statbuf.st_size)
		off++;
		if (off>=statbuf.st_size)
		    break;
		if (buffer[off]=='[') {
		    off++;
		if (off>=statbuf.st_size)
		    break;
		if (off<statbuf.st_size-strlen(section)) {
		    if (!strncasecmp(section,&buffer[off],strlen(section))) {
			off+=strlen(section);
			if (off>=statbuf.st_size)
			    break;
			if (buffer[off]==']')
			    found_section=0x1;
			else
			    found_section=0x0;
			off++;
			if (off>=statbuf.st_size)
			    break;
		    }
		    else
			found_section=0x0;
		}
		else
		    found_section=0x0;
		}
		else if (found_section) {
		    if (off<statbuf.st_size-strlen(key)) {
			if (!strncasecmp(key,&buffer[off],strlen(key))) {
			    off+=strlen(key);
			    while ((buffer[off]==' '||buffer[off]=='\t')&&off<statbuf.st_size)
				off++;
			    if (off>=statbuf.st_size)
				break;
			    if (buffer[off]=='=') {
				off++;
				while ((buffer[off]==' '||buffer[off]=='\t')&&off<statbuf.st_size)
				    off++;
				if (off>=statbuf.st_size)
				    break;
				len=0x0;
				while (buffer[off+len]!='\r'&&buffer[off+len]!='\n'&&buffer[off+len]!=';'&&off+len<statbuf.st_size)
				    len++;
				ret_buffer=(char *)malloc(len+0x1);
				strncpy(ret_buffer,&buffer[off],len);
				ret_buffer[len]='\0';
				off+=len;
				found_key=0x1;
			    }
			}
		    }
		}
		while (buffer[off]!='\r'&&buffer[off]!='\n'&&off<statbuf.st_size)
		    off++;
	}
	free(buffer);
	fclose(file);
    }
    return ret_buffer;
}

void TGBotsSurrender(void)
{
    TGTheEnd(0x1); // all the bots are down
}

void TGTheEnd(int winner)
{
    char *s_msg;
    int nr_frags;

    TGDestroy();
    nr_frags=main_tank->GetNrFrags();
    tgui.Clear();
    tgui.Redraw();

    if (winner)
	tgui.PrintMessage("You are winner!");
    else
	tgui.PrintMessage("You was killed by enemy!");

    s_msg=(char *)malloc(sizeof(char)*0x100);
    sprintf(s_msg,"Total enemies: %d",TGGetNumberOfBots());
    tgui.PrintStatus(s_msg);

    sprintf(s_msg,"\nTotal frags: %d",main_tank->GetNrFrags());
    tgui.PrintStatus(s_msg);
    free(s_msg);
}

/*=GAME ACTION HANDLERS=*/
void on_fire(void *p)
{
    TGAddGameBullet(main_tank);
}

void on_left(void *p)
{
    main_tank->MoveLeft();
}

void on_right(void *p)
{
    main_tank->MoveRight();
}

void on_up(void *p)
{
    main_tank->MoveUp();
}

void on_down(void *p)
{
    main_tank->MoveDown();
}

void on_escape(void *p)
{
    TGMakeDebugDump();
}
