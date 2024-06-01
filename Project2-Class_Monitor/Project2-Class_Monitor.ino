/*
  Class Monitoring
*/

#include <LiquidCrystal.h>

#define PIRPIN 2

#define LIGHTPIN 11
#define FANPIN 12

#define BUTTONPIN 13

#define RS 8
#define EN 9
#define D4 4
#define D5 5
#define D6 6
#define D7 7

#define CALIBRATION_TIME 30

#define LOOP_PERIOD 500

int senseInterval = 10000;
int currentTick;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const char OCCUPIED[] = "OCCUPIED";
const char EMPTY[] = "EMPTY";

const char ON[] = "ON";
const char OFF[] = "OFF";

void setup() {
  Serial.begin(9600);

  currentTick = senseInterval;

  pinMode(PIRPIN, INPUT);

  pinMode(BUTTONPIN, INPUT);

  pinMode(LIGHTPIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);

  lcd.begin(16, 2);

  Serial.println(F("Starting calibration"));
  delay(1000 * CALIBRATION_TIME);
  Serial.println(F("Calibration complete"));
}

void loop() {

  readCommand();
  int buttonPressed = digitalRead(BUTTONPIN);

  if (currentTick >= senseInterval || buttonPressed == HIGH) {
    int infraredReading = digitalRead(PIRPIN);
    currentTick = 0;

    bool empty = infraredReading == LOW;

    Serial.print("Room is ");
    if (empty) {
      Serial.println(EMPTY);
    } else {
      Serial.println(OCCUPIED);
    }

    updateLEDState(empty);
    displayLCD(empty);
  }

  delay(LOOP_PERIOD);
  currentTick += LOOP_PERIOD;
}

void readCommand() {
  char cmd = Serial.read();
  switch (cmd) {
    case '!':
      Serial.println(F("Reading sensor"));
      currentTick = senseInterval;
      break;

    case '=':
      char msg[5];
      Serial.readBytes(msg, 5);

      senseInterval = atoi(msg);

    case '?':
      Serial.print(F("Interval set to:"));
      Serial.println(senseInterval);

      break;
  }
}

void updateLEDState(bool empty) {
  if (empty) {
    digitalWrite(LIGHTPIN, LOW);
    digitalWrite(FANPIN, LOW);
  } else {
    digitalWrite(LIGHTPIN, HIGH);
    digitalWrite(FANPIN, HIGH);
  }
}

void displayLCD(bool empty) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ROOM: ");
  if (empty) {
    lcd.print(EMPTY);
  } else {
    lcd.print(OCCUPIED);
  }

  const char* status = empty ? OFF : ON;

  lcd.setCursor(0, 1);
  lcd.print("FAN:");
  lcd.print(status);
  lcd.print("LIGHT:");
  lcd.print(status);
}