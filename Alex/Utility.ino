void waitForHello() {
  int exit = 0;

  while (!exit) {
    TPacket hello;
    TResult result;

    do {
      result = readPacket(&hello);
    } while (result == PACKET_INCOMPLETE);

    if (result == PACKET_OK) {
      if (hello.packetType == PACKET_TYPE_HELLO) {
        sendOK();
        exit = 1;
      } else sendBadResponse();
    } else if (result == PACKET_BAD) {
      sendBadPacket();
    } else if (result == PACKET_CHECKSUM_BAD)
      sendBadChecksum();
  } // !exit
}

// Clears all our counters
void clearCounters() {
  leftForwardTicks = 0;
  rightForwardTicks = 0;
  leftReverseTicks = 0;
  rightReverseTicks = 0;

  leftForwardTicksTurn = 0;
  rightForwardTicksTurn = 0;
  leftReverseTicksTurn = 0;
  rightReverseTicksTurn = 0;

  leftRevs = 0;
  rightRevs = 0;

  forwardDist = 0;
  reverseDist = 0;
}

// Clears one particular counter
void clearOneCounter(int which) {
  clearCounters(); return;

  switch (which) {
    case 0:
      clearCounters();
      break;

    case 1:
      leftForwardTicks = 0;
      break;

    case 2:
      rightForwardTicks = 0;
      break;

    case 3:
      leftReverseTicks = 0;
      break;

    case 4:
      rightReverseTicks = 0;
      break;

    case 5:
      forwardDist = 0;
      break;

    case 6:
      reverseDist = 0;
      break;
  }
}

// Intialize Vincet's internal states
void initializeState() {
  clearCounters();
}
