import serial
import struct


def sendBit(bit):
    global ser
    if bit == 1:
        high = 1050
        low = 350
    elif bit == 0:
        high = 350
        low = 1050
    else:
        high = 350
        low = 10850
    ser.write(struct.pack('>HH', high, low))
    print ser.read()
    print struct.unpack('>HH', ser.read(4))


def monitor(high, low, n=1):
    global ser,data
    
    for i in range(n):
        ser.write(struct.pack('>HH', high, low))
        while True:
            c = ser.read()
            if c == 'S':
                c = ser.read(4)
                o = struct.unpack('>HH', c)
                print o
                data.append( (high, o[0], low, o[1]) )
                break

data = []

ser=serial.Serial('/dev/cu.usbmodem1421',timeout=1,baudrate=9600)

# Obtain data
monitor(25000, 10000, 4)
monitor(25000, 30000, 4)
monitor(15000,  5000, 4)
monitor(15000,   300, 4)

monitor( 350, 10850, 8) # Sync bit   return 4-5, 133-135
monitor( 350,  1050, 8) # Zero       return 4-5, 13
monitor(1050,   350, 8) # One        return 13-14, 4-5

# Output 
X=np.array([i[0] for i in data]+[i[2] for i in data])
Y=np.array([i[1] for i in data]+[i[3] for i in data])

a, _, _, _ = np.linalg.lstsq(X[:,np.newaxis], Y)

plot(X,Y, 'o', X, a*X, '--k')
print a