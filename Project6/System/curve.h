#define NUMPOINTS  10
void RotateY(Point *p, float deg, float xc, float yc, float zc)
{
    float rad = deg * (M_PI / 180.f); // radians
    float x = p->x0 - xc;
    float y = p->y0 - yc;
    float z = p->z0 - zc;

    float xp = x * cos(rad) + z * sin(rad);
    float yp = y;
    float zp = -x * sin(rad) + z * cos(rad);

    p->x = xp + xc;
    p->y = yp + yc;
    p->z = zp + zc;
}
void RotateZ(Point *p, float deg, float xc, float yc, float zc)
{
    float rad = deg * (M_PI / 180.f); // radians
    float x = p->x0 - xc;
    float y = p->y0 - yc;
    float z = p->z0 - zc;

    float xp = x * cos(rad) - y * sin(rad);
    float yp = x * sin(rad) + y * cos(rad);
    float zp = z;

    p->x = xp + xc;
    p->y = yp + yc;
    p->z = zp + zc;
}

void DrawCatmullRomCurve()
{
    glLineWidth(3.);
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_STRIP);
    for (int it = 0; it <= NUMPOINTS; it++)
    {
        float t = (float)it / (float)NUMPOINTS;
        // float x =...
        // float y =...
        // float z =...
        glVertex3f(x, y, z);
    }
    glEnd();
    glLineWidth(1.);
}