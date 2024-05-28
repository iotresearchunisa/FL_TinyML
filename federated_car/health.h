#include "classification.h"
#include "mqtt.h"

//------------------------------------------------------------------------------
//Se si usa MKR1010, scrivere a mano i path dei file che fa i capricci se troppo lunghi

//Codice identificativo del dispositivo. Ogni scheda ne deve avere uno proprio
//Per i nodi che fanno training usare 1 - 2 - 3.
//Per i nodi passivi, usare un numero superiore a 3
const int nodeNumber = 0;

//Se true, la scheda non contribuisce al training, ma acquisisce solo i pesi
bool trainingDisabled = false;

//Se true, la scheda non eseguirà la fase di testing. 
//Da impostare su true se la scheda non è collegata a un lettore di schede SD 
bool testingDisabled = false;

//Se true inizializza le reti neurali con un modello precompilato salvato su scheda SD
//Se false i pesi sono randomizzati
bool useTransferLearning = true;

//Se true viene fatto fedAvg, altrimenti la versione semplificata
bool useFedAvg = true;
//------------------------------------------------------------------------------
const int numberInputLayer = 187;
const int numberOutputLayer = 5;
const int numberHiddenLayer = 2;
const int numberHiddenLayerNeurons = 25;
const int numberEpochs = 50;

String pathTrain = "";
String pathResult = "";
String pathSaveWeights = "";

const String pathTest = "/dataset/health/test.csv";


float input[numberInputLayer];
float output[numberOutputLayer];
genann* ann;

int weightIndex = 0;
bool waitingWeights = false;

File resultFile;
File trainSetFile;
File testSetFile;
float learningRate = 1.0f;
const float anneal = 0.995f;
int epoch = 0;


bool initFiles(){
    pathTrain = "/dataset/health/train" + String(nodeNumber) + ".csv";
    pathResult = "/resulthealth" + String(nodeNumber) + ".csv";
    pathSaveWeights = "/healthWeights" + String(nodeNumber) + ".txt";
  
    resultFile = SD.open(pathResult, FILE_WRITE);
    if (!resultFile) {
        Serial.println("Could not create result file.");
        while (1);
        return false;
    }

    if(!trainingDisabled){
        trainSetFile = SD.open(pathTrain);
        if (!trainSetFile) {
            Serial.print("Could not open file: ");
            Serial.println(pathTrain);
            while (1);
            return false;
        }
    }

    if(!testingDisabled){
        testSetFile = SD.open(pathTest);
        if (!testSetFile) {
            Serial.print("Could not open file: ");
            Serial.println(pathTest);
            while (1);
            return false;
        }
    }

    return true;
}

void execute(int epoch) {
    resetMetrics();
    Serial.println("Executing neural network...");

    trainSetFile.seek(0);
    testSetFile.seek(0);

    Serial.print("nodeNumber: ");Serial.print(nodeNumber);
    if (!trainingDisabled ) {
        Serial.print("\titeration: ");Serial.print(epoch+1);Serial.print("/");Serial.print(numberEpochs);
        Serial.print(" :: learning rate: ");Serial.print(learningRate);

        // TRAINING PHASE
        startTrainingTimer();
        while (readData(trainSetFile, input, output))
            genann_train(ann, input, output, learningRate);
        printTrainingTimer(resultFile);
        learningRate *= anneal;

        // ACCURACY TRAINING CALCULATOR
        trainSetFile.seek(0);
        resetMetrics();
        while (readData(trainSetFile, input, output))
            predict(ann, input, output);
        printResult(resultFile);
        Serial.print("\tTR: ");Serial.print(getMacroAccuracy());Serial.print("\t");
    }

    if(!testingDisabled){
        resetMetrics();
        while (readData(testSetFile, input, output)) {
            predict(ann, input, output);
        }
        printTrainingTimer(resultFile);printResult(resultFile);
        Serial.print("\tTE: ");Serial.print(getMacroAccuracy());Serial.print("\n");
    }

    if (!trainingDisabled) {
        saveWeightsJson(ann, pathSaveWeights, getNumberDataset(), getMacroAccuracy());
    }
}


#if defined(ESP32) || defined(ESP8266)
void callback(const char* topic, byte* payload, unsigned int length) {
#else
void callback(char* topic, byte * payload, unsigned int length) {
#endif
    waitingWeights = true;
    Serial.print("Receving weight: ");
    Serial.print(weightIndex+1);
    Serial.print("\\");
    Serial.print(ann->total_weights);

    ann->weight[weightIndex] = atof(reinterpret_cast<const char*>(payload));
    Serial.print("\t");
    Serial.println(ann->weight[weightIndex]);
    weightIndex++;
    if (weightIndex >= ann->total_weights) {
        //closeConnection();
        //initMQTT(nodeNumber);
        //awaitWeights(callback, nodeNumber, !trainingDisabled);
        Serial.println("All weights received");
        //closeConnection();
        //sleep(1000);
        weightIndex = 0;
        
        if(trainingDisabled && !testingDisabled)
          execute(0);
        waitingWeights = false;
    }
}


void trainingMode() {
    initMQTT(nodeNumber);

    if(!trainingDisabled || !testingDisabled){
      initSdCard();
      if(!initFiles()){
        Serial.println("Initialization failed. Exit");
        while(1);
        return;
      }
    }

    initDataframe(numberInputLayer, numberOutputLayer);
    initMetrics(numberOutputLayer);

    ann = genann_init(numberInputLayer, numberHiddenLayer, numberHiddenLayerNeurons, numberOutputLayer);
    if (ann == 0) {
        Serial.println("OUT MEMORY");
        while(1);
        return;
    }

    if(trainingDisabled){
        awaitWeights(callback, nodeNumber, !trainingDisabled);
        return;
    }

    srand(time(0));
    if (useTransferLearning){
        Serial.println("Loading transfer learning model...");
        loadWeightsJson(ann, "/healthWeights_start.txt");
        Serial.println("Transfer learning model loaded");
    }
    else genann_randomize(ann);

    Serial.println("Waiting other nodes for 1 minute...");
    delay(1*60*1000);
    Serial.println("starting training:");
       
    awaitWeights(callback, nodeNumber, !trainingDisabled);
}

void _loop() {
    if(!trainingDisabled && !waitingWeights && epoch < numberEpochs){
        closeConnection(); //disable MQTT before training to save memory
        execute(epoch);
        epoch++;
        initMQTT(nodeNumber); //enable MQTT
        awaitWeights(callback, nodeNumber, !trainingDisabled);
        if(epoch >= numberEpochs || useFedAvg){
            if(epoch >= numberEpochs){
                Serial.println("Training ended.");
                trainingDisabled = true;
                //resultFile.close();
                trainSetFile.close();
                //testSetFile.close();
            }else Serial.println("Waiting weights...");
            waitingWeights = true;
            sendWeights(nodeNumber, ann);
        }
    }
    mqttLoop();
}

