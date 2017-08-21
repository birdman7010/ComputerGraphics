
#include "CSCIx229.h"

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=45;         //  Elevation of view angle
int fov=70;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=80.0;   //  Size of world
int fpv;
double xfpv=0.0;
double yfpv =0.0;
double zfpv =20.0;
int cameraRotation = 0.0;
double Cx = 0.0;
double Cy = 0.0;
double Cz = 0.0;
double scaling = 0.1;
int light = 1;
int rep = 1;
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;   // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =  50;  // Elevation of light
int ntex      =   0;

unsigned int sidepanel = 0;
unsigned int glass = 0;
unsigned int asphalt = 0;
unsigned int grass = 0;
unsigned int window = 0;
unsigned int antenna = 0;
unsigned int tire = 0;
unsigned int tread = 0;
unsigned int paneling = 0;
unsigned int greenbeacon = 0;
unsigned int redbeacon = 0;
unsigned int blackconcrete = 0;

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
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 *    example 15 adapted
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, unsigned int n)
{
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, rotate, scale
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D, paneling);

   glColor3f(1,0,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back
   glColor3f(0,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glColor3f(1,1,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glColor3f(0,1,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glColor3f(0,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(1,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
/*
 *  Draw a trianglequad
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
*      adapted from cube
 */
static void trianglequad(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,unsigned int n)
{
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  //  Save transformation
  glPushMatrix();
  //  Offset, rotate, scale
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  glColor3f(1,1,1);
  glBindTexture(GL_TEXTURE_2D, paneling);
  glColor3f(1,0,0);
  if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   //  back
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0);glVertex3f(0,0,0);
   glTexCoord2f(1,0);glVertex3f(1,0,0);
   glTexCoord2f(1,1);glVertex3f(1,1,0);
   glTexCoord2f(0,1);glVertex3f(0,1,0);
   glEnd();

   //  triangle side
   glColor3f(0,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0);glVertex3f(0,0,0);
   glTexCoord2f(1,0);glVertex3f(0,1,-1);
   glTexCoord2f(1,1);glVertex3f(0,1,0);
   glTexCoord2f(0,1);glVertex3f(0,0,0);
   glEnd();
   //  triangle side
   glColor3f(0,1,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   glNormal3f(1, 0, 0);
   glTexCoord2f(0,0);glVertex3f(1,0,0);
   glTexCoord2f(1,0);glVertex3f(1,1,0);
   glTexCoord2f(1,1);glVertex3f(1,1,-1);
   glTexCoord2f(0,1);glVertex3f(1,0,0);
   glEnd();
  //  top
   glColor3f(1,0,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   glNormal3f( 0,1, 0);
   glTexCoord2f(0,0);glVertex3f(0,1,-1);
   glTexCoord2f(1,0);glVertex3f(1,1,-1);
   glTexCoord2f(1,1);glVertex3f(1,1,0);
   glTexCoord2f(0,1);glVertex3f(0,1,0);
   glEnd();
  //  front
   glColor3f(1,0,0);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, -1);
   glTexCoord2f(0,0);glVertex3f(0,0,0);
   glTexCoord2f(1,0);glVertex3f(1,0,0);
   glTexCoord2f(1,1);glVertex3f(1,1,-1);
   glTexCoord2f(0,1);glVertex3f(0,1,-1);

   glEnd();
   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

 /*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 * from ex15
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

/*
 * Draws a Cylinder
*/
void cylinder(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, unsigned int n,unsigned int n1,unsigned int n2)
{
   double radius = 0.3, height = 0.2;
   double i,j;
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

   glPushMatrix();

   //  Transform cylinder
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D, paneling);
   glColor3f(1,0,0);

   if (ntex) glBindTexture(GL_TEXTURE_2D,n);
   /* Tube of Cylinder */
   glBegin(GL_QUAD_STRIP);
      for(j=0;j<=360;j+=1){

        const float c = (j / (float) 360);

        double x = radius * Cos(j);
        double y = height;
        double z = radius * Sin(j);

        glNormal3d(Cos(j), 0, Sin(j));

        glTexCoord2f(-c, 0.0); glVertex3d(x, -y, z);
        glTexCoord2f(-c, 1.0); glVertex3d(x, y, z);
      }
   glEnd();

   glColor3f(1,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n1);
   /* Top of Cylinder */
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(0,1,0);
      glTexCoord2f(0.5,0.5);
      glVertex3d(0.0, height, 0.0);
      for(i=0;i<=360;i+=1) {
         glTexCoord2f(0.5*Cos(i)+0.5, 0.5*Sin(i)+0.5);
         glVertex3d(radius * cos(i), height, radius * sin(i));
      }
   glEnd();

   glColor3f(1,0,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,n2);
    /* Bottom of Cylinder */
   glColor3f(0.0,0.0,1.0);
   glBegin(GL_TRIANGLE_FAN);
   glNormal3d(0,-1,0);
      glTexCoord2f(0.5,0.5);
      glVertex3d(0.0, -height, 0.0);
      for(i=0;i<=360;i+=1) {
         glTexCoord2f(0.5*Cos(i)+0.5, 0.5*Sin(i)+0.5);
         glVertex3d(radius * cos(i), -height, radius * sin(i));
      }
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

//runway plane
static void runway(double x,double y, double z,double s)
{
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  //  Save transformation
  glPushMatrix();
  glScaled(s,s,s);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
  glColor3f(1,0,1);

  glColor3f(0.3,0.3,0.3);
  if (ntex) glBindTexture(GL_TEXTURE_2D,grass);
  glBegin(GL_QUADS);
  glNormal3f( 0,1, 0);
  glTexCoord2f(0,0);glVertex3f(-10,-0.151,-10);
  glTexCoord2f(1,0);glVertex3f(10,-0.151,-10);
  glTexCoord2f(1,1);glVertex3f(10,-0.151,10);
  glTexCoord2f(0,1);glVertex3f(-10,-0.151,10);
  glEnd();

  glColor3f(1,1,0);
  if (ntex) glBindTexture(GL_TEXTURE_2D,asphalt);
  glBegin(GL_QUADS);
  glNormal3f( 0,1, 0);
  glTexCoord2f(0,0);glVertex3f(-2,-0.15,-10);
  glTexCoord2f(1,0);glVertex3f(2,-0.15,-10);
  glTexCoord2f(1,1);glVertex3f(2,-0.15,10);
  glTexCoord2f(0,1);glVertex3f(-2,-0.15,10);
  glEnd();

  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}


static void tower(double x, double y, double z, double s)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glScaled(s,s,s);
  cylinder(25,10,0,5,60,5,0,paneling,paneling,paneling);
  trianglequad(21.75,20,0,7,7,7,0,window);
  trianglequad(28.75,20,0,7,7,7,180,window);
  trianglequad(25,20,3.5,7,7,7,90,window);
  trianglequad(25,20,-3.5,7,7,7,270,window);
  cylinder(25,28,0,25,5,25,0,blackconcrete,blackconcrete,blackconcrete);
  cylinder(25,32,0,3,20,2,0,antenna,paneling,paneling);
  glPopMatrix();
}

static void plane(double x, double y, double z,double s, double th)
{
  glTranslated(x,y,z);
  glRotatef(th,0,1,0);
  glScaled(s,s,s);

  glPushMatrix();
  glRotatef(180,0,0,1);
  glScaled(0.5,0.5,0.5);
  cube(0,1,0,1,2,2,0,sidepanel);
  trianglequad(-1,-1,-2,2,2,2,0,glass);
  cube(0,0,3,1,1,1,0,sidepanel);
  glPopMatrix();

  glTranslated(0,-2,0);
  glScaled(0.5,0.5,0.5);
  trianglequad(-1,1,-2,2,2,2,0,sidepanel);
  trianglequad(1,1,2,2,2,2,180,sidepanel);

  cylinder(0,5,0,1,40,6.5,270,sidepanel,redbeacon,greenbeacon);

  glPushMatrix();
  glRotatef(90,1,0,0);
  cylinder(0,8,-4,1,20,2,0,sidepanel,paneling,paneling);
  glPopMatrix();

  cylinder(1,1,2,1,5,1,45,sidepanel,paneling,paneling);
  cylinder(-1,1,2,1,5,1,135,sidepanel,paneling,paneling);
  cylinder(0,1,-2,1,5,1,180,sidepanel,paneling,paneling);
  cylinder(0,4,11.5,1,10,2,90,sidepanel,redbeacon,greenbeacon);

  cylinder(2,0,2,2,1,2,90,tread,tire,tire);
  cylinder(-2,0,2,2,1,2,90,tread,tire,tire);
  cylinder(0,0,-2,2,1,2,90,tread,tire,tire);
  cylinder(2,3,0,0.75,14,1,130,sidepanel,paneling,paneling);
  cylinder(-2,3,0,0.75,14,1,45,sidepanel,paneling,paneling);

  glPushMatrix();
  glRotatef(180,0,0,1);
  trianglequad(-0.25,-6,12,0.5,2,2,0,sidepanel);
  glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=20;  //  Length of axes
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim*Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();

   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);

   if (light)
   {

      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light direction
      float Position[]  = {75*Cos(zh),ylight,75*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);
   //draw plane
   plane(-20,0,-40,1,0);
   plane(0,0,40,2.5,25);
   plane(0,0,50,5,45);
   tower(0,0,0,1);
   runway(0,0,0,4);

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
   Print("Angle=%d,%d  Dim=%.1f Light=%s Texture=%s",th,ph,dim,light?"On":"Off",mode?"Replace":"Modulate");
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
     //  Render the scene and make it visible

   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
  //if (!fpv) {
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
  //}
   //  Update projection
   Project(fov,asp,dim);
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
  //  Reset view angle
  if (ch == '0')
     th = ph = 0;
  //  Switch display mode
  else if (ch == 'm')
     mode = 1-mode;
  // Toggle Axes
  else if (ch == 'x')
     axes = 1-axes;
  //  Change field of view angle
  else if (ch == '1')
     fov--;
  else if (ch == '2')
     fov++;
  else if (ch =='o')
     dim += 7;
  else if (ch == 'p')
     dim -= 7;
  else if (ch == 'l' || ch == 'L')
       light = 1-light;
  //  Ambient level
  else if (ch=='a' && ambient>0)
     ambient -= 5;
  else if (ch=='A' && ambient<100)
     ambient += 5;
  //  Diffuse level
  else if (ch=='d' && diffuse>0)
     diffuse -= 5;
  else if (ch=='D' && diffuse<100)
     diffuse += 5;
  //  Specular level
  else if (ch=='s' && specular>0)
     specular -= 5;
  else if (ch=='S' && specular<100)
     specular += 5;
  //  Emission level
  else if (ch=='e' && emission>0)
     emission -= 5;
  else if (ch=='E' && emission<100)
     emission += 5;
  //  Shininess level
  else if (ch=='n' && shininess>-1)
     shininess -= 1;
  else if (ch=='N' && shininess<7)
     shininess += 1;
  else if (ch == 'y')
     ylight += 5;
  else if (ch == 'u')
     ylight -= 5;
   //  Toggle textures mode
  else if (ch == 't')
     ntex = 1-ntex;
  //  Translate shininess power to value (-1 => 0)
  shiny = shininess<0 ? 0 : pow(2.0,shininess);

//}
   //  Reproject
   Project(fov,asp,dim);
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
   Project(fov,asp,dim);
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
   glutCreateWindow("HW3: Jesse Bird");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);

   sidepanel = LoadTexBMP("sidepanel.bmp");
   paneling = LoadTexBMP("concrete.bmp");
   glass = LoadTexBMP("glass.bmp");
   asphalt = LoadTexBMP("runwayview.bmp");
   grass = LoadTexBMP("felt.bmp");
   window = LoadTexBMP("window.bmp");
   antenna = LoadTexBMP("radioantenna.bmp");
   tire = LoadTexBMP("tire.bmp");
   tread = LoadTexBMP("tread.bmp");
   greenbeacon = LoadTexBMP("greenbeacon.bmp");
   redbeacon = LoadTexBMP("redbeacon.bmp");
   blackconcrete = LoadTexBMP("blackconcrete.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
