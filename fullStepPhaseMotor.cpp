// Number of phases
const int numPhases = 4;
// Phase pin
const int phasePins[numPhases] = {2, 3, 4, 5};
// Phase degree
const int phaseDegree = 90;
// number of increments per step (full is 1, half is 2)
const int numIncrementStep = 4;
// delay time between increments.
const int delayTime = 1000;

// Global variable to keep track of current increment
int currentIncrement = 0;

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
}


void loop() {
	// Set each phase wire on or off
	// According to current increment:
	for (int phase = 0; phase < numPhases; phase++){
      	bool x = polesOnOff[currentIncrement][phase];
		digitalWrite(phasePins[phase], x);
	}
	// Delay time
	delay(delayTime);
	// Increment current increment by 1 in range of 0 - 3.
	currentIncrement = (currentIncrement + 1) % numPhases;
}
