/*
 * Node that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.0.1
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <Homie.hpp>
#include <HomieNode.hpp>
#include <PubSubClient.h>

#include "MqttFrame.hpp"

class MqttNode : public HomieNode
{
private:
  const char *MQTT_SERVER = "MQTT_SERVER";
  const char *MQTT_TOPIC = "MQTT_TOPIC";
  const char *cTypeTopic = "$type";
  const char *cPropsTopic = "$properties";
  const char *cStatusTopic = "status";

  WiFiClient _wifiClient;
  PubSubClient *_mqtt;
  MqttFrame *_mqttFrame;

  unsigned long _lastTry = 0;

  std::string _name;
  std::vector<std::string> _values;
  std::vector<std::string> _units;

  bool hasSuffix(const std::string str, const std::string suffix);
  void getNodeProperties(const std::string value);
  std::string getPayload(byte *payload, unsigned int length);

  void reconnect();
  void subscribeTo(const char *subtopic);
  void unsubscribeFrom(const char *subtopic);

protected:
  virtual void loop() override;
  void callback(char *topic, byte *payload, unsigned int length);

public:
  MqttNode(const char *name);

  void beforeSetup();
  void setupHandler();
};
