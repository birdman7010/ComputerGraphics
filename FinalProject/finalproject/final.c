
#include "CSCIx229.h"
int axes=1;       //  Display axes
int mode=1;       //  Shader mode
int move=1;       //  Move light
int roll=1;       //  Rolling brick texture
int proj=1;       //  Projection type
int obj=0;        //  Object
int th=275;         //  Azimuth of view angle
int ph=25;         //  Elevation of view angle
int fov=100;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=10.0;   //  Size of world
int zh=90;        //  Light azimuth
int lab=0;       //  Object
int wheel=0;
int car=0;
float ylight=0.5;   //  Light elevation
#define MODE 3
int shader[MODE] = {0,0,0}; //  Shader programs
char* text[] = {"No Shader","Colored Heightmap","Lighting"};
float X=0,Y=0,Z=1;
int xh = 1;
int light = 1;
int spin = 0;
int fh = 1;
unsigned int wall = 0;
double xd,yd,zd,td;

//first person camera
int fpv;
double xfpv=0.0;    //forward/backward
double yfpv =3.0;   //up/down
double zfpv =7.0;  //forward/backward
int cameraRotation = 0.0;
double Cx = 0.0;
double Cy = 0.0;
double Cz = 0.0;
double scaling = 0.1;
float _angle = 0.0;

static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   // first person view
   if (fpv) {
     gluPerspective(fov,asp,dim/4,4*dim);
   }
   else
   {
     //  Perspective transformation
      gluPerspective(fov,asp,dim/4,4*dim);
    }
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

static void wheelRender(double x,double y,double z,
                 double s,double th)
{
    glPushMatrix();
    glTranslated(x,y,z);
    glRotatef(xh,0.0,0.0,1.0);
    glRotatef(th,0,1,0);
    glScaled(s,s,s);
    glCallList(wheel);
    glPopMatrix();
}
static void carRender(double x,double y,double z,
                 double s)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(s,s,s);
   glCallList(car);
   glPopMatrix();
}

static void labRender(double x,double y,double z,
                 double s,double th)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glScaled(s,s,s);
  glCallList(lab);
  glPopMatrix();
}

static void carBody(double x,double y,double z,
                 double s, double th, double zh)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glRotated(zh,0,0,1);
  glScaled(s,s,s);
  carRender(0,0,0,1);
  wheelRender(0.477,-0.03,0.495,1,180);
  wheelRender(-0.481,-0.03,0.495,1,180);
  wheelRender(0.481,-0.03,-0.495,1,0);
  wheelRender(-0.481,-0.03,-0.495,1,0);
  glPopMatrix();
}

static void carTranslate (double xd, double yd, double zd, double td)
{
  glPushMatrix();
  if (xd < 5 && zd < 4.5 && xd > -4 && zd > -8)
  {
  glTranslated(xd,yd,zd);
  glRotated(td,0,1,0);
  carBody(1+xd,0.128,3+zd,1,td,0);
  }
  else {
    glWindowPos2i(5,100);
    Print("You have crashed into a wall");
  }
  glPopMatrix();
}

static void box (double x, double y, double z, double s)
{
  int emission = 0;
  float shiny = 0.0;
  float white[] = {1,1,1,1};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
  //  Save transformation
  glPushMatrix();
  glScaled(s,s,s);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glColor3f(1,0,0);
  glBindTexture(GL_TEXTURE_2D, wall);
  glBegin(GL_QUADS);
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0);glVertex3f(-14,10,-13.0);
  glTexCoord2f(1,0);glVertex3f(11,10,-13.0);
  glTexCoord2f(1,1);glVertex3f(11,10,13.0);
  glTexCoord2f(0,1);glVertex3f(-14,10,13.0);
  glEnd();
  glBegin(GL_QUADS);
  glNormal3f(0,0,-1);
  glTexCoord2f(0,0);glVertex3f(-14,0,13.0);
  glTexCoord2f(1,0);glVertex3f(11,0,13.0);
  glTexCoord2f(1,1);glVertex3f(11,10,13.0);
  glTexCoord2f(0,1);glVertex3f(-14,10,13.0);
  glEnd();
  glBegin(GL_QUADS);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0);glVertex3f(-14,0,-13.0);
  glTexCoord2f(1,0);glVertex3f(11,0,-13.0);
  glTexCoord2f(1,1);glVertex3f(11,10,-13.0);
  glTexCoord2f(0,1);glVertex3f(-14,10,-13.0);
  glEnd();
  glBegin(GL_QUADS);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0);glVertex3f(-14,0,-13.0);
  glTexCoord2f(1,0);glVertex3f(-14,0,13.0);
  glTexCoord2f(1,1);glVertex3f(-14,10,13.0);
  glTexCoord2f(0,1);glVertex3f(-14,10,-13.0);
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
   //  Light position and colors
   float Emission[]  = {0.0,0.0,0.0,1.0};
   float Ambient[]   = {0.3,0.3,0.3,1.0};
   float Diffuse[]   = {1.0,1.0,1.0,1.0};
   float Specular[]  = {1.0,1.0,1.0,1.0};
   float Position[]  = {2*Cos(zh),ylight,2*Sin(zh),1.0};
   float Shinyness[] = {16};

   glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
	 glShadeModel(GL_SMOOTH);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (fpv)
   //Using First person view and adjusting cameraRotation
   {
     Cx = +2*dim*Sin(cameraRotation);
     Cz = -2*dim*Cos(cameraRotation);
     gluLookAt(xfpv,yfpv,zfpv,Cx+xfpv,yfpv,Cz+zfpv,0,1,0);
   }
   else
   {
     //  Perspective - set eye position
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim*Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez,0,0,0,0,Cos(ph),0);

    }
   //  Draw light position as sphere (still no lighting here)
   glPushMatrix();
   glTranslated(Position[0],Position[1],Position[2]);
   glutSolidSphere(0.03,10,10);
   glPopMatrix();
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
   //  Set materials
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

   glUseProgram(shader[mode]);

   if (mode>0)
   {
      int id;
      float time = roll ? 0.001*glutGet(GLUT_ELAPSED_TIME) : 0;
      id = glGetUniformLocation(shader[mode],"Xcenter");
      if (id>=0) glUniform1f(id,X);
      id = glGetUniformLocation(shader[mode],"Ycenter");
      if (id>=0) glUniform1f(id,Y);
      id = glGetUniformLocation(shader[mode],"Zoom");
      if (id>=0) glUniform1f(id,Z);
      id = glGetUniformLocation(shader[mode],"time");
      if (id>=0) glUniform1f(id,time);
   }

   if (obj == 1)
   {
     labRender(0,0,0,1,270);
   }
   else if (obj == 3)
   {
     wheelRender(0,0,0,3,180);
   }
   else if (obj == 2)
   {
     carRender(0,0,0,1);
   }
   else if (obj==0)
   {
     carTranslate(xd,yd,zd,td);
     labRender(0,0,0,3,270);
     box(0,0,0,1);
   }
   glUseProgram(0);
   glDisable(GL_LIGHTING);
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
   if (!fpv)
   {
     glWindowPos2i(5,45);
     Print("Angle=%d,%d  Dim=%.1f Light Elevation=%.1f",th,ph,dim,ylight);
     glWindowPos2i(5,25);
     Print("Projection=%s Mode=%s First Person = Off",proj?"Perspective":"Orthogonal",text[mode]);
     glWindowPos2i(5,5);
     Print("Locational IMU Data=%.1f,%.1f,%.1f",xd,zd,td);
   }
   else
   {
     glWindowPos2i(5,5);
     Print("Angle=%d,%d  Dim=%.1f Light Elevation=%.1f First Person = On",
       th,ph,dim,ylight);
   }
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if (move) {
     zh = fmod(90*t,360.0);
   }
   if (spin)
   {
     xh = fmod(-90*t,360.0);
     //fh = fmod(30*t,360.0);
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
  if(!fpv)
  {
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
  else if (ch == 'f')
  {
    fpv = 1-fpv;
  }

  else if (fpv) {
    if (ch == 'w'){
       xfpv = xd + xfpv;
       zfpv = zd + zfpv;
       xfpv += Cx*scaling;
       zfpv += Cz*scaling;
    }
    else if (ch == 'a'){
       cameraRotation -= 2;
    }
    else if (ch == 's'){
       xfpv -= Cx*scaling;
       zfpv -= Cz*scaling;
    }
    else if (ch == 'd'){
       cameraRotation += 2;
    }
    else if (ch == 'e')
    {
      yfpv += 1;
    }
    else if (ch == 'q')
    {
      yfpv -= 1;
    }
    else if (ch == 'p')
       proj = 1-proj;

    //  Keep angles to +/-360 degrees
    cameraRotation %= 360;
   }
   else {
   //  Reset view angle
   if (ch == '0')
      {th = 275;
      ph = 25;
      dim = 10;}
   //  Toggle axes
   else if (ch == 'x')
      axes = 1-axes;
   //  Toggle light movement
   else if (ch == 'l')
      move = 1-move;
   //  Toggle objects
   else if (ch == 'o')
      obj = (obj+1)%4;
   //  Cycle modes
   else if (ch == 'm')
      mode = (mode+1)%MODE;
   //  Light elevation
   else if (ch == 'u')
      ylight += 0.5;
   else if (ch == 'y')
      ylight -= 0.5;
   else if (ch == '+')
      dim -=1;
   else if (ch == '-')
      dim +=1;
   else if (ch == 'i')
      spin = 1-spin;
   if (ch == 'w'){
     xd-=0.3;
   }
   else if (ch == 'a'){
     zd+=0.3;
   }
   else if (ch == 's'){
     xd+=0.3;
   }
   else if (ch == 'd'){
     zd-=0.3;
   }
   else if (ch =='q')
     td += 5;
   else if (ch == 'e')
     td-=5;
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
 *  Read text file
 */
char* ReadText(char *file)
{
   int   n;
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Print Shader Log
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

/*
 *  Create Shader
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER  ,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
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
   glutInitWindowSize(1000,700);
   glutCreateWindow("Final Project: Jesse Bird");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load object
   lab = LoadOBJ("lab.obj");
   car = LoadOBJ("car.obj");
   wheel = LoadOBJ("wheel.obj");
   wall = LoadTexBMP("blackconcrete.bmp");
   //  Create Shader Programs
   shader[2] = CreateShaderProg("phong.vert","phong.frag"); //lighting
   shader[1] = CreateShaderProg("mesh.vert","mesh.frag");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
