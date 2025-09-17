/*
 * I2C_utility.h
 *
 *  Created on: 18 lug 2024
 *      
 */

#include <stdio.h>
#include "xil_printf.h"
#include "xiicps.h" 

#ifndef SRC_I2C_UTILITY_H_
#define SRC_I2C_UTILITY_H_

class I2CDevice {
public:
  I2CDevice(u16 IIC_DEVICE_ID, u16 SCLK_RATE, u8 IIC_ADDR);

  int read(u8 *MsgPtr, s32 ByteCount);

  int write(u8 *MsgPtr, s32 ByteCount);

  int write_then_read(u8 *write_buffer, size_t write_len, u8 *read_buffer, size_t read_len);

  uint8_t address(void);

  int begin(void);

private:
  u8 _addr;
  u16 _deviceID;
  u16 _sclkRate;
  XIicPs *_InstancePtr;

};

#endif 
