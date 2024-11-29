#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>

std::queue<int> Cola;
bool final = false;

/////// MUTEX ///////////////
pthread_mutex_t cola_mutex;
pthread_cond_t condicion;

void* Funcion(void* R) {
    while (true)
    {
        pthread_mutex_lock(&cola_mutex);

        while (Cola.empty() && !final)
        {
            pthread_cond_wait(&condicion, &cola_mutex);
        }

        if (final && Cola.empty()){
            pthread_mutex_unlock(&cola_mutex);

            break;
        }else{}

        int Tarea = Cola.front();
        Cola.pop();
        pthread_mutex_unlock(&cola_mutex);

        std::cout << "-> Thread " << (long)R << " Procesando Tarea :" << Tarea << std::endl;
        sleep(1); // Simula la ejecución de la tarea
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    int thread_count = strtol(argv[1], NULL, 10);
    pthread_t* thread_handles = new pthread_t[thread_count];
    pthread_mutex_init(&cola_mutex, NULL);
    pthread_cond_init(&condicion, NULL);

    for (int thread = 0; thread < thread_count; thread++){
        pthread_create(&thread_handles[thread], NULL, Funcion, (void*)thread);
    }

    for (int i = 0; i < 10; i++){

        pthread_mutex_lock(&cola_mutex);
        Cola.push(i);
        pthread_cond_signal(&condicion);
        pthread_mutex_unlock(&cola_mutex);
        sleep(1); // Agrega tareas cada segundo
    }

    pthread_mutex_lock(&cola_mutex);
    final = true;
    pthread_cond_broadcast(&condicion);
    pthread_mutex_unlock(&cola_mutex);

    for (int thread = 0; thread < thread_count; thread++){

        pthread_join(thread_handles[thread], NULL);
    }

    pthread_mutex_destroy(&cola_mutex);
    pthread_cond_destroy(&condicion);
    delete[] thread_handles;

    return 0;
}
