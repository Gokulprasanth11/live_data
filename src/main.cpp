#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi Credentials - Replace with your own!
const char* ssid = "Naveen PG";
const char* password = "9940646141";

// API Endpoint (Free API may not be reliable. Consider getting your own)
const char* sensex_api_url = "https://your-sensex-api-endpoint.com/sensex"; // Replace with a reliable API
const char* nifty_api_url = "https://your-nifty-api-endpoint.com/nifty";     // Replace with a reliable API

// OLED Display Definitions
#define OLED_SDA 12
#define OLED_SCL 14
#define SCREEN_WIDTH 128  // OLED display width (pixels)
#define SCREEN_HEIGHT 64  // OLED display height (pixels)
#define OLED_ADDR 0x3C    // I2C address (common for SSD1306)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Function Declarations
float getIndexValue(const char* apiURL);
void displayIndexValues(float sensexValue, float niftyValue);

void setup() {
  Serial.begin(115200);

  // Initialize I2C with custom pins
  Wire.begin(OLED_SDA, OLED_SCL);

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.display();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void loop() {
  // Get Sensex and Nifty Values
  float sensexValue = getIndexValue(sensex_api_url);
  float niftyValue = getIndexValue(nifty_api_url);

  // Display Values on OLED
  displayIndexValues(sensexValue, niftyValue);

  // Wait for a while before refreshing
  delay(10000); // Update every 10 seconds.  Adjust as needed.
}

float getIndexValue(const char* apiURL) {
  HTTPClient http;
  http.begin(apiURL);

  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Payload: " + payload);

      // Parse JSON (Example assuming the API returns {"value": 60000.00})
      // Adapt the JSON parsing to the structure of *your* API's response
      DynamicJsonDocument doc(1024); // Adjust size if needed.  Use ArduinoJson Assistant to estimate.
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        http.end();
        return -1.0; // Indicate an error
      }

      // Extract the index value (adjust path based on your API response!)
      float indexValue = doc["value"];  // Example: API returns {"value": ...}
      Serial.print("Index Value: ");
      Serial.println(indexValue);
      http.end();
      return indexValue;


    } else {
      Serial.printf("HTTP GET failed, error code: %d\n", httpCode);
      http.end();
      return -1.0; // Indicate an error
    }
  } else {
    Serial.printf("HTTP GET failed, connection error\n");
    http.end();
    return -1.0; // Indicate an error
  }
}



void displayIndexValues(float sensexValue, float niftyValue) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.print("Sensex: ");
  if (sensexValue >= 0) {
    display.println(sensexValue);
  } else {
    display.println("Error");
  }

  display.print("Nifty: ");
  if (niftyValue >= 0) {
    display.println(niftyValue);
  } else {
    display.println("Error");
  }

  display.display();
}