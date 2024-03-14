#include "lps22.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lps22 {

//Constans
static const char *const TAG = "lps22.sensor";

//Sensor variants
static const uint8_t LPS22HB_CHIP_ID = 0xB1;

static const uint8_t LPS22HH_CHIP_ID = 0xB3;
static const uint8_t LPS22CH_CHIP_ID = 0xB3;

static const uint8_t LPS22DF_CHIP_ID = 0xB4;
static const uint8_t LPS22QS_CHIP_ID = 0xB4;

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
  ESP_LOGCONFIG(TAG, "Setting up LPS22...");
  
  uint8_t chip_id = 0;

  if (!this->read_byte(WHO_AM_I, &chip_id)) {
    this->error_code_ = COMMUNICATION_FAILED;
    this->mark_failed();
    return;
  }

  ESP_LOGD(TAG, "Chip ID: %x", chip_id);

  if (chip_id != LPS22_CHIP_ID) {
    ESP_LOGD(TAG, "Wrong Chip ID");
    this->error_code_ = WRONG_CHIP_ID;
    this->mark_failed();
    return;
  }
  
  //RESET
  if (!this->Register_SetBit_(CTRL_REG2, 4)) {
      this->status_set_warning();
      return;
  }
}

void LPS22Component::dump_config() {
  ESP_LOGCONFIG(TAG, "LPS22:");
  LOG_I2C_DEVICE(this);
  switch (this->error_code_) {
    case COMMUNICATION_FAILED:
      ESP_LOGE(TAG, "Communication with LPS22 failed!");
      break;
    case WRONG_CHIP_ID:
      ESP_LOGE(TAG, "LPS22 has wrong chip ID! Is it a LPS22?");
      break;
    case NONE:
    default:
      break;
  }
  LOG_UPDATE_INTERVAL(this);

  LOG_SENSOR("  ", "Temperature", this->temperature_);
  LOG_SENSOR("  ", "Pressure", this->pressure_);
}

float LPS22Component::get_setup_priority() const { return setup_priority::DATA; }

void LPS22Component::update() {
  uint8_t buffer[6];

    ESP_LOGD(TAG, "LPS22Component::update()");
    if (!this->write_byte(CTRL_REG1, 0x00)) {
      this->status_set_warning();
      return;
    }

    if (!this->write_byte(CTRL_REG2, 0x01)) {
      this->status_set_warning();
      return;
    }

    uint32_t start = millis();
    while (this->read_byte(STATUS, &buffer[0]) && ((buffer[0] & 0x01) == 0)) {
      if (millis() - start > 100) {
        ESP_LOGW(TAG, "Reading LPS22 timed out");
        this->status_set_warning();
        return;
      }
      yield();
    }
  
    if (!this->read_bytes(PRESS_OUT_XL, buffer, 5)) {
      this->mark_failed();
      return;
    }

    uint8_t CTRL_REG2_val;
    this->read_byte(CTRL_REG2, &CTRL_REG2_val);
    ESP_LOGD(TAG, "CTRL_REG2: %x", CTRL_REG2_val);
    

    float reading_p = ((buffer[0] << 0) | (buffer[1] << 8) | (buffer[2] << 16)) / 4096.0;
    float reading_t = (buffer[3] << 0) | (buffer[4] << 8);

    if (this->temperature_ != nullptr) {
        this->temperature_->publish_state(reading_t/100);
    }

    if (this->pressure_ != nullptr) {
        this->pressure_->publish_state(reading_p);
    }

}


bool LPS22Component::Register_SetBit_(uint8_t reg, uint8_t bit){
  uint8_t register_value;
  
  this->read_byte(reg, &register_value);
  register_value |= (1 << bit);
  return (this->write_byte(CTRL_REG2, register_value));
}


}  // namespace lps22
}  // namespace esphome