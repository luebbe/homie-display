/*
 * Class that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.1.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#pragma once

#include <Homie.hpp>

// #include "MqttFrame.hpp"

class MqttCollector
{
private:
  const char *MQTT_SERVER = "MQTT_SERVER";
  const char *MQTT_TOPIC = "MQTT_TOPIC";
  const char *cTypeTopic = "$type";
  const char *cPropsTopic = "$properties";
  const char *cStatusTopic = "status";
  const char *cUnitTopic = "$unit";

  unsigned long _lastTry = 0;

  std::string _name;
  std::vector<std::string> _values;
  std::vector<std::string> _units;

  bool hasSuffix(const std::string str, const std::string suffix);
  void getNodeProperties(char *payload, size_t len);

  void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
  void reconnect();
  void subscribeTo(const char *topic);
  void subscribeToSubtopics(const char *topic);
  void unsubscribeFrom(const char *topic);

  // protected:
  //   virtual void loop() override;

public:
  explicit MqttCollector(const char *name, int something);
  void beforeSetup();
  void onReadyToOperate();
};
