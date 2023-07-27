#pragma once
#include "../huawei_r4850.h"
#include "esphome/core/component.h"
#include "esphome/components/button/button.h"

namespace esphome {
namespace huawei_r4850 {

class HuaweiR4850Button : public button::Button, public Component {
 public:
  void set_parent(HuaweiR4850Component *parent) { this->parent_ = parent; };

 protected:
  HuaweiR4850Component *parent_;

  void press_action() override;
};

}  // namespace huawei_r4850
}  // namespace esphome
