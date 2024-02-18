//SPMD alghoritm strategy
//The ideas is I have a single program and I run a multiple copies of this same program

#include <stdio.h>
#include <omp.h>

/*
To create a parallel version of the pi program using parallel construct, pay close attention to shared versus private variables.
In addition to a parallel construct, you will need the runtime library routines
-int omp_get_num_threads(); -> Number of threads in the team
-int omp_get_thread_num(); -> Thread ID or rank
-double omp_get_wtime(); -> time in Seconds since a fixed point in the past 
*/

static long num_steps=1000000000;
double step;
#define PAD 8 //assume 4 byte L1 cache line size
#define NUM_THREADS 8 //defin a constant for dim of the array

void main(){
    int i, nthreads;
    double pi, sum[NUM_THREADS][PAD]; //Promote scalar to an array dimensioned by number of threads to avoid race condition
                                      //Pad the array so each sum values is in a different cache line
    step=1.0/(double) num_steps;

    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        int i,ID, nthrds;
        double x;
        ID = omp_get_thread_num(); //ID del thread
        nthrds = omp_get_num_threads(); //number of threads in the gang

        if(ID==0)
            nthreads=nthrds; //Only one thread should copy the number of threads to the global value to make sure multiple threads writing to the same address don't conflict
        
        for(i=ID, sum[ID][0]=0.0; i<num_steps; i=i+nthrds){
            x=(i+0.5)*step;
            sum[ID][0]+=4.0/(1.0+x*x); //This is a common trick in SPMD programs to create a cyclic distribution of loop iterations
        }
    }
    for(i=0, pi=0.0; i<nthreads;i++)
        pi+=sum[i][0]*step;

    printf("The result is: %f\n", pi);
}

/*
Il problema che si riscontra in questa situazione è di false sharing. Ovvero aumentando il numero dei threads il tempo di esecuzione aumenta, invece che diminuire.
Questo è dovuto al fatto che la memoria della cache occupata dai vari threads non viene suddivisa in maniera allineata nei suoi livelli.
Quindi è necessario tempo per passare da un livello a un altro.
Questo problema di allineamento non consente di guadagnare tempo programmando in parallelo su CPU.

Quindi quello che si fa è andare a riempire/ imbottire PAD gli array in maniera tale da ottenere una lunghezza congeniale alla cache L1. (64 KB)
Si utilizza una matrice sum[NUM_THREADS][PAD]; e si riempiono gli array solo con componente sum[ID][0].
*/

/*
Il termine "false sharing" si riferisce a una situazione in cui più thread o processori condividono la stessa area di memoria, ma ognuno di essi modifica dati separati all'interno di quella area. 
Questo può portare a un degrado delle prestazioni a causa della competizione per la coerenza della cache, anche se i dati stessi non sono realmente condivisi.
Nel contesto del parallelismo e della programmazione concorrente, i moderni sistemi hardware utilizzano spesso una gerarchia di cache per migliorare le prestazioni. 
Quando più thread accedono a dati condivisi, la coerenza della cache diventa cruciale. 
Il problema del false sharing si presenta quando due o più thread modificano dati che risiedono nello stesso blocco di cache, anche se i dati specifici che stanno modificando sono diversi.
*/

/*
La "coerenza della cache" si riferisce alla consistenza dei dati tra le diverse memorie cache in un sistema multiprocessore. 
In un ambiente multiprocessore, ogni processore ha la sua cache locale che memorizza temporaneamente i dati frequentemente utilizzati. 
La coerenza della cache è fondamentale per garantire che tutti i processori visualizzino una visione coerente e aggiornata della memoria condivisa.

Quando più processori accedono e modificano gli stessi dati nella memoria principale, è possibile che i dati nelle cache dei vari processori diventino non coerenti. Ciò può portare a risultati imprevedibili e comportamenti indesiderati del programma. La coerenza della cache gestisce questo problema attraverso vari meccanismi:

1. Protocolli di Coerenza della Cache:
I sistemi multiprocessore implementano protocolli di coerenza della cache per definire le regole e le politiche secondo cui le cache devono essere aggiornate e coordinate. 
Alcuni protocolli comuni includono MESI (Modificato, Esclusivo, Condiviso, Invalido) e MOESI (Modificato, Proprio, Esclusivo, Condiviso, Invalido).

2. Operazioni di Controllo:
Le operazioni di controllo della coerenza della cache, come la lettura e la scrittura, sono gestite in modo da garantire che tutti i processori visualizzino i dati aggiornati. Ciò può comportare la trasmissione di segnali di invalidazione o aggiornamento tra le cache.

3. Scrittura Retroattiva e Scrittura Diretta:
Le politiche di scrittura retroattiva e scrittura diretta determinano quando una cache scrive i dati modificati nella memoria principale. Con la scrittura retroattiva, i dati vengono scritti solo quando una linea di cache viene sostituita, mentre con la scrittura diretta i dati vengono scritti immediatamente nella memoria principale.

4.Allineamento di Memoria:
L'allineamento della memoria è utilizzato per garantire che i dati inizino in posizioni di memoria che sono multipli interi del numero di byte della cache. Questo può ridurre il rischio di false condivisioni e migliorare la coerenza della cache.

5. Barriere di Memoria e Sincronizzazione:
L'uso di barriere di memoria e istruzioni di sincronizzazione può essere impiegato per coordinare l'accesso e la sincronizzazione tra i processori, garantendo una visione coerente della memoria.
La gestione della coerenza della cache è essenziale per garantire che un programma parallelo in un ambiente multiprocessore produca risultati corretti e prevedibili. Gli sviluppatori devono essere consapevoli di questi meccanismi e considerarli durante la progettazione di applicazioni parallele.
*/



