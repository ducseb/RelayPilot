//This is the default configuration file


#ifndef SSIDWIFI
#define SSIDWIFI "WifiSSIDHere" //SSID wifi (for client mode)
#endif

#ifndef WIFIPASSWORD
#define WIFIPASSWORD "WifiPasswordHere" // Wifi password
#endif

#ifndef HOSTDOMOTICZ
#define HOSTDOMOTICZ "domoticz hostname here" //DOmoticz hostname ou IP
#endif

#ifndef HOSTDOMOTICZPORT
#define HOSTDOMOTICZPORT 8086 //Domoticz Port
#endif

#ifndef MODULENAME
#define MODULENAME "RELAYPLT1" //Module name
#endif

#ifndef DHCPMODE
#define DHCPMODE true //DHCP Mode
#endif

#ifndef IPMODULE
#define IPMODULE {192, 168, 0, 193} // Client IP Adress for the ESP8266
#endif

#ifndef IPGATEWAY
#define IPGATEWAY {192, 168, 0, 1} // Gateway IP 
#endif

#ifndef MASK
#define MASK {255, 255, 255, 0} // Netmask
#endif

#ifndef DNSSERVER
#define DNSSERVER {8,8,8,8} //DNS Server
#endif

#ifndef IDCAPTEURDOMOTICZTEMP
#define IDCAPTEURDOMOTICZTEMP 1 // Device ID of temperature sensors on domoticz
#endif

#ifndef IDCAPTEURDOMOTICZHUM
#define IDCAPTEURDOMOTICZHUM 1 //Device of humidity sensors on domoticz
#endif
