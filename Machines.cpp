#include<allegro.h>
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include "MyVector.h"
#include "Object.h"
#include "Listable.h"
#include "PlayGround.h"
#include "InputHandler.h"
#include "Mask.h"
using namespace std;


int main(int argc, char* argv[]){
	LinkedList list_of_movables,list_of_objects;
	bool pause = false;
	bool exit = false;
	bool stop = false;
	allegro_init();
	set_color_depth(32);
	install_timer();
	install_keyboard();
	install_mouse();
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,1280,700,0,0);
	set_alpha_blender();
	set_color_conversion(COLORCONV_TOTAL);
	clear_to_color(screen,BG_COL);
	#define WIDTH_PLAY SCREEN_W-20
	#define HEIGHT_PLAY SCREEN_H * 4 / 5
	#define WIDTH_BAR SCREEN_W-20
	#define HEIGHT_BAR SCREEN_H / 5 - 70
	BITMAP *play_ground_bmp = create_sub_bitmap(screen,10,10,WIDTH_PLAY,HEIGHT_PLAY);
	BITMAP *object_bar_bmp = create_sub_bitmap(screen,10,HEIGHT_PLAY + 20,WIDTH_BAR,HEIGHT_BAR);
	PlayGround *play_ground = new PlayGround(play_ground_bmp);
	ObjectBar *object_bar = new ObjectBar(object_bar_bmp);
	Container bowling_cont(object_bar->getHeight());
	Container tennis_cont(object_bar->getHeight());
	Container baseball_cont(object_bar->getHeight());
	Container basketball_cont(object_bar->getHeight());
	Container soccer_cont(object_bar->getHeight());
	Container star_cont(object_bar->getHeight());
	Container inclined_cont(object_bar->getHeight());
	Container slant_cont(object_bar->getHeight());
	InputHandler ih(WIDTH_BAR,HEIGHT_PLAY,HEIGHT_BAR,WIDTH_PLAY);
	for(int i = 0;i < 20;i++){
		BowlingBall *bowling = new BowlingBall();
		bowling_cont.addObject(bowling);

	}
	for(int i = 0;i < 20;i++){
			TennisBall *tennis = new TennisBall();
			tennis_cont.addObject(tennis);

	}
	for(int i = 0;i < 20;i++){
			BaseballBall *baseball = new BaseballBall();
			baseball_cont.addObject(baseball);

	}
	for(int i = 0;i < 20;i++){
		BasketballBall *basketball = new BasketballBall();
		basketball_cont.addObject(basketball);
	}
	for(int i = 0;i < 20;i++){
		SoccerBall *soccer = new SoccerBall();
		soccer_cont.addObject(soccer);
	}
	for(int i = 0;i < 50;i++){
		star_cont.addObject(new Star());
	}
	for(int i = 0;i < 50;i++){
		inclined_cont.addObject(new InclinedPlane());
	}
	for(int i = 0;i < 50;i++){
		slant_cont.addObject(new Slant());
	}
	object_bar->addObject(&tennis_cont);
	object_bar->addObject(&baseball_cont);
	object_bar->addObject(&bowling_cont);
	object_bar->addObject(&basketball_cont);
	object_bar->addObject(&soccer_cont);
	//object_bar->addObject(&star_cont);
	object_bar->addObject(&inclined_cont);
	object_bar->addObject(&slant_cont);
	object_bar->reDraw();
	//enable_hardware_cursor();
	while(!exit){
		clear_keybuf();
		if(!(ih.getObj()))
			show_mouse(screen);
		while(!stop && !exit){
			//acquire_bitmap(play_ground_bmp);
			if(keypressed())
				ih.processKeyPress(&exit,&stop,&pause);
			if(mouse_b){
				ih.processMouse(mouse_b,object_bar,play_ground);
				rest(100);
			}
			ih.drawCursor(object_bar,play_ground);
			if(!(ih.getObj()))
				show_mouse(screen);
			//release_bitmap(play_ground_bmp);
			for(int i = 0; i < 20;i++){
				rest(1);
			}
		}
		show_mouse(NULL);
		stop = false;
		clear_keybuf();
		while(!stop && !exit){
			acquire_bitmap(play_ground_bmp);
			if(keypressed())
				ih.processKeyPress(&exit,&stop,&pause);
			if(!pause)
				play_ground->step();
			release_bitmap(play_ground_bmp);
			rest(20);
		}
		play_ground->reset();
		stop = false;
	}
	return 0;
}
END_OF_MAIN();
