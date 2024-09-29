#include "led_strip/enabled.h"

#if FASTLED_RMT51

#warning "Work in progress: ESP32 ClocklessController for IDF 5.1 - this is still a prototype"

#include "idf5_rmt.h"
#include "led_strip/led_strip.h"
#include "esp_log.h"
#include "led_strip/demo.h"
#include "led_strip/configure_led.h"
#include "led_strip/rmt_strip.h"

USING_NAMESPACE_LED_STRIP

#define TAG "idf5_rmt.cpp"

RmtController5::RmtController5(int DATA_PIN, int T1, int T2, int T3) {
    // Stub implementation
    ESP_LOGI(TAG, "RmtController5 constructor called");
    mPin = DATA_PIN;
}

RmtController5::~RmtController5() {
    // Stub implementation
    ESP_LOGI(TAG, "RmtController5 destructor called");
}

void RmtController5::showPixels(PixelIterator &pixels) {
    ESP_LOGI(TAG, "showPixels called");
    uint32_t max_leds = pixels.size();
    Rgbw rgbw = pixels.get_rgbw();
    const bool is_rgbw = rgbw.active();
    LedStripMode mode = rgbw.active() ? kWS2812_RGBW : kWS2812;
    // RmtLedStrip led_strip(mPin, max_leds, mode);
    IRmtLedStrip* led_strip = create_rmt_led_strip(mPin, max_leds, is_rgbw);
    if (is_rgbw) {
        uint8_t r, g, b, w;
        for (uint16_t i = 0; pixels.has(1); i++) {
            pixels.loadAndScaleRGBW(&r, &g, &b, &w);
            led_strip->set_pixel_rgbw(i, r, g, b, w);
            pixels.advanceData();
            pixels.stepDithering();
        }
    } else {
        uint8_t r, g, b;
        for (uint16_t i = 0; pixels.has(1); i++) {
            pixels.loadAndScaleRGB(&r, &g, &b);
            led_strip->set_pixel(i, r, g, b);
            pixels.advanceData();
            pixels.stepDithering();
        }
    }
    led_strip->draw();
    delete led_strip;
}

#endif  // FASTLED_RMT51

