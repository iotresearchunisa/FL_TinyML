#include "library.h"

int _inputLayers = 0;
int _outputLayers = 0;

const int lineCharLength = 30;
char line[lineCharLength];

void initSdCard() {
  Serial.print("Initializing SD card...");
#if defined(ESP8266)
  if (!SD.begin(D8)) {  //D8 ESP8266
#elif defined(ESP32)
  if (!SD.begin(5)) {  //5 ESP32 
#else
  if (!SD.begin(4)) {  //4 arduino 
#endif
    Serial.println("initialization failed!");
    while (1)
      ;
  }
  Serial.println("initialization done.");
}

bool readData(File in, float* input, float* output) {
  float value;

  for (int i = 0; i < _inputLayers; i++) input[i] = 0;
  for (int i = 0; i < _outputLayers; i++) output[i] = 0;
  for (int i = 0; i < lineCharLength; i++) line[i] = '\0';

  int i = 0, res;
  char c;
  bool isOutput = false;

  if (in.available()) {
    int v = 0;
    while (true) {
      res = in.read();

      if (res == EOF) return false;
      c = (char)res;

      if (c == '\n' || c == '\r') {
        if (!isOutput) {
          if (i == 0) continue;
          i = 0;
        }
        float value = atof(line);
        output[i++] = value;
        return true;
      }

      if ((int)c < 0) {
        return false;
      }

      if (c == ',') {
        v = 0;
        value = atof(line);
        for (int m = 0; m < lineCharLength; m++) line[m] = '\0';

        if (!isOutput && i >= _inputLayers) {
          isOutput = true;
          i = 0;
        }

        if (isOutput && i >= _outputLayers) {
          return true;
        } else if (isOutput && i < _outputLayers) {
          output[i++] = value;
          if (i >= _outputLayers) {
            return true;
          }
        } else if (!isOutput && i < _inputLayers)
          input[i++] = value;
        else {
          //Serial.println("return F 5");

          return false;
        }
      } else
        line[v++] = c;
    }

    //Serial.println("return F 2");
    return false;
  }
  //Serial.println("return F 3");

  return false;
}

// Randomly shuffles a data object.
/*void _shuffle(float** input, float** output, int numberRows) {
  for (int a = 0; a < numberRows; a++) {
    const int b = rand() % numberRows;
    float* ot = output[a];
    float* it = input[a];
    // Swap output.
    output[a] = output[b];
    output[b] = ot;
    // Swap input.
    input[a] = input[b];
    input[b] = it;
  }
}
*/

void initDataframe(int inputLayers, int outputLayers) {
  // int memory = GetFreeSize();
  // int split = 1;//(memory * .8) / (sizeof(float *) * inputLayers * 1.3);

  // if (split <= 0) split = 1;
  // if (split > 1000) split = 1000;

  //*input = malloc(split * sizeof(float *));
  //*output = malloc(split * sizeof(float *));

  // for (int i = 0; i < split; i++) {
  //(*input) = malloc(inputLayers * sizeof(float));
  //(*output) = malloc(outputLayers * sizeof(float));
  // }

  _inputLayers = inputLayers;
  _outputLayers = outputLayers;

  // return split;
}

void saveWeightsJson(genann const* ann, String path, int items, float accuracy) {
  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Could not open file: weights");
    return;
  }

  file.println(items);
  file.println(accuracy, 3);

  for (int i = 0; i < ann->total_weights; ++i) {
    file.println(ann->weight[i], 20);
  }
  file.close();
}


void loadWeightsJson(genann const* ann, String path) {
  File file = SD.open(path);
  if (!file) {
    Serial.println("Could not open weights file");
    return;
  }

  int i = 0;
  int items;
  float accuracy;
  char c;
  if (!file.available()) return;

  //skip items and accuracy field
  while (true) {
    if (i == 2) break;
    c = (char)file.read();
    if (c == '\n')
      i++;
  }

  int m = 0;
  i = 0;
  for (m = 0; m < lineCharLength; m++) line[m] = '\0';
  m = 0;

  while (file.available()) {
    c = (char)file.read();

    if (c == '\n' || c == '\r') {
      if (m == 0) continue;

      float value = atof(line);
      ann->weight[i++] = value;
      for (m = 0; m < lineCharLength; m++) line[m] = '\0';
      m = 0;
    } else line[m++] = c;
  }
  file.close();
}
