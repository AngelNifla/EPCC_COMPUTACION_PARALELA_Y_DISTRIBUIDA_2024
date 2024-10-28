#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void generadorDatos(int *data, int N, int valorMaximo) {
    for (int i = 0; i < N; i++) data[i] = rand() % valorMaximo;
}

void histogramaLOcal(int *data, int N, int *histograma, int bins) {////
    for (int i = 0; i < N; i++) histograma[data[i]]++;
}

int main(int argc, char** argv) {
    int rank, numProces, n = 1000, valorMaximo = 10;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProces);

    // Imprimir que el proceso está trabajando
    printf("Proceso %d de %d está trabajando...\n", rank, numProces);

    int local_n = n / numProces;
    int *data = NULL, *local_data = (int*)malloc(local_n * sizeof(int));
    int *histogram = (int*)calloc(valorMaximo, sizeof(int));
    int *global_histogram = NULL;

    if (rank == 0) {
        data = (int*)malloc(n * sizeof(int));
        generadorDatos(data, n, valorMaximo);
        printf("Proceso %d generó %d datos:\n", rank, n);
        /*for (int i = 0; i < n; i++)
        {
            printf("%d ", data[i]);
        }*/
        printf("\n");
    }

    // Distribuir datos a todos los procesos
    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Informar cuántos datos se pasaron a cada proceso
    printf("Proceso %d recibió %d datos:\n", rank, local_n);
    
    // Imprimir los datos que maneja cada proceso
    /*printf("Proceso %d maneja los datos:\n", rank);
    for (int i = 0; i < local_n; i++)
    {
        printf("%d ", local_data[i]);
    }
    printf("\n");*/

    // Calcular el histograma local
    histogramaLOcal(local_data, local_n, histogram, valorMaximo);
    printf("Proceso %d calculó el histograma local:\n", rank);
    for (int i = 0; i < valorMaximo; i++) {
        printf("Bin %d: %d\n", i, histogram[i]);
    }

    // Reducir el histograma global
    if (rank == 0) global_histogram = (int*)malloc(valorMaximo * sizeof(int));
    MPI_Reduce(histogram, global_histogram, valorMaximo, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Imprimir el histograma global en el proceso 0
    if (rank == 0) {
        printf("Histograma global:\n");
        for (int i = 0; i < valorMaximo; i++) {
            printf("%d: %d\n", i, global_histogram[i]);
        }
    }

    // Liberar memoria
    free(data);
    free(local_data);
    free(histogram);
    free(global_histogram);
    MPI_Finalize();
    return 0;
}