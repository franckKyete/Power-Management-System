
#include "building.h"

void init_building(Building *building, PowerSource prefered_power_source){

    building->size = 0;
    building->prefered_power_source = prefered_power_source;
    if(pthread_mutex_init(&building->building_lock, NULL) != 0){
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }
    building->null = '\0';
}


int add_room(Building *building, bool natural_light){
    
    Room *room;

    if (building->size >= MAX_ROOM){
        printf("Cannot add any more rooms\n");
        return -1;
    }

    
    room = &building->rooms[building->size];
    room->id = building->size;
    building->size++;

    room->devices_nb = 0;
    room->ac_temp = 0;
    room->ac = false;
    room->light = false;
    room->ventilation = false;
    room->natural_light = natural_light;
    room->power_source = building->prefered_power_source;

    if(pthread_mutex_init(&room->lock, NULL) != 0){
        perror("Mutex initialization failed");
        exit(EXIT_FAILURE);
    }

    Sensor *sensor;
    for (size_t i = 0; i < 4; i++){
        sensor = &room->sensors[i];
        sensor->type = i;
        sensor->value = 0;
        sensor->room_id = room->id;
    }
    
    return 0;
}