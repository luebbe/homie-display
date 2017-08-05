/*
 * A Homie node that shows (Homie) status information
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#ifndef SRC_STATUSNODE_H_
#define SRC_STATUSNODE_H_

#include <OLEDIndexFrame.hpp>
#include "images.h"

class StatusNode: public HomieNode, public OLEDIndexFrame {
private:
	const char *_name;
	bool _cfgmode;
	bool _mqtt;
	bool _wifi;
  bool _alert;
	String _statusText;
	String _alertMessage;
protected:
  virtual void setup() override;
  virtual void loop() override;

public:
	StatusNode(const char *name);

	// Interface HomieNode
	void Event(const HomieEvent& event);
  bool handleBroadcast(const String& level, const String& value);
	// virtual bool handleInput(String const &property, HomieRange range, String const &value) override;

	// Interface OLEDFrame
	virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;

	void setStatusText(String const value) {
    _statusText = value;
  };
	bool isAlert() {
		return _alert;
	}
  bool isWifiConnected() {
    return _wifi;
  };
};

#endif
