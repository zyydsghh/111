// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

#define GROUP_NO "ys709"  // 本组组号
#define LEDPIN 13         // UNO 板载 LED 引脚

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

bool ledState = LOW;
unsigned long lastToggleMs = 0;
unsigned long blinkIntervalMs = 1000;
unsigned long ledOnMs = 150;    // LED 点亮时间
unsigned long ledOffMs = 150;   // LED 熄灭时间

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);
  dht.begin();
}
void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // 湿度越高，LED 闪烁间隔越短；湿度越低，闪烁间隔越长
  h = constrain(h, 0, 100);
  blinkIntervalMs = map(h, 0, 100, 600, 80);

  // 点亮时间固定，熄灭时间随湿度变化，灯光会更醒目
  ledOnMs = 150;
  ledOffMs = blinkIntervalMs - ledOnMs;
  if (ledOffMs < 30) {
    ledOffMs = 30;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Group: "));
  Serial.println(F(GROUP_NO));
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  unsigned long nowMs = millis();

  if (ledState == HIGH) {
    if (nowMs - lastToggleMs >= ledOnMs) {
      lastToggleMs = nowMs;
      ledState = LOW;
      digitalWrite(LEDPIN, LOW);
    }
  } else {
    if (nowMs - lastToggleMs >= ledOffMs) {
      lastToggleMs = nowMs;
      ledState = HIGH;
      digitalWrite(LEDPIN, HIGH);
    }
  }
}