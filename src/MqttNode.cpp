/*
 * Node that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.0.1
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
      [this](char *topic, byte *payload, unsigned int length) {
        this->callback(topic, payload, length);
      });

  if (!_mqtt->connected())
  {
    reconnect();
  }
};

bool MqttNode::hasSuffix(const std::string str, const std::string suffix)
{
  return str.size() >= suffix.size() &&
         str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string MqttNode::getPayload(byte *payload, unsigned int length)
{
  std::string result = "";
  for (int i = 0; i < length; i++)
  {
    result = result + (char)payload[i];
  }
  return result;
}

void MqttNode::getNodeProperties(const std::string value)
{
  // Parse comma separated node properties
  char *pch;
  char *buf = new char[value.size() + 1];
  std::copy(value.begin(), value.end(), buf);
  buf[value.size()] = '\0'; // don't forget the terminating 0

  pch = strtok(buf, ",");
  while (pch != NULL)
  {
    // put them in the units or values basket and tell the display frame about them
    if (hasSuffix(pch, "/unit"))
    {
      _units.push_back(pch);
      _mqttFrame->addUnit(pch); // could add any string here.
    }
    else
    {
      _values.push_back(pch);
      _mqttFrame->addValue(pch); // could add any string here.
    }
    // finally subscribe to the corresponding topics and continue to parse
    subscribeTo(pch);
    pch = strtok(NULL, ",");
  }
  delete[] buf;
}

void MqttNode::callback(char *topic, byte *payload, unsigned int length)
{
  std::string value = getPayload(payload, length);
  Homie.getLogger() << "  ◦ Received: " << topic << " " << value.c_str() << endl;

  if (hasSuffix(topic, "$type"))
  {
    // type is used as display name for the frame when nothing was provided in the config
    if (!mqttTitle.wasProvided())
      _mqttFrame->setName(value);
  }

  else if (hasSuffix(topic, "$properties"))
  {
    // autodetect and subscribe to all properties
    if (!_mqttFrame->getIsConfigured())
      getNodeProperties(value);
  }

  else
  {
    // retrieve the propeties to which we have subscribed earlier
    for (int i = 0; i < _units.size(); i++)
    {
      if (hasSuffix(topic, _units[i]))
        _mqttFrame->setUnit(i, value);
    }

    for (int i = 0; i < _values.size(); i++)
    {
      if (hasSuffix(topic, _values[i]))
        _mqttFrame->setValue(i, value);
    }
  }
}

void MqttNode::subscribeTo(const char *subtopic)
{
  char buf[256];
  snprintf(buf, sizeof buf, "%s/%s", mqttTopic.get(), subtopic);
  Homie.getLogger() << "  ◦ Subscribing to: " << buf;
  if (_mqtt->subscribe(buf))
    Homie.getLogger() << " OK" << endl;
  else
    Homie.getLogger() << " Failed" << endl;
}

void MqttNode::reconnect()
{
  Homie.getLogger() << "• MqttNode - Reconnect" << endl;
  if (mqttServer.wasProvided() && mqttTopic.wasProvided())
  {
    Homie.getLogger() << "  ◦ Connecting to: " << mqttServer.get();

    _mqtt->setServer(mqttServer.get(), 1883);
    if (_mqtt->connect(Homie.getConfiguration().deviceId))
    {
      Homie.getLogger() << " OK" << endl;
      if (mqttTopic.wasProvided())
      {
        // First subscribe just to type and properties in order to retrieve the properties
        subscribeTo("$type");
        subscribeTo("$properties");
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