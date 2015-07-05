/*
Modified from: http://rosettacode.org/wiki/Mandelbrot_set#PPM_non_interactive

c program:
--------------------------------
1. draws Mandelbrot set for Fc(z)=z*z +c
using Mandelbrot algorithm ( boolean escape time )
-------------------------------
2. technique of creating ppm file is  based on the code of Claudio Rocchini
http://en.wikipedia.org/wiki/Image:Color_complex_plot.jpg
create 24 bit color graphic file ,  portable pixmap file = PPM
see http://en.wikipedia.org/wiki/Portable_pixmap
to see the file use external application ( graphic viewer)
*/
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main(){
 
    clock_t start, end;
    double cpu_time_used;
    /* screen ( integer) coordinate */
    int iX,iY;
    const int iXmax = 16384;
    const int iYmax = 16384;
    /* world ( double) coordinate = parameter plane*/
    double Cx,Cy;
    const double CxMin=-2.5;
    const double CxMax=1.5;
    const double CyMin=-2.0;
    const double CyMax=2.0;
    /* */
    double PixelWidth=(CxMax-CxMin)/iXmax;
    double PixelHeight=(CyMax-CyMin)/iYmax;
    /* color component ( R or G or B) is coded from 0 to 255 */
    /* it is 24 bit color RGB file */
    const int MaxColorComponentValue=255;
    FILE * fp;
    char *filename="_original.ppm";
    static unsigned char color[3];
    static unsigned char vetor[49152];
    int indice;
    int i;
    /* Z=Zx+Zy*i  ;   Z0 = 0 */
    double Zx, Zy;
    double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
    /*  */
    int Iteration;
    const int IterationMax=256;
    /* bail-out value , radius of circle ;  */
    const double EscapeRadius=2;
    double ER2=EscapeRadius*EscapeRadius;
    /*create new file,give it a name and open it in binary mode  */
    fp= fopen(filename,"wb"); /* b -  binary mode */
    /*write ASCII header to the file*/
    fprintf(fp,"P6\n %d\n %d\n %d\n",iXmax,iYmax,MaxColorComponentValue);
    /* compute and write image data bytes to the file*/
    start = clock();
		
    for(iY=0;iY<iYmax;iY++)
    {
	   Cy=CyMin + iY*PixelHeight;
	   if (fabs(Cy)< PixelHeight/2) 
		  Cy=0.0; /* Main antenna */
	   indice = 0;
	   #pragma omp parallel for private(iX, Iteration)
	   for(iX=0;iX<iXmax;iX++)
	   {
		  Cx=CxMin + iX*PixelWidth;
		  /* initial value of orbit = critical point Z= 0 */
		  Zx=0.0;
		  Zy=0.0;
		  Zx2=Zx*Zx;
		  Zy2=Zy*Zy;
		  /* */

		  for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
		  {
			 Zy=2*Zx*Zy + Cy;
			 Zx=Zx2-Zy2 +Cx;
			 Zx2=Zx*Zx;
			 Zy2=Zy*Zy;
		  };
		  /* compute  pixel color (24 bit = 3 bytes) */
		  if (Iteration==IterationMax)
		  { /*  interior of Mandelbrot set = black */
			 vetor[indice]=0;
			 vetor[indice + 1]=0;
			 vetor[indice + 2]=0;
		  }
		  else
		  { /* exterior of Mandelbrot set = white */
			 vetor[indice]=((IterationMax-Iteration) % 8) *  63;  /* Red */
			 vetor[indice + 1]=((IterationMax-Iteration) % 4) * 127;  /* Green */
			 vetor[indice + 2]=((IterationMax-Iteration) % 2) * 255;  /* Blue */
		  };
		  //fwrite(color,1,3,fp);
		  indice = indice + 3;
	   }
	   indice = 0;
	   for(i =0; i< iXmax; i++){
		  color[0] = vetor[indice];
		  color[1] = vetor[indice + 1];
		  color[2] = vetor[indice + 2];

		  fwrite(color,1,3,fp);
	   }
		   
    }
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time = %f seconds\n", cpu_time_used);
    fclose(fp);
    return 0;
}
