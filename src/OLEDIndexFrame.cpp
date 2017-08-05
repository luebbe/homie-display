/*
 * An Oled display frame that just shows its index number in the bottom left corner
 *
 * Version: 1.0.0
 * Author: Lübbe Onken (http://github.com/luebbe)
 */

#include "OLEDIndexFrame.hpp"

OLEDIndexFrame::OLEDIndexFrame() {

}

void OLEDIndexFrame::drawFrame(OLEDDisplay &display,  OLEDDisplayUiState& state, int16_t x, int16_t y) {

}

// Interface OLEDOverlay
void OLEDIndexFrame::drawOverlay(OLEDDisplay& display,  OLEDDisplayUiState& state, uint8_t idx) {
  // draw current frame number at bottom left
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 54,
    String(state.currentFrame + 1) + "/" + String(OLEDFrame::count()));
};
