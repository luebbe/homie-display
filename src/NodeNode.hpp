/*
 * Dummy node for testing
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#ifndef SRC_NODENODE_H_
#define SRC_NODENODE_H_

#include <HomieNode.hpp>
#include <OLEDIndexFrame.hpp>

class NodeNode: public HomieNode, public OLEDIndexFrame {
private:
	const char *_name;
protected:
  virtual void setup() override;
  virtual void loop() override;

public:
	NodeNode(const char *name);

	// Interface HomieNode
	// virtual bool handleInput(String const &property, HomieRange range, String const &value) override;

	// Interface OLEDFrame
	virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;

};

#endif
