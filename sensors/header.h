// header.h

#ifndef HEADER_H
#define HEADER_H

#define MAX_DEVICES 10
#define MAX_MESSAGE_SIZE 256

// Structure pour représenter un appareil
typedef struct {
    long msg_type;         // Type de message (pour la file de messages)
    char name[50];         // Nom de l'appareil
    float voltage;         // Tension en Volts
    float current;         // Intensité en Ampères
    float power;           // Puissance en Watts
    float energy;          // Énergie en Wh
    int time;              // Durée d'utilisation en secondes
} Device;

// Fonction pour générer une tension aléatoire
float generate_voltage();

// Fonction pour générer une intensité aléatoire
float generate_current();

// Fonction pour générer un temps d'utilisation aléatoire
int generate_time();

// Fonction pour calculer la puissance
float calculate_power(float voltage, float current);

// Fonction pour calculer l'énergie
float calculate_energy(float power, int time);

// Fonction pour envoyer un message au serveur
void communicate_to_server(int msgid, Device device);

// Fonction pour gérer la réception des messages et calculer l'énergie totale
void handle_server_message(int msgid);

#endif // HEADER_H
