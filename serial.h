/* Optimized AVR305 half-duplex serial uart implementation
 * timing for 81N, 115.2kbps @8Mhz = 69.4 cycles/bit
 * and @16Mhz = 138.9 cycles/bit
 * @author: Ralph Doncaster
 * @version: $Id$
 *
 * http://forum.arduino.cc/index.php?topic=207467.0
 */

extern "C" {
	void TxByte(char);
}

#define SEND_INT(val) { \
    TxByte(((val) & 0xFF00) >>  8);\
    TxByte(((val) & 0x00FF)      );\
}
