// ===============================================
// @file   pong_glfw.cpp
// @author kmurphy
// @practical pong
// @brief  GLFW (OpenGL) specific code for Pong implementation
// ===============================================

// ===============================================
// Headers
// ===============================================
#include "pong.h"

#ifndef PSP

// helper function taken from GLUI library
// used to render a string 
void _glutBitmapString(void *font, char *s) {
    char *p = s;

    while(*p != '\0') {
        glutBitmapCharacter(font, *p);
        p++;
    }
}


int init(void) {

	static int argc=1;
	static char *argv[1];
	argv[0] = strdup("pong");
	glutInit(&argc, argv);
	
	glfwInit(); 
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	if( !glfwOpenWindow(WINDOW_WIDTH,WINDOW_HEIGHT, 0,0,0,0,0,0, GLFW_WINDOW) ) { 
		glfwTerminate(); 
		return 1; 
	} 
	glfwSetWindowPos(100,100);
	glfwSetWindowTitle(WINDOW_NAME);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);
	
	// frame information
    start_time = this_time = glfwGetTime();

	return 0;
}


void deinit(void) {
	glfwTerminate(); 
}


void get_input(void) {

	game_quit = glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED);
	if (game_quit) return;

    if (glfwGetKey(GLFW_KEY_UP)) {
        paddle_speed = 1;
    } else if (glfwGetKey(GLFW_KEY_DOWN)) {
        paddle_speed = -1;
    } else {
        paddle_speed = 0.0;
    }		
	if (glfwGetKey('A')) auto_mode = !auto_mode;

}


int draw_scene(void) {
	
	glClear(GL_COLOR_BUFFER_BIT); 
	
	draw_walls();

	// paddle
    glPushMatrix();
        glTranslatef(paddle_x_pos-PADDLE_WIDTH/2, paddle_y_pos-paddle_length/2, 0);
        draw_paddle();
    glPopMatrix();

	// ball
    glPushMatrix();
        glTranslatef(ball_x_pos - BALL_SIZE/2, ball_y_pos  - BALL_SIZE/2, 0);
        draw_ball();
    glPopMatrix();

	draw_status();								// status message

	glfwSwapBuffers(); 

	// update frame timer
	old_time = this_time;
	this_time = glfwGetTime();
	dt = this_time - old_time;
	
	return 0;	// success
}


void draw_walls(void) {
	
    glColor3f(1,0,0);
    glBegin(GL_QUADS);
    	// top wall
   		glVertex3f(0, WINDOW_HEIGHT, 0);
		glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
		glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT-BORDER_SIZE, 0);
		glVertex3f(0, WINDOW_HEIGHT-BORDER_SIZE, 0);
		//left wall
		glVertex3f(0, WINDOW_HEIGHT-BORDER_SIZE, 0);
		glVertex3f(BORDER_SIZE, WINDOW_HEIGHT-BORDER_SIZE, 0);
		glVertex3f(BORDER_SIZE, MARGIN_SIZE+BORDER_SIZE, 0);
		glVertex3f(0, MARGIN_SIZE+BORDER_SIZE, 0);
		//lower wall
		glVertex3f(0, MARGIN_SIZE, 0);
		glVertex3f(WINDOW_WIDTH, MARGIN_SIZE, 0);
    	glVertex3f(WINDOW_WIDTH, MARGIN_SIZE+BORDER_SIZE, 0);
    	glVertex3f(0, MARGIN_SIZE+BORDER_SIZE, 0);
	glEnd();
}


void draw_ball(void)  {
    glColor3f(0,1,0);
    glBegin(GL_QUADS);
    	glVertex3f(0, 0, 0);
    	glVertex3f(0, BALL_SIZE, 0);
    	glVertex3f(BALL_SIZE, BALL_SIZE, 0);
    	glVertex3f(BALL_SIZE, 0, 0);
    glEnd();	
}


void draw_paddle(void)  {
    glColor3f(0,0,1);
    glBegin(GL_QUADS);
    	glVertex3f(0, 0, 0);
    	glVertex3f(0, paddle_length, 0);
    	glVertex3f(PADDLE_WIDTH, paddle_length, 0);
    	glVertex3f(PADDLE_WIDTH, 0, 0);
    glEnd();	
}

void draw_status(void)  {
	// buffer to hold output message
	static char mess[300];

	sprintf(mess, 
		"FPS = %4.0f        Score = %3d    Lives = %1d		Pad = %3.0f", 1.0/dt, score, lives, paddle_length);

	glRasterPos2i(100, 1);
    _glutBitmapString(GLUT_BITMAP_9_BY_15, mess);
}

#endif
