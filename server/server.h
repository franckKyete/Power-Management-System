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

#define MAX_ROOM 20

typedef enum {
    SOLAR, GENERATOR, GRID
} PowerSource;

typedef enum {
    CO2_SENSOR=0, PRESENCE_SENSOR=1, TEMPERATURE_SENSOR=2, POWER_METER=3
} SensorType;


typedef struct
{
    int room_id;
    SensorType type;
    float value;    
} Sensor;


typedef struct 
{
    int id; 

    bool light;
    bool ventilation;
    bool ac;
    float ac_temp;
    PowerSource power_source;

    bool natural_light;

    Sensor sensors[4];
} Room;


typedef struct
{
    Room **rooms;
    Sensor **sensors;
    int size; // Number of rooms
    
} Building;


void dispatch_msg(sensor_msg *msg, SensorType sensor_type);
void *msg_dispatcher();


#endif