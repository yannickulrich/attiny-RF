# attiny-RF
The ATtiny13 is a pretty much the cheapest AVR available. However, it only has 64 byte RAM and 4k FLASH storage so it can only run the smallest projects.

With this code I am trying to implement a dead-simple RF decoder: An [433 MHz RF receiver](https://www.sparkfun.com/products/10532) is connected to the ATtiny which decodes the RF signal and sends the result over a serial connection to another device.

Note: This is work in progress.

## Testing circuit
For testing I used an Arduino UNO as an ISP and an Arduino Micro to 'talk' to the running ATtiny. Schematics will following.

## Serial interface
The code for the serial transmission was taken from [Ralph Doncaster](http://forum.arduino.cc/index.php?topic=207467.0). With I bit of fine-tuning I found that ``TXDELAY`` must be set to 38 to communicate at 9600baud.


