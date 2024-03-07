#include "InputTrainingData.h"

static inline void InputTrainingData_PrintData(TrainingData_t* data)
{
    uint32_t i, j;

    for (i = 0; i < data->m; i++)
    {
        for(j = 0; j < data->n; j++)
        {
            LOG_INFO("x(%d,%d) = %lf, ", i, j, data->x[i * data->n + j]);
        }
        LOG_INFO("y%d = %lf\n", i, data->y[i]);
    }
}

TrainingData_t* InputTrainingData_FromFile(char* filePath, uint32_t m, uint32_t n)
{
    FILE *fp;
    char line[256];
    char* token;
    uint32_t i, j;

    TrainingData_t* data = (TrainingData_t*)malloc(sizeof(TrainingData_t));
    data->x = (double*)malloc(sizeof(double) * m * n);
    data->y = (double*)malloc(sizeof(double) * m);
    data->m = m;
    data->n = n;

    fp = fopen(filePath, "r");

    for (i = 0; i < m; i++)
    {
        fgets(line, sizeof(line), fp);
        token = strtok(line, "\t");
        for(j = 0; j < n; j++)
        {
            sscanf(token, "%lf", &data->x[i * n + j]);
            token = strtok(NULL, "\t");
        }
        sscanf(token, "%lf", &data->y[i]);
    }

    InputTrainingData_PrintData(data);

    return data;
}

TrainingData_t* InputTrainingData_MeanNorm(TrainingData_t* data)
{
    double sum, mean, max, min;
    uint32_t i, j;

    for(j = 0;j < data->n; j++)
    {
        sum = 0;
        max = min = data->x[j];
        for(i = 0;i < data->m; i++)
        {
            sum += data->x[i * data->n + j];
            if(data->x[i * data->n + j] < min)
            {
                min = data->x[i * data->n + j];
            }
            if(data->x[i * data->n + j] > max)
            {
                max = data->x[i * data->n + j];
            }
        }
        mean = sum / data->m;
        for(i = 0;i < data->m; i++)
        {
            data->x[i * data->n + j] = (data->x[i * data->n + j] - mean) / (max - min);
        }
    }

    LOG_INFO("Data after mean normalization:\n");
    InputTrainingData_PrintData(data);

    return data;
}