#include <nds.h>
//required library
#include <gl2d.h>
//libgl2d (for simple graphics)
#include <stdio.h>
//standard input output function

//C Standard General Utilities Library
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time functions */

/*
A simple game for the NDS
thanks to PolyMars for the inspiration

Author : Jonas Trebicki
*/


//Main Class for the game
class Game {
	
//the class for the projectile
public: class Projectile {
public:
	float posX;
	float posY;
	float size=12;
	float p_spd=0;
};

//the class for an enemy
public: class Enemy {
public:
	float X;
	float Y;
	float size;
	float spd=0;
};

//main method of the class
public: int main(void) {
	//random seed (based on time)
	srand (time(NULL));
	
	//Game vars
	int lives = 10;
	int score = 0;
	bool gameOver = true;
	
	//player variables (position, speed, ...)
	float posX = 115;
	float posY = 90;
	float spd = 0;
	float width=24;
	
	//touchPosition Object
	touchPosition touch;
	
	//helper variables for determining swiping direction
	float t_last=-1;
	float t_current=-1;
	
	//we will have a single Projectile
	Projectile proj;
	//It starts at the player position
	proj.posX=posX;
	proj.posY=posY;
	
	//We are creating an enemy array with a size of 4 (we will have 4 enemies at the same time)
	Enemy enemy[4];
	

	//this could probably be wrapped in a method... (init_enemy(enemy[i]);
	//going through all 4 enemie objects and assigning them an random size, starting point, height and speed
	for(int i=0; i < 4; i++) {
		enemy[i].size=(rand() % 24 + 8);
		if((rand() % 2 + 1)==1) {
		enemy[i].X = 0;
		enemy[i].spd=+(rand() % 3);
		} else {
		enemy[i].X = 256;
		enemy[i].spd=-(rand() % 3);
		}
		enemy[i].Y = (rand() % 192);

	}
	
	
	
	//Standard Commands needed to make the Screen Output Work (see libgl2d documentation)
	videoSetMode(MODE_5_3D);
	glScreen2D();
	
	//main game loop
	while(1) {
		
		//we are scanning the buttons to check if anything has been pressed
		scanKeys();
		//we begin drawing 2d graphics
		glBegin2D();
		
		
		//if the game is running (not gameOver)
		if(!gameOver) {
		
		
		//All the Graphics of this project work with this method from libgl2d : drawaing simple squares with an starting x and y and an ending x and y position (and a color)
		
		//The Player
		glBoxFilled(posX, posY, posX+width, posY+width, RGB15(86, 43, 107));
		
		//The Projectile
		glBoxFilled(proj.posX, proj.posY, proj.posX+proj.size, proj.posY+proj.size, RGB15(12, 12, 12));
		
		//All the enemies
		for(int i=0; i < 4; i++) {
			glBoxFilled(enemy[i].X, enemy[i].Y, enemy[i].X+enemy[i].size, enemy[i].Y+enemy[i].size, RGB15(160, 51, 61));
		}
		
		//Drawing the healthbar
		for(int i=0; i < lives; i++) {
			glBoxFilled(0+i*(16), 0, 0+i*(16)+12, 12, RGB15(51, 51, 51));
		}
		
		//The Score display can only display numbers up to 99; if you get this far you are probably bored already; we just end the game here (you could probably do something else)
		if(score > 99) {
			gameOver=true;
		}
		
		
		
		//We want to get the first and second digit so we can draw them
		int first = score/10;
		int second = score%10;
		
		//Char Variables for the digits
		char first_c='0';
		char second_c='0';
		
		
		//There is probably a more efficient way of doing this (I wrote this code a year ago...)
		//we get strings values for the numbers
		switch(first) {
			case 0:
				first_c='0';
			break;
			case 1:
				first_c='1';
			break;
			case 2:
				first_c='2';
			break;
			case 3:
				first_c='3';
			break;
			case 4:
			first_c='4';
			break;
			case 5:
			first_c='5';
			break;
			case 6:
			first_c='6';
			break;
			case 7:
			first_c='7';
			break;
			case 8:
			first_c='8';
			break;
			case 9:
			first_c='9';
			break;
		}
		
		switch(second) {
			case 0:
				second_c='0';
			break;
			case 1:
				second_c='1';
			break;
			case 2:
				second_c='2';
			break;
			case 3:
				second_c='3';
			break;
			case 4:
			second_c='4';
			break;
			case 5:
			second_c='5';
			break;
			case 6:
			second_c='6';
			break;
			case 7:
			second_c='7';
			break;
			case 8:
			second_c='8';
			break;
			case 9:
			second_c='9';
			break;
		}
		
		
		//we draw the score in the buttom left corner
		drawLetter(0,187,first_c);
		drawLetter(5,187,second_c);
		
		//we end drawing for this cycle
		glEnd2D();
		
		
		//Moving upwards with Buttons presses
		//if you press a you go up double as fast
		if (keysDown()==KEY_A) {
			spd = -2.6;
		}
		
		if (keysDown()==KEY_B) {
			spd = -1.3;
		}
		
		
		/*
		rawx and rawy are exactly 
		what their names imply - raw values read from the screen. 
		What makes them raw? 
		Well, they're not exactly user-friendly as they are not translated to pixel positions on the screen, 
		they're large and require further calculations to be used in pointing and clicking, 
		however they are read much quicker then other values, 
		thus are useful at for example implementing swiping and dragging with the stylus. 
		px and py values are calculated from raw values and refer to specific pixels on the screen. 
		The screen is 192 pixels high and 256 pixels wide, with the top left-hand corner marked as pixel 0,0 - 
		knowing this alone will help you use the px and py values successfuly. As for z1 and z2 values, they refer to the 
		panel resistance and are useful (from what I know) at reading the pressure, however it's not something 
		you'd normally use and thus we won't talk much about them.
		*/
		
		
		//if touch the screen
		int held = keysHeld();
		//last value gets written with the current one
		t_last = t_current;
		if(held & KEY_TOUCH) {
			
			//we read the touchPosition
			touchRead(&touch);

			//the current value gets the x value of the touch position
			t_current = touch.rawx;
			
			//If the current position is larger than the last one you swiped right
			if(t_current > t_last) {
				proj.p_spd=3;
			//otherwise you swiped left
			} else if (t_current < t_last) {
				proj.p_spd=-3;
			}
			//do nothing if you stay in place

			

		}
		
		
		//dont let the projectile out of the screen
		if (proj.posX > 256 || proj.posX < 0) {
			//put it pack to the player position
			proj.posX=posX;
		}
		
		//the projectile always stays at the same height as the player
		proj.posY=posY;
		
		
		//the projectile X position changes depending on the projectile speed
		proj.posX += proj.p_spd;
		
		
		
		//the player position changes by speed
		posY += spd;
		//the speed also gets constantly added to to simulate gravity
		spd += 0.0625;
		
		
		//if the Player leaves the screen
		if (posY > 192) {
			//we remove a live and put him up again and reset the "gravity (spd)
			lives -= 1;
			posY = 90;
			spd = 0;
		}
		
		//Enemy Code
		//Enemy Position changes depending on their speed
	
		for(int i=0; i < 4; i++) {
			enemy[i].X += enemy[i].spd;
		}
		
		//If an enemy leaves the screen, reset them (same code as the init code)
		for(int i=0; i < 4; i++) {
			if( (enemy[i].X < -16) || (enemy[i].X > 272) ) {
						enemy[i].size=(rand() % 24 + 8);
						if((rand() % 2 + 1)==1) {
						enemy[i].X = 0;
						enemy[i].spd=+(rand() % 3);
						} else {
						enemy[i].X = 256;
						enemy[i].spd=-(rand() % 3);
						}
						
						if (enemy[i].spd==0) {
							enemy[i].spd=2.5;
						}
						enemy[i].Y = (rand() % 192);
			}
		}
		
		//Dont let the player leave the screen upwards
		if (posY < -4) {
			posY = -4;
		}
		
		
		//Collisions with Enemies
		//posX und width are player variables
		
		
		for(int i=0; i < 4; i++) {
		if (posX < enemy[i].X + enemy[i].size &&
		   posX + width > enemy[i].X &&
		   posY < enemy[i].Y + enemy[i].size &&
		   posY + width > enemy[i].Y) {
			// Collision detected!
			
			//Remove a live
			lives -= 1;
			//if there are no more lives left, end the game
			if (lives <= 0) {
				gameOver=true;
			}
			
			//Again, the init code of the enemy (reset them after hitting the player)
						enemy[i].size=(rand() % 24 + 8);
						if((rand() % 2 + 1)==1) {
						enemy[i].X = 0;
						enemy[i].spd=+(rand() % 3);
						} else {
						enemy[i].X = 256;
						enemy[i].spd=-(rand() % 3);
						}
						
						if (enemy[i].spd==0) {
							enemy[i].spd=2.5;
						}
						enemy[i].Y = (rand() % 192);
		}
		}
		
		//collisions with the projectile
		for(int i=0; i < 4; i++) {
		if (proj.posX < enemy[i].X + enemy[i].size &&
		   proj.posX + proj.size > enemy[i].X &&
		   proj.posY < enemy[i].Y + enemy[i].size &&
		   proj.posY + proj.size > enemy[i].Y) {
			// collision detected!
			
			
			//The score gets increasted
			score += 1;
			
			//Enemy gets resetted again
						enemy[i].size=(rand() % 24 + 8);
						if((rand() % 2 + 1)==1) {
						enemy[i].X = 0;
						enemy[i].spd=+(rand() % 3);
						} else {
						enemy[i].X = 256;
						enemy[i].spd=-(rand() % 3);
						}
						
						if (enemy[i].spd==0) {
							enemy[i].spd=2.5;
						}
						enemy[i].Y = (rand() % 192);
		}
		}
		} else {
			//before the game/after the game ends: set the score to 0 again and reset the lives to 10
			score = 0;
			lives = 10;
			
			//draw a simple "a to start"
			drawLetter(40,120,'a');
			
			drawLetter(50,120,'t');
			drawLetter(55,120,'o');
			
			drawLetter(65,120,'s');
			drawLetter(70,120,'t');
			drawLetter(75,120,'a');
			drawLetter(80,120,'r');
			drawLetter(85,120,'t');
			
			//if a gets pressed start the game
			if (keysDown()==KEY_A) {
				gameOver=false;
			}
		}
		
		
		glFlush(0);
		swiWaitForVBlank();
		
	}
	
}

void drawLetter(int x, int y, char letter) {
	//Drawing letters with simple filled boxes
	switch(letter) {
		case 'g':
			
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x+2, y+3, x+2, y+3, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+4, x, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+4, x+1, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+4, x+2, y+4, RGB15(255, 255, 255));
		
			break;
		case 'a':
		
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+3, x, y+3, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+3, x+2, y+3, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+3, x, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+3, x+2, y+4, RGB15(255, 255, 255));
			
			break;
		case 'm':
		
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x+3, y, x+3, y, RGB15(255, 255, 255));
			
			glBoxFilled(x+4, y, x+4, y, RGB15(255, 255, 255));
			glBoxFilled(x+4, y+1, x+4, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+4, y+2, x+4, y+2, RGB15(255, 255, 255));
			
			break;
			case 'e':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+3, x, y+3, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+4, x, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+4, x+1, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+4, x+2, y+4, RGB15(255, 255, 255));

			break;
			
			case 'o':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			break;
			
			case 'v':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			break;
			
			case 'r':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			break;
			
			case 't':
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+1, x+1, y+1, RGB15(255, 255, 255));
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+3, x+1, y+3, RGB15(255, 255, 255));
			break;
			
			case 's':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x+2, y+3, x+2, y+3, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+4, x, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+4, x+1, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+4, x+2, y+4, RGB15(255, 255, 255));
			break;
			
			
			case '0':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			break;
			
			case '1':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			break;
			
			case '2':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+3, x, y+3, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+4, x, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+4, x+1, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+4, x+2, y+4, RGB15(255, 255, 255));
			break;
			
			case '3':
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+1, x+1, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+3, x+1, y+3, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+4, x+1, y+4, RGB15(255, 255, 255));
			
			glBoxFilled(x, y, x+1, y, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x+1, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+4, x+1, y+4, RGB15(255, 255, 255));
			break;
			
			case '4':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x+1, y+1, x+1, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			break;
			case '5':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x+2, y+3, x+2, y+3, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+4, x, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+4, x+1, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+4, x+2, y+4, RGB15(255, 255, 255));
			break;
			
			case '6':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			
			
			glBoxFilled(x, y+3, x, y+3, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+3, x+2, y+3, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+4, x, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+4, x+1, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+4, x+2, y+4, RGB15(255, 255, 255));
			break;
			
			case '7':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+1, x+1, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+3, x+1, y+3, RGB15(255, 255, 255));
			break;
			
			case '8':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			
			
			glBoxFilled(x, y+3, x, y+3, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+3, x+2, y+3, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+4, x, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+4, x+1, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+4, x+2, y+4, RGB15(255, 255, 255));
			break;
			
			case '9':
			glBoxFilled(x, y, x, y, RGB15(255, 255, 255));
			glBoxFilled(x+1, y, x+1, y, RGB15(255, 255, 255));
			glBoxFilled(x+2, y, x+2, y, RGB15(255, 255, 255));
			
			
			glBoxFilled(x, y+1, x, y+1, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+1, x+2, y+1, RGB15(255, 255, 255));
			
			glBoxFilled(x, y+2, x, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+1, y+2, x+1, y+2, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+2, x+2, y+2, RGB15(255, 255, 255));
			
			glBoxFilled(x+2, y+3, x+2, y+3, RGB15(255, 255, 255));
			
			//glBoxFilled(x, y+4, x, y+4, RGB15(255, 255, 255));
			//glBoxFilled(x+1, y+4, x+1, y+4, RGB15(255, 255, 255));
			glBoxFilled(x+2, y+4, x+2, y+4, RGB15(255, 255, 255));
			break;
	
	}
}
};
//main method
int main(void) {
	//create a game object
	Game game;
	//run the game objects main method
	game.main();
}
