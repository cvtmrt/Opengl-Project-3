/*********
   CTIS164 - Template Source Program
----------
STUDENT : CAVIT MERT ERGUL
SECTION : 2. SECTION
HOMEWORK: CTIS-164-222-Homework-3
----------
PROBLEMS:
----------
ADDITIONAL FEATURES: 1-) there are three different fruits and there are a knife and a spinning blade which chase fruits.
                     2-) if any fruits are cut, other fruits will be spawn.
                     3-) when mause left click option is clicked, knife style can be changed and position of knife can be determined based on where mause image is,
                     4-) when f1 is pressed, the game will stop or continue,
 *********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "vec.h"

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define ENEMY_SPEED 4
#define PLAYER_SPEED 1

#define D2R 0.0174532

 /* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, spacebar = false, scoreb = false;
int  winWidth, winHeight; // current Window width and height
int salise = 0;
int saniye = 0;
bool active = 0;
int order = 1;
int extime = 0;
int totaltime = 0;
int finishtime = 19;
int finish = 0;
int phase = 0;
int knifechange = 1;
bool hit = false;
bool knifehide = false;
bool clickm = false;

float A = 300, //amplitude
fq = 0.5,  //frequency
C = 0,   //horizontal phase shift
B = 0;   //vertical phase shift

float angle = 0;
bool pr = false;

typedef struct {
    float x, y;
} point_t;


typedef struct {
    vec_t pos;
    float angle;
    bool active;
    vec_t vel;
} fire_t;

typedef struct {
    vec_t center;
    float width;
    vec_t speed;
} target_t;

point_t mouse;



fire_t   fr[2] = { {0,-200},{0,200} };
int fire_rate = 0;
target_t target[3];








// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

//
// To display onto window using OpenGL commands
void pie_filled(float x, float y, float r, float start, float end) {
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    for (float angle = start; angle < end; angle += 10) {
        glVertex2f(r * cos(angle * D2R) + x, r * sin(angle * D2R) + y);
    }
    glVertex2f(r * cos(end * D2R) + x, r * sin(end * D2R) + y);
    glEnd();
}
void myCircle_filled(float x, float y, float r) {
    glBegin(GL_POLYGON);
    for (float angle = 0; angle < 360; angle += 10) {
        glVertex2f(r * cos(angle * D2R) + x, r * sin(angle * D2R) + y);
    }
    glEnd();
}

float f(float x) {
    return A * sin((fq * x + C) * D2R) + B;
}

void Vertex(vec_t P, vec_t Tr, double angle) {
    double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
    double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
    glVertex2d(xp, yp);
}
void background()
{
    glColor3f(102. / 255, 51 / 255., 0);
    glBegin(GL_QUADS);
    glVertex2f(-430, 400);
    glVertex2f(-400, 400);
    glVertex2f(-570, 250);
    glVertex2f(-600, 250);
    glEnd();

    glColor3f(102. / 255, 51 / 255., 0);
    glBegin(GL_QUADS);
    glVertex2f(-430, -400);
    glVertex2f(-400, -400);
    glVertex2f(-570, -250);
    glVertex2f(-600, -250);
    glEnd();

    glColor3f(102. / 255, 51 / 255., 0);
    glBegin(GL_QUADS);
    glVertex2f(430, 400);
    glVertex2f(400, 400);
    glVertex2f(570, 250);
    glVertex2f(600, 250);
    glEnd();

    glColor3f(102. / 255, 51 / 255., 0);
    glBegin(GL_QUADS);
    glVertex2f(430, -400);
    glVertex2f(400, -400);
    glVertex2f(570, -250);
    glVertex2f(600, -250);
    glEnd();


    glRectf(-575, 250, -600, -250);
    glRectf(575, 250, 600, -250);

    glRectf(300, 250, -300, 230);
    glRectf(300, -250, -300, -230);
    glRectf(300, 250, 280, -250);
    glRectf(-300, 250, -280, -250);


    circle(-100, 100, 40);

    glBegin(GL_TRIANGLES);
    glVertex2f(300, 250);
    glVertex2f(300, -250);
    glVertex2f(-300, -250);
    glEnd();

    glColor3f(153. / 255, 76 / 255., 0);
    circle(100, -100, 40);

}
void fruites(target_t* t)
{
    //orange
    switch (phase)
    {
    case 0: {
        glColor3f(1, 229 / 255., 119 / 255.);
        pie_filled(angle - t[0].center.x, f(angle) + t[0].center.y, 50, 180, 360);
        glColor3f(1, 153 / 255., 51 / 255.);
        pie_filled(angle - t[0].center.x, f(angle) + t[0].center.y, 42, 180, 360);
        glColor3f(1, 229 / 255., 119 / 255.);
        pie_filled(angle - t[0].center.x, f(angle) + t[0].center.y, 8, 180, 360);
        glBegin(GL_QUADS);
        glVertex2f(angle - t[0].center.x - 7, f(angle) + t[0].center.y - 3);
        glVertex2f(angle - t[0].center.x - 5, f(angle) + t[0].center.y - 7);
        glVertex2f(angle - t[0].center.x - 35, f(angle) + t[0].center.y - 31);
        glVertex2f(angle - t[0].center.x - 33, f(angle) + t[0].center.y - 35);
        glEnd();
        glBegin(GL_QUADS);
        glVertex2f(angle - t[0].center.x + 7, f(angle) + t[0].center.y - 3);
        glVertex2f(angle - t[0].center.x + 5, f(angle) + t[0].center.y - 7);
        glVertex2f(angle - t[0].center.x + 35, f(angle) + t[0].center.y - 31);
        glVertex2f(angle - t[0].center.x + 33, f(angle) + t[0].center.y - 35);
        glEnd();
        break;
    }

    case 1: {
        //watermelon
        glColor3f(0, 153 / 255., 0);
        pie_filled(angle - t[1].center.x, f(angle) + t[1].center.y, 50, 230, 310);
        glColor3f(153 / 255., 1, 153 / 255);
        pie_filled(angle - t[1].center.x, f(angle) + t[1].center.y, 45, 230, 310);
        glColor3f(1, 102 / 255., 102 / 255);
        pie_filled(angle - t[1].center.x, f(angle) + t[1].center.y, 35, 230, 310);
        glColor3f(0, 0, 0);
        circle(angle - t[1].center.x + 10, f(angle) + t[1].center.y - 25, 2);
        circle(angle - t[1].center.x - 5, f(angle) + t[1].center.y - 15, 2);
        circle(angle - t[1].center.x - 7, f(angle) + t[1].center.y - 28, 2);
        break;
    }
          //cherry
    case 2: {   glColor3f(153 / 255., 0, 0);
        circle(angle - t[2].center.x + 15, f(angle) + t[2].center.y - 10, 20);
        circle(angle - t[2].center.x - 30, f(angle) + t[2].center.y - 20, 20);

        glColor3f(0, 204 / 255., 0);

        glBegin(GL_LINES);
        glVertex2f(angle - t[2].center.x + 15, f(angle) + t[2].center.y);
        glVertex2f(angle - t[2].center.x, f(angle) + t[2].center.y + 30);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(angle - t[2].center.x - 30, f(angle) + t[2].center.y - 10);
        glVertex2f(angle - t[2].center.x, f(angle) + t[2].center.y + 30);
        glEnd();

        glLineWidth(2.0f);
        break;
    }
    }

}
void orange()
{


    glColor3f(1, 178 / 255., 102 / 255.);
    glColor3f(1, 229 / 255., 119 / 255.);
    pie_filled(0, 0, 50, 180, 360);
    glColor3f(1, 153 / 255., 51 / 255.);
    pie_filled(0, 0, 42, 180, 360);
    glColor3f(1, 229 / 255., 119 / 255.);
    pie_filled(0, 0, 8, 180, 360);
    glBegin(GL_QUADS);
    glVertex2f(-7, -3);
    glVertex2f(-5, -7);
    glVertex2f(-35, -31);
    glVertex2f(-33, -35);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(7, -3);
    glVertex2f(5, -7);
    glVertex2f(35, -31);
    glVertex2f(33, -35);
    glEnd();

}

void watermelon()
{

    glColor3f(0, 153 / 255., 0);
    pie_filled(0, 0, 50, 230, 310);
    glColor3f(153 / 255., 1, 153 / 255);
    pie_filled(0, 0, 45, 230, 310);
    glColor3f(1, 102 / 255., 102 / 255);
    pie_filled(0, 0, 35, 230, 310);
    glColor3f(0, 0, 0);
    circle(10, -25, 2);
    circle(-5, -15, 2);
    circle(-7, -28, 2);
}
void cherry()
{
    glColor3f(153 / 255., 0, 0);
    circle(15, -10, 20);
    circle(-30, -20, 20);

    glColor3f(0, 204 / 255., 0);

    glBegin(GL_LINES);
    glVertex2f(15, 0);
    glVertex2f(0, 30);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-30, -10);
    glVertex2f(0, 30);
    glEnd();

    glLineWidth(2.0f);
}
void knife()
{
    float angle2 = atan2(angle - target[0].center.x, target[0].center.y + f(angle));

    glColor3f(254 / 255., 214 / 255., 0);
    //  glRectf(fr[0].pos.x + 8, fr[0].pos.y - 45, fr[0].pos.x - 8, fr[0].pos.y);
    glBegin(GL_QUADS);
    Vertex({ -8, -120 }, fr[0].pos, angle2);
    Vertex({ -8, -75 }, fr[0].pos, angle2);
    Vertex({ 8, -30 }, fr[0].pos, angle2);
    Vertex({ 8, -120 }, fr[0].pos, angle2);
    glEnd();

    // circle(fr.pos.x, fr.pos.y - 45, 8);

    // glRectf(fr[0].pos.x + 16, fr[0].pos.y - 4, fr[0].pos.x - 16, fr[0].pos.y);

    glBegin(GL_QUADS);
    Vertex({ 16, -75 }, fr[0].pos, angle2);
    Vertex({ 16, -79 }, fr[0].pos, angle2);
    Vertex({ -16, -79 }, fr[0].pos, angle2);
    Vertex({ -16, -75 }, fr[0].pos, angle2);
    glEnd();

    glColor3f(160 / 255., 160 / 255., 160 / 255.);
    // glRectf(fr[0].pos.x + 10, fr[0].pos.y + 60, fr[0].pos.x - 10, fr[0].pos.y);

    glBegin(GL_QUADS);
    Vertex({ -10, -75 }, fr[0].pos, angle2);
    Vertex({ -10, -15 }, fr[0].pos, angle2);
    Vertex({ 10, -15 }, fr[0].pos, angle2);
    Vertex({ 10, -75 }, fr[0].pos, angle2);
    glEnd();

    glBegin(GL_TRIANGLES);
    /* glVertex2f(fr[0].pos.x - 10, fr[0].pos.y + 60);
     glVertex2f(fr[0].pos.x + 10, fr[0].pos.y + 60);
     glVertex2f(fr[0].pos.x, fr[0].pos.y + 75);*/
    Vertex({ -10, -15 }, fr[0].pos, angle2);
    Vertex({ 10, -15 }, fr[0].pos, angle2);
    Vertex({ 0, 0 }, fr[0].pos, angle2);
    glEnd();

    glColor3f(96 / 255., 96 / 255., 96 / 255.);
    glBegin(GL_LINES);
    /*glVertex2f(fr[0].pos.x + 5, fr[0].pos.y + 50);
    glVertex2f(fr[0].pos.x + 5, fr[0].pos.y + 10);*/
    Vertex({ 5, -25 }, fr[0].pos, angle2);
    Vertex({ 5, -65 }, fr[0].pos, angle2);
    glEnd();
    glLineWidth(2.0f);

}

void spinningBlade() {

    float angle2 = atan2(angle - target[0].center.x, target[0].center.y + f(angle));

    for (int i = 0; i < 12; i++) {
        glColor3ub(0, 0, 0);
        circle_wire(fr[1].pos.x, fr[1].pos.y, 22 - i);

    }

    glColor3ub(0, 0, 0);

    glBegin(GL_QUADS);
    Vertex({ -10, 20 }, fr[1].pos, angle2);
    Vertex({ -10, 22 }, fr[1].pos, angle2);
    Vertex({ 10, 22 }, fr[1].pos, angle2);
    Vertex({ 10, 20 }, fr[1].pos, angle2);
    glEnd();

    glColor3ub(0, 0, 0);

    glBegin(GL_QUADS);
    Vertex({ -10, -22 }, fr[1].pos, angle2);
    Vertex({ -10, -20 }, fr[1].pos, angle2);
    Vertex({ 10, -20 }, fr[1].pos, angle2);
    Vertex({ -10, -20 }, fr[1].pos, angle2);
    glEnd();

    glColor3ub(0, 0, 0);

    glBegin(GL_QUADS);
    Vertex({ 22, 10 }, fr[1].pos, angle2);
    Vertex({ 20, 10 }, fr[1].pos, angle2);
    Vertex({ 20, -10 }, fr[1].pos, angle2);
    Vertex({ 22, -10 }, fr[1].pos, angle2);
    glEnd();

    glColor3ub(0, 0, 0);

    glBegin(GL_QUADS);
    Vertex({ 22, 10 }, fr[1].pos, angle2);
    Vertex({ 20, 10 }, fr[1].pos, angle2);
    Vertex({ 20, -10 }, fr[1].pos, angle2);
    Vertex({ 22, -10 }, fr[1].pos, angle2);
    glEnd();

    glColor3f(0.6, 0.6, 0.6);
    glBegin(GL_POLYGON);
    Vertex({ -10, 22 }, fr[1].pos, angle2);
    Vertex({ 10, 22 }, fr[1].pos, angle2);
    Vertex({ 13, 57 }, fr[1].pos, angle2);
    Vertex({ 0, 92 }, fr[1].pos, angle2);
    Vertex({ -13, 57 }, fr[1].pos, angle2);
    glEnd();

    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_QUADS);
    Vertex({ -1, 25 }, fr[1].pos, angle2);
    Vertex({ 1, 25 }, fr[1].pos, angle2);
    Vertex({ 1, 72 }, fr[1].pos, angle2);
    Vertex({ -1, 72 }, fr[1].pos, angle2);
    glEnd();

    glColor3f(0.6, 0.6, 0.6);
    glBegin(GL_POLYGON);
    Vertex({ -10, -22 }, fr[1].pos, angle2);
    Vertex({ +10, -22 }, fr[1].pos, angle2);
    Vertex({ 13, -57 }, fr[1].pos, angle2);
    Vertex({ 0, -92 }, fr[1].pos, angle2);
    Vertex({ -13, -57 }, fr[1].pos, angle2);
    glEnd();

    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_QUADS);
    Vertex({ -1, -25 }, fr[1].pos, angle2);
    Vertex({ 1, -25 }, fr[1].pos, angle2);
    Vertex({ 1, -72 }, fr[1].pos, angle2);
    Vertex({ -1, -72 }, fr[1].pos, angle2);
    glEnd();

    glColor3f(0.6, 0.6, 0.6);
    glBegin(GL_POLYGON);
    Vertex({ -22, -10 }, fr[1].pos, angle2);
    Vertex({ -22, 10 }, fr[1].pos, angle2);
    Vertex({ -57, 13 }, fr[1].pos, angle2);
    Vertex({ -92, 0 }, fr[1].pos, angle2);
    Vertex({ -57, -13 }, fr[1].pos, angle2);
    glEnd();

    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_QUADS);
    Vertex({ -25, -1 }, fr[1].pos, angle2);
    Vertex({ -72, -1 }, fr[1].pos, angle2);
    Vertex({ -72, 1 }, fr[1].pos, angle2);
    Vertex({ -25, 1 }, fr[1].pos, angle2);
    glEnd();

    glColor3f(0.6, 0.6, 0.6);
    glBegin(GL_POLYGON);
    Vertex({ 22, -10 }, fr[1].pos, angle2);
    Vertex({ 22, 10 }, fr[1].pos, angle2);
    Vertex({ 57, 13 }, fr[1].pos, angle2);
    Vertex({ 92, 0 }, fr[1].pos, angle2);
    Vertex({ 57, -13 }, fr[1].pos, angle2);
    glEnd();

    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_QUADS);
    Vertex({ 25, -1 }, fr[1].pos, angle2);
    Vertex({ 72, -1 }, fr[1].pos, angle2);
    Vertex({ 72, 1 }, fr[1].pos, angle2);
    Vertex({ 25, 1 }, fr[1].pos, angle2);
    glEnd();
}
void display() {
    //
    // clear window to black
    //
    glClearColor(153. / 255, 76 / 255., 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);


    background();


    if (knifechange == 0)
        knife();
    else
    {
        spinningBlade();
    }

    fruites(target);




    // light source 



    if (active || salise + saniye != 0)
    {
        glColor3f(1, 1, 1);
        vprint(-210, -340, GLUT_BITMAP_9_BY_15, "click left to play, <F1> to resume or pause.");
    }
    else
    {
        glColor3f(1, 1, 1);
        vprint(-150, -340, GLUT_BITMAP_9_BY_15, "Press <F1> to start a new game.");
    }




    glutSwapBuffers();
}

void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ')
    {
    }
    if (key == ' ')
    {
        spacebar = true;

    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);
    if (key == ' ')
        spacebar = false;




    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    }
    if (key == GLUT_KEY_UP)
    {

    }
    if (key == GLUT_KEY_DOWN)
    {

    }

    if (key == GLUT_KEY_F1)
    {
        active = !active;
        pr = !pr;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.

    point_t click = { x - winWidth / 2 , winHeight / 2 - y };
    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {

        knifechange++;
        knifechange = knifechange % 2;
        if (knifechange == 1)
        {
            fr[0].pos.x = mouse.x;
            fr[0].pos.y = mouse.y;
            fr[1].pos.x = mouse.x;
            fr[1].pos.y = mouse.y;
        }
        else
        {
            fr[1].pos.x = mouse.x;
            fr[1].pos.y = mouse.y;
            fr[0].pos.x = mouse.x;
            fr[0].pos.y = mouse.y;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.

    mouse.x = x - winWidth / 2;
    mouse.y = winHeight / 2 - y;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}




bool testCollision(fire_t fr) {
    float dx = angle - fr.pos.x;
    float dy = f(angle) - fr.pos.y;
    float d = sqrt(dx * dx + dy * dy);
    return d <= 40;
}


#if TIMER_ON == 1
void onTimer(int v) {


    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (active)
    {


        angle += 2;
        if (angle > 700)
            angle = -700;




        switch (knifechange)
        {

        case 0:
        {
            if (!hit) {
                fr[0].pos = addV(fr[0].pos, fr[0].vel);
                fr[0].vel = mulV(ENEMY_SPEED, unitV(subV({ angle, f(angle) }, fr[0].pos)));
            }
            else {


                fr[0].vel = mulV(ENEMY_SPEED, unitV(subV({ 0, 0 }, fr[0].pos)));
                fr[0].pos = addV(fr[0].pos, fr[0].vel);
                target[phase].speed = mulV(ENEMY_SPEED, unitV(subV({ 0, 0 }, fr[0].pos)));
                target[phase].center = addV(fr[0].pos, fr[0].vel);
            }
            break;
        }
        case 1:
        {
            if (!hit) {
                fr[1].pos = addV(fr[1].pos, fr[1].vel);
                fr[1].vel = mulV(ENEMY_SPEED, unitV(subV({ angle, f(angle) }, fr[1].pos)));
            }
            else {


                fr[1].vel = mulV(ENEMY_SPEED, unitV(subV({ 0, 0 }, fr[1].pos)));
                fr[1].pos = addV(fr[1].pos, fr[1].vel);
                target[phase].speed = mulV(ENEMY_SPEED, unitV(subV({ 0, 0 }, fr[1].pos)));
                target[phase].center = addV(fr[1].pos, fr[1].vel);
            }
            break;
        }
        }

        if (testCollision(fr[knifechange]))
        {
            hit = true;
            knifehide = true;
        }

        if (knifehide) {

            knifehide = false;
            angle = -700;

            target[phase].center = { 0, 0 };
            if (phase == 2) {
                phase = 0;

            }
            else if (phase == 1) {
                phase++;

            }
            else {
                phase++;

            }
            clickm = true;






        }
        if (clickm)
        {
            fr[knifechange].pos = { 500,-500 };
            clickm = false;
        }


        hit = false;
















        // to refresh the window it calls display() function
        glutPostRedisplay(); // display()

    }
}

#endif

void Init() {



    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Knife at Grocery Shop BY Cavit Mert Ergul");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);




#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();
    glutMainLoop();
}