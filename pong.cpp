// ===============================================
// @file   pong.cpp
// @author jpower
// @practical pong
// @brief  Main code for PSP+GLFW(OpenGL) pong game 
// ===============================================


// ===============================================
// Headers
// ===============================================
#include "pong.h"
#include <stdio.h>    
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
using namespace std;

// ===============================================
// Global identifiers
// ===============================================





float paddle_x_pos;
float paddle_y_pos;
float paddle_speed;
float paddle_y_min, paddle_y_max;

//DECLARING THE VARIABLE PADDLE_LENGTH

float paddle_length;

int ballCount;							//A counter for the num of balls
Ball balls[MAX_BALL_COUNT];



int game_quit = 0;						// flag - true for user want to quit game
int score = 0;							// number bounces off paddle
int lives = 3;							// number of balls left to play						
int auto_mode = 1;						// flag - true for computer playing
int paddle_hits = 0;

// frame information
double this_time, old_time, dt, start_time;


    


void start_life() {
	ballCount = 1;			//initialise ball count

	
	
	// initial ball position and direction
	balls[ballCount - 1].x     = WINDOW_WIDTH/2;
    balls[ballCount - 1].y     = WINDOW_HEIGHT/2;
	balls[ballCount - 1].angle = 0.961;
	

	
	
	// initial paddle position
	paddle_x_pos    = WINDOW_WIDTH - PADDLE_WIDTH/2;
	paddle_y_pos    = WINDOW_HEIGHT/2;

	//SETTING THE INITIAL SIZE OF THE PADDLE_LENGTH TO 100

	paddle_length = PADDLE_LENGTH_MAX;

	paddle_hits = 0;  	//reset the value of hits per life
}



void onPaddleHit(int index,const float maxPos){
	score++;
	paddle_hits++;

	if (paddle_length <= PADDLE_LENGTH_MIN){
 		paddle_length = PADDLE_LENGTH_MIN;
 	}
 	else{
 		paddle_length = (PADDLE_LENGTH_MAX - (paddle_hits*5));
 	}

	balls[index].angle = M_PI - balls[index].angle;
	balls[index].x = maxPos;
}


//called when the ball misses the paddle
void onPaddleMiss(int index){
	if(ballCount == 1 && lives > 0){
		start_life();
		lives--;
	} else if(ballCount == 1 && lives <= 0){
		lives--;
	} else{
		destroyBall(index);
	}
				
	std::cout << "The ball count is: " << ballCount << std::endl;
}





void createBall(float x,float y, float angle){
	if(ballCount + 1 > MAX_BALL_COUNT) return;		
	ballCount++;
	
	balls[ballCount -1].x = x;
	balls[ballCount -1].y = y;
	balls[ballCount -1].angle = angle;
}

void destroyBall(int index){
	balls[index] = balls[ballCount - 1];
	ballCount--;
}




void update() {
	
	// automatically update paddle direction (if required)
	if (auto_mode) {
		if (paddle_y_pos-paddle_length/2>balls[0].y) {
			paddle_speed = -1;
		} else if (paddle_y_pos+paddle_length/2<balls[0].y) {
			paddle_speed = 1;
		}
	}

	// update paddle position
	paddle_y_pos += PADDLE_STEP*paddle_speed;
	
	// calculate limits for paddle movement 
	const float paddle_y_min = (paddle_length/2+MARGIN_SIZE+BORDER_SIZE);
	const float paddle_y_max = (WINDOW_HEIGHT-BORDER_SIZE-paddle_length/2);

	// ensure paddle stays within limits
	if (paddle_y_pos<paddle_y_min) {
		paddle_y_pos = paddle_y_min;
	} else if (paddle_y_pos>paddle_y_max) {
		paddle_y_pos = paddle_y_max;
	}
	
	for(int n = 0; n < ballCount;n++){

		// update ball position (centre)
		balls[n].x += BALL_STEP*cos(balls[n].angle);
		balls[n].y += BALL_STEP*sin(balls[n].angle);
		
		// calculate limits for ball movement 
		const float ball_y_min = BALL_SIZE + MARGIN_SIZE + BORDER_SIZE;
		const float ball_y_max = WINDOW_HEIGHT - BALL_SIZE - BORDER_SIZE;
		const float ball_x_min = BORDER_SIZE + BALL_SIZE;
		const float ball_x_max = WINDOW_WIDTH - PADDLE_WIDTH - BALL_SIZE;
		
	    	// check - ball hit top or bottom wall
	 	if ((balls[n].y <= ball_y_min  || balls[n].y >= ball_y_max )) {
	        	balls[n].angle = -balls[n].angle;
			if(balls[n].y <= ball_y_min)balls[n].y = ball_y_min + 2;
			else if(balls[n].y >= ball_y_min)balls[n].y = ball_y_max - 2;
	    	} 

		// check - ball hit left wall
		if  (balls[n].x <= ball_x_min) {
			balls[n].angle = M_PI - balls[n].angle;
			balls[n].x = ball_x_min + 2;
		}
		
		// check - ball hits paddle or misses
		if (balls[n].x >= ball_x_max) {
			if(fabs(balls[n].y-paddle_y_pos) <= (paddle_length+BALL_SIZE)/2) {
				onPaddleHit(n,WINDOW_WIDTH - PADDLE_WIDTH - BALL_SIZE - 2);
			}else{	
				std::cout << "Ball missed the paddle and should be destroyed " << std::endl;
				onPaddleMiss(n);
			}
	 	}

	}


	if(ballCount == 0){
		start_life();
		lives --;
	}
}

int main(void) {

	init();							// initial graphics/sound components
	start_life();					// reset game to starting state

	while (!game_quit && lives>0) {
		draw_scene();
		get_input();
		update();	
	}
	deinit();
	return 0;
}
