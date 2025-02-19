#include "huawei_r4850_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const int8_t SET_FAN_SPEED_MAX_FUNCTION = 0x34;
static const int8_t SET_STANDBY_FUNCTION = 0x32;

void HuaweiR4850Switch::write_state(bool state) {
  parent_->set_value(this->functionCode_, state, 0);
}

void HuaweiR4850Switch::handle_update(bool success, uint8_t function_code, bool bit, int32_t raw) {
  if (function_code != this->functionCode_)
    return;
  if(success) {
    this->publish_state(bit);
  } else {
    // can't publish invalid state so just don't publish anything
  }
}

void HuaweiR4850Switch::set_offline() {
  switch (this->functionCode_) {
    case SET_FAN_SPEED_MAX_FUNCTION: // fan speed doesnt have offline
    case SET_STANDBY_FUNCTION: // don't save standby mode (if that is even possible)
    default:
      break;
  }
}

}  // namespace huawei_r4850
}  // namespace esphome
