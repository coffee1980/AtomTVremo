#include <M5Atom.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

//https://shuzo-kino.hateblo.jp/entry/2021/02/22/230207
//http://www.gatelink.co.jp/hw/etc/regzaremo/index.html
#define CMD_POWER  0x02FD48B7UL
#define CMD_VOLUP  0x02FD58A7UL
#define CMD_VOLDN  0x02FD7887UL
#define CMD_VOLZ   0x02FD08F7UL
#define CMD_JIMAKU 0xC23D4AB5UL
#define CMD_BHYOU  0x02FD7689UL
#define CMD_ENTER  0x02FDBC43UL
#define CMD_RET    0x02FDDC23UL
#define CMD_ARRUP  0x02FD7C83UL
#define CMD_ARRDN  0x02FDFC03UL
#define CMD_ARRRT  0x02FDDA25UL
#define CMD_ARRLT  0x02FDFA05UL
#define CMD_CHUP   0x02FDD827UL
#define CMD_CHDN   0x02FDF807UL
#define CMD_BB     0x027D2CD3UL
#define CMD_PLAY   0x027DB44BUL
#define CMD_PAUSE  0x02FD0AF5UL

const char* ssid = "xxx";
const char* password = "xxx";

String htmltxt, htmltxt_root, htmltxt_core;

const uint16_t kIrLed = 12;
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "this is root page");
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


void setup() {

htmltxt_core = 
String("<p style=\"color:red; line-height:1.5; font-size: 5em; width:600px; margin:0 auto;\">") + 
String("<a href = \"/power\">power</a><br>") + 
String("</p>") + 

String("<p style=\"color:red; line-height:1.5; font-size: 5em; width:600px; margin:0 auto;\">") + 
String("<a href = \"/volumeup\">volume +</a><br>") + 
String("<a href = \"/volumedown\">volume -</a><br>") + 
String("<a href = \"/volumezero\">消音</a><br>") + 
String("</p>") + 

String("<p>") + 
String("<table style=\"table-layout: fixed; border-collapse: collapse; border: 2px solid green; width: 600px; font-size: 3em; margin:0 auto;\"><tr>") + 
String("<td></td><td><a href = \"/up\">↑</a></td><td></td>") + 
String("</tr><tr>") + 
String("<td><a href = \"/left\">←</a></td>") + 
String("<td><a href = \"/enter\">決定</a></td>") + 
String("<td><a href = \"/right\">→</a></td>") + 
String("</tr><tr>") + 
String("<td></td><td><a href = \"/down\">↓</a></td>") + 
String("<td><a href = \"/return\">戻る</a></td>") + 
String("</tr></table>") + 
String("</p>") + 

String("<p style=\"color:red; line-height:1.5; font-size: 5em; width:600px; margin:0 auto;\">") + 
String("<a href = \"/jimaku\">字幕</a><br>") + 
String("<a href = \"/bhyou\">番組表</a><br>") + 
String("<a href = \"/chup\">ch +</a><br>") + 
String("<a href = \"/chdown\">ch -</a><br>") + 
String("<a href = \"/bb\">みるコレ</a><br>") + 
String("<a href = \"/play\">再生</a><br>") + 
String("<a href = \"/pause\">一時停止</a><br>") + 
String("</p>");



htmltxt = 
String("<html><head><meta charset=\"utf-8\"><meta http-equiv=\"refresh\" content=\"0;url=/\"></head><body>") + 
htmltxt_core + 
String("</body></html>");

htmltxt_root = 
String("<html><head><meta charset=utf-8></head><body>") + 
htmltxt_core + 
String("</body></html>");
  
  M5.begin(true, false, true);
  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266

  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("m5atom")) {
    Serial.println("MDNS responder started");
  }

//  server.on("/", handleRoot);

  server.on("/", []() {
    server.send(200, "text/html", htmltxt_root);
  });

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/power", []() {
    irsend.sendNEC(CMD_POWER);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/volumeup", []() {
    irsend.sendNEC(CMD_VOLUP);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/volumedown", []() {
    irsend.sendNEC(CMD_VOLDN);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/volumezero", []() {
    irsend.sendNEC(CMD_VOLZ);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/up", []() {
    irsend.sendNEC(CMD_ARRUP);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/down", []() {
    irsend.sendNEC(CMD_ARRDN);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/right", []() {
    irsend.sendNEC(CMD_ARRRT);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });
  
  server.on("/left", []() {
    irsend.sendNEC(CMD_ARRLT);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/jimaku", []() {
    irsend.sendNEC(CMD_JIMAKU);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/bhyou", []() {
    irsend.sendNEC(CMD_BHYOU);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/return", []() {
    irsend.sendNEC(CMD_RET);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/enter", []() {
    irsend.sendNEC(CMD_ENTER);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/chup", []() {
    irsend.sendNEC(CMD_CHUP);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/chdown", []() {
    irsend.sendNEC(CMD_CHDN);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/bb", []() {
    irsend.sendNEC(CMD_BB);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });

  server.on("/play", []() {
    irsend.sendNEC(CMD_PLAY);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });
  server.on("/pause", []() {
    irsend.sendNEC(CMD_PAUSE);
    delay(100);
    server.send(200, "text/html", htmltxt);            
  });


  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  if (M5.Btn.wasPressed())
  {
    Serial.printf("button was pressed...\n");
    Serial.println("NEC");
    irsend.sendNEC(CMD_POWER);
    delay(2000);
  }
  delay(50);
  M5.update();
    
}
