from websocket_server import WebsocketServer
import colorama
from colorama import Fore, Back
import logging
import socket
import pickle
import threading
import time

IP_ADDR = "192.168.3.50" # IPアドレスを指定
PORT_WS = 9001 # ポートを指定
PORT_SOCK = 9002
BUFFER = 1024

#data = ["A","E","F","D","G"]
seats = ""
optimized_pos = "0,0"

class Websocket_Server():
	client_route = 0
	client_assign = 0
	client_robot = 0
	
	def __init__(self, host, port):
		self.server = WebsocketServer(port=PORT_WS, host=IP_ADDR)
		FormatPrint(0,"wait","Awaiting Connection")

	# クライアント接続時に呼ばれる関数
	def new_client(self, client, server):
		FormatPrint(0, "accept", "new client connected and was given id {}".format(client['id']))
		# 全クライアントにメッセージを送信
		#self.server.send_message_to_all("hey all, a new client has joined us")

	# クライアント切断時に呼ばれる関数
	def client_left(self, client, server):
		FormatPrint(0, "close", "client({}) disconnected".format(client['id']))

		# 登録済みのクライアントから切断されたら、情報を削除
		if client == self.client_route:
			self.client_route = 0
		elif client == self.client_assign:
			self.client_assign = 0
		elif client == self.client_robot:
			self.client_robot = 0

    # クライアントからメッセージを受信したときに呼ばれる関数
	def message_received(self, client, server, message):
		global optimized_pos, seats

		FormatPrint(0, "recv", "client({}) said: {}".format(client['id'], message))
		# 全クライアントにメッセージを送信
		#self.server.send_message_to_all(message)

		# ルートサーチシステムから座標データが飛んできたときの処理
		if client == self.client_route and self.client_route != 0:
			optimized_pos = message
			FormatPrint(0, "recv", "arrived optimize route: {}".format(optimized_pos))
		# ルートアサインシステムから座席データが飛んできたときの処理
		elif client == self.client_assign and self.client_assign != 0:
			seats = message
			FormatPrint(0, "recv", "arrived seats: {}".format(seats))
			# 到達確認応答
			self.server.send_message(self.client_assign, "ack")
			# ここで座席データを送信
			if(self.client_route != 0):
				self.server.send_message(self.client_route, seats)
				FormatPrint(0, "send", "Sending seats to RouteSearchSys: {}".format(seats))

		# 各システムの初回接続時に飛んでくるメッセージを元に、クライアント情報を登録
		if message == "routecalc" and self.client_route == 0:
			self.client_route = client
			FormatPrint(0, "recv", "route client({}) registerd".format(self.client_route['address']))
		elif message == "routeassign" and self.client_assign == 0:
			self.client_assign = client
			FormatPrint(0, "recv", "assign client({}) registerd".format(self.client_assign['address']))
		elif message == "robot" and self.client_robot == 0:
			self.client_robot = client
			FormatPrint(0, "recv", "assign client({}) registerd".format(self.client_robot['address']))

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

# Socket通信サーバー
def TCP():
	global optimized_pos

	sv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	# IPとポート番号を指定
	sv.bind((IP_ADDR, PORT_SOCK))

	while True:
		FormatPrint(1,"wait","Awaiting Connection")
		msg = ""
		sv.listen(5)
		cl, address = sv.accept()
		FormatPrint(1,"accept"," Connection from {} has been established".format(address))
		
		while True:
			FormatPrint(1,"wait","Awaiting Responce")
			msg = cl.recv(128).decode("utf-8")
			FormatPrint(1,"recv","{}".format(msg))

			#if msg == "route":
			FormatPrint(1,"send","Sending positions to MierBot: {}".format(optimized_pos))
			cl.send(bytes(optimized_pos, "utf-8"))
			FormatPrint(1,"close","Socket Closed")
			cl.close()
			break;
			#elif msg == "end":
			#	print(Fore.YELLOW + "TCP/Close: Socket Closed")
			#	cl.close()

	'''
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
	'''

	#cl.close()

# WebSocketサーバー
def WebSocket():
	ws_server = Websocket_Server(IP_ADDR, PORT_WS)
	ws_server.run()

def FormatPrint(type, proc, msg):
	# 0=Websocket, 1=TCP
	if type == 0:
		print(Fore.CYAN + "WebSocket" + Fore.WHITE + "/",end='')
	elif type == 1:
		print(Fore.GREEN + "TCP" + Fore.WHITE + "/",end='')
	else:
		print(Fore.MAGENTA + "PrintError: Invalid socket value")
		return 

	if proc == "wait":
		print(Fore.YELLOW + "Wait: " + Fore.WHITE + msg)
	elif proc == "accept":
		print(Fore.BLUE + "Accept: " + Fore.WHITE + msg)
	elif proc == "close":
		print(Fore.YELLOW + "Close: " + Fore.WHITE + msg)
	elif proc == "recv":
		print(Fore.LIGHTGREEN_EX + "Recv: " + Fore.WHITE + msg)
	elif proc == "send":
		print(Fore.LIGHTCYAN_EX + "Send: " + Fore.WHITE + msg)
	elif proc == "error":
		print(Fore.RED + "Error: " + msg)
	else:
		print(Fore.MAGENTA + "PrintError: Invalid process name")



colorama.init(autoreset=True)
thread_tcp = threading.Thread(target=TCP)
thread_web = threading.Thread(target=WebSocket)
thread_tcp.start()
time.sleep(1)
thread_web.start()