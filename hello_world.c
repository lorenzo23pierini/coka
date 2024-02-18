#include <omp.h> //OpenMP include file
#include <stdio.h>

int main(){
    #pragma omp parallel //Parallel region with default number of threads
    {
        int ID=omp_get_thread_num(); //runtime library function to return a thread ID

        printf("hello(%d)", ID);
        printf("world(%d)\n", ID);
    }//end of the Parallel region   
}