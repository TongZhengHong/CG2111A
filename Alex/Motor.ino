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

// Assign global motor speed to OCRxA/B
void setMotorSpeed(float speed) {
  if (speed < 0.0) speed = 0;
  else if (speed > 100.0) speed = 100.0;

  int value = (int) ((speed / 100.0) * 255.0);
  
  // LEFT motor duty cycle
  const int LEFT_MOTOR_OFFSET = 5;
  OCR0A = value - LEFT_MOTOR_OFFSET;
  OCR0B = value - LEFT_MOTOR_OFFSET;

  // RIGHT motor duty cycle
  OCR1B = value;
  OCR2A = value;
}


void forward() { // float dist, float speed
  dir = FORWARD;
  setMotorSpeed(SPEED_FAST);

  targetDist = forwardDist + distance;
  
  leftMotorForward();
  rightMotorForward();
}


void reverse() { // float dist, float speed
  dir = REVERSE;
  setMotorSpeed(SPEED_FAST);
  
  targetDist = reverseDist + distance;

  leftMotorReverse();
  rightMotorReverse();
}


void left() { // float ang, float speed
  dir = LEFT;
  setMotorSpeed(SPEED_FAST);

//  unsigned long deltaTicks = (angle / 360.0) * (ALEX_CIRC / WHEEL_CIRC) * COUNTS_PER_REV;
//  targetTurnTicks = leftReverseTicks + angle;
//  dbprintf("Left ticks: %d\n", deltaTicks);
  lastTurnTime = millis();

  leftMotorReverse();
  rightMotorForward();
}


void right() { // float ang, float speed
  dir = RIGHT;
  setMotorSpeed(SPEED_FAST);

//  unsigned long deltaTicks = (angle / 360.0) * (ALEX_CIRC / WHEEL_CIRC) * COUNTS_PER_REV;
//  targetTurnTicks = rightReverseTicks + angle;
//  dbprintf("Right ticks: %d\n", deltaTicks);
  lastTurnTime = millis();

  leftMotorForward();
  rightMotorReverse();
}


void stop() {
  dir = STOP;
  TCCR2A = B00000001; // Disable waveform on OC2A pin
  TCCR1A = B00000001; // Disable waveform on OC1B pin
  TCCR0A = B00000001; // Disable OC0A and OC0B pins
}

