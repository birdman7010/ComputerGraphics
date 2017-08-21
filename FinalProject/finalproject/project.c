
#include "CSCIx229.h"
int fpv;
void Project(double fov,double asp,double dim)
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
    if (fov)
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
