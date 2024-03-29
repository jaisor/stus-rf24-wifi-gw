#pragma once

#if defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Print.h>
#include <vector>

#include "BaseManager.h"
#include "BaseMessage.h"
#include "wifi/SensorProvider.h"
#include "RF24MessageQueue.h"

typedef enum {
  WF_CONNECTING = 0,
  WF_LISTENING = 1
} wifi_status;

class CWifiManager: public CBaseManager {

private:
  ISensorProvider *sensorProvider;
  IMessageQueue *messageQueue;
  
  bool rebootNeeded;
  uint8_t wifiRetries;
  unsigned long tMillis;
  wifi_status status;
  char softAP_SSID[32];
  char SSID[32];
  char mqttSubcribeTopicConfig[255];
  unsigned long tsAPReboot;
  
  AsyncWebServer* server;
  PubSubClient mqtt;

  StaticJsonDocument<2048> sensorJson;
  StaticJsonDocument<2048> configJson;
  StaticJsonDocument<2048> rfJson;

  void connect();
  void listen();

  void handleRoot(AsyncWebServerRequest *request);
  void handleConnect(AsyncWebServerRequest *request);
  void handleConfig(AsyncWebServerRequest *request);
  void handleFactoryReset(AsyncWebServerRequest *request);

  void printHTMLTop(Print *p);
  void printHTMLBottom(Print *p);

  void postSensorUpdate();
  void processQueue();
  bool isApMode();

  void mqttCallback(char *topic, uint8_t *payload, unsigned int);
  bool ensureMQTTConnected();
  const String getTopicForMessageId(uint8_t msgId);

public:
	CWifiManager(ISensorProvider *sensorProvider, IMessageQueue *messageQueue);
  virtual void loop();

  virtual const bool isRebootNeeded() { return rebootNeeded; }
};
