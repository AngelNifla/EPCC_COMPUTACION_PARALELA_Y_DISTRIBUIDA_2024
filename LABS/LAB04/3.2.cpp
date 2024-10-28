#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Función que lanza los dardos
double Dardos(int num) {
    int c = 0;
    for (int i = 0; i < num; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) c++;
    }
    return c;
}

int main(int argc, char** argv)
{
    int rank, numProces, total_throws = 1000000, local_throws, local_in_circle, global_in_circle;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProces);

    local_throws = total_throws / numProces;

    // Informa que el proceso está trabajando
    printf("Proceso %d está lanzando %d dardos...\n", rank, local_throws);
    
    local_in_circle = Dardos(local_throws);
    MPI_Reduce(&local_in_circle, &global_in_circle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_estimate = 4.0 * global_in_circle / total_throws;
        printf("Estimated Pi: %f\n", pi_estimate);
    }

    MPI_Finalize();
    return 0;
}