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
#define UPDATE_INTERVAL 200  // aumentei pra facilitar leitura no monitor

#define ANALOG_INPUTS_COUNT sizeof(analogPins)
#define DIGITAL_INPUTS_COUNT sizeof(digitalPins)
#define DIGITAL_BITMAPPED_INPUTS_COUNT sizeof(digitalBitmappedPins)

#define NUM_CHANNELS ( (ANALOG_INPUTS_COUNT) + (DIGITAL_INPUTS_COUNT) + (15 + (DIGITAL_BITMAPPED_INPUTS_COUNT))/16 )

IBus ibus(NUM_CHANNELS);

void setup() {
  analogReference(ANALOG_REFERENCE);
  Serial.begin(BAUD_RATE);

  pinMode(PIN5, INPUT_PULLUP);
  pinMode(PIN6, INPUT_PULLUP);

  Serial.println("=== TESTE PINOS DIGITAIS ===");
}

void loop() {
  unsigned long time = millis();

  ibus.begin();

  // Analógicos
  for(int i = 0; i < ANALOG_INPUTS_COUNT; i++) {
    int val = analogRead(analogPins[i]);
    int mapped = 1000 + (uint32_t)val * 1000 / 1023;
    ibus.write(mapped);
  }

  // Digitais + debug
  Serial.print("PIN5: ");
  int val5 = digitalRead(PIN5);
  Serial.print(val5);
  Serial.print(" | ");

  Serial.print("PIN6: ");
  int val6 = digitalRead(PIN6);
  Serial.print(val6);
  Serial.print(" | ");

  // Envia para iBus
  ibus.write(val5 == HIGH ? 2000 : 1000);
  ibus.write(val6 == HIGH ? 2000 : 1000);

  // Mostra o valor enviado
  Serial.print("IBUS -> CH5: ");
  Serial.print(val5 == HIGH ? 2000 : 1000);
  Serial.print(" | CH6: ");
  Serial.println(val6 == HIGH ? 2000 : 1000);

  ibus.end();

  time = millis() - time;
  if(time < UPDATE_INTERVAL)
    delay(UPDATE_INTERVAL - time);
}