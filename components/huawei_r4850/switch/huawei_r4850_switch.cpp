#include "huawei_r4850_switch.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const int16_t SET_FAN_SPEED_MAX_FUNCTION = 0x134;
static const int16_t SET_STANDBY_FUNCTION = 0x132;

void HuaweiR4850Switch::write_state(bool state) {
  std::vector<uint8_t> data = {0x00, (uint8_t)(state ? 0x01 : 0x00), 0x00, 0x00, 0x00, 0x00};
  parent_->set_value(this->registerId_, data);
}

void HuaweiR4850Switch::handle_update(bool success, uint16_t register_id, std::vector<uint8_t> &data) {
  if (register_id != this->registerId_)
    return;
  if(success) {
    this->publish_state(data[1]);
  } else {
    // can't publish invalid state so just don't publish anything
  }
}

}  // namespace huawei_r4850
}  // namespace esphome
