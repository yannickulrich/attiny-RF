void transmit(int nHighPulses, int nLowPulses) {
  digitalWrite(10, HIGH);
  delayMicroseconds( 350 * nHighPulses);
  digitalWrite(10, LOW);
  delayMicroseconds( 350 * nLowPulses);
}
void send1()
{
  Serial.write(0xf1);
  transmit(3,1);
}
void send0()
{
  Serial.write(0xf0);
  transmit(1,3);
}
void sendSync()
{
  Serial.write(0xf2);
  transmit(1,31);
}



void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(13,OUTPUT);
  pinMode(10,OUTPUT);
  
}

void loop() {
  if (Serial.available() > 0) {
      // read the incoming byte:
      while (Serial.available() < 2)
        delay(1);
      unsigned char high = Serial.read();
      unsigned char low  = Serial.read();
      unsigned int d1 = low | (high<<8);



#ifdef CALIBRATE
      while (Serial.available() < 2)
        delay(1);
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
      //Serial.print("Sending ");Serial.println(d1);
      uint8_t bitLength = 8;
      digitalWrite(13, HIGH);
      while (bitLength > 0) {
        if (d1 & 1)
          send1();
        else
          send0();
        d1 >>= 1;
        bitLength--;
        digitalWrite(13, LOW);
      }
#endif

  }

  if (Serial1.available() > 0) {
      // read the incoming byte:
      unsigned char incomingByte1 = Serial1.read();
      Serial.write(incomingByte1);
  }

}
