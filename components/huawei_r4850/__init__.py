import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components.canbus import CanbusComponent
from esphome.const import CONF_ID

MULTI_CONF = True

CONF_CANBUS_ID = "canbus_id"
CONF_HUAWEI_R4850_ID = "huawei_r4850_id"
CONF_PSU_ADDRESS = "psu_address"
CONF_PSU_MAX_CURRENT = "psu_max_current"

huawei_r4850_ns = cg.esphome_ns.namespace("huawei_r4850")
HuaweiR4850Component = huawei_r4850_ns.class_(
    "HuaweiR4850Component", cg.PollingComponent
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HuaweiR4850Component),
        cv.Required(CONF_CANBUS_ID): cv.use_id(CanbusComponent),
        cv.Required(CONF_PSU_ADDRESS): cv.int_range(min=1, max=127),
        cv.Optional(CONF_PSU_MAX_CURRENT, default=63.3): cv.float_range(min=0, min_included=False),
    }
).extend(cv.polling_component_schema("5s"))


async def to_code(config):
    canbus = await cg.get_variable(config[CONF_CANBUS_ID])
    canbus_var = cg.new_Pvariable(config[CONF_ID], canbus)
    await cg.register_component(canbus_var, config)

    hub = await cg.get_variable(config[CONF_ID])

    address = config[CONF_PSU_ADDRESS]
    cg.add(hub.set_psu_address(address))

    max_current = config[CONF_PSU_MAX_CURRENT]
    cg.add(hub.set_psu_max_current(max_current))
