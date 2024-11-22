#include "systems.h"


// building : Represente le batiment
//            Le batiment est composé de different pieces (rooms) 
//            et chaque piece, contient plusieurs capteurs.
//            Pour acceder aux information d'un piece, par example le capteur de presence, 
//            vous pouvez utiliser la syntaxe suivante :
//
//                  building->rooms[2].sensors[CAPTEUR_PRESENCE].value
//
//            Cette inforamtion peut etre ensuite utilisé pour controler un systeme precis 
//            comme la lumière suivant les conditions fixé dans la phase de planification
//            
//                  ex : 
//                      presence = building->rooms[2].sensors[CAPTEUR_PRESENCE].value;
//                      if(presence){
//                          building->rooms[2].light = true; // Modifie la variable light pour allumer la lumiere dans cette piece
//                      }else{
//                          building->rooms[2].light = false;
//                      }

#define MAX_CO2_LEVEL 1000

void *ventilation(void *_building){
    Building *building = (Building*)_building;
    bool ventilation;

    while (1){
        for (size_t room_id = 0; room_id < building->size; room_id++)
        {
            int presence= building->rooms[room_id].sensors[PRESENCE_SENSOR].value;
            float co2_level = building->rooms[room_id].sensors[CO2_SENSOR].value;
            
            if ( co2_level > MAX_CO2_LEVEL && presence == 1){
                ventilation = true;
            } else {            
                ventilation = false;    
            }

            if(ventilation != building->rooms[room_id].ventilation){
                pthread_mutex_lock(&building->building_lock);
                building->rooms[room_id].ventilation  = ventilation;
                pthread_mutex_unlock(&building->building_lock);
            }
        }
    }
    pthread_exit(NULL);
}           


