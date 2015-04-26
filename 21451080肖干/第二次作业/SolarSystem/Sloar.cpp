//
//  Sloar.cpp
//  SolarSystem
//
//  Created by xiaoo_gan on 4/26/15.
//  Copyright (c) 2015 xiaoo_gan. All rights reserved.
//

// Rotation amounts

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLFrame.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>

#include <math.h>
#include <stdio.h>

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

void SpecialKeys(int key, int x, int y);
void ChangeSize( GLsizei w, GLsizei h );
void DrawAxis();
void SetupRC();

void RenderScene()
{
    //清空颜色缓冲区，填充的颜色由 glClearColor( 0, 0.0, 0.0, 1 ); 指定为黑色
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Save matrix state and do the rotation
    
    glPushMatrix();
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);
    
    //glTranslatef( 0, 0, -200 );//将整个场景移动到视野中
    
    
    DrawAxis();
    
    
    //绘制太阳
    
    glColor3f( 1.0f, 0.0f, 0.0f );//红色
    
    glDisable( GL_LIGHTING );
    glutSolidSphere( 50.f, 15, 15 );
    
    //启动0号光源（模拟太阳的光芒）
    
    glEnable( GL_LIGHTING );
    GLfloat lightPos[] = { 0, 0, 0 };
    glLightfv( GL_LIGHT0, GL_POSITION, lightPos );
    
    
    //绘制地球
    
    static GLfloat earthAngle = 0;
    const GLfloat RADIUS = 100.f;
    glRotatef( earthAngle, 0, 0, 1 );
    glTranslatef( RADIUS, RADIUS, 0 );
    glColor3f( 0.0f, 0.0f, 1.0f );//蓝色
    
    glutSolidSphere( 20.f, 15, 15 );
    
    //启动1号光源,模拟地球的散射光
    
    glLightfv( GL_LIGHT1, GL_POSITION, lightPos );
    GLfloat whiteDiffuseLight[] = {0.1, 0.1, 0.1}; //set the diffuse light to white
    
    glLightfv( GL_LIGHT1, GL_DIFFUSE, whiteDiffuseLight );
    
    // 绘制白色的“月亮”
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glRotatef(earthAngle/30.0*360.0 - earthAngle/360.0*360.0, 0.0f, 0.0f, -1.0f);
    glTranslatef(RADIUS/3, 0.0f, 0.0f);
    glutSolidSphere(7, 20, 20);
    
    
    //启动2号光源,模拟月球的反射光
    
    glLightfv( GL_LIGHT2, GL_POSITION, lightPos );
    GLfloat graySpecularLight[] = {0.2, 0.2, 0.2}; //set the Specular light to gray white
    
    glLightfv( GL_LIGHT2, GL_DIFFUSE, graySpecularLight );
    
    earthAngle += 0.1;
    if ( earthAngle >= 360 )
    {
        earthAngle -= 360;
    }
    
    printf( "Angle:%f\n", earthAngle );
    
    // Restore transformations
    
    glPopMatrix();
    
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (800, 600);
    glutInitWindowPosition (100, 100);
    glutCreateWindow( "Triangles Example" );
    glutDisplayFunc( RenderScene );
    glutIdleFunc (RenderScene); //glutIdleFunc 表示在CPU空闲的时间调用某一函数
    
    glutReshapeFunc( ChangeSize );
    glutSpecialFunc( SpecialKeys );
    SetupRC();
    glutMainLoop();
    
    return 1;
}


void SetupRC()
{
    glClearColor( 0, 0.0, 0.0, 1 );
    glShadeModel( GL_SMOOTH );
    glEnable(GL_DEPTH_TEST);//被遮住的部分不绘制
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
}


void ChangeSize( GLsizei w, GLsizei h )
{
    GLfloat nRange = 200.0f;
    
    // Prevent a divide by zero
    
    if(h == 0)
        h = 1;
    
    // Set Viewport to window dimensions
    
    glViewport(0, 0, w, h);
    
    // Reset projection matrix stack
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Establish clipping volume (left, right, bottom, top, near, far)
    
    if (w <= h)
        glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else
        glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
    
    //45度视野，近、远平面分别为1和200
    
    //gluPerspective( 45.f, (GLfloat)w/(GLfloat)h, 1, 200 );
    
    
    // Reset Model view matrix stack
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpecialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP)
        xRot -= 5.0f;
    
    if(key == GLUT_KEY_DOWN)
        xRot += 5.0f;
    
    if(key == GLUT_KEY_LEFT)
        yRot -= 5.0f;
    
    if(key == GLUT_KEY_RIGHT)
        yRot += 5.0f;
    
    if(xRot > 356.0f)
        xRot = 0.0f;
    
    if(xRot < -1.0f)
        xRot = 355.0f;
    
    if(yRot > 356.0f)
        yRot = 0.0f;
    
    if(yRot < -1.0f)
        yRot = 355.0f;
    
    // Refresh the Window
    
    glutPostRedisplay();// this will refresh the window, so, it works the same to call RenderScene() directly
    
}


void DrawAxis()
{
    //绘制x、y、z坐标轴
    
    {
        glColor3f( 0.0f, 0.0f, 1.0f );//指定线的颜色,蓝色
        
        glBegin( GL_LINES );
        {
            // x-axis
            
            glVertex3f( 0.0f, 0.0f, 0.0f);
            glVertex3f( 200.0f, 0.0f, 0.0f);
            
            // x-axis arrow
            
            glVertex3f( 200.0f, 0.0f, 0.0f);
            glVertex3f( 193.0f, 3.0f, 0.0f);
            glVertex3f( 200.0f, 0.0f, 0.0f);
            glVertex3f( 193.0f,-3.0f, 0.0f);
        }
        glEnd();
        
        glColor3f( 0.0f, 1.0f, 0.0f );//指定线的颜色,绿色
        
        glBegin( GL_LINES );
        {
            // y-axis
            
            glVertex3f( 0.0f, 0.0f, 0.0f);
            glVertex3f( 0.0f, 200.0f, 0.0f);
            glVertex3f( 0.0f, 200.0f, 0.0f);
            glVertex3f( 3.0f, 193.0f, 0.0f);
            glVertex3f( 0.0f, 200.0f, 0.0f);
            glVertex3f( -3.0f, 193.0f, 0.0f);
        }
        glEnd();
        
        
        glColor3f( 1.0f, 0.0f, 0.0f );//指定线的颜色,红色
        
        glBegin( GL_LINES );
        {
            // z-axis
            
            glVertex3f( 0.0f, 0.0f, 0.0f );
            glVertex3f( 0.0f, 0.0f, 200.0f );
            glVertex3f( 0.0f, 0.0f, 200.0f );
            glVertex3f( 0.0f, 3.0f, 193.0f );
            glVertex3f( 0.0f, 0.0f, 200.0f );
            glVertex3f( 0.0f, -3.0f, 193.0f);
        }
        glEnd();
    }
}
