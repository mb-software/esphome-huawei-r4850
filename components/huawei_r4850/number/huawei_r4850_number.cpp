#include "huawei_r4850_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const int8_t SET_VOLTAGE_FUNCTION = 0x0;
static const int8_t SET_CURRENT_FUNCTION = 0x3;

void HuaweiR4850Number::set_parent(HuaweiR4850Component *parent, uint8_t functionCode) {
  this->parent_ = parent;
  this->functionCode_ = functionCode;

  if(functionCode == SET_CURRENT_FUNCTION) {
    this->multiplier_ = 1.0f / parent->get_psu_nominal_current();
  } else {
    this->multiplier_ = 1.0f;
  }
}

void HuaweiR4850Number::control(float value) {
  parent_->set_value(this->functionCode_, 0, value * this->multiplier_ * 1024.0f);
}

void HuaweiR4850Number::handle_update(bool success, uint8_t function_code, bool bit, int32_t raw) {
  if (function_code != this->functionCode_)
    return;
  if(success) {
    float value = (raw / this->multiplier_ / 1024.0f);
    this->publish_state(std::round(value * 10.0f) / 10.0f); // assume 1 decimal
  } else {
    this->publish_state(NAN);
  }
}

void HuaweiR4850Number::set_offline() {
  switch (this->functionCode_) {
    case SET_VOLTAGE_FUNCTION:
    case SET_CURRENT_FUNCTION:
      parent_->set_value(this->functionCode_+1, 0, this->state * this->multiplier_);
      break;

    default:
      break;
  }
}

}  // namespace huawei_r4850
}  // namespace esphome
