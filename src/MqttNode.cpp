/*
 * Node that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

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

  _mqtt->setCallback(
    [this](char *topic, byte *payload, unsigned int length)
    {this->callback(topic, payload, length);}
  );

  if (!_mqtt->connected()) {
    reconnect();
  }
};

bool has_suffix(const std::string &str, const std::string &suffix)
{
  return str.size() >= suffix.size() &&
    str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void MqttNode::callback(char* topic, byte* payload, unsigned int length) {
  Homie.getLogger() << "Mqtt message [" << topic << "][" << length << "] byte" << endl;
  // for (int i = 0; i < length; i++) {
  //   Serial.print((char)payload[i]);
  // }
  if (has_suffix(topic, "$type")) {
    // _name = "Outdoor";
    _name = "";
    for (int i = 0; i < length; i++) {
      _name = _name + (char)payload[i];
    }
  }    
  if (has_suffix(topic, "temperature")) {
    _temp = "";
    for (int i = 0; i < length; i++) {
      _temp = _temp + (char)payload[i];
    }
    _temp.concat("°C");
  }
  if (has_suffix(topic, "humidity")) {
    _humid = "";
    for (int i = 0; i < length; i++) {
      _humid = _humid + (char)payload[i];
    }
    _humid.concat("%");
  }
}

void MqttNode::reconnect() {
  if (mqttServer.wasProvided() && mqttTopic.wasProvided()) {
    Homie.getLogger() << "  ◦ Connecting to: " << mqttServer.get();

    _mqtt->setServer(mqttServer.get(), 1883);
    if (_mqtt->connect(Homie.getConfiguration().deviceId)) {
      Homie.getLogger() << " OK" << endl;
      if (mqttTopic.wasProvided()) {
        Homie.getLogger() << "  ◦ Subscribing to: " << mqttTopic.get();
        if (_mqtt->subscribe(mqttTopic.get()))
          Homie.getLogger() << " OK" << endl;
        else
          Homie.getLogger() << " Failed" << endl;
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

//  display.setFont(ArialMT_Plain_24);
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(63 + x, 16 + y, _temp);
  display.drawString(63 + x, 32 + y, _humid);
};

void MqttNode::loop() {
  if (!_mqtt->connected()) {
    reconnect();
  }
  _mqtt->loop();
}