#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void mult_matriz_vec(int *L_A, int *Lx, int *Ly, int Lm, int n) {
    for (int i = 0; i < Lm; i++) {
        Ly[i] = 0;
        for (int j = 0; j < n; j++) {
            Ly[i] += L_A[i * n + j] * Lx[j];
        }
    }
}

int main(int argc, char** argv) {
    int rank, numProcess, m = 2, n = 2;

    // Inicialización de MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcess);

    // Asegúrate de que m es divisible por numProcess
    if (m % numProcess != 0) {
        if (rank == 0) {
            printf("Error: m debe ser divisible por el número de procesos.\n");
        }
        MPI_Finalize();
        return -1;
    }

    int local_m = m / numProcess;
    int *local_A = (int*)malloc(local_m * n * sizeof(int)); // Cast a int*
    int *local_x = (int*)malloc(n * sizeof(int));           // Cast a int*
    int *local_y = (int*)malloc(local_m * sizeof(int));     // Cast a int*
    int *x = NULL, *y = NULL;

    // Solo el proceso 0 inicializa la matriz y el vector
    if (rank == 0) {
        x = (int*)malloc(n * sizeof(int));                    // Cast a int*
        y = (int*)malloc(m * sizeof(int));                    // Cast a int*

        // Inicializa el vector x
        for (int i = 0; i < n; i++) {
            x[i] = 1;
        }

        // Inicializa la matriz A
        int *A = (int*)malloc(m * n * sizeof(int));           // Cast a int*
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                A[i * n + j] = i + j;  // Ejemplo de inicialización
            }
        }

        // Distribuye la matriz A a todos los procesos
        printf("Proceso %d distribuyendo la matriz A...\n", rank);
        MPI_Scatter(A, local_m * n, MPI_INT, local_A, local_m * n, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Proceso %d ha recibido su parte de la matriz A.\n", rank);
        free(A);
    } else {
        // Otros procesos inicializan local_A
        printf("Proceso %d esperando la matriz A...\n", rank);
        MPI_Scatter(NULL, local_m * n, MPI_INT, local_A, local_m * n, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Proceso %d ha recibido su parte de la matriz A.\n", rank);
    }

    // Distribuye el vector x a todos los procesos
    MPI_Bcast(x, n, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Proceso %d ha recibido el vector x.\n", rank);

    // Multiplica la matriz local_A por el vector x
    printf("Proceso %d comenzando la multiplicación...\n", rank);
    mult_matriz_vec(local_A, x, local_y, local_m, n);
    printf("Proceso %d ha terminado la multiplicación.\n", rank);

    // Reúne los resultados en el proceso 0
    MPI_Gather(local_y, local_m, MPI_INT, y, local_m, MPI_INT, 0, MPI_COMM_WORLD);

    // Solo el proceso 0 imprime el resultado
    if (rank == 0) {
        printf("Vector resultante y:\n");
        for (int i = 0; i < m; i++) {
            printf("%d ", y[i]);
        }
        printf("\n");
        free(x);
        free(y);
    }

    // Liberación de memoria
    free(local_A);
    free(local_x);
    free(local_y);
    MPI_Finalize();
    return 0;
}
