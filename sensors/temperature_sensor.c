#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "../msg_queue.h"
#include "../shared_mem.h"

#define CHAUFFAGE_MIN 10
#define CHAUFFAGE_MAX 31

float get_temperature(shared_memory* shm_ptr, const int room_id) {
    if(!shm_ptr->is_written){
        return (rand() % (CHAUFFAGE_MAX - CHAUFFAGE_MIN)) + CHAUFFAGE_MIN;
    }
    return shm_ptr->values[((room_id-1)*4) + 2];
}

int main(int argc, char **argv) {

    int msgid, shmid;
    sensor_msg message;
    shared_memory *shm_ptr;

    int temperature;
    const int interval = 100000;

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

    // printf("Sensor for room  %d started\n\n", (int)room_id);

    while (1) { // Boucle infinie pour générer des données en continu
        temperature = get_temperature(shm_ptr, room_id);
        
        
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
            usleep(interval); // Sous Linux/Unix, sleep est en secondes
        #endif
    }
    return 0;
}