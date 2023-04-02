/* Motor set up:
  A1IN - Pin 5, PD5, OC0B
  A2IN - Pin 6, PD6, OC0A
  B1IN - Pin 10, PB2, OC1B
  B2In - pIN 11, PB3, OC2A
*/  
void setupMotors() {
  TCNT0 = 0; TCNT1 = 0; TCNT2 = 0;

  OCR0A = 0; OCR0B = 0;
  OCR2A = 0; OCR1B = 0;
  
//  TIMSK0 = B00000110; // Enable interrupt for OCR0A & OCR0B (bit 2 & 1)

// Set phase correct PWM mode, disable waveform output
  TCCR0A = B00000001; 
  TCCR1A = B00000001; // 8-bit phase correct PWM 
  TCCR2A = B00000001; 

  DDRB |= (1 << PB2) | (1 << PB3); // Set pin 10 & 11 to OUTPUT
  DDRD |= (1 << PD5) | (1 << PD6); // Set pin 5 & 6 to OUTPUT
}

void startMotors() { // Start PWM for motors
  // Set 64 prescaler for 490Hz PWM freq (16Mhz / 64 * 510)
  TCCR0B = B00000011; 
  TCCR1B = B00000011; 
  TCCR2B = B00000011;
}

void rightMotorReverse() {
  TCCR1A = B00100001; // Enable waveform on OC1B pin
  TCCR2A = B00000001; // Disable waveform on OC2A pin
}

void rightMotorForward() {
  TCCR1A = B00000001; // Disable waveform on OC1B pin
  TCCR2A = B10000001; // Enable waveform on OC2A pin
}

void leftMotorReverse() {
  // Enable OC0A pin, Disable OC0B pin
  TCCR0A = B10000001; 
}

void leftMotorForward() {
  // Disable OC0A pin, Enable OC0B pin
  TCCR0A = B00100001; 
}

// Convert percentages to PWM values
int pwmVal(float percent) {
  if (percent < 0.0)
    percent = 0;

  if (percent > 100.0)
    percent = 100.0;

  // Left motor duty cycle
  OCR0A = (int) ((percent / 100.0) * 255.0);
  OCR0B = (int) ((percent / 100.0) * 255.0);

  // Right motor duty cycle
  OCR1B = (int) ((percent / 100.0) * 255.0);
  OCR2A = (int) ((percent / 100.0) * 255.0);
  
  return (int) ((percent / 100.0) * 255.0);
}

// Move Alex forward "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// move forward at half speed.
// Specifying a distance of 0 means Alex will
// continue moving forward indefinitely.
void forward() { // float dist, float speed
  //dir = FORWARD;
  int val = pwmVal(speed);

//  targetDist = forwardDist + dist;
  
  leftMotorForward();
  rightMotorForward();

//  analogWrite(LF, val);
//  analogWrite(RF, val);
//  analogWrite(LR, 0);
//  analogWrite(RR, 0);
}

// Reverse Alex "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// reverse at half speed.
// Specifying a distance of 0 means Alex will
// continue reversing indefinitely.
void reverse() { // float dist, float speed
  //dir = REVERSE;
  int val = pwmVal(speed);
  
//  targetDist = reverseDist + dist;

  leftMotorReverse();
  rightMotorReverse();

//  analogWrite(LR, val);
//  analogWrite(RR, val);
//  analogWrite(LF, 0);
//  analogWrite(RF, 0);
}

// Turn Alex left "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn left indefinitely.
void left() { // float ang, float speed
  //dir = LEFT;
  int val = pwmVal(speed/2); // Use half of normal speed for turn

//  targetTurnTicks = leftReverseTicks + deltaTicks;

  leftMotorReverse();
  rightMotorForward();

//  analogWrite(LR, val);
//  analogWrite(RF, val);
//  analogWrite(LF, 0);
//  analogWrite(RR, 0);
}

// Turn Alex right "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn right indefinitely.
void right() { // float ang, float speed
  //dir = RIGHT;
  int val = pwmVal(speed/2); // Use half of normal speed for turn

//  unsigned long deltaTicks = (ang / 360.0) * (ALEX_CIRC / WHEEL_CIRC) * COUNTS_PER_REV;
//  targetTurnTicks = rightReverseTicks + deltaTicks;

  leftMotorForward();
  rightMotorReverse();
  
//  analogWrite(RR, val);
//  analogWrite(LF, val);
//  analogWrite(LR, 0);
//  analogWrite(RF, 0);
}

void stop() {
  //dir = STOP;
  
  TCCR2A = B00000001; // Disable waveform on OC2A pin
  TCCR1A = B00000001; // Disable waveform on OC1B pin
  TCCR0A = B00000001; // Disable OC0A and OC0B pins
}

void manualMove(float speed, float dist, char dir)
{
	int val = pwmVal(speed);
	uint32_t temp;
	switch (dir) {
		case 'w':
			forward();
			temp = forwardDist;
			clearOneCounter(5);
			while (forwardDist < dist);
			forwardDist += temp;
			stop();
			break;
		case 'a':
			left();
			temp = leftReverseTicks;
			clearOneCounter(3);
			while (leftReverseTicks <  dist);
			leftReverseTicks += temp;
			stop();
			break;
		case 's':
			reverse();
			temp = reverseDist;
			clearOneCounter(6);
			while (reverseDist < dist);
			forwardDist += temp;
			stop();
			break;
		case 'd':
			right();
			temp = leftForwardTicks;
			clearOneCounter(1);
			while (leftForwardTicks < dist);
			leftForwardTicks += temp;
			stop();
			break;

		default:
			break;
	}
}
