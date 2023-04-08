void setupColor() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);      
  digitalWrite(S1, LOW);
}

// Take 5 readings and return average frequency
int avgFREQ() {
  int reading;
  int total = 0;

  for (int i = 0; i < 5; i++) {
    reading = pulseIn(sensorOut, LOW);
    total += reading;
    delay(colorAverageDelay);
  }
  
  return total / 5;
}

// Find the color of the paper
void findColor() { 
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  delay(colorSensorDelay);

  // Reading the output frequency for red
  redFreq = avgFREQ();
  delay(colorSensorDelay);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(colorSensorDelay);

  // Reading the output frequency for green
  greenFreq = avgFREQ();
  delay(colorSensorDelay);
}

void sendColor(uint32_t dist) {                 
  TPacket colorPacket;
  colorPacket.packetType = PACKET_TYPE_RESPONSE;
  colorPacket.command = RESP_COLOR;
  
  colorPacket.params[0] = redFreq;
  colorPacket.params[1] = greenFreq;
  colorPacket.params[2] = dist;
  
  sendResponse(&colorPacket);  
}

