#include "huawei_r4850_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const int16_t SET_FAN_SPEED_MAX_FUNCTION = 0x134;
static const int16_t SET_STANDBY_FUNCTION = 0x132;

void HuaweiR4850Switch::write_state(bool state) {
  std::vector<uint8_t> data = {0x00, (uint8_t)(state ? 0x01 : 0x00), 0x00, 0x00, 0x00, 0x00};
  this->parent_->set_value(this->registerId_, data);
}

bool HuaweiR4850Switch::assumed_state() {
  // if we could set the value to "unknown" we wouldn't need this, but we can't
  // so we just tell HA we don't know the state.
  return true;
}

void HuaweiR4850Switch::handle_update(uint16_t register_id, std::vector<uint8_t> &data) {
  if (register_id != this->registerId_)
    return;
  this->publish_state(data[1]);
}
void HuaweiR4850Switch::handle_error(uint16_t register_id, std::vector<uint8_t> &data) {
  if (register_id != this->registerId_)
    return;
  // we should set the state to "unknown" here, but ESPHome doesn't have a way to do that (for switch entities).
}
void HuaweiR4850Switch::handle_timeout() {
  // we should set the state to "unavailable" here, but ESPHome doesn't have a way to do that:
  // https://github.com/esphome/feature-requests/issues/1568
  // and there isn't a way to set it to "unknown" either.
}

}  // namespace huawei_r4850
}  // namespace esphome
