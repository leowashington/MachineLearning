#include "InputTrainingData.h"
#include "GradientDescent.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
TrainingData_t* data;
GradientDescent_t* gd;

int main(int argc, char *argv[])
{
    char ch;

    double initW[] = {0, 0};
    double initPoly[] = {2, 1};   //f(x) = w1* x1 + b
    data = InputTrainingData_FromFile("mydata_univar_x2+2x+1.txt", 10, 1);  //mydata_multi.txt  mydata_univar.txt|10 mydata_practice_univar.txt|97 mydata_univar_x2+2x+1.txt
    //data = InputTrainingData_MeanNorm(data);
    gd   = GradientDescent_InitStruct(data, initW, 10, initPoly, 2, 1e-6);

    GradientDescent_Calculate(gd);

    LOG_CRITICAL("Program end!\n");
    scanf("%c", &ch);

    free(data);
    free(gd);

    return 0;
}