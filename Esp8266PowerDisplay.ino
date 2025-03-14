#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED显示屏设置
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

// WiFi 设置
const char* ssid = "SSID"; // 替换为您的WiFi名称
const char* password = "PASSWORD"; // 替换为您的WiFi密码

// Home Assistant 设置
const char* haServer = "192.168.0.IP"; // 替换为您的Home Assistant服务器IP
const int haPort = 8123; // 替换为您的Home Assistant服务器端口
const char* sensorUrl = "/api/states/sensor.XXXXXXX"; // 替换为您的传感器URL
const char* haToken = "TOKENTOKENTOKENTOKENTOKENTOKENTOKENTOKEN"; // 替换为您的长令牌

void setup() {
  // 初始化串口
  Serial.begin(115200);

  // 初始化I2C
  Wire.begin(D3, D4); // SDA接D3 (GPIO0), SCL接D4 (GPIO2)

  // 初始化显示屏
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 初始化显示屏，地址0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // 卡住，不继续执行
  }
  display.display(); // 显示缓冲内容
  display.clearDisplay(); // 清屏

  // 连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    String url = "http://" + String(haServer) + ":" + String(haPort) + sensorUrl;

    // 开始连接到服务器
    http.begin(client, url);

    // 设置HTTP头
    http.addHeader("Authorization", "Bearer " + String(haToken));
    http.addHeader("Content-Type", "application/json");

    // 发送HTTP GET请求
    int httpCode = http.GET();

    if (httpCode > 0) {
      // HTTP请求成功
      String payload = http.getString();
      Serial.println(payload);

      // 使用ArduinoJson解析JSON数据
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      // 提取状态信息
      String state = doc["state"];

      // 显示在OLED屏幕上
      display.clearDisplay();

      // 绘制边框
      display.drawRect(0, 2, 128, 64, SSD1306_WHITE); // 绘制一个矩形边框

      // 显示标题
      display.setTextSize(2); // 设置标题字体大小
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(15, 20); // 设置标题位置
      display.print("Power Now");

      // 显示功耗数值
      display.setTextSize(2); // 设置数值字体大小
      display.setCursor(30, 35); // 设置数值位置
      display.print(state);
      display.print(" W"); // 添加单位

      // 显示分隔线
      display.drawLine(0, 50, 128, 50, SSD1306_WHITE);

      // 显示刷新时间
      display.setTextSize(1);
      display.setCursor(10, 55); // 设置刷新时间位置
      display.print("Update Time: ");
      display.print(millis() / 1000); // 显示上次刷新时间（秒）
      display.print("s");

      display.display(); // 更新显示
    } else {
      Serial.println("HTTP GET request failed");
    }

    http.end(); // 关闭连接
  } else {
    Serial.println("WiFi not connected.");
  }

  delay(5000); // 更新间隔，这里设置为5秒
}