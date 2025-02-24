import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    CONF_ID,
    CONF_ICON,
    ICON_FAN,
    ICON_POWER,
)

from .. import HuaweiR4850Component, huawei_r4850_ns, CONF_HUAWEI_R4850_ID


CONF_FAN_SPEED_MAX = "fan_speed_max"
CONF_STANDBY = "standby"

HuaweiR4850Switch = huawei_r4850_ns.class_(
    "HuaweiR4850Switch", switch.Switch, cg.Component
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_HUAWEI_R4850_ID): cv.use_id(HuaweiR4850Component),
            cv.Optional(CONF_FAN_SPEED_MAX): switch.switch_schema(HuaweiR4850Switch, icon=ICON_FAN),
            cv.Optional(CONF_STANDBY): switch.switch_schema(HuaweiR4850Switch, icon=ICON_POWER),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_HUAWEI_R4850_ID])
    if CONF_FAN_SPEED_MAX in config:
        conf = config[CONF_FAN_SPEED_MAX]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await switch.register_switch(var, conf)
        cg.add(getattr(hub, "register_input")(var))
        cg.add(var.set_parent(hub, 0x134))
    if CONF_STANDBY in config:
        conf = config[CONF_STANDBY]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await switch.register_switch(var, conf)
        cg.add(getattr(hub, "register_input")(var))
        cg.add(var.set_parent(hub, 0x132))
