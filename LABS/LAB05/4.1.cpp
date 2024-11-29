#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>

int* HISTOGRAM;  //Memoria para ARRAY del Histograma.
int N_intervalo;
int N_threads;
int N_datos;
double v_min;
double v_max;
std::vector<double> Vec_Datos;
pthread_mutex_t mutex;

void* CreandoHisto(void* rank) {
    long my_rank = (long)rank; //Identificador de thread.
    int interva_local;
    double size_intervalo = (v_max - v_min) / N_intervalo;

    std::cout << "Hilo " << my_rank << " ha iniciado." << std::endl;  // Mensaje de inicio

    for (int i = my_rank; i < N_datos; i += N_threads) {
        interva_local = (int)((Vec_Datos[i] - v_min) / size_intervalo);
        if (interva_local >= N_intervalo)
        {
            interva_local = N_intervalo - 1;
        }else{}

        pthread_mutex_lock(&mutex);     //bloquea el mutex
        HISTOGRAM[interva_local]++;
        pthread_mutex_unlock(&mutex);   //desbloquea el mutex
    }

    std::cout << "Hilo " << my_rank << " ha terminado." << std::endl;  // Mensaje de fin
    return NULL;
}

int main(int argc, char* argv[])        //Argumentos: N_hilos c_datos n_interv min max
{
    N_threads = strtol(argv[1], NULL, 10);
    N_datos = strtol(argv[2], NULL, 10);
    N_intervalo = strtol(argv[3], NULL, 10);
    v_min = atof(argv[4]);
    v_max = atof(argv[5]);

    HISTOGRAM = new int[N_intervalo]();
    pthread_t* thread_handles = new pthread_t[N_threads];       //array que identifique los hilos.
    pthread_mutex_init(&mutex, NULL);       //se incializa el mutex.

    for (int i = 0; i < N_datos; i++)           //generacion de datos aleatorios.
    {
        Vec_Datos.push_back(v_min + ((double)rand() / RAND_MAX) * (v_max - v_min));
    }

    for (int thread = 0; thread < N_threads; thread++)      //creación de threads.
    {
        pthread_create(&thread_handles[thread], NULL, CreandoHisto, (void*)(long)thread);
    }

    for (int thread = 0; thread < N_threads; thread++)      //espera para que cada thread termine.
    {
        pthread_join(thread_handles[thread], NULL);
    }

    for (int i = 0; i < N_intervalo; i++)       //impresion del histograma.
    {    
        std::cout << "Intervalo " << i << ": " << HISTOGRAM[i] << std::endl;
    }

    pthread_mutex_destroy(&mutex);  //destrucción del mutex.
    //destructores
    delete[] thread_handles;
    delete[] HISTOGRAM;

    return 0;
}