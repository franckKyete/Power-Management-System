#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include "building.h"

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
void *source(Building *building) {
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
            puissance_totale += puissance_piece;

            // Gestion de l'énergie en fonction de l'état de la GRID
            if (!grid_active && building->rooms[i].power_source == GRID) {
                building->rooms[i].power_source = GENERATOR; // Basculer sur le générateur
                //printf("Pièce %d déplacée sur le générateur.\n", i);
            } else if (grid_active && building->rooms[i].power_source == GENERATOR) {
                building->rooms[i].power_source = GRID; // Remettre sur la GRID
                //printf("Pièce %d remise sur la GRID.\n", i);
            }
        }

        // Si la puissance dépasse la batterie, gestion des pièces les plus consommatrices
        if (puissance_totale > puissance_batterie && !grid_active) {
            //printf("La batterie ne peut pas supporter la charge. Réduction progressive...\n");
            for (int i = 0; i < building->size; i++) {
                float puissance_piece = 0.0;

                // Calcul de la puissance consommée par la pièce
                for (int j = 0; j < MAX_DEVICES; j++) {
                    puissance_piece += building->rooms[i].devices[j].power;
                }

                // Basculer les pièces non prioritaires hors de la batterie
                if (puissance_piece > 0) {
                    building->rooms[i].power_source = GENERATOR;
                    puissance_totale -= puissance_piece;
                    //printf("Pièce %d déplacée sur le générateur pour alléger la batterie.\n", i);
                }

                if (puissance_totale <= puissance_batterie) {
                    break; // Arrêter dès que la charge est suffisante
                }
            }
        }

        sleep(1); // Éviter une boucle trop rapide
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    pthread_t source_thread, grid_thread;
    Building building;

    init_building(&building, GRID);

    // Ajouter des pièces et initialiser le bâtiment
    add_room(&building, true);  // Ajouter une pièce avec lumière naturelle
    add_room(&building, false); // Ajouter une pièce sans lumière naturelle

    // Démarrer le thread pour gérer les sources d'énergie
    if (pthread_create(&source_thread, NULL, (void *(*)(void *))source, &building) != 0) {
        perror("Échec de la création du thread source");
        exit(EXIT_FAILURE);
    }

    // Démarrer le thread pour simuler l'état de la GRID
    if (pthread_create(&grid_thread, NULL, simulate_grid, NULL) != 0) {
        perror("Échec de la création du thread simulate_grid");
        exit(EXIT_FAILURE);
    }

    // Garder les threads en exécution
    pthread_join(source_thread, NULL);
    pthread_join(grid_thread, NULL);

    printf("Simulation terminée.\n");

    return 0;
}
