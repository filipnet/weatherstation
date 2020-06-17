# Changelog

## 2020-05-17 | BUGFIX | No huminity values received on mqtt broker

***ISSUE:*** Equalization of the function "void readsensor_bme280()"
The BME280 sensor sent values for temperature, pressure but not for humidity. The value is empty before sending to the MQTT broker.

***SOLUTION:*** Equalization of function 1.) Read temperature 2.) Convert temperature 3.) publish temperature to MQTT broker 4.) Delay 100ms and then the next sensor value.