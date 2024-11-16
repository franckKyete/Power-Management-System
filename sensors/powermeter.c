#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header.h"
#define MAX_DEVICES 10  // Nombre maximal d'appareils

// Définition des variables et structures nécessaires
Device added_devices[MAX_DEVICES];  // Tableau pour stocker les appareils ajoutés
int device_count = 0;  // Compteur pour le nombre d'appareils dans le tableau
float total_energy_solar = 0.0;
float total_energy_hydro = 0.0;
#define MAX_ENERGY_SOLAR 60.0  // Capacité maximale de l'énergie solaire en Wh

// Fonctions déjà présentes : generate_voltage, generate_current, generate_time, calculate_power, calculate_energy, communicate_to_server
float generate_voltage() {
    return (rand() % 240) + 100; // Tension entre 100V et 240V
}

// Fonction pour générer une intensité aléatoire
float generate_current() {
    return (rand() % 10) + 1;  // Intensité entre 1A et 10A
}

// Fonction pour générer un temps d'utilisation aléatoire
int generate_time() {
    return (rand() % 81) + 10; // Durée entre 10 secondes et 1min30 secondes
}

// Fonction pour calculer la puissance
float calculate_power(float voltage, float current) {
    return voltage * current;  // P = U * I
}

// Fonction pour calculer l'énergie
float calculate_energy(float power, int time) {
    return power * time / 3600;  // E = P * t (converti en Wh)
}

// Fonction pour envoyer un message au serveur
void communicate_to_server(int msgid, Device device) {
    device.msg_type = 1;  // Type de message
    if (msgsnd(msgid, &device, sizeof(Device) - sizeof(long), 0) == -1) {
        perror("Erreur lors de l'envoi du message");
        exit(1);
    }
}


void update_device_times() {
    for (int i = 0; i < device_count; i++) {
        added_devices[i].time -= 1;  // Décrémenter le temps de 1 seconde
        if (added_devices[i].time <= 0) {
            // Soustraire l'énergie de la source correcte
            if (strncmp(added_devices[i].name, "Ampoule", 7) == 0) {
                total_energy_solar -= added_devices[i].energy;
            } else {
                total_energy_hydro -= added_devices[i].energy;
            }
            // Supprimer l'appareil si le temps est écoulé
            for (int j = i; j < device_count - 1; j++) {
                added_devices[j] = added_devices[j + 1];
            }
            device_count--;  // Réduire le nombre d'appareils
            i--;  // Ajuster l'indice pour la suppression
        }
    }
    sleep(1);  // Attendre 1 seconde avant de décrémenter à nouveau
}

void recalculate_energies() {
    total_energy_solar = 0.0;
    total_energy_hydro = 0.0;

    for (int i = 0; i < device_count; i++) {
        if (strncmp(added_devices[i].name, "Ampoule", 7) == 0) {
            total_energy_solar += added_devices[i].energy;
        } else {
            total_energy_hydro += added_devices[i].energy;
        }
    }
}

int main() {
    int msgid;
    Device device;
    key_t key = ftok("server.c", 65);  // Clé pour la file de messages

    // Initialiser les appareils
    char* devices[] = {
        "Ampoule 1", "Ampoule 2", "Ampoule 3", "Ampoule 4", "Ampoule 5",
        "Climatiseur", "Ventilateur", "Télévision", "Ordinateur", "Congélateur"
    };

    while (1) {
        printf("\nChoisissez un appareil (1 à 10) ou 'F' pour finir : ");
        char choice;
        scanf(" %c", &choice);

        if (choice == 'F' || choice == 'f') {
            break; // Quitter si 'F' est choisi
        }

        int selected_device = choice - '0' - 1; // Convertir l'input en index de tableau

        if (selected_device >= 0 && selected_device < MAX_DEVICES) {
            // Générer les caractéristiques aléatoires pour l'appareil
            strncpy(device.name, devices[selected_device], sizeof(device.name) - 1);
            device.voltage = generate_voltage();
            device.current = generate_current();
            device.time = generate_time();
            device.power = calculate_power(device.voltage, device.current);
            device.energy = calculate_energy(device.power, device.time);

            // Envoyer l'appareil au serveur
            communicate_to_server(msgid, device);

            // Ajouter l'appareil à la liste des appareils
            added_devices[device_count] = device;
            device_count++;

            // Si le seuil de 60 Wh est dépassé, recalculer les énergies
            if (total_energy_solar > MAX_ENERGY_SOLAR) {
                recalculate_energies();
            } else {
                // Ajouter l'énergie à l'énergie solaire avant le seuil
                total_energy_solar += device.energy;
            }

            // Afficher le tableau des appareils avec le temps
            printf("\nTableau des appareils en cours:\n");
            printf("-----------------------------------------------------------------------------------------\n");
            printf("| Appareil       | Tension (V) | Intensité (A) | Puissance (W) |  Temps (s) |  Energie (Wh) |\n");
            printf("-----------------------------------------------------------------------------------------\n");

            for (int i = 0; i < device_count; i++) {
                printf("| %-15s | %-12.2f | %-14.2f | %-13.2f | %-8d | %-13.2f |\n", 
                    added_devices[i].name, added_devices[i].voltage, 
                    added_devices[i].current, added_devices[i].power, added_devices[i].time,
                    added_devices[i].energy);
            }
            printf("-----------------------------------------------------------------------------------------\n");

            // Afficher les totaux d'énergie
            printf("Total Energie Solaire : %.2f Wh\n", total_energy_solar);
            printf("Total Energie Hydroélectrique : %.2f Wh\n", total_energy_hydro);

            // Attendre que le temps soit écoulé pour chaque appareil
            sleep(1); // Mettre en pause pour simuler un délai entre les ajouts d'appareils
        } else {
            printf("Choix invalide. Veuillez entrer un nombre entre 1 et 10.\n");
        }
    }

    return 0;
}
