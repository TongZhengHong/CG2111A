void setupUltrasonic() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as Output
  pinMode(echoPin, INPUT); // Sets the echoPin as Input
}

uint32_t getUltrasonicDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  unsigned long duration = pulseIn(echoPin, HIGH);
  return duration * SPEED_OF_SOUND / 20000.0 - DIST_OFFSET; // Calculate the distance
}


void sendDistance() {
  uint32_t ultrasonicDist = getUltrasonicDistance();
  
  TPacket distancePacket;
  distancePacket.packetType = PACKET_TYPE_RESPONSE;
  distancePacket.command = RESP_DIST;
  distancePacket.params[0] = ultrasonicDist;
  sendResponse(&distancePacket);

  sendOK();
}

