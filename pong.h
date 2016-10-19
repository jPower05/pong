// ===============================================
// @file   pong.h
// @author kmurphy
// @practical pong
// @brief  Headfile for PSP+GLFW(OpenGL) pong game
// ===============================================

#ifndef PONG_H
#define PONG_H

// uncomment next line for PSP executable
//#define PSP 1
 
// ===============================================
// Headers
// ===============================================
#ifdef PSP								// PSP specific headers
	#include <psptypes.h>
	#include <kernel.h>
	#include <psperror.h>		
	#include <displaysvc.h>
	#include <ctrlsvc.h>				// controller 
	#include <libgu.h>					// basic graphics
	#include <libgum.h>					// 3D "OpenGLish" graphics 

#else          						// Unix based system specific
	#include <GL/gl.h>          
	#include <GL/glu.h>         
	#include <GL/glut.h>  
	#include <GL/glfw.h> 
#endif

#include <stdio.h>      				// for sprintf()
#include <stdlib.h>     				// for atof() 
#include <string.h>     				// for strcpy()
#include <math.h>       				// for cos(), sin()

// ===============================================
// Global identifiers
// ===============================================

// window & scene
#ifdef PSP   							// psp data types (see psptypes.h)
	#define WINDOW_WIDTH    SCEGU_SCR_WIDTH
	#define WINDOW_HEIGHT   SCEGU_SCR_HEIGHT
	#define MARGIN_SIZE      20
	#define BORDER_SIZE      10
#else
	#define WINDOW_NAME     "Pong using GLFW"
	#define WINDOW_WIDTH    640
	#define WINDOW_HEIGHT   480
	#define MARGIN_SIZE      15
	#define BORDER_SIZE      18
#endif

// ball
#define BALL_SIZE        15
#ifdef PSP   					
	#define BALL_STEP    5  			// distance ball travels per time step
#else
	#define BALL_STEP    dt*600  	
#endif

// paddle
#define PADDLE_WIDTH     BORDER_SIZE

//CHANGED THE PADDLE_LENGTH TO A FLOAT VARIABLE FROM A CONST
//CREATED TWO CONST FOR A MIN AND MAX PADDLE SIZE

extern float paddle_length;
#define PADDLE_LENGTH_MAX 100
#define PADDLE_LENGTH_MIN 30






#ifdef PSP   		
	#define PADDLE_STEP  10    			// distance paddle travels per time step
#else
	#define PADDLE_STEP  dt*520
#endif

extern float ball_x_pos;
extern float ball_y_pos;
extern float ball_angle;				// angle of ball trajectory

extern float paddle_x_pos;
extern float paddle_y_pos;
extern float paddle_speed;

extern int game_quit;					// flag - true for user want to quit game
extern int auto_mode;					// flag - true for computer playing
extern int score;						// number bounces off paddle
extern int lives;						// number of balls left to play						

// frame information
extern double this_time, old_time, dt, start_time;

// ===============================================
// Function prototypes
// ===============================================

// platform generic functions
void start_life(void);					// restart game for new life
void update(void);						// update game entities

// function with platform specific implementations in pong_*.c
int init(void);							// initialise graphics system
void deinit(void);						// release resources

void draw_walls(void);					
void draw_ball(void);
void draw_paddle(void); 
int draw_scene(void);					// draw scene by calling draw_* functions
void draw_status(void);					// draw status message

void get_input(void);					// keyboard/mouse/pad input

#endif
