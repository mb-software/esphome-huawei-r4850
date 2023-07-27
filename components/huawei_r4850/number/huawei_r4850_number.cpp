#include "huawei_r4850_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const int8_t SET_VOLTAGE_FUNCTION = 0x0;
static const int8_t SET_CURRENT_FUNCTION = 0x3;

void HuaweiR4850Number::control(float value) {
  switch (this->functionCode_) {
    case SET_VOLTAGE_FUNCTION:
      parent_->set_output_voltage(value);
      this->publish_state(value);
      break;
    case SET_CURRENT_FUNCTION:
      parent_->set_max_output_current(value);
      break;

    default:
      break;
  }
}

}  // namespace huawei_r4850
}  // namespace esphome
