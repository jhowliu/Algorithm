#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define SWAP(type, a, b) {type tmp=a; a=b; b=tmp;}

int size;

void copyArray(int *a, int *b, int len) {
    int i;
    //printf("\nCOPY, %d\n", len);
    for (i=0; i<len; i++) {
        //printf("%d ", b[i]);
        a[i] = b[i];
    }
}

int getPivot(int a, int b, int c) {
    int pivot = c;

    if (a>b && a<c) pivot = a; 
    else if (b>a && b<c) pivot = b; 

    return pivot;
}


int adjusting(int *data, int left, int right, int pivot)
{
    int i, j;

    i = left;
    j = right-1;

    while (i < j) 
    {
        while (data[i] <= pivot && i<=j) i++;
        while (data[j] > pivot && i<=j)  j--;

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

    copyArray(tmp, boundary, num+1); 
    copyArray(result, data, size);

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

        tmp[j+1]=len;
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
        int k, l;
        for (k=0; k<round; k++) 
        {
            //pivot = *(data+boundary[k*j]+rand()%(boundary[k*j+1]-boundary[k*j]-1));
            //pivot = *(data+boundary[k*j]);
            pivot = getPivot(*(data+boundary[k*j]), *(data+boundary[k*j+1]), *(data+(boundary[k*j+1]+boundary[k*j])/2));
            // Adjust data ordering 
            for (i=k*j, l=k*j; i<(k+1)*j; i++, l++) 
                idx[l] = adjusting(data, boundary[i], boundary[i+1], pivot);
            // Swapping data position 
            data = swapping(data, boundary+k*j, idx+k*j, j);
        }

        j /= 2;
    }
    printf("\nInsertionSorting\n");
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
