/*
 * BNO055.h
 *
 *  Created on: 18 lug 2024
 *      
 */

#include "BNO055.h"

BNO055::BNO055(u16 IIC_DEVICE_ID, u16 SCLK_RATE, u8 IIC_ADDR) {
  i2c_dev = new I2CDevice(IIC_DEVICE_ID, SCLK_RATE, IIC_ADDR);
}

bool BNO055::begin(bno055_opmode_t mode) {
	i2c_dev->begin();

	
	u8 id = read8(BNO055_CHIP_ID_ADDR);
	if (id != BNO055_ID) {
		usleep(1000); 
		id = read8(BNO055_CHIP_ID_ADDR);
			if (id != BNO055_ID) {
				return false; 
			}
	}

	setMode(OPERATION_MODE_CONFIG);

	
	write8(BNO055_SYS_TRIGGER_ADDR, 0x20);
	
	usleep(30);

	while (read8(BNO055_CHIP_ID_ADDR) != BNO055_ID) {
		usleep(10);
	}
	usleep(50);

	
	write8(BNO055_PWR_MODE_ADDR, POWER_MODE_NORMAL);
	usleep(10);

	write8(BNO055_PAGE_ID_ADDR, 0);

	write8(BNO055_SYS_TRIGGER_ADDR, 0x0);
	usleep(10);
	
	setMode(mode);
	usleep(20);

	return true;
}

u8 BNO055::getTemp() {
  u8 temp = read8(BNO055_TEMP_ADDR);
  return temp;
}

imu::Vector<3> BNO055::getVector(vector_type_t vector_type) {
  imu::Vector<3> xyz;
  uint8_t buffer[6];
  memset(buffer, 0, 6);

  int16_t x, y, z;
  x = y = z = 0;

  
  readLen((bno055_reg_t)vector_type, buffer, 6);

  x = ((int16_t)buffer[0]) | (((int16_t)buffer[1]) << 8);
  y = ((int16_t)buffer[2]) | (((int16_t)buffer[3]) << 8);
  z = ((int16_t)buffer[4]) | (((int16_t)buffer[5]) << 8);

  
  switch (vector_type) {
  case VECTOR_MAGNETOMETER:
    
    xyz[0] = ((double)x) / 16.0;
    xyz[1] = ((double)y) / 16.0;
    xyz[2] = ((double)z) / 16.0;
    break;
  case VECTOR_GYROSCOPE:
    
    xyz[0] = ((double)x) / 16.0;
    xyz[1] = ((double)y) / 16.0;
    xyz[2] = ((double)z) / 16.0;
    break;
  case VECTOR_EULER:
    
    xyz[0] = ((double)x) / 16.0;
    xyz[1] = ((double)y) / 16.0;
    xyz[2] = ((double)z) / 16.0;
    break;
  case VECTOR_ACCELEROMETER:
    
    xyz[0] = ((double)x) / 100.0;
    xyz[1] = ((double)y) / 100.0;
    xyz[2] = ((double)z) / 100.0;
    break;
  case VECTOR_LINEARACCEL:
    
    xyz[0] = ((double)x) / 100.0;
    xyz[1] = ((double)y) / 100.0;
    xyz[2] = ((double)z) / 100.0;
    break;
  case VECTOR_GRAVITY:
    
    xyz[0] = ((double)x) / 100.0;
    xyz[1] = ((double)y) / 100.0;
    xyz[2] = ((double)z) / 100.0;
    break;
  }

  return xyz;
}

void BNO055::setMode(bno055_opmode_t mode) {
  _mode = mode;
  write8(BNO055_OPR_MODE_ADDR, _mode);
  usleep(30);
}

bno055_opmode_t BNO055::getMode() {
  return (bno055_opmode_t)read8(BNO055_OPR_MODE_ADDR);
}

u8 BNO055::getCHIP_ID() {
  //xil_printf("getCHIP_ID()\r\n");
  return read8(BNO055_CHIP_ID_ADDR);
}


int BNO055::write8(bno055_reg_t reg, uint8_t value) {
	uint8_t buffer[2] = {(uint8_t)reg, (uint8_t)value};
	return i2c_dev->write(buffer, 2);
}


uint8_t BNO055::read8(bno055_reg_t reg) {
	uint8_t buffer[1] = {reg};
	i2c_dev->write_then_read(buffer, 1, buffer, 1);
	return (uint8_t)buffer[0];
}

int BNO055::readLen(bno055_reg_t reg, uint8_t *buffer, uint8_t len) {
	uint8_t reg_buf[1] = {(uint8_t)reg};
	return i2c_dev->write_then_read(reg_buf, 1, buffer, len);
}

int BNO055::bno055_read_accel_x(s16 *accel_x_s16) {
	
	u8 data_u8[BNO055_ACCEL_DATA_SIZE] = { BNO055_INIT_VALUE, BNO055_INIT_VALUE };

	
	readLen(BNO055_ACCEL_DATA_X_LSB_VALUEX_REG, data_u8, BNO055_LSB_MSB_READ_LENGTH);

	data_u8[BNO055_SENSOR_DATA_LSB] = BNO055_GET_BITSLICE(data_u8[BNO055_SENSOR_DATA_LSB],
														  BNO055_ACCEL_DATA_X_LSB_VALUEX);
	data_u8[BNO055_SENSOR_DATA_MSB] = BNO055_GET_BITSLICE(data_u8[BNO055_SENSOR_DATA_MSB],
														  BNO055_ACCEL_DATA_X_MSB_VALUEX);
	*accel_x_s16 =
		(s16)((((s32)(s8)(data_u8[BNO055_SENSOR_DATA_MSB])) << (BNO055_SHIFT_EIGHT_BITS)) |
			  (data_u8[BNO055_SENSOR_DATA_LSB]));

    return XST_SUCCESS;
}
