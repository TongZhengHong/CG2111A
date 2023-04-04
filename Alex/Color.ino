
void setupColor() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);     // Setting the sensorOut as an input
  digitalWrite(S0, HIGH);      // Setting frequency scaling to 20%
  digitalWrite(S1, LOW);
}

////take 5 readings and return average frequency
//int avgFREQ()             
//{
//  int reading;
//  int total = 0;
//
//  for (int i = 0; i < 5; i += 1) {
//    reading = pulseIn(sensorOut, LOW);
//    total += reading;
//    delay(20);
//  }
//  return total / 5;
//}
//
//int findColor()             //find the color of the paper
//{
//  // Setting RED (R) filtered photodiodes to be read
//  digitalWrite(S2, LOW);
//  digitalWrite(S3, LOW);
//  delay(100);
//
//  // Reading the output frequency for red
//  redFreq = avgFREQ();
//  redColour = map(redFreq, 180, 410, 255, 0);
//  delay(100);
//
//  // Setting GREEN (G) filtered photodiodes to be read
//  digitalWrite(S2, HIGH);
//  digitalWrite(S3, HIGH);
//  delay(100);
//
//  // Reading the output frequency for green
//  greenFreq = avgFREQ();
//  greenColour = map(greenFreq, 282, 470, 255, 0);
//  delay(100);
//
//
//  int colorCode;
//  if (redColor > greenColor) {
//    colorCode = 0;    //0 represent red
//  } else if (greenColor > redColor) {
//    colorCode = 1;     //1 represent green
//  }
//
//  return colorCode;
//}
//
//void sendColor() {                       //send color packet
//  TPacket colorPacket;
//  colorPacket.packetType = PACKET_TYPE_RESPONSE;
//  colorPacket.command = RESP_COLOR;
//  colorPacket.params[0] = findColor();
//  sendResponse(&colorPacket);  
//}
