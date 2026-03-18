#include "ibus.h"

// Seus pinos (mantidos)
#define RIGHT_VRX A0
#define RIGHT_VRY A1
#define LEFT_VRX A3
#define LEFT_VRY A4

// --- DECLARAÇÃO DOS PINOS DIGITAIS ---
#define D2_PIN  2
#define D3_PIN  3
#define D4_PIN  4
#define D5_PIN  5
#define D6_PIN  6
#define D7_PIN  7
#define D8_PIN  8
#define D9_PIN  9
#define D10_PIN 10
#define D11_PIN 11
#define D12_PIN 12

// Analógicos
byte analogPins[] = {RIGHT_VRX, RIGHT_VRY, LEFT_VRX, LEFT_VRY};

// Digitais (agora com todos)
byte digitalPins[] = {
  D2_PIN, D3_PIN, D4_PIN, D5_PIN, D6_PIN,
  D7_PIN, D8_PIN, D9_PIN, D10_PIN,
  D11_PIN, D12_PIN
};

byte digitalBitmappedPins[] = {};

// Configuração
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

  // Configura todos os pinos como INPUT_PULLUP
  for(int i = 0; i < DIGITAL_INPUTS_COUNT; i++) {
    pinMode(digitalPins[i], INPUT_PULLUP);
  }
}

void loop() {
  unsigned long time = millis();

  ibus.begin();

  // Envia os 4 eixos
  for(int i = 0; i < ANALOG_INPUTS_COUNT; i++) {
    int val = analogRead(analogPins[i]);
    ibus.write(1000 + (uint32_t)val * 1000 / 1023);
  }

  // --- BOTÕES (D2 até D12) ---
  for(int i = 0; i < DIGITAL_INPUTS_COUNT; i++) {
    int val = digitalRead(digitalPins[i]);

    // LOW = pressionado
    if(val == LOW)
      ibus.write(2000); // botão ON
    else
      ibus.write(1000); // botão OFF
  }

  ibus.end();

  time = millis() - time;
  if(time < UPDATE_INTERVAL)
    delay(UPDATE_INTERVAL - time);
}