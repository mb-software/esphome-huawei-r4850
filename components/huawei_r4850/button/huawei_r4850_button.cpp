#include "huawei_r4850_button.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

void HuaweiR4850Button::press_action() { this->parent_->set_offline_values(); }

}  // namespace huawei_r4850
}  // namespace esphome
