/**Contributed by Gokul and Isen**/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

// OLED configuration
#define OLED_SDA 12
#define OLED_SCL 14
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiMulti wifiMulti;
String nifty50 = "N/A";
String sensex = "N/A";

void setup() {
  Serial.begin(115200);

  // Initialize OLED display
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 allocation failed");
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Connecting to WiFi...");
  display.display();

  // Connect to WiFi
  wifiMulti.addAP("Wokwi-GUEST", "");

  // Initial delay
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    delay(1000);
  }
}

void loop() {
  if (wifiMulti.run() == WL_CONNECTED) {
    fetchStockData();
    updateDisplay();
  } else {
    displayConnectionMessage();
  }
  delay(3000); // Respect API rate limits
}

void fetchStockData() {
  // Fetch Nifty 50 data
  HTTPClient http;
  http.begin("http://api.thingspeak.com/apps/thinghttp/send_request?api_key=VK4BI3ZD8OP2E14C");
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    nifty50 = http.getString();
    Serial.print("NIFTY 50: ");
    Serial.println(nifty50);
  }
  http.end();

  // Fetch Sensex data
  http.begin("http://api.thingspeak.com/apps/thinghttp/send_request?api_key=S39YSKQF3DUGFIPX");
  httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    sensex = http.getString();
    Serial.print("SENSEX: ");
    Serial.println(sensex);
  }
  http.end();
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  
  // Display Nifty 50
  display.setCursor(0, 0);
  display.print("NIFTY 50:");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.println(nifty50);

  // Display Sensex
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("SENSEX:");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.println(sensex);

  display.display();
}

void displayConnectionMessage() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Connecting to WiFi...");
  display.display();
}