# attiny-RF
The ATtiny13 is a pretty much the cheapest AVR available. However, it only has 64 byte RAM and 4k FLASH storage so it can only run the smallest projects.

With this code I am trying to implement a dead-simple RF decoder: An [433 MHz RF receiver](https://www.sparkfun.com/products/10532) is connected to the ATtiny which decodes the RF signal and sends the result over a serial connection to another device.

Note: This is work in progress.

## Fuse
In factory settings your ATtiny will run at 1.2MHz instead of 9.6MHz. Use the following command
```
avrdude -P /dev/tty.usbmodem1411 -b 19200 -c avrisp -p attiny13 -U lfuse:w:0x7a:m
```
You can read your fuse settings with 
```
avrdude -P /dev/tty.usbmodem1411 -c avrisp -p attiny13 -U lfuse:r:-:h -b 19200
```


## Testing circuit
For testing I used an Arduino UNO as an ISP and an Arduino Micro to 'talk' to the running ATtiny. Schematics will following.

## Serial interface
The code for the serial transmission was taken from [Ralph Doncaster](http://forum.arduino.cc/index.php?topic=207467.0). With I bit of fine-tuning I found that ``TXDELAY`` must be set to 38 to communicate at 9600baud.


