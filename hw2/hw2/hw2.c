e
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.1416

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=45;         //  Elevation of view angle
int fov=70;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=70.0;   //  Size of world
int fpv;
double xfpv=0.0;
double yfpv =0.0;
double zfpv =20.0;
int cameraRotation = 0.0;
double Cx = 0.0;
double Cy = 0.0;
double Cz = 0.0;
double scaling = 0.1;

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (fpv) {
     gluPerspective(fov,asp,dim/4,4*dim);
   }
   else
   {
    if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
    else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
    }

   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  fpv
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(0.5,0.5,0.5);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0.5,0.5,0.5);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}
/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void trianglequad(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  fpv
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  back
   glColor3f(0,0,1);
   glVertex3f(0,0,0);
   glVertex3f(1,0,0);
   glVertex3f(1,1,0);
   glVertex3f(0,1,0);

   //  triangle side
   glColor3f(0,0,1);
   glVertex3f(0,0,0);
   glVertex3f(0,1,-1);
   glVertex3f(0,1,0);
   glVertex3f(0,0,0);
   //  triangle side
   glColor3f(0,1,0);
   glVertex3f(1,0,0);
   glVertex3f(1,1,0);
   glVertex3f(1,1,-1);
   glVertex3f(1,0,0);
  //  top
   glColor3f(1,0,1);
   glVertex3f(0,1,-1);
   glVertex3f(1,1,-1);
   glVertex3f(1,1,0);
   glVertex3f(0,1,0);
  //  front
   glColor3f(1,0,0);
   glVertex3f(0,0,0);
   glVertex3f(1,0,0);
   glVertex3f(1,1,-1);
   glVertex3f(0,1,-1);

   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw vertex in polar coordinates
 */
 static void Vertex(double th,double ph)
 {
   //glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
 }

  /*
   * Draws a Cylinder
   */
void cylinder(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   double radius = 0.3, height = 0.2;

   glPushMatrix();

   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   glColor3f(1.0,0.0,0.0);
   /* Tube of Cylinder */
   glBegin(GL_QUAD_STRIP);
      for(th=0;th<=360;th+=1){
         glVertex3d(radius * cos(th), height, radius * sin(th));
         glVertex3d(radius * cos(th), -height, radius * sin(th));
      }
   glEnd();

   /* Top of Cylinder */
   glColor3f(0.0,1.0,0.0);
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, height, 0.0);
      for(th=0;th<=360;th+=1) {
         glVertex3d(radius * cos(th), height, radius * sin(th));
      }
   glEnd();
   /* Bottom of Cylinder */
   glColor3f(0.0,0.0,1.0);
   glBegin(GL_TRIANGLE_FAN);
      glVertex3d(0.0, -height, 0.0);
      for(th=0;th<=360;th+=1) {
         glVertex3d(radius * cos(th), -height, radius * sin(th));
      }
   glEnd();

   glPopMatrix();
}

//runway plane
static void runway(double x,double y, double z,double s)
{
  glScaled(s,s,s);
  glBegin(GL_QUADS);
  glColor3f(0.3,0.3,0.3);
  glVertex3f(-10,-0.151,-10);
  glVertex3f(10,-0.151,-10);
  glVertex3f(10,-0.151,10);
  glVertex3f(-10,-0.151,10);

  glColor3f(1,1,0);
  glVertex3f(-2,-0.15,-10);
  glVertex3f(2,-0.15,-10);
  glVertex3f(2,-0.15,10);
  glVertex3f(-2,-0.15,10);

  glEnd();
}

/*
 *  Draw a sphere
 *     at (x,y,z)
 *     radius (r)
 */
static void sphere(double x,double y,double z,double r)
{
   const int d=5;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  fpv and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }
   //  Undo transformations
   glPopMatrix();
}

static void tower(double x, double y, double z, double s)
{
  glTranslated(x,y,z);
  glScaled(s,s,s);
  cylinder(25,10,0,5,60,5,0);
  trianglequad(21,20,0,7,7,7,0);
  trianglequad(28,20,0,7,7,7,180);
}

static void plane(double x, double y, double z,double s, double th)
{
  glTranslated(x,y,z);
  glScaled(s,s,s);
  glRotatef(th,0,1,0);
  glPushMatrix();
  glRotatef(180,0,0,1);
  glScaled(0.5,0.5,0.5);
  cube(0,0,0,1,1,2,0);
  trianglequad(-1,-1,-2,2,2,2,0);
  cube(0,0,3,1,1,1,0);
  glPopMatrix();

  glTranslated(0,-2,0);
  glScaled(0.5,0.5,0.5);
  cube(0,2,0,1,1,2,0);
  trianglequad(-1,1,-2,2,2,2,0);
  trianglequad(1,1,2,2,2,2,180);

  cylinder(0,5,0,1,40,6.5,90);

  glPushMatrix();
  glRotatef(90,1,0,0);
  cylinder(0,8,-4,1,20,2,0);
  glPopMatrix();

  cylinder(1,1,2,1,5,1,45);
  cylinder(-1,1,2,1,5,1,135);
  cylinder(0,1,-2,1,5,1,180);
  cylinder(0,4,11.5,1,10,2,90);

  sphere(2,0,2,0.65);
  sphere(-2,0,2,0.65);
  sphere(0,0,-2,0.65);

  glPushMatrix();
  glRotatef(180,0,0,1);
  trianglequad(-0.25,-6,12,0.5,2,2,0);
  glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=20;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();

   if (fpv)
   //Using First person view and adjusting cameraRotation
   {
     Cx = +2*dim*Sin(cameraRotation);
     Cz = -2*dim*Cos(cameraRotation);

     gluLookAt(xfpv,yfpv,zfpv, Cx+xfpv,yfpv,Cz+zfpv, 0,1,0);
   }
   else
   {
     //  Perspective - set eye position
     if (mode)
     {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim*Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
  //  Orthogonal - set world orientation
      else
      {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
      }
    }

   //draw plane
   plane(-20,0,-40,1,0);
   plane(0,0,30,2.5,25);
   plane(0,0,40,5,45);
   tower(0,0,0,1);

   runway(0,0,0,4);

   //  glTranslated(15,2,20);
   //  glScaled(5,5,5);
   //  plane(0,0,0,0);
   //
   //  glTranslated(20,5,10);
   //  glScaled(2,2,2);
   //  plane(0,0,0,0);


   //  Draw axes
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);

   if (fpv) {
      Print("FPV=On View Angle=%d Camera Orentation %d,%.1f",th,xfpv,cameraRotation);
   }
   else {
      Print("FPV=Off Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   }
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
  if (!fpv) {
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
  }
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
  //  Exit on ESC
if (ch == 27)
  exit(0);
//  Toggle first person
else if (ch == 'f')
{
  fpv = 1-fpv;
}

if (fpv) {
  if (ch == 'w'){
     xfpv += Cx*scaling;
     zfpv += Cz*scaling;
  }
  else if (ch == 'a'){
     cameraRotation -= 3;
  }
  else if (ch == 's'){
     xfpv -= Cx*scaling;
     zfpv -= Cz*scaling;
  }
  else if (ch == 'd'){
     cameraRotation += 3;
  }
  else if (ch == 'e')
  {
    yfpv += 1;
  }
  else if (ch == 'q')
  {
    yfpv -= 1;
  }

  //  Keep angles to +/-360 degrees
  cameraRotation %= 360;
}
else {
  //  Reset view angle
  if (ch == '0')
     th = ph = 0;
  //  Switch display mode
  else if (ch == 'p')
     mode = 1-mode;
  // Toggle Axes
  else if (ch == 'x')
     axes = 1-axes;
  //  Change field of view angle
  else if (ch == '1')
     fov--;
  else if (ch == '2')
     fov++;
  else if (ch =='l')
     dim++;
  else if (ch == 'k')
     dim--;

}
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("HW2: Jesse Bird");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
