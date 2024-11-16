#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include "server.h"

pthread_mutex_t building_lock;
bool running = true;
Building *building;

int main(int argc, char **argv){
    pthread_t msg_dispatcher_thread;

    building = init_building(SOLAR);

    if(add_room(building, true) == -1){
        perror("Failed to add a room");
        exit(0);
    }

    if(pthread_create(&msg_dispatcher_thread, NULL, msg_dispatcher, building) != 0){
        perror("Failed to create message dispatcher thread");
    }
    sleep(5);
    running = false;
    pthread_join(msg_dispatcher_thread, NULL);
    printf("Complete\n");

    free_building(building);

    return 0;
}

Building *init_building(PowerSource prefered_power_source){
    Building *_building;
    _building = (Building *)malloc(sizeof(Building));
    _building->size = 0;
    _building->prefered_power_source = prefered_power_source;
}
void free_building(Building *_building){

    for (size_t i = 0; i < _building->size; i++){
        for (size_t s = 0; s < 4; s++){
            free(_building->sensors[i*4 + s]);
        }
        free( _building->rooms[i]);
    }
}

int add_room(Building *_building, bool natural_light){
    
    Room *_room;

    if (_building->size >= MAX_ROOM){
        printf("Cannot add any more rooms\n");
        return -1;
    }

    _room = (Room *)malloc(sizeof(Room));

    _room->ac_temp = 0;
    _room->ac = false;
    _room->light = false;
    _room->ventilation = false;
    _room->natural_light = natural_light;
    _room->power_source = _building->prefered_power_source;

    _room->id = _building->size;
    _building->size++;

    Sensor *sensor;
    for (size_t i = 0; i < 4; i++){
        sensor = (Sensor*)malloc(sizeof(Sensor));
        sensor->type = i;
        sensor->value = 0;
        sensor->room_id = _room->id;
        _room->sensors[i] = sensor;
        _building->sensors[(_room->id*4) + i] = sensor;
    }
    
    return 0;
}

void *msg_dispatcher(Building *_building){
    int msgids[4];
    sensor_msg message;

    // `i` will take the values 0 to 4 which are the same values for the sensor type msg_queue ids
    for(int i=0; i<4; i++){
        // Access the message queue
        msgids[i] = msgget(i+1, 0666 | IPC_CREAT);
        if (msgids[i] == -1) {
            perror("msgget failed");
            exit(1);
        }
    }

    while (running)
    {
        for(int i=0; i<4; i++){
            // Receive the message
            if (msgrcv(msgids[i], &message, sizeof(message.value), 0, IPC_NOWAIT) == -1) {// Remove the IPC_NOWAIT
                if(errno == ENOMSG){
                    //printf("No message received\n");
                }else{
                    perror("msgrcv failed");
                    exit(1);
                }   
            }else{
                printf("Received : %f\n", message.value);
                dispatch_msg(_building, &message, i);
            }
        }
    }
    pthread_exit(NULL);
}



void dispatch_msg(Building *_building, sensor_msg *message, SensorType sensor_type){

    Sensor *sensor = _building->sensors[((message->room_id-1) * 4) + sensor_type];
    sensor->value = message->value;
}