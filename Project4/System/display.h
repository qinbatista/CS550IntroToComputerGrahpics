float White[] = {1., 1., 1., 1.};
float Green[] = {0., 1., 0., 1.};
void SetLight();
float *Array3(float a, float b, float c)
{
    static float array[4];
    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = 1.;
    return array;
}
float *MulArray3(float factor, float array0[3])
{
    static float array[4];
    array[0] = factor * array0[0];
    array[1] = factor * array0[1];
    array[2] = factor * array0[2];
    array[3] = 1.;
    return array;
}
void DisplaySetting()
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
        gluLookAt(8., 4., 7., 0., 0., 1.6, 0., 1., 0.);
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

    if (AxesOn != 0)
    {
        glColor3fv(&Colors[WhichColor][0]);
        glCallList(AxesList);
    }
    SetLight();
    // SetMaterial();
    glEnable(GL_NORMALIZE);
}
void SetMaterial(float r, float g, float b, float shininess)
{
    glMaterialfv(GL_BACK, GL_EMISSION, Array3(0., 0., 0.));
    glMaterialfv(GL_BACK, GL_AMBIENT, MulArray3(.4f, White));
    glMaterialfv(GL_BACK, GL_DIFFUSE, MulArray3(1., White));
    glMaterialfv(GL_BACK, GL_SPECULAR, Array3(0., 0., 0.));
    glMaterialf(GL_BACK, GL_SHININESS, 2.f);
    glMaterialfv(GL_FRONT, GL_EMISSION, Array3(0., 0., 0.));
    glMaterialfv(GL_FRONT, GL_AMBIENT, Array3(r, g, b));
    glMaterialfv(GL_FRONT, GL_DIFFUSE, Array3(r, g, b));
    glMaterialfv(GL_FRONT, GL_SPECULAR, MulArray3(.8f, White));
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
void SetLight()
{
    glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // possibly draw the axes:
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_POSITION, Array3(10, 0, 0));
    gluLookAt(1., 1., 1., 0., 0., 0., 0., 1., 0.);


    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, MulArray3(.2, White));
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightfv(GL_LIGHT0, GL_AMBIENT, Array3(0., 0., 0.));
    glLightfv(GL_LIGHT0, GL_DIFFUSE, White);
    glLightfv(GL_LIGHT0, GL_SPECULAR, White);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.);
}
void DisplayBuffer()
{
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