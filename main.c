#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define SWAP(type, a, b) {type tmp=a; a=b; b=tmp;}

int size;

void copyArray(int *a, int *b, int len) {
    int i;

    for (i=0; i<len; i++) a[i] = b[i];
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

        if (i < j) SWAP(int, data[i], data[j]);
    }
    
    return j;
}

int *loadFile(const char *fileName) 
{
    FILE *fptr = fopen(fileName, "r");
    int *data, i;

    if (!fptr) 
    {
        printf("Invalid fileName.\n");
        exit(1);
    }
   
    fscanf(fptr, "%d\n", &size);

    data = malloc(size*sizeof(int));

    for (i = 0; i < size; ++i) fscanf(fptr, "%d\n", data+i);
    
    fclose(fptr);
    return data;
}

void writefile(const char *fileName, int *data) 
{
    FILE *fptr = fopen(fileName, "w");
    int i;

    if (!fptr) {
        printf("Invalid fileName.\n");
        exit(1);
    }

    for (i=0; i<size; ++i) fprintf(fptr, "%d\n", data[i]);
    fclose(fptr);
}

int *swapping(int *data, int *boundary, int *idx, int num) 
{
    int i, j, k;
    int len;
    int *tmp = malloc((num+1)*sizeof(int));
    int *result = malloc(size*sizeof(int));
    
    k=0;
    len=boundary[0];
    printf("NUM=%d\n", num);
    copyArray(tmp, boundary, num+1); 
    copyArray(result, data, size);
    printf("Before Boundary\n");
    for (i=0; i<=8; i++) printf("%d ", tmp[i]);
    /* Upperside */
    for (i=0, j=num/2 ; i<num/2; i++, j++) {
        printf("\nLen\n");
        copyArray(result+len, data+boundary[i], idx[i]-boundary[i]+1);
        len += (idx[i]-boundary[i]+1);
        copyArray(result+len, data+boundary[j], idx[j]-boundary[j]+1);
        len += (idx[j]-boundary[j]+1);
        tmp[i+1]=len;
        printf("%d\n", len);
    }

    printf("\n");
    for (i=0; i<=8; i++) printf("%d ", tmp[i]);
    for (i=0, j=num/2 ; i<num/2; i++, j++) {
        copyArray(result+len, data+idx[i]+1, boundary[i+1]-(idx[i]+1));
        len += (boundary[i+1]-(idx[i]+1));
        copyArray(result+len, data+idx[j]+1, boundary[j+1]-(idx[j]+1));
        len += (boundary[j+1]-(idx[j]+1));
        tmp[j+1]=len;
        printf("%d\n", len);
    }

    copyArray(boundary, tmp, num+1);
    printf("\n");
    for (i=0; i<=8; i++) printf("%d ", tmp[i]);
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
    int pivot, i, interval, j, tid;
    int boundary[num+1];
    int idx[num];

    interval = (int)((float)size / (float)num + 0.5);

    /* Get boudary of partition */ 
    for (i=0; i<num; i++) boundary[i] = i*interval;
    boundary[i] = size;

    j = num; 

    while (j != 1) {
        int round = num/j;
        int k;

        for (k=0; k<round; k++) 
        {
            printf("Data=  %d\n", k*num/j);
            pivot = *(data+boundary[k*j]+rand()%(boundary[k*j+1]-boundary[k*j]-1));

            // Adjust data ordering 
            for (i=k*num/j; i<(k+1)*j; i++) 
                idx[i] = adjusting(data, boundary[i], boundary[i+1], pivot);

            // Swapping data position 
            data = swapping(data, boundary+k*j, idx+k*j, j);
        }

        j /= 2;
    }
    printf("\nInsertionSorting\n");
    for (i=0; i<=num; i++) printf("%d ", boundary[i]);
    #pragma omp parallel for
    for (i=0; i<num; i++) 
    {
        insertionSort(data+boundary[i], boundary[i+1]-boundary[i]);
    }

    return data;
}


int main(int argc, char **argv) 
{
    int *data;

    if (argc < 3) 
    {
        printf("./* <input> <output>");
        exit(0);
    }

    srand(time(NULL));

    data = loadFile(argv[1]);
    data = partition(data, atoi(argv[3]));
    writefile(argv[2], data);    

    return 0;
}
