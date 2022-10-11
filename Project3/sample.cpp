#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable : 4996)
#endif

#include </opt/homebrew/Cellar/glew/2.2.0_1/include/GL/glew.h>
#include <OpenGL/GL.h>
#include <OpenGL/glu.h>
#include "MyLibrary/glut.h"
#include "sample.h"
#include "System/enum.h"
#include "System/initialize.h"
#include "System/controller.h"
#include "System/menu.h"
#include "Objects/cessna.550"
#include "Objects/plane.h"
#include "Objects/bmptotexture.h"
#include "Objects/Axes.h"
//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Yupeng Qin

// main program:
int main(int argc, char *argv[])
{
    // turn on the glut package:
    // (do this before checking argc and argv since it might
    // pull some command line arguments out)
    glutInit(&argc, argv);
    // setup all the graphics stuff:
    InitGraphics();
    // create the display structures that will not change:
    InitLists();
    // init all the global variables used by Display( ):
    // this will also post a redisplay
    Reset();
    // setup all the user interface stuff:
    InitMenus();
    // draw the scene once and wait for some interaction:
    // (this will never return)
    glutSetWindow(MainWindow);
    glutMainLoop();
    // glutMainLoop( ) never actually returns
    // the following line is here to make the compiler happy:
    return 0;
}

// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutPostRedisplay( ) do it
void Animate()
{
    // put animation stuff in here -- change some global variables
    // for Display( ) to find:
    int ms = glutGet(GLUT_ELAPSED_TIME); // milliseconds
    ms %= MS_IN_THE_ANIMATION_CYCLE;
    TimeCycle = (float)ms / (float)MS_IN_THE_ANIMATION_CYCLE; // [0., 1.]
    BladeAngle = TimeCycle * 360.;
    // force a call to Display( ) next time it is convenient:
    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

void InitLists()
{
    glutSetWindow(MainWindow);
    CreateObject();
    WireFrame();
    PolygonFrame();
    Propeller();
    CreateAxis();
}

// draw the complete scene:
void Display()
{
    // set which window we want to do the graphics into:
    glutSetWindow(MainWindow);
    // erase the background:
    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
#ifdef DEMO_DEPTH_BUFFER
    if (DepthBufferOn == 0)
        glDisable(GL_DEPTH_TEST);
#endif
    // specify shading to be flat:
    glShadeModel(GL_FLAT);
    // set the viewport to a square centered in the window:
    GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
    GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
    GLsizei v = vx < vy ? vx : vy; // minimum dimension
    GLint xl = (vx - v) / 2;
    GLint yb = (vy - v) / 2;
    glViewport(xl, yb, v, v);
    // set the viewing volume:
    // remember that the Z clipping  values are actually
    // given as DISTANCES IN FRONT OF THE EYE
    // USE gluOrtho2D( ) IF YOU ARE DOING 2D !
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (WhichProjection == ORTHO)
        glOrtho(-2.f, 2.f, -2.f, 2.f, 0.1f, 1000.f);
    else
        gluPerspective(70.f, 1.f, 0.1f, 1000.f);
    // place the objects into the scene:
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // set the eye position, look-at position, and up-vector:
    // set the eye position, look-at position, and up-vector:
    if (WhichViewMode == INSIDE)
    {
        gluLookAt(0, 1.2, 1, 0., 5, 10., 0., 1., 0.);
    }
    else
    {
        gluLookAt(16., 7., 15., 0., 0., 1.6, 0., 1., 0.);
    }

    // rotate the scene:

    glRotatef((GLfloat)Yrot, 0.f, 1.f, 0.f);
    glRotatef((GLfloat)Xrot, 1.f, 0.f, 0.f);

    // uniformly scale the scene:

    if (Scale < MINSCALE)
        Scale = MINSCALE;
    glScalef((GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale);

    // set the fog parameters:

    if (DepthCueOn != 0)
    {
        glFogi(GL_FOG_MODE, FOGMODE);
        glFogfv(GL_FOG_COLOR, FOGCOLOR);
        glFogf(GL_FOG_DENSITY, FOGDENSITY);
        glFogf(GL_FOG_START, FOGSTART);
        glFogf(GL_FOG_END, FOGEND);
        glEnable(GL_FOG);
    }
    else
    {
        glDisable(GL_FOG);
    }

    // possibly draw the axes:

    if (AxesOn != 0)
    {
        glColor3fv(&Colors[WhichColor][0]);
        glCallList(AxesList);
    }

    // since we are using glScalef( ), be sure the normals get unitized:

    glEnable(GL_NORMALIZE);

    // draw the box object by calling up its display list:

    glCallList(BoxList);
    glCallList(PlaneList);
    glCallList(PolygonList);

    glPushMatrix();
    glTranslatef(8., 2.9, 0.);
    glScalef(2., 2., 2.);
    glRotatef(BladeAngle * 30, 0., 1., 0.);
    glRotatef(90., 1., 0., 0.);
    glColor3f(1., 1., 1.);
    glCallList(PropellerList);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-8., 2.9, 0.);
    glScalef(2., 2., 2.);
    glRotatef(-BladeAngle * 30, 0., 1., 0.);
    glRotatef(90., 1., 0., 0.);
    glColor3f(1., 1., 1.);
    glCallList(PropellerList);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0., 7.9);
    glScalef(4, 4, 4);
    glRotatef(BladeAngle * 15, 0., 0., 1.);
    glCallList(PropellerList);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 5., 15);
    glScalef(1, 1, 1);
    glCallList(BoxList);
    glPopMatrix();
#ifdef DEMO_Z_FIGHTING
    if (DepthFightingOn != 0)
    {
        glPushMatrix();
        glRotatef(90.f, 0.f, 1.f, 0.f);
        glCallList(BoxList);
        glPopMatrix();
    }
#endif

    // draw some gratuitous text that just rotates on top of the scene:
    // i commented out the actual text-drawing calls -- put them back in if you have a use for them
    // a good use for thefirst one might be to have your name on the screen
    // a good use for the second one might be to have vertex numbers on the screen alongside each vertex

    glDisable(GL_DEPTH_TEST);
    glColor3f(0.f, 1.f, 1.f);
    // DoRasterString( 0.f, 1.f, 0.f, (char *)"Text That Moves" );

    // draw some gratuitous text that is fixed on the screen:
    //
    // the projection matrix is reset to define a scene whose
    // world coordinate system goes from 0-100 in each axis
    //
    // this is called "percent units", and is just a convenience
    //
    // the modelview matrix is reset to identity as we don't
    // want to transform these coordinates

    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.f, 100.f, 0.f, 100.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.f, 1.f, 1.f);
    // DoRasterString( 5.f, 5.f, 0.f, (char *)"Text That Doesn't" );

    // swap the double-buffered framebuffers:

    glutSwapBuffers();

    // be sure the graphics buffer has been sent:
    // note: be sure to use glFlush( ) here, not glFinish( ) !

    glFlush();
}

///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////
