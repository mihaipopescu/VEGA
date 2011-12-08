#include "camera.h"
#include <GL/GLut.h>
#include <GL/freeglut_ext.h>


vega::render::camera::camera()
{
    reset();
}

void vega::render::camera::reset()
{
    myPitch = 0.f;
    myYaw = 0.f;
    myRoll = 0.f;
    myScale = 1.f;
    myOffsetX = 0.f;
    myOffsetY = 0.f;
    myOffsetZ = -2.f;
    myOffsetDelta = 0.1f;
}

void vega::render::camera::apply()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(myOffsetX, myOffsetY, myOffsetZ);
    glScalef(myScale, myScale, 1.f);
    glRotatef(myPitch, 1, 0, 0);   // pitch
    glRotatef(myYaw, 0, 1, 0);   // yaw
    glRotatef(myRoll, 0, 0, 1);   // roll
}

void vega::render::camera::handle_common_keys( unsigned char key, int x, int y )
{
    switch(key)
    {
    case '*':
        myScale *= 2.f;
        break;
    case '/':
        myScale /= 2.f;
        break;
    case '+':
        myOffsetZ += myOffsetDelta;
        break;
    case '-':
        myOffsetZ -= myOffsetDelta;
        break;
    case '5':
        reset();
        break;
    case '8':
        myOffsetY -= myOffsetDelta;
        break;
    case '2':
        myOffsetY += myOffsetDelta;
        break;
    case '4':
        myOffsetX += myOffsetDelta;
        break;
    case '6':
        myOffsetX -= myOffsetDelta;
        break;
    }
}

void vega::render::camera::handle_special_keys( int key, int x, int y )
{
    switch(key)
    {
    case GLUT_KEY_PAGE_UP:
        myOffsetDelta *= 10.f;
        break;
    case GLUT_KEY_PAGE_DOWN:
        myOffsetDelta /= 10.f;
        break;
    }
}

void vega::render::camera::handle_mouse_buttons( int button, int state, int x, int y )
{
    myMouseX = x;
    myMouseY = y;
    switch( button )
    {
    case GLUT_LEFT_BUTTON:
        myMouseLeftDown = state == GLUT_DOWN ? true : state == GLUT_UP ? false : myMouseLeftDown;
        break;

    case GLUT_RIGHT_BUTTON:
        myMouseRightDown = state == GLUT_DOWN ? true : state == GLUT_UP ? false : myMouseRightDown;
        break;

    case GLUT_MIDDLE_BUTTON:
        mouseMiddleDown = state == GLUT_DOWN ? true : state == GLUT_UP ? false : mouseMiddleDown;
        break;
    }
}

void vega::render::camera::handle_mouse_motions( int x, int y )
{
    if(myMouseLeftDown)
    {
        myYaw += (x - myMouseX);
        myPitch += (y - myMouseY);
        myMouseX = x;
        myMouseY = y;
    }

    if(myMouseRightDown)
    {
        myOffsetZ += (y - myMouseY) * 0.2f;
        myMouseY = y;
    }
}

void vega::render::camera::render() const
{
    float O[3] = {0,0,0}, X[3] = {1,0,0}, Y[3] = {0,1,0}, Z[3] = {0,0,1};
    glPushMatrix();
    glBegin(GL_LINES);
        glColor3f(1,0,0); glVertex3fv(O); glVertex3fv(X); // OX axis is red
        glColor3f(0,1,0); glVertex3fv(O); glVertex3fv(Y); // OY axis is green
        glColor3f(0,0,1); glVertex3fv(O); glVertex3fv(Z); // OZ axis is blue
    glEnd();
    glPopMatrix();
}
