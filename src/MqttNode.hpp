/*
 * Node that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.0.0
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
  WiFiClient _wifiClient;
  PubSubClient *_mqtt;
  MqttFrame *_mqttFrame;

  String _name;

  void reconnect();

protected:
  virtual void loop() override;
  void callback(char *topic, byte *payload, unsigned int length);

public:
  MqttNode(const char *name);

  void beforeSetup();
  void setupHandler();
};
