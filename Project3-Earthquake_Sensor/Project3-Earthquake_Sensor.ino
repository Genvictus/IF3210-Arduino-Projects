// Define sensor and output pins
const int vibrationPin = 2; // Digital pin for vibration sensor
const int tiltPin = 3; // Digital pin for tilt sensor
const int flamePin = 4; // Digital pin for flame sensor
const int buzzerPin = 5; // Digital pin for buzzer
const int buttonPin = 8; // Digital pin for button
#define PIN_AOUT A0 // Analog pin for smoke sensor (MQ-2)

bool buzzerState = false;


void setup() {
  Serial.begin(9600);
  
  pinMode(vibrationPin, INPUT);
  pinMode(tiltPin, INPUT);
  pinMode(flamePin, INPUT);

  pinMode(buzzerPin, OUTPUT);
  
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // Read sensor values
  int vibrationState = digitalRead(vibrationPin);
  int tiltState = digitalRead(tiltPin);
  int flameState = digitalRead(flamePin);
  int buttonState = digitalRead(buttonPin);

  // Read smoke sensor value
  // int ppm = MQ2.readSensor();
  int smokeState = analogRead(PIN_AOUT);
  Serial.print("SmokeState PPM: ");
  Serial.println(smokeState);

  Serial.println("Vibration");
  Serial.println(vibrationState);
  Serial.println("tilt");
  Serial.println(tiltState);
  Serial.println("Flame");
  Serial.println(flameState);
   // Check Earthquake
  if (vibrationState == HIGH && tiltState == HIGH) { 
    buzzerState = true;
  }

  if (smokeState > 950){
    buzzerState = true;
  }

  if (flameState == LOW){
    buzzerState = true;
  }
  // If the button is pressed, reset the buzzer state
  if (buttonState == LOW) {
    buzzerState = false;
  }

  Serial.println("Button State");
  Serial.println(buttonState);
  Serial.println("Buzzer state");
  Serial.println(buzzerState);
  // Control the buzzer based on the buzzer state
  if (buzzerState) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  // Small delay to avoid bouncing issues with the button
  delay(1000);
}
