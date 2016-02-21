void transmit(int nHighPulses, int nLowPulses) {
  digitalWrite(10, HIGH);
  delayMicroseconds( 350 * nHighPulses);
  digitalWrite(10, LOW);
  delayMicroseconds( 350 * nLowPulses);
}
void send1()
{
  //Serial.write('1');
  transmit(3,1);
}
void send0()
{
  //Serial.write('0');
  transmit(1,3);
}
void sendSync()
{
  //Serial.write('S');
  transmit(1,31);
}


void send(const char* sCodeWord) {
    int i = 0;
    while (sCodeWord[i] != '\0') {
      switch(sCodeWord[i]) {
        case '0':
          send0();
        break;
        case '1':
          send1();
        break;
      }
      i++;
    }
    sendSync();
}


void setup() {

  Serial.begin(9600);
  Serial1.begin(115200);

  pinMode(13,OUTPUT);
  pinMode(10,OUTPUT);
  
}

void loop() {
  if (Serial.available() > 0) {
      // read the incoming byte:
      



#ifdef CALIBRATE
      while (Serial.available() < 4)
        delay(1);
      unsigned char high = Serial.read();
      unsigned char low  = Serial.read();
      unsigned int d1 = low | (high<<8);
      
      high = Serial.read();
      low  = Serial.read();
      unsigned int d2 = low | (high<<8);
      
      delay(10);
      digitalWrite(10, HIGH);
      digitalWrite(13, HIGH);
      delayMicroseconds(d1);
      digitalWrite(10, LOW);
      digitalWrite(13, LOW);
      delayMicroseconds(d2);
      digitalWrite(10, HIGH);
      digitalWrite(13, HIGH);
      delay(1000);
      digitalWrite(10, LOW);
      digitalWrite(13, LOW);
#else
      /*while (Serial.available() < 3)
        delay(1);
      unsigned char high = Serial.read();
      unsigned char middle  = Serial.read();
      unsigned char low  = Serial.read();
      long d1 = low | (middle<<8) | (high<<16);
      
      uint8_t bitLength = 24;
      Serial.print(high);
      Serial.print(' ');
      Serial.print(middle);
      Serial.print(' ');
      Serial.print(low);
      Serial.print(' ');
      Serial.println(d1);
      */
      digitalWrite(13, HIGH);
      
      Serial.read();
      send( "000101010100000100010101");
      
      send1();
      digitalWrite(13, LOW);
      
      
#endif

  }

  if (Serial1.available() > 0) {
      // read the incoming byte:
      unsigned char incomingByte1 = Serial1.read();
      Serial.write(incomingByte1);
  }

}



char* dec2binWcharfill(unsigned long dec, unsigned int bitLength, char fill) {
  static char bin[32];

  bin[bitLength] = '\0';
  while (bitLength > 0) {
    bitLength--;
    bin[bitLength] = (dec & 1) ? '1' : fill;
    dec >>= 1;
  }

  return bin;
}
