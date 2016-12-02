
commandArray = {}
hysteresis = uservariables['HYSTERESIS_CONSIGNE']

main_item =' - Chauffage - Main'
etat_item =' - Chauffage - Etat'
ThermostatActifInactif = 'Chauffage actif' -- Nom du device pour la consigne de temperature




-- loop through all the devices
for deviceName,deviceValue in pairs(otherdevices) do
    
    nomItem= ''
    temperature =''
    tempthermo='Thermostat'
    continueTraitement=false
    
    if (deviceName=='Bureau Temp') then
      
        nomItem= 'Bureau'
        temperature =tonumber(string.sub(deviceValue,0,5)) --Temperature relevée dans le salon*
        continueTraitement=true
    end
    
    if (deviceName=='Chambre Temp') then
        nomItem= 'Chambre'
        temperature =tonumber(deviceValue) --Temperature relevée dans le salon
        tempthermo = 'Thermostat chambre' -- Nom du device pour la consigne de temperature
        continueTraitement=true
    end
    
    
    
    if (deviceName=='Salon Temp') then
        nomItem= 'Salon'
        temperature =tonumber(deviceValue) --Temperature relevée dans le salon
        continueTraitement=true
    end
    
   
    
    if(continueTraitement == true) then 
    
     --print('-- --------------------------------------'..nomItem..' -------------------------------')
     commande = nomItem..main_item
     etat = nomItem..etat_item
     consigne = tonumber(otherdevices_svalues[tempthermo]) --Température de consigne
     --print('Thermostat:'..otherdevices[ThermostatActifInactif])
     if (otherdevices[ThermostatActifInactif]=='On') then  --On n'agit que si le "Thermostat" est actif
         
         
           
          --print('Temperature:'..temperature)
          --print('Consigne :'..consigne )
          --print('Etat du radiateur :'..otherdevices[etat] )
          --print('Device etat:'..etat..'(etat:'..otherdevices[etat]..')' )
          --print('Device commande:'..commande..'(etat:'..otherdevices[commande]..')' )
             
             
            if ((temperature < (consigne - hysteresis))) then
                
                      if(otherdevices[etat]=='Off') then 
                        print('Allumage du chauffage dans '..nomItem)
                        commandArray[commande]='On'
                      end
            elseif ((temperature > (consigne + hysteresis)) ) then
                
                 if(otherdevices[etat]=='On') then 
                     print('Extinction du chauffage dans '..nomItem)
                     commandArray[commande]='Off'
                  end
            else
      	        --print('-- Pas d\'action')	 
            end
     end
        
        
        
    end
    
     
      
      
end



return commandArray
