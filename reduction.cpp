#include <iostream>
#include <omp.h>

int main() {
    int total_sum = 0;

    #pragma omp parallel for reduction(+:total_sum)
    for (int i = 0; i < 10; ++i) {
        total_sum += i;
    }

    std::cout << "Total Sum: " << total_sum << std::endl;

    return 0;
}

//In questo esempio, total_sum è una variabile di riduzione che accumula la somma dei valori.
//La direttiva reduction(+:total_sum) specifica che ogni thread avrà una copia locale di total_sum, e alla fine dell'esecuzione parallela, tutte le copie locali verranno sommate in modo appropriato per ottenere il risultato totale.
//L'utilizzo di variabili di riduzione è un approccio efficace per migliorare le prestazioni in HPC quando è necessario aggregare dati da più thread o processi in modo efficiente.





