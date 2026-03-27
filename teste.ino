#include <Joystick.h>

// ===== ANALÓGICOS =====
#define RIGHT_VRX A0
#define RIGHT_VRY A1
#define LEFT_VRX  A3
#define LEFT_VRY  A2

// ===== DIGITAIS =====
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

// ===== SLIDER =====
int sliderValue = 0;
#define SLIDER_MIN 0
#define SLIDER_MAX 1023
#define SLIDER_SPEED 8

// ===== ARRAYS =====
byte digitalPins[] = {
  D2_PIN, D3_PIN, D4_PIN, D5_PIN, D6_PIN,
  D7_PIN, D8_PIN, D9_PIN, D10_PIN,
  D16_PIN, D14_PIN
};

// ===== CONFIG JOYSTICK =====
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  11,
  0,
  true, true, true, true, true, // Z ativado aqui
  false, false, false, false, false
);

// ===== CONFIG =====
#define UPDATE_INTERVAL 10
#define DEBOUNCE_DELAY 5

// ===== FUNÇÃO DEBOUNCE =====
bool readButton(int pin) {
  if (digitalRead(pin) == LOW) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(pin) == LOW) return true;
  }
  return false;
}

void setup() {
  for (int i = 0; i < sizeof(digitalPins) / sizeof(digitalPins[0]); i++) {
    pinMode(digitalPins[i], INPUT_PULLUP);
  }

  Joystick.begin();

  Joystick.setXAxisRange(0, 1023);
  Joystick.setYAxisRange(0, 1023);
  Joystick.setRxAxisRange(0, 1023);
  Joystick.setRyAxisRange(0, 1023);
  Joystick.setZAxisRange(0, 1023); // slider
}

void loop() {
  unsigned long start = millis();

  // ===== EIXOS =====
  Joystick.setXAxis(analogRead(RIGHT_VRX));
  Joystick.setYAxis(analogRead(RIGHT_VRY));
  Joystick.setRxAxis(analogRead(LEFT_VRX));
  Joystick.setRyAxis(analogRead(LEFT_VRY));

  // ===== SLIDER COM D10 E D16 =====
  bool btnLeft  = readButton(D10_PIN);
  bool btnRight = readButton(D16_PIN);

  if (btnLeft && sliderValue > SLIDER_MIN) {
    sliderValue -= SLIDER_SPEED;
  }

  if (btnRight && sliderValue < SLIDER_MAX) {
    sliderValue += SLIDER_SPEED;
  }

  Joystick.setZAxis(sliderValue);

  // ===== BOTÕES =====
  for (int i = 0; i < sizeof(digitalPins) / sizeof(digitalPins[0]); i++) {
    bool pressed = readButton(digitalPins[i]);
    Joystick.setButton(i, pressed);
  }

  // ===== CONTROLE DE TEMPO =====
  unsigned long elapsed = millis() - start;
  if (elapsed < UPDATE_INTERVAL) {
    delay(UPDATE_INTERVAL - elapsed);
  }
}