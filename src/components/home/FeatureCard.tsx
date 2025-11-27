import { LucideIcon } from "lucide-react";
import { Card } from "@/components/ui/card";

interface FeatureCardProps {
  icon: LucideIcon;
  title: string;
  onClick: () => void;
}

export const FeatureCard = ({ icon: Icon, title, onClick }: FeatureCardProps) => {
  return (
    <Card
      onClick={onClick}
      className="flex flex-col items-center justify-center gap-4 p-6 cursor-pointer transition-all hover:scale-105 active:scale-95 bg-device-bg hover:bg-device-bg-hover border-border hover:border-glow/50 hover:shadow-lg hover:shadow-glow/20"
    >
      <div className="p-4 rounded-full bg-primary/10 ring-1 ring-primary/20">
        <Icon className="h-8 w-8 text-primary" />
      </div>
      <h3 className="text-sm font-medium text-center text-foreground">{title}</h3>
    </Card>
  );
};
