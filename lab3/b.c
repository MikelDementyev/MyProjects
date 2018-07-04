#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bsort(void *a, int item, int size, int (*cmp)(const void*, const void*)) {
    int i;
    void *tmp = NULL;
    void *prev, *cur;
    char flag;
 
    tmp = malloc(item);
 
    do {
        flag = 0;
        i = 1;
        prev = (char*)a;
        cur  = (char*)prev + item;
        while (i < size) {
            if (cmp(cur, prev)) {
                memcpy(tmp, prev, item);
                memcpy(prev, cur, item);
                memcpy(cur, tmp, item);
                flag = 1;
            }
            i++;
            prev = (char*)prev + item;
            cur  = (char*)cur  + item;
        }
    } while (flag);
 
    free(tmp);
}

int compareChar(const void* a, const void* b){
        if((*(char*)a)<(*(char*)b)) return -1;
        else if((*(char*)a)>(*(char*)b)) return 1;
        else return 0;
}

int compareInt(const void* a, const void* b){
	if((*(int*)a)<(*(int*)b)) return -1;
	else if((*(int*)a)>(*(int*)b)) return 1;
	else return 0;
}

int compareDouble(const void* a, const void* b){
	if((*(double*)a)<(*(double*)b)) return -1;
	else if((*(double*)a)>(*(double*)b)) return 1;
	else return 0;
}

int main(){
    int size, count;
    printf("Введите размер одного элемента массива: ");
    scanf("%d", &size);

    if(size != 1 && size != 4 && size != 8)
    {            
        printf("Указан неверный размер элемента массива\n");
        return 0;
    }

    printf("Введите количество элементов в массиве: ");
    scanf("%d", &count);

    if(size == 4)
    {
            int arr[count];
            for (int i = 0; i < count; i++)
	    {
                scanf("%d", &arr[i]);
            }

            bsort(arr,sizeof(int),count,&compareInt);

            for (int i = 0; i < count; i++)
	    {
                printf("%d ",arr[i]);
            }
            printf("\n");
    }

    if(size == 8)
    {
            double arr[count];
            for (int i = 0; i < count; i++)
  	    {
                scanf("%lf", &arr[i]);
            }

            bsort(arr,sizeof(double),count,&compareDouble);

            for (int i = 0; i < count; i++)
 	    {
                printf("%lf ",arr[i]);
            }
            printf("\n");
    }

    if(size == 1)
    {
            char arr[count];
            for (int i = 0; i < count; i++)
	    {
                scanf(" %c", &arr[i]);
            }

            bsort(arr,sizeof(char),count,&compareChar);

            for (int i = 0; i < count; i++)
	    {
                printf("%c ",arr[i]);
            }
            printf("\n");
    }
}
