#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>


#include "../building.h"


// Fonction pour calculer la puissance
float calculate_power(float voltage, float current) {
    return voltage * current;  // P = U * I
}

// Fonction pour calculer l'énergie
float calculate_energy(float power, int time) {
    return power * time / 3600;  // E = P * t (converti en Wh)
}

void* room_power_meter(Room *room){
    Device *device;
    int total_consomation;

    while (1) {
        
        total_consomation = 0;
        for (size_t d = 0; d < room->devices_nb; d++){
            device = &room->devices[d];
            total_consomation = calculate_power(device->voltage, device->current);
        }
        pthread_mutex_lock(&room->lock);
        room->sensors[POWER_METER].value = total_consomation;
        pthread_mutex_unlock(&room->lock);
        
    }
    pthread_exit(NULL);
}

void* power_meters(Building *building) {

    pthread_t room_power_meter_thread[MAX_ROOM];
    Room *room;

    for (size_t i = 0; i < building->size; i++){
        room = &building->rooms[i];
        if(pthread_create(&room_power_meter_thread[i], NULL, room_power_meter, room) != 0){
            perror("Failed to create power meter thread");
            exit(EXIT_FAILURE);
        }
    }
    for (size_t i = 0; i < building->size; i++){
        pthread_join(room_power_meter_thread[i], NULL);
    }
    pthread_exit(NULL);
}
