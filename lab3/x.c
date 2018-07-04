#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void bsort (void* arr, int num, int size, int (*compar)(const void* a, const void* b))
{
	for (int i=0; i<num-1; i++)
	{
		for(int j=i+1; j<num; j++)
		{
			if(compar(arr+i*size, arr+j*size) > 0 )
			{
				for (int k=0; k<size; k++)
				{
					char t = (*((char*)arr+i*size+k));
					(*((char*)arr+i*size+k))=(*((char*)arr+j*size+k));
					(*((char*)arr+j*size+k))=t;
				}
			}
		}
	}
}			

int comparInt(const void* a, const void* b)
{
	if (*(int*)a > *(int*)b) return 1;
        if (*(int*)a < *(int*)b) return -1;
        if (*(int*)a == *(int*)b) return 0;
}

int comparDouble(const void* a, const void*b)
{
        if ( (*(double*)a > *(double*)b) && (fabs(*(double*)a - *(double*)b) > 0.00001) ) return 1;
        if ( (*(double*)a < *(double*)b) && (fabs(*(double*)b - *(double*)a) > 0.00001) ) return -1;
        if (fabs(*(double*)a - *(double*)b) <= 0.00001) return 0;
}
    
int comparChar(const void* a, const void* b)
{
        if (*(char*)a > *(char*)b) return 1;
        if (*(char*)a < *(char*)b) return -1;
        if (*(char*)a == *(char*)b) return 0;

}


int main()
{
	int size;
	int count;
	printf("Введите размер элемента массива: \n"); 
	scanf("%d", &size);

	if(size!=1 && size!= 4 && size!= 8)
	{
		printf("Неверный размер элемента массива\n");
		return 0;
	}

	printf("Введите количество элементов массива: \n");
	scanf("%d", &count);

	if(size == 1)
	{
		char arr[count];
		for(int i = 0; i < count; i++) 
                {
			scanf("%c", &arr[i]);
		} 
                bsort(arr, count, size, &comparChar);
		
		for(int i = 0; i < count; i++)
		{
               		printf("%c ", arr[i]);
		}
		printf("\n");
	}
	
	if(size == 4)
        {
                int arr[count];
                for(int i = 0; i < count; i++)
                {
                        scanf("%d", &arr[i]);
                }
                bsort(arr, count, size, &comparInt);

                for(int i = 0; i < count; i++)
                {
                        printf("%d ", arr[i]);
                }
		printf("\n");
        }

	if(size == 8)
        {
                double arr[count];
                for(int i = 0; i < count; i++)
                {
                        scanf("%lf", &arr[i]);
                }
                bsort(arr, count, size, &comparDouble);

                for(int i = 0; i < count; i++)
                {
                        printf("%lf ", arr[i]);
                }
		printf("\n");
        }
return 0;
}







