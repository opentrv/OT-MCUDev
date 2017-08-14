
/**
 * Trying to make i2c work with Si7021. This is supposedly software compatible with the SHT21.
 * Steps:
 * - Init i2c peripheral.
 * - Init Si7021
 * - Make temperature reading
 * - Write to serial? (debug semihosting)
 */

extern "C" {  // Simplicity Studio headers. These are all C files.
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
//#include "em_i2c.h"
#include "InitDevice.h"  // Not included by default for some reason
}

#include <my_i2cspm.h>
#include "sensors.h"

OTV0P2BASE::RoomTemperatureC16_SHT21 si7021;


void SysTick_Handler()
{

}


int main(void)
{
    I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;  // I2C settings
  /* Chip errata */
    enter_DefaultMode_from_RESET(); // This calls CHIP_Init() anyway

    SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 10);  // ???

    I2C::i2c.init(i2cInit);  // Initialise I2C peripheral
    GPIO_PinOutSet(SI7021_CE_PORT, SI7021_CE_PIN);  // Power up Si7021

    for (auto i = 0; i < 100000; ++i) {}

    const int readVal = si7021.read();


  /* Infinite loop */
  while (1) {

  }
}
