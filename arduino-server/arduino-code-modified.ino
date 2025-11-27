// CÓDIGO ARDUINO MODIFICADO PARA ACEITAR COMANDOS DO APP
// Este código mantém as funcionalidades automáticas E aceita comandos via Serial

#include <Servo.h>

// ----------------------------------------
// PINOS E CONFIGURAÇÕES
// ----------------------------------------
const int pinoSensorAgua = A0;
const int pinoServo = 10;
const int pinoLED_Clima = 13;
const int pinoSensorTemperatura = A1;

const int trigPin = 2;
const int echoPin = 3;

const int ledDistancia1 = 6;  // LED Sala
const int ledDistancia2 = 7;  // LED Quarto
const int ledDistancia3 = 8;  // LED Cozinha

Servo meuServo;
bool varalEstaRecolhido = true;
const int LIMITE_AGUA = 250;
const int DISTANCIA_LIMITE = 20;
const long TEMPO_GIRO_COMPLETO_MS = 3000;

// Estados dos LEDs (controlados pelo app)
bool led1Estado = false;
bool led2Estado = false;
bool led3Estado = false;
bool modoManual = false; // Se true, ignora sensor de distância

// ----------------------------------------
// FUNÇÕES AUXILIARES
// ----------------------------------------
long lerDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duracao = pulseIn(echoPin, HIGH);
  long distancia = duracao * 0.034 / 2;
  return distancia;
}

void moverServo(int angulo) {
  meuServo.attach(pinoServo);
  meuServo.write(angulo);
  delay(TEMPO_GIRO_COMPLETO_MS);
  meuServo.detach();
}

void processarComando(String comando) {
  comando.trim();
  
  // Comandos de LED: LED1:ON, LED1:OFF, LED2:ON, etc.
  if (comando.startsWith("LED1:")) {
    modoManual = true;
    led1Estado = comando.endsWith("ON");
    digitalWrite(ledDistancia1, led1Estado ? HIGH : LOW);
    Serial.println("LED1 " + String(led1Estado ? "ligado" : "desligado"));
  }
  else if (comando.startsWith("LED2:")) {
    modoManual = true;
    led2Estado = comando.endsWith("ON");
    digitalWrite(ledDistancia2, led2Estado ? HIGH : LOW);
    Serial.println("LED2 " + String(led2Estado ? "ligado" : "desligado"));
  }
  else if (comando.startsWith("LED3:")) {
    modoManual = true;
    led3Estado = comando.endsWith("ON");
    digitalWrite(ledDistancia3, led3Estado ? HIGH : LOW);
    Serial.println("LED3 " + String(led3Estado ? "ligado" : "desligado"));
  }
  // Comando de Servo: SERVO:0, SERVO:90, SERVO:180
  else if (comando.startsWith("SERVO:")) {
    int angulo = comando.substring(6).toInt();
    if (angulo >= 0 && angulo <= 180) {
      moverServo(angulo);
      varalEstaRecolhido = (angulo == 0);
      digitalWrite(pinoLED_Clima, varalEstaRecolhido ? LOW : HIGH);
      Serial.println("Servo movido para " + String(angulo) + " graus");
    }
  }
  // Comando para desativar modo manual dos LEDs
  else if (comando == "AUTO") {
    modoManual = false;
    Serial.println("Modo automático ativado");
  }
}

// ----------------------------------------
// SETUP E LOOP
// ----------------------------------------
void setup() {
  pinMode(pinoLED_Clima, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledDistancia1, OUTPUT);
  pinMode(ledDistancia2, OUTPUT);
  pinMode(ledDistancia3, OUTPUT);
  
  meuServo.attach(pinoServo);
  meuServo.write(0);
  delay(100);
  meuServo.detach();
  
  Serial.begin(9600);
  Serial.println(">>> Arduino: Automação com Controle Remoto <<<");
}

void loop() {
  // Processar comandos via Serial
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    processarComando(comando);
  }
  
  // Sistema Varal/Clima (Automático)
  int valorUmidade = analogRead(pinoSensorAgua);
  
  if (valorUmidade > LIMITE_AGUA && !varalEstaRecolhido) {
    Serial.println("Chuva detectada. Recolhendo varal.");
    moverServo(0);
    digitalWrite(pinoLED_Clima, LOW);
    varalEstaRecolhido = true;
  }
  else if (valorUmidade <= LIMITE_AGUA && varalEstaRecolhido) {
    Serial.println("Tempo seco. Expondo varal.");
    moverServo(180);
    digitalWrite(pinoLED_Clima, HIGH);
    varalEstaRecolhido = false;
  }
  
  // Sistema de Distância/Iluminação (Automático se não estiver em modo manual)
  if (!modoManual) {
    long distanciaAtual = lerDistancia();
    
    if (distanciaAtual < DISTANCIA_LIMITE && distanciaAtual > 0) {
      digitalWrite(ledDistancia1, HIGH);
      digitalWrite(ledDistancia2, HIGH);
      digitalWrite(ledDistancia3, HIGH);
    } else {
      digitalWrite(ledDistancia1, LOW);
      digitalWrite(ledDistancia2, LOW);
      digitalWrite(ledDistancia3, LOW);
    }
  }
  
  delay(100);
}
