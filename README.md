# WeatherStation

Your personal weather station with an Arduino D1 Mini MQTT and Node-RED

<img src="images/wetter-dot-com-logo.png" alt="Wetter.com Logo" width="200" align="right"/>
Proud to be part of the "Wetter.com Network" with weather station 19782. (https://netzwerk.wetter.com/wetterstation/19782/)

<!-- TOC -->

- [WeatherStation](#weatherstation)
    - [FEATURES](#features)
    - [REQUIREMENTS](#requirements)
        - [ARDUINO AND SENSORS](#arduino-and-sensors)
        - [CASE FOR SENSORS](#case-for-sensors)
    - [BOARDS](#boards)
        - [BOARD FOR AC-DC TRANSFORMATOR](#board-for-ac-dc-transformator)
        - [BOARD FOR ARDUINO AND MULTILTUPLEXER](#board-for-arduino-and-multiltuplexer)
        - [BOARD FOR SENSORS](#board-for-sensors)
        - [PROTECTION OF BME280](#protection-of-bme280)
    - [INSTALL AND USAGE](#install-and-usage)
        - [PINOUT](#pinout)
        - [CONNECTION BRIDGE](#connection-bridge)
        - [LIBRARIES](#libraries)
    - [DIRECTORIES AND FILES](#directories-and-files)
    - [AUTOMATION WITH NODE-RED](#automation-with-node-red)
        - [PLUGIN REQUIREMENTS](#plugin-requirements)
        - [NODE-RED FLOW](#node-red-flow)
        - [POLLINATION](#pollination)
        - [MQTT TOPICS](#mqtt-topics)
    - [UPCOMING DEVELOPMENTS IN THIS PROJECT](#upcoming-developments-in-this-project)
    - [LICENSE](#license)

<!-- /TOC -->

## FEATURES

- Connects to the Wireless Access Point / Wifi Router
- Securely connects to the MQTT Broker (TLS)
- Subscribes to the desired MQTT topics
- Handles incoming MQTT messages
- Controls ESP8266 Chip Level Data Pins
- Sends a heartbeat signal to the MQTT broker periodically
- Determine the weather values and publish via MQTT to the broker:
    - Temperature in *C
    - Humidity in %
    - Air pressure in hPa
    - Brightness in %

## REQUIREMENTS

### ARDUINO AND SENSORS

- Wemos D1 Mini (should work with other Arduino microcontrollers too)
- PlatformIO or Arduino IDE (for Arduino IDE you have to move files from /src to root-folder)
- MQTT Broker such as the Mosquitto MQTT Broker (https://mosquitto.org/)
- ControllApp such as MQTT Dash (https://play.google.com/store/apps/details?id=net.routix.mqttdash&hl=en)
- MH Sensor Series Flying-Fish Light Detector analog digital
- GY-BME280 Barometric sensor for temperature, humidity and air pressure (7,99 €) Make sure that you really order the BME and not the BMP sensor, as this can only measure temperature and air pressure but not relative humidity.

### CASE FOR SENSORS 

- Hexagon nuts M6 DIN 934 cl.8 galvanized 100x (Sechskantmuttern M6 DIN 934 Kl.8 galvanisch verzinkt)
- Threaded rod M6 for self-cutting galvanized (Gewindestange M6 galv. verzinkt zum selbst zuschneiden)
- Transparent orchid saucher 13,5cm x 2,0cm (Transparente Blumentopfuntersetzer für Orchideen)
- OBO Bettermann 2007093 Junction box (L x B x H) 190 x 150 x 77mm Light grey (RAL 7035) IP66

## BOARDS

The Generation 1 weatherstation was simply plugged onto a spreadboard and installed inside the OBO box. Generation 2 should now consist of different boards. I am not a friend of fixed soldering and so I decided to use screw terminals and PINs. The idea behind it is, to connect the "Arduino and Multiplexer board" with the "Sensor board" via screw terminals and a suitable multi-core cable. This allows each module to be easily removed at any time.

<img src="images/weatherstation_gen2_collection_diagonal.jpg" alt="Boards overview diagonal" width="400"/>

### BOARD FOR AC-DC TRANSFORMATOR

I have used this AC-DC transformer many times for Arduino projects. The PINs fit exactly into the standard hole pattern of boards. This makes it very easy to connect them with two screw terminals on the bottom of the board. Attention, there should be a sufficient isolation of the 230V phases.

<img src="images/weatherstation_gen2_acdc.jpg" alt="Board AC-DC transformator" width="400"/>

### BOARD FOR ARDUINO AND MULTILTUPLEXER

It is not easy to find an optimal layout. It was important to me personally,
- that the 5V/GND inputs are connected directly to the PINs of the Arduino,
- several 5V, 3V and GND-PINs are available, as these are the most commonly used,
- the GPIO-PINs can be flexibly connected to the screw terminals like on a patch panel.

<img src="images/weatherstation_gen2_arduino_diagonal.jpg" alt="Board arduino and multiplexer diagonal" width="300"/><img src="images/weatherstation_gen2_arduino_top.jpg" alt="Board arduino and multiplexer top" width="300"/><img src="images/weatherstation_gen2_arduino_wired.jpg" alt="Board arduino and multiplexer wired" width="300"/>

### BOARD FOR SENSORS

I have dimensioned the sensor board, so that it fits inside the self-built sensor case. The hole in the middle is prepared for a 10x10 cm 5V-fan, which I want to use later in a "solar project" for ventilation to protect against trapped heat to get representative values in midsummer. 

The concept is similar to the "Arduino and multiplexer board" again:
- The three left screw terminals are for 3V, GND and 5V
- All other PINs are intended for GPIO to sensor PIN
- The connection is made again by patching from PIN to PIN

<img src="images/weatherstation_gen2_sensor_diagonal.jpg" alt="Board sensors diagonal" width="400"/><img src="images/weatherstation_gen2_sensor_top.jpg" alt="Board sensors top" width="400"/>

### PROTECTION OF BME280

To avoid condensation and damage to the sensor, it is located in a weatherproof but air-permeable case. I covered the BME280 sensor with PTFE tape. This Teflon-based tape is usually used to seal threads and is available in DIY stores. Teflon is permeable to water vapor, while it protects the sensor from pollen, dirt particles and insects.  

<img src="images/weatherstation_sensor_case.jpg" alt="BME280 PTFE Tape" height="300"/>
<img src="images/bme280_ptfe_tape.jpg" alt="BME280 PTFE Tape" height="300"/>

## INSTALL AND USAGE

### PINOUT

| Sensor-Description | Sensor-PIN | Arduino-PIN | GPIO | Comment |
| --- | --- | --- | --- | --- |
| ROBOT AC-DC | AC | 230V |  | Any on L or N, because ac current |
| ROBOT AC-DC | AC | 230V |  | Any on L or N, because ac current |
| ROBOT AC-DC | +Vo | VCC+ 5V | - | - |
| ROBOT AC-DC | -Vo | GND | - | - |
| MH Sensor Series Flying-Fish Light Detector | A0 | Y0 | - | Multiplexer A1 Channel 0 |
| MH Sensor Series Flying-Fish Light Detector | D0 | - | - | only analog |
| MH Sensor Series Flying-Fish Light Detector | GND | GND | - | - |
| MH Sensor Series Flying-Fish Light Detector | VCC+ 3V | 3.3 V | - | - |
| BME280 Temperature, Humidity, Pressure-Sensor | VIN / VCC+ 3V | 3.3 V | - | - |
| BME280 Temperature, Humidity, Pressure-Sensor | SCL | D1 | GPIO5 | SCL |
| BME280 Temperature, Humidity, Pressure-Sensor | SDA | D2 | GPIO4 | SDA |
| BME280 Temperature, Humidity, Pressure-Sensor | GND | GND | - | - |
| 4051 74HC4051 8-Channel-Analog-Multiplexer-Modul | S0 | D5 | GPIO14 | - |
| 4051 74HC4051 8-Channel-Analog-Multiplexer-Modul | S1 | D6 | GPIO12 | - |
| 4051 74HC4051 8-Channel-Analog-Multiplexer-Modul | S2 | D7 | GPIO13 | - |
| 4051 74HC4051 8-Channel-Analog-Multiplexer-Modul | Z | A0 | A0 | Analog PIN |
| 4051 74HC4051 8-Channel-Analog-Multiplexer-Modul | VCC | 5.0 V | - | left side |
| 4051 74HC4051 8-Channel-Analog-Multiplexer-Modul | GND | GND | - | right side |
| Arduino D1-Mini | RST | D0 | GPIO16 | Deep sleep reset bridge if you would like to activate |

### CONNECTION BRIDGE

| Sensor-board | Cable color | Arduino- and Multiplexer Board |
| --- | --- | --- |
| BME280 SDA | blue / white | D2 |
| BME280 SCL | blue | D1 |
| - | green / white | Y1 |
| Flying-Fish | green | Y0 |
| - | brown / white | - |
| - | brown | - |
| BME280 + Flying-Fish | orange / white | GND |
| BME280 + Flying-Fish | orange | 3.3 V |

<img src="images/connection_bridge.png" alt="Colormap network cable connection bridge" width="900"/>

### LIBRARIES

- PubSubClient by Nick O'Leary
- esp8266_mdns by dunk
- Adafruit BME280 Library by Adafruit
- Adafruit Unified Sensor by Adafruit

## DIRECTORIES AND FILES

- src/WeatherStation.ino - The main programm code
- src/config.h - GPIO configuration, definition of threshold values, etc.
- src/credentials.h.sample - Sample configuration file for WiFi and MQTT credentials (please rename to secrets.h)
- README.md - The manual for this Arduino Script
- LICENSE - The license notes for this Arduino script
- platformio.ini - Wemos D1 Mini Configuration for PlatformIO

## AUTOMATION WITH NODE-RED

### PLUGIN REQUIREMENTS

- node-red-contrib-schedex
- node-red-contrib-telegrambot (optional if you would like to get status messages)

### NODE-RED FLOW

Node-RED works with so-called flows, which can be imported and exported. 
An example of a flow can be found in **flows.json** in the github repository.

### POLLINATION

If you are interested about pollination, take a look at this GitHub repository: https://github.com/filipnet/nodered-dwd-pollination

### MQTT TOPICS

| MQTT Topic | Description | Possible values |
| --- | --- | --- |
| home/outdoor/weather/temperature | Sensor result temperature in *C | {value} |
| home/outdoor/weather/humidity | Sensor result humidity in % (percent) | {value} |
| home/outdoor/weather/pressure | Sensor result Pressure in hPa (hectopascal) | {value} |
| home/outdoor/weather/altitude | Sensor result approx altitude in m (meter) | {value} |
| home/outdoor/weather/brightness/raw | Sensor result brightness | {value} min. 21 = bright, max. 864 = dark |
| home/outdoor/weather/rain/raw | Sensor result rain | {value} |
| home/outdoor/weather/heartbeat | Heartbeat for troubleshooting | on |

## UPCOMING DEVELOPMENTS IN THIS PROJECT

[For reading about upcoming developments in this project](DEVELOPMENT.md)

## LICENSE

WeatherStation and all individual scripts are under the BSD 3-Clause license unless explicitly noted otherwise. Please refer to the LICENSE