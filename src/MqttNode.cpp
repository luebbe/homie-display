/*
 * Node that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include <Homie.hpp>
#include "MqttNode.hpp"

HomieSetting<const char*> mqttTopic("MqttTopic", "The MQTT topic to which this node shall listen");

const char * MQTT_TOPIC = "MQTT_TOPIC";

MqttNode::MqttNode(const char *name) :
  HomieNode(name, "test")
{
  _name = name;
  mqttTopic.setDefaultValue(MQTT_TOPIC);
}

void MqttNode::beforeSetup() {
  // This has to be called before Homie.setup, because otherwise the default Values will
  // override the values which were already read from config.json
  Homie.getLogger() << "• MqttNode Before Setup" << endl;
  mqttTopic.setDefaultValue(MQTT_TOPIC);
}

// Interface OLEDFrame
void MqttNode::drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(x, 0, _name);
  // + ':' + String(x) + ' ' + String(y));

  String temp("22,5");
  String hum("55.5");

  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  temp.concat("°C");
  display.drawString(63 + x, 16 + y, temp);
  display.setFont(ArialMT_Plain_16);
  hum.concat("% rel");
};

void MqttNode::setup() {
  Homie.getLogger() << "MqttNode Setup" << endl;
  if (mqttTopic.wasProvided()) {
    Homie.getLogger() << "Topic:" << mqttTopic.get() << endl;
  }
};

void MqttNode::loop() {

};
