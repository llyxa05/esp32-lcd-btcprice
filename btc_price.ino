#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//Initialization LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//WiFi Info
const char* ssid = "MikroTik";
const char* password = "password";

const char* apiEndPoint = "https://api-testnet.bybit.com/v5/market/tickers?category=spot&symbol=BTCUSDT";

void setup() {
  Serial.begin(9600);
  lcd.begin();  
  lcd.backlight();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  lcd.print("WiFi Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    long rssi = WiFi.RSSI();
    HTTPClient http;  
    http.begin(apiEndPoint);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      
      float bitcoinPrice = doc["result"]["list"][0]["usdIndexPrice"]; 

      bitcoinPrice = round(bitcoinPrice * 100.0) / 100.0;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BTC Price:");
      lcd.setCursor(0, 1);
      lcd.print(bitcoinPrice);
      lcd.print(" USD");
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpCode);
      lcd.clear();
      lcd.print("HTTP Error ");
      lcd.print(httpCode);
      lcd.setCursor(0, 1);
      lcd.print("WiFi Signal ");
      lcd.print(rssi);
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
    lcd.clear();
    lcd.print("WiFi Disconnected");
  }
  delay(200); 
}



