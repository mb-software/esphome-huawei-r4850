#include "huawei_r4850.h"
#include "esphome/core/application.h"
#include "esphome/core/base_automation.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace huawei_r4850 {

static const char *const TAG = "huawei_r4850";

static const uint8_t R48xx_CMD_DATA = 0x40;
static const uint8_t R48xx_CMD_CONTROL = 0x80;

static const uint16_t R48xx_DATA_INPUT_POWER = 0x170;
static const uint16_t R48xx_DATA_INPUT_FREQ = 0x171;
static const uint16_t R48xx_DATA_INPUT_CURRENT = 0x172;
static const uint16_t R48xx_DATA_OUTPUT_POWER = 0x173;
static const uint16_t R48xx_DATA_EFFICIENCY = 0x174;
static const uint16_t R48xx_DATA_OUTPUT_VOLTAGE = 0x175;
static const uint16_t R48xx_DATA_OUTPUT_CURRENT_MAX = 0x176;
static const uint16_t R48xx_DATA_INPUT_VOLTAGE = 0x178;
static const uint16_t R48xx_DATA_OUTPUT_TEMPERATURE = 0x17F;
static const uint16_t R48xx_DATA_INPUT_TEMPERATURE = 0x180;
static const uint16_t R48xx_DATA_OUTPUT_CURRENT = 0x181;
static const uint16_t R48xx_DATA_OUTPUT_CURRENT1 = 0x182;

HuaweiR4850Component::HuaweiR4850Component(canbus::Canbus *canbus) { this->canbus = canbus; }

void HuaweiR4850Component::setup() {
  Automation<std::vector<uint8_t>, uint32_t, bool> *automation;
  LambdaAction<std::vector<uint8_t>, uint32_t, bool> *lambdaaction;
  canbus::CanbusTrigger *canbus_canbustrigger;

  canbus_canbustrigger = new canbus::CanbusTrigger(this->canbus, 0, 0, true);
  canbus_canbustrigger->set_component_source("canbus");
  App.register_component(canbus_canbustrigger);
  automation = new Automation<std::vector<uint8_t>, uint32_t, bool>(canbus_canbustrigger);
  auto cb = [=](std::vector<uint8_t> x, uint32_t can_id, bool remote_transmission_request) -> void {
    this->on_frame(can_id, remote_transmission_request, x);
  };
  lambdaaction = new LambdaAction<std::vector<uint8_t>, uint32_t, bool>(cb);
  automation->add_actions({lambdaaction});
}

void HuaweiR4850Component::update() {
  ESP_LOGD(TAG, "Sending request message");
  uint32_t canId = this->canid_pack_(this->psu_addr_, R48xx_CMD_DATA, true, false);
  std::vector<uint8_t> data = {0, 0, 0, 0, 0, 0, 0, 0};
  this->canbus->send_data(canId, true, data);

  // no new value for 5* intervall -> set sensors to NAN)
  if (millis() - lastUpdate_ > this->update_interval_ * 5) {
    this->publish_sensor_state_(this->input_power_sensor_, NAN);
    this->publish_sensor_state_(this->input_voltage_sensor_, NAN);
    this->publish_sensor_state_(this->input_current_sensor_, NAN);
    this->publish_sensor_state_(this->input_temp_sensor_, NAN);
    this->publish_sensor_state_(this->input_frequency_sensor_, NAN);
    this->publish_sensor_state_(this->output_power_sensor_, NAN);
    this->publish_sensor_state_(this->output_current_sensor_, NAN);
    this->publish_sensor_state_(this->output_current_setpoint_sensor_, NAN);
    this->publish_sensor_state_(this->output_voltage_sensor_, NAN);
    this->publish_sensor_state_(this->output_temp_sensor_, NAN);
    this->publish_sensor_state_(this->efficiency_sensor_, NAN);
  }
}

void HuaweiR4850Component::set_value(uint16_t register_id, std::vector<uint8_t> &data) {
  if(data.size() != 6)
  {
    ESP_LOGE(TAG, "Invalid data count for register id %03x", register_id);
    return;
  }

  uint32_t canId = this->canid_pack_(this->psu_addr_, R48xx_CMD_CONTROL, true, false);

  std::vector<uint8_t> message = {(uint8_t)((register_id & 0xF00) >> 8), (uint8_t)(register_id & 0x0FF)};
  message.insert(message.end(), data.begin(), data.end());

  this->canbus->send_data(canId, true, message);
}

void HuaweiR4850Component::on_frame(uint32_t can_id, bool rtr, std::vector<uint8_t> &message) {
  if (message.size() < 8) {
    return;
  }

  uint8_t psu_addr, cmd;
  bool src_controller, incomplete;
  this->canid_unpack_(can_id, &psu_addr, &cmd, &src_controller, &incomplete);

  if (psu_addr != this->psu_addr_) {
    return;
  }

  uint8_t error_type = (message[0] & 0xF0) >> 4;
  uint16_t register_id = ((message[0] & 0x0F) << 8) | message[1];

  if (cmd == R48xx_CMD_DATA && !src_controller) {
    int32_t value = (message[4] << 24) | (message[5] << 16) | (message[6] << 8) | message[7];
    float conv_value = 0;
    switch (register_id) {
      case R48xx_DATA_INPUT_POWER:
        conv_value = value / 1024.0f;
        this->publish_sensor_state_(this->input_power_sensor_, conv_value);
        ESP_LOGV(TAG, "Input power: %f", conv_value);
        break;

      case R48xx_DATA_INPUT_FREQ:
        conv_value = value / 1024.0f;
        this->publish_sensor_state_(this->input_frequency_sensor_, conv_value);
        ESP_LOGV(TAG, "Input frequency: %f", conv_value);
        break;

      case R48xx_DATA_INPUT_CURRENT:
        conv_value = value / 1024.0f;
        this->publish_sensor_state_(this->input_current_sensor_, conv_value);
        ESP_LOGV(TAG, "Input current: %f", conv_value);
        break;

      case R48xx_DATA_OUTPUT_POWER:
        conv_value = value / 1024.0f;
        this->publish_sensor_state_(this->output_power_sensor_, conv_value);
        ESP_LOGV(TAG, "Output power: %f", conv_value);
        break;

      case R48xx_DATA_EFFICIENCY:
        conv_value = value / 1024.0f * 100.0f;
        this->publish_sensor_state_(this->efficiency_sensor_, conv_value);
        ESP_LOGV(TAG, "Efficiency: %f", conv_value);
        break;

      case R48xx_DATA_OUTPUT_VOLTAGE:
        conv_value = value / 1024.0f;
        this->publish_sensor_state_(this->output_voltage_sensor_, conv_value);
        ESP_LOGV(TAG, "Output voltage: %f", conv_value);
        break;

      case R48xx_DATA_OUTPUT_CURRENT_MAX:
        // this is not equal to the value set via max_output_current
        // as it is also set (according to the current AC input voltage) when AC limit is set
        conv_value = value / 1024.0f * this->psu_nominal_current_;
        this->publish_sensor_state_(this->output_current_setpoint_sensor_, conv_value);
        ESP_LOGV(TAG, "Max Output current: %f", conv_value);
        break;

      case R48xx_DATA_INPUT_VOLTAGE:
        conv_value = value / 1024.0f;
        this->publish_sensor_state_(this->input_voltage_sensor_, conv_value);
        ESP_LOGV(TAG, "Input voltage: %f", conv_value);
        break;

      case R48xx_DATA_OUTPUT_TEMPERATURE:
        conv_value = value / 1024.0f;
        this->publish_sensor_state_(this->output_temp_sensor_, conv_value);
        ESP_LOGV(TAG, "Output temperature: %f", conv_value);
        break;

      case R48xx_DATA_INPUT_TEMPERATURE:
        conv_value = value / 1024.0f;
        this->publish_sensor_state_(this->input_temp_sensor_, conv_value);
        ESP_LOGV(TAG, "Input temperature: %f", conv_value);
        break;

      case R48xx_DATA_OUTPUT_CURRENT1:
        // printf("Output Current(1) %.02fA\r\n", value / 1024.0f);
        // output_current = value / 1024.0f;
        break;

      case R48xx_DATA_OUTPUT_CURRENT:
        conv_value = value / 1024.0f;
        this->publish_sensor_state_(this->output_current_sensor_, conv_value);
        ESP_LOGV(TAG, "Output current: %f", conv_value);
        break;

      default:
        // printf("Unknown parameter 0x%02X, 0x%04X\r\n",frame[1], value);
        break;
    }
    if (!incomplete) {
      this->lastUpdate_ = millis();
    }
  } else if (cmd == R48xx_CMD_CONTROL && !src_controller) {
    std::vector<uint8_t> data(message.begin() + 2, message.end());
    if (error_type == 0) {
      for (auto &input : this->registered_inputs_) {
        input->handle_update(true, register_id, data);
      }
      ESP_LOGD(TAG, "Value %03x set OK: %02x %02x %02x %02x %02x %02x", register_id, data[0], data[1], data[2], data[3], data[4], data[5]);
    } else {
      for (auto &input : this->registered_inputs_) {
        input->handle_update(false, register_id, data);
      }
      ESP_LOGW(TAG, "Value %03x set error: %d", register_id, error_type);
    }
  }
}

void HuaweiR4850Component::publish_sensor_state_(sensor::Sensor *sensor, float value) {
  if (sensor) {
    sensor->publish_state(value);
  }
}

uint32_t HuaweiR4850Component::canid_pack_(uint8_t addr, uint8_t command, bool src_controller, bool incomplete) {
  uint32_t id = 0x1080007E; // proto ID, group mask, HW/SW id flag already set
  id |= (uint32_t)(addr & 0x7F) << 16; // 7 bit PSU address (0 = broadcast, 1 = first, ...)
  id |= (uint32_t)command << 8; // command id
  id |= (uint32_t)src_controller << 7; // msg source (0 = PSU, 1 = controller)
  id |= (uint32_t)incomplete; // last message marker (0 = finished, always 0 in requests)
  return id;
}

void HuaweiR4850Component::canid_unpack_(uint32_t canId, uint8_t *addr, uint8_t *command, bool *src_controller, bool *incomplete) {
  *addr =           (canId & 0x007F0000) >> 16;
  *command =        (canId & 0x0000FF00) >> 8;
  *src_controller = (canId & 0x00000080) >> 7;
  *incomplete =     (canId & 0x00000001);
}

}  // namespace huawei_r4850
}  // namespace esphome
