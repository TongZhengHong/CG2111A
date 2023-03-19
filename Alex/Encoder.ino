/*
   Setup and start codes for external interrupts and
   pullup resistors.
*/
// Enable pull up resistors on pins 2 and 3
void enablePullups() {
  // Use bare-metal to enable the pull-up resistors on pins
  // 2 and 3. These are pins PD2 and PD3 respectively.
  // We set bits 2 and 3 in DDRD to 0 to make them inputs.
  DDRD &= B11110011;
  PORTD |= B00001100;
}

// Functions to be called by INT0 and INT1 ISRs.
void leftISR() {
  if (dir == FORWARD) {
    leftForwardTicks++;
    forwardDist = (unsigned long) ((float) leftForwardTicks / COUNTS_PER_REV * WHEEL_CIRC);
    
  } else if (dir == REVERSE) {
    leftReverseTicks++;
    reverseDist = (unsigned long) ((float) leftReverseTicks / COUNTS_PER_REV * WHEEL_CIRC);
    
  } else if (dir == LEFT) {
    leftReverseTicks++;
    
  } else if (dir == RIGHT) {
    leftForwardTicks++;
  }
  //  Serial.print("LEFT: ");
  //  Serial.println((float) leftTicks / COUNTS_PER_REV);
  //  Serial.println((float) left/Ticks / COUNTS_PER_REV * WHEEL_CIRC);
}

void rightISR() {
  if (dir == FORWARD) {
    rightForwardTicks++;
    
  } else if (dir == REVERSE) {
    rightReverseTicks++;
    
  } else if (dir == LEFT) {
    rightForwardTicks++;
    
  } else if (dir == RIGHT) {
    rightReverseTicks++;
  }
  //  Serial.print("RIGHT: ");
  //  Serial.println((float) rightTicks / COUNTS_PER_REV);
  //  Serial.println((float) rightTicks / COUNTS_PER_REV * WHEEL_CIRC);
}

void setupEINT() {
  // Trigger INT0 and INT1 on FALLING edge
  EICRA |= (1 << ISC11) | (1 << ISC10) | (1 << ISC01) | (1 << ISC00);
  
  // Enable INT0 (PIN 2) and INT1 (PIN 3) interrupts
  EIMSK |= (1 << INT0) | (1 << INT1);
}

ISR(INT0_vect) {
  leftISR();
}

ISR(INT1_vect) {
  rightISR();
}
