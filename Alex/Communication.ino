/*
   Setup and start codes for serial communications
*/

void setupSerial() {
  // b = round(16,000,000 / 16.0 * 9600) - 1 = 103
  UBRR0L = 103;
  UBRR0H = 0;

  // Async mode (bit 7 & 6: 00), 8N1 config
  // No parity (bit 5 & 4: 00), 1 stop bit (bit 3: 0)
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bit setting
  UCSR0A = 0; // Turn off double-speed mode
//  Serial.begin(9600);
}

void startSerial() {
  // Disable interrupt RXCIE0, TXCIE0, UDRIE0 (bit 7 & 6 & 5: 000)
  // UCSZ02: 0 for 8-bit data size
  // Disable 9-bit data size by disabling RXB80 & TXB80 (bit 1 & 0: 00)
  UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable transmitter and receiver
}

// Read the serial port. Returns the read character in
// ch if available. Also returns TRUE if ch is valid.
int readSerial(char *buffer) {
  while (UCSR0A & (1 << RXC0) == 0);
  buffer[0] = UDR0; // Assign received byte to buffer array
  return 1;

//  int count = 0;
//  while (Serial.available())
//    buffer[count++] = Serial.read();
//  return count;
}

// Write to the serial port. 
void writeSerial(const char *buffer, int len) {
  for (int i = 0; i < len; i++) {
    while (UCSR0A & (1 << UDRE0) == 0);
    UDR0 = buffer[i];
  }
//  Serial.write(buffer, len);
}

TResult readPacket(TPacket *packet) {
  // Reads in data from the serial port and
  // deserializes it.Returns deserialized
  // data in "packet".

  char buffer[PACKET_SIZE];
  int len;

  len = readSerial(buffer);

  if (len == 0) return PACKET_INCOMPLETE;
  else return deserialize(buffer, len, packet);
}

void sendResponse(TPacket *packet) {
  // Takes a packet, serializes it then sends it out
  // over the serial port.
  char buffer[PACKET_SIZE];
  int len;

  len = serialize(buffer, packet, sizeof(TPacket));
  writeSerial(buffer, len);
}
