#include <stdio.h>
#include "fileManager.h"

float **readMatrix(FILE *file, int lines, int n)//LE UMA MATRIZ EM UM ARQUIVO TEXTO
{
    int i, j;
    float num;
    float **m;
    
    m = new float*[lines];
    
    for(i=0; i<lines; i++)
        m[i] = new float[n];
    
    i=j=0;
    
    while(!feof(file))
    {
        fscanf(file, "%f", &num);
        m[i][j++] = num;
        
        fgetc(file);
        
        if(j == n)
        {
            j = 0;
            i++;
        }
    }
    
    return m;
}