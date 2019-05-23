#include <math.h>
#include "perceptron.h"
#include "fileManager.h"
#define LINES 105

float **normalizeColumn(float **m, int n, int elements)
{
    float sum, SD, mean;
    int i,j;
    
    for(i=0; i<n-1; i++)
    {
        sum = SD = 0;
        
        for(j=0; j<elements;j++)
            sum += m[j][i];
    
        mean = sum/elements;
        
        for(j=0; j<elements;j++)
            SD += pow(m[j][i] - mean, 2);
    
        SD = sqrt(SD/elements);
        
        for(j=0; j<elements;j++)
            m[j][i] = (m[j][i]-mean)/SD;
    }
    
    return m;
}

float **initMatrix(char *file_name, int lines, int n)//INICIALIZA UMA MATRIZ COM ZERO OU COM O CENTEUDO DE UM ARQUIVO TEXTO
{
    FILE *file;
    float **m;
    
    file = fopen(file_name, "r");
    
    if(file != NULL)
    {
        rewind(file);
        m = readMatrix(file, lines, n);
        fclose(file);
    }
    else
    {
        m = new float*[lines];
    
        for(int i=0; i<lines; i++)
            m[i] = new float[n];   
            
        for(int i=0; i<lines; i++)
            for(int j=0; j<n; j++)
                m[i][j] = 0;
    }
    
    return m;
}

bool errorPercentage(int it, float error, float value)//VERIFICA SE O ERRO ESTA ABAIXO DO ALVEJADO
{
    if(error <= value)
        return false;
    return true;
}

bool maxIterations(int it, float error, float value)//VERIFICA SE O NUMERO DE ITERACOES CHEGOU NO ESTIPULADO
{
    if(it >= value)
        return false;
    return true;
}

//data = NOME DO ARQUIVO DE TREINAMENTO
//weights = NOME DO POSSIVEL ARQUIVO COM PESOS JA DEFINIDOS
//n = QUANTIDADE DE ENTRADAS (DIMENSOES) +1
//classes = QUANTIDADE DE CLASSES DO PROBLEMA
//lr = TAXA DE APRENDIZADO
//sc = TIPO DO CRITERIO DE PARADA
//value = VALOR DO CRITERIO DE PARADA
float **trainPerceptron(char *data, char *weights, int n, int classes, int elements, float lr, bool sc, float value, char *vl_data, int vl_elem)//TREINA A REDE
{
    int it = 0, i, j, k, l, overfitting = 0;
    float **w, **m, error, sum, exp, hit = 0, l_hit = 0;
    bool (*stoppingCriteria) (int, float, float);
    
    w = initMatrix(weights, classes, n);//PREENCHE/INICIALIZA MATRIZ DE PESOS
    m = initMatrix(data, elements, n);//PREENCHE MATRIZ COM DADOS DE TREINAMENTO
    //m = normalizeColumn(m, n, elements);//NORMALIZA OS ATRIBUTOS
    
    if(sc == 0)//ESCOLHE QUAL A FUNÇAO DE PARADA (POR MINIMIZACAO DO ERRO OU QUANTIDADE DE ITERACOES)
        stoppingCriteria = maxIterations;
    else
        stoppingCriteria = errorPercentage;
    
    //ACABA O TREINAMENTO CASO O NUMERO DE ITERACOES SEJA ATINGIDO, 
    //OU APOS A TAXA DE ACERTO PARAR DE AUMENTAR DEPOIS DE UM CERTO PERIODO
    while(stoppingCriteria(it, error, value) && overfitting <= value*0.3)
    {
        for(i=0; i<elements; i++)
        {
            for(j=0; j<classes; j++)
            {
                sum = 0;
                for(k=0; k<n-1; k++)
                    sum += m[i][k]*w[j][k];//CALCULA O NET
                
                sum += 1 * w[j][k];//BIAS

                if(sum > 0)//APLICA A FUNÇAO DE LIMIAR
                    sum = 1;
                else
                    sum = 0;

                exp = j == m[i][k]?1:0;//OBTEM O RESULTADO ESPERADO

                if(sum != exp)//VERIFICA SE O OBTIDO BATE COM O ESPERADO
                {
                    for(l=0; l<n-1; l++)
                        w[j][l] += lr * (exp - sum) * m[i][l];//CASO NAO BATA, REAJUSTA OS PESOS
                    w[j][l] += lr * (exp - sum) * 1;//BIAS
                }
            }
        }
        it++;
        
        //VALIDAÇAO
        if(vl_elem > 0)
        {
            l_hit = testPerceptron(vl_data, w, n+1, classes, vl_elem, 1);
            if(l_hit > hit)
                hit = l_hit;
            else
                overfitting++;
        }
    }
    
    fprintf(stdout, "\n\n");
    for(i=0; i<classes; i++)
    {
        for(j=0; j<n-1;j++)
            fprintf(stdout,"%.2f,", w[i][j]);
        fprintf(stdout,"%.2f\n", w[i][j]);
    }
    fprintf(stdout, "\n\n");
    
    //fprintf(stdout, "LEARNING RATE: %.2f ITERATIONS: %d\n", lr, it);
    
    return w;
}

//data = NOME DO ARQUIVO DE TESTES
//w = MATRIZ DE PESOS
//n = QUANTIDADE DE ENTRADAS (DIMENSOES) +1
//classes = QUANTIDADE DE CLASSES DO PROBLEMA 
float testPerceptron(char *data, float **w, int n, int classes, int elements, int v)
{
    int i,j,k, result, success = 0;
    float sum, temp;
    float **m, **mc;
    m = initMatrix(data, elements, n);
    mc = initMatrix(NULL, classes, classes);
    
    for(i=0; i<elements; i++)
    {
        temp = -999;
        for(j=0; j<classes; j++)
        {
            sum = 0;
            for(k=0; k<n-1; k++)//APLICA PESOS
                sum += m[i][k]*w[j][k];
            
            sum += 1 * w[j][k];//BIAS
            

            if(sum > temp)//PROCURA O NEURONIO COM MAIOR ATIVAÇAO
            {
                temp = sum;
                result = j;
            }
        }
        
        //fprintf(stdout,"%.0f - %d\n", m[i][k], result);//EXIBE O "OBTIDO - ESPERADO" DE CADA LINHA
        
        if(result == m[i][k])
            success++;
        /*else if(v == 0)
            mc[(int)(m[i][k])][result]++;*/
    }
    
    /*if(v == 0)
        for(i=0; i<classes; i++)
        {
            for(j=0; j<classes;j++)
                fprintf(stdout,"%d ", mc[i][j]);
            fprintf(stdout,"%d\n", mc[i][j]);
        }*/
    
    //fprintf(stdout,"%.1f%% \t",(float)success/elements*100);
    
    return ((float)success/elements)*100;
}