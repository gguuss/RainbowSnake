# RainbowSnake
LED Server with One-button interface and mobile phone sensor bridge.

# Quickstart
* See `config.h` for pin settings and board configurations.
* Connect APA102 as specified in `config.h`.
* Install data folder using [ESP8266 FS tool](https://github.com/esp8266/Arduino/blob/master/doc/filesystem.md#uploading-files-to-file-system)
* Upload sketch and wait for board to come up.
* Connect to `Rainbow Snake` AP, server [192.168.4.1](http://192.168.4.1).

# Notes
The ESP8266 is terrible at serving multiple files so the app is optimized as such
with bootstrap and jquery embedded in script.js/style.css.

Known Issues
* The current configuration for the Feather does not work with FastLed, I blame pin 16.
* The SPA stops transmitting when the screen is turned off.

# Hardware
If you are on this repo, find @gguuss, there is hardware for you.
