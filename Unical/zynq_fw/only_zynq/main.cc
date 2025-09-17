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
#include "KNN.h"

#define MIMU_IIC_DEVICE_ID XPAR_XIICPS_0_DEVICE_ID
#define MIMU_SLAVE_ADDR BNO055_I2C_ADDR1
#define MIMU_SCLK_RATE 50000 #define IIC_BUFFER_SIZE 1 
#define SPI_DEVICE_ID XPAR_XSPIPS_0_DEVICE_ID
#define SPI_BUFFER_SIZE 12 
#define FIXED_POINT_FRACTIONAL_BITS 8

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
u32 predicted_class = 0;
u8 n_test = 10;
float MIMU_data[9];
KNeighborsClassifier knn;

const float X_test[10][9] = {
    { -1.73, -0.37, 9.29, -6.12, 1.56, 0.62, -7.0, -4.25, -31.19},
    { -1.56, -0.57, 9.33, -1.5, 0.37, 0.37, -8.38, -5.37, -31.06},
    { -1.64, -0.59, 9.45, 3.69, -0.19, 1.56, -7.0, -5.37, -30.69},
    { -1.7, -0.36, 9.41, -2.37, 2.06, -0.56, -7.69, -5.37, -30.25},
    { -1.86, -0.7, 9.25, -4.25, 0.44, -5.31, -8.38, -5.75, -30.25},
    { -1.74, -0.63, 9.47, -0.69, 0.13, -3.75, -7.69, -5.06, -31.5},
    { -1.98, -0.75, 9.25, -3.5, -1.44, -7.06, -7.0, -4.69, -29.5},
    { -2.08, -1.3, 9.29, -5.0, -2.5, -10.25, -9.19, -5.06, -30.69},
    { -1.78, -1.23, 9.18, 2.12, -5.0, -15.94, -7.25, -5.06, -31.5},
    { -1.73, -1.47, 9.12, 7.31, -6.62, -19.75, -7.69, -5.37, -31.87}
};

int y_test[10] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

#endif

XTime gbl_time_before_test;
XTime *p_gbl_time_before_test = &gbl_time_before_test;
XTime gbl_time_after_test;
XTime *p_gbl_time_after_test = &gbl_time_after_test;

#ifdef MaUWB
static XSpiPs SpiInstance;
u8 TxBuffer[SPI_BUFFER_SIZE] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B };
u8 RxBuffer[SPI_BUFFER_SIZE];

struct coordinates {
    float x;
    float y;
};

struct Anchor {
	u8 id;
	uint32_t x;
	uint32_t y;
	uint32_t dist;
};

struct Anchor AN0, AN1, AN2;

struct coordinates coord;

#endif

#ifdef DEBUG
		int countTransaction = 0;
#endif

#ifdef MaUWB
void trilateration_2D_3A(Anchor AN0, Anchor AN1, Anchor AN) {

    float A = 2 * AN1.x - 2 * AN0.x;
    float B = 2 * AN1.y - 2 * AN0.y;
    float C = pow(AN0.dist, 2) - pow(AN1.dist, 2) - pow(AN0.x, 2) + pow(AN1.x, 2) - pow(AN0.y, 2) + pow(AN1.y, 2);
    float D = 2 * AN2.x - 2 * AN1.x;
    float E = 2 * AN2.y - 2 * AN1.y;
    float F = pow(AN1.dist, 2) - pow(AN2.dist, 2) - pow(AN1.x, 2) + pow(AN2.x, 2) - pow(AN1.y, 2) + pow(AN2.y, 2);

    coord.x = (C * E - F * B) / (E * A - B * D);
    coord.y = (C * D - A * F) / (B * D - A * E);
}
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
		else {
			printf("BNO055 succesfully started!\n");
		}
		knn.fit(X_train, y_train, n_samples, n_features);
#endif

		while(1) {

#ifdef MaUWB

								XSpiPs_PolledTransfer(&SpiInstance, TxBuffer, RxBuffer, SPI_BUFFER_SIZE);


	#if defined(DEBUG) && defined(MaUWB)
								printf("\n\n--> SPI RX BUFFER: \n");
				for(int i=0; i<SPI_BUFFER_SIZE; i++) {
					printf("%#04x \t", RxBuffer[i]);
				}
				printf("\r\n");

	#endif
				
	#ifdef DEBUG
					printf("Discard SPI RX Buffer \n");
					printf("\n-----------------------------------\n");
	#endif

								
	#ifdef DEBUG
					printf("\n-----------------------------------\n");
	#endif
										AN0.id = 0;
					AN0.x = (uint32_t)(RxBuffer[0]) << 24;
	#ifdef DEBUG
					xil_printf("AN0.x: 0x%08X \n\r",AN0.x);
	#endif
					AN0.y = (uint32_t)(RxBuffer[1]) << 24;
	#ifdef DEBUG
					xil_printf("AN0.y: 0x%08X \n\r", AN0.y);
					printf("Anchor 0 x: %.2f, y: %.2f \n", to_float(AN0.x >> 20), to_float(AN0.y >> 20));
	#endif
					AN0.dist = (uint32_t)(RxBuffer[2]) << 24;
	#ifdef DEBUG
					xil_printf("dist0: 0x%08X \n\r", AN0.dist);
					printf("Dist0 from tag to Anchor 0: %.2f \n\n", to_float(AN0.dist >> 20));
	#endif

										AN1.id = 1;
					AN1.x = (uint32_t)(RxBuffer[3]) << 24;
	#ifdef DEBUG
					xil_printf("AN1.x: 0x%08X \n\r", AN1.x);
	#endif
					AN1.y = (uint32_t)(RxBuffer[4])  << 24;
	#ifdef DEBUG
					xil_printf("AN1.y: 0x%08X \n\r", AN1.y);
					printf("Anchor 1 x: %.2f, y: %.2f \n", to_float(AN1.x >> 20), to_float(AN1.y >> 20));
	#endif
					AN1.dist = (uint32_t)(RxBuffer[5]) << 24;
	#ifdef DEBUG
					xil_printf("dist1: 0x%08X \n\r", AN1.dist);
					printf("Dist1 from tag to Anchor 1: %.2f \n\n", to_float(AN1.dist >> 20));
	#endif

										AN2.id = 2;
					AN2.x = (uint32_t)(RxBuffer[6]) << 24;
	#ifdef DEBUG
					xil_printf("AN2.x: 0x%08X \n\r", AN2.x);
	#endif
					AN2.y = (uint32_t)(RxBuffer[7])  << 24;
	#ifdef DEBUG
					xil_printf("AN2.y: 0x%08X \n\r", AN2.y);
					printf("Anchor 2 x: %.2f, y: %.2f \n", to_float(AN2.x >> 20), to_float(AN2.y >> 20));
	#endif
					AN2.dist = (uint32_t)(RxBuffer[8]) << 24;
	#ifdef DEBUG
					xil_printf("Wrote (dist2): 0x%08X \n\r", AN2.dist);
					printf("Dist2 from tag to Anchor 2: %.2f \n\n", to_float(AN2.dist >> 20));

																				
					printf("-----------------------------------\n");
	#endif
					uint32_t time_for_trilateration;
					XTime_GetTime(p_gbl_time_before_test); 					trilateration_2D_3A(AN0, AN1, AN2);
					XTime_GetTime(p_gbl_time_after_test); 					time_for_trilateration = (u64) gbl_time_after_test - (u64) gbl_time_before_test;
					printf("Execution time of trilateration = %Lf sec\n\n",(long double)(time_for_trilateration *2)/(long double)XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ);
	#ifdef DEBUG
										xil_printf("X_pos (HEX): 0x%08x \n\r", coord.x);
					printf("X_pos (FixP): %.2f\n", to_float(coord.x));
										xil_printf("Y_pos (HEX): 0x%08x \n\r", coord.y);
					printf("Y_pos (FixP): %.2f\n", to_float(coord.y));
	#endif
										
						#if !defined(DEBUG) && defined(VISUALIZATION)
					printf("#%ld#%ld#\r\n", *(baseaddr_p+9), *(baseaddr_p+10));
	#endif
	#ifdef DEBUG
					printf("-----------------------------------\n");
	#endif
			    #endif

#ifdef M_IMU
			uint32_t time_for_prediction;

			XTime_GetTime(p_gbl_time_before_test); 						imu::Vector<3> acc = bno.getVector(BNO055::VECTOR_ACCELEROMETER);
			usleep(200);

				#ifdef DEBUG
							#endif
			MIMU_data[0] = acc.x();
		    MIMU_data[1] = acc.y();
		    MIMU_data[2] = acc.z();

			imu::Vector<3> gyr = bno.getVector(BNO055::VECTOR_GYROSCOPE);
			usleep(200);

	#ifdef DEBUG
				#endif
			MIMU_data[3] = gyr.x();
			MIMU_data[4] = gyr.y();
			MIMU_data[5] = gyr.z();

			imu::Vector<3> mag = bno.getVector(BNO055::VECTOR_MAGNETOMETER);
			usleep(200);

	#ifdef DEBUG
				#endif
			 MIMU_data[6] = mag.x();
			 MIMU_data[7] = mag.y();
			 MIMU_data[8] = mag.z();

			int predictions = knn.predict((float (*)[9])&MIMU_data);
			XTime_GetTime(p_gbl_time_after_test); 			time_for_prediction = (u64) gbl_time_after_test - (u64) gbl_time_before_test;
			printf("Execution time of prediction = %Lf sec\n\n",(long double)(time_for_prediction *2)/(long double)XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ);

									
			

#endif
		}
	return 0;
}

