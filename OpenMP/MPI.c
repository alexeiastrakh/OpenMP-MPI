#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX 100

void mergeSort(int a[], int l, int r);
void merge(int a[], int l, int m, int r);

int main() {
    MPI_Init(NULL, NULL);
    int my_rank, no_proc;
    double start_parallel, end_parallel, start_serial, end_serial;
    int* arr;
    arr = malloc(MAX * sizeof(int));

    long long i;
    int num;
    srand(time(0));

    for (i = 0; i < MAX; i++) {
        num = rand() % 100000;
        arr[i] = num;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &no_proc);

    int user_no_proc;
    if (my_rank == 0) {
        printf("Enter the number of processes: ");
        fflush(stdout);
        scanf_s("%d", &user_no_proc);
    }

    MPI_Bcast(&user_no_proc, 1, MPI_INT, 0, MPI_COMM_WORLD);
    no_proc = user_no_proc;
    int partition_size = MAX / no_proc;

    int* local_array;
    local_array = malloc(partition_size * sizeof(int));

    MPI_Scatter(arr, partition_size, MPI_INT, local_array, partition_size, MPI_INT, 0, MPI_COMM_WORLD);

    start_parallel = MPI_Wtime();

    mergeSort(local_array, 0, (partition_size - 1));

    MPI_Gather(local_array, partition_size, MPI_INT, arr, partition_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        mergeSort(arr, 0, (MAX - 1));
        end_parallel = MPI_Wtime();
    }

    if (my_rank == 0) {
        start_serial = MPI_Wtime();
        mergeSort(arr, 0, (MAX - 1));
        end_serial = MPI_Wtime();

        printf("Array Size: %d, Processes: %d\n", MAX, no_proc);
        printf("Parallel Time Taken: %f seconds\n", end_parallel - start_parallel);
        printf("Serial Time Taken: %f seconds\n", end_serial - start_serial);

        printf("Sorted Array: ");
        for (i = 0; i < MAX; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    free(arr);
    free(local_array);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}

void merge(int* arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int* Left = malloc(n1 * sizeof(int));
    int* Right = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        Left[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        Right[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (Left[i] <= Right[j]) {
            arr[k] = Left[i];
            i++;
        }
        else {
            arr[k] = Right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = Left[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = Right[j];
        j++;
        k++;
    }

    free(Left);
    free(Right);
}

void mergeSort(int a[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(a, l, m);
        mergeSort(a, m + 1, r);
        merge(a, l, m, r);
    }
}
