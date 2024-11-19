#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void func(char *buf){

    float x, y, z;
    char *line = strtok(buf, "\n");
    while (line != NULL)
    {
        if(sscanf(line, "%f;%f;%f", &x, &y, &z) == 3){
            printf("x : %f, y : %f, z : %f\n", x, y, z);
        }
        
        line = strtok(NULL, "\n");
    }
}

int main(){
    
    char buffer[] = "15.4;65;90\n3;9;8\n38;1;8\n";
    
    func(buffer);

    return 0;
} 