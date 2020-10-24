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

MqttNode::MqttNode(const char *name)
    : HomieNode(name, "MqttClient", "info")
{
  _name = name;
  _mqtt = new PubSubClient(_wifiClient);
  _mqttFrame = new MqttFrame(name);
}

void MqttNode::beforeSetup()
{
  // This has to be called before Homie.setup, because otherwise the default Values will
  // override the values which were already read from config.json
#ifdef DEBUG
  Homie.getLogger() << "• MqttNode - Before Setup" << endl;
#endif
  mqttServer.setDefaultValue(MQTT_SERVER);
  mqttTopic.setDefaultValue(MQTT_TOPIC);
  mqttTitle.setDefaultValue("");
}

void MqttNode::onReadyToOperate()
{
#ifdef DEBUG
  Homie.getLogger() << "• MqttNode - onReadyToOperate" << endl;
#endif
  if (mqttTitle.wasProvided())
  {
    _name = mqttTitle.get();
    _mqttFrame->setName(mqttTitle.get());
  }

  _mqtt->setCallback(
      [this](char *topic, byte *payload, uint16_t length) {
        this->callback(topic, payload, length);
      });

  if (!_mqtt->connected())
  {
    reconnect();
  }
  _mqttFrame->onReadyToOperate();
};

bool MqttNode::hasSubtopic(const std::string str, const std::string topic)
{
  std::string checktopic = "/" + topic;
  return str.size() >= checktopic.size() &&
         str.compare(str.size() - checktopic.size(), checktopic.size(), checktopic) == 0;
}

std::string MqttNode::getPayload(byte *payload, uint16_t length)
{
  std::string result = "";
  for (uint16_t i = 0; i < length; i++)
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
    if (!hasSubtopic(pch, cStatusTopic)) // we're not interested in parsing the status topic any further
    {                                  //
      char *unit;
      asprintf(&unit, "%s/$unit", pch);
      _values.push_back(pch);          // store name of value topic in values basket
      _units.push_back(unit);          // store name of unit topic in values basket
      _mqttFrame->addValue(0.0);       // initialize display frame with dummy values
      _mqttFrame->addUnit("N/A");      //
    }                                  //
    subscribeTo(makeTopic(pch, true)); // subscribe to the corresponding topic and wildcard subtopics
    pch = strtok(NULL, ",");           // and continue to parse
  }
  delete[] buf;
}

void MqttNode::callback(char *topic, byte *payload, uint16_t length)
{
#ifdef DEBUG
  Homie.getLogger() << "  ◦ Received: " << topic << " " << value.c_str() << endl;
#endif

  std::string value = getPayload(payload, length);
  // type is used as display name for the frame when nothing was provided in the config
  if (hasSubtopic(topic, cTypeTopic))
  {
    unsubscribeFrom(makeTopic(cTypeTopic, false));
    if (!mqttTitle.wasProvided())
      _mqttFrame->setName(value);
  }

  // autodetect and subscribe to all properties
  else if (hasSubtopic(topic, cPropsTopic))
  {
    unsubscribeFrom(makeTopic(cPropsTopic, false));
    if (!_mqttFrame->getIsConfigured())
    {
      getNodeProperties(value);
      _mqttFrame->setIsConfigured(true);
    }
  }

  // Check if status of node is ok (if sensor could be read)
  else if (hasSubtopic(topic, cStatusTopic))
  {
    _mqttFrame->setIsOk(value == "ok");
  }

  // retrieve the propeties to which we have subscribed earlier
  else
  {
    for (uint8_t i = 0; i < _units.size(); i++)
    {
      if (hasSubtopic(topic, _units[i]))
        _mqttFrame->setUnit(i, value);
    }

    for (uint8_t i = 0; i < _values.size(); i++)
    {
      if (hasSubtopic(topic, _values[i]))
      {
        float floatVal = 0.0;
        if (sscanf(value.c_str(), "%f", &floatVal) > 0)
          _mqttFrame->setValue(i, floatVal);
      }
    }
  }
}

void MqttNode::subscribeTo(const char *topic)
{
#ifdef DEBUG
  Homie.getLogger() << "  ◦ Subscribing to: " << topic;
  if (_mqtt->subscribe(topic))
    Homie.getLogger() << " OK" << endl;
  else
    Homie.getLogger() << " Failed" << endl;
#else
  _mqtt->subscribe(topic);
#endif
}

void MqttNode::unsubscribeFrom(const char *topic)
{
#ifdef DEBUG
  Homie.getLogger() << "  ◦ Unsubscribing from: " << topic;
  if (_mqtt->unsubscribe(topic))
    Homie.getLogger() << " OK" << endl;
  else
    Homie.getLogger() << " Failed" << endl;
#else
  _mqtt->unsubscribe(topic);
#endif
}

char *MqttNode::makeTopic(const char *topic, bool subtopics)
{
#define bufSize 256
  char *buf = new char[bufSize];
  if (subtopics)
  {
    snprintf(buf, bufSize, "%s/%s/#", mqttTopic.get(), topic);
  }
  else
  {
    snprintf(buf, bufSize, "%s/%s", mqttTopic.get(), topic);
  }
  return buf;
}

void MqttNode::reconnect()
{
  if (mqttServer.wasProvided() && mqttTopic.wasProvided())
  {
    Homie.getLogger() << "• MqttNode - Reconnect" << endl
                      << "  ◦ Connecting to: " << mqttServer.get();

    _units.clear();
    _values.clear();
    _mqttFrame->clear();

    _mqtt->setServer(mqttServer.get(), 1883);
    if (_mqtt->connect(Homie.getConfiguration().deviceId))
    {
      Homie.getLogger() << " OK" << endl;
      if (mqttTopic.wasProvided())
      {
        // First subscribe just to type and properties in order to retrieve
        // the properties and default node name
        subscribeTo(makeTopic(cTypeTopic, false));
        subscribeTo(makeTopic(cPropsTopic, false));
        subscribeTo(makeTopic(cStatusTopic, false));
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
    unsigned int now = millis();
    // retry every two seconds
    if ((now - _lastTry > 2000) || (_lastTry == 0))
    {
      _lastTry = now;
      reconnect();
    }
  }
  _mqtt->loop();
  _mqttFrame->loop();
}