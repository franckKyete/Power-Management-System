#include <stdlib.h>
#include <stdio.h>

typedef struct{
    char a;
    int array[10];
}Struct;

int main(){

    Struct arr;

    printf("sizeof(int) : %d\n", (int)sizeof(int));
    printf("sizeof(Struct) : %d\n", (int)sizeof(Struct));
    printf("sizeof(arr) : %d\n", (int)sizeof(arr));
    printf("sizeof(&arr) : %d\n", (int)sizeof(*(&arr)));

    return 0;
}