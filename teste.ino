#include <Joystick.h>

// --- Configuração dos pinos ---
#define VRx_PIN  A0
#define VRy_PIN  A1
#define BTN_PIN  2

// --- Cria o objeto Joystick ---
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,   // ID padrão
  JOYSTICK_TYPE_JOYSTICK,       // Tipo: Joystick
  1,                            // Número de botões (1 = click do analógico)
  0,                            // Número de Hat Switches
  true,                         // Eixo X habilitado
  true,                         // Eixo Y habilitado
  false,                        // Eixo Z desabilitado
  false,                        // Rotação X desabilitada
  false,                        // Rotação Y desabilitada
  false,                        // Rotação Z desabilitada
  false,                        // Rudder desabilitado
  false,                        // Throttle desabilitado
  false,                        // Acelerador desabilitado
  false,                        // Freio desabilitado
  false                         // Direção desabilitada
);

// --- Variáveis de estado do botão (para debounce) ---
bool lastButtonState = HIGH;
bool currentButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50; // ms

void setup() {
  // Configura o pino do botão com pull-up interno
  // (botão conecta o pino ao GND quando pressionado)
  pinMode(BTN_PIN, INPUT_PULLUP);

  // Define o range dos eixos (0 a 1023 = 10 bits do ADC)
  Joystick.setXAxisRange(0, 1023);
  Joystick.setYAxisRange(0, 1023);

  // Inicia o Joystick (true = auto-envio de estado)
  Joystick.begin(false);
}

void loop() {
  // --- Leitura dos eixos analógicos ---
  int xValue = analogRead(VRx_PIN);
  int yValue = analogRead(VRy_PIN);

  Joystick.setXAxis(xValue);
  Joystick.setYAxis(yValue);

  // --- Leitura do botão com debounce ---
  int reading = digitalRead(BTN_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != currentButtonState) {
      currentButtonState = reading;

      // INPUT_PULLUP: LOW = pressionado, HIGH = solto
      if (currentButtonState == LOW) {
        Joystick.pressButton(0);   // Pressiona botão 0
      } else {
        Joystick.releaseButton(0); // Solta botão 0
      }
    }
  }

  lastButtonState = reading;

  // Envia o estado atual para o PC
  Joystick.sendState();

  delay(10); // ~100Hz de atualização
}