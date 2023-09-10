# esphome-huawei-r4850

[![License][license-shield]](LICENSE)


<a href="https://www.buymeacoffee.com/mbsoftware"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=mbsoftware&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff" /></a>

## EspHome Component to control and read values from a huawei r4850 power supply via CAN bus.

### Requirements
This component is currently tested and verified to work on a ESP32 using the Esphome **esp32_can** platform. So in addition to any **ESP32 board** of your choice you need a CAN tranceiver like the **sn65hvd230**. These are cheap and available from many sources and can be wired directly to the 3.3v GPIO and supply pins of the ESP32 board.
In theory it should work also with the mcp2515 platform, but this is currently not tested and verified.

### How to use
Please check the provided example esphome configuration:
[huawei_r4850.yaml](https://github.com/mb-software/esphome-huawei-r4850/blob/main/huawei_r4850.yaml)


[license-shield]: https://img.shields.io/github/license/mb-software/homeassistant-powerbrain.svg?style=for-the-badge
