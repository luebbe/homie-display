/*
 * Node that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "MqttNode.hpp"

HomieSetting<const char *> mqttServer("MqttServer", "The MQTT server to which this node shall connect");
HomieSetting<const char *> mqttTopic("MqttTopic", "The MQTT topic to which this node shall listen");
HomieSetting<const char *> mqttTitle("MqttTitle", "The title that shall be shown on the frame");

MqttNode::MqttNode(const char *name) : HomieNode(name, "test")
{
  _name = name;
  _mqtt = new PubSubClient(_wifiClient);
  _mqttFrame = new MqttFrame(name);
}

void MqttNode::beforeSetup()
{
  // This has to be called before Homie.setup, because otherwise the default Values will
  // override the values which were already read from config.json
  Homie.getLogger() << "• MqttNode - Before Setup" << endl;
  mqttServer.setDefaultValue(MQTT_SERVER);
  mqttTopic.setDefaultValue(MQTT_TOPIC);
  mqttTitle.setDefaultValue("");
}

void MqttNode::setupHandler()
{
  Homie.getLogger() << "• MqttNode - Setuphandler" << endl;

  if (mqttTitle.wasProvided())
  {
    _name = mqttTitle.get();
  }

  _mqtt->setCallback(
      [this](char *topic, byte *payload, unsigned int length) { this->callback(topic, payload, length); });

  if (!_mqtt->connected())
  {
    reconnect();
  }
};

bool has_suffix(const std::string &str, const std::string &suffix)
{
  return str.size() >= suffix.size() &&
         str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void MqttNode::callback(char *topic, byte *payload, unsigned int length)
{
  Homie.getLogger() << "Mqtt message [" << topic << "][" << length << "] byte" << endl;
  // for (int i = 0; i < length; i++) {
  //   Serial.print((char)payload[i]);
  // }
  if (!mqttTitle.wasProvided())
  {
    if (has_suffix(topic, "$type"))
    {
      _name = "";
      for (int i = 0; i < length; i++)
      {
        _name = _name + (char)payload[i];
      }
      _mqttFrame->setName(_name);
    }
  }

  if (has_suffix(topic, "temperature"))
  {
    String _temp = "";
    for (int i = 0; i < length; i++)
    {
      _temp = _temp + (char)payload[i];
    }
    _temp.concat("°C");
    _mqttFrame->setTemperature(_temp);
  }
  if (has_suffix(topic, "humidity"))
  {
    String _humid = "";
    for (int i = 0; i < length; i++)
    {
      _humid = _humid + (char)payload[i];
    }
    _humid.concat("%");
    _mqttFrame->setHumidity(_humid);
  }
}

void MqttNode::reconnect()
{
  if (mqttServer.wasProvided() && mqttTopic.wasProvided())
  {
    Homie.getLogger() << "  ◦ Connecting to: " << mqttServer.get();

    _mqtt->setServer(mqttServer.get(), 1883);
    if (_mqtt->connect(Homie.getConfiguration().deviceId))
    {
      Homie.getLogger() << " OK" << endl;
      if (mqttTopic.wasProvided())
      {
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

void MqttNode::loop()
{
  if (!_mqtt->connected())
  {
    reconnect();
  }
  _mqtt->loop();
}