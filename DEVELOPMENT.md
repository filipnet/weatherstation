# WeatherStation GEN3

This manual is only a description of the future development of the project.
[The current manual of the project can be found here](README.md)

<!-- TOC -->

- [WeatherStation GEN3](#weatherstation-gen3)
    - [OPERATION VIA A SOLAR CELL](#operation-via-a-solar-cell)
        - [TEMPERATURE SENSOR COOLING BY SOLAR](#temperature-sensor-cooling-by-solar)
        - [PINOUT (SOLAR COMPONENTS)](#pinout-solar-components)
    - [MORE SENSORS](#more-sensors)
        - [RAIN SENSOR DETECTION OR MEASUREMENT](#rain-sensor-detection-or-measurement)
    - [LICENSE](#license)

<!-- /TOC -->

## OPERATION VIA A SOLAR CELL

The personal weather station powered by solar energy has a very special charm. Unfortunately this project was not successful. After many nice hours with which I was allowed to occupy myself with the topic solar energy, the concept was created at some point, the construction plan was created, the modules for this were implemented.

The weather station has the complete summer months with
- a 12V solar panel,
- a DC-DC converter from 12V to 5V,
- two charging modules connected in parallel,
- and two lithium-ion batteries
functions satisfactorily. Unfortunately only in midsummer, with warm temperatures and lots of sunshine during the day. Then came the autumn, the first rainy and overcast days and the lithium-ion batteries did not manage to bring the Arduino with sensors over the night. During the autumn I was able to save the project with Deepsleep and only periodic operation (every half hour) and sending the sensor data. In the long, dark winter months in Germany the Arduino "died", but especially here it makes sense to have access to weather data.

### TEMPERATURE SENSOR COOLING BY SOLAR

Hybrid operation via a power supply unit and solar module is no option, so what to do with the solar module? During a family trip to the Offenbach weather park (German weather service) I learned, that the measurement of the temperature should not take place in the shade but in the sun in a ventilated case (I already have). In order to avoid heat accumulation during sunshine, these cases are actively ventilated. 

If the sun is shining, the solar module could operate a 10x10cm computer fan and blow away the heat from the sensor (climate-neutral), the values would be representative. During the night, the sun does not shine, i.e. the need for the fan to rotate is not given. A reasonable use for the module.

### PINOUT (SOLAR COMPONENTS)

| Description | PIN | Color | Target | Comment |
| --- | --- | --- | ---| ---- |
| Offgridtec 12V solar panel | OUT 12V - | BLUE | Connected to DC-DC IN 12V - (minus) | --- |
| Offgridtec 12V solar panel | OUT 12V + | BROWN | Connected to DC-DC IN 12V + (plus) | --- |
| Offgridtec 12V solar panel | OUT 12V + | YELLOW | --- | PE not connected |
| Chuangruifa Car Power DC-DC 15W 12V IN 5V/3A OUT | IN 12V + | RED | --- | --- |
| Chuangruifa Car Power DC-DC 15W 12V IN 5V/3A OUT | IN 12V - | BLACK | --- | --- |
| Chuangruifa Car Power DC-DC 15W 12V IN 5V/3A OUT | OUT 5V - | YELLOW | --- | --- |
| Chuangruifa Car Power DC-DC 15W 12V IN 5V/3A OUT | OUT 5V + | RED | --- | --- |

## MORE SENSORS

### RAIN SENSOR DETECTION OR MEASUREMENT

In the weather station generation 1 I have installed a Sodial Rain Sensor, unfortunately this looks no longer good after one year of operation. The sensor plate has been badly damaged by corrosion, here another solution is needed.

<img src="images/weatherstation_gen1_rainsensor_corrosion.jpg" alt="Corrosion on rainsensor after 2 years" width="400"/>

| Sensor-Description | Sensor-PIN | Arduino-PIN | GPIO | Comment |
| --- | --- | --- | --- | --- |
| SODIAL Rain-Sensor | A0 | Y1 | - | Multiplexer A1 Channel 1 |
| SODIAL Rain-Sensor | D0 | - | - | only analog |
| SODIAL Rain-Sensor | GND | GND | - | - |
| SODIAL Rain-Sensor | VCC+ 3V | 3.3 V | - | - |

## LICENSE

WeatherStation and all individual scripts are under the BSD 3-Clause license unless explicitly noted otherwise. Please refer to the LICENSE