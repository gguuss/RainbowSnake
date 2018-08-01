# RainbowSnake
LED Server with One-button interface and mobile phone sensor bridge.

# Quickstart
* See `config.h` for pin settings and board configurations.
* Connect APA102 as specified in `config.h`.
* Install data folder using [ESP8266 FS tool](https://github.com/esp8266/Arduino/blob/master/doc/filesystem.rst)
  * Note that you should be using 3M SPIFFS, the files are bloaty.
* Upload sketch and wait for board to come up.
* Connect to `Rainbow Snake` AP, server [192.168.4.1](http://192.168.4.1).

# Notes
The ESP8266 is terrible at serving multiple files so the app is optimized as such
with bootstrap and jquery embedded in script.js/style.css. I recommend minifying your
javascript, and there's potential to gzip it for even better performance.

Known Issues
* The current configuration for the Feather does not work with FastLed, I blame pin 16.
* The SPA stops transmitting when the screen is turned off.

# Hardware
If you are on this repo, find @gguuss, there is hardware for you.

# Demos

Changing Node Patterns using 1-Button Interface 

[![Mesh Network Node Change](https://img.youtube.com/vi/vfW2BwxKj68/0.jpg)](https://www.youtube.com/watch?v=vfW2BwxKj68 "Mesh Network Node Change")

Storing Waypoint using phone GPS and presenting distance to waypoint

[![GPS waypoint distance](https://img.youtube.com/vi/qmCWH-nlgYE/0.jpg)](https://www.youtube.com/watch?v=qmCWH-nlgYE "GPS waypoint distance")

Resizing the strip size for control of large LED arrays

[![Control Large LED arrays](https://img.youtube.com/vi/dDma-7SCzwo/0.jpg)](https://www.youtube.com/watch?v=dDma-7SCzwo "Control Large LED arrays")
