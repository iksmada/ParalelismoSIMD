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

int main()
{
		clock_t start, end;
		double cpu_time_used;
        /* screen ( integer) coordinate */
		int endOfLoop[8],endOfLoopBp[8]={-1,-1,-1,-1,-1,-1,-1,-1};
        int iX,iY,i;
        const int iXmax = 16384; 
        const int iYmax = 16384;
        /* world ( double) coordinate = parameter plane*/
        double Cx,Cy;
		double Cxvet[4],Cyvet[4];
        const double CxMin=-2.5;
        const double CxMax=1.5;
        const double CyMin=-2.0;
        const double CyMax=2.0;
		const double vet2[4]={2.0,2.0,2.0,2.0}; 
        /* */
        double PixelWidth=(CxMax-CxMin)/iXmax;
        double PixelHeight=(CyMax-CyMin)/iYmax;
        /* color component ( R or G or B) is coded from 0 to 255 */
        /* it is 24 bit color RGB file */
        const int MaxColorComponentValue=255; 
        FILE * fp;
        char *filename="_simd_avx.ppm";
        static unsigned char color[3];
        /* Z=Zx+Zy*i  ;   Z0 = 0 */
        double Zxbackup[4],Zx[4], Zybackup[4],Zy[4];
        double Zx2backup[4],Zx2[4], Zy2backup[4],Zy2[4]; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
        /*  */
        int Iteration;
        const int IterationMax=256;
        /* bail-out value , radius of circle ;  */
        const double EscapeRadius=2;
        double ER2=EscapeRadius*EscapeRadius;
		double ER2vet[4]={ER2,ER2,ER2,ER2};
        /*create new file,give it a name and open it in binary mode  */
        fp= fopen(filename,"wb"); /* b -  binary mode */
        /*write ASCII header to the file*/
        fprintf(fp,"P6\n %d\n %d\n %d\n",iXmax,iYmax,MaxColorComponentValue);
        /* compute and write image data bytes to the file*/
		start = clock();
        for(iY=0;iY<iYmax;iY++)
        {
			 for(i=0;i<4;i++){
				Cyvet[i]=CyMin + (iY+i)*PixelHeight;
				if (fabs(Cyvet[i])< PixelHeight/2) Cyvet[i]=0.0; /* Main antenna */
			 }

             for(iX=0;iX<iXmax;iX=iX+4)
             {         
                        /* initial value of orbit = critical point Z= 0 */
						for(i=0;i<4;i++){
						endOfLoopBp[2*i]=-1;
						endOfLoop[2*i]=-1;
						Cxvet[i]=CxMin + (iX+i)*PixelWidth;
                        Zx[i]=0.0;
                        Zy[i]=0.0;
                        Zx2[i]=Zx[i]*Zx[i]; //falta paralelizar
                        Zy2[i]=Zy[i]*Zy[i];	// falta paralelizar

						}
                        /* */
                        for (Iteration=0;Iteration<IterationMax &&( endOfLoop[0]!=0 
																||  endOfLoop[2]!=0
																||  endOfLoop[4]!=0 
																||  endOfLoop[6]!=0 )/*((Zx2+Zy2)<ER2)*/ 
																;Iteration++)
                        {

							__asm {
								//vmovupd ymm1, Zx[0]
								vmovupd ymm2, Zy[0] //coloca 4 double no ymm2 
								vmulpd ymm1,ymm2,Zx[0] // multiplica ymm2*4 double no zx
								vmovaps ymm2,ymm1 //coloca no ymm2 o ymm1
								vmulpd ymm1,ymm2,vet2[0] // multiplica por 2
								vmovaps ymm2,ymm1
								vaddpd ymm1,ymm2,Cyvet[0]
								vmovupd Zy[0],ymm1


								// atribui Zx
								vmovupd ymm2, Zx2[0] //coloca 4 double no ymm2 
								vsubpd ymm1,ymm2,Zy2[0] // multiplica ymm2*4 double no zx
								vmovaps ymm2,ymm1 //coloca no ymm2 o ymm1
								vaddpd ymm1,ymm2,Cxvet[0]
								vmovupd Zx[0],ymm1

								// atribui Zx2
								vmovupd ymm2, Zx[0] //coloca 4 double no ymm2 
								vmulpd ymm1,ymm2,Zx[0] // multiplica ymm2*4 double no zx
								vmovupd Zx2[0],ymm1

								// atribui Zy2
								vmovupd ymm2, Zy[0] //coloca 4 double no ymm2 
								vmulpd ymm1,ymm2,Zy[0] // multiplica ymm2*4 double no zx
								vmovupd Zy2[0],ymm1

								//fazer  comparacao ((Zx2+Zy2)<ER2) e atribuir endOfLoop
								vmovupd ymm2, Zx2[0] //coloca 4 double no ymm2 
								vaddpd ymm1,ymm2,Zy2[0] // multiplica ymm2*4 double no zx
								vmovaps ymm2,ymm1 //coloca no ymm2 o ymm1
								vmovupd ymm3,ER2vet[0]
								vcmpltpd ymm1,ymm2,ymm3 // compara com LT , para setar para less than
								vmovupd endOfLoop[0],ymm1  // salva 0 ou 1 no vetor de int

							}

							//salvando valor ou nao dependendo se ja atingiu condicao de parada anteriormente e agr
							for(i=0;i<4;i++){
							if(endOfLoopBp[2*i]==0){
								endOfLoop[2*i]=0;
								 //Zy[i]= Zybackup[i];
								 //Zx[i]= Zxbackup[i];
								//Zx2[i]=Zx2backup[i];
								//Zy2[i]=Zy2backup[i];
								
							}
							else if(endOfLoop[2*i]==0){
									endOfLoopBp[2*i]=0;
									endOfLoopBp[2*i+1]=Iteration+1;
									 //Zybackup[i]= Zy[i];
									 //Zxbackup[i]= Zx[i];
									//Zx2backup[i]=Zx2[i];
									//Zy2backup[i]=Zy2[i];
								}
							}
							
                            //Zy=2*Zx*Zy + Cy;
                            //Zx=Zx2-Zy2 +Cx;
                            //Zx2=Zx*Zx;
                            //Zy2=Zy*Zy;

                        };
                        /* compute  pixel color (24 bit = 3 bytes) */


						for(i=0;i<4;i++){
							if (endOfLoopBp[2*i]!=0)
							{ /*  interior of Mandelbrot set = black */
								color[0]=0;
								color[1]=0;
								color[2]=0;                           
							}
							else 
							{ /* exterior of Mandelbrot set = white */ //falta paralelizar
								color[0]=((IterationMax-endOfLoopBp[2*i+1]) % 8) *  63;  /* Red */ 
								color[1]=((IterationMax-endOfLoopBp[2*i+1]) % 4) * 127;  /* Green */ 
								color[2]=((IterationMax-endOfLoopBp[2*i+1]) % 2) * 255;  /* Blue */
							}
						
                        /*write color to the file*/
							fwrite(color,1,3,fp);
						}
						
                }
        }
		end = clock();
        fclose(fp);

		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("time = %f seconds\n", cpu_time_used);
        return 0;
}

