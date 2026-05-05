#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#include "esp_bt.h"
#include "esp_wifi.h"

#define CE 4
#define CSN 5

RF24 radio(CE, CSN);

// Broadcast-Adresse (alle 0xFF)
const byte addr[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t bleChannels[] = {37, 38, 39};
uint8_t blueToothChannels[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                               10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
                               20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                               30, 31, 32, 33, 34, 35, 36, 37, 38, 39};

// 32 Byte Payload (max)
uint8_t payload[32] = "HELLO__________________________";

void setup()
{
  Serial.begin(115200);

  esp_bt_controller_deinit();
  esp_wifi_stop();
  esp_wifi_deinit();
  esp_wifi_disconnect();

  radio.begin();

  radio.setAutoAck(false); // keine ACKs
  radio.disableCRC();      // optional für maximale "raw" Übertragung
  radio.setRetries(0, 0);

  radio.setDataRate(RF24_2MBPS); // maximale Datenrate
  radio.setPALevel(RF24_PA_MAX, true);
  radio.setCRCLength(RF24_CRC_DISABLED); // optional für maximale "raw" Übertragung

  radio.setPayloadSize(32); // volle Framegröße

  // radio.startConstCarrier(RF24_PA_HIGH, blueToothChannels); // optional für Bluetooth-Emulation, siehe
  radio.openWritingPipe(addr);
  radio.stopListening();

  Serial.println("RF24 broadcast ready");
}

void loop()
{
  uint8_t ch = blueToothChannels[random(0, sizeof(blueToothChannels))];
  radio.setChannel(ch);
  radio.write(&payload, sizeof(payload));
  uint8_t chBLE = bleChannels[random(0, sizeof(bleChannels))];
  radio.setChannel(chBLE);
  radio.write(&payload, sizeof(payload));
}