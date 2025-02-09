# Live Stock Data Display

This project uses an ESP32 microcontroller to fetch and display live stock data (Nifty 50 and Sensex) on an OLED screen. The data is retrieved from the ThingSpeak API and displayed on the screen in real-time.

## Components Used

- ESP32 microcontroller
- OLED display (128x64 pixels)
- Jumper wires
- Breadboard

## Libraries Required

- Wire
- Adafruit GFX
- Adafruit SSD1306
- WiFi
- WiFiMulti
- HTTPClient

## Circuit Diagram

Connect the components as follows:

- **OLED SDA** to GPIO 12
- **OLED SCL** to GPIO 14
- **VCC** to 3.3V
- **GND** to GND

## Code Explanation

### Initialization

The code begins by including the necessary libraries and defining the OLED display configuration. The `WiFiMulti` object is used to manage multiple WiFi connections.

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#define OLED_SDA 12
#define OLED_SCL 14
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiMulti wifiMulti;
String nifty50 = "N/A";
String sensex = "N/A";
```

### Setup

In the `setup` function, the serial communication is initialized, and the OLED display is configured. The ESP32 connects to the WiFi network, and a message is displayed on the OLED screen.

```cpp
void setup() {
    Serial.begin(115200);

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

    wifiMulti.addAP("Wokwi-GUEST", "");

    for (uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        delay(1000);
    }
}
```

### Loop

The `loop` function checks if the ESP32 is connected to WiFi. If connected, it fetches the stock data and updates the display. Otherwise, it shows a connection message.

```cpp
void loop() {
    if (wifiMulti.run() == WL_CONNECTED) {
        fetchStockData();
        updateDisplay();
    } else {
        displayConnectionMessage();
    }
    delay(3000);
}
```

### Fetching Stock Data

The `fetchStockData` function retrieves the Nifty 50 and Sensex data from the ThingSpeak API.

```cpp
void fetchStockData() {
    HTTPClient http;
    http.begin("http://api.thingspeak.com/apps/thinghttp/send_request?api_key=VK4BI3ZD8OP2E14C");
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        nifty50 = http.getString();
        Serial.print("NIFTY 50: ");
        Serial.println(nifty50);
    }
    http.end();

    http.begin("http://api.thingspeak.com/apps/thinghttp/send_request?api_key=S39YSKQF3DUGFIPX");
    httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        sensex = http.getString();
        Serial.print("SENSEX: ");
        Serial.println(sensex);
    }
    http.end();
}
```

### Updating the Display

The `updateDisplay` function updates the OLED screen with the latest stock data.

```cpp
void updateDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    
    display.setCursor(0, 0);
    display.print("NIFTY 50:");
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.println(nifty50);

    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("SENSEX:");
    display.setTextSize(2);
    display.setCursor(0, 45);
    display.println(sensex);

    display.display();
}
```

### Displaying Connection Message

The `displayConnectionMessage` function shows a message on the OLED screen when the ESP32 is not connected to WiFi.

```cpp
void displayConnectionMessage() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("Connecting to WiFi...");
    display.display();
}
```

## How It Works

1. The ESP32 connects to the specified WiFi network.
2. It fetches the latest Nifty 50 and Sensex data from the ThingSpeak API.
3. The data is displayed on the OLED screen.
4. The display is updated every 3 seconds to respect API rate limits.

## Conclusion

This project demonstrates how to use an ESP32 to fetch and display live stock data on an OLED screen. It can be extended to display other types of data or to use different APIs for more comprehensive information.
