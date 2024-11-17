#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "building.h"


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


void *ventilation(Building *building){
    
    while (1){
        // Lire la mesure des capteurs necessaire
        // Controler le systeme (chauffage, lumière, ventilation ou source d'energie)
    }
    pthread_exit(NULL);
}



int main(int argc, char **argv){
    pthread_t ventilation_thread;
    Building building;

    init_building(&building, SOLAR);
    // Add rooms

    if(pthread_create(&ventilation_thread, NULL, ventilation, &building) != 0){
        perror("Failed to create system thread");
        exit(EXIT_FAILURE);
    }

    sleep(30);

    pthread_join(ventilation_thread, NULL);

    printf("Complete\n");

    return 0;
}

