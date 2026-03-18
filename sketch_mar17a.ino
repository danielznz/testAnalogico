#include "ibus.h"

// Seus pinos analógicos (mantidos)
#define RIGHT_VRX A0
#define RIGHT_VRY A1
#define LEFT_VRX A3
#define LEFT_VRY A4

// Pinos digitais para teste
#define PIN5 5
#define PIN6 6

// Define os analógicos usados
byte analogPins[] = {RIGHT_VRX, RIGHT_VRY, LEFT_VRX, LEFT_VRY};

// Agora com digitais
byte digitalPins[] = {PIN5, PIN6};
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

  // Configura os pinos digitais
  pinMode(PIN5, INPUT_PULLUP);
  pinMode(PIN6, INPUT_PULLUP);
}

void loop() {
  unsigned long time = millis();

  ibus.begin();

  // Envia os analógicos
  for(int i = 0; i < ANALOG_INPUTS_COUNT; i++) {
    int val = analogRead(analogPins[i]);
    ibus.write(1000 + (uint32_t)val * 1000 / 1023);
  }

  // Envia os digitais (0 ou 1)
  for(int i = 0; i < DIGITAL_INPUTS_COUNT; i++) {
    int val = digitalRead(digitalPins[i]);

    // Converte para padrão iBus (1000 ou 2000)
    if(val == HIGH)
      ibus.write(2000);
    else
      ibus.write(1000);
  }

  ibus.end();

  time = millis() - time;
  if(time < UPDATE_INTERVAL)
    delay(UPDATE_INTERVAL - time);
}