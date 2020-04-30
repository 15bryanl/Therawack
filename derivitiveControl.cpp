int chA = 2;
int chB = 3;
int counter = 0;
int input1 = 4;
int input2 = 5;
int enable1 = 6;
int pot = A1;
void changeA() {
  if (digitalRead(chA) != digitalRead(chB)) {
    counter += 1;
  } else if (digitalRead(chA) == digitalRead(chB)) {
    counter -= 1;
  }
}
void changeB() {
  if (digitalRead(chA) != digitalRead(chB)) {
    counter -= 1;
  } else if (digitalRead(chA) == digitalRead(chB)) {
    counter += 1;
  }
}
void setup() {
  // set up interrupt pins to measure changes in ChA and ChB
  pinMode(chA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(chA), changeA, CHANGE);
  pinMode(chB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(chB), changeB, CHANGE);
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  pinMode(enable1, OUTPUT);
  pinMode(pot, INPUT);
  // set up serial monitor
  Serial.begin(9600);
}
float counter_to_angle =  1/(2*3.14159);
float v_to_counter = 34.0 * 48.0 / 1024;
float Kprop = 3; // V/rad
float Kd = 0.5;
float deltaT = 10; // ms
float previousTime = 0;
float prevVal = 0;
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(counter);
  float in = analogRead(pot);  // Read in potentiometer value
  float desired_counter = in * v_to_counter;  // change value from 0-1023 to desired counter value
  float err_rads = (desired_counter - counter) * counter_to_angle;    // Calculate error in radians
  float slope = (err_rads - prevVal) / deltaT;   // Calculate the slope of value
  float control_signal = err_rads * Kprop + Kd * slope;   // Calculate control signal
 // Movement of motor
  if (control_signal < 0) {
    digitalWrite(input1, HIGH);
    digitalWrite(input2, LOW);
  } else {
    digitalWrite(input1, LOW);
    digitalWrite(input2, HIGH);
  }
  int volts_ctrl_sig = constrain(abs(control_signal), 0, 255);  // Constrain control signal to 255 for arduino
  analogWrite(enable1, volts_ctrl_sig);   // Write val to enable 1
  prevVal = err_rads;  // Remember previous error
  while (millis() - previousTime < deltaT) { }
  previousTime = millis();
}
