import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_POWER,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
    UNIT_HERTZ,
    UNIT_AMPERE,
    UNIT_PERCENT,
    UNIT_CELSIUS,
    UNIT_WATT,
    ICON_FLASH,
    ICON_PERCENT,
    ICON_THERMOMETER,
    ICON_CURRENT_AC,
    ICON_POWER,
)
from . import HuaweiR4850Component, CONF_HUAWEI_R4850_ID

CONF_INPUT_VOLTAGE = "input_voltage"
CONF_INPUT_FREQUENCY = "input_frequency"
CONF_INPUT_CURRENT = "input_current"
CONF_INPUT_POWER = "input_power"
CONF_INPUT_TEMP = "input_temp"
CONF_EFFICIENCY = "efficiency"
CONF_OUTPUT_VOLTAGE = "output_voltage"
CONF_OUTPUT_CURRENT = "output_current"
CONF_MAX_OUTPUT_CURRENT = "max_output_current"
CONF_OUTPUT_POWER = "output_power"
CONF_OUTPUT_TEMP = "output_temp"


TYPES = [
    CONF_INPUT_VOLTAGE,
    CONF_INPUT_FREQUENCY,
    CONF_INPUT_CURRENT,
    CONF_INPUT_POWER,
    CONF_INPUT_TEMP,
    CONF_EFFICIENCY,
    CONF_OUTPUT_VOLTAGE,
    CONF_OUTPUT_CURRENT,
    # CONF_MAX_OUTPUT_CURRENT,
    CONF_OUTPUT_POWER,
    CONF_OUTPUT_TEMP,
]


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_HUAWEI_R4850_ID): cv.use_id(HuaweiR4850Component),
            cv.Optional(CONF_INPUT_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_FLASH,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INPUT_FREQUENCY): sensor.sensor_schema(
                unit_of_measurement=UNIT_HERTZ,
                icon=ICON_FLASH,
                accuracy_decimals=3,
                device_class=DEVICE_CLASS_FREQUENCY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INPUT_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                icon=ICON_CURRENT_AC,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INPUT_POWER): sensor.sensor_schema(
                unit_of_measurement=UNIT_WATT,
                icon=ICON_POWER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INPUT_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_EFFICIENCY): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_PERCENT,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_OUTPUT_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_FLASH,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_OUTPUT_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                icon=ICON_CURRENT_AC,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            # cv.Optional(CONF_MAX_OUTPUT_CURRENT): sensor.sensor_schema(
            #     unit_of_measurement=UNIT_AMPERE,
            #     icon=ICON_CURRENT_AC,
            #     accuracy_decimals=2,
            #     device_class=DEVICE_CLASS_CURRENT,
            #     state_class=STATE_CLASS_MEASUREMENT,
            # ),
            cv.Optional(CONF_OUTPUT_POWER): sensor.sensor_schema(
                unit_of_measurement=UNIT_WATT,
                icon=ICON_POWER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_OUTPUT_TEMP): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def setup_conf(config, key, hub):
    if key in config:
        conf = config[key]
        sens = await sensor.new_sensor(conf)
        cg.add(getattr(hub, f"set_{key}_sensor")(sens))


async def to_code(config):
    hub = await cg.get_variable(config[CONF_HUAWEI_R4850_ID])
    for key in TYPES:
        await setup_conf(config, key, hub)
