# RelayPilot
ESP8266 Arduino code to control heater pilot wire and send Temp/Humidity to Domoticz server


// RELAYPILOT V2
// BY DUCSEB
// ESP8266 Arduino code to control heater pilot wire and send Temp/Humidity to Domoticz server
// USAGE:
//  http://IP/temp :    Show temperature
//  http://IP/hum :     Show humidity
//  http://IP/settings: Show setttings s ( host domoticz, port, ssid wifi, wifi password, etc...)
//  http://IP/confort:  Heater Confort mode
//  http://IP/eco:      Heater Eco mode
//  http://IP/hors-gel: Heater No-Freezing mode
//  http://IP/arret:    Heater stop mode
//  http://IP/APMode:   Activate AP mode for setting , SSID=device name , password = device name
//  http://IP/reboot:   Reboot =
//Works with DS18B20 sensors or DHT11/DHT22 temp/humidity sensors
