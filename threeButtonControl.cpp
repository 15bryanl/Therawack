// Number of phases
const int numPhases = 4;
// Phase pin
const int phasePins[numPhases] = {2, 3, 4, 5};
// Phase degree
const int phaseDegree = 360;
// number of increments per step (full is 1, half is 2)
const int numIncrementStep = 4;
// delay time between increments.
const int delayTime = 50;
// Step angle
const float stepAngle = 7.5;

// Global variable to keep track of current increment
int currentIncrement = 0;

// Direction
int dir = 1;

// Pattern of phase switching for one cycle of phases:
const boolean polesOnOff[numIncrementStep][numPhases] = {
	{true, true, false, false}, // step 1
	{false, true, true, false}, // step 2
	{false, false, true, true}, // step 3
	{true, false, false, true}  // step 4
};
void setup(){
	// Turn off all pins to low.
	for (int phase = 0; phase < numPhases; phase++) {
		pinMode(phasePins[phase], OUTPUT);
		digitalWrite(phasePins[phase], LOW);
	}
	// set digital input for switch in input_pullup mode.
	pinMode(12, INPUT_PULLUP); // Step
	pinMode(11, INPUT_PULLUP); // Revolution
	pinMode(10, INPUT_PULLUP); // Reverse
}

void loop() {
	// Set each phase wire on or off
	// According to current increment:
	// If we get a step…
	if (digitalRead(12) == LOW) {
		for (int phase = 0; phase < numPhases; phase++){
			digitalWrite(phasePins[phase], polesOnOff[currentIncrement][phase]);
		}
		// Delay time
		delay(delayTime);
		currentIncrement = (currentIncrement+dir) % numPhases;
		// Deal with edge case for neg. values.
		if (currentIncrement < 0) {
			currentIncrement += 4;
		}
	}
	// If we get a revolution…
	if (digitalRead(11) == LOW) {
		for (int x = 0; x < phaseDegree / stepAngle; x++) {
			for (int phase = 0; phase < numPhases; phase++){
				digitalWrite(phasePins[phase], polesOnOff[currentIncrement][phase]);
			}
			// Delay time
			delay(delayTime);
		}
		currentIncrement = (currentIncrement+dir) % numPhases;
		// Deal with edge case for neg. values.
		if (currentIncrement < 0) {
			currentIncrement += 4;
		}
	}
	// If we want to change direction…
	if (digitalRead(10) == LOW) {
		dir *= -1;
	}
}
