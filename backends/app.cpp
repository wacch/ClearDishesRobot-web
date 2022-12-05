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

extern void onpush(MQTT::MessageData& md);
Serial pc(USBTX, USBRX);
Zumo zumo;

static int speed = 80;
static int steer = 0;

void onpush(MQTT::MessageData& md)
{
	pc.printf("onpush\n\r");
    MQTT::Message &message = md.message;
    DataElement de = DataElement((char*)message.payload);
	int v = de.getInt("z");
}

void ledStatus(int r, int g, int b)
{
	led_r = r;
	led_g = g;
	led_b = b;
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


void task_main(intptr_t exinf) {

	int ret;

	pc.baud(115200);
	pc.printf("\r\ninitializing\r\n");

	//networkの初期化
	WiFi_init();
	connect_tcp_session();

	//文字列を送信する
	char* data = strdup("Connection Success\r\n");
	for (int i=0; i<20; ++i){
		ret = socket.send(data, strlen(data)); //文字列,文字数
		if(ret<0){
			pc.printf("socket send error\r\n");
			return;
		}
		else{
			pc.printf("socket send %d byte\r\n", ret);
		}
	}
	dly_tsk(5000); //サーバが受信し終えるのを待つ

	bool flg = true;
	char rstr[10];
	while (1) {
		ledStatus(0,1,1);
		zumo.driveTank(0,0);
		ret = socket.receive(rstr, 1);
		if(ret >= 0){
			ledStatus(1,0,1);
			pc.printf("rcv command = %s\r\n",rstr);
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
	}
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
		/*error */
		ledStatus(1,0,0);
		while(1){
			Thread::wait(1000);
		}
	}
}
