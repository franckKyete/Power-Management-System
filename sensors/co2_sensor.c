#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "../msg_queue.h"

// Fonction qui génère une valeur simulée de CO2 entre 400 et 5000 ppm
int genererValeurCO2() {
    return rand() % 4601 + 400; // génère une valeur entre 400 et 5000
}

int main(int argc, char **argv) {
    int msgid;
    sensor_msg message;

    float CO2_level = 400.5;// niveau de CO2 en partie par million  (exemple)
    int compteurTemps = 0;
    const int interval = 1;

    if(argc < 2){
        printf("The room number must be set\n");
        exit(1);
    }
    const long room_id = atoi(argv[1]);

    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    msgid = msgget(CAPTEUR_CO2, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("Sensor for room  %d started\n\n", (int)room_id);

    // Boucle infinie pour simuler la mesure toutes les 30 secondes
    while (1) {
        // Attendre 30 secondes
        sleep(interval);
        compteurTemps += interval;

        // Générer et afficher une nouvelle valeur de CO2
        CO2_level = genererValeurCO2();
        printf("Temps: %d secondes, Valeur de CO2: %f ppm\n", compteurTemps, CO2_level);

        // Préparation du message
        message.room_id = room_id; // type de message, nécessaire pour l'envoi 
        message.value = CO2_level;
        
        // Send the message
        if (msgsnd(msgid, &message, sizeof(message.value), 0) == -1) {
            perror("msgsnd failed");
        }else{
            printf("Message sent: %f\n", message.value);
        }
        printf("\n");
    }
}

