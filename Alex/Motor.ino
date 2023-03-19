/*
   Alex's motor drivers.
*/

// Set up Alex's motors. Right now this is empty, but
// later you will replace it with code to set up the PWMs
// to drive the motors.
void setupMotors() {
  /* Our motor set up is:
        A1IN - Pin 5, PD5, OC0B
        A2IN - Pin 6, PD6, OC0A
        B1IN - Pin 10, PB2, OC1B
        B2In - pIN 11, PB3, OC2A
  */
}

// Start the PWM for Alex's motors.
// We will implement this later. For now it is
// blank.
void startMotors() {

}

// Convert percentages to PWM values
int pwmVal(float percent) {
  if (percent < 0.0)
    percent = 0;

  if (percent > 100.0)
    percent = 100.0;

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

  analogWrite(LF, val);
  analogWrite(RF, val);
  analogWrite(LR, 0);
  analogWrite(RR, 0);
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
  analogWrite(LR, val);
  analogWrite(RR, val);
  analogWrite(LF, 0);
  analogWrite(RF, 0);
}

// Turn Alex left "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn left indefinitely.
void left() { // float ang, float speed
  dir = LEFT;
  int val = pwmVal(speed);

//  unsigned long deltaTicks = (ang / 360.0) * (ALEX_CIRC / WHEEL_CIRC) * COUNTS_PER_REV;
//  targetTurnTicks = leftReverseTicks + deltaTicks;

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn left we reverse the left wheel and move
  // the right wheel forward.
  analogWrite(LR, val);
  analogWrite(RF, val);
  analogWrite(LF, 0);
  analogWrite(RR, 0);
}

// Turn Alex right "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn right indefinitely.
void right() { // float ang, float speed
  dir = RIGHT;
  int val = pwmVal(speed);

//  unsigned long deltaTicks = (ang / 360.0) * (ALEX_CIRC / WHEEL_CIRC) * COUNTS_PER_REV;
//  targetTurnTicks = rightReverseTicks + deltaTicks;

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn right we reverse the right wheel and move
  // the left wheel forward.
  analogWrite(RR, val);
  analogWrite(LF, val);
  analogWrite(LR, 0);
  analogWrite(RF, 0);
}

// Stop Alex. To replace with bare-metal code later.
void stop() {
  dir = STOP;
  analogWrite(LF, 0);
  analogWrite(LR, 0);
  analogWrite(RF, 0);
  analogWrite(RR, 0);
}
