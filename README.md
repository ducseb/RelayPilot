#### RelayPilot
ESP8266 Arduino code to control heater pilot wire and send Temp/Humidity to Domoticz server

RELAYPILOT V2.3.1
ESP8266 Arduino code to control heater pilot wire and send Temp/Humidity to Domoticz server

<img src="https://cloud.githubusercontent.com/assets/646980/10129874/4af6b26a-65c3-11e5-8261-0ce86ecc34dc.jpg" ></img> 



### Usage
- http://IP/temp :    Show temperature
- http://IP/hum :     Show humidity
- http://IP/tempsend:  Send temperature data to domoticz server
- http://IP/humsend: Send Humidity data to domoticz server
- http://IP/heatstatussend:  Send heater status do domoticz server (text sensor)
- http://IP/settings: Show setttings s ( host domoticz, port, ssid wifi, wifi password, etc...)
- http://IP/comfort:  Heater Confort mode
- http://IP/eco:      Heater Eco mode
- http://IP/hors-gel: Heater No-Freezing mode
- http://IP/arret:    Heater stop mode
- http://IP/APMode:   Activate AP mode for setting , SSID=device name , password = device name
- http://IP/reboot:   Reboot
- http://IP/update:  Update the module with OTA ( see http://esp8266.github.io/Arduino/versions/2.1.0-rc2/doc/ota_updates/ota_updates.html)
- http://IP/tempdata: Show raw temp data on body response
- http://IP/humiditydata: Show raw temp data on body response
- http://IP/statusdata: Show raw temp data on body response

Works with DS18B20 sensors or DHT11/DHT22 temp/humidity sensors

![Screenshot](/Photos/RelayPilot_Web.jpg)
![Screenshot](/Photos/RelayPilot3_circuit.png)



#### Required devices
- NodeMCU 0.9 or any ESP8266-12 ESP8266-12E compatible module
- DS18B20 temperature sensors or DHT11/DHT22 temperature/humidity sensors
- 2 NPN 2n2222 Transistor
- 2 4.7KΩ Resistor (for ESP8666 to Transistor base)
- 2 1KΩ Resistor (for 5V to Transistor emmiter)
- 2 1N4007 Diode (or any 230V 1A capable diode)

### Required Software and plugin ( plugin is available from Arduino Plugin manager)
- Arduino IDE 1.6.12
- Arduino for ESP8266 V.2.3.0
- DHT11/DHT22 Library https://github.com/adafruit/DHT-sensor-library
- DS18B20 DallasTemperature Library https://github.com/milesburton/Arduino-Temperature-Control-Library.git
- OneWire library http://www.pjrc.com/teensy/td_libs_OneWire.html
- Adafruit Unified Sensor Library https://github.com/adafruit/Adafruit_Sensor

- A Domoticz server 

### Usage
Go to src an open RelayPilot.ino in the ARduino IDE
Edit RelayPilotConfig.h with your config info (ssid wifi, password,etc...)


ChangeLog
V2.3.1 - 2016/02/01
------------------------------------
- Add Raw GET data value (temp, humidity and status)
- Add Jeedom mode for virtual device (like domoticz)
- Add SEND_DATA option, this option bypass data sending to domoticz or jeedom, you can configure Jeedom HTTP script to get data remotly

V2.2.1 - 2016/12/02
------------------------------------
- Heater status can be send to domoticz has text or boolean (on/off)
- Add device and time lua script for domoticz server

V2.2 - 2016/12/01
------------------------------------
- Add compatibility to latest ESP8266 Arduino SDK and Arduino IDE 1.6.12
- Add Heater status can now inform Domoticz server about is state 

V2.1 - 2016/02/11
------------------------------------
- Add few parameters to config file 
- Add OTA updade
- Readme change
- Build on ESP8266/arduino  V 2.1.0 RC2
- Add fix for 85°C boot value of DS18B20 


