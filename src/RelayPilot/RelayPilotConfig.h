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

