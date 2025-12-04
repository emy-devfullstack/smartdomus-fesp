import express from "express";
import http from "http";
import { Server } from "socket.io";
import { SerialPort } from "serialport";
import { ReadlineParser } from "@serialport/parser-readline";

const app = express();
const server = http.createServer(app);
const io = new Server(server, {
  cors: { origin: "*" }
});

// -------- SERIAL ARDUINO --------
const port = new SerialPort({
  path: "COM9",    // ALTERAR AQUI para sua porta (COM3, /dev/ttyUSB0, etc)
  baudRate: 9600,
});

const parser = port.pipe(new ReadlineParser({ delimiter: "\n" }));

parser.on("data", (data) => {
  data = data.trim();
  console.log("Arduino:", data);

  // Enviar para o FRONT
  io.emit("arduino-data", data);
});

// -------- RECEBER COMANDOS DO FRONT --------
io.on("connection", (socket) => {
  console.log("Cliente conectado!");

  // Comando para o VARAL
  socket.on("comando", (cmd) => {
    console.log("Comando recebido:", cmd);

    if (cmd === "ESTENDER") {
      port.write("ESTENDER\n");
    } else if (cmd === "RECOLHER") {
      port.write("RECOLHER\n");
    }
  });

  // Comandos para ILUMINAÇÃO
  socket.on("luz", (data) => {
    console.log("Comando de luz:", data);
    const { led, estado } = data;
    
    if (led === "LED1") {
      port.write(estado ? "LED1:ON\n" : "LED1:OFF\n");
    } else if (led === "LED2") {
      port.write(estado ? "LED2:ON\n" : "LED2:OFF\n");
    } else if (led === "LED3") {
      port.write(estado ? "LED3:ON\n" : "LED3:OFF\n");
    }
  });
});

server.listen(3000, () => {
  console.log("🚀 Server SmartDomus rodando em http://localhost:3000");
});
