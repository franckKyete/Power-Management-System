#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include "../building.h"

int getMaxConsomationRoom(float* array, int size) {
    if (size <= 0) {
        fprintf(stderr, "Error: Array size must be greater than 0.\n");
        return -1; // Return a default value or handle the error
    }

    float max = array[0]; // Assume the first element is the maximum
    int id = 0;

    for (int i = 1; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
            id = i;
        }
    }

    return id;
}
int getMinConsomationRoom(float* array, int size) {
    if (size <= 0) {
        fprintf(stderr, "Error: Array size must be greater than 0.\n");
        return -1; // Return a default value or handle the error
    }

    float min = array[0]; // Assume the first element is the minimum
    int id = 0;

    for (int i = 1; i < size; i++) {
        if (array[i] < min) {
            min = array[i];
            id = i;
        }
    }

    return id;
}


// Variable globale pour simuler l'état de la GRID (1 = active, 0 = coupée)
int grid_active = 1;

// Fonction pour simuler une coupure et le retour de la GRID
void *simulate_grid(void *arg) {
    while (1) {
        sleep(10); // Après 10 secondes, couper la GRID
        grid_active = 0;
        //printf("GRID: Coupure détectée.\n");
        
        sleep(15); // Après 15 secondes, rétablir la GRID
        grid_active = 1;
        //printf("GRID: Alimentation rétablie.\n");
    }
    pthread_exit(NULL);
}

// Fonction principale pour gérer les sources d'énergie
void *power_source(Building *building) {

    float rooms_power[MAX_ROOM];
    // int sorted_by_power[MAX_ROOM];

    while (1) {
        float puissance_totale = 0.0; // Initialisation de la puissance totale
        float puissance_batterie = 1000.0; // Exemple de capacité de la batterie

        // Boucle qui parcourt toutes les pièces
        for (int i = 0; i < building->size; i++) {
            float puissance_piece = 0.0;

            // Calcul de la puissance totale consommée par la pièce
            for (int j = 0; j < MAX_DEVICES; j++) {
                puissance_piece += building->rooms[i].devices[j].power;
            }
            rooms_power[i] = puissance_piece;
            puissance_totale += puissance_piece;
        }

        // Si la puissance dépasse la batterie, gestion des pièces les plus consommatrices
        if (puissance_totale > puissance_batterie) {
            
            //printf("La batterie ne peut pas supporter la charge. Réduction progressive...\n");
            for (int i = 0; i < building->size; i++) {
                int highest_room = getMaxConsomationRoom(rooms_power, building->size);
                float puissance_piece = rooms_power[highest_room];
                rooms_power[highest_room] = -rooms_power[highest_room]; // This ensures that the room is no longer the maximum

                if(building->grid_available){
                    building->rooms[highest_room].power_source = GRID;
                }else{
                    building->rooms[highest_room].power_source = GENERATOR;
                }
                
                puissance_totale -= puissance_piece;

                if (puissance_totale <= puissance_batterie) {
                    break; // Arrêter dès que la charge est suffisante
                }
            }
        }else if(puissance_totale < (puissance_batterie - 100)){
            // La batterie peut supporter une charge supplaimentaire. Addition progressive...
            for (int i = 0; i < building->size; i++) {

                int lowest_room;
                float puissance_piece;
                do{
                    lowest_room = getMinConsomationRoom(rooms_power, building->size);
                    puissance_piece = rooms_power[lowest_room];
                    rooms_power[lowest_room] = rooms_power[lowest_room] < 0 ? -rooms_power[lowest_room] : rooms_power[lowest_room];
                }
                while(puissance_piece < 0);
            
                rooms_power[lowest_room] = 100000.0; // This ensures that the room is no longer the minimum

                if(building->rooms[lowest_room].power_source == GRID || building->rooms[lowest_room].power_source == GENERATOR){
                    if( (puissance_totale + puissance_piece) < (puissance_batterie - 100)){
                        building->rooms[lowest_room].power_source = SOLAR;
                        puissance_totale += puissance_piece;
                    }else{
                        break;
                    }
                }
            }
        }
    }
    pthread_exit(NULL);
}

// int main(int argc, char **argv) {
//     pthread_t source_thread, grid_thread;
//     Building building;

//     init_building(&building, GRID);

//     // Ajouter des pièces et initialiser le bâtiment
//     add_room(&building, true);  // Ajouter une pièce avec lumière naturelle
//     add_room(&building, false); // Ajouter une pièce sans lumière naturelle

//     // Démarrer le thread pour gérer les sources d'énergie
//     if (pthread_create(&source_thread, NULL, (void *(*)(void *))source, &building) != 0) {
//         perror("Échec de la création du thread source");
//         exit(EXIT_FAILURE);
//     }

//     // Démarrer le thread pour simuler l'état de la GRID
//     if (pthread_create(&grid_thread, NULL, simulate_grid, NULL) != 0) {
//         perror("Échec de la création du thread simulate_grid");
//         exit(EXIT_FAILURE);
//     }

//     // Garder les threads en exécution
//     pthread_join(source_thread, NULL);
//     pthread_join(grid_thread, NULL);

//     printf("Simulation terminée.\n");

//     return 0;
// }
