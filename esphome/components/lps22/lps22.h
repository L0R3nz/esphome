#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace lps22 {

enum LPS22_OutputDataRate {
  LPS22_OUTPUT_DATA_RATE_NONE   = 0,
  LPS22_OUTPUT_DATA_RATE_1_HZ   = 1,
  LPS22_OUTPUT_DATA_RATE_10_HZ  = 2,
  LPS22_OUTPUT_DATA_RATE_25_HZ  = 3,
  LPS22_OUTPUT_DATA_RATE_50_HZ  = 4,
  LPS22_OUTPUT_DATA_RATE_75_HZ  = 5,
};

enum LPS22_LowPassFilterConf {
  LPS22_FILTER_1_2  = 0,
  LPS22_FILTER_1_9  = 1,
  LPS22_FILTER_1_20 = 2
};


class LPS22Component : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_temperature(sensor::Sensor *temperature) { temperature_ = temperature; }
  void set_pressure(sensor::Sensor *pressure) { pressure_ = pressure; }

  void setup() override;
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override;

 protected:
  sensor::Sensor *temperature_{nullptr};
  sensor::Sensor *pressure_{nullptr};

  enum ErrorCode {
    NONE = 0,
    COMMUNICATION_FAILED,
    WRONG_CHIP_ID,
  } error_code_{NONE};
};

}  // namespace lps22
}  // namespace esphome