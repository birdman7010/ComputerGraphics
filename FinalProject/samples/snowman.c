#include <GL/glut.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

int DLid;
GLuint createDL() {
	GLuint snowManDL;

	snowManDL = glGenLists(1);

	glNewList(snowManDL,GL_COMPILE);
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			drawSnowMan();
			glPopMatrix();
		}
	glEndList();

	return(snowManDL);
}


void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Draw ground

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

// Draw 36 SnowMen

	glCallList(DLid);

	glutSwapBuffers();
}

int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Create window
   glutCreateWindow("Hello World");
   //  Register function used to display scene
   glutDisplayFunc(renderScene);
   //  Pass control to GLUT for events
   glutMainLoop();
   //  Return to OS
   return 0;
}
