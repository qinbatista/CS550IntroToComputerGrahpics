//	Author:			Yupeng Qin
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define _USE_MATH_DEFINES
#include <math.h>
#ifdef WIN32
#include <windows.h>
#pragma warning(disable : 4996)
#endif
#include <GL/glew.h>
#include <OpenGL/GL.h>
#include <OpenGL/glu.h>
#include "glslprogram.h"
#include <list>
#include <iostream>
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
using namespace std;
list<GLuint> modelList;
int main(int argc, char *argv[])
{
    MetaInitial(argc, argv);
    return 0;
}
void WorldInitial()
{
    CreateAxis();
    // MyFirstObject();
    // WireFrame();
    // PolygonFrame();
    // Propeller();
    CreateEarthTexture();
    SetLight();
    modelList.push_front(InitialObjFile_SMOOTH((char *)"ObjModel/FinalBaseMesh.obj", 10, 0, 0, -90, 0, 1, 0));
    modelList.push_front(InitialObjFile_SMOOTH((char *)"ObjModel/FinalBaseMesh.obj", -10, 0, 0, 90, 0, 1, 0));
    modelList.push_front(InitialObjFile_FLAT((char *)"ObjModel/FinalBaseMesh.obj", 0, 0, -10, 0, 0, 1, 0));
    // SetPointLight(GL_LIGHT0, 0, 20, 0, 1, 1, 1);
    // SetSpotLight(GL_LIGHT1, 0, 35, 0, 1, 0, 0, 0, 1., 0);
}
void WorldDisplay()
{
    for (GLuint model : modelList)
    {
        glCallList(model);
    }

    if (Light0On)
    {
        SetPointLight(GL_LIGHT0, 0, 0, 0, 1, 1, 1);
    }
    else
    {
        glDisable(GL_LIGHT0);
    }

    if (Light1On)
    {
        SetPointLight(GL_LIGHT1, 0, 10 + 20 * sin(1. * M_PI * TimeCycle), 0, 0.3 * sin(1. * M_PI * TimeCycle), 0, 0.9 * sin(1. * M_PI * TimeCycle));
    }
    else
    {
        glDisable(GL_LIGHT1);
    }

    if (Light2On)
    {
        SetSpotLight(GL_LIGHT2, 0, 0, 0, 0, 20, 0, 0.5 * sin(1. * M_PI * TimeCycle), 0.2 * sin(1. * M_PI * TimeCycle), 0.1);
    }
    else
    {
        glDisable(GL_LIGHT2);
    }
    // SetSpotLight(GL_LIGHT2, 0, 0, 0, 10, 20, 0, 0.3 * sin(1. * M_PI * TimeCycle), sin(1. * M_PI * TimeCycle), 0.7);

    OSUSphereDisplayTextureOnNoLight(OSUSphere(1.0, 20, 20, 0, 20 * sin(1. * M_PI * TimeCycle), 0));
    // SetMaterial(1., 0, 0, 0.5);
    OSUSphereDisplayTextureOn(OSUSphere(1.0, 20, 20, 0, 20, 0));
    OSUSphereDisplay(OSUSphere(2.0, 20, 20, 0, 30, 0));
}
void WorldUpdate()
{
    int ms = glutGet(GLUT_ELAPSED_TIME); // milliseconds
    ms %= MS_IN_THE_ANIMATION_CYCLE;
    TimeCycle = (float)ms / (float)MS_IN_THE_ANIMATION_CYCLE; // [0., 1.]
    OSUSphereAnimation(TimeCycle);
}