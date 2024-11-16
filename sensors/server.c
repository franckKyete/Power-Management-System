// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "header.h"

// Fonction pour gérer la réception des messages et calculer l'énergie totale
void handle_server_message(int msgid) {
    Device device;
    float total_energy = 0;

    while (1) {
        printf("Serveur en attente de messages...\n");  // Débogage : vérifier que le serveur attend

        // Recevoir un message du client
        if (msgrcv(msgid, &device, sizeof(Device) - sizeof(long), 1, 0) == -1) {
            perror("Erreur lors de la réception du message");
            exit(1);
        }

        printf("Message reçu : %s\n", device.name);  // Débogage : vérifier que le message est bien reçu
        
        // Calcul de l'énergie totale
        total_energy += device.energy;
        printf("Appareil: %s, Energie: %.2f Wh\n", device.name, device.energy);
        printf("Total énergie consommée: %.2f Wh\n", total_energy);

        if (total_energy > 2000) {
            printf("Alerte: La capacité de la source d'énergie a été dépassée !\n");
            total_energy = 0; // Reset total energy after exceeding capacity
        }
    }
}

// Fonction pour communiquer avec le serveur (via la file de messages)
void communicate_to_server(int msgid, Device device) {
    device.msg_type = 1;  // Type de message
    if (msgsnd(msgid, &device, sizeof(Device) - sizeof(long), 0) == -1) {
        perror("Erreur lors de l'envoi du message");
        exit(1);
    }
}

int main() {
    // Clé pour la file de messages
    key_t key = ftok("server.c", 65);
    if (key == -1) {
        perror("Erreur lors de la création de la clé");
        exit(1);
    }

    // Création de la file de messages
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("Erreur lors de la création de la file de messages");
        exit(1);
    }

    // Boucle pour gérer la réception des messages
    while (1) {
        handle_server_message(msgid);
    }

    return 0;
}
