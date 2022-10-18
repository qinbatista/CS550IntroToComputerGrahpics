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
#include "System/display.h"
#include "System/loadobjfile.h"
#include "Objects/cessna.550"
#include "Objects/plane.h"
#include "Objects/bmptotexture.h"
#include "Objects/Axes.h"
#include "Objects/osusphere.h"
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
    InitObjectsLists();
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
GLuint DinoDL;
void InitObjectsLists()
{
    glutSetWindow(MainWindow);

    CreateAxis();
    // MyFirstObject();
    // WireFrame();
    // PolygonFrame();
    // Propeller();
    // CreateEarthTexture();
    // a = InitialObjFile((char *)"ObjModel/FinalBaseMesh.obj");

    DinoDL = glGenLists(1);
    glNewList(DinoDL, GL_COMPILE);
    LoadObjFile((char *)"ObjModel/FinalBaseMesh.obj");
    glEndList();
}

// draw the complete scene:
void Display()
{
    DisplaySetting();
    glCallList(DinoDL);
    // OsuSphere(1.0, 20, 20);
    // OSUSphereDisplay();
    DisplayBuffer();

}
void Animate()
{
    int ms = glutGet(GLUT_ELAPSED_TIME); // milliseconds
    ms %= MS_IN_THE_ANIMATION_CYCLE;
    TimeCycle = (float)ms / (float)MS_IN_THE_ANIMATION_CYCLE; // [0., 1.]

    OSUSphereAnimation(TimeCycle);

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}