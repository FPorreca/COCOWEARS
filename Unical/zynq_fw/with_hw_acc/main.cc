#include <stdio.h>
#include "xil_io.h"
#include "xspips.h"
#include "xparameters.h"
#include "xbasic_types.h"
#include "xil_cache.h"
#include "math.h"
#include "xtime_l.h"
#include "BNO055.h"
#include "utility/imumaths.h"
#include "SSD1306.h"

#define MIMU_IIC_DEVICE_ID XPAR_XIICPS_0_DEVICE_ID
#define MIMU_SLAVE_ADDR BNO055_I2C_ADDR1
#define MIMU_SCLK_RATE 50000 #define IIC_BUFFER_SIZE 1 
#define SPI_DEVICE_ID XPAR_XSPIPS_0_DEVICE_ID
#define SPI_BUFFER_SIZE 12 
#define FIXED_POINT_FRACTIONAL_BITS 8

#define DEBUG
#define M_IMU
#define MaUWB
#define DISPLAY

int BNO055_test();

double to_float(int x);

#ifdef DEBUG
void watermark();
#endif

#ifdef M_IMU
BNO055 bno = BNO055(MIMU_IIC_DEVICE_ID, MIMU_SCLK_RATE, MIMU_SLAVE_ADDR);
u8 RecvBuffer[IIC_BUFFER_SIZE];    
Xuint32 *baseaddr_kNN = (Xuint32 *)XPAR_KNN_REV1_0_S00_AXI_BASEADDR; u32 predicted_class = 0;
#endif

#ifdef TIME_ANALYSIS
uint32_t time_for_transaction;
uint32_t time_for_calculus;
XTime gbl_time_before_test;
XTime *p_gbl_time_before_test = &gbl_time_before_test;
XTime gbl_time_after_test;
XTime *p_gbl_time_after_test = &gbl_time_after_test;
#endif

#ifdef MaUWB
Xuint32 *baseaddr_p = (Xuint32 *)XPAR_AXI_TRILATERATION_MO_0_S00_AXI_BASEADDR; static XSpiPs SpiInstance;
u8 TxBuffer[SPI_BUFFER_SIZE] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B };
u8 RxBuffer[SPI_BUFFER_SIZE];
#endif

#ifdef DEBUG
		int countTransaction = 0;
#endif


double to_float(int x) {
    int c = abs(x);
    int sign = 1;
    if (x < 0) {
                c += 3;
        c = ~x;
        sign = -1;
        printf("hello");
    }
    double f = (double)c / (double)pow(2, FIXED_POINT_FRACTIONAL_BITS);
	f = f * sign;
    return f;
}


int main() {
#ifdef DEBUG
	    watermark();
#endif
		Xil_DCacheDisable();

#ifdef MaUWB
				XSpiPs_Config *SpiConfig;
		int Status;

				SpiConfig = XSpiPs_LookupConfig(SPI_DEVICE_ID);
		if (NULL == SpiConfig) {
			xil_printf("Ooops, SPI configuration error!");
			return XST_FAILURE;
		}

				Status = XSpiPs_CfgInitialize(&SpiInstance, SpiConfig,SpiConfig->BaseAddress);
		if (Status == XST_SUCCESS) {
#ifdef DEBUG
				printf("SPI device successfully started!\n");
#endif
		}
		else if (Status == XST_DEVICE_IS_STARTED ) {
			printf("SPI device is is already started. It must be stopped to re-initialize.\n");
			return XST_FAILURE;
		}
		else {
			printf("Unexpected error!\n");
			return XST_FAILURE;
		}

				Status = XSpiPs_SelfTest(&SpiInstance);
		if (Status != XST_SUCCESS) {
			xil_printf("Ooops, SPI configuration error!");
			return XST_FAILURE;
		}

				XSpiPs_SetOptions(&SpiInstance, XSPIPS_MASTER_OPTION | XSPIPS_FORCE_SSELECT_OPTION);

				XSpiPs_SetClkPrescaler(&SpiInstance, XSPIPS_CLK_PRESCALE_32);

#endif

#ifdef M_IMU
				if(!bno.begin()) {
			xil_printf("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
			return XST_FAILURE;
		}
#endif

		while(1) {

#ifdef MaUWB
	#ifdef TIME_ANALYSIS
				XTime_GetTime(p_gbl_time_before_test); 	#endif
								XSpiPs_PolledTransfer(&SpiInstance, TxBuffer, RxBuffer, SPI_BUFFER_SIZE);
	#ifdef TIME_ANALYSIS
				XTime_GetTime(p_gbl_time_after_test); 				time_for_transaction = (u64) gbl_time_after_test - (u64) gbl_time_before_test;
	#endif

	#if defined(DEBUG) && defined(MaUWB)
								countTransaction++;
				printf("\n\nTransaction #%d --> SPI RX BUFFER: \n", countTransaction);
				for(int i=0; i<SPI_BUFFER_SIZE; i++) {
					printf("%#04x \t", RxBuffer[i]);
				}
				printf("\r\n");
		#ifdef TIME_ANALYSIS
				printf("Execution time of transaction: %Lf sec\r\n",(long double)(time_for_transaction *2)/(long double)XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ);
		#endif
	#endif
				if(RxBuffer[0] == 0xFF && RxBuffer[1] == 0xFF && RxBuffer[2] == 0xFF && RxBuffer[3] == 0xFF) {

	#ifdef DEBUG
					printf("Discard SPI RX Buffer \n");
					printf("\n-----------------------------------\n");
	#endif

				}
				else {
	#ifdef TIME_ANALYSIS
					XTime_GetTime(p_gbl_time_before_test); 	#endif
	#ifdef DEBUG
					printf("\n-----------------------------------\n");
	#endif
										*(baseaddr_p+0) = (uint32_t)(RxBuffer[0]) << 24;
	#ifdef DEBUG
					xil_printf("Wrote (AN0.x): 0x%08X \n\r", *(baseaddr_p+0));
	#endif
					*(baseaddr_p+1) = (uint32_t)(RxBuffer[1]) << 24;
	#ifdef DEBUG
					xil_printf("Wrote (AN0.y): 0x%08X \n\r", *(baseaddr_p+1));
					printf("Anchor 0 x: %.2f, y: %.2f \n", to_float((*(baseaddr_p+0)) >> 20), to_float((*(baseaddr_p+1)) >> 20));
	#endif
					*(baseaddr_p+2) = (uint32_t)(RxBuffer[2]) << 24;
	#ifdef DEBUG
					xil_printf("Wrote (dist1): 0x%08X \n\r", *(baseaddr_p+2));
					printf("Dist0 from tag to Anchor 0: %.2f \n\n", to_float((*(baseaddr_p+2)) >> 20));
	#endif

										*(baseaddr_p+3) = (uint32_t)(RxBuffer[3]) << 24;
	#ifdef DEBUG
					xil_printf("Wrote (AN1.x): 0x%08X \n\r", *(baseaddr_p+3));
	#endif
					*(baseaddr_p+4) = (uint32_t)(RxBuffer[4])  << 24;
	#ifdef DEBUG
					xil_printf("Wrote (AN1.y): 0x%08X \n\r", *(baseaddr_p+4));
					printf("Anchor 1 x: %.2f, y: %.2f \n", to_float((*(baseaddr_p+3)) >> 20), to_float((*(baseaddr_p+4)) >> 20));
	#endif
					*(baseaddr_p+5) = (uint32_t)(RxBuffer[5]) << 24;
	#ifdef DEBUG
					xil_printf("Wrote (dist1): 0x%08X \n\r", *(baseaddr_p+5));
					printf("Dist1 from tag to Anchor 1: %.2f \n\n", to_float((*(baseaddr_p+5)) >> 20));
	#endif

										*(baseaddr_p+6) = (uint32_t)(RxBuffer[6]) << 24;
	#ifdef DEBUG
					xil_printf("Wrote (AN2.x): 0x%08X \n\r", *(baseaddr_p+6));
	#endif
					*(baseaddr_p+7) = (uint32_t)(RxBuffer[7])  << 24;
	#ifdef DEBUG
					xil_printf("Wrote (AN2.y): 0x%08X \n\r", *(baseaddr_p+7));
					printf("Anchor 2 x: %.2f, y: %.2f \n", to_float((*(baseaddr_p+6)) >> 20), to_float((*(baseaddr_p+7)) >> 20));
	#endif
					*(baseaddr_p+8) = (uint32_t)(RxBuffer[8]) << 24;
	#ifdef DEBUG
					xil_printf("Wrote (dist2): 0x%08X \n\r", *(baseaddr_p+8));
					printf("Dist2 from tag to Anchor 2: %.2f \n\n", to_float((*(baseaddr_p+8)) >> 20));

															printf("Anchor 3 x: 0x%04X, y: 0x%04X \n", (uint8_t)((RxBuffer[6] & 0xFF00) >> 8), (uint8_t)(RxBuffer[6] & 0x00FF));
					printf("Dist3 from tag to Anchor 3: 0x%04x \n", (uint8_t)((RxBuffer[7] & 0xFF00) >> 8));

					printf("-----------------------------------\n");

										xil_printf("X_pos (HEX): 0x%08x \n\r", *(baseaddr_p+9));
					printf("X_pos (FixP): %.2f\n", to_float(*(baseaddr_p+9)));
										xil_printf("Y_pos (HEX): 0x%08x \n\r", *(baseaddr_p+10));
					printf("Y_pos (FixP): %.2f\n", to_float(*(baseaddr_p+10)));
	#endif
										usleep(100000);
	#ifdef TIME_ANALYSIS
					XTime_GetTime(p_gbl_time_after_test); 					time_for_calculus = (u64) gbl_time_after_test - (u64) gbl_time_before_test;

					printf("Execution time of calculus = %Lf sec\n",(long double)(time_for_calculus *2)/(long double)XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ);
					printf("Total execution time (SPI transaction+calculus) = %Lf sec\n", (long double)((time_for_calculus+time_for_transaction)*2)/(long double)XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ);
	#endif
						#if !defined(DEBUG) && defined(VISUALIZATION)
					printf("#%ld#%ld#\r\n", *(baseaddr_p+9), *(baseaddr_p+10));
	#endif
	#ifdef DEBUG
					printf("-----------------------------------\n");
	#endif
			    }
#endif

#ifdef M_IMU
						imu::Vector<3> acc = bno.getVector(BNO055::VECTOR_ACCELEROMETER);
			usleep(200);

						*(baseaddr_kNN+0) = 0x40000000; 	#ifdef DEBUG
			xil_printf("Wrote control signal: 0x%08X \n\r", *(baseaddr_kNN+0));
	#endif

				#ifdef DEBUG
			printf("M-IMU RAW DATA: \n\n");
			printf("ACC --> X: %.3f\tY: %.3f\tZ: %.3f\r\n", acc.x(), acc.y(), acc.z());
	#endif
			*(baseaddr_kNN+1) = (uint32_t)(int32_t)(acc.x() * (1 << 7)) << 13;
	#ifdef DEBUG
			xil_printf("Wrote ACC_X: 0x%08X \n\r", *(baseaddr_kNN+1));
	#endif
			*(baseaddr_kNN+2) = (uint32_t)(int32_t)(acc.y() * (1 << 7)) << 13;
	#ifdef DEBUG
			xil_printf("Wrote ACC_Y: 0x%08X \n\r", *(baseaddr_kNN+2));
	#endif
			*(baseaddr_kNN+3) = (uint32_t)(int32_t)(acc.z() * (1 << 7)) << 13;
	#ifdef DEBUG
			xil_printf("Wrote ACC_Z: 0x%08X \n\r", *(baseaddr_kNN+3));
	#endif
			imu::Vector<3> gyr = bno.getVector(BNO055::VECTOR_GYROSCOPE);
			usleep(200);

	#ifdef DEBUG
			printf("GYR --> X: %.3f\tY: %.3f\tZ: %.3f\r\n", gyr.x(), gyr.y(), gyr.z());
	#endif
			*(baseaddr_kNN+4) = (uint32_t)(int32_t)(gyr.x() * (1 << 7)) << 13;
	#ifdef DEBUG
			xil_printf("Wrote GYR_X: 0x%08X \n\r", *(baseaddr_kNN+4));
	#endif
			*(baseaddr_kNN+5) = (uint32_t)(int32_t)(gyr.y() * (1 << 7)) << 13;
	#ifdef DEBUG
			xil_printf("Wrote GYR_Y: 0x%08X \n\r", *(baseaddr_kNN+5));
	#endif
			*(baseaddr_kNN+6) = (uint32_t)(int32_t)(gyr.z() * (1 << 7)) << 13;
	#ifdef DEBUG
			xil_printf("Wrote GYR_Z: 0x%08X \n\r", *(baseaddr_kNN+6));
	#endif

			imu::Vector<3> mag = bno.getVector(BNO055::VECTOR_MAGNETOMETER);
			usleep(200);

	#ifdef DEBUG
			printf("MAG --> X: %.3f\tY: %.3f\tZ: %.3f\r\n", mag.x(), mag.y(), mag.z());
	#endif
			*(baseaddr_kNN+7) = (uint32_t)(int32_t)(mag.x() * (1 << 7)) << 13;
	#ifdef DEBUG
			xil_printf("Wrote MAG_X: 0x%08X \n\r", *(baseaddr_kNN+7));
	#endif
			*(baseaddr_kNN+8) = (uint32_t)(int32_t)(mag.y() * (1 << 7)) << 13;
	#ifdef DEBUG
			xil_printf("Wrote MAG_Y: 0x%08X \n\r", *(baseaddr_kNN+8));
	#endif
			*(baseaddr_kNN+9) = (uint32_t)(int32_t)(mag.z() * (1 << 7)) << 13;
	#ifdef DEBUG
			xil_printf("Wrote MAG_Z: 0x%08X \n\r", *(baseaddr_kNN+9));
	#endif

						*(baseaddr_kNN+0) = 0x80000000; 	#ifdef DEBUG
			xil_printf("Wrote control signal: 0x%08X \n\r", *(baseaddr_kNN+0));
			printf("-----------------------------------\n\n\n");
	#endif

						predicted_class = *(baseaddr_kNN+10);
	#ifdef DEBUG
			xil_printf("Read from AXI register 10: 0x%08x \n\r", predicted_class);
	#endif
			if(((predicted_class & 0b00000000000000000000000000000100) >> 2) == 1) {
	#ifdef DEBUG
				xil_printf("Valid = 1 --> Predicted Class: ");
	#endif
				if((predicted_class & 0b00000000000000000000000000000011) == 3) {
	#ifdef DEBUG
					xil_printf("Walk\n\r");
	#endif
				}
				else if((predicted_class & 0b00000000000000000000000000000011) == 0) {
	#ifdef DEBUG
					xil_printf("Sit\n\r");
	#endif
				}
			}
			#endif
		}
	return 0;
}

