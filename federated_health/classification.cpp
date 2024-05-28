#include "classification.h"

int totals = 0;
unsigned short confusionMatrix[5][5];
int __outputLayers;

float mse = 0, multiClassEntropy = 0;
unsigned long testTime;

int higherIndex(const float* output) {
    float t = -1;
    int index = -1;
    for (int i = 0; i < __outputLayers; i++) {
        if (output[i] > t) {
            t = output[i];
            index = i;
        }
    }
    return index;
}

void predict(genann const* ann, float* input, float* output) {
    const float* pd = genann_run(ann, input);

    totals++;
    int indexOutput = higherIndex(output);
    int indexPredicted = higherIndex(pd);

    confusionMatrix[indexPredicted][indexOutput]++;

    multiClassEntropy += -log(pd[indexPredicted]);
    mse += powf(1 - pd[indexPredicted], 2);
}

void resetMetrics() {
    mse = 0;
    totals = 0;
    mse = 0; multiClassEntropy = 0;
    for (int i = 0; i < __outputLayers; i++)
        for (int m = 0; m < __outputLayers; m++)
            confusionMatrix[i][m] = 0;
    testTime = millis();
}

void initMetrics(int outputLayers) {
    __outputLayers = outputLayers;
    //confusionMatrix = (int**)malloc(outputLayers * sizeof(int*));
    //for (int i = 0; i < outputLayers; i++) {
    //    confusionMatrix[i] = (int*)malloc(outputLayers * sizeof(int*));
    //}
}

float getTP(int _class) {
    return confusionMatrix[_class][_class];
}

float getTN(int _class) {
    float toReturn = 0;
    for (int i = 0; i < __outputLayers; i++) {
        if (i != _class) {
            for (int m = 0; m < __outputLayers; m++) {
                if (m != _class)
                    toReturn += confusionMatrix[i][m];
            }
        }
    }
    return toReturn;
}

float getFP(int _class) {
    float toReturn = 0;
    for (int i = 0; i < __outputLayers; i++) {
        if (i != _class)
            toReturn += confusionMatrix[_class][i];
    }
    return toReturn;
}

float getFN(int _class) {
    float toReturn = 0;
    for (int i = 0; i < __outputLayers; i++) {
        if (i != _class)
            toReturn += confusionMatrix[i][_class];
    }
    return toReturn;
}

float getAccuracy(int _class) {
    float Tp = getTP(_class), Tn = getTN(_class), Fp = getFP(_class), Fn = getFN(_class);
    return (Tp + Tn) / (Tp + Tn + Fp + Fn);
}

float getPrecision(int _class) {
    float Tp = getTP(_class), Fp = getFP(_class);
    if(Tp == 0 && Fp == 0) return 1;
    return (Tp / (Tp + Fp));
}

float getRecall(int _class) {
    float Tp = getTP(_class), Fn = getFN(_class);
    if(Tp == 0 && Fn == 0) return 1;
    return (Tp / (Tp + Fn));
}

float getF1(int _class) {
    float p = getPrecision(_class);
    float r = getRecall(_class);
    return 2 * ((p * r) / (p + r));
}

float getMacroAccuracy() {
    float toReturn = 0;
    for (int i = 0; i < __outputLayers; i++) {
        toReturn += confusionMatrix[i][i];
    }
    return toReturn / totals;
}

float getMicroAccuracy(){
    return getMacroAccuracy();
}


float getMacroPrecision() {
    float toReturn = 0;
    for (int i = 0; i < __outputLayers; i++) {
        toReturn += getPrecision(i);
    }
    return toReturn / __outputLayers;
}

float getMicroPrecision() {
    float Tp = 0, Fp = 0;
    for (int i = 0; i < __outputLayers; i++) {
        Tp += getTP(i);
        Fp += getFP(i);
    }
    return Tp / (Tp+ Fp);
}


float getMacroRecall() {
    float toReturn = 0;
    for (int i = 0; i < __outputLayers; i++) {
        toReturn += getRecall(i);
    }
    return toReturn / __outputLayers;
}


float getMicroRecall() {
    float Tp = 0, Fn = 0;
    for (int i = 0; i < __outputLayers; i++) {
        Tp += getTP(i);
        Fn += getFN(i);
    }
    return (Tp / (Tp + Fn));
}


float getMacroF1() {
    float toReturn = 0;
    for (int i = 0; i < __outputLayers; i++) {
        toReturn += getF1(i);
    }
    return toReturn / __outputLayers;
}

float getMicroF1() {
    float p = getMicroPrecision();
    float r = getMicroRecall();
    return 2 * ((p * r) / (p + r));
}

void printConfusion() {
    printf("\n");
    for (int i = 0; i < __outputLayers; i++) {
        for (int m = 0; m < __outputLayers; m++)
            printf("%d\t", confusionMatrix[i][m]);
        printf("\n");
    }
}

float getMse() {
    return mse / totals;
}

float getMultiClassEntropy() {
    return multiClassEntropy / totals;
}

void startTrainingTimer() {
    testTime = millis();
}

void printSdValue(File file, float value) {
    file.print(value, 4); file.write(',');
}

void printTrainingTimer(File file) {
    printSdValue(file, (float)(millis() - testTime));
}

void printResult(File file) {
    printSdValue(file, (float)(millis() - testTime));
    for (int i = 0; i < __outputLayers; i++) {
        printSdValue(file, getAccuracy(i));
        printSdValue(file, getPrecision(i));
        printSdValue(file, getRecall(i));
        printSdValue(file, getF1(i));
    }
    printSdValue(file, getMacroAccuracy());
    printSdValue(file, getMacroPrecision());
    printSdValue(file, getMacroRecall());
    printSdValue(file, getMacroF1());
    printSdValue(file, getMicroAccuracy());
    printSdValue(file, getMicroPrecision());
    printSdValue(file, getMicroRecall());
    printSdValue(file, getMicroF1());
    printSdValue(file, getMse());
    file.println(getMultiClassEntropy(), 4);
}

int getNumberDataset() {
    return totals;
}

