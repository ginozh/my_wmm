/************************************************************************       
\link   www.twinklingstar.cn
\author Twinkling Star
\date   2015/02/03
\file   opengl_helloworld.cpp
****************************************************************************/
#include <GL/glut.h> 
 
void init()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
 
void draw(void)
{
    //set the modelview matrix stack. GL_MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-6.0f);
    glBegin(GL_TRIANGLES);                              // Start Drawing A Triangle
        glColor3f(1.0f,0.0f,0.0f);                      // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Front)
        glColor3f(0.0f,1.0f,0.0f);                      // Green
        glVertex3f(-1.0f,-1.0f, 1.0f);                  // Left Of Triangle (Front)
        glColor3f(0.0f,0.0f,1.0f);                      // Blue
        glVertex3f( 1.0f,-1.0f, 1.0f);                  // Right Of Triangle (Front)
        glColor3f(1.0f,0.0f,0.0f);                      // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Right)
        glColor3f(0.0f,0.0f,1.0f);                      // Blue
        glVertex3f( 1.0f,-1.0f, 1.0f);                  // Left Of Triangle (Right)
        glColor3f(0.0f,1.0f,0.0f);                      // Green
        glVertex3f( 1.0f,-1.0f, -1.0f);                 // Right Of Triangle (Right)
        glColor3f(1.0f,0.0f,0.0f);                      // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Back)
        glColor3f(0.0f,1.0f,0.0f);                      // Green
        glVertex3f( 1.0f,-1.0f, -1.0f);                 // Left Of Triangle (Back)
        glColor3f(0.0f,0.0f,1.0f);                      // Blue
        glVertex3f(-1.0f,-1.0f, -1.0f);                 // Right Of Triangle (Back)
        glColor3f(1.0f,0.0f,0.0f);                      // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Left)
        glColor3f(0.0f,0.0f,1.0f);                      // Blue
        glVertex3f(-1.0f,-1.0f,-1.0f);                  // Left Of Triangle (Left)
        glColor3f(0.0f,1.0f,0.0f);                      // Green
        glVertex3f(-1.0f,-1.0f, 1.0f);                  // Right Of Triangle (Left)
    glEnd();                                            // Done Drawing The Pyramid
 
    glFlush();
}
 
void reshape(GLsizei w,GLsizei h)
{
    //set viewport
    glViewport(0,0,w,h);
    //Set the projection stack, GL_PROJECTION
    glMatrixMode(GL_PROJECTION);
    //set the unit matrix.
    glLoadIdentity();
    gluPerspective(45, w / h, 1.0f, 100.0f);
}
 
int main(int argc,char ** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(400,400);
 
    glutCreateWindow("example");
 
    init();
 
    glutReshapeFunc(reshape);
    glutDisplayFunc(draw);
 
    glutMainLoop();
    return(0);
}
