#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
    #include <windows.h> // Pour Sleep() sous Windows
#else
    #include <unistd.h>  // Pour sleep() sous Unix/Linux
#endif

#include "../msg_queue.h"
#include "../shared_mem.h"

// Fonction pour simuler la détection de présence
int get_presence(shared_memory* shm_ptr, const int room_id) {
    if(!shm_ptr->is_written)
        return rand() % 2; // Génère un nombre aléatoire 0 ou 1
    return shm_ptr->values[((room_id-1)*4) + 1];
}

int main(int argc, char **argv) {
    int msgid, shmid;
    sensor_msg message;
    shared_memory *shm_ptr;

    int presence;
    // int compteurTemps = 0;
    const int interval = 1;

    if(argc < 2){
        printf("The room number must be set\n");
        exit(1);
    }
    const long room_id = atoi(argv[1]);

    // Initialiser le générateur de nombres aléatoires
    srand(room_id);

    msgid = msgget(CAPTEUR_PRESENCE, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    shmid = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
    if(shmid == -1){
        perror("shmget failed");
        exit(1);
    }
    shm_ptr = (shared_memory*)shmat(shmid, NULL, 0);
    if(shm_ptr == (shared_memory*)(-1)){
        perror("shmat failed");
        exit(1);
    }

    printf("Sensor for room  %d started\n\n", (int)room_id);

    while (1) { // Boucle infinie pour générer des données en continu
        presence = get_presence(shm_ptr, room_id);
        
        // if (presence) {
        //     printf("Présence détectée.\n");
        // } else {
        //     printf("Aucune présence détectée.\n");
        // }

        message.room_id = room_id;
        message.value = presence;
        // Send the message
        if (msgsnd(msgid, &message, sizeof(message.value), 0) == -1) {
            perror("msgsnd failed");
        }else{
            // printf("Message sent: %f\n", message.value);
        }

        // Temporisation d'une seconde avant la prochaine génération
        #ifdef _WIN32
            Sleep(interval*1000); // Sous Windows, Sleep est en millisecondes
        #else
            sleep(interval); // Sous Linux/Unix, sleep est en secondes
        #endif
    }
    return 0;
}