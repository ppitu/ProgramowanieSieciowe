import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('127.0.0.1', 20124)

sock.connect(server_address)

try:
	data = sock.recv(4096)

	print(data)

finally:
	sock.close()
