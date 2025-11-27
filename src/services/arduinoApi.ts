import { io, Socket } from "socket.io-client";

class ArduinoService {
  private socket: Socket | null = null;
  private serverUrl = "http://localhost:3000";

  connect() {
    if (this.socket?.connected) return this.socket;

    this.socket = io(this.serverUrl, {
      reconnection: true,
      reconnectionDelay: 1000,
      reconnectionAttempts: 5,
    });

    this.socket.on("connect", () => {
      console.log("✅ Conectado ao servidor Arduino");
    });

    this.socket.on("disconnect", () => {
      console.log("❌ Desconectado do servidor Arduino");
    });

    this.socket.on("arduino-data", (data: string) => {
      console.log("📡 Arduino:", data);
    });

    return this.socket;
  }

  disconnect() {
    this.socket?.disconnect();
    this.socket = null;
  }

  // Controle de iluminação
  setLight(led: "LED1" | "LED2" | "LED3", estado: boolean) {
    if (!this.socket?.connected) {
      console.error("❌ Socket não conectado");
      return;
    }

    this.socket.emit("luz", { led, estado });
    console.log(`💡 ${led} ${estado ? "LIGADO" : "DESLIGADO"}`);
  }

  // Controle do varal
  setVaral(comando: "ESTENDER" | "RECOLHER") {
    if (!this.socket?.connected) {
      console.error("❌ Socket não conectado");
      return;
    }

    this.socket.emit("comando", comando);
    console.log(`🏠 Varal: ${comando}`);
  }

  // Escutar dados do Arduino
  onArduinoData(callback: (data: string) => void) {
    this.socket?.on("arduino-data", callback);
  }

  // Verificar status de conexão
  isConnected(): boolean {
    return this.socket?.connected || false;
  }
}

export const arduinoService = new ArduinoService();
