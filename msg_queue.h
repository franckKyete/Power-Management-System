// msg_queue.h
#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPTEUR_CO2         1234
#define CAPTEUR_PRESENCE    1235
#define CAPTEUR_TEMPERATURE 1236
#define CAPTEUR_ENERGY      1237


struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

#endif
