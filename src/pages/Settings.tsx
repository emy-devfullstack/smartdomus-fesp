import { Header } from "@/components/layout/Header";
import { Card } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { useTheme } from "next-themes";
import { motion } from "framer-motion";

const Settings = () => {
  const { theme, setTheme } = useTheme();

  const toggleTheme = () => {
    setTheme(theme === "dark" ? "light" : "dark");
  };

  return (
    <div className="min-h-screen bg-[#0A0F1C] px-10 py-6">
      <Header title="Configurações do Sistema" />
      
      <main className="container max-w-2xl mx-auto">
        <div className="grid gap-6">
          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4 }}
          >
            <Card className="p-8 bg-[#0D1326] border-none shadow-[0_0_30px_5px_rgba(0,0,255,0.15)]">
              <Button
                onClick={toggleTheme}
                className="w-full py-8 text-lg rounded-xl bg-gradient-to-r from-pink-500 to-purple-500 text-white shadow-[0_0_30px_rgba(236,72,153,0.5)] hover:opacity-90"
              >
                Alternar Tema (Claro/Escuro)
              </Button>
            </Card>
          </motion.div>

          <motion.div
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.4, delay: 0.1 }}
          >
            <Card className="p-8 bg-[#0D1326] border-none shadow-[0_0_30px_5px_rgba(0,0,255,0.15)]">
              <Button
                disabled
                className="w-full py-8 text-lg rounded-xl bg-[#1a1f35] text-blue-300 border border-blue-800/30 opacity-60 cursor-not-allowed"
              >
                Configurações de Usuário (em breve)
              </Button>
            </Card>
          </motion.div>
        </div>
      </main>
    </div>
  );
};

export default Settings;
