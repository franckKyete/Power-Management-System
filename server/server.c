#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <signal.h>

#include "server.h"
#include "cli_socket.h"

pthread_mutex_t building_lock;
bool running = true;

int main(int argc, char **argv){
    
    Building building;
    pthread_t msg_dispatcher_thread, cli_socket_thread, power_meters_threads;

    init_building(&building, SOLAR);

    for (size_t i = 0; i < 9; i++)
    {
        if(add_room(&building, true) == -1){
            perror("Failed to add a room");
            exit(EXIT_FAILURE);
        }
    }
    

    add_device(&building, NULL, 1, 120.0, 10.0);

    start_sensors(&building);

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

void start_sensors(Building *building){

    char argv[5];
    for(int sensor_type = 0; sensor_type < 3; sensor_type++){
        for (size_t room_id = 1; room_id <= building->size; room_id++){
            pid_t pid = fork();
            if(pid == 0){
                prctl(PR_SET_PDEATHSIG, SIGTERM);
                if(getpid() == 1){
                    exit(EXIT_FAILURE);
                }
                snprintf(argv, sizeof(argv), "%d", (int)room_id);
                // sprintf(argv, "%d", (int)room_id);
                // itoa(room_id, argv, 10);
                switch (sensor_type)
                {
                case CO2_SENSOR:
                    execlp("./build/co2_sensor", "co2_sensor", argv, NULL);
                    break;
                case PRESENCE_SENSOR:
                    execlp("./build/presence_sensor", "presence_sensor", argv, NULL);
                    break;
                case TEMPERATURE_SENSOR:
                    execlp("./build/temperature_sensor", "temperature_sensor", argv, NULL);
                    break;
                default:
                    break;
                }
                
                perror("execlp failed");
                exit(EXIT_FAILURE);
            }else if (pid > 0)
            {
                continue;
            }else{
                perror("fork failed");
                exit(EXIT_FAILURE);
            }
        }
    }
    
}

void *msg_dispatcher(void *_building){
    Building *building = (Building*)_building;
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
    
    if(message->room_id > building->size){
        return;
    }

    Room *room = &building->rooms[message->room_id-1];
    Sensor *sensor = &room->sensors[sensor_type];

    pthread_mutex_lock(&building->building_lock);
    sensor->value = message->value;
    pthread_mutex_unlock(&building->building_lock);
}