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

PowerUp powerUps[POWER_UP_MAX_COUNT];
int powerupCount;


int game_quit = 0;						// flag - true for user want to quit game
int score = 0;							// number bounces off paddle
int lives = 3;							// number of balls left to play						
int auto_mode = 1;						// flag - true for computer playing
int paddle_hits = 0;

int count_to_spawn;							

// frame information
double this_time, old_time, dt, start_time;


    


void start_life() {
	ballCount = 1;			//initialise ball count
	powerupCount = 0;

	count_to_spawn = 0;				//initialise counter when to spawn ball
	
	// initial ball position and direction
	balls[ballCount - 1].x     = WINDOW_WIDTH/2;
    balls[ballCount - 1].y     = WINDOW_HEIGHT/2;
	balls[ballCount - 1].angle = 0.961;
	

	srand (time(NULL));
	
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
	count_to_spawn++;

	if (paddle_length <= PADDLE_LENGTH_MIN){
 		paddle_length = PADDLE_LENGTH_MIN;
 	}
 	else{
 		//paddle_length = (PADDLE_LENGTH_MAX - (paddle_hits*5));
 	}

	balls[index].angle = M_PI - balls[index].angle;
	balls[index].x = maxPos;
	std::cout<< "Amount until spawn is " << count_to_spawn << std::endl;
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
	std::cout<< "Ball count = " << ballCount << std::endl;
	
	balls[ballCount -1].x = x;
	balls[ballCount -1].y = y;
	balls[ballCount -1].angle = angle;

	count_to_spawn = 0;
	std::cout<< "Spawned at " << x << " " << y << " " << angle << std:: endl;
}

void destroyBall(int index){
	balls[index] = balls[ballCount - 1];
	ballCount--;
	std::cout<< "destroyed so Ball count = " << ballCount << std::endl;
}

void createPowerup(float x, float y){
	

	if (powerupCount + 1 > POWER_UP_MAX_COUNT) return;
	
		powerupCount++;
		std::cout<< "PowerupCount = " << powerupCount << std::endl;

		powerUps[powerupCount - 1].x = x;
		powerUps[powerupCount - 1].y = y;
		int set_type = rand() % POWER_UP_TYPE_MAX_COUNT + 0;
		std::cout<< "Type value = " << set_type << std::endl;
		powerUps[powerupCount - 1].type = powers[set_type];
		std::cout<<"The value of powerup No " << powerupCount -1 << " is x = " << x << " y = " << y << std::endl;

}

void destroyPowerup(int index){
	powerUps[index]=powerUps[powerupCount - 1];
	powerupCount--;
	std::cout<< "destroyed so powerup count = " << powerupCount << std::endl;
}



void checkNeedForSpawn(){
	if (count_to_spawn < 2) return;
	if (count_to_spawn > 2) count_to_spawn = 0;
	else if(count_to_spawn == 2){
		//float xRand = rand() %  (WINDOW_WIDTH - (BORDER_SIZE * 2)) + (BORDER_SIZE * 2);
		float yRand = BORDER_SIZE * 2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(WINDOW_WIDTH - (BORDER_SIZE * 2)) + (BORDER_SIZE * 2)));
		//float yRand = rand() % (BORDER_SIZE * 2) + (WINDOW_HEIGHT - (BORDER_SIZE * 2));
		float xRand = WINDOW_HEIGHT - (BORDER_SIZE * 2) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(BORDER_SIZE * 2) + (WINDOW_HEIGHT - (BORDER_SIZE * 2))));
		std::cout<<"Im creating a powerup" << std::endl;			
		count_to_spawn = 0;
		createPowerup(xRand,yRand);
		
		
		
	}
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
	
	for(int ball_index = 0; ball_index < ballCount;ball_index++){
		checkNeedForSpawn();

		// update ball position (centre)
		balls[ball_index].x += BALL_STEP*cos(balls[ball_index].angle);
		balls[ball_index].y += BALL_STEP*sin(balls[ball_index].angle);
		
		// calculate limits for ball movement 
		const float ball_y_min = BALL_SIZE + MARGIN_SIZE + BORDER_SIZE;
		const float ball_y_max = WINDOW_HEIGHT - BALL_SIZE - BORDER_SIZE;
		const float ball_x_min = BORDER_SIZE + BALL_SIZE;
		const float ball_x_max = WINDOW_WIDTH - PADDLE_WIDTH - BALL_SIZE;
		
    	if ((balls[ball_index].y <= ball_y_min  || balls[ball_index].y >= ball_y_max )) {
        	balls[ball_index].angle = -balls[ball_index].angle;
			
			if(balls[ball_index].y <= ball_y_min){
				balls[ball_index].y = ball_y_min + 2;
			}
			else if(balls[ball_index].y >= ball_y_min){
				balls[ball_index].y = ball_y_max - 2;
			}
    	} 

		// check - ball hit left wall
		if  (balls[ball_index].x <= ball_x_min) {
			balls[ball_index].angle = M_PI - balls[ball_index].angle;
			balls[ball_index].x = ball_x_min + 2;
		}
		
		// check - ball hits paddle or misses
		if (balls[ball_index].x >= ball_x_max) {
			if(fabs(balls[ball_index].y-paddle_y_pos) <= (paddle_length+BALL_SIZE)/2) {
				onPaddleHit(ball_index,WINDOW_WIDTH - PADDLE_WIDTH - BALL_SIZE - 2);
			}else{	
				std::cout << "Swing and a miss. Destroy plz " << std::endl;
				onPaddleMiss(ball_index);
			}
	 	}

		//Check powerup collision
		for(int powerUp_index = 0; powerUp_index < powerupCount; powerUp_index++){
			bool isCollision = false; //Test collision for x vertices
			
			int ball_col_index; 
			int powerup_col_index;
			if (((balls[ball_index].x + BALL_SIZE >= powerUps[powerUp_index].x) && (powerUps[powerUp_index].x + POWER_UP_SIZE >= 					balls[ball_index].x)) && ((balls[ball_index].y + BALL_SIZE >= powerUps[powerUp_index].y) && 						(powerUps[powerUp_index].y + POWER_UP_SIZE >= balls[ball_index].y))){
				//There is a collision for x and y
				
				ball_col_index = ball_index;
				powerup_col_index = powerUp_index;
				isCollision = true;
				
			}	
			
			//Use a powerup as there has been a collision with one
			if (isCollision == true){
				std::cout<<"CHoosing a powerup" << std::endl;
				if (powerUps[powerup_col_index].type == "SPAWN_BALL")
				{
					
					float xRand = rand() % (BORDER_SIZE * 2) + (WINDOW_WIDTH - (BORDER_SIZE * 2));
					float yRand = rand() % (BORDER_SIZE * 2) + (WINDOW_HEIGHT - (BORDER_SIZE * 2));
					createBall(xRand, yRand, 0.961);
					destroyPowerup(powerup_col_index);
					std::cout<< "Spawn Ball" << std::endl;
				}
				else if (powerUps[powerup_col_index].type == "DESTROY_BALL"){
					destroyBall(ball_col_index);
					destroyPowerup(powerup_col_index);
					std::cout<< "Destroy Ball" << std::endl;
				}
				else if (powerUps[powerup_col_index].type == "MAX_PADDLE_LENGTH"){
					paddle_length = PADDLE_LENGTH_MAX;
					destroyPowerup(powerup_col_index);
					std::cout<< "Max Length" << std::endl;
				}
				else if (powerUps[powerup_col_index].type == "MAX_MIN_LENGTH"){
					paddle_length = PADDLE_LENGTH_MIN;
					destroyPowerup(powerup_col_index);
					std::cout<< "Min Length" << std::endl;
				}
				else if (powerUps[powerup_col_index].type == "MOVE_BALL"){
				
					float xBallRand = rand() % (BORDER_SIZE * 2) + (WINDOW_WIDTH - (BORDER_SIZE * 2));
					float yBallRand = rand() % (BORDER_SIZE * 2) + (WINDOW_HEIGHT - (BORDER_SIZE * 2));
					balls[ball_col_index].x = xBallRand;
					balls[ball_col_index].y = yBallRand;
					destroyPowerup(powerup_col_index);
					std::cout<< "Moving the Ball" << std::endl;
				}

				

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
