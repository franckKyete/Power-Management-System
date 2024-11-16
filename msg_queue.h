// msg_queue.h
#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPTEUR_CO2         1
#define CAPTEUR_PRESENCE    2
#define CAPTEUR_TEMPERATURE 3
#define CAPTEUR_ENERGY      4


typedef struct {
    long room_id;
    float value;
} sensor_msg;

#endif