import { useState, useEffect } from "react";
import { Header } from "@/components/layout/Header";
import { Card } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { ChevronUp, ChevronDown } from "lucide-react";
import { arduinoService } from "@/services/arduinoApi";
import { motion } from "framer-motion";

const RetractableRoof = () => {
  const [position, setPosition] = useState<"closed" | "open">("closed");
  const [connected, setConnected] = useState(false);

  useEffect(() => {
    const socket = arduinoService.connect();
    
    socket.on("connect", () => setConnected(true));
    socket.on("disconnect", () => setConnected(false));

    return () => {
      arduinoService.disconnect();
    };
  }, []);

  const handlePositionChange = (newPosition: "closed" | "open") => {
    setPosition(newPosition);
    
    if (newPosition === "open") {
      arduinoService.setVaral("ESTENDER");
    } else {
      arduinoService.setVaral("RECOLHER");
    }
  };

  const positions = [
    { 
      value: "closed" as const, 
      label: "Recolhido", 
      icon: ChevronDown,
      description: "Varal totalmente recolhido"
    },
    { 
      value: "open" as const, 
      label: "Estendido", 
      icon: ChevronUp,
      description: "Varal totalmente estendido"
    },
  ];

  return (
    <div className="min-h-screen bg-[#0A0F1C] px-10 py-6">
      <Header title="Controle do Varal" />
      
      <div className="flex justify-center mb-6">
        <div className={`flex items-center gap-2 px-4 py-2 rounded-full ${
          connected ? "bg-green-500/20 text-green-400" : "bg-red-500/20 text-red-400"
        }`}>
          <div className={`w-2 h-2 rounded-full ${connected ? "bg-green-400" : "bg-red-400"}`} />
          <span className="text-sm">{connected ? "Conectado" : "Desconectado"}</span>
        </div>
      </div>

      <main className="container max-w-2xl mx-auto">
        <motion.div
          initial={{ opacity: 0, scale: 0.9 }}
          animate={{ opacity: 1, scale: 1 }}
          transition={{ duration: 0.5 }}
        >
          <Card className="p-8 bg-[#0D1326] border-none shadow-[0_0_40px_10px_rgba(0,0,255,0.25)]">
            <div className="text-center mb-10">
              <h2 className="text-3xl font-bold text-blue-200 mb-3">
                Estado Atual
              </h2>
              <p className="text-2xl text-pink-400 font-semibold">
                {positions.find(p => p.value === position)?.label}
              </p>
            </div>

            <div className="flex gap-6">
              {positions.map((pos, i) => (
                <motion.div
                  key={pos.value}
                  className="flex-1"
                  initial={{ opacity: 0, y: 20 }}
                  animate={{ opacity: 1, y: 0 }}
                  transition={{ duration: 0.4, delay: i * 0.15 }}
                >
                  <Button
                    onClick={() => handlePositionChange(pos.value)}
                    disabled={!connected}
                    className={`w-full h-32 flex flex-col items-center justify-center gap-3 rounded-2xl transition-all ${
                      position === pos.value
                        ? "bg-gradient-to-r from-pink-500 to-purple-500 text-white shadow-[0_0_30px_rgba(236,72,153,0.5)]"
                        : "bg-[#1a1f35] text-blue-300 hover:bg-[#232942] border border-blue-800/30"
                    }`}
                  >
                    <pos.icon className="h-12 w-12" />
                    <div className="font-bold text-lg">{pos.label}</div>
                  </Button>
                </motion.div>
              ))}
            </div>
          </Card>
        </motion.div>
      </main>
    </div>
  );
};

export default RetractableRoof;
