#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int *arr, int *temp, int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) temp[k++] = arr[i] <= arr[j] ? arr[i++] : arr[j++];
    while (i <= mid)
    {
        temp[k++] = arr[i++]
    };
    while (j <= right)
    {
        temp[k++] = arr[j++]
    };
    for (i = left; i <= right; i++)
    {
        arr[i] = temp[i]
    };
}

void merge_sort(int *arr, int *temp, int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort(arr, temp, left, mid);
        merge_sort(arr, temp, mid + 1, right);
        merge(arr, temp, left, mid, right);
    }else{}
}

int main(int argc, char** argv) {
    int rank, numProcess, n = 8;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcess);

    int local_n = n / numProcess;
    int *local_data = (int*)malloc(local_n * sizeof(int));
    int *data = NULL;

    if (rank == 0) {
        data = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) data[i] = rand() % 100;
        printf("Unsorted array:\n");
        for (int i = 0; i < n; i++) printf("%d ", data[i]);
        printf("\n");
    }

    // Distribuir los datos entre los procesos
    MPI_Scatter(data, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Mensaje indicando que el proceso está comenzando a ordenar
    printf("Proceso %d: Comenzando merge_sort en sus datos locales.\n", rank);
    
    int *temp = (int*)malloc(local_n * sizeof(int)); // Conversión explícita
    merge_sort(local_data, temp, 0, local_n - 1);

    int step = 1;
    while (step < numProcess) {
        if (rank % (2 * step) == 0) {
            int *recv_data = (int*)malloc(local_n * step * sizeof(int));
            printf("Proceso %d: Esperando datos del proceso %d.\n", rank, rank + step);
            MPI_Recv(recv_data, local_n * step, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Proceso %d: Recibió datos del proceso %d.\n", rank, rank + step);
            
            merge(local_data, temp, 0, local_n * step - 1, local_n * (2 * step) - 1);
            local_n *= 2;
            free(recv_data); // Liberar memoria
        } else {
            printf("Proceso %d: Enviando datos a proceso %d.\n", rank, rank - step);
            MPI_Send(local_data, local_n, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
            break;
        }
        step *= 2;
    }

    if (rank == 0) {
        printf("Sorted array:\n");
        for (int i = 0; i < n; i++) printf("%d ", data[i]);
        printf("\n");
    }

    free(data); free(local_data); free(temp);
    MPI_Finalize();
    return 0;
}
