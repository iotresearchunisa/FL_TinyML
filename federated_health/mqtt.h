#ifndef MQTT_h
#define MQTT_h

#include <PubSubClient.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h> //WiFiEsp   WiFi.h
#else
  #include <WiFiNINA.h>
#endif

#include "library.h"

void closeConnection();
void mqttLoop();
void initMQTT(int node);
void sendWeights(int node, genann const* ann);

#if defined(ESP32) || defined(ESP8266)
  void awaitWeights(void (*callback)(const char*, uint8_t*, unsigned int), int node, bool train);
#else
  void awaitWeights(void (*callback)(char*, uint8_t*, unsigned int), int node, bool train);
#endif

#endif