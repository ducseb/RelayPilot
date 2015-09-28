#### RelayPilot
ESP8266 Arduino code to control heater pilot wire and send Temp/Humidity to Domoticz server


RELAYPILOT V2
BY DUCSEB
ESP8266 Arduino code to control heater pilot wire and send Temp/Humidity to Domoticz server



### Usage
http://IP/temp :    Show temperature
http://IP/hum :     Show humidity
http://IP/settings: Show setttings s ( host domoticz, port, ssid wifi, wifi password, etc...)
http://IP/confort:  Heater Confort mode
http://IP/eco:      Heater Eco mode
http://IP/hors-gel: Heater No-Freezing mode
http://IP/arret:    Heater stop mode
http://IP/APMode:   Activate AP mode for setting , SSID=device name , password = device name
http://IP/reboot:   Reboot =
Works with DS18B20 sensors or DHT11/DHT22 temp/humidity sensors



#### Required devices
- NodeMCU 0.9 or any ESP8266-12 compatible module
- DS18B20 temperature sensors or DHT11/DHT22 temperature/humidity sensors
- 2 NPN 2n2222 Transistor
- 2 4.7KΩ Resistor (for ESP8666 to Transistor base)
- 2 1KΩ Resistor (for 5V to Transistor emmiter)
- 2 Diode 1N4007 (or any 230V 1A capable diode)

### Required Software
- Arduino IDE for ESP8266 (https://github.com/esp8266/Arduino)
- DHT11/DHT22 Library https://github.com/adafruit/DHT-sensor-library
- DS18B20 DallasTemperature Library 
- A Domoticz server 

### Usage
Go to src an open RelayPilot.ino in the ARduino IDE
Edit RelayPilotConfig.h with your config info (ssid wifi, password,etc...)
