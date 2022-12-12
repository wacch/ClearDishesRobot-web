from websocket_server import WebsocketServer
import logging
import socket
import pickle
import time

IP_ADDR = "192.168.3.50" # IPアドレスを指定
PORT_WS = 9001 # ポートを指定
PORT_SOCK = 9002

data = ["A","E","F","D","G"]

class WebsocketServer():
	client_route = 0
	client_assign = 0
	
	def __init__(self, host, port):
		self.server = WebsocketServer(port=port, host=host, loglevel=logging.DEBUG)

	# クライアント接続時に呼ばれる関数
	def new_client(self, client, server):
		print("new client connected and was given id {}".format(client['id']))
		# 全クライアントにメッセージを送信
		#self.server.send_message_to_all("hey all, a new client has joined us")

	# クライアント切断時に呼ばれる関数
	def client_left(self, client, server):
		print("client({}) disconnected".format(client['id']))
		if client == self.client_route:
			self.client_route = 0
		elif client == self.client_assign:
			self.client_assign = 0

	# クライアントからメッセージを受信したときに呼ばれる関数
	def message_received(self, client, server, message):
		#if not busy: 
		print("client({}) said: {}".format(client['id'], message))
		# 全クライアントにメッセージを送信
		#self.server.send_message_to_all(message)

		if message == "routecalc" and self.client_route != 0:
			self.client_route = client
			self.server.send_message(self.client_route, data)
			print("route client({}) registerd".format(self.client_route)
		elif message == "routeassign" and self.client_assign != 0:
			self.client_assign = client
			print("assign client({}) registerd".format(self.client_assign)
		'''
		if message == "robot":
			self.server.send_message_to_all("send positions to the robot. please wait")
			print("Connection Waiting from robot")
			#busy = True
			SocketServer()
			print("Data transmission is complete")
		
		#else:
		#	self.server.send_message_to_all("server is busy now")
		'''

	# サーバーを起動する
	def run(self):
		# クライアント接続時のコールバック関数にself.new_client関数をセット
		self.server.set_fn_new_client(self.new_client)
		# クライアント切断時のコールバック関数にself.client_left関数をセット
		self.server.set_fn_client_left(self.client_left)
		# メッセージ受信時のコールバック関数にself.message_received関数をセット
		self.server.set_fn_message_received(self.message_received) 
		self.server.run_forever()

def SocketServer():
	sv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	# IPとポート番号を指定
	sv.bind((IP_ADDR, PORT_SOCK))
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
	
	#片道一回の送信、While無し
	#msg = pickle.dumps(data)
	#print("Send to client:", msg)
	#cl.send(msg)
	#cl.close()
	#busy = False

#busy = False
ws_server = WebsocketServer(IP_ADDR, PORT_WS)
ws_server.run()