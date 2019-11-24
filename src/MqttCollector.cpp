/*
 * Class that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.1.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "MqttCollector.hpp"

HomieSetting<const char *> mqttServer("MqttServer", "The MQTT server to which this node shall connect");
HomieSetting<const char *> mqttTopic("MqttTopic", "The MQTT topic to which this node shall listen");
HomieSetting<const char *> mqttTitle("MqttTitle", "The title that shall be shown on the frame");

MqttCollector::MqttCollector(const char *name, int something)
    : _name(name)
{
}

void MqttCollector::beforeSetup()
{
  // This has to be called before Homie.setup, because otherwise the default Values will
  // override the values which were already read from config.json
  Homie.getLogger() << "• MqttCollector - Before Setup" << endl;
  mqttServer.setDefaultValue(MQTT_SERVER);
  mqttTopic.setDefaultValue(MQTT_TOPIC);
  mqttTitle.setDefaultValue("");
}

void MqttCollector::onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  char *buf = new char[len + 1]; // Allocate buffer for parsing
  strncpy(buf, payload, len);
  buf[len] = 0;

  Homie.getLogger() << "Len: " << len << " Idx: " << index << " Tot: " << total << " " << topic << ":" << buf << endl;

  if (isSubtopic(topic, cTypeTopic))
  {
    // type is used as display name for the frame when nothing was provided in the config
    // if (!mqttTitle.wasProvided())
    //   _mqttFrame->setName(value);
    unsubscribeFrom(cTypeTopic);
  }

  else if (isSubtopic(topic, cPropsTopic))
  {
    // autodetect and subscribe to all properties
    // if (!_mqttFrame->getIsConfigured())
    // {
    getNodeProperties(buf, len);
    //   _mqttFrame->setIsConfigured(true);
    // }
    unsubscribeFrom(cPropsTopic);
  }

  else if (isSubtopic(topic, cStatusTopic))
  {
    // _mqttFrame->setIsOk(value == "ok");
  }

  else
  {
    // retrieve the propeties to which we have subscribed earlier
    for (uint8_t i = 0; i < _units.size(); i++)
    {
      if (isSubtopic(topic, _units[i]))
        Homie.getLogger() << "Unit: " << i << " " << buf << endl;
      // _mqttFrame->setUnit(i, value);
    }

    for (uint8_t i = 0; i < _values.size(); i++)
    {
      if (isSubtopic(topic, _values[i]))
      {
        float floatVal = 0.0;
        if (sscanf(buf, "%f", &floatVal) > 0)
          Homie.getLogger() << "Value: " << i << " " << buf << endl;
        // _mqttFrame->setValue(i, floatVal);
      }
    }
  }
  delete[] buf;
}

void MqttCollector::onReadyToOperate()
{
  Homie.getLogger() << "• MqttCollector - onReadyToOperate" << endl;

  Homie.getMqttClient().onMessage(std::bind(&MqttCollector::onMqttMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6));

  if (mqttTitle.wasProvided())
  {
    _name = mqttTitle.get();
  }

  if (mqttTopic.wasProvided())
  {
    subscribeTo(cPropsTopic);
  }
};

bool MqttCollector::isSubtopic(const std::string topic, const std::string subtopic)
{
  std::string prefixSubtopic = "/" + subtopic;
  return (topic.size() >= prefixSubtopic.size()) &&
         (topic.compare(topic.size() - prefixSubtopic.size(), prefixSubtopic.size(), prefixSubtopic) == 0);
}

void MqttCollector::getNodeProperties(char *payload, size_t len)
{
  char *unitTopic = new char[len]; // Allocate buffer for unit topic (it's big enough, too lazy to calculate)

  // Parse comma separated node properties
  char delim[] = ",";
  char *pch = strtok(payload, delim);
  while (pch != NULL)
  {
    // put them in the units or values basket and tell the display frame about them
    if (!isSubtopic(pch, cStatusTopic))
    {
      snprintf(unitTopic, len, "%s/%s", pch, cUnitTopic);

      subscribeTo(pch);       // subscribe to value topic
      subscribeTo(unitTopic); // subscribe to unit topic
      _values.push_back(pch);
      _units.push_back(unitTopic);
    }
    pch = strtok(NULL, delim);
  }
  delete[] unitTopic;

  Homie.getLogger() << "Units";
  for (uint8_t i = 0; i < _units.size(); i++)
  {
    Homie.getLogger() << " " << _units[i].c_str();
  }
  Homie.getLogger() << endl;
  Homie.getLogger() << "Values";
  for (uint8_t i = 0; i < _values.size(); i++)
  {
    Homie.getLogger() << " " << _values[i].c_str();
  }
  Homie.getLogger() << endl;
}

void MqttCollector::subscribeTo(const char *topic)
{
  char _mqttTopic[256];
  snprintf(_mqttTopic, sizeof _mqttTopic, "%s/%s", mqttTopic.get(), topic);
  Homie.getLogger() << "  ◦ Subscribing to: " << _mqttTopic;
  if (Homie.getMqttClient().subscribe(_mqttTopic, 1))
    Homie.getLogger() << " OK" << endl;
  else
    Homie.getLogger() << " Failed" << endl;
}

void MqttCollector::subscribeToSubtopics(const char *topic)
{
  char _mqttTopic[256];
  snprintf(_mqttTopic, sizeof _mqttTopic, "%s/%s/#", mqttTopic.get(), topic);
  Homie.getLogger() << "  ◦ Subscribing to: " << _mqttTopic;
  if (Homie.getMqttClient().subscribe(_mqttTopic, 1))
    Homie.getLogger() << " OK" << endl;
  else
    Homie.getLogger() << " Failed" << endl;
}

void MqttCollector::unsubscribeFrom(const char *topic)
{
  char _mqttTopic[256];
  snprintf(_mqttTopic, sizeof _mqttTopic, "%s/%s", mqttTopic.get(), topic);
  Homie.getLogger() << "  ◦ Unsubscribing from: " << _mqttTopic;
  if (Homie.getMqttClient().unsubscribe(_mqttTopic))
    Homie.getLogger() << " OK" << endl;
  else
    Homie.getLogger() << " Failed" << endl;
}
