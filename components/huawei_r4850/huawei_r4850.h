#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/canbus/canbus.h"

namespace esphome {
namespace huawei_r4850 {

class HuaweiR4850Input {
 public:
    HuaweiR4850Input() {}
    virtual void handle_update(uint16_t register_id, std::vector<uint8_t> &data) = 0;
    virtual void handle_error(uint16_t register_id, std::vector<uint8_t> &data) = 0;
    virtual void handle_timeout() = 0;
};

class HuaweiR4850Component : public PollingComponent {
 public:
  HuaweiR4850Component(canbus::Canbus *canbus);
  void setup() override;
  void update() override;

  void set_value(uint16_t register_id, std::vector<uint8_t> &data);

  void set_input_voltage_sensor(sensor::Sensor *input_voltage_sensor) { input_voltage_sensor_ = input_voltage_sensor; }
  void set_input_frequency_sensor(sensor::Sensor *input_frequency_sensor) {
    input_frequency_sensor_ = input_frequency_sensor;
  }
  void set_input_current_sensor(sensor::Sensor *input_current_sensor) { input_current_sensor_ = input_current_sensor; }
  void set_input_power_sensor(sensor::Sensor *input_power_sensor) { input_power_sensor_ = input_power_sensor; }
  void set_input_temp_sensor(sensor::Sensor *input_temp_sensor) { input_temp_sensor_ = input_temp_sensor; }
  void set_efficiency_sensor(sensor::Sensor *efficiency_sensor) { efficiency_sensor_ = efficiency_sensor; }
  void set_output_voltage_sensor(sensor::Sensor *output_voltage_sensor) {
    output_voltage_sensor_ = output_voltage_sensor;
  }
  void set_output_current_sensor(sensor::Sensor *output_current_sensor) {
    output_current_sensor_ = output_current_sensor;
  }
  void set_output_current_setpoint_sensor(sensor::Sensor *output_current_setpoint_sensor) {
    output_current_setpoint_sensor_ = output_current_setpoint_sensor;
  }
  void set_output_power_sensor(sensor::Sensor *output_power_sensor) { output_power_sensor_ = output_power_sensor; }
  void set_output_temp_sensor(sensor::Sensor *output_temp_sensor) { output_temp_sensor_ = output_temp_sensor; }

  void register_input(HuaweiR4850Input *number) {
    this->registered_inputs_.push_back(number);
  }

  void set_psu_address(uint8_t value) {
    this->psu_addr_ = value;
  }

  void set_psu_max_current(float value) {
    psu_max_current_ = value;
  }

  float get_psu_max_current() {
    return psu_max_current_;
  }

 protected:
  canbus::Canbus *canbus;
  uint32_t lastUpdate_;
  float psu_max_current_;
  uint8_t psu_addr_;

  sensor::Sensor *input_voltage_sensor_{nullptr};
  sensor::Sensor *input_frequency_sensor_{nullptr};
  sensor::Sensor *input_current_sensor_{nullptr};
  sensor::Sensor *input_power_sensor_{nullptr};
  sensor::Sensor *input_temp_sensor_{nullptr};
  sensor::Sensor *efficiency_sensor_{nullptr};
  sensor::Sensor *output_voltage_sensor_{nullptr};
  sensor::Sensor *output_current_sensor_{nullptr};
  sensor::Sensor *output_current_setpoint_sensor_{nullptr};
  sensor::Sensor *output_power_sensor_{nullptr};
  sensor::Sensor *output_temp_sensor_{nullptr};

  std::vector<HuaweiR4850Input *> registered_inputs_{};

  void on_frame(uint32_t can_id, bool rtr, std::vector<uint8_t> &data);

  void publish_sensor_state_(sensor::Sensor *sensor, float value);
  uint32_t canid_pack_(uint8_t addr, uint8_t command, bool src_controller, bool incomplete);
  void canid_unpack_(uint32_t canId, uint8_t *addr, uint8_t *command, bool *src_controller, bool *incomplete);
};

}  // namespace huawei_r4850
}  // namespace esphome
