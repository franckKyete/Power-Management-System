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


void *eclairage(void *_building) {
    Building *building = (Building*)_building;

    bool light;

    while (1) {
        
        for (int i = 0; i < building->size; i++) {

            // Vérifier l'état du capteur de présence
            if (building->rooms[i].sensors[PRESENCE_SENSOR].value == 1.0) {
                // Quelqu'un est présent dans la pièce
                light = true; // Allumer la lumière

            } else {
                // Personne n'est présent dans la pièce
                light = false; // Éteindre la lumière
            }
            

            if(light != building->rooms[i].light){
                pthread_mutex_lock(&building->building_lock);
                building->rooms[i].light = light;
                if(light){
                    building->rooms[i].devices[LIGHTING_DEVICE].power = 130.0;
                }else{
                    building->rooms[i].devices[LIGHTING_DEVICE].power = 0;
                }
                pthread_mutex_unlock(&building->building_lock);
            }
        }
    }

    pthread_exit(NULL);
}