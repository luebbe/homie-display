/*
 * Dummy node for testing
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include <Homie.hpp>
#include "NodeNode.hpp"

NodeNode::NodeNode(const char *name) :
  HomieNode(name, "test")
{
  _name = name;
}

// Interface HomieNode
// bool NodeNode::handleInput(String const &property, HomieRange range, String const &value) {
//
// }

// Interface OLEDFrame
void NodeNode::drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) {
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

void NodeNode::setup() {
  Homie.getLogger() << "NodeNode Setup" << endl;
};

void NodeNode::loop() {

};
