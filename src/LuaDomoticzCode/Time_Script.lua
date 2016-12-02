commandArray = {}
main_item =' - Chauffage - Main'
etat_item =' - Chauffage - Etat'
ThermostatActifInactif = 'Chauffage actif' -- Nom du device pour la consigne de temperature
LesChauffages={'Bureau','Salon','Chambre'}


for i,nomItem in ipairs(LesChauffages) do      
     print('Test item'..nomItem)
     if (otherdevices[ThermostatActifInactif]=='Off') then  --On n'agit que si le "Thermostat" est actif
       if otherdevices[nomItem..etat_item]=='On' then
    	    print('-- Chauffage allumé hors période de chauffage, extinction pour élément:  '..nomItem..main_item)
    	     commandArray[nomItem..main_item]='Off'
        end
     end
     if (otherdevices[ThermostatActifInactif]=='On') then  --On n'agit que si le "Thermostat" est actif
       if otherdevices[nomItem..etat_item]=='Off' then
    	    print('-- Chauffage non allumé hors période de chauffage, extinction pour élément:  '..nomItem..main_item)
    	     commandArray[nomItem..main_item]='On'
        end
     end
        
end
    
     
      
      




return commandArray
