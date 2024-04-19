//#include <string>
//#include "mpi.h"
//#include <chrono>
//#include <cmath>
//
//using namespace std;
//
//void printArray(int* arr, int size) {
//    for (int i = 0; i < size; ++i) {
//        fprintf(stdout, "%d ", arr[i]);
//    }
//    fprintf(stdout, "\n");
//    fflush(stdout);
//}
//
//void merge(int* arr, int l, int m, int r) {
//    int i, j, k;
//    int n1 = m - l + 1;
//    int n2 = r - m;
//
//    // Create temporary arrays
//    int* L = new int[n1];
//    int* R = new int[n2];
//
//    // Copy data to temp arrays L[] and R[]
//    for (i = 0; i < n1; i++)
//        L[i] = arr[l + i];
//    for (j = 0; j < n2; j++)
//        R[j] = arr[m + 1 + j];
//
//    // Merge the temp arrays back into arr[l..r]
//    i = 0;
//    j = 0;
//    k = l;
//    while (i < n1 && j < n2) {
//        if (L[i] <= R[j]) {
//            arr[k] = L[i];
//            i++;
//        }
//        else {
//            arr[k] = R[j];
//            j++;
//        }
//        k++;
//    }
//
//    // Copy the remaining elements of L[], if there are any
//    while (i < n1) {
//        arr[k] = L[i];
//        i++;
//        k++;
//    }
//
//    // Copy the remaining elements of R[], if there are any
//    while (j < n2) {
//        arr[k] = R[j];
//        j++;
//        k++;
//    }
//
//    delete[] L;
//    delete[] R;
//}
//
//void mergeSort(int* arr, int l, int r) {
//    if (l < r) {
//        // Same as (l+r)/2, but avoids overflow for large l and r
//        int m = l + (r - l) / 2;
//
//        // Sort first and second halves
//        mergeSort(arr, l, m);
//        mergeSort(arr, m + 1, r);
//
//        // Merge the sorted halves
//        merge(arr, l, m, r);
//    }
//}
//
//int* parallelMergeSort(int* arr, int N, int rank, int size) {
//    double startTime;
//    if (rank == 0) {
//        startTime = MPI_Wtime();
//    }
//
//    // Each process determines its portion of the array
//    int localN = N / size;
//    int localStart = rank * localN;
//    int localEnd = (rank + 1) * localN - 1;
//
//    // Allocate memory for local array segment
//    int* localArr = new int[localN];
//
//    // Send each process its portion of the array
//    MPI_Send(&arr[localStart], localN, MPI_INT, rank, 0, MPI_COMM_WORLD);
//
//    // Receive the local array segment
//    MPI_Recv(localArr, localN, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//
//    // Perform local sort
//    mergeSort(localArr, 0, localN - 1);
//
//    // Gather sorted subarrays at root
//    int* sortedArr = nullptr;
//    if (rank == 0) {
//        sortedArr = new int[N];
//    }
//    MPI_Gather(localArr, localN, MPI_INT, sortedArr, localN, MPI_INT, 0, MPI_COMM_WORLD);
//
//    if (rank == 0) {
//        // Final merge of sorted subarrays
//        mergeSort(sortedArr, 0, N - 1);
//
//        double endTime = MPI_Wtime();
//        double elapsedTime = endTime - startTime;
//        fprintf(stdout, "Elapsed parallel time:  %f\n", elapsedTime);
//        fflush(stdout);
//    }
//
//    delete[] localArr;
//
//    return sortedArr;
//}
//
//int main(int argc, char** argv) {
//    int N = 2000;
//    srand(time(nullptr));
//    int* myArray = new int[N];
//
//    for (int i = 0; i < N; ++i) {
//        myArray[i] = 0 + rand() % (10000 - 0 + 1);
//    };
//
//    int rank, size;
//    int namelen;
//    char processor_name[MPI_MAX_PROCESSOR_NAME];
//    MPI_Init(&argc, &argv);
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Comm_size(MPI_COMM_WORLD, &size);
//    MPI_Get_processor_name(processor_name, &namelen);
//
//    int* sortedArray = parallelMergeSort(myArray, N, rank, size);
//    if (rank == 0) {
//        // Print the sorted array
//        printArray(sortedArray, N);
//    }
//
//    MPI_Finalize();
//
//    delete[] myArray;
//    delete[] sortedArray;
//
//    return 0;
//}
