include ../makefile.rules

blink.elf: blink.o
	@echo "ELF $<"
	@$(CC) $(CFLAGS) -o $@ $^

calibrate.elf: calibrate.o  ../serial.o
	@echo "ELF $<"
	@$(CC) $(CFLAGS) -o $@ $^
