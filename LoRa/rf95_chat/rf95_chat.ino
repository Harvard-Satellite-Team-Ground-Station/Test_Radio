// rf95_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_client
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with
// the RFM95W, Adafruit Feather M0 with RFM95

#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
//RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
//RH_RF95 rf95(8, 3); // Adafruit Feather M0 with RFM95 

// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init())
    Serial.println("init failed");

  rf95.setFrequency(915.0);
}

void loop()
{
  if(rf95.available()) {
    // Should be a message for us now   
    uint8_t rxbuf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(rxbuf);
    if(rf95.recv(rxbuf, &len)) {
      Serial.println((char*)rxbuf);
    }
  }

  uint8_t counter = 0;
  
  while(Serial.available()) {
    uint8_t txbuf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(txbuf);

    txbuf[counter] = Serial.read();

    if(txbuf[counter] == '\n') {
      txbuf[counter + 1] = '\0';
      Serial.println((char*)txbuf);
      
      rf95.send(txbuf, len);
      rf95.waitPacketSent();
    }
    counter++;
  }
}
