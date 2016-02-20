data = []
def monitor(high, low):
    global ser,data
    ser.write(chr(high)+chr(low))
    
    while True:
        c = ser.read()
        if c == 'S':
            c = ser.read(4)
            o = struct.unpack('>HH', c)
            print o
            data.append( (high, o[0], low, o[1]) )
            break

ser=serial.Serial('/dev/cu.usbmodem1421',timeout=1,baudrate=9600)

# Obtain data
monitor(25, 10);
monitor(25, 10);
monitor(25, 10);
monitor(25, 10);
monitor(2, 10);
monitor(2, 10);
monitor(2, 10);
monitor(2, 10);
monitor(2, 1);
monitor(2, 1);
monitor(2, 1);
monitor(3, 1);
monitor(3, 1);
monitor(3, 1);
monitor(3, 100);
monitor(3, 100);
monitor(3, 100);

# Output 
X=[i[0] for i in data]+[i[2] for i in data]
Y=[i[1] for i in data]+[i[3] for i in data]
fit = np.polyfit(X,Y,1)
fit_fn = np.poly1d(fit) 
plot(X,Y, 'o', X, fit_fn(X), '--k')
