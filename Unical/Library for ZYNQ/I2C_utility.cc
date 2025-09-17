/*
 * SSD1306.h
 *
 *  Created on: 18 lug 2024
 *      
 */

#include "I2C_utility.h"

XIicPs IicPsInstance;

I2CDevice::I2CDevice(u16 IIC_DEVICE_ID, u16 SCLK_RATE, u8 IIC_ADDR) {
	_addr = IIC_ADDR;
	_InstancePtr = &IicPsInstance;
	_sclkRate = SCLK_RATE;
	_deviceID = IIC_DEVICE_ID;
}

int I2CDevice::begin(void) {
	int Status;

	XIicPs_Config *Config;
	Config = XIicPs_LookupConfig(_deviceID);
	if (NULL == Config) {
		xil_printf("Configuration error\r\n");
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(_InstancePtr, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("Initialization error\r\n");
		return XST_FAILURE;
	}

	
	Status = XIicPs_SelfTest(_InstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
		xil_printf("Self Test Failed \r\n");
	}

	
	XIicPs_SetSClk(_InstancePtr, _sclkRate);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
		xil_printf("Clock Rate Set Failed \r\n");
	}

	return XST_SUCCESS;
}


u8 I2CDevice::address(void) { return _addr; }

int I2CDevice::write(u8 *MsgPtr, s32 ByteCount) {
	int Status = XIicPs_MasterSendPolled(_InstancePtr, MsgPtr,
			ByteCount, _addr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
		xil_printf("Write Failed \r\n");
	}


	while (XIicPs_BusIsBusy(_InstancePtr)) {
			
		}

	return XST_SUCCESS;
}

int I2CDevice::read(u8 *MsgPtr, s32 ByteCount) {
	int Status = XIicPs_MasterRecvPolled(_InstancePtr, MsgPtr,
			ByteCount, _addr);
	if (Status != XST_SUCCESS) {
			return XST_FAILURE;
			xil_printf("Read Failed \r\n");
		}
	return Status;
}

int I2CDevice::write_then_read(u8 *write_buffer, size_t write_len, u8 *read_buffer, size_t read_len) {
	if(write(write_buffer, write_len) != XST_SUCCESS) {
		xil_printf("write_then_read Failed\r\n");
		return XST_FAILURE;
	}
	return read(read_buffer, read_len);
}





