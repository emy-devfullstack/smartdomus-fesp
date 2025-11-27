# 🏠 SmartDomus - Integração Arduino com Socket.IO

## 📋 Visão Geral

Este projeto conecta o app React com Arduino via servidor Node.js usando Socket.IO para comunicação em tempo real.

## 🔌 Arquitetura

```
[App React] ←→ [Servidor Node.js + Socket.IO] ←→ [Arduino via Serial]
```

## 📦 1. Instalar Dependências do Servidor

```bash
cd arduino-server
npm install
```

## 🔧 2. Configurar o Arduino

1. Faça upload do arquivo `geral-automate.ino` para o Arduino
2. Conecte os componentes:

### Pinos do Varal:
- Sensor de Umidade: A0
- Servo Motor: Pino 10
- LED Status: Pino 13

### Pinos da Iluminação:
- LED 1 (Sala): Pino 6
- LED 2 (Quarto): Pino 8
- LED 3 (Cozinha): Pino 7
- Sensor PIR: Pino 2

## ⚙️ 3. Configurar o Servidor Node.js

1. Abra `arduino-server/server.js`
2. Altere a linha 13 para sua porta serial:

```javascript
path: "COM3",  // Windows: COM3, COM4, etc
                // Linux/Mac: /dev/ttyUSB0, /dev/ttyACM0, etc
```

Para descobrir a porta no Arduino IDE: **Ferramentas > Porta**

## 🚀 4. Iniciar o Servidor

```bash
cd arduino-server
npm start
```

Você verá:
```
🚀 Server SmartDomus rodando em http://localhost:3000
```

## 📱 5. Comandos Disponíveis

### Via Socket.IO:

#### Controle do Varal:
```javascript
socket.emit("comando", "ESTENDER");  // Estende o varal
socket.emit("comando", "RECOLHER");  // Recolhe o varal
```

#### Controle de Iluminação:
```javascript
socket.emit("luz", { led: "LED1", estado: true });   // Liga LED1 (Sala)
socket.emit("luz", { led: "LED2", estado: false });  // Desliga LED2 (Quarto)
socket.emit("luz", { led: "LED3", estado: true });   // Liga LED3 (Cozinha)
```

#### Receber dados do Arduino:
```javascript
socket.on("arduino-data", (data) => {
  console.log("Arduino:", data);
});
```

## 🧪 6. Testar a Integração

### Teste 1: Verificar Conexão
1. Inicie o servidor Node.js
2. Abra o app React
3. Verifique o indicador de conexão (verde = conectado)

### Teste 2: Controlar Iluminação
1. Navegue para "Iluminação"
2. Ligue/desligue as luzes
3. Verifique os LEDs físicos

### Teste 3: Controlar Varal
1. Navegue para "Varal"
2. Clique em "Estendido" ou "Recolhido"
3. Observe o movimento do servo motor

## 🐛 7. Solução de Problemas

### Servidor não conecta ao Arduino:
- Verifique se a porta serial está correta
- Certifique-se que o Arduino está conectado via USB
- Verifique se nenhum outro programa está usando a porta serial (feche Arduino IDE Serial Monitor)

### App não conecta ao servidor:
- Verifique se o servidor está rodando em `http://localhost:3000`
- Verifique o firewall
- Abra o console do navegador para ver logs

### Comandos não funcionam:
- Verifique os logs do servidor Node.js
- Verifique o Serial Monitor do Arduino
- Confirme que o código do Arduino foi carregado corretamente

## 📝 Comandos Serial do Arduino

O Arduino aceita estes comandos via Serial:

```
ESTENDER       - Estende o varal
RECOLHER       - Recolhe o varal
LED1:ON        - Liga LED 1 (manual)
LED1:OFF       - Desliga LED 1
LED2:ON        - Liga LED 2 (manual)
LED2:OFF       - Desliga LED 2
LED3:ON        - Liga LED 3 (manual)
LED3:OFF       - Desliga LED 3
AUTO           - Volta ao modo automático de presença
```

## 🌐 Uso em Rede Local

Para usar em outros dispositivos na mesma rede:

1. No servidor (`server.js`), o servidor já aceita conexões de qualquer origem
2. No app (`arduinoApi.ts`), altere:
```typescript
private serverUrl = "http://192.168.1.X:3000";  // IP do computador rodando o servidor
```

3. Descubra o IP do computador:
   - Windows: `ipconfig`
   - Linux/Mac: `ifconfig` ou `ip addr`

## 🎯 Funcionalidades

### Automáticas:
- ✅ Varal recolhe automaticamente quando detecta chuva
- ✅ Varal estende automaticamente quando o tempo está seco
- ✅ Luzes acendem por 5s quando detecta movimento (modo automático)

### Manuais (via App):
- ✅ Controle individual de 3 cômodos (Sala, Quarto, Cozinha)
- ✅ Controle manual do varal (Estender/Recolher)
- ✅ Indicador de status em tempo real
- ✅ Animações e feedback visual

## 📊 Monitoramento

O Arduino envia dados via Serial:
```
>>> Sistema SmartDomus Iniciado <<<
Umidade: 150
➡ Estendendo varal (girando anti-horário 220° → 180°)
Movimento detectado. Luzes LIGADAS.
LED1 LIGADO (manual)
```

## 🔒 Segurança

- O servidor aceita conexões de qualquer origem (CORS: *)
- Para produção, configure CORS adequadamente
- Não exponha a porta 3000 na internet sem segurança adicional

## 📚 Tecnologias Utilizadas

- **Frontend**: React + TypeScript + Socket.IO Client + Framer Motion
- **Backend**: Node.js + Express + Socket.IO
- **Hardware**: Arduino + SerialPort
- **Comunicação**: WebSocket (Socket.IO) + Serial (UART)
