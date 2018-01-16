import socket

ip_port = ('127.0.0.1', 666)
sk = socket.socket()
sk.connect(ip_port)
sk.sendall(bytes('GET http://127.0.0.1 HTTP/1.0\r\n\r\n'))
server_rply = sk.recv(1024)
print str(server_rply)
sk.close()
