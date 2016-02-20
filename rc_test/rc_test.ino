/*


def unpack(ser):
   t = ser.read(1)
   if t == 'I':
    print "Int", struct.unpack(">H", ser.read(2))[0]
   if t == 'L':
    print "Long", struct.unpack(">I", ser.read(4))[0]
   return t
   

*/

/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  Serial1.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
  
}

void loop() {
  if (Serial.available() > 0) {
      // read the incoming byte:
      while (Serial.available() < 4)
        delay(1);
      unsigned char high = Serial.read();
      unsigned char low  = Serial.read();
      unsigned int d1 = low | (high<<8);

      high = Serial.read();
      low  = Serial.read();
      unsigned int d2 = low | (high<<8);
      
      
      //mySwitch.send(0x050515+incomingByte, 24);
      delay(10);
      digitalWrite(10, HIGH);
      delayMicroseconds(d1);
      digitalWrite(10, LOW);
      delayMicroseconds(d2);
      digitalWrite(10, HIGH);
      delay(1000);
      digitalWrite(10, LOW);

      // say what you got:
      //Serial.print("I sent ");
      //Serial.println(incomingByte);
      //Serial.println(0x050515+incomingByte, HEX);

  }

  if (Serial1.available() > 0) {
      // read the incoming byte:
      unsigned char incomingByte1 = Serial1.read();
      Serial.write(incomingByte1);
  }

}
