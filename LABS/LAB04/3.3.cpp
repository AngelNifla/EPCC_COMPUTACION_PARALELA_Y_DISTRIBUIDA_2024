#include <mpi.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    int numProcess;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcess);

    int local_val = rank + 1;
    int sum = local_val;
    int step = 1;

    // Indicador inicial
    std::cout << "Proceso " << rank << " ha comenzado con valor local " << local_val << "." << std::endl;

    while (step < numProcess)
    {
        if (rank % (2 * step) == 0)
        {
            int recv_val;
            std::cout << "Proceso " << rank << " está esperando recibir del proceso " << (rank + step) << "..." << std::endl;
            MPI_Recv(&recv_val, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += recv_val;
            std::cout << "Proceso " << rank << " ha recibido " << recv_val << ", suma local ahora " << sum << "." << std::endl;
        } else if ((rank - step) % (2 * step) == 0)
        {
            std::cout << "Proceso " << rank << " está enviando suma local " << sum << " al proceso " << (rank - step) << "..." << std::endl;
            MPI_Send(&sum, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
            std::cout << "Proceso " << rank << " ha enviado suma local " << sum << "." << std::endl;
            break;
        }
        step *= 2;
    }

    if (rank == 0) {
        std::cout << "Suma global: " << sum << std::endl;
    }

    MPI_Finalize();
    return 0;
}
