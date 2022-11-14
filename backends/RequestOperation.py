import socket
import pickle

data = ["a","q1","c","q2","q3","d"]

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((socket.gethostname(), 9002))

full_msg = b''
while True:
	msg = s.recv(1024)
	if len(msg) <= 0:
		break
	full_msg += msg
	#s.close()
d = pickle.loads(full_msg)
print(d)
#msg = pickle.dumps(data)
#print(msg)
#s.send(msg)
