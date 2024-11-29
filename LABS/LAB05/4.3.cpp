#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <cmath>

double total_area = 0.0;
int N_threads;
double a, b;
int n;

/// MUTEX///////
pthread_mutex_t mutex;
sem_t semaphore;

double f(double x) // Función a integrar
{
    return x * x;
}

void* Trapezoidal(void* rank){  ////////////Función de Trapezoide.
    long my_rank = (long)rank;
    int local_n = n / N_threads;

    double h = (b - a) / n;
    double local_a = a + my_rank * local_n * h;
    double local_b = local_a + local_n * h;

    double local_area = (f(local_a) + f(local_b)) / 2.0;
    for (int i = 1; i < local_n; i++)
    {
        double x = local_a + i * h;
        local_area += f(x);
    }
    local_area *= h;

    /////////////////////// Sección crítica con mutex
    pthread_mutex_lock(&mutex);
    total_area += local_area;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[])    ////Funcion principal
{
    N_threads = strtol(argv[1], NULL, 10);
    a = atof(argv[2]);
    b = atof(argv[3]);
    n = strtol(argv[4], NULL, 10);

    pthread_t* thread_handles = new pthread_t[N_threads];
    pthread_mutex_init(&mutex, NULL);               ///Iniciando el Mutex
    sem_init(&semaphore, 0, 1);

    for (int thread = 0; thread < N_threads; thread++){
        pthread_create(&thread_handles[thread], NULL, Trapezoidal, (void*)thread);
    }

    for (int thread = 0; thread < N_threads; thread++){
        pthread_join(thread_handles[thread], NULL);
    }

    std::cout << "Área total = " << total_area << std::endl;

    /////////Destruyendo mutex
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);

    delete[] thread_handles;

    return 0;
}