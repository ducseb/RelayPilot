//This is the user configuration file

#define SSIDWIFI "sebwifidlink" //SSID wifi (for client mode)
#define WIFIPASSWORD "caas0302nnts6382" // Wifi password
#define HOSTDOMOTICZ "192.168.0.10" //DOmoticz hostname ou IP
#define HOSTDOMOTICZPORT 8084 //Domoticz Port
#define MODULENAME "MODBASIC" //Module name
#define DHCPMODE true //DHCP Mode
#define IPMODULE {192, 168, 0, 194} // Client IP Adress for the ESP8266
#define IPGATEWAY {192, 168, 0, 253} // Gateway IP 
#define MASK {255, 255, 255, 0} // Netmask
#define DNSSERVER {192,168,0,253} //DNS Server
#define IDCAPTEURDOMOTICZTEMP 100 // Device ID of temperature sensors on domoticz
#define IDCAPTEURDOMOTICZHUM 101 //Device of humidity sensors on domoticz

#define SENSOR_PIN_DS18B20 D1  // DS18B20 pin (D1 on ESP8266 NodeMCU dev board)
#define SENSOR_PIN_DHT D2  // DHT pin (D1 on ESP8266 NodeMCU dev board)
#define RELAY1_PIN D7  // RELAY PIN 1 : ALTERNANCE NEGATIVE
#define RELAY2_PIN D8  // RELAY PIN 2 : ALTERNANCE POSITIVE
#define PIN_MODE_CONFIG D0 //PIN POUR LE MODE CONFIG



//#include  "RelayPilotConfig_bureau.h" //Import config
//#include  "RelayPilotConfig_chambre.h" //Import config
//#include  "RelayPilotConfig_salon.h" //Import config
#include  "RelayPilotConfig_taff.h" //Import config



