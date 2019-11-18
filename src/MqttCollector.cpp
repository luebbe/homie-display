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
  Homie.getLogger() << "Len: " << len << " Idx: " << index << " Tot: " << total << " " << topic << ":" << ((String)payload).substring(0, len) << endl;
  // void MqttCollector::callback(char *topic, byte *payload, uint16_t length)
  // {
  //   std::string value = getPayload(payload, length);
  //   Homie.getLogger() << "  ◦ Received: " << topic << " " << value.c_str() << endl;

  if (hasSuffix(topic, cTypeTopic))
  {
    // type is used as display name for the frame when nothing was provided in the config
    // if (!mqttTitle.wasProvided())
    //   _mqttFrame->setName(value);
    unsubscribeFrom(cTypeTopic);
  }

  else if (hasSuffix(topic, cPropsTopic))
  {
    // autodetect and subscribe to all properties
    // if (!_mqttFrame->getIsConfigured())
    // {
    getNodeProperties(payload, len);
    //   _mqttFrame->setIsConfigured(true);
    // }
    unsubscribeFrom(cPropsTopic);
  }

  else if (hasSuffix(topic, cStatusTopic))
  {
    // _mqttFrame->setIsOk(value == "ok");
  }

  // else
  // {
  //   // retrieve the propeties to which we have subscribed earlier
  //   for (uint8_t i = 0; i < _units.size(); i++)
  //   {
  //     if (hasSuffix(topic, _units[i]))
  //       _mqttFrame->setUnit(i, value);
  //   }

  //   for (uint8_t i = 0; i < _values.size(); i++)
  //   {
  //     if (hasSuffix(topic, _values[i]))
  //     {
  //       float floatVal = 0.0;
  //       if (sscanf(value.c_str(), "%f", &floatVal) > 0)
  //         _mqttFrame->setValue(i, floatVal);
  //     }
  //   }
  // }
  // }
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

bool MqttCollector::hasSuffix(const std::string str, const std::string suffix)
{
  return str.size() >= suffix.size() &&
         str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void MqttCollector::getNodeProperties(char *payload, size_t len)
{
  char *buf = new char[len + 1];   // Allocate buffer for parsing
  char *unitTopic = new char[len]; // Allocate buffer for unit topic (it's big enough, too lazy to calculate)
  strncpy(buf, payload, len);
  buf[len] = 0;

  // Parse comma separated node properties
  char delim[] = ",";
  char *pch = strtok(buf, delim);
  while (pch != NULL)
  {
    // put them in the units or values basket and tell the display frame about them
    if (!hasSuffix(pch, cStatusTopic))
    {
      snprintf(unitTopic, len, "%s/%s", pch, cUnitTopic);

      subscribeTo(pch);       // subscribe to value topic
      subscribeTo(unitTopic); // subscribe to unit topic
      _values.push_back(pch);
      _units.push_back(unitTopic);
    }
    pch = strtok(NULL, delim);
  }
  delete[] buf;
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

void MqttCollector::reconnect()
{
  // if (mqttServer.wasProvided() && mqttTopic.wasProvided())
  // {
  //   Homie.getLogger() << "• MqttCollector - Reconnect" << endl
  //                     << "  ◦ Connecting to: " << mqttServer.get();

  //   _units.clear();
  //   _values.clear();

  //   _mqtt->setServer(mqttServer.get(), 1883);
  //   if (_mqtt->connect(Homie.getConfiguration().deviceId))
  //   {
  //     Homie.getLogger() << " OK" << endl;
  //     if (mqttTopic.wasProvided())
  //     {
  //       // First subscribe just to type and properties in order to retrieve
  //       // the properties and default node name
  //       subscribeTo(cTypeTopic);
  //       subscribeTo(cPropsTopic);
  //       subscribeTo(cStatusTopic);
  //     }
  //   }
  //   else
  //     Homie.getLogger() << " Failed" << endl;
  // }
}

// void MqttCollector::loop()
// {
//   if (!_mqtt->connected())
//   {
//     unsigned int now = millis();
//     // retry every two seconds
//     if ((now - _lastTry > 2000) || (_lastTry == 0))
//     {
//       _lastTry = now;
//       reconnect();
//     }
//   }
//   _mqtt->loop();
// }