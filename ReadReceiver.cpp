

int pins[4];
int centerVals[4];
int differenceVals[4];
volatile bool started[4];
volatile bool finished[4];
unsigned long startTime[4];
unsigned long lastCycle[4];
unsigned long lastTimeTriggered[4];
double stickVals[4];

double getGenValue(unsigned long per, int zeroValue, int maxDifference) {
	int period = (int)per;
	double val = ((double)(period - zeroValue)) / maxDifference;
	if (abs(val)>1.0) {
		val = val / (abs(val));
	}
	if (abs(val)<0.1) {
		val = 0.0;
	}
	return val;
}
void generalTrigger(int i) {
	digitalRead(pins[i]) == HIGH ? started[i] = true : finished[i] = true;

	if (started[i]) {
		started[i] = false;
		startTime[i] = micros();
	}
	if (finished[i]) {
		finished[i] = false;
		lastCycle[i] = micros() - startTime[i];
		stickVals[i] = getGenValue(lastCycle[i], centerVals[i], differenceVals[i]);
	}
}
void trigger1() {
	generalTrigger(0);
}
void trigger2() {
	generalTrigger(1);
}
void trigger3() {
	generalTrigger(2);
}
void trigger4() {
	generalTrigger(3);
}

void initReceiver(int pin1, int pin2, int pin3, int pin4) {
	int tempPins[] = { pin1,pin2,pin3,pin4 };
	for (int i = 0; i < 4; i++) {
		pins[i] = tempPins[i];
		centerVals[i] = 1500;
		differenceVals[i] = 400;
		started[i] = false;
		finished[i] = false;
		startTime[i] = 0;
		lastCycle[i] = 0;
		lastTimeTriggered[i] = micros();
		stickVals[i] = 0.0;
		int curPin = pins[i];
		if (curPin != -1) {
			pinMode(curPin, INPUT);
		}
	}
	enableInterrupt(pin1, trigger1, CHANGE);
	enableInterrupt(pin2, trigger2, CHANGE);
	enableInterrupt(pin3, trigger3, CHANGE);
	enableInterrupt(pin4, trigger4, CHANGE);

}
void setCenterVal(int channelNumber, int centerVal) {
	centerVals[channelNumber] = centerVal;
}
void setDifferenceVal(int channelNumber, int differenceVal) {
	differenceVals[channelNumber] = differenceVal;
}


