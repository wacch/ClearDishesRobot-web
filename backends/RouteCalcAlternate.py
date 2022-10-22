import socket
import pickle
import os
import json
import requests

sv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sv.bind((socket.gethostname(), 1234))  # IPとポート番号を指定します
sv.listen(5)

cl, address = sv.accept()
print(f"Accept: {address} との接続を確立しました")
while True:
	#while True:	
	print("Job: 指示待機中 ", end="")
	msg = cl.recv(1024)
		#if len(msg) <= 0:
		#	break
		#full_msg += msg
	#d = pickle.loads(full_msg)
	d = pickle.loads(msg)
	print("--> recv:", d)