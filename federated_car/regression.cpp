#include "regression.h"

int totals = 0;
int correct = 0;
unsigned long testTime;

float mae = 0, mse = 0;


bool isSimilarNumber(const float* predicted, float* output) {
    float result = fabs(predicted[0] - output[0]);
    if (result >= .19) {
    }
    return result < 0.19;
}


void predict(genann const* ann, float* input, float* output) {
    const float* pd = genann_run(ann, input);
    totals++;

    if (isSimilarNumber(pd, output))
        correct++;

    mae += fabs(output[0] - pd[0]);
    mse += powf(output[0] - pd[0], 2);
}

void resetMetrics() {
    mae = 0; mse = 0;
    totals = 0;
    correct = 0;
    testTime = millis();
}

void initMetrics(int outputLayers) {
}

float getAccuracy() {
    return (float)correct / (float)totals;
}

float getMae() {
    return mae / totals;
}


float getMse() {
    return mse / totals;
}

float getRmse() {
    return sqrt(getMse());
}

float getRSquared() {
    return mse;
}

void startTrainingTimer(){
    testTime = millis();
}

void printSdValue(File file, float value){
    file.print(value,4); file.write(',');
}

void printTrainingTimer(File file){
    printSdValue(file, (float)(millis() - testTime));
}


void printResult(File file) {
    printSdValue(file, (float)(millis() - testTime));
    printSdValue(file, getAccuracy());
    printSdValue(file, getMae());
    printSdValue(file, getMse());
    printSdValue(file, getRmse());
    file.println(getRSquared(),4);
}

int getNumberDataset() {
    return totals;
}

