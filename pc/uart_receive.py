import serial
import socket

ser = serial.Serial('COM8', 115200, timeout=1)

server_ip = '127.0.0.1'
server_port = 12345
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((server_ip, server_port))

try:
    while True:
        data = ser.readline().decode('utf-8').strip()

        print("Received:", data)
        sock.sendall(data.encode('utf-8'))

except KeyboardInterrupt:
    ser.close()
    sock.close()
    print("Serial port and socket closed.")