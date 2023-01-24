# -*- coding: utf-8 -*-
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

seats = ""
optimized_seats = ""
optimized_pos = "0,0"
return_pos = "0,0"
ws_server = None

'''
0 = ロボット待機状態(新規の座席リスト受信を許可)
1 = ロボット一時期間状態(新規の座席リスト受信を拒否)
'''
lock = 0

class Websocket_Server():
	client_route = 0
	client_assign = 0
	client_robot = 0
	
	def __init__(self, host, port):
		self.server = WebsocketServer(port=PORT_WS, host=IP_ADDR)
		FormatPrint(0,"wait","Awaiting Connection")

	# クライアント接続時に呼ばれる関数
	def client_connected(self, client, server):
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
	def on_message(self, client, server, message):
		global optimized_pos, lock, return_pos, seats, optimized_seats

		FormatPrint(0, "recv", "client({}) said: {}".format(client['id'], message))
		# 全クライアントにメッセージを送信
		#self.server.send_message_to_all(message)

		# ルートサーチシステムから座標データが飛んできたときの処理
		if client == self.client_route and self.client_route != 0:
			optimized_pos = message
			FormatPrint(0, "recv", "arrived optimize route: {}".format(optimized_pos))
			optimized_seats = Convert(message)
			FormatPrint(0, "debug", "converted positions to seats: {}".format(optimized_seats))
			#ルートサーチの処理が終わったらackを送信
			self.server.send_message(self.client_assign, "ack")
			FormatPrint(0, "send", "sending message to assign: ack")
			# ロボット一時帰還状態の場合
			#if lock == 1:
			#	self.server.send_message(self.client_robot, optimized_pos)
			#	lock = 0

		# ルートアサインシステムから座席データが飛んできたときの処理
		elif client == self.client_assign and self.client_assign != 0:
			# 座席取得メッセージだった場合
			if message == "get":
				self.server.send_message(self.client_assign, optimized_seats)
			else:
				if self.client_route != 0:
					if lock == 0:
						seats = message
						FormatPrint(0, "recv", "arrived seats: {}".format(seats))
						# 受信応答メッセージ
						self.server.send_message(self.client_assign, "debug")
						FormatPrint(0, "send", "sending message to assign: debug")
						# ここで座席データをルートサーチに送信
						self.server.send_message(self.client_route, seats)
						FormatPrint(0, "send", "sending seats to RoutesSearchSys: {}".format(seats))
					else:
						self.server.send_message(self.client_assign, "lock")
						FormatPrint(0, "send", "sending error: lock")
				else:
					self.server.send_message(self.client_assign, "refuse")
					FormatPrint(0, "send", "sending error: refuse")

		# ロボットから座標データが飛んできたときの処理(TCPの方に書き直す必要あり)
		elif client == self.client_robot and self.client_robot != 0:
			if self.client_route != 0:
				lock = 1
				self.server.send_message(self.client_route, message)

		# 各システムの初回接続時に飛んでくるメッセージを元に、クライアント情報を登録
		if message == "routecalc" and self.client_route == 0:
			self.client_route = client
			FormatPrint(0, "recv", "route client({}) registerd".format(self.client_route['address']))
		elif message == "routeassign" and self.client_assign == 0:
			self.client_assign = client
			FormatPrint(0, "recv", "assign client({}) registerd".format(self.client_assign['address']))
		'''
		elif message == "robot" and self.client_robot == 0:
			self.client_robot = client
			FormatPrint(0, "recv", "assign client({}) registerd".format(self.client_robot['address']))
		'''

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
		self.server.set_fn_new_client(self.client_connected)
		# クライアント切断時のコールバック関数にself.client_left関数をセット
		self.server.set_fn_client_left(self.client_left)
		# メッセージ受信時のコールバック関数にself.message_received関数をセット
		self.server.set_fn_message_received(self.on_message) 
		self.server.run_forever()
		
	def link(self,message):
		print("test message:",message)

# Socket通信サーバー
def TCP():
	global optimized_pos, lock, return_pos, ws_server

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
			'''
			elif msg == "return":
				return_pos = cl.recv(128).decode("utf-8")
				ws_server.link("test")
			elif msg == "end":
				print(Fore.YELLOW + "TCP/Close: Socket Closed")
				cl.close()
			'''

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

def Convert(place):
	place_list = place.split(',')
	positions = []
	seats = []
	for i in range(0,len(place_list),2):
		positions.append((int(place_list[i]),int(place_list[i+1])))

	for i in positions:
		if i == (0,0):
			seats.append("A")
		if i == (1,0):
			seats.append("B")
		if i == (2,0):
			seats.append("C")
		if i == (0,1):
			seats.append("D")
		if i == (1,1):
			seats.append("E")
		if i == (2,1):
			seats.append("F")
		if i == (0,2):
			seats.append("G")
		if i == (1,2):
			seats.append("H")
		if i == (2,2):
			seats.append("I")
	return ','.join(seats)

# WebSocketサーバー
def WebSocket():
	global ws_server
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
	elif proc == "debug":
		print(Fore.LIGHTMAGENTA_EX + "Debug: " + Fore.WHITE + msg)
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