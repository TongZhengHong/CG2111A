/*
   Alex's motor drivers.
*/

//volatile char leftMotorPWM = 0;
//volatile char rightMotorPWM = 0;
//
//ISR(TIMER0_COMPA_vect) {
//  leftMotorPWM = !leftMotorPWM; // Toggle PWM signal
//}
//
//ISR(TIMER0_COMPB_vect) {
//  rightMotorPWM = !rightMotorPWM; // Toggle PWM signal
//}

/* Motor set up:
  A1IN - Pin 5, PD5, OC0B
  A2IN - Pin 6, PD6, OC0A
  B1IN - Pin 10, PB2, OC1B
  B2In - pIN 11, PB3, OC2A
*/  
void setupMotors() {
  TCNT0 = 0;
  TCNT2 = 0
  
  OCR0A = 0; OCR0B = 0;
  OCR2A = 0; OCR2B = 0;
  
//  TIMSK0 = B00000110; // Enable interrupt for OCR0A & OCR0B (bit 2 & 1)
  TCCR0A = B00000001; // Set phase correct PWM mode, disable waveform output
  TCCR2A = B00000001; // Set phase correct PWM mode, disable waveform output
}

void startMotors() { // Start PWM for motors
  TCCR0B = B00000011; // Set 64 prescaler for 490Hz PWM freq (16Mhz / 64 * 510)
  TCCR2B = B00000011; // Set 64 prescaler for 490Hz PWM freq (16Mhz / 64 * 510)
}

void rightMotorForward() {
  TCC2A = B10000001; // Enable waveform on OC2A pin
}

void rightMotorReverse() {
  TCC2A = B00100001; // Enable waveform on OC2B pin
}

void leftMotorForward() {
  TCC0A = B10000001; // Enable waveform on OC0A pin
}

void leftMotorReverse() {
  TCC0A = B00100001; // Enable waveform on OC0B pin
}

// Convert percentages to PWM values
int pwmVal(float percent) {
  if (percent < 0.0)
    percent = 0;

  if (percent > 100.0)
    percent = 100.0;

  OCR0A = (int) ((percent / 100.0) * 255.0);
  OCR0B = (int) ((percent / 100.0) * 255.0);
  return (int) ((percent / 100.0) * 255.0);
}

// Move Alex forward "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// move forward at half speed.
// Specifying a distance of 0 means Alex will
// continue moving forward indefinitely.
void forward() { // float dist, float speed
  dir = FORWARD;
  int val = pwmVal(speed);

//  targetDist = forwardDist + dist;

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.
  
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
  dir = REVERSE;
  int val = pwmVal(speed);
  
//  targetDist = reverseDist + dist;

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.

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
  dir = LEFT;
  int val = pwmVal(speed/2); // Use half of normal speed for turn

//  unsigned long deltaTicks = (ang / 360.0) * (ALEX_CIRC / WHEEL_CIRC) * COUNTS_PER_REV;
//  targetTurnTicks = leftReverseTicks + deltaTicks;

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn left we reverse the left wheel and move
  // the right wheel forward.
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
  dir = RIGHT;
  int val = pwmVal(speed/2); // Use half of normal speed for turn

//  unsigned long deltaTicks = (ang / 360.0) * (ALEX_CIRC / WHEEL_CIRC) * COUNTS_PER_REV;
//  targetTurnTicks = rightReverseTicks + deltaTicks;

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn right we reverse the right wheel and move
  // the left wheel forward.
  leftMotorForward();
  rightMotorReverse();
//  analogWrite(RR, val);
//  analogWrite(LF, val);
//  analogWrite(LR, 0);
//  analogWrite(RF, 0);
}

// Stop Alex. To replace with bare-metal code later.
void stop() {
  dir = STOP;
  analogWrite(LF, 0);
  analogWrite(LR, 0);
  analogWrite(RF, 0);
  analogWrite(RR, 0);
}
