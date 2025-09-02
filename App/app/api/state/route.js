import { NextResponse } from "next/server";

let lastState = { relay1: false, relay2: false, switch: false, source: "init" };

// ✅ Reçoit les données envoyées par l’ESP32
export async function POST(request) {
  const body = await request.json();
  console.log("📡 État reçu :", body);
  lastState = body;
  return NextResponse.json({ ok: true, received: body });
}

// ✅ Permet au dashboard de lire le dernier état
export async function GET() {
  return NextResponse.json(lastState);
}
