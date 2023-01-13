# -*- coding: utf-8 -*-
import random
import csv
import networkx as nx
import numpy as np
import copy
import websocket
import time
import pickle
import gc
import sys, os
try:
    import thread
except ImportError:
    import _thread as thread
	
REPEAT_NUM = 100 # 繰り返し数
ANT_NUM = 100   # アリの数
PHERO_Q = 10	   # 1回の巡回で分泌するフェロモン
EVA_R = 0.05	   # フェロモンの蒸発率
PHERO_R = 0.95	 # フェロモンに基づいて経路を選択する確率
PHERO_L = 1		# フェロモンを考慮する度合い
HEU_L = 1		  # ヒューリスティック情報を考慮する度合い

#コロニークラス
class Colony:
	def __init__(self, field):
		# 経路選択確率の分子
		self.numerator=[[0 for i in range (field.nodeNum)] for j in range(field.nodeNum)]
		

	# 経路選択
	def selectRoute(self, field, ant):
		for i in range(field.nodeNum):
			for j in range(1, i):
				self.numerator[i][j]=(field.pheromone[i][j]**PHERO_L)*((1/field.distance[i][j])**HEU_L)
			
			for j in range(i+1, field.nodeNum):
				self.numerator[i][j]=(field.pheromone[i][j]**PHERO_L)*((1/field.distance[i][j])**HEU_L)
				
		for i in range(ANT_NUM):
			ant[i].selectRoute(self, field)
		
	# フェロモン量更新   
	def renewPheromone(self, field, ant):
		for i in range(field.nodeNum):
			for j in range(1, field.nodeNum):
				field.pheromone[i][j] *= 1-EVA_R
		
		for i in range(ANT_NUM):
			ant[i].putPheromone(field)
		
			  
	# フェロモン量表示
	def printPheromone(self, field):
		for i in range(field.nodeNum):
			print()
			for j in range(field.nodeNum):
				print(f'{field.pheromone[i][j]:8.3f} ', end='')

#アントクラス
class Ant:
	minDis = []
	minList = []
	def __init__(self, field):
		self.route = [0 for i in range(field.nodeNum)]	  # 経路
		self.candidate = [0 for i in range(field.nodeNum)]  # 未訪問ノード
		self.totalDis = 0 # 総移動距離

	# 経路選択	
	def selectRoute(self, colony, field):
		for i in range(1, field.nodeNum):
			self.candidate[i] = 1
		
		self.totalDis = 0.0
		for i in range(field.nodeNum - 2):
			denominator = 0.0
			r = random.random()
			# 未訪問都市の枝の評価値aを分母に加算
			for j in range(1, field.nodeNum):
				if self.candidate[j] == 1:
					denominator += colony.numerator[self.route[i]][j]
			
			# 次のノードの選択		
			next_n = -1
			# フェロモン量に基づく選択
			if (denominator != 0 and r <= PHERO_R):
				for next_n in range(1, field.nodeNum):
					if self.candidate[next_n] == 1:
						prob = colony.numerator[self.route[i]][next_n]/denominator
						if r <= prob:
							break
						r -= prob
				if next_n == field.nodeNum:
					next_n = -1
			
			# ランダム選択			   
			if next_n == -1:
				white_list=[]
				for j in range (field.nodeNum):
					if self.candidate[j] == 1:
						white_list.append(j)
				next_n = np.random.choice(white_list)
				

			self.route[i+1] = next_n
			self.candidate[next_n]=0
			self.totalDis += field.distance[self.route[i]][next_n]
		
		# 最後の1ノードの探索
		for next_n in range(1, field.nodeNum):
			if self.candidate[next_n] == 1:
				break

		self.route[field.nodeNum - 1] = next_n
		self.totalDis += field.distance[self.route[field.nodeNum-2]][next_n]
		self.totalDis += field.distance[next_n][0]
		
		'''
		#毎探索ルートの表示-※動作遅くなる
		print(self.totalDis)
		print(self.route)
		print('')
		'''
		
		Ant.minDis.append(self.totalDis) #これまでの探索での経路長を保存
		
		'''
		#デバック用
		print('経路長',Ant.minDis)
		print('最短経路リスト',Ant.minList)
		print()
		'''
		if self.totalDis <= min(Ant.minDis):
			Ant.minList.append(copy.deepcopy(self.route))

			
	# フェロモン分泌
	def putPheromone(self, field):
		p = PHERO_Q / self.totalDis
		for i in range(field.nodeNum - 1):
			if self.route[i] < self.route[i+1]:
				field.pheromone[self.route[i]][self.route[i+1]] += p
			else:
				field.pheromone[self.route[i+1]][self.route[i]] += p
				
		field.pheromone[0][self.route[field.nodeNum-1]] += p

#フィールドクラス
class Field:
	def __init__(self,x):
		with open(x, "r") as f:
			reader = csv.reader(f)
			line = [row for row in reader]
			nodenum = len(line)
			
			self.nodeNum = nodenum  # ノード数
			self.distance = [[0 for i in range(nodenum)]for j in range(nodenum)]   # ノード間の距離
			self.pheromone = [[0 for i in range(nodenum)]for j in range(nodenum)]  # 各エッジのフェロモン
			
			# 距離行列からノード間の距離を設定
			for i in range(len(line)):
				for j in range(len(line)):
					self.distance[i][j]=float(line[i][j])

		f.close()
					
class WebsocketClient():
	#新規の座席リストを受信したら、演算し、送信
	seats = []
	positions = []

	def __init__(self, host_addr):
		websocket.enableTrace(False)

		# コールバック登録
		self.ws = websocket.WebSocketApp(host_addr,
			on_message = lambda ws, msg: self.on_message(ws, msg),
			on_error = lambda ws, msg: self.on_error(ws, msg),
			on_close = lambda ws: self.on_close(ws))
		self.ws.on_open = lambda ws: self.on_open(ws)

	# メッセージ受信に呼ばれる関数
	def on_message(self, ws, message):
		self.seats = message.split(',')
		print("receive : {}".format(self.seats))
		self.positions = optimize(self.seats)
		self.ws.send(self.positions)

		#再起動(デバッグ用)
		sys.exit()
		#thread.start_new_thread(self.run, (self.positions))

	# エラー時に呼ばれる関数
	def on_error(self, ws, error):
		print(error)

	# サーバーから切断時に呼ばれる関数
	def on_close(self, ws):
		print("### connection closed ###")

	# サーバーから接続時に呼ばれる関数
	def on_open(self, ws):
		print("### connection established ###")
		self.ws.send("routecalc")
		#thread.start_new_thread(self.run, (self.positions, True))

	# サーバーへのメッセージ送信に呼ばれる関数
	def run(self, positions, setup = False):
		#while True:
		time.sleep(0.1)
		#input_data = input("send data:") 
		#if setup == True:
		#	self.ws.send("routecalc")
		#else:
		self.ws.send(positions)
	
		#self.ws.close()
		print("send complete")
	
	# websocketクライアント起動
	def run_forever(self):
		self.ws.run_forever()


#座標から距離行列を作成する関数
def create_dist_matrix(places,f_name):
	dist_matrix=[[0 for i in range(len(places))] for j in range(len(places))] # 距離行列
	# 座標から距離行列を作成
	for i in range(len(places)):
		for j in range(len(places)):  
			if i==j:continue
			else:
				
				'''
				#ユークリッド距離を計算
				dist_matrix[i][j]=math.sqrt((places[i][0]-places[j][0])**2+(places[i][1]-places[j][1])**2)
				dist_matrix[j][i]=math.sqrt((places[i][0]-places[j][0])**2+(places[i][1]-places[j][1])**2)
				'''
				
				#マンハッタン距離を計算
				dist_matrix[i][j]=abs(places[i][0]-places[j][0])+abs(places[i][1]-places[j][1])
				dist_matrix[j][i]=abs(places[i][0]-places[j][0])+abs(places[i][1]-places[j][1])
				
	with open(f_name, 'w', newline="")as f:
		writer = csv.writer(f)
		writer.writerows(dist_matrix)

	f.close()

def main():
	HOST_ADDR = "ws://192.168.3.50:9001/" #アドレス設定
	ws_client = WebsocketClient(HOST_ADDR)
	ws_client.run_forever()
	#test = [0,0,1,2,2,0,0,1]
	#test2 = ['A','H','C','D']
	#for i in range(10):
	#	optimize(test2)

def optimize(place):	
	positions = []
	OptRouteCIE = []
	OptRouteCIE1 = []
	OptRouteStr = ""

	result = str(place[0]).isdigit()

	if result == False:
		for i in range(len(place)):
			if place[i] == "A":
				positions.append((0,0))
			if place[i] == "B":
				positions.append((1,0))
			if place[i] == "C":
				positions.append((2,0))
			if place[i] == "D":
				positions.append((0,1))
			if place[i] == "E":
				positions.append((1,1))
			if place[i] == "F":
				positions.append((2,1))
			if place[i] == "G":
				positions.append((0,2))
			if place[i] == "H":
				positions.append((1,2))
			if place[i] == "I":
				positions.append((2,2))
	else:
		for i in range(0,len(place),2):
			positions.append((place[i],place[i+1]))
			print(place[i],place[i+1])

	print('入力座標:',positions)

	'''
	#下げ膳のサンプル問題
	positions = [(0,0),	#0 a
		  (0,5),	#1 b
		  (0,10),   #2 c
		  (0,15),   #3 d
		  (5,15),   #4 e
		  (10,15),  #5 f
		  (15,15),  #6 g
		  (20,15),  #7 h
		  (20,10),  #8 i
		  (20,5),   #9 j
		  (7.5,10), #10 k
		  (7.5,5),  #11 l
		  (12.5,10), #12 m  
		  (12.5,5), #13 n
		  ]
'''

	# 座標からフィールドのcsvファイルを作成
	create_dist_matrix(positions, 'field.csv')


	field = Field('field.csv') # フィールドのファイル読み込み
	colony = Colony(field)
	ant = [Ant(field) for i in range(ANT_NUM)]


	'''
	#フェロモン初期化確認
	print('最初の各枝のフェロモン')
	colony.printPheromone(field)
	print()
	'''

	for i in range(REPEAT_NUM):
		colony.selectRoute(field, ant)
		colony.renewPheromone(field, ant)
	
	OptRoute = Ant.minList[-1]
	for i in range(len(OptRoute)):
		OptRouteCIE.append(positions[OptRoute[i]])

	for i in range(len(OptRouteCIE)):
		OptRouteCIE1.append(OptRouteCIE[i][0])
		OptRouteCIE1.append(OptRouteCIE[i][1])

	OptRouteStr = ','.join(map(str,OptRouteCIE1))

	'''
	#各ステップのノードのフェロモン量を表示
	#	print(str(i)+'ステップ目の各ノード間のフェロモン')
	#	colony.printPheromone(field)
	#	print()
	'''

	#入力
	print('入力:',place)

	'''
	#入力リストの順番の座標
	print('入力順席座標:',positions)

	#最短経路長を表示
	print('最短経路長:',min(Ant.minDis))
	
	#最短経路更新リストの最後を取り出し表示
	print('最短経路:',OptRoute)
	'''

	#最適化後の座標リスト
	print('最適化座標:',OptRouteCIE)

	#最適化の座標リストを1次元に直したもの
	print('出力:',OptRouteCIE1)

	#return OptRouteCIE1

	#最適化の座標リストを1次元に直したものを文字列化したもの(出力)
	print('送信:',OptRouteStr)

	del colony
	del field
	del ant
	gc.collect()

	return OptRouteStr

	'''
	#ノードのフェロモン量を表示
	print('最終的な各枝のフェロモン')
	colony.printPheromone(field)

	'''
	'''
	# グラフの描画(フェロモン量)
	G = nx.Graph()
	pos = {}
	for i in range(len(positions)):
		G.add_node(i)
	pos[i] = (positions[i][0], positions[i][1])
	
	edges=[]   
	for i in range(len(positions)):
		for j in range(len(positionspositions)):
			if i>=j:continue
			else:
				G.add_edge(i,j)
				edges.append(field.pheromone[i][j]*0.3)
	nx.draw(G, pos,width=edges)
	'''

	'''
	入力するリスト = place
	出力リスト = OptRouteCIE1

	'''

if __name__ == "__main__":
	main()