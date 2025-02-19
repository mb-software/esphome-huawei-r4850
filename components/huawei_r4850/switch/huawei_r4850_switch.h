#pragma once
#include "../huawei_r4850.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace huawei_r4850 {

class HuaweiR4850Switch : public switch_::Switch, public Component, public HuaweiR4850Input {
 public:
  void set_parent(HuaweiR4850Component *parent, uint8_t functionCode) {
    this->parent_ = parent;
    this->functionCode_ = functionCode;
  };

  void handle_update(bool success, uint8_t function_code, bool bit, int32_t raw) override;
  void set_offline();

 protected:
  HuaweiR4850Component *parent_;
  uint8_t functionCode_;

  void write_state(bool state) override;
};

}  // namespace huawei_r4850
}  // namespace esphome
