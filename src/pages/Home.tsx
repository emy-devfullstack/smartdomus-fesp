import { useNavigate } from "react-router-dom";
import { Lightbulb, Shirt, Settings } from "lucide-react";
import { Card } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { motion } from "framer-motion";

const Home = () => {
  const navigate = useNavigate();

  const items = [
    { icon: Lightbulb, label: "Iluminação", path: "/lighting" },
    { icon: Shirt, label: "Varal", path: "/retractable-roof" },
    { icon: Settings, label: "Configurações", path: "/settings" },
  ];

  return (
    <div className="min-h-screen bg-[#0A0F1C] px-10 py-6 text-white">
      <header className="flex justify-between items-center mb-20">
        <div className="text-2xl font-bold flex items-center gap-2">
          <span className="text-pink-500">⬢</span> SmartDomus
        </div>
        <Button 
          onClick={() => navigate("/settings")}
          className="bg-gradient-to-r from-pink-500 to-purple-500 text-white px-6 py-2 rounded-full shadow-lg hover:opacity-90"
        >
          Configurações
        </Button>
      </header>

      <div className="flex justify-center gap-20 items-center flex-wrap">
        {items.map((item, i) => {
          const Icon = item.icon;
          return (
            <motion.div
              key={i}
              initial={{ opacity: 0, scale: 0.8 }}
              animate={{ opacity: 1, scale: 1 }}
              transition={{ duration: 0.6, delay: i * 0.15 }}
              onClick={() => navigate(item.path)}
            >
              <Card className="w-56 h-56 bg-[#0D1326] rounded-3xl shadow-[0_0_40px_10px_rgba(0,0,255,0.25)] flex flex-col items-center justify-center hover:scale-105 transition-all cursor-pointer border-none">
                <div className="text-blue-400 mb-3">
                  <Icon size={80} />
                </div>
                <span className="text-blue-300 text-lg">{item.label}</span>
              </Card>
            </motion.div>
          );
        })}
      </div>
    </div>
  );
};

export default Home;
