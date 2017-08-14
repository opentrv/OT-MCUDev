/*
 * i2cspmconfig.h
 *
 *  Created on: 11 Aug 2017
 *      Author: denzo
 */

#ifndef SRC_MY_I2CSPMCONFIG_H_
#define SRC_MY_I2CSPMCONFIG_H_

/***************************************************************************//**
 * @file
 * @brief I2C simple poll-based master mode driver for the DK/STK.
 * @version 5.2.2
 *******************************************************************************
 * # License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
extern "C" {
#include "em_gpio.h"
#include "em_i2c.h"
}
/***************************************************************************//**
 * @defgroup kitdrv Kit Drivers
 * @brief Kit support and drivers
 * @details
 *  Drivers and support modules for board components such as displays, sensors and
 *  memories on EFM32, EZR32 and EFR32 kits.
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup I2CSPM
 * @{
 ******************************************************************************/

/*******************************************************************************
 ********************************   STRUCTS   **********************************
 ******************************************************************************/
/** I2C driver instance initialization structure.
    This data structure contains a number of I2C configuration options
    required for driver instance initialization.
    This struct is passed to @ref I2CSPM_Init() when initializing a I2CSPM
    instance. */
typedef struct {
  I2C_TypeDef           *port;          /**< Peripheral port */
  GPIO_Port_TypeDef     sclPort;        /**< SCL pin port number */
  uint8_t               sclPin;         /**< SCL pin number */
  GPIO_Port_TypeDef     sdaPort;        /**< SDA pin port number */
  uint8_t               sdaPin;         /**< SDA pin number */
#if defined (_I2C_ROUTELOC0_MASK)
  uint8_t               portLocationScl; /**< Port location of SCL signal */
  uint8_t               portLocationSda; /**< Port location of SDA signal */
#else
  uint8_t               portLocation;   /**< Port location */
#endif
  uint32_t              i2cRefFreq;     /**< I2C reference clock */
  uint32_t              i2cMaxFreq;     /**< I2C max bus frequency to use */
  I2C_ClockHLR_TypeDef  i2cClhr;        /**< Clock low/high ratio control */
} I2CSPM_Init_TypeDef;

/** Default config for I2C init structure. The default may be overridden
    by a i2cspmconfig.h file. */
#define I2CSPM_INIT_DEFAULT                                                    \
  { I2C0,                       /* Use I2C instance 0 */                       \
    gpioPortC,                  /* SCL port */                                 \
    10,                          /* SCL pin */                                  \
    gpioPortC,                  /* SDA port */                                 \
    11,                          /* SDA pin */                                  \
    0,                          /* Location */                                 \
    0,                          /* Use currently configured reference clock */ \
    I2C_FREQ_STANDARD_MAX,      /* Set to standard rate  */                    \
    i2cClockHLRStandard,        /* Set to use 4:4 low/high duty cycle */       \
  }

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/


class I2CSPM
{
private:
    static constexpr uint32_t I2CSPM_TRANSFER_TIMEOUT = 300000;
    I2CSPM_Init_TypeDef i2cConfig;

//    // TX and RX buffers note: unused in blocking transfer.
    uint8_t rxBuf[16];
    uint8_t txBuf[16];
    I2C_TransferReturn_TypeDef transfer(I2C_TransferSeq_TypeDef &seq);
public:

    void init(I2CSPM_Init_TypeDef &config);
    I2C_TransferReturn_TypeDef read(uint16_t addr, uint8_t *buf, uint16_t len);
    I2C_TransferReturn_TypeDef write(uint16_t addr, uint8_t *buf, uint16_t len);
};

namespace I2C {
// class def
extern I2CSPM i2c;
}


#endif /* SRC_MY_I2CSPMCONFIG_H_ */
