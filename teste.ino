#include <Joystick.h>

#define RIGHT_VRX A0
#define RIGHT_VRY A1
#define LEFT_VRX  A3
#define LEFT_VRY  A4

#define D2_PIN  2
#define D3_PIN  3
#define D4_PIN  4
#define D5_PIN  5
#define D6_PIN  6
#define D7_PIN  7
#define D8_PIN  8
#define D9_PIN  9
#define D10_PIN 10
#define D16_PIN 16 
#define D14_PIN 14 

// ===== ARRAYS =====
byte analogPins[] = {RIGHT_VRX, RIGHT_VRY, LEFT_VRX, LEFT_VRY};

byte digitalPins[] = {
  D2_PIN, D3_PIN, D4_PIN, D5_PIN, D6_PIN,
  D7_PIN, D8_PIN, D9_PIN, D10_PIN,
  D16_PIN, D14_PIN
};

// ===== JOYSTICK CONFIG =====
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  11,     // número de botões
  0,      // hat switch
  true, true, true, true, false, false,
  false, false, false, false, false
);

// ===== CONFIG =====
#define UPDATE_INTERVAL 10

void setup() {
  // Configura botões
  for(int i = 0; i < sizeof(digitalPins); i++) {
    pinMode(digitalPins[i], INPUT_PULLUP);
  }

  Joystick.begin();

  //AXIS DEFINE
  Joystick.setXAxisRange(0, 1023);
  Joystick.setYAxisRange(0, 1023);
  Joystick.setRxAxisRange(0, 1023);
  Joystick.setRyAxisRange(0, 1023);
}

void loop() {
  unsigned long start = millis();

  //AXIS
  Joystick.setXAxis(analogRead(RIGHT_VRX));
  Joystick.setYAxis(analogRead(RIGHT_VRY));
  Joystick.setRxAxis(analogRead(LEFT_VRX));
  Joystick.setRyAxis(analogRead(LEFT_VRY));

  //BUTTON
  for(int i = 0; i < sizeof(digitalPins); i++) {
    int val = digitalRead(digitalPins[i]);

    // LOW = pressionado
    Joystick.setButton(i, val == LOW);
  }

  // ===== UPDATE =====
  delay(UPDATE_INTERVAL - min(UPDATE_INTERVAL, millis() - start));
}