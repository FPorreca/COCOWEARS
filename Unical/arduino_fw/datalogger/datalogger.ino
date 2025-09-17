//##################################################
//#                                                #
//#            Research Fellow project             #
//#                   COCOWEARS                    #
//#              developed by: F.P.                #
//#                                                #
//#             University of Calabria             #
//#                                                #
//##################################################

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Adafruit_BNO055.h>
#include <Adafruit_SSD1306.h>

#define I2C_SDA 4
#define I2C_SCL 5

#define SCLK 14
#define CS 2
#define MOSI 13
#define MISO 15

#define  BUTTON_SPACE 39
#define  BUTTON_START 36

Adafruit_BNO055 bno = Adafruit_BNO055(55);

#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  

#define OLED_RESET -1  
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int n_samples = 0;

const unsigned char bmp_logo[] PROGMEM = {

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xfe,

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xf0,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xe0,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xc0,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0x80,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xfe, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xfc, 0x00,

    0x00, 0x00, 0x00, 0x00, 0x07, 0xf8, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00,

    0xff, 0xff, 0xff, 0xff, 0xe7, 0xe0, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0xc0, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0x80, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe7, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe6, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xe4, 0x00, 0x00
};


#define LOGO_HEIGHT 40
#define LOGO_WIDTH 55

float ACC[3];
float MAG[3];
float GYR[3];
String dataMessage;

unsigned long lastTime = 0;
unsigned long sampleRate = 10; 

int debounceDelay = 20;

boolean start_aq = false;

boolean debounce(int pin) {
    boolean state;
    boolean previousState;
    previousState = digitalRead(pin);
    for (int counter = 0; counter < debounceDelay; counter++) {
        delay(1);
        state = digitalRead(pin);
        if ( state != previousState) {
            counter = 0;
            previousState = state;
        }
    }
    return state;
}

void readButton_START() {
    if (debounce(BUTTON_START) == HIGH) {
        
        while (debounce(BUTTON_START) == HIGH) {
            delay(5);
        }
        if (start_aq == true) {
            start_aq = false;
            display.setTextSize(2);
            display.setCursor(1, 1);
            display.clearDisplay();
            display.print(F("STOP"));
            display.setTextSize(1);
            display.setCursor(1, 20);
            display.print(n_samples);
            display.display();
            Serial.println("STOP");
        }
        else {
            start_aq = true;
        }
    }
}

void readButton_SPACE() {
    if (debounce(BUTTON_SPACE) == HIGH) {
        
        while (debounce(BUTTON_SPACE) == HIGH) {
            delay(5);
        }
        if (start_aq == false) {
            Serial.println("SPACE");
            display.setTextSize(2);
            display.setCursor(1, 1);
            display.clearDisplay();
            display.print(F("SPACE"));
            display.setTextSize(1);
            display.setCursor(1, 20);
            display.print(n_samples);
            display.display();
            appendFile(SD, "/LOG.txt", "\n\n\n\n\n");
        }
    }
}

void initIMU() {
    
    if (!bno.begin()) {
        Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        
        while (1);
    }
    Serial.println("BNO055 Initialized succesfully.");
    bno.setExtCrystalUse(true); 
}


void initOLED() {
    Serial.println(F("Inizializing OLED display..."));
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Display init failed"));
        
        while (1)
            yield();  
    }
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setRotation(0);
    display.setCursor(1, 1);
    Serial.println(F("OLED display inizialized succesfully"));
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path) {
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path)) {
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path) {
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path)) {
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path) {
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message)) {
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message)) {
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path) {
    Serial.printf("Deleting file: %s\n", path);
    if (fs.remove(path)) {
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path) {
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if (file) {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len) {
            size_t toRead = len;
            if (toRead > 512) {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++) {
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

void initSD() {
    SPI.begin(SCLK, MISO, MOSI, CS);
    if (!SD.begin(CS)) {
        Serial.println("Card Mount Failed");
        display.setTextSize(2);
        display.setCursor(1, 1);
        display.clearDisplay();
        display.print(F("ERROR!"));
        display.display();
        while (1); 
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        display.setTextSize(2);
        display.setCursor(1, 1);
        display.clearDisplay();
        display.print(F("NO SD CARD"));
        display.display();
        while (1); 
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void displayLogo_OLED() {
    Serial.println(F("Display University logo"));
    display.clearDisplay();
    display.drawBitmap((display.width() - LOGO_WIDTH) / 2, (display.height() - LOGO_HEIGHT) / 2, bmp_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
}

void setup() {
    Serial.begin(115200);
    watermark();

    Wire.begin(I2C_SDA, I2C_SCL);

    initOLED();
    displayLogo_OLED();

    delay(1000);

    initSD();

    initIMU();

    pinMode(BUTTON_START, INPUT);
    pinMode(BUTTON_SPACE, INPUT);

    
    
    File file = SD.open("/LOG.txt");
    if (!file) {
        Serial.println("File doesn't exist");
        Serial.println("Creating file...");
        writeFile(SD, "/LOG.txt", "ACC X, ACC Y, ACC Z, GYR X, GYR Y, GYR Z, MAG X, MAG Y, MAG Z,\r\n");
    }
    else {
        Serial.println("File already exists");
        writeFile(SD, "/LOG.txt", "\r\n");
    }

    file.close();
    display.setTextSize(2);
    display.setCursor(1, 1);
    display.clearDisplay();
    display.print(F("READY"));
    display.setTextSize(1);
    display.setCursor(1, 20);
    display.print(n_samples);
    display.display();
}

void loop() {
    readButton_START();
    readButton_SPACE();

    if (((millis() - lastTime) > sampleRate) && start_aq == true) {
        
        imu::Vector<3> accellerometer = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
        ACC[0] = accellerometer.x();
        ACC[1] = accellerometer.y();
        ACC[2] = accellerometer.z();

        imu::Vector<3> magnetometer = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
        MAG[0] = magnetometer.x();
        MAG[1] = magnetometer.y();
        MAG[2] = magnetometer.z();

        imu::Vector<3> gyroscope = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
        GYR[0] = gyroscope.x();
        GYR[1] = gyroscope.y();
        GYR[2] = gyroscope.z();

        dataMessage = String(ACC[0]) + "," + String(ACC[1]) + "," + String(ACC[2]) + "," +
                      String(GYR[0]) + "," + String(GYR[1]) + "," + String(GYR[2]) + "," +
                      String(MAG[0]) + "," + String(MAG[1]) + "," + String(MAG[2]) +
                      "\r\n";
        Serial.print("Saving data: ");
        Serial.println(dataMessage);
        display.setTextSize(2);
        display.setCursor(1, 1);
        display.clearDisplay();
        display.print(F("START"));
        display.setTextSize(1);
        display.setCursor(1, 20);
        display.print(n_samples);
        display.display();
        n_samples++;

        
        appendFile(SD, "/LOG.txt", dataMessage.c_str());

        lastTime = millis();
    }
}
