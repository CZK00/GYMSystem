//#define CAMERA_MODEL_WROVER_KIT
#include <WiFi.h>
#include "Arduino.h"
#include "esp_camera.h"


const char *ssid = "hello world";//wifi id
const char *password = "jhy429520";//密码x
bool workstate;
const IPAddress serverIP(192,168,25,162); //欲访问的地址
uint16_t serverPort = 8888;         //服务器端口号

WiFiClient client; //声明一个客户端对象，用于与服务器进行连接

#define KEY_PIN       15
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

static camera_config_t camera_config = {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sscb_sda = SIOD_GPIO_NUM,
    .pin_sscb_scl = SIOC_GPIO_NUM,
    
    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,
    
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    
    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_VGA,
    .jpeg_quality = 15,
    .fb_count = 1,
};
/*
void gpio_init()
{
  pinMode(2, INPUT);
  
  // 检测到引脚 2 下降沿，触发中断函数 blink
  attachInterrupt(2, blink,CHANGE);


}

// 中断函数
void blink()
{
  Serial.println("IRQ");
}
*/
esp_err_t camera_init() {
    //initialize the camera
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        Serial.print("Camera Init Failed");
        return err;
    }
    sensor_t * s = esp_camera_sensor_get();
    //initial sensors are flipped vertically and colors are a bit saturated
    if (s->id.PID == OV2640_PID) {
    //        s->set_vflip(s, 1);//flip it back
    //        s->set_brightness(s, 1);//up the blightness just a bit
    //        s->set_contrast(s, 1);
    }
    
    Serial.print("Camera Init OK");
    return ESP_OK;
}


void sendImage()
{
     camera_fb_t * fb = esp_camera_fb_get();
     int x = int(fb->len);//将图片的大小存入x
     char s[10];
     snprintf(s, sizeof(s), "%d", x);//int转为char             
     client.write(s);  //传输char[100]大小的数据            
     client.flush();//清空缓存                
     while(!client.available());//等待服务器第二次回应
     client.write(fb->buf, fb->len);//传输jpeg编码数据     
     esp_camera_fb_return(fb);
     client.flush();
}

void faceIdentify()
{
     workstate = true;
     int i = 0;
     sendImage();
     i++;
     while(i<=15)
     {
     if(client.available())//等待服务器第一次回应
            {
                sendImage();
                i++;
            }  
     }
     Serial.println("over");
     workstate = false;
}


void setup()
{
    Serial.begin(115200);
    Serial.println();
    

    camera_init(); // 摄像头初始化  
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

      Serial.println("尝试访问服务器");
      if (!client.connect(serverIP, serverPort)) //尝试访问目标地址
      {
          return;
      }
     Serial.println("访问成功");

    //gpio14外部中断下降沿触发
    workstate = false;
    pinMode(14, INPUT);
    //attachInterrupt(14, blink, CHANGE);
    
    //faceIdentify();
    
}

void loop()
{   
    
    if(digitalRead(14) && !workstate)
    {
      Serial.println("customer come");
      faceIdentify();
    }
}

/* 中断函数
void blink()
{   
   if(!workstate)
   {
   Serial.println("customer come");
   faceIdentify();
   }
}
*/
