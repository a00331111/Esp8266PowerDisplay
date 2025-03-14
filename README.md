# Esp8266PowerDisplay
通过配置ESP8266实时显示Homeassistant中的设备功率信息  
家中有一台nas，因为功率焦虑所以买了一个米家的插座，接入了米家和homeassistant。每次想查看实时功率的时候都要打开homeassistant查看很不方便，所以使用手头的ESP8266核显示模块做了一个这个小东西，用着还挺方便，记录一下过程，帮助来者。  
  
一、硬件需求：  
1、ESP8266 nanoMCU（我使用的是ESP8266 12E nanoMCU）  
2、0.96寸IIC通信4针OLED 128X64液晶屏模块（如果家里没有电烙铁，去买焊接好排针的款式）  
3、杜邦线四根（母-母）  
4、数据线  
5、如果可以的话，弄一个薄亚克力板和一些螺丝（M2*4  M3*4）去固定一下更好看  
最终成本核算：ESP8266nanoMCU 8.35元+数据线1.2元+OLED液晶屏7.1元（我的是未焊接的，自己焊的排针）+杜邦线40根1.65元+螺丝0.2元+亚克力板0.05元+邮费4元=22.55元  
  
二、成品展示：  
![7c3895d267c1ea78d89c2e8c38724d4](https://github.com/user-attachments/assets/26852364-e7f5-457a-bb5f-1fa7784d2c1a)  
  
三、软件需求：  
0、家中有homeassistant以及接入了homeassistant的能显示功率的设备。  
1、需要使用Arduino IDE  
2、在Arduino中下载ESP8266开发板模块（不多讲了网上教程多得是）  
3、在Arduino中下载如下库：ESP8266WiFi、ESP8266HTTPClient、Adafruit_GFX、Adafruit_SSD1306  
4、导入本项目中的Esp8266PowerDisplay.ino文件  
5、修改Esp8266PowerDisplay.ino中的部分参数  
  
四、homeassistant设置  
1、确保家中已搭建homeassistant（如果没有的话要么搭一个要么不要弄了）  
2、申请长期访问令牌（token）并记录好  位置：左下角id——安全——长期访问令牌-创建令牌  
3、确定设备id并记录好  位置：设置——设备与服务——实体——找到你要展示功率的实体并点击——右上角设置——实体标识符  
一会要用到的参数：homeassistant的ip+端口、长期访问令牌、实体标识符  
  
五、接线教程：  
显示屏                    ESP8266  
VCC———————————————————————3V3  
GND———————————————————————GND  
SCL———————————————————————D4  
SDA———————————————————————D3  
使用4P杜邦线按如上连接，固定好后可以开始修改Esp8266PowerDisplay.ino文件前的信息部分  
  
六、程序修改教程：  
1、// OLED显示屏设置  
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)  
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);  
这里一般来说默认即可，如果不显示信息，询问卖家IIC地址是否为0x3C,如为其他值，需修改下方初始化显示屏的if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))中0x3C为其他值  
  
2、// WiFi 设置  
const char* ssid = "SSID"; // 替换为您的WiFi名称  
const char* password = "PASSWORD"; // 替换为您的WiFi密码  
这里按照要求填写你家WIFI名称和密码即可  
  
3、// Home Assistant 设置  
const char* haServer = "192.168.0.IP"; // 替换为您的Home Assistant服务器IP  
const int haPort = 8123; // 替换为您的Home Assistant服务器端口  
const char* sensorUrl = "/api/states/sensor.XXXXXXX"; // 替换为您的传感器URL  
const char* haToken = "TOKENTOKENTOKENTOKENTOKENTOKENTOKENTOKEN"; // 替换为您的长令牌  
这里按照要求填写HA的信息。其中ip部分不要加http/https等无用信息，后面也不要加'/'。  
  
4、// 更新间隔  
在程序最后，有这样一行：  
delay(5000); // 更新间隔，这里设置为5秒  
此处设置为5秒，不要设置太快，一般5-30秒即可，单位是ms，即5000-30000.  
  
修改完成之后，即可将ESP8266连接电脑，点击Arduino中的上传，静等上传完成即可显示。  
