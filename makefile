CC         = /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-gcc
OBJCOPY    = /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-objcopy
DUDE       = /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude
DUDECONFIG = /Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf 
AVRSIZE    = /Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-size

BOARD = attiny13
BAUD = 19200
PROGRAMMER = avrisp
PORT = /dev/tty.usbmodem1411

CFLAGS = -g -DF_CPU=1200000 -Wall -Os -mmcu=$(BOARD)
OFLAGS = -j .text -j .data -O ihex
DFLAGS = -C $(DUDECONFIG) -P $(PORT) -b $(BAUD) -c $(PROGRAMMER) -p $(BOARD)

%.o: %.cpp
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	@echo "ASM $<"
	@$(CC) $(CFLAGS) -c -o $@ $<


blink.elf: blink.o  serial.o
	@echo "ELF $<"
	@$(CC) $(CFLAGS) -o $@ $^

%.size: %.elf
	@$(AVRSIZE) --mcu=$(BOARD) -C $<

%.hex: %.elf
	@echo "OBJC $<"
	@$(OBJCOPY) $(OFLAGS) $< $@

%.upload: %.hex
	@echo "AVRDUDE $<"
	@$(DUDE) $(DFLAGS) -U flash:w:"$<"

clean:
	rm -f *.hex *.elf *.o