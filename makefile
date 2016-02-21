include makefile.rules

recv.elf: recv.o  serial.o
	@echo "ELF $<"
	@$(CC) $(CFLAGS) -o $@ $^
