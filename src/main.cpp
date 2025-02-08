#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA 12
#define OLED_SCL 14
#define SCREEN_WIDTH 128  // OLED display width (pixels)
#define SCREEN_HEIGHT 64  // OLED display height (pixels)
#define OLED_ADDR 0x3C    // I2C address (common for SSD1306)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Initialize I2C with custom pins
  Wire.begin(OLED_SDA, OLED_SCL);
  
  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Display static text
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Hello, World!"));
  display.setTextSize(2);
  display.println(F("SSD1306"));
  display.setTextSize(1);
  display.println(F("ESP32 Example"));
  display.display();

  delay(2000);
}

void loop() {
  // Clear display
  display.clearDisplay();
  
  // Display dynamic content
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("Milliseconds: "));
  display.println(millis());
  
  // Show the display buffer
  display.display();
  
  delay(100);
}