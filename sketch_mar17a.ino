#include "ibus.h"

// Analógicos
#define RIGHT_VRX A0
#define RIGHT_VRY A1
#define LEFT_VRX A3
#define LEFT_VRY A4

byte analogPins[] = {RIGHT_VRX, RIGHT_VRY, LEFT_VRX, LEFT_VRY};

// Digitais (D2 até D10)
byte digitalPins[] = {2,3,4,5,6,7,8,9,10};
byte digitalBitmappedPins[] = {};

// Config
#define ANALOG_REFERENCE DEFAULT
#define BAUD_RATE 115200
#define UPDATE_INTERVAL 10

#define ANALOG_INPUTS_COUNT sizeof(analogPins)
#define DIGITAL_INPUTS_COUNT sizeof(digitalPins)
#define DIGITAL_BITMAPPED_INPUTS_COUNT sizeof(digitalBitmappedPins)

#define NUM_CHANNELS ( (ANALOG_INPUTS_COUNT) + (DIGITAL_INPUTS_COUNT) + (15 + (DIGITAL_BITMAPPED_INPUTS_COUNT))/16 )

IBus ibus(NUM_CHANNELS);

void setup() {
  analogReference(ANALOG_REFERENCE);
  Serial.begin(BAUD_RATE);

  // Configura todos os digitais
  for(int i = 0; i < DIGITAL_INPUTS_COUNT; i++) {
    pinMode(digitalPins[i], INPUT_PULLUP);
  }
}

void loop() {
  unsigned long time = millis();

  ibus.begin();

  // Analógicos
  for(int i = 0; i < ANALOG_INPUTS_COUNT; i++) {
    int val = analogRead(analogPins[i]);
    ibus.write(1000 + (uint32_t)val * 1000 / 1023);
  }

  // Digitais como botões
  for(int i = 0; i < DIGITAL_INPUTS_COUNT; i++) {
    int val = digitalRead(digitalPins[i]);

    // LOW = pressionado
    if(val == LOW)
      ibus.write(2000);
    else
      ibus.write(1000);
  }

  ibus.end();

  time = millis() - time;
  if(time < UPDATE_INTERVAL)
    delay(UPDATE_INTERVAL - time);
}