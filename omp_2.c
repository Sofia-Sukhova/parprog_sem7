#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#define ISIZE 1000
#define JSIZE 1000


int main(int argc, char **argv)
{

FILE *ff;
double time_start, time_end;
int i, j;

double ** a;
double ** b;

a = calloc(ISIZE, sizeof(double *));
for (int i = 0; i < ISIZE; i++){
    a[i] = calloc(JSIZE, sizeof(double));
}

b = calloc(ISIZE, sizeof(double *));
for (int i = 0; i < ISIZE; i++){
    b[i] = calloc(JSIZE, sizeof(double));
}

for (i=0; i<ISIZE; i++){
    for (j=0; j<JSIZE; j++){
        a[i][j] = 10*i +j;
        b[i][j] = a[i][j];
    }
}

int * iter;
iter = calloc(atoi(argv[1]), sizeof(int));

//начало измерения времени
time_start = (double) omp_get_wtime();

#pragma omp parallel num_threads(atoi(argv[1])){
    #pragma omp for collapse(2) dynamic
        for (j = 6; j < JSIZE; j++){
            for (int i = 0; i < ISIZE - 1; i++){  
                a[i][j] = sin(0.2*a[i + 1][j - 6]);
                //for debug purposes
                //int thread = omp_get_thread_num();
                //iter[thread] ++;
            }
    #pragma omp ordered depend(source)
        }

        //окончание измерения времени
        time_end = (double) omp_get_wtime();
        printf("%f\n", (time_end - time_start));

    #pragma omp single {
        ff = fopen("omp.txt","w");
        for(int i= 0; i < ISIZE; i++){
            for (int j= 0; j < JSIZE; j++){
                fprintf(ff,"%f ",a[i][j]);
            }
            fprintf(ff,"\n");
        }
        fclose(ff);
    }
}
//for debug purposes
// for (int i = 0; i < atoi(argv[1]); i++){
//     printf("%d ", iter[i]);
// }

for (int i = 0; i < ISIZE; i++){
    free(a[i]);
}
free(a);

return 0;
}












