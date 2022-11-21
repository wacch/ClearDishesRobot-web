import socket
import pickle
import os
import json
import requests

IP_ADDR = "192.168.3.50" # IPアドレスを指定
PORT = 9002 # ポートを指定
PORT_SOCK = 9002

data = [0,0,1,0,2,0,2,1,0,1]

print(f"Wait: 接続待機中")
sv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sv.bind((socket.gethostname(), PORT))  # IPとポート番号を指定します
sv.listen(5)

cl, address = sv.accept()
print(f"Socket Accept: Connection from {address} has been established")

print("Wait: 応答待機中 ", end="")
msg = cl.recv(1024)
print("--> recv:", msg.decode("utf-8"))

cansend = True
msg = ""
while True:	
	if cansend:
		cmd = input("Send: コマンドを入力してください:")
		if cmd == "e":
			break
		cansend = False
		cl.send(bytes(cmd, "utf-8"))
	
	msg = cl.recv(1024).decode("utf-8")
	if msg == "comp":
		cansend = True
cl.close()

#sv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# IPとポート番号を指定
#sv.bind((IP_ADDR, PORT_SOCK))
#sv.listen(5)
#cl, address = sv.accept()
#print(f"Socket Accept: Connection from {address} has been established")

#	msg = pickle.dumps(data)
#	print("Send to client:", msg)
#	cl.send(msg)
#	cl.close()
	#busy = False