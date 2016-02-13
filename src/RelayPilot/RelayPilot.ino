

// RELAYPILOT V2
// BY DUCSEB
// CONTROLE DE FIL PILOTE RADIATEUR  VIA UN MODULE ESP8266 ET UN SERVEUR DOMITICZ
// USAGE:
//  http://IP/temp :    Affichage de la temperature
//  http://IP/hum :     Affichage de l'humidite
//  http://IP/settings: Réglages des paramêtres ( host domoticz, port, ssid wifi, wifi password, etc...)
//  http://IP/confort:  Mode confort sur le chauffage
//  http://IP/eco:      Active le mode Eco sur le chauffage
//  http://IP/hors-gel: Active le mode Hors gel sur le chauffage   
//  http://IP/arret:    Desactive le chauffage
//  http://IP/APMode:   Active le mode AP pour la config
//  http://IP/reboot:   Reboot le module
//  http://IP/update:   Update the module with OTA ( see http://esp8266.github.io/Arduino/versions/2.1.0-rc2/doc/ota_updates/ota_updates.html)

// Works with DS18B20 temp sensors or DHT11/DHT22 temp/humidity sensor



// EEPROM config flag, increment this each time EEPROM need to be rewrited
#define ID_PARAM_PROFIL_VERSION 2015




//Switch between sensors type, assign this value to TYPE_CAPTEUR define
#define ID_PARAM_CAPTEUR_DS18B20 1 
#define ID_PARAM_CAPTEUR_DHT11 2
#define ID_PARAM_CAPTEUR_DHT22 3


//Import the config files
#include  "RelayPilotConfig.h" //Import config
#include  "RelayPilotConfig_default.h" //Import config



#include <DallasTemperature.h>
#include <OneWire.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266HTTPUpdateServer.h>
#include <EEPROM.h>







//System init
MDNSResponder mdns; 
ESP8266WebServer server(80); //Init the webserver
ADC_MODE(ADC_VCC); //Switch ADC to VCC chip ( for settings info)
DHT dht(SENSOR_PIN_DHT, DHT22,15);
ESP8266HTTPUpdateServer httpUpdater(false); //Init the OTA updater, accessible with the /update URL
OneWire oneWire(SENSOR_PIN_DS18B20);
DallasTemperature DS18B20(&oneWire);






String DebugInfo="";


int compteurBoucle = 0;
int RemonterInfoCapteurTousLesXSecondes=DELAY_BETWEEN_SENSORS_SEND_TO_DOMOTICZ;



String modelePage= "<html><head><title>%TITRE%</title>  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\" rel=\"stylesheet\"/><script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js\"></script><link href=\"https://bootswatch.com/darkly/bootstrap.min.css\" rel=\"stylesheet\"></head><body><nav class='navbar navbar-inverse'><div class='container-fluid'><div class='navbar-header'><a href='/' class='navbar-brand'>RELAYPILOT V0.2</a></div></div></nav><div class='row'><div class='col-lg-4'><img src='http://www.realogi.fr/img/RelayPilotLogo.png' alt='NODEMCU RELAYPILOT V0.2 BY DUCSEB'></div><div class='col-lg-6'>%CONTENU%</div></div></body></html>";
String modeChauffage = "Confort";
String formParametre = String("<form action='/settingsSet' method=\"post\" class='form-horizontal'><fieldset><legend>Paramêtres</legend><div class='form-group'><label class='col-lg-2 control-label' for='parametre'>Valeur à modifier:</label> <div class='col-lg-10'><select class='form-control' type='select' name='parametre' id='parametre'>Valeur à modifier:<option value='ssid'>SSIDWIFI</option><option value='passwordssid'>Password wifi</option><option value='ipDevice'>Adresse IP</option><option value='ipGateway'>IP Passerelle</option><option value='NetMask'>Masque reseau</option><option value='DNS'>Serveur DNS</option><option value='DomoticzDeviceIDTemp'>Domoticz Device ID Temperature</option><option value='DomoticzDeviceIDHum'>Domoticz Device ID Humidite</option>")
                       +String("<option value='NomDevice'>Nom du l'appareil</option><option value='HostDomoticz'>Adresse du serveur domoticz</option><option value='PortDomoticz'>Port serveur Domoticz</option><option value='DHCP'>DHCP (0/1)</option></select></div></div><div class='form-group'><label class='col-lg-2 control-label' for='valeur'>Valeur:</label><div class='col-lg-10'><input type='text' name='valeur' value='' class='form-control'/></div></div><div class='form-group'> <div class='col-lg-10 col-lg-offset-2'><input type='submit' value='Valider' class='btn btn-success btn-lg btn-block'/></div></div></fieldset></form>");








//Base struct for saving the parameters to EEPROM
struct ConfigurationDuModule {
  int Init;
  char nomDuDevice[255];
  IPAddress adresseIP;
  IPAddress adresseIPGateway;
  IPAddress NetMask;
  IPAddress DNS1;
  bool dhcp;
  char SSIDWifi[255];
  char PasswordWifi[255];
  int Capteur;
  bool SendHumidite;
  int DomoticzDeviceIDTemp;  
  int DomoticzDeviceIDHum;  
  char hostDomoticz[255];
  int hostDomoticzPort;
};

//Default configuration, editable with web interface
ConfigurationDuModule ConfigModuleDefaut // Defauklt config
{
    ID_PARAM_PROFIL_VERSION, // Parameter InitValue (change each time you modify the content)
    MODULENAME, //Name of the device
    IPMODULE, // Client IP Adress for the ESP8266
    IPGATEWAY, // Gateway IP 
    MASK, // Netmask
    DNSSERVER, // DNS server
    DHCPMODE, //Mode DHCP
    SSIDWIFI, // SSID Wifi
    WIFIPASSWORD, // Wifi password
    TYPE_CAPTEUR, //1=DS18B20 2=DHT11 3=DHT22
    SEND_HUMIDITY_DHT22, // Envoi de l'humidite à domoticz    
    IDCAPTEURDOMOTICZTEMP, // Domoticz ID  Temperature
    IDCAPTEURDOMOTICZHUM, // Domoticz ID  Humidite
    HOSTDOMOTICZ,
    HOSTDOMOTICZPORT //Domoticz Port
};


ConfigurationDuModule laConfigDuModule; //Config in use





// Acces point config
bool modePointAcces=false;
const char* ssidAPMode = laConfigDuModule.nomDuDevice;
const char* passwordAPMode =laConfigDuModule.nomDuDevice; // AP Mode wifi password






void setup() {  


  Serial.begin(115200); 
  
 
  delay(1000);
  Serial.println("Lecture de la configuration");

  //Lecture de la config depuis l'EEPROM
  if(EEPROMReadConfig()==false)
  {
    Serial.println("Chargement de la config par defaut");
    laConfigDuModule=ConfigModuleDefaut;
    Serial.println("Sauvegarde de la config par defaut");
    EEPROMSaveConfig();
    delay(2000);
    Serial.println("Reboot dans 2 sec");
    RebootModule();
  }
 
 
  
  
  
  //Connection au wifi
  Serial.print("Connection au wifi ");
  Serial.println("SSID:"+String(laConfigDuModule.SSIDWifi));   
  Serial.println("Mode DHCP"+String(laConfigDuModule.dhcp));  

  if(laConfigDuModule.dhcp==false) WiFi.config(laConfigDuModule.adresseIP,laConfigDuModule.adresseIPGateway,laConfigDuModule.NetMask,laConfigDuModule.DNS1);
  else WiFi.begin(laConfigDuModule.SSIDWifi,laConfigDuModule.PasswordWifi);
  int compteurTentativeConnection=0;
  Serial.println("Connection au Wifi");
  while (WiFi.status() != WL_CONNECTED && compteurTentativeConnection<20) {
    delay(500);
    Serial.print(".");
    compteurTentativeConnection++;
  } 

  

  if(compteurTentativeConnection>18)
  {
    Serial.println("Problème de connection au Wifi client, activation du mode WIFI AP");
    InitModeAccesPoint();  
    modePointAcces=true;  
  }
  else{
     Serial.println("WiFi Client connecté");  
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  
    
    //Chargement du MDNS responder
    if (mdns.begin(laConfigDuModule.nomDuDevice, WiFi.localIP())) {
      Serial.println("MDNS responder started");
       mdns.addService("http", "tcp", 80);
      }
  }
  
  
  httpUpdater.setup(&server);
 

  
  //Definition des commandes HTTP
  server.on("/", PageAcceuil);
  server.on("/temp", PageTemp);  
  server.on("/tempsend", PageTempAvecEnvoiInfo);
  server.on("/hum", PageHum);  
  server.on("/humsend", PageHumAvecEnvoiInfo);
  server.on("/confort", PassageDuChauffageEnModeConfort);
  server.on("/eco", PassageDuChauffageEnModeECO);
  server.on("/hors-gel", PassageDuChauffageEnModeHorsGel);
  server.on("/arret", PassageDuChauffageEnModeArret);
  server.on("/APMode", InitModeAccesPoint);   
  server.on("/settings", InitFormSettings);
  server.on("/settingsSet", ChangeParamValue);
  server.on("/reboot", RebootPage);
  
  
  /*server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });*/
  //Definition du 404
  server.onNotFound(handleNotFound);
  //Demarage du serveur
  server.begin();  
  Serial.println("HTTP server started");


  Serial.println("Initialisation des relays");
  pinMode(RELAY1_PIN,OUTPUT);
  pinMode(RELAY2_PIN,OUTPUT);

  //Passage en mode arret
  digitalWrite(RELAY1_PIN,LOW);
  digitalWrite(RELAY2_PIN,HIGH);

 Serial.println("Initialisation des relays2");
  //Detection du capteur
  if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DS18B20) // Capteur DS18B20
  {
    Serial.println("Capteur: DS18B20");
   
    OneWire oneWire(SENSOR_PIN_DS18B20);
    DallasTemperature DS18B20(&oneWire);
    
  }
  else if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT11) //Capteur DHT11
  {
   
    Serial.println("Capteur: DHT11");
    DHT dht(SENSOR_PIN_DHT, DHT11,15);
  }
  else if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT22)
  {
   
    Serial.println("Capteur: DHT22");
    DHT dht(SENSOR_PIN_DHT, DHT22,15);
  }
  
  //Attach le bouton USER au mode config AP
  pinMode(PIN_MODE_CONFIG,INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_MODE_CONFIG), ActiverModeCOnfig, CHANGE);
  
}



void loop() 
{

  if(modePointAcces==false)
  {
    if((compteurBoucle/2)>RemonterInfoCapteurTousLesXSecondes)
    {   
      if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DS18B20)
        GetTemperateureDS18B20(true);
      else if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT11 || laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT22)
      {
        if(laConfigDuModule.SendHumidite==false)
        {
           GetTemperateureDHT(true);
        }
        else
        {
          GetTemperateureHumDHT(true);
        }
       
        //GetHumiditeDHT(true);
      }
      
        
     
      compteurBoucle=0;
    }
    else
    {
         compteurBoucle=compteurBoucle+1;  
         
    }
    
  }

  
  
  server.handleClient();
  if(modePointAcces)
  {
    delay(50);  
  }
  else{
    delay(1000);
  }
  
 
}






//------------ FONCTION DE RECUPERATION DE LA TEMPERATURE---------------------------------------------------------------


float GetTemperateureDS18B20(bool sendInfo)
{
   float t;

   //Recup temperature
   DS18B20.requestTemperatures(); 
   t = DS18B20.getTempCByIndex(0); 
   AjouteInfoDebug("Temperature: "+String(t)+" *C ");     

    //8¨°C is the default temperature of the DS18B22 at boot
   if(t<85){
    if(sendInfo)SendInfoToDomoticz(t,0,false,laConfigDuModule.DomoticzDeviceIDTemp); 
   }
    
    

     return t;
}

float GetTemperateureDHT(bool sendInfo)
{
   float t = dht.readTemperature();
   AjouteInfoDebug("Temperature: "+String(t)+" *C ");     
    
    
    if(sendInfo)SendInfoToDomoticz(t,0,false,laConfigDuModule.DomoticzDeviceIDTemp);

     return t;
}
float GetTemperateureHumDHT(bool sendInfo)
{
   float t = dht.readTemperature();
   AjouteInfoDebug("Temperature: "+String(t)+" *C ");     
   float h = dht.readHumidity();
   AjouteInfoDebug("Humidity: "+String(h)+" purcent ");      
    
    if(sendInfo)SendInfoToDomoticz(t,h,true,laConfigDuModule.DomoticzDeviceIDTemp);

     return t;
}
float GetHumiditeDHT(bool sendInfo)
{
    float h = dht.readHumidity();
   AjouteInfoDebug("Humidity: "+String(h)+" purcent ");      
    
    
    if(sendInfo)SendInfoToDomoticz(h,0,false,laConfigDuModule.DomoticzDeviceIDHum);

     return h;
}








//----------------ENVOI DES INFOS AUX SERVEURS WEB ----------------------------------------------


void SendInfoToDomoticz(float t,float h,bool modeHumidite,int deviceID)
{
  AjouteInfoDebug("Envoi info sur Domoticz......................");
  WiFiClient client;
  
  AjouteInfoDebug("Hote : "+String(laConfigDuModule.hostDomoticz));
  AjouteInfoDebug("Port : "+String(laConfigDuModule.hostDomoticzPort));
  

  
  
  if (!client.connect(laConfigDuModule.hostDomoticz, laConfigDuModule.hostDomoticzPort)) {
    AjouteInfoDebug("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/json.htm?type=command&param=udevice&idx=";
  url+=deviceID;
  url+="&nvalue=0&svalue=";
  url += t;
  if(modeHumidite)
  {
    url +=";"+String(h)+";0";
  }
  
  
   //AjouteInfoDebug("Requesting URL: ");
   
  //AjouteInfoDebug(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + laConfigDuModule.hostDomoticz + "\r\n" + 
               "Connection: close\r\n\r\n");
   delay(10);
   
    while(client.available()){
    String line = client.readStringUntil('\r');
    //AjouteInfoDebug(line);    
  }
  
  
 AjouteInfoDebug("closing connection");
 client.stop();
}




//------------------------- FONCTION QUI PERMET DE SORTIR LES INFOS DES DEBUGS---------------------------------

void AjouteInfoDebug(char* info)
{
  DebugInfo+=info;
  Serial.println(info);
  //infoDebugWeb+=String(info)+"<br/>";
}
void AjouteInfoDebug(String info)
{
  DebugInfo+=info;
  Serial.println(info);
  //infoDebugWeb+=info+"<br/>";
}





//------------------------------- FONCTION DE MISE EN FORME HTTP ----------------------------------------------

String MettreEnFormeTexte(String texte,String titrePage)
{

  String rendu = modelePage;
  rendu.replace("%TITRE%",titrePage);
  rendu.replace("%CONTENU%",texte); 
  return rendu;
}

String ConstuireLigneAccueil(String nomParam,String value)
{
  String leRendu="<li class='list-group-item'>"+nomParam+"<span class='badge '>"+value+"</span></li>";
  return leRendu;
}
void PageAcceuil() 
{  
  String InfoServeur = "<div class='panel panel-success'><div class='panel-heading'><h3 class='panel-title'>INFORMATIONS</h3></div>";
  InfoServeur+="<div class='panel-body'><ul class='list-group'>";
  InfoServeur+=ConstuireLigneAccueil("Nom du device",String(laConfigDuModule.nomDuDevice));
  InfoServeur+=ConstuireLigneAccueil("SSID wifi",String(laConfigDuModule.SSIDWifi));
  InfoServeur+=ConstuireLigneAccueil("Password wifi",String(laConfigDuModule.PasswordWifi));
  InfoServeur+=ConstuireLigneAccueil("Force du wifi",String(WiFi.RSSI())+"dbm");
  InfoServeur+=ConstuireLigneAccueil("IP du module (STA)",String(WiFi.localIP()));
  InfoServeur+=ConstuireLigneAccueil("IP du module (AP)",String(WiFi.softAPIP()));
  InfoServeur+=ConstuireLigneAccueil("MAC address du module (STA)",String(WiFi.macAddress()));
  InfoServeur+=ConstuireLigneAccueil("MAC address du module (AP)",String(WiFi.softAPmacAddress()));
  
  InfoServeur+="</ul><ul class='list-group'>";
InfoServeur+=ConstuireLigneAccueil("ID du capteur Domoticz Temperature",String(laConfigDuModule.DomoticzDeviceIDTemp));
InfoServeur+=ConstuireLigneAccueil("ID du capteur Domoticz Humidite (seul)",String(laConfigDuModule.DomoticzDeviceIDHum));
InfoServeur+=ConstuireLigneAccueil("Serveur Domoticz",String(laConfigDuModule.hostDomoticz));
InfoServeur+=ConstuireLigneAccueil("Port HTTP du serveur Domoticz",String(laConfigDuModule.hostDomoticzPort));
    InfoServeur+="</ul><ul class='list-group'>";
    InfoServeur+=ConstuireLigneAccueil("Port capteur DS18B20","GPIO"+String(SENSOR_PIN_DS18B20));
        InfoServeur+=ConstuireLigneAccueil("Port capteur DHT","GPIO"+String(SENSOR_PIN_DHT));
    InfoServeur+=ConstuireLigneAccueil("Port relay 1","GPIO"+String(RELAY1_PIN));
    InfoServeur+=ConstuireLigneAccueil("Port relay 2","GPIO"+String(RELAY2_PIN));
    InfoServeur+=ConstuireLigneAccueil("Interval d'envoi des infos capteurs",String(RemonterInfoCapteurTousLesXSecondes)+"s");
    InfoServeur+=ConstuireLigneAccueil("Etat du chauffage",modeChauffage);
    InfoServeur+="</ul><ul class='list-group'>";
    InfoServeur+=ConstuireLigneAccueil("Tension d'alimentation",String(ESP.getVcc())+"mV");
    InfoServeur+=ConstuireLigneAccueil("Chip ID",String(ESP.getChipId()));
    InfoServeur+=ConstuireLigneAccueil("Frequence du microcontrolleur",String(ESP.getFlashChipSpeed())+"Hz");
    InfoServeur+=ConstuireLigneAccueil("FreeHeap (mémoire)",String(String(ESP.getFreeHeap())));

    InfoServeur+="</ul>";
  

  
  InfoServeur+="<br/><br/>";
   InfoServeur+="<a href=\"/settings\" class='btn btn-success btn-lg btn-block'>Paramètres</a><br/>";
   InfoServeur+="</div></div>";
  char infoServeurChar[InfoServeur.length()+1];
  InfoServeur.toCharArray(infoServeurChar,(InfoServeur.length()+1));
  InfoServeur="";
  
  server.send(200, "text/html", MettreEnFormeTexte(infoServeurChar,"NODEMCU RELAYPILOT V0.2"));  
}



void handleNotFound(){    
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  } 
  server.send(404, "text/html", MettreEnFormeTexte(message,"Page invalide"));  
 
}


void PageTemp()
{
  float temperature=0.0;
    if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DS18B20)
        temperature=GetTemperateureDS18B20(false);
      else if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT11 || laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT22)
      {
        temperature=GetTemperateureDHT(false);        
      }
   String texte= "<b>Temperature</b>:"+String(temperature);   
   EnvoyerInfoClientHTTPAvecCadre(texte,"Temperature");
   
   
}
void PageHum()
{
    float humidite=0.0;
    
        
   if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT11 || laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT22)
      humidite= GetHumiditeDHT(false);
   
   
   String texte= "<b>Humidite</b>:"+String(humidite);   
   EnvoyerInfoClientHTTPAvecCadre(texte,"Humidité");
   
   
}
void PageTempAvecEnvoiInfo()
{ 
   float temperature=0.0;
    if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DS18B20)
        temperature=GetTemperateureDS18B20(true);
      else if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT11 || laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT22)
      {
        temperature=GetTemperateureDHT(true);        
      }
     String texte= "<b>Temperature</b>:"+String(temperature)+"<br/><br/><B>Info de debug</b>:<br/>"+DebugInfo; 
     EnvoyerInfoClientHTTPAvecCadre(texte,"Température avec envoi info sur serveur");   
     
}

void PageHumAvecEnvoiInfo()
{ 
   float humidite=0.0;
   if(laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT11 || laConfigDuModule.Capteur==ID_PARAM_CAPTEUR_DHT22)
   {
      humidite= GetHumiditeDHT(true);
   }
    String texte= "<b>Humidite</b>:"+String(humidite)+"<br/><br/><B>Info de debug</b>:<br/>"+DebugInfo;    
      EnvoyerInfoClientHTTPAvecCadre(texte,"Humidité avec envoi info");   
   
}

void NonDisponible() {  
     EnvoyerInfoClientHTTPAvecCadre("Non disponible sur la BETA! SORRY :)","Pas dispo");     
}

void EnvoyerInfoClientHTTP(String leTexte,String LeTitre)
{
   server.send(200, "text/html", MettreEnFormeTexte(leTexte,LeTitre));  
   DebugInfo="";
}
void EnvoyerInfoClientHTTPAvecCadre(String leTexte,String LeTitre)
{

  String contentFinal="<div class='panel panel-default'><div class='panel-heading'>"+LeTitre+"</div><div class='panel-body'>"+leTexte+"</div></div>";                
                  

              
   server.send(200, "text/html", MettreEnFormeTexte(contentFinal,LeTitre));  
   DebugInfo="";
}







//---------------- FONCTION POUR LA GESTION DU CHAUFFAGE -------------------------------
// PAS DE SIGNAL
void PassageDuChauffageEnModeConfort()
{
    digitalWrite(RELAY1_PIN,LOW);
    digitalWrite(RELAY2_PIN,LOW);
    modeChauffage="Confort";
    String texte= "Activation du mode "+modeChauffage+" <b>OK</b>";    
    texte+="<br/><br/><a href='/' class='btn btn-primary'>Retour accueil</a>";
    EnvoyerInfoClientHTTPAvecCadre(texte,"Changement Etat chauffage");
}
// COMPLET
void PassageDuChauffageEnModeECO()
{
    digitalWrite(RELAY1_PIN,HIGH);
    digitalWrite(RELAY2_PIN,HIGH);
    modeChauffage="Eco";
    String texte= "Activation du mode "+modeChauffage+" <b>OK</b>";    
    texte+="<br/><br/><a href='/' class='btn btn-primary'>Retour accueil</a>";
    EnvoyerInfoClientHTTPAvecCadre(texte,"Changement Etat chauffage");
}
// ALTERNANCE POSITIVE
void PassageDuChauffageEnModeArret()
{
    digitalWrite(RELAY1_PIN,LOW);
    digitalWrite(RELAY2_PIN,HIGH);
    modeChauffage="Arret";
    String texte= "Activation du mode "+modeChauffage+" <b>OK</b>";    
    texte+="<br/><br/><a href='/' class='btn btn-primary'>Retour accueil</a>";
    EnvoyerInfoClientHTTPAvecCadre(texte,"Changement Etat chauffage");
}
// ALTERNANCE NEGATIVE
void PassageDuChauffageEnModeHorsGel()
{
    digitalWrite(RELAY1_PIN,HIGH);
    digitalWrite(RELAY2_PIN,LOW);
    modeChauffage="HorsGel";
     String texte= "Activation du mode "+modeChauffage+" <b>OK</b>";    
    texte+="<br/><br/><a href='/' class='btn btn-primary'>Retour accueil</a>";
    EnvoyerInfoClientHTTPAvecCadre(texte,"Changement Etat chauffage");
}

void InitModeAccesPoint()
{ Serial.println("");
 Serial.println("----INIT DU MODE ACCESS POINT----");
   WiFi.mode(WIFI_AP);
  WiFi.softAP(ssidAPMode,passwordAPMode);
  //WiFi.softAPConfig(ipNodeMcuAccessPoint,ipGatewayAP,ipSubnetAP);
   Serial.println("");
  Serial.println("WiFi Serveur connecté");  
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());  

  WiFi.printDiag(Serial);
}






/* ---------------- ECRITURE ET LECTURE DE LA CONFIG DEPUIS l'EEPROM ---------------------------------------*/

void EEPROMSaveConfig()
{
  Serial.println("Sauvegarde la configuration du module dans l'EEPROM");

  EEPROM.begin(4096);



    
  EEPROM.begin(sizeof(ConfigModuleDefaut));
  

  
   EEPROM.put(0,laConfigDuModule);
   EEPROM.end();
}

bool EEPROMReadConfig(){
  Serial.println("Lecture la configuration du module dans l'EEPROM");
  ConfigurationDuModule laConfigLue;
  delay(2000);
   EEPROM.begin(sizeof(ConfigModuleDefaut));
   EEPROM.get(0,laConfigLue);
    
   if(laConfigLue.Init>=ID_PARAM_PROFIL_VERSION)
   {
    Serial.println("Lecture OK, init OK");
    laConfigDuModule=laConfigLue;
    Serial.println("Init:"+String(laConfigLue.Init));
    Serial.println("nomDuDevice:"+String(laConfigLue.nomDuDevice));
    Serial.println("adresseIP:"+String(laConfigLue.adresseIP));
    Serial.println("adresseIPGateway:"+String(laConfigLue.adresseIPGateway));
    Serial.println("NetMask:"+String(laConfigLue.NetMask));
    Serial.println("DNS1:"+String(laConfigLue.DNS1));
    Serial.println("SSIDWifi:"+String(laConfigLue.SSIDWifi));
    Serial.println("DCHP:"+String(laConfigLue.dhcp));
    Serial.println("PasswordWifi:"+String(laConfigLue.PasswordWifi));
    Serial.println("DomoticzDeviceID Temperature:"+String(laConfigLue.DomoticzDeviceIDTemp));    
    Serial.println("DomoticzDeviceID Humidite:"+String(laConfigLue.DomoticzDeviceIDHum));    
    Serial.println("hostDomoticz:"+String(laConfigLue.hostDomoticz));
    Serial.println("hostDomoticzPort:"+String(laConfigLue.hostDomoticzPort));
    
    return true;
   }
   else
   {
    Serial.println("Lecture pas OK, init avec les infos de base");
    return false;
   }
   
   EEPROM.commit();
   
}


void InitFormSettings(){
  EnvoyerInfoClientHTTP(formParametre,"Param");
}

void ChangeParamValue(){
  String parametre = server.arg("parametre");
  String valeur = server.arg("valeur");
  String erreur="";

  bool ConfigModifier=false;

  if(valeur=="")erreur="La valeur ne peux pas être vide";
  else{
  if(parametre=="ssid")
    {
      valeur.toCharArray(laConfigDuModule.SSIDWifi,255);      
      ConfigModifier=true;
    }
    if(parametre=="passwordssid")
    {
      valeur.toCharArray(laConfigDuModule.PasswordWifi,255);
      ConfigModifier=true;
    }
    if(parametre=="ipDevice")
    {
     IPAddress adresseIP = GetIpAdressFromString(valeur);
     if(TestIpAddressOK(adresseIP))
     {
       laConfigDuModule.adresseIP=adresseIP;
         ConfigModifier=true;
     }
     else{
      erreur+="L'adresse IP est invalide";
     }     
      
      
    }
     if(parametre=="ipGateway")
    {
       IPAddress adresseIP = GetIpAdressFromString(valeur);
     if(TestIpAddressOK(adresseIP))
     {
       laConfigDuModule.adresseIPGateway=adresseIP;
         ConfigModifier=true;
     }
     else{
      erreur+="L'adresse IP est invalide";
     }     
    }
     if(parametre=="NetMask")
    {
      IPAddress adresseIP = GetIpAdressFromString(valeur);
     if(TestIpAddressOK(adresseIP))
     {
       laConfigDuModule.NetMask=adresseIP;
         ConfigModifier=true;
     }
     else{
      erreur+="Le mask est invalide";
     }     
    }
     if(parametre=="DNS")
    {
       IPAddress adresseIP = GetIpAdressFromString(valeur);
     if(TestIpAddressOK(adresseIP))
     {
       laConfigDuModule.DNS1=adresseIP;
         ConfigModifier=true;
     }
     else{
      erreur+="L'adresse IP est invalide";
     }     
    }
    if(parametre=="DomoticzDeviceIDTemp")
    {
      int DeviceID = valeur.toInt();
       if(DeviceID==0)DeviceID=1;            
       laConfigDuModule.DomoticzDeviceIDTemp=DeviceID;
       ConfigModifier=true;
    }
    if(parametre=="DomoticzDeviceIDHum")
    {
      int DeviceID = valeur.toInt();
       if(DeviceID==0)DeviceID=2;            
       laConfigDuModule.DomoticzDeviceIDHum=DeviceID;
       ConfigModifier=true;
    }
    if(parametre=="NomDevice")
    {
      valeur.toCharArray(laConfigDuModule.nomDuDevice,11);
      ConfigModifier=true;
    }
     if(parametre=="HostDomoticz")
    {
      valeur.toCharArray(laConfigDuModule.hostDomoticz,255);
      Serial.println("Modification du host:"+String(laConfigDuModule.hostDomoticz));
      ConfigModifier=true;
    }
     if(parametre=="PortDomoticz")
    {
       int portDomoticz = valeur.toInt();
       if(portDomoticz==0)portDomoticz=80;            
       Serial.println("Modification du port domoticz:"+String(laConfigDuModule.hostDomoticzPort));
       laConfigDuModule.hostDomoticzPort=portDomoticz;
       ConfigModifier=true;
    }
    if(parametre=="DHCP")
    {
       int valDCHCP= valeur.toInt();
       if(valDCHCP==0)laConfigDuModule.dhcp=false;
       else laConfigDuModule.dhcp=true;
       ConfigModifier=true;
    }
    
  }

  if(ConfigModifier)
  {
     EEPROMSaveConfig();    
  }
  
  if(erreur!="")
  {
   EnvoyerInfoClientHTTPAvecCadre("<div class='alert alert-dismissible alert-danger'>Erreur dans la modification du paramêtre</div>"+erreur,"Modification de paramètres"); 
  }
  else
  {
    EnvoyerInfoClientHTTPAvecCadre( String("<div class='alert alert-dismissible alert-success'>Vous avez modifie le paramètre '")+parametre+"' avec la valeur <b>'"+valeur+"'</b></div> <a href='/reboot' class='btn btn-success btn-lg btn-block'>Rebooter le module pour prendre en compte les modifs</a><br/><br/><a href='/settings' class='btn btn-info btn-lg btn-block'>Faire un autre réglage</a>","Modification de parametres");
  }
   
  
  
}


IPAddress GetIpAdressFromString(String valeurEntree)
{
      String blocIP1=getStringPartByNr(valeurEntree,'.',0);
      String blocIP2=getStringPartByNr(valeurEntree,'.',1);
      String blocIP3=getStringPartByNr(valeurEntree,'.',2);
      String blocIP4=getStringPartByNr(valeurEntree,'.',3);

       int ipDigit1=blocIP1.toInt();     
         int ipDigit2=blocIP2.toInt();     
         int ipDigit3=blocIP3.toInt();     
         int ipDigit4=blocIP4.toInt();
    IPAddress adresseIP{ipDigit1,ipDigit2,ipDigit3,ipDigit4};

    return adresseIP;
    
      
  
}

bool TestIpAddressOK(IPAddress addressATester)
{
    if(addressATester[0]==0 && addressATester[1]==0 && addressATester[2]==0 && addressATester[3]==0)
    {
      return false;
    }
    else{
      return true;
    }
}





String getStringPartByNr(String data, char separator, int index) {
    int stringData = 0;        //variable to count data part nr 
    String dataPart = "";      //variable to hole the return text

    for(int i = 0; i<data.length()-1; i++) {    //Walk through the text one letter at a time

        if(data[i]==separator) {
            //Count the number of times separator character appears in the text
            stringData++;

        }else if(stringData==index) {
            //get the text when separator is the rignt one
            dataPart.concat(data[i]);

        }else if(stringData>index) {
            //return text and stop if the next separator appears - to save CPU-time
            return dataPart;
            break;

        }

    }
    //return text if this is the last part
    return dataPart;
}

void RebootPage(){
      String JSAutoLink=String("<script type=\"text/javascript\">")+
                   String("function openUrl(){")+
                    String("window.open('/');")+
                    String("}")+
                    String("setTimeout(openUrl, 3000); // Wait 3 seconds")+
                  
                    String("</script>");
                    
       EnvoyerInfoClientHTTP("<div class='bs-component'>Reboot du module en cours ....</div><br/><br/><a href=\"/\" class='btn btn-success'>Retour à la home (patienter 10 sec)</a>"+JSAutoLink,"Modification de parametres"); 
       ESP.restart(); 
}
void RebootModule()
{
  ESP.restart();  
}


void ActiverModeCOnfig()
{
  Serial.println("Activation du mode config");
  modePointAcces=true;
   InitModeAccesPoint(); 
}









