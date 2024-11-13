    // sender.c
#include "msg_queue.h"

int main() {
    int msgid;
    struct msg_buffer message;
    //déclaration  des variables capteur
    float CO2_level = 400.5;// niveau de CO2 en partie par million  (exemple)
    float temperature = 22.3;// temperature en degrés Celsius (exemple)
    float humidity = 45.2; // humidity (exemple)
}
    // Create or access the message queue
    msgid = msgget(CAPTEUR_CO2, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    // Fonction qui génère une valeur simulée de CO2 entre 400 et 5000 ppm
int genererValeurCO2() {
    return rand() % 4601 + 400; // génère une valeur entre 400 et 5000
}
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));

    // Compteur de temps simulé
    int compteurTemps = 0;

    // Boucle infinie pour simuler la mesure toutes les 30 secondes
    while (1) {
        // Attendre 30 secondes
        sleep(30);
        compteurTemps += 30;

        // Générer et afficher une nouvelle valeur de CO2
        int valeurCO2 = genererValeurCO2();
        printf("Temps: %d secondes, Valeur de CO2: %d ppm\n", compteurTemps, valeurCO2);

    // Préparation du message
    message.msg_type = 1; // type de message, nécessaire pour l'envoi 

    //formatage des données capteur dans msg_text
    snprintf(message.msg_text, sizeof(message.msg_text),"CO2: %.1f ppm,temperature:%.1f Celsius, humidity: %.1f%% ", CO2_level, temperature, humidity);

    // Send the message
        if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
        }
        
printf("Message sent: %s\n", message.msg_text);
return 0;
    }
