import socket
import sys

#Create TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#Bind the socket port
sock.bind(('127.0.0.1', 20124))

#Listen 
sock.listen(5)

while True:
	connection, addr = sock.accept()
	
	while True:
		connection.send("Hello sever")

	connection.close()
