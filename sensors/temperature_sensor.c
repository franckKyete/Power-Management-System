#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "../msg_queue.h"

#define CHAUFFAGE_MIN 10
#define CHAUFFAGE_MAX 31

float valeur_temperature() {
    return (rand() % (CHAUFFAGE_MAX - CHAUFFAGE_MIN)) + CHAUFFAGE_MIN;
}

int main(int argc, char **argv) {

    int msgid;
    sensor_msg message;

    int temperature;
    const int interval = 1;

    if(argc < 2){
        printf("The room number must be set\n");
        exit(1);
    }
    const long room_id = atoi(argv[1]);

    // Initialiser le générateur de nombres aléatoires
    srand(room_id);

    msgid = msgget(CAPTEUR_TEMPERATURE, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // printf("Sensor for room  %d started\n\n", (int)room_id);

    while (1) { // Boucle infinie pour générer des données en continu
        temperature = valeur_temperature();
        
        
        // printf("Temperature détectée : %d \n", temperature);

        message.room_id = room_id;
        message.value = temperature;
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