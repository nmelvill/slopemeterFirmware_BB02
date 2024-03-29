#ifndef REGISTERS_H
#define REGISTERS_H

#include "Arduino.h"

//ICM20948 Registers
#define WHO_AM_I 0x00
#define USR_CTRL 0x03
#define LP_CONFIG 0x05
#define PWR_MGMT_1 0x06
#define PWR_MGMT_2 0x07
#define REG_BANK_SEL 0x7F
#define INT_PIN_CFG 0x0F
#define  ODR_ALIGN_EN 0x09
#define GYRO_SMPLRT_DIV 0x00
#define GYRO_CONFIG_1 0x01
#define GYRO_CONFIG_2 0x02
#define ACCEL_SMPLRT_DIV_1 0x10
#define ACCEL_SMPLRT_DIV_2 0x11
#define ACCEL_CONFIG_1 0x14
#define ACCEL_CONFIG_2 0x15



//AK09916 Registers
#define CNTL2 0x31
#define CNTL3 0x32
#define HXL 0x11
#define ST1 0x10
#define ST2 0x18

#endif