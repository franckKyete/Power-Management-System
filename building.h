#ifndef BUILDING_H
#define BUILDING_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define MAX_ROOM 20
#define MAX_DEVICES 20

typedef enum {
    SOLAR, GENERATOR, GRID
} PowerSource;

typedef enum {
    CO2_SENSOR=0, 
    PRESENCE_SENSOR=1, 
    TEMPERATURE_SENSOR=2, 
    POWER_METER=3
} SensorType;


typedef struct
{
    int room_id;
    SensorType type;
    float value;    
} Sensor;

// Structure pour représenter un appareil
typedef struct {
    long msg_type;         // Type de message (pour la file de messages)
    char name[50];         // Nom de l'appareil
    float voltage;         // Tension en Volts
    float current;         // Intensité en Ampères
    float power;           // Puissance en Watts
    float energy;          // Énergie en Wh
    int time;              // Durée d'utilisation en secondes
} Device;


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
    Device divices[MAX_DEVICES];
} Room;


typedef struct
{
    Room rooms[MAX_ROOM];
    
    int size; // Number of rooms
    PowerSource prefered_power_source;

    char null;
} Building;


void init_building(Building *building, PowerSource prefered_power_source);
int add_room(Building *_building, bool natural_light);


#endif