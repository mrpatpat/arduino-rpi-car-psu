// TODO the arduino itself should propably be powered by accessory power. it then can "sense" the ignition and control our raspberry, but be off when the car is off completely

const int ignitionInPin = A0;
const int rpiPowerSwitchOut = 7;
const int rpiShutdownSignalOut = 6;
const int ignitionInputThreshold = 600;
const int delayMs = 200;
const int delayBeforeRpiPowerCutOff = 15;
const int delayBeforeRpiShutdownSignalStart = 5;
const int delayBeforeRpiShutdownSignalEnd = 7;

const int rpiPowerSwitchOnStatusLed = 2;
const int rpiPowerSwitchOnStatusLedGround = 3;
const int ignitionStatusLed = 12;
const int ignitionStatusLedGround = 13;
const int shutdownSignalStatusLed = 4;
const int shutdownSignalStatusLedGround = 5;

int ignitionOffTime = 0;

void setup() {
  pinMode(ignitionInPin, INPUT);
  pinMode(rpiPowerSwitchOut, OUTPUT);
  pinMode(rpiShutdownSignalOut, OUTPUT);
  pinMode(ignitionStatusLed, OUTPUT);
  pinMode(ignitionStatusLedGround, OUTPUT);
  pinMode(shutdownSignalStatusLed, OUTPUT);
  pinMode(shutdownSignalStatusLedGround, OUTPUT);
  pinMode(rpiPowerSwitchOnStatusLed, OUTPUT);
  pinMode(rpiPowerSwitchOnStatusLedGround, OUTPUT);  
  Serial.begin(9600);
}

void loop() {
  updateState();
  printStateLed();
  printStateSerial();
  delay(delayMs);
}

void updateState() {
  updateIgnitionTimers();
  if (isIgnitionOn()) {
    activateRpiPowerSwitch();
    deactivateRpiShutdownSignal();
  } else if (ignitionOffTime >= delayBeforeRpiShutdownSignalStart && ignitionOffTime < delayBeforeRpiShutdownSignalEnd) {
    activateRpiShutdownSignal();
  } else if (ignitionOffTime >= delayBeforeRpiShutdownSignalEnd && ignitionOffTime < delayBeforeRpiPowerCutOff) {
    deactivateRpiShutdownSignal();
  } else if (ignitionOffTime >= delayBeforeRpiPowerCutOff) {
    deactivateRpiPowerSwitch();
    activateRpiShutdownSignal();
  }
}

void updateIgnitionTimers() {
  if (isIgnitionOn()) {
    ignitionOffTime = 0;
  } else if (ignitionOffTime < delayBeforeRpiPowerCutOff) { // just an optimization to prevent overflows
    ignitionOffTime++;
  }
}

bool isIgnitionOn() {
  return analogRead(ignitionInPin) > ignitionInputThreshold;
}

bool isRpiShutdownSignalOn() {
  return digitalRead(rpiShutdownSignalOut) == HIGH;
}

bool isRpiPowerSwitchOn() {
  return digitalRead(rpiPowerSwitchOut) == HIGH;
}

void deactivateRpiPowerSwitch() {
  digitalWrite(rpiPowerSwitchOut, LOW);
}

void activateRpiShutdownSignal() {
  digitalWrite(rpiShutdownSignalOut, LOW);
}

void deactivateRpiShutdownSignal() {
  digitalWrite(rpiShutdownSignalOut, HIGH);
}

void activateRpiPowerSwitch() {
  digitalWrite(rpiPowerSwitchOut, HIGH);
}

void activateIgnitionStatusLed() {
  digitalWrite(ignitionStatusLed, HIGH);
  digitalWrite(ignitionStatusLedGround, LOW);
}

void deactivateIgnitionStatusLed() {
  digitalWrite(ignitionStatusLed, LOW);
  digitalWrite(ignitionStatusLedGround, LOW);
}

void activateShutdownSignalStatusLed() {
  digitalWrite(shutdownSignalStatusLed, HIGH);
  digitalWrite(shutdownSignalStatusLedGround, LOW);
}

void deactivateShutdownSignalStatusLed() {
  digitalWrite(shutdownSignalStatusLed, LOW);
  digitalWrite(shutdownSignalStatusLedGround, LOW);
}

void activateRpiPowerSwitchOnStatusLed() {
  digitalWrite(rpiPowerSwitchOnStatusLed, HIGH);
  digitalWrite(rpiPowerSwitchOnStatusLedGround, LOW);
}

void deactivateRpiPowerSwitchOnStatusLed() {
  digitalWrite(rpiPowerSwitchOnStatusLed, LOW);
  digitalWrite(rpiPowerSwitchOnStatusLedGround, LOW);
}

void printStateLed() {
  if(isIgnitionOn()) {
     activateIgnitionStatusLed();
  } else {
     deactivateIgnitionStatusLed();
  }
  if(isRpiShutdownSignalOn()) {
     activateShutdownSignalStatusLed();
  } else {
     deactivateShutdownSignalStatusLed();
  }
  if(isRpiPowerSwitchOn()) {
     activateRpiPowerSwitchOnStatusLed();
  } else {
     deactivateRpiPowerSwitchOnStatusLed();
  }
}

void printStateSerial() {
  Serial.println("IGN: " + String(isIgnitionOn()) + " - RPI_POWER: " + String(isRpiPowerSwitchOn()) + " - RPI_SHUTDOWN_SIGNAL: " + String(isRpiShutdownSignalOn()) + " - IGN_OFF_TIME(max " + String(delayBeforeRpiPowerCutOff) + "): " + String(ignitionOffTime));
}
