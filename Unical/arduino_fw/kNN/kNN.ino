#include <Adafruit_SSD1306.h>
#include <Adafruit_BNO055.h>
#include "KNN.h"

#define I2C_SDA 4
#define I2C_SCL 5

#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  

#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float MIMU_data[1][9];

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

Adafruit_BNO055 bno = Adafruit_BNO055(55);

KNeighborsClassifier knn;

void initIMU() {
  if (!bno.begin()) {
    Serial.println(F("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!"));
    while(1);
  }
  Serial.println(F("BNO055 initialized succesfully!"));
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

void displayLogo_OLED() {
  Serial.println(F("Display University logo"));
  display.clearDisplay();
  display.drawBitmap((display.width() - LOGO_WIDTH) / 2, (display.height() - LOGO_HEIGHT) / 2, bmp_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void readACC() {
  imu::Vector<3> accellerometer = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

  MIMU_data[0][0] = accellerometer.x();
  MIMU_data[0][1] = accellerometer.y();
  MIMU_data[0][2] = accellerometer.z();
}

void readGYR() {
  imu::Vector<3> gyroscope = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

  MIMU_data[0][3] = gyroscope.x();
  MIMU_data[0][4] = gyroscope.y();
  MIMU_data[0][5] = gyroscope.z();
}

void readMAG() {
  imu::Vector<3> magnetometer = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  MIMU_data[0][6] = magnetometer.x();
  MIMU_data[0][7] = magnetometer.y();
  MIMU_data[0][8] = magnetometer.z();
}

void initKNN() {
  Serial.println(F("KNN model initialized."));

  knn.fit(X_train, y_train, n_samples, n_features);
  Serial.println(F("Model fitted with training data."));
}

void setup() {
  Serial.begin(115200);
  watermark();

  Wire.begin(I2C_SDA, I2C_SCL);

  initOLED();
  displayLogo_OLED();

  initKNN();

  initIMU();
}

void loop() {
  readACC();
  readGYR();
  readMAG();

  unsigned long t_start_calc = micros();
  int* predictions = knn.predict(MIMU_data, 1);
  unsigned long t_end_calc = micros();
  
  Serial.print(F("Predicted class: "));
  switch(predictions[0]) {
    case 2:
      Serial.println(F("Sit"));
       display.setCursor(1, 1);
      display.clearDisplay();
      display.print(F("Sit"));
      display.display();
    break;
    case 3:
      Serial.println(F("Stand"));
       display.setCursor(1, 1);
      display.clearDisplay();
      display.print(F("Stand"));
      display.display();
    break;
    case 4:
      Serial.println(F("Walk"));
       display.setCursor(1, 1);
      display.clearDisplay();
      display.print(F("Walk"));
      display.display();
    break;
    case 5:
      Serial.println(F("Run"));
       display.setCursor(1, 1);
      display.clearDisplay();
      display.print(F("Run"));
      display.display();
    break;
    default:
      Serial.println(F("Class not recognized!"));
       display.setCursor(1, 1);
      display.clearDisplay();
      display.print(F("NaN"));
      display.display();
  }
  Serial.print(F("Time taken by the prediction:  "));
  Serial.print(t_end_calc-t_start_calc); 
  Serial.println(F(" us"));

  delete[] predictions;

  delay(100);
}
