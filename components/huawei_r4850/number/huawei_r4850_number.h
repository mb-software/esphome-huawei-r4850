#pragma once
#include "../huawei_r4850.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace huawei_r4850 {

class HuaweiR4850Number : public number::Number, public Component {
 public:
  void set_parent(HuaweiR4850Component *parent, int8_t functionCode) {
    this->parent_ = parent;
    this->functionCode_ = functionCode;
  };

 protected:
  HuaweiR4850Component *parent_;
  int8_t functionCode_;

  void control(float value) override;
};

}  // namespace huawei_r4850
}  // namespace esphome
