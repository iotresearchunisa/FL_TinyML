#ifndef LIBRARY_H
#define LIBRARY_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "genann.h"
#include <SPI.h>
#include <SD.h>

void initDataframe(int inputLayers, int outputLayers);

//Read lines from CSV file and populate the input and output dataframe and then shuffle. Return the number of line read
bool readData(File in, float *input, float *output);

void saveWeightsJson(genann const *ann, String path, int items, float accuracy);
void loadWeightsJson(genann const *ann, String path);

void initSdCard();


#endif /* TEST_H */