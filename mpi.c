#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>
#define ISIZE 1000
#define JSIZE 1000
// #define serial
#define parallel
#define T 0.012


int main(int argc, char **argv)
{

double a[ISIZE][JSIZE];
int i, j;
FILE *ff;
double time_start, time_end;

#ifdef serial

for (i=0; i<ISIZE; i++){
    for (j=0; j<JSIZE; j++){
        a[i][j] = 10*i +j;
    }
}
//начало измерения времени

time_start = (double) clock();
for (i=8; i<ISIZE; i++){
    for (j = 0; j < JSIZE-3; j++){  
        a[i][j] = sin(4*a[i-8][j+3]);

    }
}

//окончание измерения времени
time_end = (double) clock();
printf("time serial: %f\n", (time_end - time_start)/CLOCKS_PER_SEC);

ff = fopen("result.txt","w");
for(i= 0; i < ISIZE; i++){
    for (j= 0; j < JSIZE; j++){
        fprintf(ff,"%f ",a[i][j]);
    }
    fprintf(ff,"\n");
    }
fclose(ff);

#endif

#ifdef parallel

for (i=0; i<ISIZE; i++){
    for (j=0; j<JSIZE; j++){
        a[i][j] = 10*i +j;
    }
}
//начало измерения времени
int rank;
int size;

MPI_Init(&argc, &argv);
time_start = MPI_Wtime();
MPI_Comm_size (MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Request request;
    MPI_Status status;
    int period = 8 * ((size - 1) / 8 + 1);
    //have sense only for size > 8
    int gap = period - size;


        for(int i = 8 + rank; i < ISIZE; i ++){
            if ((i - 8) % period % size != rank){
                continue;
            }
            if((i > 15) && (size > 8)){
                int src = 0;
                if ((i - 8) % period >= size) {
                    src = (i - 8) % period - 8;
                } else {
                    if ((i - 8) % period < 8) {
                        src = (rank + period - 8) % size;
                    }
                    else {
                        src = (rank + size - 8) % size;
                    }
                }
                // printf ("rank %d, src %d\n", rank, src);
                if (src != rank){
                    MPI_Recv(&a[i - 8], JSIZE - 3, MPI_DOUBLE, src, (i - 8) + ISIZE, MPI_COMM_WORLD, &status);
                }
            }
            for (j = 0; j < JSIZE - 3; j ++){
                a[i][j] = sin(4*a[i-8][j+3]);
            }
            if ((i + 8 < ISIZE) && (size > 8)){
                int dst = 0;
                if ((i - 8) % period >= size) {
                    dst = rank + (8 - gap);
                } else {
                    if (rank >= period - 8){
                        dst = (rank + 8 - gap) % size;
                    } else {
                        dst = (rank + 8) % size;
                    }
                }
                if (dst != rank){
                    MPI_Isend(&a[i], JSIZE - 3, MPI_DOUBLE, dst, i + ISIZE, MPI_COMM_WORLD, &request);
                }

            }
        }


if (rank != 0){
    for(int i = 8 + rank; i < ISIZE; i ++){
        if ((i - 8) % period % size == rank){
            MPI_Send(&a[i], JSIZE - 3, MPI_DOUBLE, 0, i, MPI_COMM_WORLD);
        }
    }
}

if (rank == 0){  
        for(int i = 9; i < ISIZE; i ++){
                if ((i - 8) % period % size != rank){
                    MPI_Recv(&a[i], JSIZE - 3, MPI_DOUBLE, (i - 8) % period  % size, i, MPI_COMM_WORLD, &status);
                }
        }
        time_end = MPI_Wtime();
        printf("%f",(time_end - time_start));

        ff = fopen("mpi.txt","w");
        for(i= 0; i < ISIZE; i++){
            for (j= 0; j < JSIZE; j++){
                fprintf(ff,"%f ",a[i][j]);
            }
            fprintf(ff,"\n");
        }
        
        
        fclose(ff); 

    }

MPI_Finalize();
#endif


return 0;
}
