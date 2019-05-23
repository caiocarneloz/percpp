#ifndef TRAINING_H
#define TRAINING_H
#include <stdio.h>
#include <stdlib.h>

float **trainPerceptron(char *data, char *weights, int n, int classes, int elements, float lr, bool sc, float value, char *vl_data, int vl_elem);
float testPerceptron(char *data, float **w, int n, int classes, int elements, int v);

#endif

