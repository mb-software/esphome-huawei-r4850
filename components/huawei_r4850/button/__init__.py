import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ENTITY_CATEGORY, ENTITY_CATEGORY_CONFIG, CONF_ID

from .. import HuaweiR4850Component, huawei_r4850_ns, CONF_HUAWEI_R4850_ID

HuaweiR4850Button = huawei_r4850_ns.class_(
    "HuaweiR4850Button", button.Button, cg.Component
)

CONF_SET_OFFLINE_VALUES = "set_offline_values"

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_HUAWEI_R4850_ID): cv.use_id(HuaweiR4850Component),
            cv.Optional(CONF_SET_OFFLINE_VALUES): button.BUTTON_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(HuaweiR4850Button),
                    cv.Optional(
                        CONF_ENTITY_CATEGORY, default=ENTITY_CATEGORY_CONFIG
                    ): cv.entity_category,
                }
            ),
        }
    ).extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_HUAWEI_R4850_ID])
    if config[CONF_SET_OFFLINE_VALUES]:
        conf = config[CONF_SET_OFFLINE_VALUES]
        var = cg.new_Pvariable(conf[CONF_ID])
        await cg.register_component(var, conf)
        await button.register_button(
            var,
            conf,
        )
        cg.add(var.set_parent(hub))
