#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int buzzerPin = 12;
const int LEDPin = 9;
const int sensorPin = A0;

unsigned long previousTime = 0;
// int beatThreshold = 450;
// bool beatDetected = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(10, INPUT);          // Setup for leads off detection LO +
  pinMode(11, INPUT);          // Setup for leads off detection LO -
  pinMode(buzzerPin, OUTPUT);  // Setup output for buzzer actuator
  pinMode(sensorPin, INPUT);   //Setup sensor input
  // initialize the serial communication:
  Serial.begin(9600);
  lcd.init();                  // Initialize the LCD actuator
  lcd.backlight();             // Turn on the backlight

  lcd.setCursor(0,0);
  lcd.print("Welcome");
  delay(2000);
  lcd.clear();
}

void loop() {

  if ((digitalRead(10) == 1) || (digitalRead(11) == 1)) {
    Serial.println('!');
  } else {
    // read from sensor and record time
    int heartbeatSignal = analogRead(sensorPin);
    Serial.println(heartbeatSignal);

    if (Serial.available() > 0) {
      char received = Serial.read();
      String bpm = Serial.readStringUntil('\n'); 
      if (received == 'B') {
        lcd.setCursor(0,0); //set cursor on character 0, line 0
        lcd.print("BPM:");
        lcd.setCursor(0,1); //set cursor on character 0, line 1
        lcd.print(bpm); // Display the BPM value calculated from Python
        delay(2000);    // Keep the display for 2 seconds
        lcd.clear();

        digitalWrite(buzzerPin, HIGH);
        delay(50);
        digitalWrite(buzzerPin, LOW);
      }
    }

  }
  //Wait for a bit to keep serial data from saturating
  delay(10);
}