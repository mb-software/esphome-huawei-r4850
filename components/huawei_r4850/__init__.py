import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.canbus import CanbusComponent
from esphome.const import CONF_ID

CONF_CANBUS_ID = "canbus_id"
CONF_HUAWEI_R4850_ID = "huawei_r4850_id"

huawei_r4850_ns = cg.esphome_ns.namespace("huawei_r4850")
HuaweiR4850Component = huawei_r4850_ns.class_(
    "HuaweiR4850Component", cg.PollingComponent
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HuaweiR4850Component),
        cv.Required(CONF_CANBUS_ID): cv.use_id(CanbusComponent),
    }
).extend(cv.polling_component_schema("5s"))


async def to_code(config):
    canbus = await cg.get_variable(config[CONF_CANBUS_ID])
    var = cg.new_Pvariable(config[CONF_ID], canbus)
    await cg.register_component(var, config)
