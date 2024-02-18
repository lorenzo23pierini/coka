//Critical

#include <stdio.h>
#include <omp.h>

static long num_steps=1000000000;
double step;
#define NUM_THREADS 4 //defin a constant for number of threads

void main(){
    int i, nthreads;
    double pi;//, sum[NUM_THREADS]; 
                            
    step=1.0/(double) num_steps;

    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        int i,ID, nthrds;
        double x;

        double sum; //Create a scalar local to each thread to accumulate partial sums.
        ID = omp_get_thread_num(); 
        nthrds = omp_get_num_threads(); 

        if(ID==0)
            nthreads=nthrds; //Only one thread should copy the number of threads to the global value to make sure multiple threads writing to the same address don't conflict
        
        for(i=ID, sum=0.0; i<num_steps; i=i+nthrds){
            x=(i+0.5)*step;
            //#pragma omp critical //DA NON FARE MAI NON RIESCE A PRENDERE DATI PER SUM
            sum+=4.0/(1.0+x*x); //Partial sum
        }
        sum=sum*step;
    #pragma atomic//Atomic
         pi+=sum; //Sum goes "out of scope" beyond the parallel region... so you must sum it in here.
                  //Must protect summation into pi so updates don't conflict
    }
    printf("The result is: %f\n", pi);
}

