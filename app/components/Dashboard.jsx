import { useEffect, useState } from "react";

export default function Dashboard() {
  const [state, setState] = useState({ relay1: false, relay2: false, switch: false });

  useEffect(() => {
    const interval = setInterval(async () => {
      try {
        const res = await fetch("/api/state");
        if (res.ok) {
          const data = await res.json();
          setState(data);
        }
      } catch (err) {
        console.error("Erreur récupération état :", err);
      }
    }, 2000);
    return () => clearInterval(interval);
  }, []);

  return (
    <div>
      <h1>État des relais</h1>
      <p>Relais 1 : {state.relay1 ? "ON" : "OFF"}</p>
      <p>Relais 2 : {state.relay2 ? "ON" : "OFF"}</p>
      <p>Switch : {state.switch ? "ON" : "OFF"}</p>
      <p>Raison : {state.reason}</p>
    </div>
  );
}
