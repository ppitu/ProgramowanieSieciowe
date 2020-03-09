import socket
import sys
serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serv.bind((sys.argv[1], int(sys.argv[2])))
serv.listen(5)
while True:
	conn, addr = serv.accept()
	from_client = b''
	conn.send(b'Hello server<br>')
	conn.close()
