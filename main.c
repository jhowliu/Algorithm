#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define SWAP(type, a, b) {type tmp=a; a=b; b=tmp;}

int size;

void copyArray(int *a, int *b, int len) {
    int i;

    for (i=0; i<len; i++) {
        a[i] = b[i];
    }
}


int adjusting(int *data, int left, int right, int pivot)
{
    int i, j;

    i = left;
    j = right-1;

    while (i < j) 
    {
        while (data[i] <= pivot) i++;
        while (data[j] > pivot)  j--;

        //printf("i=%d, j=%d\n %d, %d\n", i, j,data[i], data[j]);

        if (i < j) SWAP(int, data[i], data[j]);
    }

    //printf("idx=%d\n", j);

    
    return j;
}

int *loadFile(const char *fileName) 
{
    FILE *fptr = fopen(fileName, "r");

    if (!fptr) 
    {
        printf("Invalid fileName.\n");
        exit(1);
    }

    int *data, i;
   
    fscanf(fptr, "%d\n", &size);

    data = malloc(size*sizeof(int));

    for (i = 0; i < size; ++i)
    {
        fscanf(fptr, "%d\n", data+i);
    }

    return data;
}

int *swapping(int *data, int *boundary, int *idx, int num) 
{
    int i, j, k;
    int len;
    int tmp[num+1];
    int *result = malloc(size*sizeof(int));
    
    k=0;
    len=0;
    copyArray(tmp, boundary, num+1); 
    /* Upperside */
    for (i=0, j=num/2 ; i<num/2; i++, j++) {
        copyArray(result+len, data+boundary[i], idx[i]-boundary[i]+1);
        len += (idx[i]-boundary[i]+1);
        copyArray(result+len, data+boundary[j], idx[j]-boundary[j]+1);
        len += (idx[j]-boundary[j]+1);
        tmp[i+1]=len;
    }

    for (i=0, j=num/2 ; i<num/2; i++, j++) {
        copyArray(result+len, data+idx[i]+1, boundary[i+1]-(idx[i]+1));
        len += (boundary[i+1]-(idx[i]+1));
        copyArray(result+len, data+idx[j]+1, boundary[j+1]-(idx[j]+1));
        len += (boundary[j+1]-(idx[j]+1));
    }
    
    copyArray(boundary, tmp, num+1);
    free(data);

    return result;
}

void insertionSort(int *data, int len) 
{
    int i, j, tmp;

    for (i=1; i<len; i++) {
        j = i-1;
        tmp = *(data+i);
        while (data[j] > tmp && j >= 0) {
            *(data+j+1) = *(data+j);
            j--;
        }
        *(data+j+1) = tmp;
    }
}

int *partition(int *data, int num) 
{
    int pivot, i, interval, j;
    int boundary[num+1];
    int idx[num];
    
    interval = (int)((float)size / (float)num + 0.5);

    /* Get boudary of partition */ 
    for (i=0; i<num; i++) 
        boundary[i] = i*interval;
    boundary[i] = size;

    j = num; 

    while (j != 1) {
        int round = num/j;
        int k;
        printf("Partition = %d\n\n", j);
        for (k=0; k<round; k++) {
            pivot = *(data+boundary[k*num]);
            printf("Pivot = %d\n", pivot);
            printf("%d ", k*num);
            /* Adjust data ordering 
            for (i=k*num; i<num; i++) {
                printf("%d, %d\n", boundary[i], boundary[i+1]);
                idx[i] = adjusting(data, boundary[i], boundary[i+1], pivot);
            }

            printf("\nAfter Adjusting\n");
            for (i=0; i<size; i++) printf("%d\n", data[i]);
            printf("\nAfter Swapping\n");
             Swapping data position 
            data = swapping(data, boundary, idx, num);
            for (i=0; i<size; i++) printf("%d\n", data[i]);
            */
            
        }
        printf("\n");
        j /= 2;
    }
    /*
    for (i=0; i<num; i++) {
        printf("Boundary=%d, %d", boundary[i], boundary[i+1]);
        printf("Len=%d\n", boundary[i+1]-boundary[i]);
        insertionSort(data+boundary[i], boundary[i+1]-boundary[i]);
    }
    */

    return data;
}


int main(int argc, char **argv) 
{
    int nthreads, tid, *data, i;

    data = loadFile(argv[1]);

    data = partition(data, 4);

    for (i=0; i<size; i++) printf("%d\n", data[i]);
    //insertionSort(data, 10);

    /*
    #pragma omp parallel private(tid) 
    {
        tid = omp_get_thread_num();
        printf("Hello World from thread = %d\n", tid);

        if (tid == 0) 
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
    }
    */

    return 0;
}
