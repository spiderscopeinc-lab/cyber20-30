# CYBER 2.0 — Full starter project

## Included
- Responsive React/Vite dashboard for desktop and phone.
- Web Bluetooth connect and motor commands.
- Speed slider, stop and emergency-stop commands.
- Phone camera preview (local to the phone).
- Phone GPS coordinates and save-current-location-as-home reference.
- Time-based local distance estimate.
- ESP32/TB6612FNG Arduino firmware starter.
- Safety and setup notes.

## Deploy on GitHub + Vercel
1. Extract this ZIP.
2. Upload the **contents** of this folder to your GitHub repository.
3. In Vercel select Add New → Project, import the repository.
4. Build command: `npm run build`; output directory: `dist`; framework: Vite.

## Run locally
Install Node.js 20+, then `npm install` and `npm run dev`.

## Honest feature status
Bluetooth commands require the ESP32 firmware to be flashed and the motor wiring to be correct. Web Bluetooth needs a supported browser and HTTPS. Phone camera preview is not a remote robot camera stream. Phone GPS is geographic location, not precise short-distance odometry. The local distance counter is only a time-based estimate. Accurate metre tracking needs wheel encoders and encoder-counting firmware. Autonomous vision, obstacle avoidance, and return-home navigation are not implemented and are intentionally disabled rather than pretending to be safe.

## Hardware safety
For a typical TB6612 breakout: battery positive to VM; battery negative to driver GND; ESP32 GND to the same GND; VCC logic to 3.3V; STBY to GPIO33. The example pins are in the firmware. Never connect motor battery directly to the ESP32 3.3V pin. Check driver current limits for four motors. Test with wheels raised at low speed. A physical motor power switch is recommended.
