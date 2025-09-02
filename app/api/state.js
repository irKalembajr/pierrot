let lastState = { relay1: false, relay2: false, switch: false, reason: "init" };

export default function handler(req, res) {
  if (req.method === "POST") {
    lastState = req.body;
    console.log("État reçu:", lastState);
    return res.status(200).json({ ok: true, received: lastState });
  }
  if (req.method === "GET") {
    return res.status(200).json(lastState);
  }
  res.status(405).json({ error: "Méthode non autorisée" });
}
