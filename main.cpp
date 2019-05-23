#include <stdlib.h>
#include <string.h>
#include "perceptron.h"

//PARAMETROS PARA RODAR OS 5 TESTES
//5 iris training_ testing_ NULL 105 45 0 4 3
//5 wine training_ testing_ NULL 127 51 0 13 3

//PARAMETROS PARA RODAR OS 5 TESTES COM VALIDACAO
//5 iris_v training_ testing_ validating_ 107 23 20 4 3
//5 wine_v training_ testing_ validating_ 127 27 24 13 3

int main(int argc, char** argv)
{
    int i, j, it;
    float **w, lr;
    int tr_elem, ts_elem, vl_elem;
    char *tmp_tr, *tmp_ts, *tmp_vl;
    int amount, atributes, classes;    
    char *tr_prefix, *ts_prefix, *vl_prefix, *path;
    
    amount = atoi(argv[1]);
    path = argv[2];
    tr_prefix = argv[3];
    ts_prefix = argv[4];
    vl_prefix = argv[5];
    tr_elem = atoi(argv[6]);
    ts_elem = atoi(argv[7]);
    vl_elem = atoi(argv[8]);
    atributes = atoi(argv[9]);
    classes = atoi(argv[10]);
    
    tmp_tr = new char[strlen(tr_prefix)+4];
    tmp_ts = new char[strlen(ts_prefix)+4];
    tmp_vl = new char[strlen(vl_prefix)+4];

    
    for(i=1; i<=amount; i++)
    {
        fprintf(stdout, "\\item %dst set\n", i);
        it = 250;
        for(int k=0; k<4; k++)
        {
            lr = 0.2;
            fprintf(stdout,"IT = %d ",it);
            for(j=0; j<4; j++)
            {
                sprintf(tmp_tr, "%s/%s%d.txt", path, tr_prefix, i);
                sprintf(tmp_ts, "%s/%s%d.txt", path, ts_prefix, i);
                sprintf(tmp_vl, "%s/%s%d.txt", path, vl_prefix, i);

                w = trainPerceptron(tmp_tr, NULL, atributes+1, classes, tr_elem, lr, 0, it, tmp_vl, vl_elem);

                fprintf(stdout,"(V %.2f)%.1f%% ", lr, testPerceptron(tmp_tr, w, atributes+1, classes, tr_elem, 0));
                fprintf(stdout,"(T %.2f)%.1f%% ", lr,  testPerceptron(tmp_ts, w, atributes+1, classes, ts_elem, 0));
                
                lr += 0.1;
            }
            fprintf(stdout, "\n\n");
            it += 250;
        }
    }
    return 0;
}