#pragma once
#include "../huawei_r4850.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace huawei_r4850 {

class HuaweiR4850Switch : public switch_::Switch, public Component, public HuaweiR4850Input {
 public:
  void set_parent(HuaweiR4850Component *parent, uint8_t registerId) {
    this->parent_ = parent;
    this->registerId_ = registerId;
  };

  void handle_update(bool success, uint16_t register_id, std::vector<uint8_t> &data) override;
  void set_offline();

 protected:
  HuaweiR4850Component *parent_;
  uint8_t registerId_;

  void write_state(bool state) override;
};

}  // namespace huawei_r4850
}  // namespace esphome
