#ifndef CLASSIFICATION_H
#define CLASSIFICATION_H
#include "library.h"

void predict(genann const* ann, float* input, float *output);

void resetMetrics();
void initMetrics(int outputLayers);

void printResult(File file);
float getMacroAccuracy();

int getNumberDataset();
void startTrainingTimer();
void printTrainingTimer(File file);

#endif /* TEST_H */