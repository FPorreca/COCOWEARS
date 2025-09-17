//##################################################
//#                                                #
//#            Research Fellow project             #
//#                   COCOWEARS                    #
//#              developed by: F.P.                #
//#                                                #
//#             University of Calabria             #
//#                                                #
//##################################################

#include <ESP32SPISlave.h>     
#include <SPI.h>               
#include "DW1000Ranging.h"     
#include <Wire.h>              
#include <Adafruit_SSD1306.h>  

#define TAG_ADDR "13:00:22:EA:82:60:3B:9C"  

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define SPI_CS 21  
#define PIN_RST 27
#define PIN_IRQ 34
#define PIN_WAKEUP 32
#define PIN_EXTON 33


#define HSPI_SCK 14
#define HSPI_MISO 12
#define HSPI_MOSI 13
#define HSPI_CS 2

#define I2C_SDA 4
#define I2C_SCL 5

#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  

#define OLED_RESET -1  
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

ESP32SPISlave slave;

#define FIXED_POINT_FRACTIONAL_BITS 4

typedef uint8_t fixed_point_t;
inline fixed_point_t double_to_fixed(double input) {
  return (fixed_point_t)(round(input * (1 << FIXED_POINT_FRACTIONAL_BITS)));
}


void initOLED() {
  Serial.println("Inizializing OLED display...");
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Display init failed");
    
    while (1)
      yield();  
  }
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setRotation(0);
  display.clearDisplay();

  logoshow();

  Serial.println("OLED display inizialized succesfully");
}

struct Anchor {
  String addr;
  float x;
  float y;
};



struct Anchor AN1 = { "84:17:5B:D5:A9:9A:E2:9C", 0, 0 };       
struct Anchor AN2 = { "86:17:5B:D5:A9:9A:E2:9C", 6, 0 };       
struct Anchor AN3 = { "85:17:5B:D5:A9:9A:E2:9C", 3.3, 5.44 };  

static constexpr size_t BUFFER_SIZE = 12;  
static constexpr size_t QUEUE_SIZE = 1;
uint8_t tx_buf[BUFFER_SIZE];
uint8_t rx_buf[BUFFER_SIZE];

struct SPI_MSG_DIST {
  uint8_t anch_1_x;
  uint8_t anch_1_y;

  uint8_t dist_1;

  uint8_t anch_2_x;
  uint8_t anch_2_y;

  uint8_t dist_2;

  uint8_t anch_3_x;
  uint8_t anch_3_y;

  uint8_t dist_3;

  uint8_t anch_4_x;
  uint8_t anch_4_y;

  uint8_t dist_4;
};


struct SPI_MSG_DIST SPI_Dist_to_send = {
  double_to_fixed(AN1.x),
  double_to_fixed(AN1.y),
  double_to_fixed(0),
  double_to_fixed(AN2.x),
  double_to_fixed(AN2.y),
  double_to_fixed(0),
  double_to_fixed(AN3.x),
  double_to_fixed(AN3.y),
  double_to_fixed(0),
  double_to_fixed(0.0),
  double_to_fixed(0.0),
  double_to_fixed(0.0)
};

double round2(double value) {
  return (int)(value * 100 + 0.5) / 100.0;
}

void initDW1000() {
  Serial.println("DW1000 inizialization...");
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  DW1000Ranging.initCommunication(PIN_RST, SPI_CS, PIN_IRQ);

  DW1000Ranging.attachNewRange(newRange);
  DW1000Ranging.attachNewDevice(newDevice);
  DW1000Ranging.attachInactiveDevice(inactiveDevice);

  DW1000Ranging.startAsTag(TAG_ADDR, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
  Serial.println("DW1000 inizialized succesfully");
}

void initSlaveSPI() {
  slave.setDataMode(SPI_MODE0);    
  slave.setQueueSize(QUEUE_SIZE);  

  pinMode(HSPI_SCK, INPUT_PULLUP);
  pinMode(HSPI_MOSI, INPUT_PULLUP);
  pinMode(HSPI_CS, INPUT_PULLUP);

  
  slave.begin(HSPI, HSPI_SCK, HSPI_MISO, HSPI_MOSI, HSPI_CS);  
  Serial.println("Start HSPI slave");

  memset(rx_buf, 0, BUFFER_SIZE);
  memset(tx_buf, 0, BUFFER_SIZE);
}

void setup() {
  Serial.begin(115200);

  watermark();

  initOLED();

  initDW1000();

  initSlaveSPI();
}

void loop() {
  DW1000Ranging.loop();

  if (slave.hasTransactionsCompletedAndAllResultsHandled()) {
    
    Serial.println("execute transaction in the background");
    Serial.println("Send new transaction");
    slave.queue(tx_buf, rx_buf, BUFFER_SIZE);
    slave.trigger();

    Serial.println("wait for the completion of the queued transactions...");
  }

  if (slave.hasTransactionsCompletedAndAllResultsReady(QUEUE_SIZE)) {
    Serial.println("All queued transactions completed. Update data to send");
    size_t received_bytes = slave.numBytesReceived();
    Serial.print("Received bytes: ");
    Serial.println(received_bytes);
    
    tx_buf[0] = SPI_Dist_to_send.anch_1_x;
    tx_buf[1] = SPI_Dist_to_send.anch_1_y;
    tx_buf[2] = SPI_Dist_to_send.dist_1;
    tx_buf[3] = SPI_Dist_to_send.anch_2_x;
    tx_buf[4] = SPI_Dist_to_send.anch_2_y;
    tx_buf[5] = SPI_Dist_to_send.dist_2;
    tx_buf[6] = SPI_Dist_to_send.anch_3_x;
    tx_buf[7] = SPI_Dist_to_send.anch_3_y;
    tx_buf[8] = SPI_Dist_to_send.dist_3;
    tx_buf[9] = 0;
    tx_buf[10] = 0;
    tx_buf[11] = 0;
  }
}

void newRange() {
  
  int anchorShortAddress = DW1000Ranging.getDistantDevice()->getShortAddress();
  
  float range = DW1000Ranging.getDistantDevice()->getRange();
  
  if (range >= 0) {
    switch (anchorShortAddress) {
      case 6020:  
        Serial.println("New range from 1784 [HEX] 6020 [DEC]");
        Serial.print("Range: ");
        Serial.println(range);
        SPI_Dist_to_send.anch_1_x = double_to_fixed(AN1.x);
        SPI_Dist_to_send.anch_1_y = double_to_fixed(AN1.y);
        SPI_Dist_to_send.dist_1 = double_to_fixed(range);
        break;
      case 6021:  
        Serial.println("New range from 1785 [HEX] 6021 [DEC]");
        Serial.print("Range: ");
        Serial.println(range);
        SPI_Dist_to_send.anch_3_x = double_to_fixed(AN3.x);
        SPI_Dist_to_send.anch_3_y = double_to_fixed(AN3.y);
        SPI_Dist_to_send.dist_3 = double_to_fixed(range);
        break;
      case 6022:  
        Serial.println("New range from 1786 [HEX] 6022 [DEC]");
        Serial.print("Range: ");
        Serial.println(range);
        SPI_Dist_to_send.anch_2_x = double_to_fixed(AN2.x);
        SPI_Dist_to_send.anch_2_y = double_to_fixed(AN2.y);
        SPI_Dist_to_send.dist_2 = double_to_fixed(range);
        break;
      default:
        Serial.println("ERROR!");
        break;
    }
  } else {
    Serial.println("Range discarded: negative");
  }
}

void newDevice(DW1000Device* device) {
  Serial.print("ranging init; 1 device added ! -> ");
  Serial.print(" short:");
  Serial.println(device->getShortAddress());
}

void inactiveDevice(DW1000Device* device) {
  Serial.print("delete inactive device: ");
  Serial.println(device->getShortAddress());
}

void logoshow(void) {
  display.clearDisplay();

  display.setTextSize(2);               
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(0, 0);              
  display.println(F("COCOWEARS"));

  display.setTextSize(1);
  display.setCursor(0, 20);  
  display.println(F("TAG"));
  display.display();
}
