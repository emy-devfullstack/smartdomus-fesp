#include <Servo.h>

// ----------------------------------------
// 1. PINOS E CONSTANTES DO SISTEMA VARAL/CLIMA
// ----------------------------------------
const int pinoSensorAgua = A0; 
const int pinoServo = 10; 
const int pinoLED_Clima = 13; 
const int pinoSensorTemperatura = A1; 

Servo meuServo;
bool varalEstaRecolhido = true;
// NOVO LIMITE DE CALIBRAÇÃO (valor > 110 = MOLHADO)
const int LIMITE_AGUA = 110; 
// ÂNGULOS AJUSTADOS (180 é recolhido, 0 é exposto)
const int VARAL_RECOLHIDO_ANGULO = 180;
const int VARAL_EXPOSTO_ANGULO = 0;   
const long TEMPO_GIRO_COMPLETO_MS = 3000;
unsigned long ultimoCheckUmidade = 0;
const long INTERVALO_CHECK_UMIDADE_MS = 500;


// ----------------------------------------
// 2. PINOS E CONSTANTES DO SISTEMA ILUMINAÇÃO POR PRESENÇA (PIR)
// ----------------------------------------
const int pinoPIR = 2; // Pino do sensor de presença 
const int ledDistancia1 = 6;
const int ledDistancia2 = 8; 
const int ledDistancia3 = 7; 

bool estadoLuzesAcesas = false; 
const long TEMPO_LUZ_ACESO_MS = 5000;
unsigned long ultimoMovimentoDetectado = 0;

// --------------------------------------------------------------------------------

// FUNÇÃO PARA O SISTEMA DE ILUMINAÇÃO POR PRESENÇA (PIR)
void controlarPresenca() {
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
  
  // Lógica de Desligamento: Temporização Não-Bloqueante
  if (estadoLuzesAcesas && (millis() - ultimoMovimentoDetectado >= TEMPO_LUZ_ACESO_MS)) {
    digitalWrite(ledDistancia1, LOW); 
    digitalWrite(ledDistancia2, LOW);
    digitalWrite(ledDistancia3, LOW);
    Serial.println("Tempo esgotado. Sem movimento. Luzes Desligadas.");
    estadoLuzesAcesas = false; 
  }
}

// --------------------------------------------------------------------------------

void setup() {
  // CONFIGURAÇÃO DO SISTEMA VARAL/CLIMA
  pinMode(pinoLED_Clima, OUTPUT);
  meuServo.attach(pinoServo);
  // Inicia na posição RECOLHIDA (180°)
  meuServo.write(VARAL_RECOLHIDO_ANGULO); 
  delay(100);
  meuServo.detach(); // Libera o recurso Timer/PWM

  // CONFIGURAÇÃO DO SISTEMA DE ILUMINAÇÃO POR PRESENÇA (PIR)
  pinMode(ledDistancia1, OUTPUT);
  pinMode(ledDistancia2, OUTPUT);
  pinMode(ledDistancia3, OUTPUT);
  pinMode(pinoPIR, INPUT); 
  
  Serial.begin(9600);
  Serial.println(">>> Automação Residencial Integrada (Varal e Iluminação PIR) <<<");
}

// --------------------------------------------------------------------------------

void loop() {
  
  // 1. EXECUTA O SISTEMA VARAL/CLIMA (Lógica de Umidade) - Executado a cada 500ms
  if (millis() - ultimoCheckUmidade >= INTERVALO_CHECK_UMIDADE_MS) {
    ultimoCheckUmidade = millis(); 
    
    int valorUmidade = analogRead(pinoSensorAgua);
    Serial.print("Umidade (Digital): ");
    Serial.println(valorUmidade);
    
    // CONDIÇÃO 1: DETECTOU MOLHADO (valor > 110) -> DEVE RECOLHER (180°)
    if (valorUmidade > LIMITE_AGUA && !varalEstaRecolhido) {
      Serial.println("Chuva detectada. Recolhendo Varal.");

      meuServo.attach(pinoServo); 
      meuServo.write(VARAL_RECOLHIDO_ANGULO); 
      delay(TEMPO_GIRO_COMPLETO_MS); // Bloqueio necessário (3s) 
      meuServo.detach(); 

      digitalWrite(pinoLED_Clima, LOW); 
      varalEstaRecolhido = true; 
    } 
    
    // CONDIÇÃO 2: DETECTOU SECO (valor <= 110) -> DEVE EXPOR (0°)
    else if (valorUmidade <= LIMITE_AGUA && varalEstaRecolhido) {
      Serial.println("Tempo seco. Expondo Varal.");

      meuServo.attach(pinoServo); 
      meuServo.write(VARAL_EXPOSTO_ANGULO); 
      delay(TEMPO_GIRO_COMPLETO_MS); // Bloqueio necessário (3s) 
      meuServo.detach(); 

      digitalWrite(pinoLED_Clima, HIGH); 
      varalEstaRecolhido = false; 
    }
  } 

  // 2. EXECUTA O SISTEMA DE ILUMINAÇÃO POR PRESENÇA (PIR) - Executado a cada micro-loop
  controlarPresenca();
  
  delay(10); // Pequeno delay de estabilidade
}
