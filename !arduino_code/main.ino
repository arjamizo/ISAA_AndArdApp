#include <Usb.h>
#include <AndroidAccessory.h>

#define LED          13
#define COMMAND_LED  0x1

AndroidAccessory acc("test",            // Manufacturer
                     "test",                 // Model
                     "test",   // Description
                     "1.0",                     // Version
                     "http://trolololo.com",  // URI
                     "0000000012345678");       // Serial
int buttonState = 0;

void setup()
{
  Serial.begin(115200);
  Serial.print("\r\nADK Started\r\n");

  pinMode(LED, OUTPUT);
 delay(500);
  // Power On Android Accessory interface and init USB controller
  acc.powerOn();
}
long lastTime;


void sendIfNecc()
{
  //Check if sensor should be sampled.
  if ((millis() - lastTime) > 200)
  {
    // light sensor
    uint16_t data[2];
    data[0] = 0xa2;
    data[1] = analogRead(A0);
    acc.write(data,2);

    // Output debugging to serial
    Serial.println(data[1],DEC);
    // Update timer for sensor check
    lastTime = millis();
  }

}

void loop()
{
  byte data[2];
  if (acc.isConnected()) {
    int len = acc.read(data, sizeof(data), 1);
    if (len > 0) {
      if (data[0] == 0x1) {
        digitalWrite(LED, data[1] ? HIGH : LOW);
        Serial.print("Toggle LED\r\n");
        data[0] = 0x2;
        acc.write(data, 2);
      }
    }
    sendIfNecc();
  }

  delay(10);
}