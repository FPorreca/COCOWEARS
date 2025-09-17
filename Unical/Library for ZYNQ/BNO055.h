/*
 * BNO055.h
 *
 *  Created on: 18 lug 2024
 *      
 */

#include <stdio.h>
#include "xil_printf.h"
#include "I2C_utility.h"
#include "sleep.h"
#include "utility/imumaths.h"

#ifndef SRC_BNO055_H_
#define SRC_BNO055_H_

typedef uint8_t u8; 
typedef uint16_t u16; 
typedef uint32_t u32; 
typedef uint64_t u64; 

typedef int8_t s8; 
typedef int16_t s16; 
typedef int32_t s32; 
typedef int64_t s64; 

#define BNO055_I2C_ADDR1 (0x28)
#define BNO055_I2C_ADDR2 (0x29)

#define  BNO055_INIT_VALUE                         ((u8)0)
#define  BNO055_GEN_READ_WRITE_LENGTH              ((u8)1)
#define  BNO055_LSB_MSB_READ_LENGTH                ((u8)2)
#define  BNO055_MAG_POWER_MODE_RANGE               ((u8)4)
#define  BNO055_MAG_OPR_MODE_RANGE                 ((u8)5)
#define  BNO055_ACCEL_POWER_MODE_RANGE             ((u8)6)
#define  BNO055_ACCEL_SLEEP_DURATION_RANGE         ((u8)16)
#define  BNO055_GYRO_AUTO_SLEEP_DURATION_RANGE     ((u8)8)
#define  BNO055_ACCEL_GYRO_BW_RANGE                ((u8)8)
#define  BNO055_MAG_OUTPUT_RANGE                   ((u8)8)
#define  BNO055_ACCEL_RANGE                        ((u8)5)
#define  BNO055_SHIFT_EIGHT_BITS                   ((u8)8)
#define  BNO055_GYRO_RANGE                         ((u8)5)
#define  BNO055_ACCEL_SLEEP_MODE_RANGE             ((u8)2)

#define BNO055_E_NULL_PTR                          ((s8) - 127)
#define BNO055_OUT_OF_RANGE                        ((s8) - 2)
#define BNO055_SUCCESS                             ((u8)0)
#define BNO055_ERROR

#define BNO055_REV_ID_SIZE                         (2)
#define BNO055_ACCEL_DATA_SIZE                     (2)
#define BNO055_ACCEL_XYZ_DATA_SIZE                 (6)
#define BNO055_MAG_DATA_SIZE                       (2)
#define BNO055_MAG_XYZ_DATA_SIZE                   (6)
#define BNO055_GYRO_DATA_SIZE                      (2)
#define BNO055_GYRO_XYZ_DATA_SIZE                  (6)
#define BNO055_EULER_DATA_SIZE                     (2)
#define BNO055_EULER_HRP_DATA_SIZE                 (6)
#define BNO055_QUATERNION_DATA_SIZE                (2)
#define BNO055_QUATERNION_WXYZ_DATA_SIZE           (8)
#define BNO055_GRAVITY_DATA_SIZE                   (2)
#define BNO055_GRAVITY_XYZ_DATA_SIZE               (6)
#define BNO055_ACCEL_OFFSET_ARRAY                  (6)
#define BNO055_MAG_OFFSET_ARRAY                    (6)
#define BNO055_GYRO_OFFSET_ARRAY                   (6)
#define BNO055_SOFT_IRON_CALIBRATION_MATRIX_SIZE   (18)

#define BNO055_SW_ID_LSB                           (0)
#define BNO055_SW_ID_MSB                           (1)
#define BNO055_SENSOR_DATA_LSB                     (0)
#define BNO055_SENSOR_DATA_MSB                     (1)
#define BNO055_SENSOR_DATA_EULER_LSB               (0)
#define BNO055_SENSOR_DATA_EULER_MSB               (1)
#define BNO055_SENSOR_DATA_QUATERNION_LSB          (0)
#define BNO055_SENSOR_DATA_QUATERNION_MSB          (1)

#define BNO055_SENSOR_DATA_QUATERNION_WXYZ_W_LSB   (0)
#define BNO055_SENSOR_DATA_QUATERNION_WXYZ_W_MSB   (1)
#define BNO055_SENSOR_DATA_QUATERNION_WXYZ_X_LSB   (2)
#define BNO055_SENSOR_DATA_QUATERNION_WXYZ_X_MSB   (3)
#define BNO055_SENSOR_DATA_QUATERNION_WXYZ_Y_LSB   (4)
#define BNO055_SENSOR_DATA_QUATERNION_WXYZ_Y_MSB   (5)
#define BNO055_SENSOR_DATA_QUATERNION_WXYZ_Z_LSB   (6)
#define BNO055_SENSOR_DATA_QUATERNION_WXYZ_Z_MSB   (7)

#define BNO055_SENSOR_DATA_XYZ_X_LSB               (0)
#define BNO055_SENSOR_DATA_XYZ_X_MSB               (1)
#define BNO055_SENSOR_DATA_XYZ_Y_LSB               (2)
#define BNO055_SENSOR_DATA_XYZ_Y_MSB               (3)
#define BNO055_SENSOR_DATA_XYZ_Z_LSB               (4)
#define BNO055_SENSOR_DATA_XYZ_Z_MSB               (5)

#define BNO055_SENSOR_DATA_EULER_HRP_H_LSB         (0)
#define BNO055_SENSOR_DATA_EULER_HRP_H_MSB         (1)
#define BNO055_SENSOR_DATA_EULER_HRP_R_LSB         (2)
#define BNO055_SENSOR_DATA_EULER_HRP_R_MSB         (3)
#define BNO055_SENSOR_DATA_EULER_HRP_P_LSB         (4)
#define BNO055_SENSOR_DATA_EULER_HRP_P_MSB         (5)

#define BNO055_SOFT_IRON_CALIB_0_LSB               (0)
#define BNO055_SOFT_IRON_CALIB_0_MSB               (1)
#define BNO055_SOFT_IRON_CALIB_1_LSB               (2)
#define BNO055_SOFT_IRON_CALIB_1_MSB               (3)
#define BNO055_SOFT_IRON_CALIB_2_LSB               (4)
#define BNO055_SOFT_IRON_CALIB_2_MSB               (5)
#define BNO055_SOFT_IRON_CALIB_3_LSB               (6)
#define BNO055_SOFT_IRON_CALIB_3_MSB               (7)
#define BNO055_SOFT_IRON_CALIB_4_LSB               (8)
#define BNO055_SOFT_IRON_CALIB_4_MSB               (9)
#define BNO055_SOFT_IRON_CALIB_5_LSB               (10)
#define BNO055_SOFT_IRON_CALIB_5_MSB               (11)
#define BNO055_SOFT_IRON_CALIB_6_LSB               (12)
#define BNO055_SOFT_IRON_CALIB_6_MSB               (13)
#define BNO055_SOFT_IRON_CALIB_7_LSB               (14)
#define BNO055_SOFT_IRON_CALIB_7_MSB               (15)
#define BNO055_SOFT_IRON_CALIB_8_LSB               (16)
#define BNO055_SOFT_IRON_CALIB_8_MSB               (17)

#define BNO055_SENSOR_OFFSET_DATA_X_LSB            (0)
#define BNO055_SENSOR_OFFSET_DATA_X_MSB            (1)
#define BNO055_SENSOR_OFFSET_DATA_Y_LSB            (2)
#define BNO055_SENSOR_OFFSET_DATA_Y_MSB            (3)
#define BNO055_SENSOR_OFFSET_DATA_Z_LSB            (4)
#define BNO055_SENSOR_OFFSET_DATA_Z_MSB            (5)

#define BNO055_OFFSET_RADIUS_LSB                   (0)
#define BNO055_OFFSET_RADIUS_MSB                   (1)

#define BNO055_CHIP_ID_POS                        (0)
#define BNO055_CHIP_ID_MSK                        (0xFF)
#define BNO055_CHIP_ID_LEN                        (8)
#define BNO055_CHIP_ID_REG                        BNO055_CHIP_ID_ADDR

#define BNO055_ACCEL_REV_ID_POS                   (0)
#define BNO055_ACCEL_REV_ID_MSK                   (0xFF)
#define BNO055_ACCEL_REV_ID_LEN                   (8)
#define BNO055_ACCEL_REV_ID_REG                   BNO055_ACCEL_REV_ID_ADDR

#define BNO055_MAG_REV_ID_POS                     (0)
#define BNO055_MAG_REV_ID_MSK                     (0xFF)
#define BNO055_MAG_REV_ID_LEN                     (8)
#define BNO055_MAG_REV_ID_REG                     BNO055_MAG_REV_ID_ADDR

#define BNO055_GYRO_REV_ID_POS                    (0)
#define BNO055_GYRO_REV_ID_MSK                    (0xFF)
#define BNO055_GYRO_REV_ID_LEN                    (8)
#define BNO055_GYRO_REV_ID_REG                    BNO055_GYRO_REV_ID_ADDR

#define BNO055_SW_REV_ID_LSB_POS                  (0)
#define BNO055_SW_REV_ID_LSB_MSK                  (0xFF)
#define BNO055_SW_REV_ID_LSB_LEN                  (8)
#define BNO055_SW_REV_ID_LSB_REG                  BNO055_SW_REV_ID_LSB_ADDR

#define BNO055_SW_REV_ID_MSB_POS                  (0)
#define BNO055_SW_REV_ID_MSB_MSK                  (0xFF)
#define BNO055_SW_REV_ID_MSB_LEN                  (8)
#define BNO055_SW_REV_ID_MSB_REG                  BNO055_SW_REV_ID_MSB_ADDR

#define BNO055_BL_REV_ID_POS                      (0)
#define BNO055_BL_REV_ID_MSK                      (0xFF)
#define BNO055_BL_REV_ID_LEN                      (8)
#define BNO055_BL_REV_ID_REG                      BNO055_BL_REV_ID_ADDR

#define BNO055_PAGE_ID_POS                        (0)
#define BNO055_PAGE_ID_MSK                        (0xFF)
#define BNO055_PAGE_ID_LEN                        (8)
#define BNO055_PAGE_ID_REG                        BNO055_PAGE_ID_ADDR

#define BNO055_ACCEL_DATA_X_LSB_VALUEX_POS        (0)
#define BNO055_ACCEL_DATA_X_LSB_VALUEX_MSK        (0xFF)
#define BNO055_ACCEL_DATA_X_LSB_VALUEX_LEN        (8)
#define BNO055_ACCEL_DATA_X_LSB_VALUEX_REG             \
    BNO055_ACCEL_DATA_X_LSB_ADDR

#define BNO055_ACCEL_DATA_X_MSB_VALUEX_POS        (0)
#define BNO055_ACCEL_DATA_X_MSB_VALUEX_MSK        (0xFF)
#define BNO055_ACCEL_DATA_X_MSB_VALUEX_LEN        (8)
#define BNO055_ACCEL_DATA_X_MSB_VALUEX_REG             \
    BNO055_ACCEL_DATA_X_MSB_ADDR


#define BNO055_ACCEL_DATA_Y_LSB_VALUEY_POS        (0)
#define BNO055_ACCEL_DATA_Y_LSB_VALUEY_MSK        (0xFF)
#define BNO055_ACCEL_DATA_Y_LSB_VALUEY_LEN        (8)
#define BNO055_ACCEL_DATA_Y_LSB_VALUEY_REG             \
    BNO055_ACCEL_DATA_Y_LSB_ADDR

#define BNO055_ACCEL_DATA_Y_MSB_VALUEY_POS        (0)
#define BNO055_ACCEL_DATA_Y_MSB_VALUEY_MSK        (0xFF)
#define BNO055_ACCEL_DATA_Y_MSB_VALUEY_LEN        (8)
#define BNO055_ACCEL_DATA_Y_MSB_VALUEY_REG             \
    BNO055_ACCEL_DATA_Y_MSB_ADDR

#define BNO055_ACCEL_DATA_Z_LSB_VALUEZ_POS        (0)
#define BNO055_ACCEL_DATA_Z_LSB_VALUEZ_MSK        (0xFF)
#define BNO055_ACCEL_DATA_Z_LSB_VALUEZ_LEN        (8)
#define BNO055_ACCEL_DATA_Z_LSB_VALUEZ_REG     \
    BNO055_ACCEL_DATA_Z_LSB_ADDR

#define BNO055_ACCEL_DATA_Z_MSB_VALUEZ_POS        (0)
#define BNO055_ACCEL_DATA_Z_MSB_VALUEZ_MSK        (0xFF)
#define BNO055_ACCEL_DATA_Z_MSB_VALUEZ_LEN        (8)
#define BNO055_ACCEL_DATA_Z_MSB_VALUEZ_REG     \
    BNO055_ACCEL_DATA_Z_MSB_ADDR

#define BNO055_MAG_DATA_X_LSB_VALUEX_POS          (0)
#define BNO055_MAG_DATA_X_LSB_VALUEX_MSK          (0xFF)
#define BNO055_MAG_DATA_X_LSB_VALUEX_LEN          (8)
#define BNO055_MAG_DATA_X_LSB_VALUEX_REG             \
    BNO055_MAG_DATA_X_LSB_ADDR

#define BNO055_MAG_DATA_X_MSB_VALUEX_POS          (0)
#define BNO055_MAG_DATA_X_MSB_VALUEX_MSK          (0xFF)
#define BNO055_MAG_DATA_X_MSB_VALUEX_LEN          (8)
#define BNO055_MAG_DATA_X_MSB_VALUEX_REG          BNO055_MAG_DATA_X_MSB_ADDR

#define BNO055_MAG_DATA_Y_LSB_VALUEY_POS          (0)
#define BNO055_MAG_DATA_Y_LSB_VALUEY_MSK          (0xFF)
#define BNO055_MAG_DATA_Y_LSB_VALUEY_LEN          (8)
#define BNO055_MAG_DATA_Y_LSB_VALUEY_REG          BNO055_MAG_DATA_Y_LSB_ADDR

#define BNO055_MAG_DATA_Y_MSB_VALUEY_POS          (0)
#define BNO055_MAG_DATA_Y_MSB_VALUEY_MSK          (0xFF)
#define BNO055_MAG_DATA_Y_MSB_VALUEY_LEN          (8)
#define BNO055_MAG_DATA_Y_MSB_VALUEY_REG          BNO055_MAG_DATA_Y_MSB_ADDR

#define BNO055_MAG_DATA_Z_LSB_VALUEZ_POS          (0)
#define BNO055_MAG_DATA_Z_LSB_VALUEZ_MSK          (0xFF)
#define BNO055_MAG_DATA_Z_LSB_VALUEZ_LEN          (8)
#define BNO055_MAG_DATA_Z_LSB_VALUEZ_REG          BNO055_MAG_DATA_Z_LSB_ADDR

#define BNO055_MAG_DATA_Z_MSB_VALUEZ_POS          (0)
#define BNO055_MAG_DATA_Z_MSB_VALUEZ_MSK          (0xFF)
#define BNO055_MAG_DATA_Z_MSB_VALUEZ_LEN          (8)
#define BNO055_MAG_DATA_Z_MSB_VALUEZ_REG          BNO055_MAG_DATA_Z_MSB_ADDR

#define BNO055_GYRO_DATA_X_LSB_VALUEX_POS         (0)
#define BNO055_GYRO_DATA_X_LSB_VALUEX_MSK         (0xFF)
#define BNO055_GYRO_DATA_X_LSB_VALUEX_LEN         (8)
#define BNO055_GYRO_DATA_X_LSB_VALUEX_REG         BNO055_GYRO_DATA_X_LSB_ADDR

#define BNO055_GYRO_DATA_X_MSB_VALUEX_POS         (0)
#define BNO055_GYRO_DATA_X_MSB_VALUEX_MSK         (0xFF)
#define BNO055_GYRO_DATA_X_MSB_VALUEX_LEN         (8)
#define BNO055_GYRO_DATA_X_MSB_VALUEX_REG         BNO055_GYRO_DATA_X_MSB_ADDR

#define BNO055_GYRO_DATA_Y_LSB_VALUEY_POS         (0)
#define BNO055_GYRO_DATA_Y_LSB_VALUEY_MSK         (0xFF)
#define BNO055_GYRO_DATA_Y_LSB_VALUEY_LEN         (8)
#define BNO055_GYRO_DATA_Y_LSB_VALUEY_REG         BNO055_GYRO_DATA_Y_LSB_ADDR

#define BNO055_GYRO_DATA_Y_MSB_VALUEY_POS         (0)
#define BNO055_GYRO_DATA_Y_MSB_VALUEY_MSK         (0xFF)
#define BNO055_GYRO_DATA_Y_MSB_VALUEY_LEN         (8)
#define BNO055_GYRO_DATA_Y_MSB_VALUEY_REG         BNO055_GYRO_DATA_Y_MSB_ADDR


#define BNO055_GYRO_DATA_Z_LSB_VALUEZ_POS         (0)
#define BNO055_GYRO_DATA_Z_LSB_VALUEZ_MSK         (0xFF)
#define BNO055_GYRO_DATA_Z_LSB_VALUEZ_LEN         (8)
#define BNO055_GYRO_DATA_Z_LSB_VALUEZ_REG         BNO055_GYRO_DATA_Z_LSB_ADDR


#define BNO055_GYRO_DATA_Z_MSB_VALUEZ_POS         (0)
#define BNO055_GYRO_DATA_Z_MSB_VALUEZ_MSK         (0xFF)
#define BNO055_GYRO_DATA_Z_MSB_VALUEZ_LEN         (8)
#define BNO055_GYRO_DATA_Z_MSB_VALUEZ_REG         BNO055_GYRO_DATA_Z_MSB_ADDR


#define BNO055_EULER_H_LSB_VALUEH_POS             (0)
#define BNO055_EULER_H_LSB_VALUEH_MSK             (0xFF)
#define BNO055_EULER_H_LSB_VALUEH_LEN             (8)
#define BNO055_EULER_H_LSB_VALUEH_REG             BNO055_EULER_H_LSB_ADDR


#define BNO055_EULER_H_MSB_VALUEH_POS             (0)
#define BNO055_EULER_H_MSB_VALUEH_MSK             (0xFF)
#define BNO055_EULER_H_MSB_VALUEH_LEN             (8)
#define BNO055_EULER_H_MSB_VALUEH_REG             BNO055_EULER_H_MSB_ADDR


#define BNO055_EULER_R_LSB_VALUER_POS             (0)
#define BNO055_EULER_R_LSB_VALUER_MSK             (0xFF)
#define BNO055_EULER_R_LSB_VALUER_LEN             (8)
#define BNO055_EULER_R_LSB_VALUER_REG             BNO055_EULER_R_LSB_ADDR


#define BNO055_EULER_R_MSB_VALUER_POS             (0)
#define BNO055_EULER_R_MSB_VALUER_MSK             (0xFF)
#define BNO055_EULER_R_MSB_VALUER_LEN             (8)
#define BNO055_EULER_R_MSB_VALUER_REG             BNO055_EULER_R_MSB_ADDR


#define BNO055_EULER_P_LSB_VALUEP_POS             (0)
#define BNO055_EULER_P_LSB_VALUEP_MSK             (0xFF)
#define BNO055_EULER_P_LSB_VALUEP_LEN             (8)
#define BNO055_EULER_P_LSB_VALUEP_REG             BNO055_EULER_P_LSB_ADDR


#define BNO055_EULER_P_MSB_VALUEP_POS             (0)
#define BNO055_EULER_P_MSB_VALUEP_MSK             (0xFF)
#define BNO055_EULER_P_MSB_VALUEP_LEN             (8)
#define BNO055_EULER_P_MSB_VALUEP_REG             BNO055_EULER_P_MSB_ADDR


#define BNO055_QUATERNION_DATA_W_LSB_VALUEW_POS   (0)
#define BNO055_QUATERNION_DATA_W_LSB_VALUEW_MSK   (0xFF)
#define BNO055_QUATERNION_DATA_W_LSB_VALUEW_LEN   (8)
#define BNO055_QUATERNION_DATA_W_LSB_VALUEW_REG  \
    BNO055_QUATERNION_DATA_W_LSB_ADDR


#define BNO055_QUATERNION_DATA_W_MSB_VALUEW_POS   (0)
#define BNO055_QUATERNION_DATA_W_MSB_VALUEW_MSK   (0xFF)
#define BNO055_QUATERNION_DATA_W_MSB_VALUEW_LEN   (8)
#define BNO055_QUATERNION_DATA_W_MSB_VALUEW_REG  \
    BNO055_QUATERNION_DATA_W_MSB_ADDR


#define BNO055_QUATERNION_DATA_X_LSB_VALUEX_POS   (0)
#define BNO055_QUATERNION_DATA_X_LSB_VALUEX_MSK   (0xFF)
#define BNO055_QUATERNION_DATA_X_LSB_VALUEX_LEN   (8)
#define BNO055_QUATERNION_DATA_X_LSB_VALUEX_REG  \
    BNO055_QUATERNION_DATA_X_LSB_ADDR


#define BNO055_QUATERNION_DATA_X_MSB_VALUEX_POS   (0)
#define BNO055_QUATERNION_DATA_X_MSB_VALUEX_MSK   (0xFF)
#define BNO055_QUATERNION_DATA_X_MSB_VALUEX_LEN   (8)
#define BNO055_QUATERNION_DATA_X_MSB_VALUEX_REG \
    BNO055_QUATERNION_DATA_X_MSB_ADDR


#define BNO055_QUATERNION_DATA_Y_LSB_VALUEY_POS   (0)
#define BNO055_QUATERNION_DATA_Y_LSB_VALUEY_MSK   (0xFF)
#define BNO055_QUATERNION_DATA_Y_LSB_VALUEY_LEN   (8)
#define BNO055_QUATERNION_DATA_Y_LSB_VALUEY_REG \
    BNO055_QUATERNION_DATA_Y_LSB_ADDR


#define BNO055_QUATERNION_DATA_Y_MSB_VALUEY_POS   (0)
#define BNO055_QUATERNION_DATA_Y_MSB_VALUEY_MSK   (0xFF)
#define BNO055_QUATERNION_DATA_Y_MSB_VALUEY_LEN   (8)
#define BNO055_QUATERNION_DATA_Y_MSB_VALUEY_REG  \
    BNO055_QUATERNION_DATA_Y_MSB_ADDR


#define BNO055_QUATERNION_DATA_Z_LSB_VALUEZ_POS   (0)
#define BNO055_QUATERNION_DATA_Z_LSB_VALUEZ_MSK   (0xFF)
#define BNO055_QUATERNION_DATA_Z_LSB_VALUEZ_LEN   (8)
#define BNO055_QUATERNION_DATA_Z_LSB_VALUEZ_REG \
    BNO055_QUATERNION_DATA_Z_LSB_ADDR


#define BNO055_QUATERNION_DATA_Z_MSB_VALUEZ_POS   (0)
#define BNO055_QUATERNION_DATA_Z_MSB_VALUEZ_MSK   (0xFF)
#define BNO055_QUATERNION_DATA_Z_MSB_VALUEZ_LEN   (8)
#define BNO055_QUATERNION_DATA_Z_MSB_VALUEZ_REG  \
    BNO055_QUATERNION_DATA_Z_MSB_ADDR


#define BNO055_LINEAR_ACCEL_DATA_X_LSB_VALUEX_POS (0)
#define BNO055_LINEAR_ACCEL_DATA_X_LSB_VALUEX_MSK (0xFF)
#define BNO055_LINEAR_ACCEL_DATA_X_LSB_VALUEX_LEN (8)
#define BNO055_LINEAR_ACCEL_DATA_X_LSB_VALUEX_REG  \
    BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR


#define BNO055_LINEAR_ACCEL_DATA_X_MSB_VALUEX_POS (0)
#define BNO055_LINEAR_ACCEL_DATA_X_MSB_VALUEX_MSK (0xFF)
#define BNO055_LINEAR_ACCEL_DATA_X_MSB_VALUEX_LEN (8)
#define BNO055_LINEAR_ACCEL_DATA_X_MSB_VALUEX_REG  \
    BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR


#define BNO055_LINEAR_ACCEL_DATA_Y_LSB_VALUEY_POS (0)
#define BNO055_LINEAR_ACCEL_DATA_Y_LSB_VALUEY_MSK (0xFF)
#define BNO055_LINEAR_ACCEL_DATA_Y_LSB_VALUEY_LEN (8)
#define BNO055_LINEAR_ACCEL_DATA_Y_LSB_VALUEY_REG  \
    BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR


#define BNO055_LINEAR_ACCEL_DATA_Y_MSB_VALUEY_POS (0)
#define BNO055_LINEAR_ACCEL_DATA_Y_MSB_VALUEY_MSK (0xFF)
#define BNO055_LINEAR_ACCEL_DATA_Y_MSB_VALUEY_LEN (8)
#define BNO055_LINEAR_ACCEL_DATA_Y_MSB_VALUEY_REG  \
    BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR


#define BNO055_LINEAR_ACCEL_DATA_Z_LSB_VALUEZ_POS (0)
#define BNO055_LINEAR_ACCEL_DATA_Z_LSB_VALUEZ_MSK (0xFF)
#define BNO055_LINEAR_ACCEL_DATA_Z_LSB_VALUEZ_LEN (8)
#define BNO055_LINEAR_ACCEL_DATA_Z_LSB_VALUEZ_REG \
    BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR


#define BNO055_LINEAR_ACCEL_DATA_Z_MSB_VALUEZ_POS (0)
#define BNO055_LINEAR_ACCEL_DATA_Z_MSB_VALUEZ_MSK (0xFF)
#define BNO055_LINEAR_ACCEL_DATA_Z_MSB_VALUEZ_LEN (8)
#define BNO055_LINEAR_ACCEL_DATA_Z_MSB_VALUEZ_REG  \
    BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR


#define BNO055_GRAVITY_DATA_X_LSB_VALUEX_POS      (0)
#define BNO055_GRAVITY_DATA_X_LSB_VALUEX_MSK      (0xFF)
#define BNO055_GRAVITY_DATA_X_LSB_VALUEX_LEN      (8)
#define BNO055_GRAVITY_DATA_X_LSB_VALUEX_REG  \
    BNO055_GRAVITY_DATA_X_LSB_ADDR


#define BNO055_GRAVITY_DATA_X_MSB_VALUEX_POS      (0)
#define BNO055_GRAVITY_DATA_X_MSB_VALUEX_MSK      (0xFF)
#define BNO055_GRAVITY_DATA_X_MSB_VALUEX_LEN      (8)
#define BNO055_GRAVITY_DATA_X_MSB_VALUEX_REG  \
    BNO055_GRAVITY_DATA_X_MSB_ADDR


#define BNO055_GRAVITY_DATA_Y_LSB_VALUEY_POS      (0)
#define BNO055_GRAVITY_DATA_Y_LSB_VALUEY_MSK      (0xFF)
#define BNO055_GRAVITY_DATA_Y_LSB_VALUEY_LEN      (8)
#define BNO055_GRAVITY_DATA_Y_LSB_VALUEY_REG  \
    BNO055_GRAVITY_DATA_Y_LSB_ADDR


#define BNO055_GRAVITY_DATA_Y_MSB_VALUEY_POS      (0)
#define BNO055_GRAVITY_DATA_Y_MSB_VALUEY_MSK      (0xFF)
#define BNO055_GRAVITY_DATA_Y_MSB_VALUEY_LEN      (8)
#define BNO055_GRAVITY_DATA_Y_MSB_VALUEY_REG  \
    BNO055_GRAVITY_DATA_Y_MSB_ADDR


#define BNO055_GRAVITY_DATA_Z_LSB_VALUEZ_POS      (0)
#define BNO055_GRAVITY_DATA_Z_LSB_VALUEZ_MSK      (0xFF)
#define BNO055_GRAVITY_DATA_Z_LSB_VALUEZ_LEN      (8)
#define BNO055_GRAVITY_DATA_Z_LSB_VALUEZ_REG  \
    BNO055_GRAVITY_DATA_Z_LSB_ADDR


#define BNO055_GRAVITY_DATA_Z_MSB_VALUEZ_POS      (0)
#define BNO055_GRAVITY_DATA_Z_MSB_VALUEZ_MSK      (0xFF)
#define BNO055_GRAVITY_DATA_Z_MSB_VALUEZ_LEN      (8)
#define BNO055_GRAVITY_DATA_Z_MSB_VALUEZ_REG  \
    BNO055_GRAVITY_DATA_Z_MSB_ADDR


#define BNO055_TEMP_POS                           (0)
#define BNO055_TEMP_MSK                           (0xFF)
#define BNO055_TEMP_LEN                           (8)
#define BNO055_TEMP_REG                           BNO055_TEMP_ADDR


#define BNO055_MAG_CALIB_STAT_POS                 (0)
#define BNO055_MAG_CALIB_STAT_MSK                 (0X03)
#define BNO055_MAG_CALIB_STAT_LEN                 (2)
#define BNO055_MAG_CALIB_STAT_REG                 BNO055_CALIB_STAT_ADDR


#define BNO055_ACCEL_CALIB_STAT_POS               (2)
#define BNO055_ACCEL_CALIB_STAT_MSK               (0X0C)
#define BNO055_ACCEL_CALIB_STAT_LEN               (2)
#define BNO055_ACCEL_CALIB_STAT_REG               BNO055_CALIB_STAT_ADDR


#define BNO055_GYRO_CALIB_STAT_POS                (4)
#define BNO055_GYRO_CALIB_STAT_MSK                (0X30)
#define BNO055_GYRO_CALIB_STAT_LEN                (2)
#define BNO055_GYRO_CALIB_STAT_REG                BNO055_CALIB_STAT_ADDR


#define BNO055_SYS_CALIB_STAT_POS                 (6)
#define BNO055_SYS_CALIB_STAT_MSK                 (0XC0)
#define BNO055_SYS_CALIB_STAT_LEN                 (2)
#define BNO055_SYS_CALIB_STAT_REG                 BNO055_CALIB_STAT_ADDR


#define BNO055_SELFTEST_ACCEL_POS                 (0)
#define BNO055_SELFTEST_ACCEL_MSK                 (0X01)
#define BNO055_SELFTEST_ACCEL_LEN                 (1)
#define BNO055_SELFTEST_ACCEL_REG                 BNO055_SELFTEST_RESULT_ADDR


#define BNO055_SELFTEST_MAG_POS                   (1)
#define BNO055_SELFTEST_MAG_MSK                   (0X02)
#define BNO055_SELFTEST_MAG_LEN                   (1)
#define BNO055_SELFTEST_MAG_REG                   BNO055_SELFTEST_RESULT_ADDR


#define BNO055_SELFTEST_GYRO_POS                  (2)
#define BNO055_SELFTEST_GYRO_MSK                  (0X04)
#define BNO055_SELFTEST_GYRO_LEN                  (1)
#define BNO055_SELFTEST_GYRO_REG                  BNO055_SELFTEST_RESULT_ADDR


#define BNO055_SELFTEST_MCU_POS                   (3)
#define BNO055_SELFTEST_MCU_MSK                   (0X08)
#define BNO055_SELFTEST_MCU_LEN                   (1)
#define BNO055_SELFTEST_MCU_REG                   BNO055_SELFTEST_RESULT_ADDR


#define BNO055_INTR_STAT_GYRO_ANY_MOTION_POS      (2)
#define BNO055_INTR_STAT_GYRO_ANY_MOTION_MSK      (0X04)
#define BNO055_INTR_STAT_GYRO_ANY_MOTION_LEN      (1)
#define BNO055_INTR_STAT_GYRO_ANY_MOTION_REG      BNO055_INTR_STAT_ADDR

#define BNO055_INTR_STAT_GYRO_HIGHRATE_POS        (3)
#define BNO055_INTR_STAT_GYRO_HIGHRATE_MSK        (0X08)
#define BNO055_INTR_STAT_GYRO_HIGHRATE_LEN        (1)
#define BNO055_INTR_STAT_GYRO_HIGHRATE_REG        BNO055_INTR_STAT_ADDR

#define BNO055_INTR_STAT_ACCEL_HIGH_G_POS         (5)
#define BNO055_INTR_STAT_ACCEL_HIGH_G_MSK         (0X20)
#define BNO055_INTR_STAT_ACCEL_HIGH_G_LEN         (1)
#define BNO055_INTR_STAT_ACCEL_HIGH_G_REG         BNO055_INTR_STAT_ADDR

#define BNO055_INTR_STAT_ACCEL_ANY_MOTION_POS     (6)
#define BNO055_INTR_STAT_ACCEL_ANY_MOTION_MSK     (0X40)
#define BNO055_INTR_STAT_ACCEL_ANY_MOTION_LEN     (1)
#define BNO055_INTR_STAT_ACCEL_ANY_MOTION_REG     BNO055_INTR_STAT_ADDR

#define BNO055_INTR_STAT_ACCEL_NO_MOTION_POS      (7)
#define BNO055_INTR_STAT_ACCEL_NO_MOTION_MSK      (0X80)
#define BNO055_INTR_STAT_ACCEL_NO_MOTION_LEN      (1)
#define BNO055_INTR_STAT_ACCEL_NO_MOTION_REG      BNO055_INTR_STAT_ADDR


#define BNO055_SYS_MAIN_CLK_POS                   (0)
#define BNO055_SYS_MAIN_CLK_MSK                   (0X10)
#define BNO055_SYS_MAIN_CLK_LEN                   (1)
#define BNO055_SYS_MAIN_CLK_REG                   BNO055_SYS_CLK_STAT_ADDR


#define BNO055_SYS_STAT_CODE_POS                  (0)
#define BNO055_SYS_STAT_CODE_MSK                  (0XFF)
#define BNO055_SYS_STAT_CODE_LEN                  (8)
#define BNO055_SYS_STAT_CODE_REG                  BNO055_SYS_STAT_ADDR

#define BNO055_SYS_ERROR_CODE_POS                 (0)
#define BNO055_SYS_ERROR_CODE_MSK                 (0XFF)
#define BNO055_SYS_ERROR_CODE_LEN                 (8)
#define BNO055_SYS_ERROR_CODE_REG                 BNO055_SYS_ERR_ADDR


#define BNO055_ACCEL_UNIT_POS                     (0)
#define BNO055_ACCEL_UNIT_MSK                     (0X01)
#define BNO055_ACCEL_UNIT_LEN                     (1)
#define BNO055_ACCEL_UNIT_REG                     BNO055_UNIT_SEL_ADDR


#define BNO055_GYRO_UNIT_POS                      (1)
#define BNO055_GYRO_UNIT_MSK                      (0X02)
#define BNO055_GYRO_UNIT_LEN                      (1)
#define BNO055_GYRO_UNIT_REG                      BNO055_UNIT_SEL_ADDR


#define BNO055_EULER_UNIT_POS                     (2)
#define BNO055_EULER_UNIT_MSK                     (0X04)
#define BNO055_EULER_UNIT_LEN                     (1)
#define BNO055_EULER_UNIT_REG                     BNO055_UNIT_SEL_ADDR


#define BNO055_TILT_UNIT_POS                      (3)
#define BNO055_TILT_UNIT_MSK                      (0X08)
#define BNO055_TILT_UNIT_LEN                      (1)
#define BNO055_TILT_UNIT_REG                      BNO055_UNIT_SEL_ADDR


#define BNO055_TEMP_UNIT_POS                      (4)
#define BNO055_TEMP_UNIT_MSK                      (0X10)
#define BNO055_TEMP_UNIT_LEN                      (1)
#define BNO055_TEMP_UNIT_REG                      BNO055_UNIT_SEL_ADDR


#define BNO055_DATA_OUTPUT_FORMAT_POS             (7)
#define BNO055_DATA_OUTPUT_FORMAT_MSK             (0X80)
#define BNO055_DATA_OUTPUT_FORMAT_LEN             (1)
#define BNO055_DATA_OUTPUT_FORMAT_REG             BNO055_UNIT_SEL_ADDR


#define BNO055_OPERATION_MODE_POS                 (0)
#define BNO055_OPERATION_MODE_MSK                 (0X0F)
#define BNO055_OPERATION_MODE_LEN                 (4)
#define BNO055_OPERATION_MODE_REG                 BNO055_OPR_MODE_ADDR


#define BNO055_POWER_MODE_POS                     (0)
#define BNO055_POWER_MODE_MSK                     (0X03)
#define BNO055_POWER_MODE_LEN                     (2)
#define BNO055_POWER_MODE_REG                     BNO055_PWR_MODE_ADDR


#define BNO055_SELFTEST_POS                       (0)
#define BNO055_SELFTEST_MSK                       (0X01)
#define BNO055_SELFTEST_LEN                       (1)
#define BNO055_SELFTEST_REG                       BNO055_SYS_TRIGGER_ADDR


#define BNO055_SYS_RST_POS                        (5)
#define BNO055_SYS_RST_MSK                        (0X20)
#define BNO055_SYS_RST_LEN                        (1)
#define BNO055_SYS_RST_REG                        BNO055_SYS_TRIGGER_ADDR


#define BNO055_INTR_RST_POS                       (6)
#define BNO055_INTR_RST_MSK                       (0X40)
#define BNO055_INTR_RST_LEN                       (1)
#define BNO055_INTR_RST_REG                       BNO055_SYS_TRIGGER_ADDR


#define BNO055_CLK_SRC_POS                        (7)
#define BNO055_CLK_SRC_MSK                        (0X80)
#define BNO055_CLK_SRC_LEN                        (1)
#define BNO055_CLK_SRC_REG                        BNO055_SYS_TRIGGER_ADDR


#define BNO055_TEMP_SOURCE_POS                    (0)
#define BNO055_TEMP_SOURCE_MSK                    (0X03)
#define BNO055_TEMP_SOURCE_LEN                    (2)
#define BNO055_TEMP_SOURCE_REG                    BNO055_TEMP_SOURCE_ADDR


#define BNO055_REMAP_AXIS_VALUE_POS               (0)
#define BNO055_REMAP_AXIS_VALUE_MSK               (0X3F)
#define BNO055_REMAP_AXIS_VALUE_LEN               (6)
#define BNO055_REMAP_AXIS_VALUE_REG               BNO055_AXIS_MAP_CONFIG_ADDR


#define BNO055_REMAP_Z_SIGN_POS                   (0)
#define BNO055_REMAP_Z_SIGN_MSK                   (0X01)
#define BNO055_REMAP_Z_SIGN_LEN                   (1)
#define BNO055_REMAP_Z_SIGN_REG                   BNO055_AXIS_MAP_SIGN_ADDR

#define BNO055_REMAP_Y_SIGN_POS                   (1)
#define BNO055_REMAP_Y_SIGN_MSK                   (0X02)
#define BNO055_REMAP_Y_SIGN_LEN                   (1)
#define BNO055_REMAP_Y_SIGN_REG                   BNO055_AXIS_MAP_SIGN_ADDR

#define BNO055_REMAP_X_SIGN_POS                   (2)
#define BNO055_REMAP_X_SIGN_MSK                   (0X04)
#define BNO055_REMAP_X_SIGN_LEN                   (1)
#define BNO055_REMAP_X_SIGN_REG                   BNO055_AXIS_MAP_SIGN_ADDR


#define BNO055_SIC_MATRIX_0_LSB_POS               (0)
#define BNO055_SIC_MATRIX_0_LSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_0_LSB_LEN               (8)
#define BNO055_SIC_MATRIX_0_LSB_REG               BNO055_SIC_MATRIX_0_LSB_ADDR

#define BNO055_SIC_MATRIX_0_MSB_POS               (0)
#define BNO055_SIC_MATRIX_0_MSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_0_MSB_LEN               (8)
#define BNO055_SIC_MATRIX_0_MSB_REG               BNO055_SIC_MATRIX_0_MSB_ADDR

#define BNO055_SIC_MATRIX_1_LSB_POS               (0)
#define BNO055_SIC_MATRIX_1_LSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_1_LSB_LEN               (8)
#define BNO055_SIC_MATRIX_1_LSB_REG               BNO055_SIC_MATRIX_1_LSB_ADDR

#define BNO055_SIC_MATRIX_1_MSB_POS               (0)
#define BNO055_SIC_MATRIX_1_MSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_1_MSB_LEN               (8)
#define BNO055_SIC_MATRIX_1_MSB_REG               BNO055_SIC_MATRIX_1_MSB_ADDR

#define BNO055_SIC_MATRIX_2_LSB_POS               (0)
#define BNO055_SIC_MATRIX_2_LSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_2_LSB_LEN               (8)
#define BNO055_SIC_MATRIX_2_LSB_REG               BNO055_SIC_MATRIX_2_LSB_ADDR

#define BNO055_SIC_MATRIX_2_MSB_POS               (0)
#define BNO055_SIC_MATRIX_2_MSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_2_MSB_LEN               (8)
#define BNO055_SIC_MATRIX_2_MSB_REG               BNO055_SIC_MATRIX_2_MSB_ADDR

#define BNO055_SIC_MATRIX_3_LSB_POS               (0)
#define BNO055_SIC_MATRIX_3_LSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_3_LSB_LEN               (8)
#define BNO055_SIC_MATRIX_3_LSB_REG               BNO055_SIC_MATRIX_3_LSB_ADDR

#define BNO055_SIC_MATRIX_3_MSB_POS               (0)
#define BNO055_SIC_MATRIX_3_MSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_3_MSB_LEN               (8)
#define BNO055_SIC_MATRIX_3_MSB_REG               BNO055_SIC_MATRIX_3_MSB_ADDR

#define BNO055_SIC_MATRIX_4_LSB_POS               (0)
#define BNO055_SIC_MATRIX_4_LSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_4_LSB_LEN               (8)
#define BNO055_SIC_MATRIX_4_LSB_REG               BNO055_SIC_MATRIX_4_LSB_ADDR

#define BNO055_SIC_MATRIX_4_MSB_POS               (0)
#define BNO055_SIC_MATRIX_4_MSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_4_MSB_LEN               (8)
#define BNO055_SIC_MATRIX_4_MSB_REG               BNO055_SIC_MATRIX_4_MSB_ADDR

#define BNO055_SIC_MATRIX_5_LSB_POS               (0)
#define BNO055_SIC_MATRIX_5_LSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_5_LSB_LEN               (8)
#define BNO055_SIC_MATRIX_5_LSB_REG               BNO055_SIC_MATRIX_5_LSB_ADDR

#define BNO055_SIC_MATRIX_5_MSB_POS               (0)
#define BNO055_SIC_MATRIX_5_MSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_5_MSB_LEN               (8)
#define BNO055_SIC_MATRIX_5_MSB_REG               BNO055_SIC_MATRIX_5_MSB_ADDR

#define BNO055_SIC_MATRIX_6_LSB_POS               (0)
#define BNO055_SIC_MATRIX_6_LSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_6_LSB_LEN               (8)
#define BNO055_SIC_MATRIX_6_LSB_REG               BNO055_SIC_MATRIX_6_LSB_ADDR

#define BNO055_SIC_MATRIX_6_MSB_POS               (0)
#define BNO055_SIC_MATRIX_6_MSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_6_MSB_LEN               (8)
#define BNO055_SIC_MATRIX_6_MSB_REG               BNO055_SIC_MATRIX_6_MSB_ADDR

#define BNO055_SIC_MATRIX_7_LSB_POS               (0)
#define BNO055_SIC_MATRIX_7_LSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_7_LSB_LEN               (8)
#define BNO055_SIC_MATRIX_7_LSB_REG               BNO055_SIC_MATRIX_7_LSB_ADDR

#define BNO055_SIC_MATRIX_7_MSB_POS               (0)
#define BNO055_SIC_MATRIX_7_MSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_7_MSB_LEN               (8)
#define BNO055_SIC_MATRIX_7_MSB_REG               BNO055_SIC_MATRIX_7_MSB_ADDR

#define BNO055_SIC_MATRIX_8_LSB_POS               (0)
#define BNO055_SIC_MATRIX_8_LSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_8_LSB_LEN               (8)
#define BNO055_SIC_MATRIX_8_LSB_REG               BNO055_SIC_MATRIX_8_LSB_ADDR

#define BNO055_SIC_MATRIX_8_MSB_POS               (0)
#define BNO055_SIC_MATRIX_8_MSB_MSK               (0XFF)
#define BNO055_SIC_MATRIX_8_MSB_LEN               (8)
#define BNO055_SIC_MATRIX_8_MSB_REG               BNO055_SIC_MATRIX_8_MSB_ADDR


#define BNO055_ACCEL_OFFSET_X_LSB_POS             (0)
#define BNO055_ACCEL_OFFSET_X_LSB_MSK             (0XFF)
#define BNO055_ACCEL_OFFSET_X_LSB_LEN             (8)
#define BNO055_ACCEL_OFFSET_X_LSB_REG             BNO055_ACCEL_OFFSET_X_LSB_ADDR

#define BNO055_ACCEL_OFFSET_X_MSB_POS             (0)
#define BNO055_ACCEL_OFFSET_X_MSB_MSK             (0XFF)
#define BNO055_ACCEL_OFFSET_X_MSB_LEN             (8)
#define BNO055_ACCEL_OFFSET_X_MSB_REG             BNO055_ACCEL_OFFSET_X_MSB_ADDR

#define BNO055_ACCEL_OFFSET_Y_LSB_POS             (0)
#define BNO055_ACCEL_OFFSET_Y_LSB_MSK             (0XFF)
#define BNO055_ACCEL_OFFSET_Y_LSB_LEN             (8)
#define BNO055_ACCEL_OFFSET_Y_LSB_REG             BNO055_ACCEL_OFFSET_Y_LSB_ADDR

#define BNO055_ACCEL_OFFSET_Y_MSB_POS             (0)
#define BNO055_ACCEL_OFFSET_Y_MSB_MSK             (0XFF)
#define BNO055_ACCEL_OFFSET_Y_MSB_LEN             (8)
#define BNO055_ACCEL_OFFSET_Y_MSB_REG             BNO055_ACCEL_OFFSET_Y_MSB_ADDR

#define BNO055_ACCEL_OFFSET_Z_LSB_POS             (0)
#define BNO055_ACCEL_OFFSET_Z_LSB_MSK             (0XFF)
#define BNO055_ACCEL_OFFSET_Z_LSB_LEN             (8)
#define BNO055_ACCEL_OFFSET_Z_LSB_REG             BNO055_ACCEL_OFFSET_Z_LSB_ADDR

#define BNO055_ACCEL_OFFSET_Z_MSB_POS             (0)
#define BNO055_ACCEL_OFFSET_Z_MSB_MSK             (0XFF)
#define BNO055_ACCEL_OFFSET_Z_MSB_LEN             (8)
#define BNO055_ACCEL_OFFSET_Z_MSB_REG             BNO055_ACCEL_OFFSET_Z_MSB_ADDR


#define BNO055_MAG_OFFSET_X_LSB_POS               (0)
#define BNO055_MAG_OFFSET_X_LSB_MSK               (0XFF)
#define BNO055_MAG_OFFSET_X_LSB_LEN               (8)
#define BNO055_MAG_OFFSET_X_LSB_REG               BNO055_MAG_OFFSET_X_LSB_ADDR

#define BNO055_MAG_OFFSET_X_MSB_POS               (0)
#define BNO055_MAG_OFFSET_X_MSB_MSK               (0XFF)
#define BNO055_MAG_OFFSET_X_MSB_LEN               (8)
#define BNO055_MAG_OFFSET_X_MSB_REG               BNO055_MAG_OFFSET_X_MSB_ADDR

#define BNO055_MAG_OFFSET_Y_LSB_POS               (0)
#define BNO055_MAG_OFFSET_Y_LSB_MSK               (0XFF)
#define BNO055_MAG_OFFSET_Y_LSB_LEN               (8)
#define BNO055_MAG_OFFSET_Y_LSB_REG               BNO055_MAG_OFFSET_Y_LSB_ADDR

#define BNO055_MAG_OFFSET_Y_MSB_POS               (0)
#define BNO055_MAG_OFFSET_Y_MSB_MSK               (0XFF)
#define BNO055_MAG_OFFSET_Y_MSB_LEN               (8)
#define BNO055_MAG_OFFSET_Y_MSB_REG               BNO055_MAG_OFFSET_Y_MSB_ADDR

#define BNO055_MAG_OFFSET_Z_LSB_POS               (0)
#define BNO055_MAG_OFFSET_Z_LSB_MSK               (0XFF)
#define BNO055_MAG_OFFSET_Z_LSB_LEN               (8)
#define BNO055_MAG_OFFSET_Z_LSB_REG               BNO055_MAG_OFFSET_Z_LSB_ADDR

#define BNO055_MAG_OFFSET_Z_MSB_POS               (0)
#define BNO055_MAG_OFFSET_Z_MSB_MSK               (0XFF)
#define BNO055_MAG_OFFSET_Z_MSB_LEN               (8)
#define BNO055_MAG_OFFSET_Z_MSB_REG               BNO055_MAG_OFFSET_Z_MSB_ADDR


#define BNO055_GYRO_OFFSET_X_LSB_POS              (0)
#define BNO055_GYRO_OFFSET_X_LSB_MSK              (0XFF)
#define BNO055_GYRO_OFFSET_X_LSB_LEN              (8)
#define BNO055_GYRO_OFFSET_X_LSB_REG              BNO055_GYRO_OFFSET_X_LSB_ADDR

#define BNO055_GYRO_OFFSET_X_MSB_POS              (0)
#define BNO055_GYRO_OFFSET_X_MSB_MSK              (0XFF)
#define BNO055_GYRO_OFFSET_X_MSB_LEN              (8)
#define BNO055_GYRO_OFFSET_X_MSB_REG              BNO055_GYRO_OFFSET_X_MSB_ADDR

#define BNO055_GYRO_OFFSET_Y_LSB_POS              (0)
#define BNO055_GYRO_OFFSET_Y_LSB_MSK              (0XFF)
#define BNO055_GYRO_OFFSET_Y_LSB_LEN              (8)
#define BNO055_GYRO_OFFSET_Y_LSB_REG              BNO055_GYRO_OFFSET_Y_LSB_ADDR

#define BNO055_GYRO_OFFSET_Y_MSB_POS              (0)
#define BNO055_GYRO_OFFSET_Y_MSB_MSK              (0XFF)
#define BNO055_GYRO_OFFSET_Y_MSB_LEN              (8)
#define BNO055_GYRO_OFFSET_Y_MSB_REG              BNO055_GYRO_OFFSET_Y_MSB_ADDR

#define BNO055_GYRO_OFFSET_Z_LSB_POS              (0)
#define BNO055_GYRO_OFFSET_Z_LSB_MSK              (0XFF)
#define BNO055_GYRO_OFFSET_Z_LSB_LEN              (8)
#define BNO055_GYRO_OFFSET_Z_LSB_REG              BNO055_GYRO_OFFSET_Z_LSB_ADDR

#define BNO055_GYRO_OFFSET_Z_MSB_POS              (0)
#define BNO055_GYRO_OFFSET_Z_MSB_MSK              (0XFF)
#define BNO055_GYRO_OFFSET_Z_MSB_LEN              (8)
#define BNO055_GYRO_OFFSET_Z_MSB_REG              BNO055_GYRO_OFFSET_Z_MSB_ADDR


#define BNO055_ACCEL_RADIUS_LSB_POS               (0)
#define BNO055_ACCEL_RADIUS_LSB_MSK               (0XFF)
#define BNO055_ACCEL_RADIUS_LSB_LEN               (8)
#define BNO055_ACCEL_RADIUS_LSB_REG               BNO055_ACCEL_RADIUS_LSB_ADDR

#define BNO055_ACCEL_RADIUS_MSB_POS               (0)
#define BNO055_ACCEL_RADIUS_MSB_MSK               (0XFF)
#define BNO055_ACCEL_RADIUS_MSB_LEN               (8)
#define BNO055_ACCEL_RADIUS_MSB_REG               BNO055_ACCEL_RADIUS_MSB_ADDR

#define BNO055_MAG_RADIUS_LSB_POS                 (0)
#define BNO055_MAG_RADIUS_LSB_MSK                 (0XFF)
#define BNO055_MAG_RADIUS_LSB_LEN                 (8)
#define BNO055_MAG_RADIUS_LSB_REG                 BNO055_MAG_RADIUS_LSB_ADDR

#define BNO055_MAG_RADIUS_MSB_POS                 (0)
#define BNO055_MAG_RADIUS_MSB_MSK                 (0XFF)
#define BNO055_MAG_RADIUS_MSB_LEN                 (8)
#define BNO055_MAG_RADIUS_MSB_REG                 BNO055_MAG_RADIUS_MSB_ADDR







#define BNO055_ACCEL_RANGE_POS                 (0)
#define BNO055_ACCEL_RANGE_MSK                 (0X03)
#define BNO055_ACCEL_RANGE_LEN                 (2)
#define BNO055_ACCEL_RANGE_REG                 BNO055_ACCEL_CONFIG_ADDR


#define BNO055_ACCEL_BW_POS                    (2)
#define BNO055_ACCEL_BW_MSK                    (0X1C)
#define BNO055_ACCEL_BW_LEN                    (3)
#define BNO055_ACCEL_BW_REG                    BNO055_ACCEL_CONFIG_ADDR


#define BNO055_ACCEL_POWER_MODE_POS            (5)
#define BNO055_ACCEL_POWER_MODE_MSK            (0XE0)
#define BNO055_ACCEL_POWER_MODE_LEN            (3)
#define BNO055_ACCEL_POWER_MODE_REG            BNO055_ACCEL_CONFIG_ADDR


#define BNO055_MAG_DATA_OUTPUT_RATE_POS        (0)
#define BNO055_MAG_DATA_OUTPUT_RATE_MSK        (0X07)
#define BNO055_MAG_DATA_OUTPUT_RATE_LEN        (3)
#define BNO055_MAG_DATA_OUTPUT_RATE_REG        BNO055_MAG_CONFIG_ADDR


#define BNO055_MAG_OPERATION_MODE_POS          (3)
#define BNO055_MAG_OPERATION_MODE_MSK          (0X18)
#define BNO055_MAG_OPERATION_MODE_LEN          (2)
#define BNO055_MAG_OPERATION_MODE_REG          BNO055_MAG_CONFIG_ADDR


#define BNO055_MAG_POWER_MODE_POS              (5)
#define BNO055_MAG_POWER_MODE_MSK              (0X60)
#define BNO055_MAG_POWER_MODE_LEN              (2)
#define BNO055_MAG_POWER_MODE_REG              BNO055_MAG_CONFIG_ADDR


#define BNO055_GYRO_RANGE_POS                  (0)
#define BNO055_GYRO_RANGE_MSK                  (0X07)
#define BNO055_GYRO_RANGE_LEN                  (3)
#define BNO055_GYRO_RANGE_REG                  BNO055_GYRO_CONFIG_ADDR


#define BNO055_GYRO_BW_POS                     (3)
#define BNO055_GYRO_BW_MSK                     (0X38)
#define BNO055_GYRO_BW_LEN                     (3)
#define BNO055_GYRO_BW_REG                     BNO055_GYRO_CONFIG_ADDR


#define BNO055_GYRO_POWER_MODE_POS             (0)
#define BNO055_GYRO_POWER_MODE_MSK             (0X07)
#define BNO055_GYRO_POWER_MODE_LEN             (3)
#define BNO055_GYRO_POWER_MODE_REG             BNO055_GYRO_MODE_CONFIG_ADDR



#define BNO055_ACCEL_SLEEP_MODE_POS            (0)
#define BNO055_ACCEL_SLEEP_MODE_MSK            (0X01)
#define BNO055_ACCEL_SLEEP_MODE_LEN            (1)
#define BNO055_ACCEL_SLEEP_MODE_REG            BNO055_ACCEL_SLEEP_CONFIG_ADDR


#define BNO055_ACCEL_SLEEP_DURN_POS            (1)
#define BNO055_ACCEL_SLEEP_DURN_MSK            (0X1E)
#define BNO055_ACCEL_SLEEP_DURN_LEN            (4)
#define BNO055_ACCEL_SLEEP_DURN_REG            BNO055_ACCEL_SLEEP_CONFIG_ADDR


#define BNO055_GYRO_SLEEP_DURN_POS             (0)
#define BNO055_GYRO_SLEEP_DURN_MSK             (0X07)
#define BNO055_GYRO_SLEEP_DURN_LEN             (3)
#define BNO055_GYRO_SLEEP_DURN_REG             BNO055_GYRO_SLEEP_CONFIG_ADDR


#define BNO055_GYRO_AUTO_SLEEP_DURN_POS        (3)
#define BNO055_GYRO_AUTO_SLEEP_DURN_MSK        (0X38)
#define BNO055_GYRO_AUTO_SLEEP_DURN_LEN        (3)
#define BNO055_GYRO_AUTO_SLEEP_DURN_REG        BNO055_GYRO_SLEEP_CONFIG_ADDR


#define BNO055_MAG_SLEEP_MODE_POS              (0)
#define BNO055_MAG_SLEEP_MODE_MSK              (0X01)
#define BNO055_MAG_SLEEP_MODE_LEN              (1)
#define BNO055_MAG_SLEEP_MODE_REG              BNO055_MAG_SLEEP_CONFIG_ADDR


#define BNO055_MAG_SLEEP_DURN_POS              (1)
#define BNO055_MAG_SLEEP_DURN_MSK              (0X1E)
#define BNO055_MAG_SLEEP_DURN_LEN              (4)
#define BNO055_MAG_SLEEP_DURN_REG              BNO055_MAG_SLEEP_CONFIG_ADDR



#define BNO055_GYRO_ANY_MOTION_INTR_MASK_POS   (2)
#define BNO055_GYRO_ANY_MOTION_INTR_MASK_MSK   (0X04)
#define BNO055_GYRO_ANY_MOTION_INTR_MASK_LEN   (1)
#define BNO055_GYRO_ANY_MOTION_INTR_MASK_REG   BNO055_INT_MASK_ADDR


#define BNO055_GYRO_HIGHRATE_INTR_MASK_POS     (3)
#define BNO055_GYRO_HIGHRATE_INTR_MASK_MSK     (0X08)
#define BNO055_GYRO_HIGHRATE_INTR_MASK_LEN     (1)
#define BNO055_GYRO_HIGHRATE_INTR_MASK_REG     BNO055_INT_MASK_ADDR


#define BNO055_ACCEL_HIGH_G_INTR_MASK_POS      (5)
#define BNO055_ACCEL_HIGH_G_INTR_MASK_MSK      (0X20)
#define BNO055_ACCEL_HIGH_G_INTR_MASK_LEN      (1)
#define BNO055_ACCEL_HIGH_G_INTR_MASK_REG      BNO055_INT_MASK_ADDR


#define BNO055_ACCEL_ANY_MOTION_INTR_MASK_POS  (6)
#define BNO055_ACCEL_ANY_MOTION_INTR_MASK_MSK  (0X40)
#define BNO055_ACCEL_ANY_MOTION_INTR_MASK_LEN  (1)
#define BNO055_ACCEL_ANY_MOTION_INTR_MASK_REG  BNO055_INT_MASK_ADDR


#define BNO055_ACCEL_NO_MOTION_INTR_MASK_POS   (7)
#define BNO055_ACCEL_NO_MOTION_INTR_MASK_MSK   (0X80)
#define BNO055_ACCEL_NO_MOTION_INTR_MASK_LEN   (1)
#define BNO055_ACCEL_NO_MOTION_INTR_MASK_REG   BNO055_INT_MASK_ADDR


#define BNO055_GYRO_ANY_MOTION_INTR_POS        (2)
#define BNO055_GYRO_ANY_MOTION_INTR_MSK        (0X04)
#define BNO055_GYRO_ANY_MOTION_INTR_LEN        (1)
#define BNO055_GYRO_ANY_MOTION_INTR_REG        BNO055_INT_ADDR


#define BNO055_GYRO_HIGHRATE_INTR_POS          (3)
#define BNO055_GYRO_HIGHRATE_INTR_MSK          (0X08)
#define BNO055_GYRO_HIGHRATE_INTR_LEN          (1)
#define BNO055_GYRO_HIGHRATE_INTR_REG          BNO055_INT_ADDR


#define BNO055_ACCEL_HIGH_G_INTR_POS           (5)
#define BNO055_ACCEL_HIGH_G_INTR_MSK           (0X20)
#define BNO055_ACCEL_HIGH_G_INTR_LEN           (1)
#define BNO055_ACCEL_HIGH_G_INTR_REG           BNO055_INT_ADDR


#define BNO055_ACCEL_ANY_MOTION_INTR_POS       (6)
#define BNO055_ACCEL_ANY_MOTION_INTR_MSK       (0X40)
#define BNO055_ACCEL_ANY_MOTION_INTR_LEN       (1)
#define BNO055_ACCEL_ANY_MOTION_INTR_REG       BNO055_INT_ADDR


#define BNO055_ACCEL_NO_MOTION_INTR_POS        (7)
#define BNO055_ACCEL_NO_MOTION_INTR_MSK        (0X80)
#define BNO055_ACCEL_NO_MOTION_INTR_LEN        (1)
#define BNO055_ACCEL_NO_MOTION_INTR_REG        BNO055_INT_ADDR


#define BNO055_ACCEL_ANY_MOTION_THRES_POS      (0)
#define BNO055_ACCEL_ANY_MOTION_THRES_MSK      (0XFF)
#define BNO055_ACCEL_ANY_MOTION_THRES_LEN      (8)
#define BNO055_ACCEL_ANY_MOTION_THRES_REG      BNO055_ACCEL_ANY_MOTION_THRES_ADDR


#define BNO055_ACCEL_ANY_MOTION_DURN_SET_POS   (0)
#define BNO055_ACCEL_ANY_MOTION_DURN_SET_MSK   (0X03)
#define BNO055_ACCEL_ANY_MOTION_DURN_SET_LEN   (2)
#define BNO055_ACCEL_ANY_MOTION_DURN_SET_REG   BNO055_ACCEL_INTR_SETTINGS_ADDR


#define BNO055_ACCEL_ANY_MOTION_X_AXIS_POS     (2)
#define BNO055_ACCEL_ANY_MOTION_X_AXIS_MSK     (0X04)
#define BNO055_ACCEL_ANY_MOTION_X_AXIS_LEN     (1)
#define BNO055_ACCEL_ANY_MOTION_X_AXIS_REG     BNO055_ACCEL_INTR_SETTINGS_ADDR

#define BNO055_ACCEL_ANY_MOTION_Y_AXIS_POS     (3)
#define BNO055_ACCEL_ANY_MOTION_Y_AXIS_MSK     (0X08)
#define BNO055_ACCEL_ANY_MOTION_Y_AXIS_LEN     (1)
#define BNO055_ACCEL_ANY_MOTION_Y_AXIS_REG     BNO055_ACCEL_INTR_SETTINGS_ADDR

#define BNO055_ACCEL_ANY_MOTION_Z_AXIS_POS     (4)
#define BNO055_ACCEL_ANY_MOTION_Z_AXIS_MSK     (0X10)
#define BNO055_ACCEL_ANY_MOTION_Z_AXIS_LEN     (1)
#define BNO055_ACCEL_ANY_MOTION_Z_AXIS_REG     BNO055_ACCEL_INTR_SETTINGS_ADDR


#define BNO055_ACCEL_HIGH_G_X_AXIS_POS         (5)
#define BNO055_ACCEL_HIGH_G_X_AXIS_MSK         (0X20)
#define BNO055_ACCEL_HIGH_G_X_AXIS_LEN         (1)
#define BNO055_ACCEL_HIGH_G_X_AXIS_REG         BNO055_ACCEL_INTR_SETTINGS_ADDR

#define BNO055_ACCEL_HIGH_G_Y_AXIS_POS         (6)
#define BNO055_ACCEL_HIGH_G_Y_AXIS_MSK         (0X40)
#define BNO055_ACCEL_HIGH_G_Y_AXIS_LEN         (1)
#define BNO055_ACCEL_HIGH_G_Y_AXIS_REG         BNO055_ACCEL_INTR_SETTINGS_ADDR

#define BNO055_ACCEL_HIGH_G_Z_AXIS_POS         (7)
#define BNO055_ACCEL_HIGH_G_Z_AXIS_MSK         (0X80)
#define BNO055_ACCEL_HIGH_G_Z_AXIS_LEN         (1)
#define BNO055_ACCEL_HIGH_G_Z_AXIS_REG         BNO055_ACCEL_INTR_SETTINGS_ADDR


#define BNO055_ACCEL_HIGH_G_DURN_POS           (0)
#define BNO055_ACCEL_HIGH_G_DURN_MSK           (0XFF)
#define BNO055_ACCEL_HIGH_G_DURN_LEN           (8)
#define BNO055_ACCEL_HIGH_G_DURN_REG           BNO055_ACCEL_HIGH_G_DURN_ADDR


#define BNO055_ACCEL_HIGH_G_THRES_POS          (0)
#define BNO055_ACCEL_HIGH_G_THRES_MSK          (0XFF)
#define BNO055_ACCEL_HIGH_G_THRES_LEN          (8)
#define BNO055_ACCEL_HIGH_G_THRES_REG          BNO055_ACCEL_HIGH_G_THRES_ADDR


#define BNO055_ACCEL_SLOW_NO_MOTION_THRES_POS  (0)
#define BNO055_ACCEL_SLOW_NO_MOTION_THRES_MSK  (0XFF)
#define BNO055_ACCEL_SLOW_NO_MOTION_THRES_LEN  (8)
#define BNO055_ACCEL_SLOW_NO_MOTION_THRES_REG   \
    BNO055_ACCEL_NO_MOTION_THRES_ADDR


#define BNO055_ACCEL_SLOW_NO_MOTION_ENABLE_POS (0)
#define BNO055_ACCEL_SLOW_NO_MOTION_ENABLE_MSK (0X01)
#define BNO055_ACCEL_SLOW_NO_MOTION_ENABLE_LEN (1)
#define BNO055_ACCEL_SLOW_NO_MOTION_ENABLE_REG BNO055_ACCEL_NO_MOTION_SET_ADDR


#define BNO055_ACCEL_SLOW_NO_MOTION_DURN_POS   (1)
#define BNO055_ACCEL_SLOW_NO_MOTION_DURN_MSK   (0X7E)
#define BNO055_ACCEL_SLOW_NO_MOTION_DURN_LEN   (6)
#define BNO055_ACCEL_SLOW_NO_MOTION_DURN_REG   BNO055_ACCEL_NO_MOTION_SET_ADDR



#define BNO055_GYRO_ANY_MOTION_X_AXIS_POS      (0)
#define BNO055_GYRO_ANY_MOTION_X_AXIS_MSK      (0X01)
#define BNO055_GYRO_ANY_MOTION_X_AXIS_LEN      (1)
#define BNO055_GYRO_ANY_MOTION_X_AXIS_REG      BNO055_GYRO_INTR_SETING_ADDR

#define BNO055_GYRO_ANY_MOTION_Y_AXIS_POS      (1)
#define BNO055_GYRO_ANY_MOTION_Y_AXIS_MSK      (0X02)
#define BNO055_GYRO_ANY_MOTION_Y_AXIS_LEN      (1)
#define BNO055_GYRO_ANY_MOTION_Y_AXIS_REG      BNO055_GYRO_INTR_SETING_ADDR

#define BNO055_GYRO_ANY_MOTION_Z_AXIS_POS      (2)
#define BNO055_GYRO_ANY_MOTION_Z_AXIS_MSK      (0X04)
#define BNO055_GYRO_ANY_MOTION_Z_AXIS_LEN      (1)
#define BNO055_GYRO_ANY_MOTION_Z_AXIS_REG      BNO055_GYRO_INTR_SETING_ADDR


#define BNO055_GYRO_HIGHRATE_X_AXIS_POS        (3)
#define BNO055_GYRO_HIGHRATE_X_AXIS_MSK        (0X08)
#define BNO055_GYRO_HIGHRATE_X_AXIS_LEN        (1)
#define BNO055_GYRO_HIGHRATE_X_AXIS_REG        BNO055_GYRO_INTR_SETING_ADDR

#define BNO055_GYRO_HIGHRATE_Y_AXIS_POS        (4)
#define BNO055_GYRO_HIGHRATE_Y_AXIS_MSK        (0X10)
#define BNO055_GYRO_HIGHRATE_Y_AXIS_LEN        (1)
#define BNO055_GYRO_HIGHRATE_Y_AXIS_REG        BNO055_GYRO_INTR_SETING_ADDR

#define BNO055_GYRO_HIGHRATE_Z_AXIS_POS        (5)
#define BNO055_GYRO_HIGHRATE_Z_AXIS_MSK        (0X20)
#define BNO055_GYRO_HIGHRATE_Z_AXIS_LEN        (1)
#define BNO055_GYRO_HIGHRATE_Z_AXIS_REG        BNO055_GYRO_INTR_SETING_ADDR


#define BNO055_GYRO_ANY_MOTION_FILTER_POS      (6)
#define BNO055_GYRO_ANY_MOTION_FILTER_MSK      (0X40)
#define BNO055_GYRO_ANY_MOTION_FILTER_LEN      (1)
#define BNO055_GYRO_ANY_MOTION_FILTER_REG      BNO055_GYRO_INTR_SETING_ADDR

#define BNO055_GYRO_HIGHRATE_FILTER_POS        (7)
#define BNO055_GYRO_HIGHRATE_FILTER_MSK        (0X80)
#define BNO055_GYRO_HIGHRATE_FILTER_LEN        (1)
#define BNO055_GYRO_HIGHRATE_FILTER_REG        BNO055_GYRO_INTR_SETING_ADDR


#define BNO055_GYRO_HIGHRATE_X_THRES_POS       (0)
#define BNO055_GYRO_HIGHRATE_X_THRES_MSK       (0X1F)
#define BNO055_GYRO_HIGHRATE_X_THRES_LEN       (5)
#define BNO055_GYRO_HIGHRATE_X_THRES_REG       BNO055_GYRO_HIGHRATE_X_SET_ADDR

#define BNO055_GYRO_HIGHRATE_X_HYST_POS        (5)
#define BNO055_GYRO_HIGHRATE_X_HYST_MSK        (0X60)
#define BNO055_GYRO_HIGHRATE_X_HYST_LEN        (2)
#define BNO055_GYRO_HIGHRATE_X_HYST_REG        BNO055_GYRO_HIGHRATE_X_SET_ADDR

#define BNO055_GYRO_HIGHRATE_X_DURN_POS        (0)
#define BNO055_GYRO_HIGHRATE_X_DURN_MSK        (0XFF)
#define BNO055_GYRO_HIGHRATE_X_DURN_LEN        (8)
#define BNO055_GYRO_HIGHRATE_X_DURN_REG        BNO055_GYRO_DURN_X_ADDR


#define BNO055_GYRO_HIGHRATE_Y_THRES_POS       (0)
#define BNO055_GYRO_HIGHRATE_Y_THRES_MSK       (0X1F)
#define BNO055_GYRO_HIGHRATE_Y_THRES_LEN       (5)
#define BNO055_GYRO_HIGHRATE_Y_THRES_REG       BNO055_GYRO_HIGHRATE_Y_SET_ADDR

#define BNO055_GYRO_HIGHRATE_Y_HYST_POS        (5)
#define BNO055_GYRO_HIGHRATE_Y_HYST_MSK        (0X60)
#define BNO055_GYRO_HIGHRATE_Y_HYST_LEN        (2)
#define BNO055_GYRO_HIGHRATE_Y_HYST_REG        BNO055_GYRO_HIGHRATE_Y_SET_ADDR

#define BNO055_GYRO_HIGHRATE_Y_DURN_POS        (0)
#define BNO055_GYRO_HIGHRATE_Y_DURN_MSK        (0XFF)
#define BNO055_GYRO_HIGHRATE_Y_DURN_LEN        (8)
#define BNO055_GYRO_HIGHRATE_Y_DURN_REG        BNO055_GYRO_DURN_Y_ADDR


#define BNO055_GYRO_HIGHRATE_Z_THRES_POS       (0)
#define BNO055_GYRO_HIGHRATE_Z_THRES_MSK       (0X1F)
#define BNO055_GYRO_HIGHRATE_Z_THRES_LEN       (5)
#define BNO055_GYRO_HIGHRATE_Z_THRES_REG       BNO055_GYRO_HIGHRATE_Z_SET_ADDR

#define BNO055_GYRO_HIGHRATE_Z_HYST_POS        (5)
#define BNO055_GYRO_HIGHRATE_Z_HYST_MSK        (0X60)
#define BNO055_GYRO_HIGHRATE_Z_HYST_LEN        (2)
#define BNO055_GYRO_HIGHRATE_Z_HYST_REG        BNO055_GYRO_HIGHRATE_Z_SET_ADDR

#define BNO055_GYRO_HIGHRATE_Z_DURN_POS        (0)
#define BNO055_GYRO_HIGHRATE_Z_DURN_MSK        (0XFF)
#define BNO055_GYRO_HIGHRATE_Z_DURN_LEN        (8)
#define BNO055_GYRO_HIGHRATE_Z_DURN_REG        (BNO055_GYRO_DURN_Z_ADDR)


#define BNO055_GYRO_ANY_MOTION_THRES_POS       (0)
#define BNO055_GYRO_ANY_MOTION_THRES_MSK       (0X7F)
#define BNO055_GYRO_ANY_MOTION_THRES_LEN       (7)
#define BNO055_GYRO_ANY_MOTION_THRES_REG    \
    BNO055_GYRO_ANY_MOTION_THRES_ADDR


#define BNO055_GYRO_SLOPE_SAMPLES_POS          (0)
#define BNO055_GYRO_SLOPE_SAMPLES_MSK          (0X03)
#define BNO055_GYRO_SLOPE_SAMPLES_LEN          (2)
#define BNO055_GYRO_SLOPE_SAMPLES_REG          BNO055_GYRO_ANY_MOTION_SET_ADDR


#define BNO055_GYRO_AWAKE_DURN_POS             (2)
#define BNO055_GYRO_AWAKE_DURN_MSK             (0X0C)
#define BNO055_GYRO_AWAKE_DURN_LEN             (2)
#define BNO055_GYRO_AWAKE_DURN_REG             BNO055_GYRO_ANY_MOTION_SET_ADDR

#define BNO055_GET_BITSLICE(regvar, bitname) \
    ((regvar & bitname##_MSK) >> bitname##_POS)

#define BNO055_SET_BITSLICE(regvar, bitname, val) \
    ((regvar & ~bitname##_MSK) | ((val << bitname##_POS) & bitname##_MSK))


#define BNO055_ID (0xA0)

#define BNO055_RETURN_FUNCTION_TYPE         s8

typedef struct {
  int16_t accel_offset_x; 
  int16_t accel_offset_y; 
  int16_t accel_offset_z; 

  int16_t mag_offset_x; 
  int16_t mag_offset_y; 
  int16_t mag_offset_z; 

  int16_t gyro_offset_x; 
  int16_t gyro_offset_y; 
  int16_t gyro_offset_z; 

  int16_t accel_radius; 

  int16_t mag_radius; 
} bno055_offsets_t;


typedef enum {
  OPERATION_MODE_CONFIG = 0X00,
  OPERATION_MODE_ACCONLY = 0X01,
  OPERATION_MODE_MAGONLY = 0X02,
  OPERATION_MODE_GYRONLY = 0X03,
  OPERATION_MODE_ACCMAG = 0X04,
  OPERATION_MODE_ACCGYRO = 0X05,
  OPERATION_MODE_MAGGYRO = 0X06,
  OPERATION_MODE_AMG = 0X07,
  OPERATION_MODE_IMUPLUS = 0X08,
  OPERATION_MODE_COMPASS = 0X09,
  OPERATION_MODE_M4G = 0X0A,
  OPERATION_MODE_NDOF_FMC_OFF = 0X0B,
  OPERATION_MODE_NDOF = 0X0C
} bno055_opmode_t;

class BNO055 {
public:
	
	typedef enum {
		
		BNO055_PAGE_ID_ADDR = 0X07,

		
		BNO055_CHIP_ID_ADDR = 0x00,
		BNO055_ACCEL_REV_ID_ADDR = 0x01,
		BNO055_MAG_REV_ID_ADDR = 0x02,
		BNO055_GYRO_REV_ID_ADDR = 0x03,
		BNO055_SW_REV_ID_LSB_ADDR = 0x04,
		BNO055_SW_REV_ID_MSB_ADDR = 0x05,
		BNO055_BL_REV_ID_ADDR = 0X06,

		
		BNO055_ACCEL_DATA_X_LSB_ADDR = 0X08,
		BNO055_ACCEL_DATA_X_MSB_ADDR = 0X09,
		BNO055_ACCEL_DATA_Y_LSB_ADDR = 0X0A,
		BNO055_ACCEL_DATA_Y_MSB_ADDR = 0X0B,
		BNO055_ACCEL_DATA_Z_LSB_ADDR = 0X0C,
		BNO055_ACCEL_DATA_Z_MSB_ADDR = 0X0D,

		
		BNO055_MAG_DATA_X_LSB_ADDR = 0X0E,
		BNO055_MAG_DATA_X_MSB_ADDR = 0X0F,
		BNO055_MAG_DATA_Y_LSB_ADDR = 0X10,
		BNO055_MAG_DATA_Y_MSB_ADDR = 0X11,
		BNO055_MAG_DATA_Z_LSB_ADDR = 0X12,
		BNO055_MAG_DATA_Z_MSB_ADDR = 0X13,

		
		BNO055_GYRO_DATA_X_LSB_ADDR = 0X14,
		BNO055_GYRO_DATA_X_MSB_ADDR = 0X15,
		BNO055_GYRO_DATA_Y_LSB_ADDR = 0X16,
		BNO055_GYRO_DATA_Y_MSB_ADDR = 0X17,
		BNO055_GYRO_DATA_Z_LSB_ADDR = 0X18,
		BNO055_GYRO_DATA_Z_MSB_ADDR = 0X19,

		
		BNO055_EULER_H_LSB_ADDR = 0X1A,
		BNO055_EULER_H_MSB_ADDR = 0X1B,
		BNO055_EULER_R_LSB_ADDR = 0X1C,
		BNO055_EULER_R_MSB_ADDR = 0X1D,
		BNO055_EULER_P_LSB_ADDR = 0X1E,
		BNO055_EULER_P_MSB_ADDR = 0X1F,

		
		BNO055_QUATERNION_DATA_W_LSB_ADDR = 0X20,
		BNO055_QUATERNION_DATA_W_MSB_ADDR = 0X21,
		BNO055_QUATERNION_DATA_X_LSB_ADDR = 0X22,
		BNO055_QUATERNION_DATA_X_MSB_ADDR = 0X23,
		BNO055_QUATERNION_DATA_Y_LSB_ADDR = 0X24,
		BNO055_QUATERNION_DATA_Y_MSB_ADDR = 0X25,
		BNO055_QUATERNION_DATA_Z_LSB_ADDR = 0X26,
		BNO055_QUATERNION_DATA_Z_MSB_ADDR = 0X27,

		
		BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR = 0X28,
		BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR = 0X29,
		BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR = 0X2A,
		BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR = 0X2B,
		BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR = 0X2C,
		BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR = 0X2D,

		
		BNO055_GRAVITY_DATA_X_LSB_ADDR = 0X2E,
		BNO055_GRAVITY_DATA_X_MSB_ADDR = 0X2F,
		BNO055_GRAVITY_DATA_Y_LSB_ADDR = 0X30,
		BNO055_GRAVITY_DATA_Y_MSB_ADDR = 0X31,
		BNO055_GRAVITY_DATA_Z_LSB_ADDR = 0X32,
		BNO055_GRAVITY_DATA_Z_MSB_ADDR = 0X33,

		
		BNO055_TEMP_ADDR = 0X34,

		
		BNO055_CALIB_STAT_ADDR = 0X35,
		BNO055_SELFTEST_RESULT_ADDR = 0X36,
		BNO055_INTR_STAT_ADDR = 0X37,

		BNO055_SYS_CLK_STAT_ADDR = 0X38,
		BNO055_SYS_STAT_ADDR = 0X39,
		BNO055_SYS_ERR_ADDR = 0X3A,

		
		BNO055_UNIT_SEL_ADDR = 0X3B,

		
		BNO055_OPR_MODE_ADDR = 0X3D,
		BNO055_PWR_MODE_ADDR = 0X3E,

		BNO055_SYS_TRIGGER_ADDR = 0X3F,
		BNO055_TEMP_SOURCE_ADDR = 0X40,

		
		BNO055_AXIS_MAP_CONFIG_ADDR = 0X41,
		BNO055_AXIS_MAP_SIGN_ADDR = 0X42,

		
		BNO055_SIC_MATRIX_0_LSB_ADDR = 0X43,
		BNO055_SIC_MATRIX_0_MSB_ADDR = 0X44,
		BNO055_SIC_MATRIX_1_LSB_ADDR = 0X45,
		BNO055_SIC_MATRIX_1_MSB_ADDR = 0X46,
		BNO055_SIC_MATRIX_2_LSB_ADDR = 0X47,
		BNO055_SIC_MATRIX_2_MSB_ADDR = 0X48,
		BNO055_SIC_MATRIX_3_LSB_ADDR = 0X49,
		BNO055_SIC_MATRIX_3_MSB_ADDR = 0X4A,
		BNO055_SIC_MATRIX_4_LSB_ADDR = 0X4B,
		BNO055_SIC_MATRIX_4_MSB_ADDR = 0X4C,
		BNO055_SIC_MATRIX_5_LSB_ADDR = 0X4D,
		BNO055_SIC_MATRIX_5_MSB_ADDR = 0X4E,
		BNO055_SIC_MATRIX_6_LSB_ADDR = 0X4F,
		BNO055_SIC_MATRIX_6_MSB_ADDR = 0X50,
		BNO055_SIC_MATRIX_7_LSB_ADDR = 0X51,
		BNO055_SIC_MATRIX_7_MSB_ADDR = 0X52,
		BNO055_SIC_MATRIX_8_LSB_ADDR = 0X53,
		BNO055_SIC_MATRIX_8_MSB_ADDR = 0X54,

		
		ACCEL_OFFSET_X_LSB_ADDR = 0X55,
		ACCEL_OFFSET_X_MSB_ADDR = 0X56,
		ACCEL_OFFSET_Y_LSB_ADDR = 0X57,
		ACCEL_OFFSET_Y_MSB_ADDR = 0X58,
		ACCEL_OFFSET_Z_LSB_ADDR = 0X59,
		ACCEL_OFFSET_Z_MSB_ADDR = 0X5A,

		
		MAG_OFFSET_X_LSB_ADDR = 0X5B,
		MAG_OFFSET_X_MSB_ADDR = 0X5C,
		MAG_OFFSET_Y_LSB_ADDR = 0X5D,
		MAG_OFFSET_Y_MSB_ADDR = 0X5E,
		MAG_OFFSET_Z_LSB_ADDR = 0X5F,
		MAG_OFFSET_Z_MSB_ADDR = 0X60,

		
		GYRO_OFFSET_X_LSB_ADDR = 0X61,
		GYRO_OFFSET_X_MSB_ADDR = 0X62,
		GYRO_OFFSET_Y_LSB_ADDR = 0X63,
		GYRO_OFFSET_Y_MSB_ADDR = 0X64,
		GYRO_OFFSET_Z_LSB_ADDR = 0X65,
		GYRO_OFFSET_Z_MSB_ADDR = 0X66,

		
		ACCEL_RADIUS_LSB_ADDR = 0X67,
		ACCEL_RADIUS_MSB_ADDR = 0X68,
		MAG_RADIUS_LSB_ADDR = 0X69,
		MAG_RADIUS_MSB_ADDR = 0X6A
	} bno055_reg_t;

	  
	typedef enum {
	  POWER_MODE_NORMAL = 0X00,
	  POWER_MODE_LOWPOWER = 0X01,
	  POWER_MODE_SUSPEND = 0X02
	} bno055_powermode_t;

	
	typedef enum {
	  REMAP_CONFIG_P0 = 0x21,
	  REMAP_CONFIG_P1 = 0x24, 
	  REMAP_CONFIG_P2 = 0x24,
	  REMAP_CONFIG_P3 = 0x21,
	  REMAP_CONFIG_P4 = 0x24,
	  REMAP_CONFIG_P5 = 0x21,
	  REMAP_CONFIG_P6 = 0x21,
	  REMAP_CONFIG_P7 = 0x24
	} bno055_axis_remap_config_t;

	
	typedef enum {
	  REMAP_SIGN_P0 = 0x04,
	  REMAP_SIGN_P1 = 0x00, 
	  REMAP_SIGN_P2 = 0x06,
	  REMAP_SIGN_P3 = 0x02,
	  REMAP_SIGN_P4 = 0x03,
	  REMAP_SIGN_P5 = 0x01,
	  REMAP_SIGN_P6 = 0x07,
	  REMAP_SIGN_P7 = 0x05
	} bno055_axis_remap_sign_t;

	
	typedef enum {
		VECTOR_ACCELEROMETER = BNO055_ACCEL_DATA_X_LSB_ADDR,
		VECTOR_MAGNETOMETER = BNO055_MAG_DATA_X_LSB_ADDR,
		VECTOR_GYROSCOPE = BNO055_GYRO_DATA_X_LSB_ADDR,
		VECTOR_EULER = BNO055_EULER_H_LSB_ADDR,
		VECTOR_LINEARACCEL = BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR,
		VECTOR_GRAVITY = BNO055_GRAVITY_DATA_X_LSB_ADDR
	} vector_type_t;

BNO055(u16 IIC_DEVICE_ID, u16 SCLK_RATE, u8 IIC_ADDR);

bool begin(bno055_opmode_t mode = OPERATION_MODE_NDOF);

void setMode(bno055_opmode_t mode);

bno055_opmode_t getMode();

imu::Vector<3> getVector(vector_type_t vector_type);
imu::Quaternion getQuat();
u8 getTemp();


int bno055_read_accel_x(s16 *accel_x_s16);

u8 getCHIP_ID();

private:
	uint8_t  read8(bno055_reg_t);
	int readLen(bno055_reg_t, uint8_t  *buffer, uint8_t len);
	int write8(bno055_reg_t, uint8_t  value);

	I2CDevice *i2c_dev = NULL; 
	bno055_opmode_t _mode;
};

#endif 
