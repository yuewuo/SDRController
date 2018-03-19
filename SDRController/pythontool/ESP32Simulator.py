import socket

ip_port = ('127.0.0.1', 32001)
web = socket.socket()
web.bind(ip_port)
web.listen()
while True:
	print ("listening!")
	conn, addr = web.accept()
	data = conn.recv(1024)
	print ("len: %d" % len(data))
	datastr = str(data)
	print ("receving: %s" % datastr)
	# conn.send(bytes("GET http://60.205.187.189 HTTP/1.0\r\n\r\n", encoding='utf-8'))
	

