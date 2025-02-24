import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_PERCENT,
    CONF_ID,
    CONF_ICON,
    CONF_UNIT_OF_MEASUREMENT,
    CONF_MODE,
    CONF_DEVICE_CLASS,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_VOLTAGE,
    ICON_CURRENT_AC,
    ICON_FAN,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    CONF_STEP,
)

from .. import HuaweiR4850Component, huawei_r4850_ns, CONF_HUAWEI_R4850_ID

ICON_CURRENT_DC = "mdi:current-dc"

CONF_OUTPUT_VOLTAGE = "output_voltage"
CONF_OUTPUT_VOLTAGE_DEFAULT = "output_voltage_default"
CONF_MAX_OUTPUT_CURRENT = "max_output_current"
CONF_MAX_OUTPUT_CURRENT_DEFAULT = "max_output_current_default"
CONF_MAX_AC_CURRENT = "max_ac_current"
CONF_FAN_DUTY_CYCLE = "fan_duty_cycle"

HuaweiR4850Number = huawei_r4850_ns.class_(
    "HuaweiR4850Number", number.Number, cg.Component
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_HUAWEI_R4850_ID): cv.use_id(HuaweiR4850Component),
            cv.Optional(CONF_OUTPUT_VOLTAGE): number.NUMBER_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(HuaweiR4850Number),
                    cv.Optional(CONF_MIN_VALUE, default=41.0): cv.float_,
                    cv.Optional(CONF_MAX_VALUE, default=58.6): cv.float_,
                    cv.Optional(CONF_STEP, default=0.1): cv.float_,
                    cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_VOLT): cv.string_strict,
                    cv.Optional(CONF_MODE, default="BOX"): cv.enum(number.NUMBER_MODES, upper=True),
                    cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_VOLTAGE): number.validate_device_class,
                }
            ),
            cv.Optional(CONF_OUTPUT_VOLTAGE_DEFAULT): number.NUMBER_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(HuaweiR4850Number),
                    cv.Optional(CONF_MIN_VALUE, default=48.0): cv.float_,
                    cv.Optional(CONF_MAX_VALUE, default=58.4): cv.float_,
                    cv.Optional(CONF_STEP, default=0.1): cv.float_,
                    cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_VOLT): cv.string_strict,
                    cv.Optional(CONF_MODE, default="BOX"): cv.enum(number.NUMBER_MODES, upper=True),
                    cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_VOLTAGE): number.validate_device_class,
                }
            ),
            cv.Optional(CONF_MAX_OUTPUT_CURRENT): number.NUMBER_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(HuaweiR4850Number),
                    cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                    cv.Optional(CONF_MAX_VALUE, default=63.3): cv.float_,
                    cv.Optional(CONF_STEP, default=0.1): cv.float_,
                    cv.Optional(CONF_ICON, default=ICON_CURRENT_DC): cv.icon,
                    cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE): cv.string_strict,
                    cv.Optional(CONF_MODE, default="BOX"): cv.enum(number.NUMBER_MODES, upper=True),
                    cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_CURRENT): number.validate_device_class,
                }
            ),
            cv.Optional(CONF_MAX_OUTPUT_CURRENT_DEFAULT): number.NUMBER_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(HuaweiR4850Number),
                    cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                    cv.Optional(CONF_MAX_VALUE, default=63.3): cv.float_,
                    cv.Optional(CONF_STEP, default=0.1): cv.float_,
                    cv.Optional(CONF_ICON, default=ICON_CURRENT_DC): cv.icon,
                    cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE): cv.string_strict,
                    cv.Optional(CONF_MODE, default="BOX"): cv.enum(number.NUMBER_MODES, upper=True),
                    cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_CURRENT): number.validate_device_class,
                }
            ),
            cv.Optional(CONF_MAX_AC_CURRENT): number.NUMBER_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(HuaweiR4850Number),
                    cv.Optional(CONF_MIN_VALUE, default=0): cv.float_,
                    cv.Optional(CONF_MAX_VALUE, default=20): cv.float_,
                    cv.Optional(CONF_STEP, default=0.1): cv.float_,
                    cv.Optional(CONF_ICON, default=ICON_CURRENT_AC): cv.icon,
                    cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE): cv.string_strict,
                    cv.Optional(CONF_MODE, default="BOX"): cv.enum(number.NUMBER_MODES, upper=True),
                    cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_CURRENT): number.validate_device_class,
                }
            ),
            cv.Optional(CONF_FAN_DUTY_CYCLE): number.NUMBER_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(HuaweiR4850Number),
                    cv.Optional(CONF_MIN_VALUE, default=0): cv.float_range(min=0, max=100),
                    cv.Optional(CONF_MAX_VALUE, default=100): cv.float_range(min=0, max=100),
                    cv.Optional(CONF_STEP, default=1): cv.float_,
                    cv.Optional(CONF_ICON, default=ICON_FAN): cv.icon,
                    cv.Optional(CONF_UNIT_OF_MEASUREMENT, default=UNIT_PERCENT): cv.string_strict,
                    cv.Optional(CONF_MODE, default="SLIDER"): cv.enum(number.NUMBER_MODES, upper=True),
                }
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_HUAWEI_R4850_ID])
    if CONF_OUTPUT_VOLTAGE in config:
        conf = config[CONF_OUTPUT_VOLTAGE]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await number.register_number(
            var,
            conf,
            min_value=conf[CONF_MIN_VALUE],
            max_value=conf[CONF_MAX_VALUE],
            step=conf[CONF_STEP],
        )
        cg.add(getattr(hub, "register_input")(var))
        cg.add(var.set_parent(hub, 0x100))
    if CONF_OUTPUT_VOLTAGE_DEFAULT in config:
        conf = config[CONF_OUTPUT_VOLTAGE_DEFAULT]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await number.register_number(
            var,
            conf,
            min_value=conf[CONF_MIN_VALUE],
            max_value=conf[CONF_MAX_VALUE],
            step=conf[CONF_STEP],
        )
        cg.add(getattr(hub, "register_input")(var))
        cg.add(var.set_parent(hub, 0x101))
    if CONF_MAX_OUTPUT_CURRENT in config:
        conf = config[CONF_MAX_OUTPUT_CURRENT]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await number.register_number(
            var,
            conf,
            min_value=conf[CONF_MIN_VALUE],
            max_value=conf[CONF_MAX_VALUE],
            step=conf[CONF_STEP],
        )
        cg.add(getattr(hub, "register_input")(var))
        cg.add(var.set_parent(hub, 0x103))
    if CONF_MAX_OUTPUT_CURRENT_DEFAULT in config:
        conf = config[CONF_MAX_OUTPUT_CURRENT_DEFAULT]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await number.register_number(
            var,
            conf,
            min_value=conf[CONF_MIN_VALUE],
            max_value=conf[CONF_MAX_VALUE],
            step=conf[CONF_STEP],
        )
        cg.add(getattr(hub, "register_input")(var))
        cg.add(var.set_parent(hub, 0x104))
    if CONF_MAX_AC_CURRENT in config:
        conf = config[CONF_MAX_AC_CURRENT]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await number.register_number(
            var,
            conf,
            min_value=conf[CONF_MIN_VALUE],
            max_value=conf[CONF_MAX_VALUE],
            step=conf[CONF_STEP],
        )
        cg.add(getattr(hub, "register_input")(var))
        cg.add(var.set_parent(hub, 0x109))
    if CONF_FAN_DUTY_CYCLE in config:
        conf = config[CONF_FAN_DUTY_CYCLE]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await number.register_number(
            var,
            conf,
            min_value=0.0,
            max_value=100.0,
            step=conf[CONF_STEP],
        )
        cg.add(getattr(hub, "register_input")(var))
        cg.add(var.set_parent(hub, 0x114))
