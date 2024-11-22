#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "cli.h"

void displaySummary(Building* building){

    char power_source[11];
    char ventilation[4];
    char ac[4];
    char light[4];
    char presence[4];

    printf("| %-11s | %-11s | %-11s | %-11s | %-11s | %-11s | %-11s | %-11s | %-11s |\n", "Piece", "CO2 (ppm)", "Presence", "Temp (C)", "Usage (W)", "A/C", "Lumiere", "Ventilation", "Power source");
    printf("|-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|-------------|\n");

    for (int i = 0; i < building->size; i++) {

        switch (building->rooms[i].power_source)
        {
        case SOLAR:
            sprintf(power_source, "SOLAR");
            break;
        case GENERATOR:
            sprintf(power_source, "GENERATOR");
            break;
        case GRID:
            sprintf(power_source, "GRID");
            break;
        default:
            break;
        }

        if(building->rooms[i].light){
            sprintf(light, "ON");
        }else{
            sprintf(light, "OFF");
        }

        if(building->rooms[i].ventilation){
            sprintf(ventilation, "ON");
        }else{
            sprintf(ventilation, "OFF");
        }

        if(building->rooms[i].ac){
            sprintf(ac, "ON");
        }else{
            sprintf(ac, "OFF");
        }
        if(building->rooms[i].sensors[PRESENCE_SENSOR].value){
            sprintf(presence, "YES");
        }else{
            sprintf(presence, "NO");
        }



        printf("| %-11d | %11.2f | %11s | %11.2f | %11.2f | %11s | %11s | %11s | %11s |\n", 
        building->rooms[i].id,
        building->rooms[i].sensors[CO2_SENSOR].value, 
        presence,
        building->rooms[i].sensors[TEMPERATURE_SENSOR].value, 
        building->rooms[i].sensors[POWER_METER].value, 
        ac,
        light, 
        ventilation,
        power_source);

    }

}


int main(int argc, char **argv){
 
    Building building;

    pthread_t cli_socket_thread;

    if(pthread_create(&cli_socket_thread, NULL, cli_socket, &building) != 0){
        perror("Failed to create cli_socket client thread");
        exit(EXIT_FAILURE);
    }

    while (1)
    {    
        system("clear");
        displaySummary(&building);
        // displayMenu();
        usleep(100000);
    }
    
    return 0;
}