#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>


#include "../building.h"


void* room_power_meter(void *_room){

    Room *room = (Room *)_room;

    Device *device;
    int total_consomation;
    while (1) {
        
        total_consomation = 0;
        for (size_t d = 0; d < room->devices_nb; d++){
            device = &room->devices[d];
            total_consomation += device->power;
        }
        pthread_mutex_lock(room->building_lock);
        room->sensors[POWER_METER].value = total_consomation;
        pthread_mutex_unlock(room->building_lock);
    }
    pthread_exit(NULL);
}

void* power_meters(void *_building) {

    Building *building = (Building *)_building;
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
 