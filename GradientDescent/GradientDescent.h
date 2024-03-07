#include "Common.h"

typedef struct
{
    double* w;          //actual weight
    double* tmp_w;      //temporary var during gradient descent calculation
    double b;
    double tmp_b;
    double J[2];        //Cost function value. 0:The latest, 1: Last value
    double a;           //Alpha, learning rate
    TrainingData_t* pTrainingData;

    //Polynominal calculation
    uint32_t cntItem;   //count of polynominal items
    //This array define the relation between items, suppose cntItem = 3, count of features(n) = 3, then:
    //e.g. x1 x2 x3
    //   0: 1  0  0
    //   1: 0  2  0
    //   2: 1  0  1
    //This means the f_wb(x) = x1 + x2^2 + x1*x3
    double* polyPowerArray;
}GradientDescent_t;

GradientDescent_t* GradientDescent_InitStruct(TrainingData_t* data, double* wInitArray, double bInit, double* polyInitArray, uint32_t cntItem, double aInit);
void GradientDescent_Calculate(GradientDescent_t* gd);