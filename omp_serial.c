#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#define ISIZE 1000
#define JSIZE 1000
#define serial
#define parallel
// #define T 0.012


int main(int argc, char **argv)
{

double** a;
a = calloc(ISIZE, sizeof(double *));
for (int i = 0; i < ISIZE; i++){
    a[i] = calloc(JSIZE, sizeof(double));
}
FILE *ff;
double time_start, time_end;
int i, j;

#ifdef serial

    for (i=0; i<ISIZE; i++){
        for (j=0; j<JSIZE; j++){
            a[i][j] = 10*i +j;
        }
    }
    //начало измерения времени

    time_start = (double) clock();
    for (i = 0; i<ISIZE - 1; i++){
        for (j = 6; j < JSIZE; j++){  
            a[i][j] = sin(0.2*a[i + 1][j - 6]);

        }
    }

    //окончание измерения времени
    time_end = (double) clock();
    printf("time serial: %f\n", (time_end - time_start)/CLOCKS_PER_SEC);

    ff = fopen("result.txt","w");
    for(i = 0; i < ISIZE; i++){
        for (j= 0; j < JSIZE; j++){
            fprintf(ff,"%f ",a[i][j]);
        }
        fprintf(ff,"\n");
        }
    fclose(ff);

#endif /*Serial calculations*/

for (int i = 0; i < ISIZE; i++){
    free(a[i]);
}
free(a);

return 0;
}












