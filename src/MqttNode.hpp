/*
 * Node that subscribes to a MQTT topic and displays the values that it receives
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#ifndef SRC_MQTTNODE_H_
#define SRC_MQTTNODE_H_

#include <HomieNode.hpp>
#include <OLEDIndexFrame.hpp>
#include <PubSubClient.h>

class MqttNode: public HomieNode, public OLEDIndexFrame {
private:
  const char *_name;
protected:
  virtual void setup() override;
  virtual void loop() override;

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;

public:
  MqttNode(const char *name);

  void beforeSetup();
};

#endif
