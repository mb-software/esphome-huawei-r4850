#pragma once
#include "../huawei_r4850.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

#include <optional>

namespace esphome {
namespace huawei_r4850 {

class HuaweiR4850Switch : public switch_::Switch, public Component, public HuaweiR4850Input {
 public:
  void set_parent(HuaweiR4850Component *parent, uint16_t registerId) {
    this->parent_ = parent;
    this->registerId_ = registerId;
  };

  void handle_update(uint16_t register_id, std::vector<uint8_t> &data) override;
  void handle_error(uint16_t register_id, std::vector<uint8_t> &data) override;
  void handle_timeout() override;
  void resend_state() override;

 protected:
  HuaweiR4850Component *parent_;
  uint16_t registerId_;
  std::optional<bool> last_state_;

  void send_state_(bool value);

  void write_state(bool state) override;
  bool assumed_state() override;
};

}  // namespace huawei_r4850
}  // namespace esphome
