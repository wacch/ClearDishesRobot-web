import socket
import pickle
import os
import json
import requests

IP_ADDR = "192.168.3.50" # IPアドレスを指定
PORT = 9001 # ポートを指定
PORT_SOCK = 9002

data = [0,0,1,0,2,0,2,1,0,1]

#sv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#sv.bind((socket.gethostname(), 1234))  # IPとポート番号を指定します
#sv.listen(5)

#cl, address = sv.accept()
#print(f"Accept: {address} との接続を確立しました")
#while True:
	#while True:	
#	print("Job: 指示待機中 ", end="")
#	msg = cl.recv(1024)
		#if len(msg) <= 0:
		#	break
		#full_msg += msg
	#d = pickle.loads(full_msg)
#	d = pickle.loads(msg)
#	print("--> recv:", d)

sv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# IPとポート番号を指定
sv.bind((IP_ADDR, PORT_SOCK))
sv.listen(5)
cl, address = sv.accept()
print(f"Socket Accept: Connection from {address} has been established")

	msg = pickle.dumps(data)
	print("Send to client:", msg)
	cl.send(msg)
	cl.close()
	#busy = False