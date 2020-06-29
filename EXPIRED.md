# RAIN SENSOR DETECTION OR MEASUREMENT

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