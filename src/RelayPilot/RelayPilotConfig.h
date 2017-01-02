//This is the user configuration file

#define SSIDWIFI "WifiSSIDHere" //SSID wifi (for client mode)
#define WIFIPASSWORD "WifiPasswordHere" // Wifi password
#define HOSTDOMOTICZ "domoticz.hostname.here" //DOmoticz hostname ou IP
#define HOSTDOMOTICZPORT 8086 //Domoticz Port
#define MODULENAME "RELAYPLT1" //Module name
#define DHCPMODE true //DHCP Mode
#define IPMODULE {192, 168, 0, 193} // Client IP Adress for the ESP8266
#define IPGATEWAY {192, 168, 0, 1} // Gateway IP 
#define MASK {255, 255, 255, 0} // Netmask
#define DNSSERVER {8,8,8,8} //DNS Server
#define IDCAPTEURDOMOTICZTEMP 6 // Device ID of temperature sensors on domoticz
#define IDCAPTEURDOMOTICZHUM 6 //Device of humidity sensors on domoticz
#define SEND_HUMIDITY_DHT22 false // Send Humidity with temp to domoticz if you use DHT11/DHT22
#define SEND_HEATER_STATUS_DOMOTICZ false // Send the heater status (on/off) to Domoticz server
#define HEATER_STATUS_TEXT_MODE  false // Set heater status has text value in domoticz ( instead of On Off value)
#define ID_HEATER_DOMOTICZ 0 //Domoticz heater ID 

#define SENSOR_PIN_DS18B20 D1  // DS18B20 pin (D1 on ESP8266 NodeMCU dev board)
#define SENSOR_PIN_DHT D2  // DHT pin (D1 on ESP8266 NodeMCU dev board)
#define RELAY1_PIN D7  // RELAY PIN 1 : ALTERNANCE NEGATIVE
#define RELAY2_PIN D8  // RELAY PIN 2 : ALTERNANCE POSITIVE
#define PIN_MODE_CONFIG D0 //PIN POUR LE MODE CONFIG

#define DELAY_BETWEEN_SENSORS_SEND_TO_DOMOTICZ 20

#define DOMOTICZ_MODE true
#define JEEDOM_MODE false

#define JEEDOM_API_KEY "xxxxxxxxxxxxxx" // APIKEY of jeedom instance
#define HOST_JEEDOM "domoticz.hostname or IP here" //JEEDOM host
#define PORT_JEEDOM 80 // IP port of JEEDOM API
#define JEEDOMDEVICEIDTEMP 0 //Device ID for virtual temp sensors on Jeedom
#define JEEDOMDEVICEIDHUM 0//Device ID for virtual humidity sensors on Jeedom

//#include  "RelayPilotConfig_bureau.h" //Import config
//#include  "RelayPilotConfig_chambre.h" //Import config
//#include  "RelayPilotConfig_salon.h" //Import config
//#include  "RelayPilotConfig_taff.h" //Import config



