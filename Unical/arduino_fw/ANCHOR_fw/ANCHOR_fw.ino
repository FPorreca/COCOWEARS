//##################################################
//#                                                #
//#            Research Fellow project             #
//#                   COCOWEARS                    #
//#              developed by: F.P.                #
//#                                                #
//#             University of Calabria             #
//#                                                #
//##################################################

#include <SPI.h>
#include "DW1000Ranging.h"

#define ANCHOR_ADD "85:17:5B:D5:A9:9A:E2:9C"

uint16_t adelay = 16629;

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define SPI_CS 4
#define PIN_RST 27 
#define PIN_IRQ 34
#define PIN_WAKEUP 32
#define PIN_EXTON 33

void initDW1000() {
    Serial.println("DW1000 inizialization...");
	
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    DW1000Ranging.initCommunication(PIN_RST, SPI_CS, PIN_IRQ); //Reset, CS, IRQ pin
    
    DW1000.setAntennaDelay(adelay);
    Serial.print("Antenna delay ");
    Serial.println(adelay);
    
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachBlinkDevice(newBlink);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);
    //Enable the filter to smooth the distance
    //DW1000Ranging.useRangeFilter(true);

    DW1000Ranging.startAsAnchor(ANCHOR_ADD, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);  

    Serial.println("DW1000 inizialized succesfully");
}

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    watermark();

    initDW1000();
}

void loop()
{
    DW1000Ranging.loop();
}

void newRange()
{
    Serial.print("from: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
    Serial.print("\t Range: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRange());
    Serial.print(" m");
    Serial.print("\t RX power: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
    Serial.println(" dBm");
}

void newBlink(DW1000Device *device)
{
    Serial.print("Received BLINK; Device added -> (short) ");
    Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device *device)
{
    Serial.print("Device removed -> (short) ");
    Serial.println(device->getShortAddress(), HEX);
}
