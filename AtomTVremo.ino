#include <M5Atom.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
//http://www.gatelink.co.jp/hw/etc/regzaremo/index.html

const uint16_t kIrLed = 12;

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.


#define power  0x02FD48B7UL
#define volup  0x02FD58A7UL
#define voldn  0x02FD7887UL
#define jimak  0xC23D4AB5UL
#define menu   0x02FD4AB5UL
#define modoru 0x02FDDC23UL
#define arrup  0x02FD7C83UL
#define arrdn  0x02FDFC03UL
#define arrrt  0x02FDDA25UL
#define arrlt  0x02FDFA05UL
#define chup   0x02FDD827UL
#define chdn   0x02FDF807UL


void setup() {
  M5.begin();
  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266
}

void loop() {
  if (M5.Btn.wasPressed())
  {
    Serial.printf("button was pressed...\n");
    Serial.println("NEC");
    irsend.sendNEC(jimak);
    delay(2000);
  }
  delay(50);
  M5.update();
    
}
