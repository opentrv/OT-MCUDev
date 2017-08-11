
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

I2C_TransferReturn_TypeDef write1;
I2C_TransferReturn_TypeDef write2;
I2C_TransferReturn_TypeDef read1;
uint8_t randomValue = 0;

// class def
I2CSPM i2c;

void SysTick_Handler()
{

}



static constexpr uint8_t SHT21_I2C_ADDR = 0x40;
static constexpr uint8_t SHT21_I2C_CMD_TEMP_HOLD   = 0xe3;
static constexpr uint8_t SHT21_I2C_CMD_TEMP_NOHOLD = 0xf3;
static constexpr uint8_t SHT21_I2C_CMD_RH_HOLD     = 0xe5;
static constexpr uint8_t SHT21_I2C_CMD_RH_NOHOLD   = 0xf5;
static constexpr uint8_t SHT21_I2C_CMD_USERREG     = 0xe7; // User register...

// If defined, sample 8-bit RH (for 1%) and 12-bit temp (for 1/16C).
// Reduced precision should save time and energy.
static constexpr bool SHT21_USE_REDUCED_PRECISION = true;

// Set true once SHT21 has been initialised.
static volatile bool SHT21_initialised;
void SHT21_Init()
{
    if(SHT21_USE_REDUCED_PRECISION) {
        // Soft reset in order to sample at reduced precision.
        uint8_t cmd_ureg = SHT21_I2C_CMD_USERREG;
        i2c.write(SHT21_I2C_ADDR, &cmd_ureg, 1); // initMsg

        // ????
        uint8_t rxMsg;
        read1 = i2c.read(SHT21_I2C_ADDR, &rxMsg, 1);
        const uint8_t curUR = rxMsg;
        randomValue = curUR;

        // Preserve reserved bits (3, 4, 5) and sample 8-bit RH (for for 1%) and 12-bit temp (for 1/16C).
        const uint8_t newUR = (curUR & 0x38) | 3;
        uint8_t configMsg[2] = { SHT21_I2C_CMD_USERREG, newUR};
        write2 = i2c.write(SHT21_I2C_ADDR, configMsg, sizeof(configMsg)); // setupMsg
    }
    SHT21_initialised = true;
}




int main(void)
{
    I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;  // I2C settings
  /* Chip errata */
    enter_DefaultMode_from_RESET(); // This calls CHIP_Init() anyway

    SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 10);  // ???

    i2c.init(i2cInit);  // Initialise I2C peripheral
    GPIO_PinOutSet(SI7021_CE_PORT, SI7021_CE_PIN);  // Power up Si7021

    SHT21_Init();



  /* Infinite loop */
  while (1) {

  }
}
