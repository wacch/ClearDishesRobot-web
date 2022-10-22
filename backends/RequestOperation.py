import socket
import pickle

data = ["a","q1","c","q2","q3","d",]

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((socket.gethostname(), 1234))

while True:
    msg = pickle.dumps(data)
    print(msg)

    s.send(msg)
    #s.close()