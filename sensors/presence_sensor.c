#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h> // Pour Sleep() sous Windows
#else
    #include <unistd.h>  // Pour sleep() sous Unix/Linux
#endif

#include "../msg_queue.h"

// Fonction pour simuler la détection de présence
int genererPresence() {
    return rand() % 2; // Génère un nombre aléatoire 0 ou 1
}

int main(int argc, char **argv) {

    int msgid;
    sensor_msg message;

    int presence;
    int compteurTemps = 0;
    const int interval = 1;

    if(argc < 2){
        printf("The room number must be set\n");
        exit(1);
    }
    const long room_id = atoi(argv[1]);

    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    msgid = msgget(CAPTEUR_PRESENCE, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("Sensor for room  %d started\n\n", (int)room_id);

    while (1) { // Boucle infinie pour générer des données en continu
        presence = genererPresence();
        
        if (presence) {
            printf("Présence détectée.\n");
        } else {
            printf("Aucune présence détectée.\n");
        }

        message.room_id = room_id;
        message.value = presence;
        // Send the message
        if (msgsnd(msgid, &message, sizeof(message.value), 0) == -1) {
            perror("msgsnd failed");
        }else{
            printf("Message sent: %f\n", message.value);
        }
        printf("\n");

        // Temporisation d'une seconde avant la prochaine génération
        #ifdef _WIN32
            Sleep(interval*1000); // Sous Windows, Sleep est en millisecondes
        #else
            sleep(interval); // Sous Linux/Unix, sleep est en secondes
        #endif
    }
    return 0;
}