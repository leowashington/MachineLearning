#include "InputTrainingData.h"
#include "GradientDescent.h"

static inline double myabsf(double x)
{
    return (x>0?x:-x);
}

static inline void GradientDescent_PrintStruct(GradientDescent_t* gd)
{
    uint32_t i;

    for(i = 0;i < gd->cntItem; i++)
    {
        LOG_INFO("w%d = %lf, ", i, gd->w[i]);
    }
    LOG_INFO("\nb = %lf, a = %lf, J = %lf\n", gd->b, gd->a, gd->J[0]);
}

GradientDescent_t* GradientDescent_InitStruct(TrainingData_t* data, double* wInitArray, double bInit, double* polyInitArray, uint32_t cntItem, double aInit)
{
    GradientDescent_t* gd = (GradientDescent_t*)malloc(sizeof(GradientDescent_t));
    uint32_t i, j;
    uint32_t cntValid = 0;

    gd->cntItem = cntItem;
    gd->w = (double*)malloc(sizeof(double) * gd->cntItem);
    memcpy(gd->w, wInitArray, sizeof(double) * gd->cntItem);
    gd->tmp_w = (double*)malloc(sizeof(double) * gd->cntItem);
    gd->b = bInit;
    gd->tmp_b = 0;
    gd->pTrainingData = data;
    gd->a = aInit;
    gd->J[0] = 0; gd->J[1] = 0;

    gd->polyPowerArray = (double *)malloc(sizeof(double) * data->n * gd->cntItem);
    memcpy(gd->polyPowerArray, polyInitArray, sizeof(double) * data->n * gd->cntItem);

    LOG_INFO("Init gradient descent struct:\n");
    GradientDescent_PrintStruct(gd);

    LOG_INFO("f_wb(x) = ");
    for (i = 0; i < gd->cntItem; i++)
    {
        LOG_INFO("w%d ", i + 1);
        for(j = 0; j < data->n; j++)
        {
            LOG_INFO("* ");
            if(gd->polyPowerArray[i * data->n + j] == 1)
            {
                LOG_INFO("x%d ", j + 1);
            }
            else
            {
                LOG_INFO("x%d ^ %.2lf ", j + 1, gd->polyPowerArray[i * data->n + j]);
            }
        }
        if(i != cntItem - 1)
        {
            LOG_INFO("+ ");
        }
    }

    return gd;
}

double GradientDescent_CalculateFw(GradientDescent_t* gd, uint32_t i_idx)
{
    uint32_t i, j;
    double ret = 0.0f;
    double item = 0.0f;
    TrainingData_t* data = gd->pTrainingData;

    for (i = 0; i < gd->cntItem; i++)
    {
        item = gd->w[i];
        for (j = 0; j < data->n; j++)
        {
            if (gd->polyPowerArray[i * data->n + j] != 0)
            {
                item *= powf(data->x[i_idx * data->n + j], gd->polyPowerArray[i * data->n + j]);
            }
        }
        ret += item;
    }

    return ret;
}

double GradientDescent_Calculatedw(GradientDescent_t* gd, uint32_t j_idx)
{
    uint32_t i, j;
    TrainingData_t* data = gd->pTrainingData;
    double sum = 0.0f;
    double err;

    for (i = 0; i < data->m;i++)
    {
        err = 0.0f;
        for (j = 0; j < data->n; j++)
        {
            err += GradientDescent_CalculateFw(gd, i);
        }
        err += gd->b;
        err -= data->y[i];
        err *= data->x[i * data->n + j_idx];
        sum += err;
    }

    sum /= data->m;

    return sum;
}

double GradientDescent_Calculatedb(GradientDescent_t* gd)
{
    uint32_t i, j;
    TrainingData_t* data = gd->pTrainingData;
    double sum = 0.0f;
    double err;

    for(i = 0;i < data->m;i++)
    {
        err = 0.0f;
        for(j = 0;j < data->n; j++)
        {
            err += GradientDescent_CalculateFw(gd, i);
        }
        err += gd->b;
        err -= data->y[i];
        sum += err;
    }

    sum /= data->m;

    return sum;
}

void GradientDescent_CalculateCostFunc(GradientDescent_t* gd)
{
    uint32_t i, j;
    TrainingData_t* data = gd->pTrainingData;
    double sum = 0.0f;
    double err;

    for (i = 0;i < data->m;i++)
    {
        err = 0.0f;
        for (j = 0;j < data->n; j++)
        {
            err += GradientDescent_CalculateFw(gd, i);
        }
        err += gd->b;
        err -= data->y[i];
        err = err * err;
        sum += err;
    }

    sum /= (data->m * 2);

    gd->J[1] = gd->J[0];
    gd->J[0] = sum;
}

void GradientDescent_CalculateOneStep(GradientDescent_t* gd)
{
    uint32_t j;
    TrainingData_t* data = gd->pTrainingData;

    for(j = 0;j < gd->cntItem; j++)
    {
        gd->tmp_w[j] = gd->w[j] - gd->a * GradientDescent_Calculatedw(gd, j);
    }
    gd->tmp_b = gd->b - gd->a * GradientDescent_Calculatedb(gd);

    for(j = 0;j < gd->cntItem; j++)
    {
        gd->w[j] = gd->tmp_w[j];
    }
    gd->b = gd->tmp_b;

    GradientDescent_CalculateCostFunc(gd);
}

void GradientDescent_PrintOutput(GradientDescent_t* gd)
{
    uint32_t i, j;
    TrainingData_t* data = gd->pTrainingData;
    FILE *gp = popen("gnuplot -persist", "w");

    if(gp == NULL)
    {
        printf("Error opening pipe to gnuplot.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(gp, "set terminal wxt\n");
    fprintf(gp, "plot '-' w p ls 1, ");
    for (i = 0; i < gd->cntItem; i++)
    {
        fprintf(gp, "%lf", gd->w[i]);
        for(j = 0; j < data->n; j++)
        {
            fprintf(gp, "*");
            if(gd->polyPowerArray[i * data->n + j] == 1)
            {
                fprintf(gp, "x");
            }
            else
            {
                fprintf(gp, "x**%lf", gd->polyPowerArray[i * data->n + j]);
            }
        }
        if(i != gd->cntItem - 1)
        {
            fprintf(gp, "+");
        }
        else
        {
            fprintf(gp, "+%lf\n", gd->b);
        }
    }
    for (i = 0;i < data->m; i++)
    {
        fprintf(gp, "%lf %lf\n", data->x[i], data->y[i]);
    }
    fprintf(gp, "e\n");
    pclose(gp);
}

void GradientDescent_Calculate(GradientDescent_t* gd)
{
    uint32_t cntStep = 0;
    bool flag_debug = TRUE;
    uint32_t i, j;
    TrainingData_t* data = gd->pTrainingData;
    char ctrlFlag;

    LOG_INFO("\n++++++++++++++++++++++++++++++\n");
    LOG_INFO("Gradient descent calculation start!\nInitial values:\n");
    GradientDescent_CalculateCostFunc(gd);
    GradientDescent_PrintStruct(gd);

    LOG_CRITICAL("Input c to start loop, press ENTER to single step debug.\n");
    scanf("%c", &ctrlFlag);

    while(1)
    {
        GradientDescent_CalculateOneStep(gd);

        LOG_INFO("\n++++++++++++++++++++++++++++++\n");
        LOG_INFO("Step[%d]:\n", cntStep);
        GradientDescent_PrintStruct(gd);
        if(gd->J[0] > gd->J[1])
        {
            LOG_CRITICAL("Gradient descent not descending at step %d!\n", cntStep);
        }
        cntStep++;

        //if(gd->J[1] - gd->J[0] < 1e-6 || cntStep > 1 * 1000 * 1000)
        if(gd->J[0] < 1e-6 || cntStep > 1 * 1000 * 1000)
        {
            LOG_CRITICAL("\n******************************\n");
            LOG_CRITICAL("Final loop times = %d\n", cntStep);
            for(i = 0;i < gd->pTrainingData->n; i++)
            {
                LOG_CRITICAL("w%d = %lf, ", i, gd->w[i]);
            }
            LOG_CRITICAL("\nb = %lf, a = %lf, J = %lf\n", gd->b, gd->a, gd->J[0]);
            LOG_CRITICAL("Gradient descent END!\n");

            GradientDescent_PrintOutput(gd);
            break;
        }

        switch(ctrlFlag)
        {
            case 'c':
            break;

            case '\n':
                scanf("%c", &ctrlFlag);
            break;

            default:
            break;
        }
    }
}