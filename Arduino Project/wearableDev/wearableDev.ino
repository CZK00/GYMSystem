#include <WiFi.h>
#include "AsyncUDP.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>




#pragma pack(1)
struct state{
    int order;           //指令
    int count;           //序号
    char na[10];         //姓名
    int face_ID;         //身份ID
    char deadline[20];   //套餐截至时间
    int surplusnum;      //套餐剩余次数
    int heartbeats;      //心率
    int place;           //位置
    float temperature;   //体温
    uint64_t chipid;     //芯片编号
};
#pragma pack()


state local_packet;   //本地数据包
state recieve_packet; //接收数据包
const char *ssid = "hello world";//wifi id
const char *password = "jhy429520";//密码x
bool workstate;       //工作状态

const IPAddress serverIP(192,168,95,162); //欲访问的地址
uint16_t serverPort = 8888;         //服务器端口号

int scanTime = 5; //In seconds
BLEScan* pBLEScan;
bool infield  = false;

AsyncUDP udp;                      //异步udp既可以发送也可以接收

WiFiUDP udpRe;//声明UDP对象
uint16_t udp_port=1122;  //存储需要监听的端口号

void wifiInit()
{
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); //关闭STA模式下wifi休眠，提高响应速度
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());

    
}

void waitforWorking()
{
    udp.connect(serverIP, serverPort);

    udpRe.begin(udp_port);//启动UDP监听这个端口

    local_packet.chipid = ESP.getEfuseMac();  

    local_packet.order = 0;
    local_packet.count = 0;
    sendUDPpack();

    while(!udpRe.parsePacket());
    udpRe.read((char*)&recieve_packet,sizeof(state));
    local_packet.face_ID = recieve_packet.face_ID;
    strcpy(local_packet.na,recieve_packet.na);
    Serial.println(local_packet.face_ID);
    //Serial.printf("%s",local_packet.na);
    Serial.println("startworking");
}


void searchSportData()
{
    local_packet.order = 2;
    local_packet.count++;
    sendUDPpack();    
    
    while(!udpRe.parsePacket());
    udpRe.read((char*)&recieve_packet,sizeof(state));
    Serial.printf("face_ID:%d\n",recieve_packet.face_ID);
    Serial.printf("heartbeats:%d\n",recieve_packet.heartbeats);
    Serial.printf("temperature:%d\n",recieve_packet.temperature);
    Serial.printf("place:%d\n",recieve_packet.place);
    Serial.println("........");
}

void sendUDPpack()
{
  uint8_t *p = (uint8_t*)&local_packet;
  udp.write(p,sizeof(state));
}


/*
void onPacketCallBack(AsyncUDPPacket packet)
{
   state temp = *(state*)(char*)packet.data();
   Serial.println("ready");
   Serial.println(temp.face_ID);
}
*/

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      if(advertisedDevice.getName() == "helloworld")
      {
        if(advertisedDevice.getRSSI()>-70 && !infield)
        {
          infield = true;
          local_packet.order = 1;
          local_packet.heartbeats=10;
          local_packet.temperature=23;
          local_packet.place = 6;
          sendUDPpack();
          Serial.printf("device:%s is coming",advertisedDevice.getName().c_str());
          Serial.println(advertisedDevice.getRSSI());
        }
        else if(advertisedDevice.getRSSI()<-80 && infield)
        {
          infield = false;
          Serial.println("device leave");  
        }
      
      }
    }
};

void setup() {
  Serial.begin(115200);
  
  BLEDevice::init("");                                                        
  //ESP32初始化蓝牙设备
  pBLEScan = BLEDevice::getScan();                                            
  //获取BLEDevice下的scan功能
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());  
  //注册扫描回调
  pBLEScan->setActiveScan(true);                                              
  //配置主动扫描
  pBLEScan->setInterval(100);                                                 
  //扫描间隔
  pBLEScan->setWindow(99);                                                   
  //扫描窗口      

  
  wifiInit();

  //gpio14外部中断下降沿触发
  workstate = false;
  pinMode(14, INPUT);

  waitforWorking();
  
//    udp.read((char*)&recieve_packet,sizeof(state));
  
}

void loop() {
  
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  //Serial.print("Devices found: ");
  //Serial.println(foundDevices.getCount());
  //Serial.println("Scan done!");
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory

  if(digitalRead(14) && !workstate)
  {
      searchSportData();
      
  }
    
  delay(2000);
  
}
