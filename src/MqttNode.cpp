/*
 * Node that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include <Homie.hpp>
#include "MqttNode.hpp"

HomieSetting<const char*> mqttServer("MqttServer", "The MQTT server to which this node shall connect");
HomieSetting<const char*> mqttTopic("MqttTopic", "The MQTT topic to which this node shall listen");

MqttNode::MqttNode(const char *name) :
  HomieNode(name, "test")
{
  _name = name;
  _mqtt = new PubSubClient(_wifiClient);
}

void MqttNode::beforeSetup() {
  // This has to be called before Homie.setup, because otherwise the default Values will
  // override the values which were already read from config.json
  Homie.getLogger() << "• MqttNode - Before Setup" << endl;
  mqttServer.setDefaultValue(MQTT_SERVER);
  mqttTopic.setDefaultValue(MQTT_TOPIC);
}

void MqttNode::setupHandler() {
  Homie.getLogger() << "• MqttNode - Setuphandler" << endl;
  if (!_mqtt->connected()) {
    reconnect();
  }
};

void MqttNode::callback(char* topic, byte* payload, unsigned int length) {
  Homie.getLogger() << "Message arrived [" << topic << "] " << endl;
}

void MqttNode::reconnect() {
  if (mqttServer.wasProvided() && mqttTopic.wasProvided()) {
    Homie.getLogger() << "◦ Attempting MQTT connection...";

    _mqtt->setServer(mqttServer.get(), 1883);
    // _mqtt->setCallback(
    //   [this](char *topic, byte *payload, unsigned int length) {
    //   callback(topic, payload, length);
    // });

    if (_mqtt->connect(_name)) {
      Homie.getLogger() << " Connected" << endl;
      if (mqttTopic.wasProvided()) {
        _mqtt->subscribe("mqttTopic.get()");
      }
    }
    else 
      Homie.getLogger() << " Failed" << endl;
  }
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

void MqttNode::loop() {
  if (!_mqtt->connected()) {
    reconnect();
  }
  _mqtt->loop();
}