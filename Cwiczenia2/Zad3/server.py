import socket
import sys
serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serv.bind(('0.0.0.0', int(sys.argv[1])))
serv.listen(5)
while True:
	conn, addr = serv.accept()
	conn.send(b'Hello server<br>')
	conn.close()
