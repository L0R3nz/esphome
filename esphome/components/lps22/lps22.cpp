#include "lps22.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lps22 {

//Constans
static const char *const TAG = "lps22.sensor";

//Sensor variants
static const uint8_t LPS22HB_CHIP_ID = 0xB1;

//Registers
static const uint8_t INTERRUPT_CFG = 0x0B; //RW  - Interrupt Register
static const uint8_t THS_P_L       = 0x0C; //RW  - Pressure threshold register Low
static const uint8_t THS_P_H       = 0x0D; //RW  - Pressure threshold register High

static const uint8_t WHO_AM_I      = 0x0F; //R   - Who am I
static const uint8_t CTRL_REG1     = 0x10; //RW  - Control register 1
static const uint8_t CTRL_REG2     = 0x11; //RW  - Control register 2
static const uint8_t CTRL_REG3     = 0x12; //RW  - Control register 3

static const uint8_t FIFO_CTRL     = 0x14; //RW  - FIFO configuration register
static const uint8_t REF_P_XL      = 0x15; //RW  - Reference pressure register (LBS data)
static const uint8_t REF_P_L       = 0x16; //RW  - Reference pressure register (middle part)
static const uint8_t REF_P_H       = 0x17; //WR  - Reference pressure register (MSB part)
static const uint8_t RPDS_L        = 0x18; //RW  - Pressure offset register Low
static const uint8_t RPDS_H        = 0x19; //RW  - Pressure offset register High
static const uint8_t RES_CONF      = 0x1A; //RW  - Resolution register

static const uint8_t INT_SOURCE    = 0x25; //R   - Interrupt Register
static const uint8_t FIFO_STATUS   = 0x26; //R   - FIFO status register
static const uint8_t STATUS        = 0x27; //R   - Status register
static const uint8_t PRESS_OUT_XL  = 0x28; //R   - Pressure output register (LSB)
static const uint8_t PRESS_OUT_L   = 0x29; //R   - Pressure output register (mid part)
static const uint8_t PRESS_OUT_H   = 0x2A; //R   - Pressure output register (MSB)
static const uint8_t TEMP_OUT_L    = 0x2B; //R   - Temperature output register (LSB)
static const uint8_t TEMP_OUT_H    = 0x2C; //R   - Temperature output register (MSB)

static const uint8_t LPFP_RES      = 0x33; //R   - Filter reset register


void LPS22Component::setup() {

}

void LPS22Component::dump_config() {

}

float LPS22Component::get_setup_priority() const { return setup_priority::DATA; }

void LPS22Component::update() {

}


}  // namespace lps22
}  // namespace esphome