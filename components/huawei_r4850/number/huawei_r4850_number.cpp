#include "huawei_r4850_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const uint16_t SET_VOLTAGE_FUNCTION = 0x100;
static const uint16_t SET_CURRENT_FUNCTION = 0x103;
static const uint16_t SET_INPUT_CURRENT_FUNCTION = 0x109;

void HuaweiR4850Number::set_parent(HuaweiR4850Component *parent, uint16_t registerId) {
  this->parent_ = parent;
  this->registerId_ = registerId;

  if (registerId == SET_CURRENT_FUNCTION) {
    this->multiplier_ = 1.0f / parent->get_psu_nominal_current();
  } else {
    this->multiplier_ = 1.0f;
  }
}

void HuaweiR4850Number::control(float value) {
  if (this->registerId_ == SET_INPUT_CURRENT_FUNCTION) {
    if (value > 0.0f) {
      int32_t raw = value * this->multiplier_ * 1024.0f;
      std::vector<uint8_t> data = {0x00, 0x01, (raw >> 24) & 0xFF, (raw >> 16) & 0xFF, (raw >> 8) & 0xFF, raw & 0xFF};
      parent_->set_value(this->registerId_, data);
    } else {
      std::vector<uint8_t> data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
      parent_->set_value(this->registerId_, data);
    }
  } else {
    int32_t raw = value * this->multiplier_ * 1024.0f;
    std::vector<uint8_t> data = {0x00, 0x00, (raw >> 24) & 0xFF, (raw >> 16) & 0xFF, (raw >> 8) & 0xFF, raw & 0xFF};
    parent_->set_value(this->registerId_, data);
  }
}

void HuaweiR4850Number::handle_update(bool success, uint16_t register_id, std::vector<uint8_t> &data) {
  if (register_id != this->registerId_)
    return;
  if (success) {
    int32_t raw = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | data[5];
    float value = (raw / this->multiplier_ / 1024.0f);
    this->publish_state(std::round(value * 10.0f) / 10.0f); // assume 1 decimal
  } else {
    this->publish_state(NAN);
  }
}

void HuaweiR4850Number::set_offline() {
  switch (this->registerId_) {
    case SET_VOLTAGE_FUNCTION:
    case SET_CURRENT_FUNCTION:
      parent_->set_value(this->registerId_+1, 0, this->state * this->multiplier_);
      break;

    default:
      break;
  }
}

}  // namespace huawei_r4850
}  // namespace esphome
