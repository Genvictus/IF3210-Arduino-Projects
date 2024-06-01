/*
  Garden Monitoring

  Used to monitor garden's state and actuates necessary actuators
  to respond to the garden status.

  Garden can be categorized into 3 states:

  *=================================*
  | Status | Temperature | Humidity |
  |  Good  |     Ok      |    Ok    |
  |  Ok    |     Ok      |    Bad   |
  |  Ok    |     Bad     |    Ok    |
  |  Bad   |     Bad     |    Bad   |
  *=================================*

  Temperature and humidity thresholds can be set in the constants below
*/


#include <dht11.h>
#include <LiquidCrystal.h>

#define DHT11PIN 2

#define REDLED 11
#define YELLOWLED 12
#define GREENLED 13

#define RS 8
#define EN 9
#define D4 4
#define D5 5
#define D6 6
#define D7 7

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const String GARDEN_STATUS[] = { "BAD", "OK", "GOOD" };

const float HUMI_THRESH_HIGH = 70;
const float HUMI_THRESH_LOW = 55;

const float TEMP_THRESH_HIGH = 30;
const float TEMP_THRESH_LOW = 27;

dht11 DHT11;

void setup() {
  Serial.begin(9600);

  pinMode(REDLED, OUTPUT);
  pinMode(YELLOWLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);

  lcd.begin(16, 2);
}

void loop() {
  Serial.println();

  int chk = DHT11.read(DHT11PIN);

  float humidity, temperature;

  humidity = (float)DHT11.humidity; 
  temperature = (float)DHT11.temperature;

  Serial.print("Humidity (%): ");
  Serial.println(humidity, 2);

  Serial.print("Temperature  (C): ");
  Serial.println(temperature, 2);

  uint8_t condition = 0;
  condition += humidityOk(humidity);
  condition += temperatureOk(temperature);

  startLED(condition);
  displayLCD(condition, humidity, temperature);

  delay(1000);

  stopLED();
}

void startLED(uint8_t condition) {
  switch (condition) {
    case 0:
      digitalWrite(REDLED, HIGH);
      break;
    case 1:
      digitalWrite(YELLOWLED, HIGH);
      break;
    case 2:
      digitalWrite(GREENLED, HIGH);
      break;
  }
}

void stopLED() {
  digitalWrite(REDLED, LOW);
  digitalWrite(YELLOWLED, LOW);
  digitalWrite(GREENLED, LOW);
}

void displayLCD(uint8_t condition, float humidity, float temperature) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Status: ");
  lcd.print(GARDEN_STATUS[condition]);

  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity, 2);
  lcd.setCursor(8, 1);
  lcd.print("T:");
  lcd.print(temperature, 2);
}

bool humidityOk(float humidity) {
  return (HUMI_THRESH_HIGH > humidity) && (humidity > HUMI_THRESH_LOW);
}

bool temperatureOk(float temperature) {
  return (TEMP_THRESH_HIGH > temperature) && (temperature > TEMP_THRESH_LOW);
}