#include <ctype.h>

FILE* f = fopen(file,"imu.txt");
if (!f)
{
   fprintf(stderr,"Cannot open material file %s\n",file);
   return;
}

while ((readline(f)))
{
  
}
