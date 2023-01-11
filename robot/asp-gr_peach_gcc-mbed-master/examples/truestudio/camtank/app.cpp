#include <kernel.h>
#include "kernel_cfg.h"
#include "app.h"
#include "mbed.h"
#include "app_config.h"
#include "GR_PEACH_WlanBP3595.h"
#include "Zumo.h"
#include "Milkcocoa.h"

#define SERVER_ADDRESS "192.168.3.50"
#define SERVER_PORT 9002

static void _wlan_inf_callback(uint8_t ucType, uint16_t usWid, uint16_t usSize, uint8_t *pucData);
static void error_wait(int ret, const char* str);

GR_PEACH_WlanBP3595 wlan;			// Wifiルータ用
TCPSocketConnection socket;			// TCPソケット変数

// LED
DigitalOut led_r(LED1);
DigitalOut led_g(LED2);
DigitalOut led_b(LED3);

Serial pc(USBTX, USBRX);
Zumo zumo;

static int speed = 80;
static int steer = 0;

void ledStatus(int r, int g, int b)
{
	led_r = r;
	led_g = g;
	led_b = b;
}

void ledBlink(int r, int g, int b,int count,int interval){
	for(int i=0;i<count;i++){
		ledStatus(r,g,b);
		Thread::wait(interval);
		ledStatus(0,0,0);
		Thread::wait(interval);
	}
}

// WiFi Routerとの接続
void WiFi_init() {
	int ret;

	wlan.setWlanCbFunction(_wlan_inf_callback);

	pc.printf("\r\ninitializing\r\n");
	ret = wlan.init(IP_ADDRESS, SUBNET_MASK, DEFAULT_GATEWAY);
	error_wait(ret, "init");	// if error, wait
	//緑二回点滅
	for(int i=0;i<2;i++){
		ledStatus(0,1,0);
		Thread::wait(500);
		ledStatus(0,0,0);
		Thread::wait(500);
	}
	pc.printf("My addr=%s\r\n",IP_ADDRESS);

	pc.printf("wlan connecting\r\n");
	ret = wlan.connect(WLAN_SSID, WLAN_PSK);
	error_wait(ret, "wifi connect error");	// if error, message&Red LED turn on
	pc.printf("wlan connectted SSID=%s\r\n",WLAN_SSID);		// if no error , print this message
	//青二回点滅
	for(int i=0;i<2;i++){
		ledStatus(0,0,1);
		Thread::wait(500);
		ledStatus(0,0,0);
		Thread::wait(500);
	}
	//　　wifi routerとの接続が成功するとZUMOの青色LEDが点灯する
}

//　TCP/IP通信のセッション確立処理
void connect_tcp_session() {
	int ret;

	ret = socket.connect(SERVER_ADDRESS, SERVER_PORT);
	error_wait(ret, "socket connect");	//　うまくいかないときは、赤のLEDランプが点灯する
	//青緑二回点滅
	for(int i=0;i<2;i++){
			ledStatus(0,1,1);
			dly_tsk(500);
			ledStatus(0,0,0);
			dly_tsk(500);
	}
	pc.printf("Socket connected\r\n");
	pc.printf("server IP&port= %s, %d\r\n",SERVER_ADDRESS, SERVER_PORT);	//　成功
}

void tuning_IR(int *threshold){
	char c = 0, WorB, drop = 30;
	unsigned int IR_values[6];//, IR_buf[6];

	for(char i=0;i<6;i++){
		threshold[i] = 0;
	}

	for(WorB=0;WorB<2;WorB++){
		if(!WorB){
			pc.printf("Tuning mode: white\r\n");
			ledBlink(0,1,0,5,500);
			ledStatus(0,1,0);
		}else{
			pc.printf("Tuning mode: black\r\n");
			ledBlink(0,0,1,5,500);
			ledStatus(0,0,1);
		}
		while(c<10){
			dly_tsk(500);
			zumo.readAnalogIrValue(IR_values);
			//決め打ちよりバッファを取って瞬低を検知したほうが確実。リソースが足りるかどうか（あとめんどい）
			if(IR_values[5] < drop || IR_values[3] < drop || IR_values[0] < drop){
				pc.printf("instantaneous drop detected\r\n");
				dly_tsk(1000);
				continue;
			}else{
				pc.printf("left:%d, center:%d, right:%d\r\n", IR_values[5], IR_values[3], IR_values[0]);
				for(char j=0;j<3;j++){
					//5-j*2-(j/2)...0のとき5,1のとき3,2のとき0が出てくる都合のいい数式(j/2はint)
					threshold[WorB*3+j] += IR_values[5-j*2-(j/2)];
					pc.printf("%d\r\n",threshold[WorB*3+j]);
				}
				c++;
			}
		}
		c=0;
	}
	ledStatus(0,0,0);

	for(char i=0;i<6;i++){
		threshold[i] /= 10;
	}
}

void task_main(intptr_t exinf) {
	pc.baud(115200);
	pc.printf("Initializing...\r\n");

	int ret;
	IrBitField_T irbits;
	unsigned int IR_values[6];
	int threshold[6];
	char position[2] = {0,0}, rotation = 0;
	char add_left = 0, add_right = 0;
	char position[2] = {0,0};
	char route[128];

	/*WiFi_init();
	connect_tcp_session();

	//認識メッセージを送信する
	char* data = strdup("route");
	ret = socket.send(data, strlen(data)); //文字列,文字数
	if(ret<0){
		pc.printf("MierBot/Recv: socket send error\r\n");
		return;
	}else{
		pc.printf("MierBot/Recv: socket send %d byte\r\n", ret);
	}
	dly_tsk(1000); //サーバが受信し終えるのを待つ

	while (1) {
		ledStatus(0,1,1);
		zumo.driveTank(0,0);
		ret = socket.receive(rstr, 128);
		if(ret >= 0){
			pc.printf("MierBot/Recv: arrived optimized route: %s\r\n",route);
			pc.printf("MierBot/Recv: socket close, wlan disconnect\r\n");
			//socket.close();
			wlan.disconnect();
			break;
		}else{
			ledStatus(1,0,0);
		}
	}*/

	while(1){
		zumo.driveTank(0,0);
		zumo.readAnalogIrValue(IR_values);
		zumo.readIr(irbits);

		pc.printf("left:%d %d, center:%d %d, right:%d %d\r\n", IR_values[5] ,irbits.left, IR_values[3], irbits.center, IR_values[0], irbits.right);
		if(IR_values[5] >= 150 && IR_values[3] >= 150 && IR_values[0] >= 150){
			dly_tsk(1000);
			zumo.driveTank(100,0);
			dly_tsk(900);
		}else if(IR_values[5] >= 150){
			zumo.driveTank(35, 50 + add_right);
			add_right += 2;
		}else if(IR_values[0] >= 150){
			zumo.driveTank(50 + add_left,35);
			add_left += 2;
		}else if(IR_values[5] <= 150 && IR_values[0] <= 150){
			zumo.driveTank(35,35);
			add_left = 0;
			add_right = 0;
		}
		/*if((IR_values[5] <= 150) && (IR_values[3] >= 150) && (IR_values[0] <= 150)){
			pc.printf("Drive\r\n");
			zumo.driveTank(50,50);
			position[0]++;
			dly_tsk(250);
		}else if((IR_values[5] >= 150) && (IR_values[3] >= 150) && (IR_values[0] >= 150)){
			pc.printf("Slow\r\n");
			zumo.driveTank(25,25);
			dly_tsk(250);
		}else if((IR_values[5] <= 150) && (IR_values[3] <= 150) && (IR_values[0] <= 150)){
			pc.printf("Stop\r\n");
			zumo.driveTank(0,0);
			dly_tsk(250);
		}*/
			dly_tsk(250);
	}
	/*dly_tsk(5000);
	tuning_IR(threshold);
	for(int i=0;i<6;i++){
		pc.printf("threshold[%d]:%d,\r\n",i,threshold[i]);
	*/


	//networkの初期化
	/*WiFi_init();
	connect_tcp_session();

	//文字列を送信する
	char* data = strdup("route");
	//for (int i=0; i<20; ++i){
	ret = socket.send(data, strlen(data)); //文字列,文字数
	if(ret<0){
		pc.printf("MierBot/Recv: socket send error\r\n");
		return;
	}else{
		pc.printf("MierBot/Recv: socket send %d byte\r\n", ret);
	}

	dly_tsk(1000); //サーバが受信し終えるのを待つ

	char rstr[128];
	while (1) {
		ledStatus(0,1,1);
		zumo.driveTank(0,0);
		ret = socket.receive(rstr, 128);
		if(ret >= 0){
			pc.printf("MierBot/Recv: arrived optimized route: %s\r\n",rstr);
			//pc.printf("MierBot/Recv: socket close, wlan disconnect\r\n");
			//socket.close();
			//wlan.disconnect();
			break;
		}else{
			ledStatus(1,0,0);
		}*/

		//実験用ラジコンコード
		/*if(ret >= 0){
			ledStatus(1,0,1);
			pc.printf("rcv command = %s\r\n",rstr[0]);
			switch (rstr[0]) {
			case 'w':
				zumo.driveTank(speed,speed);
				break;
			case 'a':
				zumo.driveTank(-speed*2,speed*2);
				break;
			case 's':
				zumo.driveTank(-speed,-speed);
				break;
			case 'd':
				zumo.driveTank(speed*2,-speed*2);
				break;
			case 'e':
				//ソケットクローズしたときにバックするバグがある...？
				pc.printf("socket close & disconnect\r\n");
				socket.close();
				wlan.disconnect();
				ledStatus(1,1,1);
				break;
			default:
				continue;
			}
			dly_tsk(500);
			ret = socket.send("comp", 4);
		}else{
			ledStatus(1,0,0);
		}
	}*/
}

static void _wlan_inf_callback( uint8_t ucType, uint16_t usWid, uint16_t usSize, uint8_t *pucData ){
	 if (ucType=='I'){
		 if(usWid ==0x0005){
			 if(pucData[0]==0x01){
                   ledStatus(0,1,0);
			 }else{
				   ledStatus(1,0,1);
			 }
		 }
	 }
}
static void error_wait(int ret, const char* str){
	if( ret !=0 ){
	   pc.printf(str);

		ledStatus(1,0,0);
		while(1){
			Thread::wait(1000);
		}
	}
}

