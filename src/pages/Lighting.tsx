import { useState, useEffect } from "react";
import { Header } from "@/components/layout/Header";
import { Card } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { arduinoService } from "@/services/arduinoApi";
import { motion } from "framer-motion";

const Lighting = () => {
  const [lights, setLights] = useState({
    sala: false,
    quarto: false,
    cozinha: false,
  });
  const [connected, setConnected] = useState(false);

  useEffect(() => {
    const socket = arduinoService.connect();
    
    socket.on("connect", () => setConnected(true));
    socket.on("disconnect", () => setConnected(false));

    return () => {
      arduinoService.disconnect();
    };
  }, []);

  const handleToggle = (room: keyof typeof lights, newState: boolean) => {
    setLights({ ...lights, [room]: newState });
    
    const ledMap = {
      sala: "LED1",
      quarto: "LED2", 
      cozinha: "LED3",
    };
    
    arduinoService.setLight(ledMap[room] as "LED1" | "LED2" | "LED3", newState);
  };

  const rooms = [
    { id: "sala" as const, name: "Cômodo 1" },
    { id: "quarto" as const, name: "Cômodo 2" },
    { id: "cozinha" as const, name: "Cômodo 3" },
  ];

  return (
    <div className="min-h-screen bg-[#0A0F1C] px-10 py-6">
      <Header title="Controle de Iluminação" />
      
      <div className="flex justify-center mb-6">
        <div className={`flex items-center gap-2 px-4 py-2 rounded-full ${
          connected ? "bg-green-500/20 text-green-400" : "bg-red-500/20 text-red-400"
        }`}>
          <div className={`w-2 h-2 rounded-full ${connected ? "bg-green-400" : "bg-red-400"}`} />
          <span className="text-sm">{connected ? "Conectado" : "Desconectado"}</span>
        </div>
      </div>

      <main className="container max-w-2xl mx-auto">
        <div className="grid gap-6">
          {rooms.map((room, i) => (
            <motion.div
              key={room.id}
              initial={{ opacity: 0, x: -20 }}
              animate={{ opacity: 1, x: 0 }}
              transition={{ duration: 0.4, delay: i * 0.1 }}
            >
              <Card className="p-6 bg-[#0D1326] border-none shadow-[0_0_30px_5px_rgba(0,0,255,0.15)]">
                <h2 className="text-2xl font-bold text-blue-200 mb-4">{room.name}</h2>
                <div className="flex gap-4">
                  <Button
                    onClick={() => handleToggle(room.id, true)}
                    disabled={!connected}
                    className={`flex-1 py-6 text-lg rounded-xl transition-all ${
                      lights[room.id]
                        ? "bg-gradient-to-r from-green-500 to-emerald-600 text-white shadow-[0_0_20px_rgba(34,197,94,0.5)]"
                        : "bg-[#1a1f35] text-blue-300 hover:bg-[#232942] border border-blue-800/30"
                    }`}
                  >
                    Ligar
                  </Button>
                  <Button
                    onClick={() => handleToggle(room.id, false)}
                    disabled={!connected}
                    className={`flex-1 py-6 text-lg rounded-xl transition-all ${
                      !lights[room.id]
                        ? "bg-gradient-to-r from-red-500 to-rose-600 text-white shadow-[0_0_20px_rgba(239,68,68,0.5)]"
                        : "bg-[#1a1f35] text-blue-300 hover:bg-[#232942] border border-blue-800/30"
                    }`}
                  >
                    Desligar
                  </Button>
                </div>
              </Card>
            </motion.div>
          ))}
        </div>
      </main>
    </div>
  );
};

export default Lighting;
