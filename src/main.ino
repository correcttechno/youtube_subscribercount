/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <WiFi.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32
// OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiClient wifi;

HTTPClient http;

const unsigned char epd_bitmap_youtube[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xfc,
    0x7f, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe,
    0x7f, 0xf9, 0xff, 0xfe, 0x7f, 0xf8, 0xff, 0xfe, 0x7f, 0xf8, 0x3f, 0xfe, 0x7f, 0xf8, 0x0f, 0xfe,
    0x7f, 0xf8, 0x0f, 0xfe, 0x7f, 0xf8, 0x3f, 0xfe, 0x7f, 0xf8, 0xff, 0xfe, 0x7f, 0xf9, 0xff, 0xfe,
    0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfc,
    0x3f, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xf8, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void writeDisplay(String text)
{
  display.clearDisplay();
  display.drawBitmap(0, 5, epd_bitmap_youtube, 32, 32, SSD1306_WHITE);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(50, 10);
  display.println(text);
  display.display();
}
void setup()
{
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  char ssid[] = "correcttechno";
  char pass[] = "Correct1234";
  WiFi.begin(ssid, pass);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  delay(2000);
}


void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Qosuldu");
    http.begin("https://www.googleapis.com/youtube/v3/channels?part=statistics&id=UCpAmtY7kuGVbTy4c3KkI3mA&key=[YOUR_API_KEY]"); // HTTP

    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      Serial.println(httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK)
      {
        String payload = http.getString();
        int search = payload.indexOf("subscriberCount");
        if (search != -1)
        {
          String data = "";
          for (int i = search + 19; i < payload.length(); i++)
          {

            if (payload[i] == ',')
            {
              break;
            }
            data += payload[i];
          }
          data.replace('"', ' ');
          data.replace(',', ' ');
          writeDisplay(data);
          Serial.println(data);
        }
      }
    }

    delay(50000);
  }

  else
  {
    Serial.println("Wifi not connected");

    delay(500);
  }
}
