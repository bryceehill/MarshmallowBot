#include <WiFi.h>
#include <WebServer.h>

#include <ESP32Servo.h>


#if __has_include(<esp_arduino_version.h>)
  #include <esp_arduino_version.h>
#endif

// =============================
// USER SETTINGS
// =============================

// Change these to match your ESP32-C3 expansion board servo signal pins.
const int SERVO_LEFT_PIN  = 4;   // Servo 0
const int SERVO_RIGHT_PIN = 5;   // Servo 1
const int SERVO_FIRE_PIN  = 6;   // Servo 2

// WiFi hotspot name/password.
// Password must be at least 8 characters for WPA/WPA2.
const char* AP_SSID = "MarshmallowBot";
const char* AP_PASS = "12345678";

// Servo pulse settings, in microseconds.
const int SERVO_NEUTRAL_US = 1500;
const int SERVO_MIN_US     = 1000;
const int SERVO_MAX_US     = 2000;

// For continuous-rotation servos, full +/-500 us may be too aggressive.
// Start with 400. Increase later if needed.
const int DRIVE_RANGE_US   = 400;

// Fire pulse.
const int FIRE_US          = 2000;
const unsigned long FIRE_DURATION_MS = 1000;

// Safety timeout.
// If no drive command arrives within this time, stop the robot.
const unsigned long DRIVE_TIMEOUT_MS = 500;

// If one servo is physically mirrored, flip direction here.
const bool INVERT_LEFT  = false;
const bool INVERT_RIGHT = true;

// =============================
// PWM SETTINGS
// =============================



Servo leftServo;
Servo rightServo;
Servo fireServo;

const int SERVO_FREQ_HZ = 50;
const int SERVO_RES_BITS = 16;
const int SERVO_PERIOD_US = 20000;  // 50 Hz = 20 ms

// LEDC channels for Arduino-ESP32 core 2.x.
// In core 3.x, we can attach by pin directly.
const int CH_LEFT  = 0;
const int CH_RIGHT = 1;
const int CH_FIRE  = 2;

WebServer server(80);

unsigned long lastDriveCommandMs = 0;
unsigned long fireUntilMs = 0;

int currentLeftUs  = SERVO_NEUTRAL_US;
int currentRightUs = SERVO_NEUTRAL_US;
int currentFireUs  = SERVO_NEUTRAL_US;

// =============================
// HTML CONTROL PAGE
// =============================

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>MarshmallowBot</title>
  <style>
    body {
      margin: 0;
      font-family: Arial, sans-serif;
      background: #111827;
      color: white;
      text-align: center;
      touch-action: none;
      user-select: none;
    }

    h1 {
      margin-top: 18px;
      font-size: 28px;
    }

    .subtitle {
      color: #9ca3af;
      margin-bottom: 18px;
    }

    #joystickBase {
      width: 240px;
      height: 240px;
      border-radius: 50%;
      background: #374151;
      margin: 20px auto;
      position: relative;
      box-shadow: inset 0 0 20px rgba(0,0,0,0.5);
      border: 4px solid #4b5563;
      touch-action: none;
    }

    #joystickKnob {
      width: 86px;
      height: 86px;
      border-radius: 50%;
      background: #22c55e;
      position: absolute;
      left: 77px;
      top: 77px;
      box-shadow: 0 8px 20px rgba(0,0,0,0.45);
      transition: left 0.08s linear, top 0.08s linear;
    }

    #readout {
      font-size: 18px;
      margin: 12px;
      color: #d1d5db;
    }

    #fireButton {
      margin-top: 24px;
      width: 180px;
      height: 72px;
      font-size: 28px;
      font-weight: bold;
      border-radius: 18px;
      border: none;
      color: white;
      background: #dc2626;
      box-shadow: 0 8px 0 #7f1d1d;
    }

    #fireButton:active {
      transform: translateY(6px);
      box-shadow: 0 2px 0 #7f1d1d;
      background: #ef4444;
    }

    .small {
      margin-top: 20px;
      font-size: 13px;
      color: #9ca3af;
    }
  </style>
</head>
<body>
  <h1>MarshmallowBot</h1>
  <div class="subtitle">Skid steer joystick control</div>

  <div id="joystickBase">
    <div id="joystickKnob"></div>
  </div>

  <div id="readout">X: 0 &nbsp; Y: 0</div>

  <button id="fireButton">FIRE</button>

  <div class="small">
    Connect to WiFi: MarshmallowBot<br>
    Then open: 192.168.4.1
  </div>

<script>
  const base = document.getElementById("joystickBase");
  const knob = document.getElementById("joystickKnob");
  const readout = document.getElementById("readout");
  const fireButton = document.getElementById("fireButton");

  const baseSize = 240;
  const knobSize = 86;
  const center = baseSize / 2;
  const knobCenterOffset = knobSize / 2;
  const maxRadius = 90;

  let active = false;
  let lastX = 0;
  let lastY = 0;
  let lastSendMs = 0;

  function setKnob(x, y) {
    knob.style.left = (center - knobCenterOffset + x) + "px";
    knob.style.top  = (center - knobCenterOffset - y) + "px";
  }

  function sendDrive(x, y, force=false) {
    const now = Date.now();

    // Limit updates a little so the ESP32 is not spammed.
    if (!force && now - lastSendMs < 50) return;

    lastSendMs = now;
    lastX = x;
    lastY = y;

    readout.innerHTML = "X: " + x + " &nbsp; Y: " + y;

    fetch("/drive?x=" + x + "&y=" + y)
      .catch(err => {});
  }

  function handlePointer(clientX, clientY) {
    const rect = base.getBoundingClientRect();

    let dx = clientX - rect.left - center;
    let dy = clientY - rect.top - center;

    // Browser screen y is downward; robot forward should be positive.
    dy = -dy;

    const distance = Math.sqrt(dx * dx + dy * dy);

    if (distance > maxRadius) {
      dx = dx * maxRadius / distance;
      dy = dy * maxRadius / distance;
    }

    const xPercent = Math.round(dx / maxRadius * 100);
    const yPercent = Math.round(dy / maxRadius * 100);

    setKnob(dx, dy);
    sendDrive(xPercent, yPercent);
  }

  function centerJoystick() {
    active = false;
    setKnob(0, 0);
    sendDrive(0, 0, true);
  }

  base.addEventListener("pointerdown", function(e) {
    active = true;
    base.setPointerCapture(e.pointerId);
    handlePointer(e.clientX, e.clientY);
  });

  base.addEventListener("pointermove", function(e) {
    if (!active) return;
    handlePointer(e.clientX, e.clientY);
  });

  base.addEventListener("pointerup", centerJoystick);
  base.addEventListener("pointercancel", centerJoystick);
  base.addEventListener("pointerleave", function(e) {
    if (active) centerJoystick();
  });

  fireButton.addEventListener("click", function() {
    fetch("/fire").catch(err => {});
  });

  // Initialize centered.
  setKnob(0, 0);
</script>
</body>
</html>
)rawliteral";

// =============================
// SERVO PWM FUNCTIONS
// =============================

int clampInt(int value, int lo, int hi) {
  if (value < lo) return lo;
  if (value > hi) return hi;
  return value;
}

uint32_t pulseUsToDuty(int pulseUs) {
  pulseUs = clampInt(pulseUs, SERVO_MIN_US, SERVO_MAX_US);

  const uint32_t maxDuty = (1UL << SERVO_RES_BITS) - 1;
  return (uint32_t)((uint64_t)pulseUs * maxDuty / SERVO_PERIOD_US);
}

void writeServoUs(int pin, int channel, int pulseUs) {
  pulseUs = clampInt(pulseUs, SERVO_MIN_US, SERVO_MAX_US);
  uint32_t duty = pulseUsToDuty(pulseUs);

#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcWrite(pin, duty);
#else
  ledcWrite(channel, duty);
#endif
}

void attachServoPin(int pin, int channel) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  ledcAttach(pin, SERVO_FREQ_HZ, SERVO_RES_BITS);
#else
  ledcSetup(channel, SERVO_FREQ_HZ, SERVO_RES_BITS);
  ledcAttachPin(pin, channel);
#endif
}

void setDriveServos(int leftUs, int rightUs) {
  currentLeftUs = clampInt(leftUs, SERVO_MIN_US, SERVO_MAX_US);
  currentRightUs = clampInt(rightUs, SERVO_MIN_US, SERVO_MAX_US);

  writeServoUs(SERVO_LEFT_PIN, CH_LEFT, currentLeftUs);
  writeServoUs(SERVO_RIGHT_PIN, CH_RIGHT, currentRightUs);
}

void setFireServo(int fireUs) {
  currentFireUs = clampInt(fireUs, SERVO_MIN_US, SERVO_MAX_US);
  writeServoUs(SERVO_FIRE_PIN, CH_FIRE, currentFireUs);
}

void stopDrive() {
  setDriveServos(SERVO_NEUTRAL_US, SERVO_NEUTRAL_US);
}

// =============================
// SKID STEER MIXING
// =============================

// void applyDriveMix(int x, int y) {
//   x = clampInt(x, -100, 100);
//   y = clampInt(y, -100, 100);

//   // Standard skid steer:
//   // forward/back = y
//   // turn = x
//   int leftSpeed  = y + x;
//   int rightSpeed = y - x;

//   leftSpeed  = clampInt(leftSpeed, -100, 100);
//   rightSpeed = clampInt(rightSpeed, -100, 100);

//   if (INVERT_LEFT)  leftSpeed  = -leftSpeed;
//   if (INVERT_RIGHT) rightSpeed = -rightSpeed;

//   int leftUs  = SERVO_NEUTRAL_US + (leftSpeed  * DRIVE_RANGE_US) / 100;
//   int rightUs = SERVO_NEUTRAL_US + (rightSpeed * DRIVE_RANGE_US) / 100;

//   setDriveServos(leftUs, rightUs);
// }

void applyDriveMix(int x, int y)
{
    int leftSpeed = y + x;
    int rightSpeed = y - x;

    leftSpeed = constrain(leftSpeed, -100, 100);
    rightSpeed = constrain(rightSpeed, -100, 100);

    int leftUs =
        1500 + leftSpeed * 4;

    int rightUs =
        1500 - rightSpeed * 4;

    leftServo.writeMicroseconds(leftUs);
    rightServo.writeMicroseconds(rightUs);
}

// =============================
// WEB HANDLERS
// =============================

void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleDrive() {
  if (!server.hasArg("x") || !server.hasArg("y")) {
    server.send(400, "text/plain", "Missing x or y");
    return;
  }

  int x = server.arg("x").toInt();
  int y = server.arg("y").toInt();

  applyDriveMix(x, y);
  lastDriveCommandMs = millis();

  server.send(200, "text/plain", "OK");
}

unsigned long fireUntil = 0;
bool firing = false;

void handleFire()
{
    fireServo.writeMicroseconds(2000);

    fireUntil = millis() + 1000;
    firing = true;

    server.send(200, "text/plain", "FIRE");
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

// =============================
// SETUP / LOOP
// =============================

void setup() {
  Serial.begin(115200);
  delay(500);


  // attachServoPin(SERVO_LEFT_PIN, CH_LEFT);
  // attachServoPin(SERVO_RIGHT_PIN, CH_RIGHT);
  // attachServoPin(SERVO_FIRE_PIN, CH_FIRE);

  // stopDrive();
  // setFireServo(SERVO_NEUTRAL_US);

  leftServo.attach(4);
rightServo.attach(5);
fireServo.attach(6);

leftServo.writeMicroseconds(1500);
rightServo.writeMicroseconds(1500);
fireServo.writeMicroseconds(1000);

  WiFi.mode(WIFI_AP);
  bool apStarted = WiFi.softAP(AP_SSID, AP_PASS);

  Serial.println();
  Serial.println("Starting MarshmallowBot");
  Serial.print("AP started: ");
  Serial.println(apStarted ? "yes" : "no");
  Serial.print("SSID: ");
  Serial.println(AP_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/drive", handleDrive);
  server.on("/fire", handleFire);
  server.onNotFound(handleNotFound);
  server.begin();

  lastDriveCommandMs = millis();

  Serial.println("Web server started");
}

void loop() {
  server.handleClient();

  unsigned long now = millis();

  // Safety stop if the web page disconnects or stops sending commands.
  if (now - lastDriveCommandMs > DRIVE_TIMEOUT_MS) {
    stopDrive();
  }

  // Non-blocking fire servo return.
  if (firing && millis() > fireUntil)
{
    fireServo.writeMicroseconds(1000);

    firing = false;
}
  // if (fireUntilMs != 0 && now >= fireUntilMs) {
  //   setFireServo(1000);
  //   fireUntilMs = 0;
  // }
}