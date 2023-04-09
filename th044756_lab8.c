#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

//swap function for heapify
void swap(int* x, int* y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

//Heapify Function for heapSort
//n is size of the heap
void heapify(int array[], int n, int root)
{
	int largest = root;

	int left = 2 * root + 1;

	int right = 2 * root + 2;

	//checks if left child is bigger than root
	if(left < n && array[left] > array[largest]){
		largest = left;
	}

	//checks if right child is bigger than root
	if(right < n && array[right] > array[largest]){
		largest = right;
	}
	//If the root isnt the largest
	if(largest != root){
		swap(&array[root], &array[largest]);

		//heapify the subtree
		heapify(array, n, largest);
	}

}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int array[], int n)
{
	//Building Heap for the function
	for(int i = n / 2 - 1; i >= 0; i--){
		heapify(array, n, i);
	}

	//sorting part of function
	for(int j = n -1; j >= 0; j--){
		swap(&array[0], &array[j]);

		//Get highest element at root
		heapify(array, j, 0);
	}
}

//Merge function for merge sort
void merge(int arr[], int left, int mid, int right){
	//placeholder vars
	int i, j, k;

	//Array partitions
	int n1 = mid - left + 1;
	int n2 = right - mid;

	//temp arrays
	int *leftA = (int*)malloc(n1*sizeof(int));
	int *rightA = (int*)malloc(n2*sizeof(int));

	
	//merge sort uses temporary arrays, so increase extraMemoryAllocated based on size of arrays
	extraMemoryAllocated += n1*sizeof(int) + n2*sizeof(int);

	//Data copying
	for(i = 0; i < n1; i++){
		leftA[i] = arr[left + i];
	}
	for(j = 0; j < n2; j++){
		rightA[j] = arr[mid + 1 + j];
	}
	//initial indexes
	i = 0;
	j = 0;
	k = left;
	//merging back into arrays
	while(i < n1 && j < n2){
		if(leftA[i] <= rightA[j]){
			arr[k] = leftA[i];
			i++;
		}
		else{
			arr[k] = rightA[j];
			j++;
		}
		k++;

	}
	//checks for remaining elements
	while(i < n1){
		arr[k] = leftA[i];
		i++;
		k++;
	}

	while(j < n2){
		arr[k] = rightA[j];
		j++;
		k++;
	}

	free(leftA);
	free(rightA);

}




// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int left, int right)
{
	if(left < right){
		//finds middle of array
		int mid = left + (right-left)/2;
		//recursively sorts both halves of array 
		mergeSort(pData, left, mid);
		mergeSort(pData, mid + 1, right);
		//merges sorted arrays back together
		merge(pData, left, mid, right);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i;
    printf("\tData:\n\t");
    
    // print first 100 items
    for (i=0;i<100 && i<dataSz;++i)
    {
        printf("%d ",pData[i]);
    }
    
    // check if there are more than 100 items in the array
    if (dataSz > 100) {
        printf("\n\t...");
        // print last 100 items
        for (i=dataSz-100;i<dataSz;++i)
        {
            printf("%d ",pData[i]);
        }
    }
    printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}