#ifndef REGRESSION_H
#define REGRESSION_H
#include "library.h"

void predict(genann const* ann, float* input, float *output);

void resetMetrics();
void initMetrics(int outputLayers);

void printResult(File file);
float getAccuracy();

int getNumberDataset();
void startTrainingTimer();
void printTrainingTimer(File file);

#endif /* TEST_H */