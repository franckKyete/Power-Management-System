#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include "server.h"

pthread_mutex_t building_lock;
bool running = true;
Building *building;

int main(int argc, char **argv){
    pthread_t msg_dispatcher_thread;

    if(pthread_create(&msg_dispatcher_thread, NULL, msg_dispatcher, NULL) != 0){
        perror("Failed to create message dispatcher thread");
    }
    sleep(5);
    running = false;
    pthread_join(msg_dispatcher_thread, NULL);
    printf("Complete\n");


    return 0;
}

void *msg_dispatcher(){
    int msgids[4];
    sensor_msg message;

    // `i` will take the values 0 to 4 which are the same values for the sensor type msg_queue ids
    for(int i=0; i<4; i++){
        // Access the message queue
        msgids[i] = msgget(i, 0666 | IPC_CREAT);
        if (msgids[i] == -1) {
            perror("msgget failed");
            exit(1);
        }
    }

    while (running)
    {
        for(int i=0; i<4; i++){
            // Receive the message
            printf("i : %d\n",i);
            if (msgrcv(msgids[i], &message, sizeof(message.value), 0, IPC_NOWAIT) == -1) {// Remove the IPC_NOWAIT
                if(errno == ENOMSG){
                    printf("No message received\n");
                }else{
                    perror("msgrcv failed");
                    exit(1);
                }   
            }else{
                dispatch_msg(&message, i);


                // // Optional: Delete the message queue after receiving
                // if (msgctl(msgids[i], IPC_RMID, NULL) == -1) {
                //     perror("msgctl failed");
                //     exit(1);
                // }
            }
        }
    }
    pthread_exit(NULL);
}



void dispatch_msg(sensor_msg *message, SensorType sensor_type){
    Sensor *sensor = building->sensors[message->room_id];
    sensor->value = message->value;
}