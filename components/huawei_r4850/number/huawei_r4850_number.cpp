#include "huawei_r4850_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const uint16_t SET_ONLINE_VOLTAGE_FUNCTION = 0x100;
static const uint16_t SET_OFFLINE_VOLTAGE_FUNCTION = 0x101;
static const uint16_t SET_ONLINE_CURRENT_FUNCTION = 0x103;
static const uint16_t SET_OFFLINE_CURRENT_FUNCTION = 0x104;
static const uint16_t SET_INPUT_CURRENT_FUNCTION = 0x109;

void HuaweiR4850Number::set_parent(HuaweiR4850Component *parent, uint16_t registerId) {
  this->parent_ = parent;
  this->registerId_ = registerId;
}

void HuaweiR4850Number::control(float value) {
  switch (this->registerId_)
  {
    case SET_ONLINE_VOLTAGE_FUNCTION:
    case SET_OFFLINE_VOLTAGE_FUNCTION:
    {
      int32_t raw = value * 1024.0f;
      std::vector<uint8_t> data = {0x00, 0x00, (uint8_t)((raw >> 24) & 0xFF), (uint8_t)((raw >> 16) & 0xFF), (uint8_t)((raw >> 8) & 0xFF), (uint8_t)(raw & 0xFF)};
      this->parent_->set_value(this->registerId_, data);
      break;
    }
    
    case SET_ONLINE_CURRENT_FUNCTION:
    case SET_OFFLINE_CURRENT_FUNCTION:
    {
      int32_t raw = value / this->parent_->get_psu_max_current() * 1250.0f;
      std::vector<uint8_t> data = {0x00, 0x00, (uint8_t)((raw >> 24) & 0xFF), (uint8_t)((raw >> 16) & 0xFF), (uint8_t)((raw >> 8) & 0xFF), (uint8_t)(raw & 0xFF)};
      this->parent_->set_value(this->registerId_, data);
      break;
    }

    case SET_INPUT_CURRENT_FUNCTION:
    {
      if (value > 0.0f) {
        int32_t raw = value * 1024.0f;
        std::vector<uint8_t> data = {0x00, 0x01, (uint8_t)((raw >> 24) & 0xFF), (uint8_t)((raw >> 16) & 0xFF), (uint8_t)((raw >> 8) & 0xFF), (uint8_t)(raw & 0xFF)};
        this->parent_->set_value(this->registerId_, data);
      } else {
        std::vector<uint8_t> data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        this->parent_->set_value(this->registerId_, data);
      }
      break;
    }
  
  default:
    break;
  }
}

void HuaweiR4850Number::handle_update(bool success, uint16_t register_id, std::vector<uint8_t> &data) {
  if (register_id != this->registerId_)
    return;
  if (success) {
    float value = 0;
    switch (this->registerId_)
    {
      case SET_ONLINE_VOLTAGE_FUNCTION:
      case SET_OFFLINE_VOLTAGE_FUNCTION:
      {
        int32_t raw = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | data[5];
        value = raw / 1024.0f;
        break;
      }
      
      case SET_ONLINE_CURRENT_FUNCTION:
      case SET_OFFLINE_CURRENT_FUNCTION:
      {
        int32_t raw = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | data[5];
        value = raw / 1250.0f * this->parent_->get_psu_max_current();
        break;
      }
  
      case SET_INPUT_CURRENT_FUNCTION:
      {
        bool active = data[1];
        if (active) {
          int32_t raw = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | data[5];
          value = raw / 1024.0f;
        } else {
          value = 0.0f;
        }
        break;
      }
    
    default:
      break;
    }
    this->publish_state(std::round(value * 10.0f) / 10.0f); // assume 1 decimal
  } else {
    this->publish_state(NAN);
  }
}

}  // namespace huawei_r4850
}  // namespace esphome
