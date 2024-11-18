#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "server.h"
#include "cli_socket.h"

pthread_mutex_t building_lock;
bool running = true;
Building building;

int main(int argc, char **argv){
    pthread_t msg_dispatcher_thread, cli_socket_thread, power_meters_threads;

    init_building(&building, SOLAR);

    if(add_room(&building, true) == -1){
        perror("Failed to add a room");
        exit(EXIT_FAILURE);
    }
    if(add_room(&building, true) == -1){
        perror("Failed to add a room");
        exit(EXIT_FAILURE);
    }
    if(add_room(&building, true) == -1){
        perror("Failed to add a room");
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&msg_dispatcher_thread, NULL, msg_dispatcher, &building) != 0){
        perror("Failed to create message dispatcher thread");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&cli_socket_thread, NULL, cli_socket, &building) != 0){
        perror("Failed to create message cli socket thread");
        exit(EXIT_FAILURE);
    }
    if(pthread_create(&power_meters_threads, NULL, power_meters, &building) != 0){
        perror("Failed to create message power meters threads");
        exit(EXIT_FAILURE);
    }

    pthread_join(msg_dispatcher_thread, NULL);
    pthread_join(cli_socket_thread, NULL);
    pthread_join(power_meters_threads, NULL);

    printf("Complete\n");

    return 0;
}


void *msg_dispatcher(Building *building){
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

    while (1){
        for(int sensor_type=0; sensor_type<4; sensor_type++){
            // Receive the message
            if (msgrcv(msgids[sensor_type], &message, sizeof(message.value), 0, IPC_NOWAIT) == -1) {// Remove the IPC_NOWAIT
                if(errno == ENOMSG){
                    //printf("No message received\n");
                }else{
                    perror("msgrcv failed");
                    exit(1);
                }   
            }else{
                // printf("Received : %f\n", message.value);
                dispatch_msg(building, &message, sensor_type);
            }
        }
    }
    pthread_exit(NULL);
}



void dispatch_msg(Building *building, sensor_msg *message, SensorType sensor_type){
    
    Room *room = &building->rooms[message->room_id-1];
    Sensor *sensor = &room->sensors[sensor_type];

    pthread_mutex_lock(&room->lock);
    sensor->value = message->value;
    pthread_mutex_unlock(&room->lock);
}