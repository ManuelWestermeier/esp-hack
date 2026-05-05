#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define CE 4
#define CSN 5

RF24 radio(CE, CSN);

const byte addr[6] = "NODE1";

void setup()
{
  Serial.begin(115200);

  radio.begin();
  radio.setChannel(108);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(addr);
  radio.stopListening();

  Serial.println("RF24 ready");
}

void loop()
{
  const char text[] = "HELLO";

  bool ok = radio.write(&text, sizeof(text));

  Serial.println(ok ? "sent" : "fail");

  delay(1000);
}