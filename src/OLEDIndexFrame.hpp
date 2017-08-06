/*
 * An Oled display frame that just shows its index number in the bottom left corner
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#ifndef SRC_OLEDINDEXFRAME_H_
#define SRC_OLEDINDEXFRAME_H_

#include <OLEDFrame.h>
#include <OLEDOverlay.h>

class OLEDIndexFrame: public OLEDFrame, public OLEDOverlay {
public:
  OLEDIndexFrame();

  // Interface OLEDFrame
  virtual void drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) override;

  // Interface OLEDOverlay
  virtual void drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx) override;
};

#endif
