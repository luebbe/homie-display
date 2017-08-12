/*
 * Node that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#ifndef SRC_MQTTNODE_H_
#define SRC_MQTTNODE_H_

#include <HomieNode.hpp>
#include <OLEDIndexFrame.hpp>
#include <PubSubClient.h>

class MqttNode: public HomieNode, public OLEDIndexFrame {
private:
  const char *MQTT_SERVER = "MQTT_SERVER";
  const char *MQTT_TOPIC = "MQTT_TOPIC";
  const char *_name;
  WiFiClient _wifiClient;
  PubSubClient *_mqtt;
  
  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;
  
  void reconnect();
  
protected:
  virtual void loop() override;
  void callback(char* topic, byte* payload, unsigned int length);
  
public:
  MqttNode(const char *name);

  void beforeSetup();
  void setupHandler();
};

#endif
