#pragma once
#include "../huawei_r4850.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace huawei_r4850 {

class HuaweiR4850Number : public number::Number, public Component, public HuaweiR4850Input {
 public:
  void set_parent(HuaweiR4850Component *parent, uint8_t functionCode);
  void handle_update(bool success, uint8_t function_code, bool bit, int32_t raw) override;
  void set_offline();

 protected:
  HuaweiR4850Component *parent_;
  uint8_t functionCode_;
  float multiplier_;

  void control(float value) override;
};

}  // namespace huawei_r4850
}  // namespace esphome
