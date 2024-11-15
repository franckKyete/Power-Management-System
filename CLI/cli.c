#include "cli.h"

void displaySummary(Room* rooms, int size){


    printf("| %-11s | %-11s | %-11s | %-11s | %-11s | %-11s | %-11s |\n", "Piece", "Presence", "CO2 (\%)", "Temp (C)", "Consomation (W)", "Lumiere", "Ventilation");
    printf("---------------------------------------------------------------------------------------------------\n");

    // Display each row with column separators
    for (int i = 0; i < size; i++) {
        printf("| %-11d | %-11d | %-11f | %-11f | %-11f | %-11d | %-11d |\n", 
        i,
        rooms[i].presence, 
        rooms[i].CO2_level, 
        rooms[i].temperature, 
        rooms[i].power_usage, 
        rooms[i].light, 
        rooms[i].ventilation);
    }

}

int main(int argc, char **argv){
    Room rooms[MAX_ROOM];

    
    rooms[0].presence    =  false;
    rooms[0].CO2_level   =  21.3;
    rooms[0].temperature =  23.5;
    rooms[0].power_usage =  0;
    rooms[0].light       =  false;
    rooms[0].ventilation =  false;

    rooms[1].presence    =  true;
    rooms[1].CO2_level   =  21.1;
    rooms[1].temperature =  23.52;
    rooms[1].power_usage =  2.1;
    rooms[1].light       =  true;
    rooms[1].ventilation =  false;

    rooms[2].presence    =  true;
    rooms[2].CO2_level   =  21.1;
    rooms[2].temperature =  23.52;
    rooms[2].power_usage =  2.1;
    rooms[2].light       =  true;
    rooms[2].ventilation =  false;
 

    system("clear");
    displaySummary(rooms, 3);
    return 0;
}