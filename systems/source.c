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


void *source(Building *building) {
    while (1) {
        float puissance_totale = 0.0; // Initialisation de la puissance totale
        float puissance_batterie = 1000.0; // Exemple de puissance maximale de la batterie (à ajuster selon votre contexte)

        // Boucle qui parcourt toutes les pièces du bâtiment
        for (int i = 0; i < building->size; i++) { 
        // Boucle qui parcourt tous les appareils dans la pièce
            for (int j = 0; j < MAX_DEVICES; j++) {
                puissance_totale += building->rooms[i].devices[j].power; // Ajoute la puissance de chaque appareil
            }
        }
        
        // Vérifier si la puissance totale dépasse la puissance de la batterie
        if (puissance_totale > puissance_batterie) {
            // Basculer vers une autre source d'énergie (hydroélectrique)
            building->prefered_power_source = GRID; // Supposons que HYDRO est une constante définissant la source d'énergie hydroélectrique
            printf("La puissance totale (%f) dépasse la capacité de la batterie (%f). Passage à la source hydroélectrique.\n", puissance_totale, puissance_batterie);
        } else {
            // Si la puissance totale ne dépasse pas, restez sur la source d'énergie solaire
            building->prefered_power_source = SOLAR; // Supposons que SOLAR est une constante définissant la source d'énergie solaire
            printf("La puissance totale (%f) est inférieure à la capacité de la batterie (%f). Utilisation de l'énergie solaire.\n", puissance_totale, puissance_batterie);
        }

        // Vérifier si le réseau (GRID) est disponible
        if (!grid_available) {
            // Si le réseau est coupé, passer au générateur
            building->prefered_power_source = GENERATOR;
            printf("Le réseau est coupé. Passage à la source d'énergie du générateur.\n");
        } else {
            // Si le réseau est disponible à nouveau, passer au GRID
            building->prefered_power_source = GRID;
            printf("Le réseau est rétabli. Retour à la source d'énergie du réseau.\n");
        }

        // Délai pour éviter une boucle trop rapide (ajustez la valeur selon les besoins)
        sleep(1);
    }

    pthread_exit(NULL);
}



int main(int argc, char **argv){
    pthread_t source_thread;
    Building building;

    init_building(&building, SOLAR);
    // Add rooms

    if(pthread_create(&source_thread, NULL, source, &building) != 0){
        perror("Failed to create system thread");
        exit(EXIT_FAILURE);
    }

    sleep(30);

    pthread_join(source_thread, NULL);

    printf("Complete\n");

    return 0;
}

