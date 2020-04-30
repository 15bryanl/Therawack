// set up constants for haptic feedback
int desiredCounts = 0;
int numberOfDetents = 4;
float gearRatio = 4.4;
float cyclesPerGear = 12;
float countsPerCycle = 4;
// total counts till we return to angle == 0
int maxCounts = gearRatio * cyclesPerGear * countsPerCycle;
float anglesPerDetent = 2.0 * PI / numberOfDetents;
float countsPerRevolution = maxCounts; // cycles/rev * counts/cycle
float anglesPerCount = 2.0 * PI / countsPerRevolution;
int countsPerDetent = anglesPerDetent / anglesPerCount;
float Kprop = 3.0;
float loopTime = 20.0; //ms

// set up pins
int chA = 2;
int chB = 3;
int counter = 0;
int input1 = 4;
int input2 = 5;
int enable1 = 6;
int pot = A1;
int btn = 7;

// set up some constants for the motor control
float countsPerAngle =  1.0 / anglesPerCount;
float v_to_counter = gearRatio * 48.0 / 1024;
float Kd = 0;
float deltaT = 10; // ms
float previousTime = 0;
float prevVal = 0;
float startTime = 0;

// interrupt for motor control
void changeA() {
  if (digitalRead(chA) != digitalRead(chB)) {
    counter += 1;
  } else if (digitalRead(chA) == digitalRead(chB)) {
    counter -= 1;
  }
}

// other interrupt for motor control
void changeB() {
  if (digitalRead(chA) != digitalRead(chB)) {
    counter -= 1;
  } else if (digitalRead(chA) == digitalRead(chB)) {
    counter += 1;
  }
}

// function that outputs the motor control
void motor_control(float desired_counter) {
  float err_rads = (desired_counter - counter) / countsPerAngle;
  float slope = (err_rads - prevVal) / deltaT;
  float control_signal = 255 * err_rads * Kprop/5. + Kd * slope;
  if (control_signal < 0) {
    digitalWrite(input1, HIGH);
    digitalWrite(input2, LOW);
  } else {
    digitalWrite(input1, LOW);
    digitalWrite(input2, HIGH);
  }
  int volts_ctrl_sig = constrain(abs(control_signal), 0, 255);
  analogWrite(enable1, volts_ctrl_sig);
  prevVal = err_rads;
  while (millis() - previousTime < deltaT) { }
  previousTime = millis();
}


void setup() {
  // set up interrupt pins to measure changes in ChA and ChB
  pinMode(chA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(chA), changeA, CHANGE);
  pinMode(chB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(chB), changeB, CHANGE);

  // set up input and output pins
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  pinMode(enable1, OUTPUT);
  pinMode(pot, INPUT);
  pinMode(btn, INPUT_PULLUP);

  // set up serial monitor
  Serial.begin(9600);
}

void loop() {
  if (counter > 0) {
    desiredCounts = countsPerDetent*(int)((int)(((counter + 0.5 * countsPerDetent) / countsPerDetent)));
  } else {
    desiredCounts = countsPerDetent*(int)((int)(((counter - 0.5 * countsPerDetent) / countsPerDetent)));
  }
  startTime = millis();
  while (millis() - startTime < loopTime) {
    Serial.print(counter);
    Serial.print(",");
    Serial.print(desiredCounts);
    Serial.println();
    motor_control(desiredCounts);
  }
}