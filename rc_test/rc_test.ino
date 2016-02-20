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

unsigned char incomingByte1 = 0;
unsigned char incomingByte2 = 0;
void loop() {
  if (Serial.available() > 0) {
      // read the incoming byte:
      incomingByte1 = Serial.read();
      incomingByte2 = Serial.read();
      //mySwitch.send(0x050515+incomingByte, 24);
      delay(10);
      digitalWrite(10, HIGH);
      delay(incomingByte1);
      digitalWrite(10, LOW);
      delay(incomingByte2);
      digitalWrite(10, HIGH);
      delay(1000);
      digitalWrite(10, LOW);

      // say what you got:
      //Serial.print("I sent ");
      //Serial.println(incomingByte);
      //Serial.println(0x050515+incomingByte, HEX);

      incomingByte1=0;
  }

  if (Serial1.available() > 0) {
      // read the incoming byte:
      incomingByte1 = Serial1.read();
      Serial.write(incomingByte1);
  }

}
