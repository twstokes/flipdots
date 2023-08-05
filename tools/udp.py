import socket

IP = "IP ADDRESS HERE"
PORT = 4210
# MESSAGE = b"Testing"
PAYLOAD = [0] * 56
# PAYLOAD[0] = 3
# PAYLOAD[1] = 0
print(PAYLOAD)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print("Sending payload")
sock.sendto(bytearray(PAYLOAD), (IP, PORT))

data, addr = sock.recvfrom(1024)  # buffer size is 1024 bytes
print("Received: %s" % data)
