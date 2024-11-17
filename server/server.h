#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../msg_queue.h"
#include "../building.h"


void dispatch_msg(Building *building, sensor_msg *message, SensorType sensor_type);
void *msg_dispatcher();


#endif