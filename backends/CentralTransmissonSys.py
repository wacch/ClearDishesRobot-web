from websocket_server import WebsocketServer
import logging
import socket
import pickle
import time

IP_ADDR = "192.168.3.50" # IPアドレスを指定
PORT = 9001 # ポートを指定
PORT_SOCK = 9002

data = [0,0,1,0,2,0,2,1,0,1]

class Websocket_Server():
	def __init__(self_w, host, port):
		self_w.server = WebsocketServer(port=PORT, host=IP_ADDR, loglevel=logging.DEBUG)

	# クライアント接続時に呼ばれる関数
	def new_client(self_w, client, server):
		print("new client connected and was given id {}".format(client['id']))
		# 全クライアントにメッセージを送信
		self_w.server.send_message_to_all("hey all, a new client has joined us")

	# クライアント切断時に呼ばれる関数
	def client_left(self_w, client, server):
		print("client({}) disconnected".format(client['id']))

    # クライアントからメッセージを受信したときに呼ばれる関数
	def message_received(self_w, client, server, message):
		#if not busy: 
		print("client({}) said: {}".format(client['id'], message))
		# 全クライアントにメッセージを送信
		self_w.server.send_message_to_all(message)
		if message == "robot":
			self_w.server.send_message_to_all("send positions to the robot. please wait")
			print("Connection Waiting from robot")
			#busy = True
			Socket_Server()
			print("Data transmission is complete")
		#else:
		#	self_w.server.send_message_to_all("server is busy now")

    # サーバーを起動する
	def run(self_w):
		# クライアント接続時のコールバック関数にself.new_client関数をセット
		self_w.server.set_fn_new_client(self_w.new_client)
		# クライアント切断時のコールバック関数にself.client_left関数をセット
		self_w.server.set_fn_client_left(self_w.client_left)
		# メッセージ受信時のコールバック関数にself.message_received関数をセット
		self_w.server.set_fn_message_received(self_w.message_received) 
		self_w.server.run_forever()

def Socket_Server():
	sv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	# IPとポート番号を指定
	sv.bind((IP_ADDR, PORT_SOCK))
	sv.listen(5)
	cl, address = sv.accept()
	print(f"Socket Accept: Connection from {address} has been established")
	
	while True:	
		print("Wait: 応答待機中 ", end="")
		msg = cl.recv(1024)
		print("--> recv:", msg.decode("utf-8"))
		
		cmd = input("Send: コマンドを入力してください")
		if cmd == "e":
			break
		cl.send(bytes(cmd, "utf-8"))
		
	cl.close()
	
	#片道一回の送信、While無し
	#msg = pickle.dumps(data)
	#print("Send to client:", msg)
	#cl.send(msg)
	#cl.close()
	#busy = False

#busy = False
ws_server = Websocket_Server(IP_ADDR, PORT)
ws_server.run()