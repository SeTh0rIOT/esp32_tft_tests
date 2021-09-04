
// SIGNAL  | PIN (ESP32)
//---------|-------
// MISO    | 19
// MOSI    | 23
// CLK     | 18
// LCD_DC  | 2
// LCD_RST | 3
// LCD_CS  | 15
// TP_CS   | 14

#include "../lvgl/src/lvgl.h"
#include <TFT_eSPI.h>
#include <gui.h>

int ledState = LOW;
unsigned long previousMillis = 0;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void setup()
{
  Serial.begin(115200);
  
  pinMode(17, OUTPUT);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(17, ledChannel);

  tftInit();
  lv_init();
  initDisplay();
  initDriver();
  screenInit();
  guiInit();
}

void loop()
{
  lv_timer_handler();
  //delay(5);
  unsigned long currentMillis = millis();
  if (led_one)
  {
    if (interval > 1)
    {
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;

        if (ledState == LOW)
        {
          ledState = HIGH;
          ledcWrite(ledChannel, value);
        }
        else
        {
          ledState = LOW;
          ledcWrite(ledChannel, 0);
        }
      }
    }
    else
    {
      ledcWrite(ledChannel, value);
    }
  }
  else
  {
    ledcWrite(ledChannel, 0);
  }
}
