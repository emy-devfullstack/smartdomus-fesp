#include <Servo.h>

// ----------------------------------------
// 1. PINOS DO SISTEMA VARAL/CLIMA
// ----------------------------------------
const int pinoSensorAgua = A0;
const int pinoServo = 10;
const int pinoLED_Clima = 13;
const int pinoSensorTemperatura = A1;

Servo meuServo;
bool varalEstaRecolhido = true;

// ---------- NOVAS POSIÇÕES EM MICROSEGUNDOS ----------
// Servo 270° (usar writeMicroseconds para ultrapassar 180°)
const int SERVO_POS_RECOLHE = 2600;   // ~220°
const int SERVO_POS_ESTENDE = 2400;   // ~180°
const int TEMPO_GIRO = 2500;

// Sensor de umidade
const int LIMITE_AGUA = 250;
unsigned long ultimoCheckUmidade = 0;
const long INTERVALO_CHECK_UMIDADE_MS = 500;

// ----------------------------------------
// 2. ILUMINAÇÃO COM PIR
// ----------------------------------------
const int pinoPIR = 2;
const int ledDistancia1 = 6;
const int ledDistancia2 = 8;
const int ledDistancia3 = 7;

bool estadoLuzesAcesas = false;
bool modoManualLuzes = false; // Controle manual via app
const long TEMPO_LUZ_ACESO_MS = 5000;
unsigned long ultimoMovimentoDetectado = 0;

// -------------------------------------------------------
// FUNÇÃO ILUMINAÇÃO POR PRESENÇA
// -------------------------------------------------------
void controlarPresenca() {
  // Se estiver em modo manual, não controlar por presença
  if (modoManualLuzes) return;

  int leituraPIR = digitalRead(pinoPIR);

  if (leituraPIR == HIGH) {
    ultimoMovimentoDetectado = millis();

    if (!estadoLuzesAcesas) {
      digitalWrite(ledDistancia1, HIGH);
      digitalWrite(ledDistancia2, HIGH);
      digitalWrite(ledDistancia3, HIGH);
      Serial.println("Movimento detectado. Luzes LIGADAS.");
      estadoLuzesAcesas = true;
    }
  }

  if (estadoLuzesAcesas && (millis() - ultimoMovimentoDetectado >= TEMPO_LUZ_ACESO_MS)) {
    digitalWrite(ledDistancia1, LOW);
    digitalWrite(ledDistancia2, LOW);
    digitalWrite(ledDistancia3, LOW);
    Serial.println("Sem movimento. Luzes DESLIGADAS.");
    estadoLuzesAcesas = false;
  }
}

// -------------------------------------------------------
// FUNÇÕES DO VARAL (NOVAS) COM GIRO 220° ↔ 180°
// -------------------------------------------------------
void estenderVaral() {
  Serial.println("➡ Estendendo varal (girando anti-horário 220° → 180°)");

  meuServo.attach(pinoServo);
  meuServo.writeMicroseconds(SERVO_POS_ESTENDE);
  delay(TEMPO_GIRO);
  meuServo.detach();

  digitalWrite(pinoLED_Clima, HIGH);
  varalEstaRecolhido = false;
}

void recolherVaral() {
  Serial.println("⬅ Recolhendo varal (girando horário 180° → 220°)");

  meuServo.attach(pinoServo);
  meuServo.writeMicroseconds(SERVO_POS_RECOLHE);
  delay(TEMPO_GIRO);
  meuServo.detach();

  digitalWrite(pinoLED_Clima, LOW);
  varalEstaRecolhido = true;
}

// -------------------------------------------------------
// PROCESSAR COMANDOS SERIAIS
// -------------------------------------------------------
void processarComando(String comando) {
  comando.trim();
  
  if (comando == "ESTENDER") {
    estenderVaral();
  } 
  else if (comando == "RECOLHER") {
    recolherVaral();
  }
  else if (comando == "LED1:ON") {
    digitalWrite(ledDistancia1, HIGH);
    modoManualLuzes = true;
    Serial.println("LED1 LIGADO (manual)");
  }
  else if (comando == "LED1:OFF") {
    digitalWrite(ledDistancia1, LOW);
    Serial.println("LED1 DESLIGADO");
  }
  else if (comando == "LED2:ON") {
    digitalWrite(ledDistancia2, HIGH);
    modoManualLuzes = true;
    Serial.println("LED2 LIGADO (manual)");
  }
  else if (comando == "LED2:OFF") {
    digitalWrite(ledDistancia2, LOW);
    Serial.println("LED2 DESLIGADO");
  }
  else if (comando == "LED3:ON") {
    digitalWrite(ledDistancia3, HIGH);
    modoManualLuzes = true;
    Serial.println("LED3 LIGADO (manual)");
  }
  else if (comando == "LED3:OFF") {
    digitalWrite(ledDistancia3, LOW);
    Serial.println("LED3 DESLIGADO");
  }
  else if (comando == "AUTO") {
    modoManualLuzes = false;
    Serial.println("Modo automático ativado");
  }
}

// -------------------------------------------------------
// SETUP
// -------------------------------------------------------
void setup() {
  pinMode(pinoLED_Clima, OUTPUT);

  meuServo.attach(pinoServo);
  meuServo.writeMicroseconds(SERVO_POS_RECOLHE);
  delay(500);
  meuServo.detach();

  pinMode(ledDistancia1, OUTPUT);
  pinMode(ledDistancia2, OUTPUT);
  pinMode(ledDistancia3, OUTPUT);
  pinMode(pinoPIR, INPUT);

  Serial.begin(9600);
  Serial.println(">>> Sistema SmartDomus Iniciado <<<");
}

// -------------------------------------------------------
// LOOP PRINCIPAL
// -------------------------------------------------------
void loop() {
  
  // ---------- Ler comandos seriais ----------
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    processarComando(comando);
  }

  // ---------- Monitoramento do sensor de umidade ----------
  if (millis() - ultimoCheckUmidade >= INTERVALO_CHECK_UMIDADE_MS) {
    ultimoCheckUmidade = millis();
    int valorUmidade = analogRead(pinoSensorAgua);

    Serial.print("Umidade: ");
    Serial.println(valorUmidade);

    if (valorUmidade > LIMITE_AGUA && !varalEstaRecolhido) {
      recolherVaral();
    }
    else if (valorUmidade <= LIMITE_AGUA && varalEstaRecolhido) {
      estenderVaral();
    }
  }

  // ---------- Sistema de iluminação ----------
  controlarPresenca();

  delay(10);
}
