#ifndef _INPUT_TRAINING_DATA_H
#define _INPUT_TRAINING_DATA_H

#include "Common.h"

typedef struct
{
    uint32_t m;    //Size of the training data set.
    uint32_t n;    //Number of the features.
    //For univariate linear regression, pointer x points to a one-dimensional array, with the length of m.
    //For multivariate linear regression, pointer x points to a more-than 2 dimensional array.
    //                                    The size of the column is n and the size of the row is m.
    double* x;    //Input x
    double* y;    //Input y
}TrainingData_t;

TrainingData_t* InputTrainingData_FromFile(char* filePath, uint32_t m, uint32_t n);
TrainingData_t* InputTrainingData_MeanNorm(TrainingData_t* data);

#endif