#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server_sockets.h"
#include <asm-generic/socket.h>


#include "server.h"

void get_json_room_state(Room *room, char *output, size_t size){
    char power_source[11];
    char ventilation[6];
    char ac[6];
    char light[6];
    char presence[6];

    switch (room->power_source)
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

        if(room->light){
            sprintf(light, "true");
        }else{
            sprintf(light, "false");
        }

        if(room->ventilation){
            sprintf(ventilation, "true");
        }else{
            sprintf(ventilation, "false");
        }

        if(room->ac){
            sprintf(ac, "true");
        }else{
            sprintf(ac, "false");
        }
        if(room->sensors[PRESENCE_SENSOR].value){
            sprintf(presence, "true");
        }else{
            sprintf(presence, "false");
        }


    snprintf(output, size,
    "{"
    "\"light\" : %s,"
    "\"ventilation\" : %s,"
    "\"presence\" : %s,"
    "\"ac\" : %s,"
    "\"power_source\" : \"%s\""
    "}",
    light, ventilation, presence, ac, power_source);
}

void get_json_building_state(Building *building, char*output, size_t size){
    
    char rooms_json[256*20] = "[";
    char temp[256];

    for (size_t i = 0; i < building->size; i++)
    {
        get_json_room_state(&building->rooms[i], temp, sizeof(temp));
        strcat(rooms_json, temp);
        if(i< building->size - 1){
            strcat(rooms_json, ",");
        }
    }
    strcat(rooms_json, "]");

    snprintf(output, size,
    "{"
    "\"size\" : %d,"
    "\"rooms\" : %s"
    "}",
    building->size, rooms_json);

}


#define BUFFER_SIZE 5120

void *gui_socket(void *_building){

    Building *building = (Building *)_building;
    char building_json[BUFFER_SIZE];

    int server_fd, new_socket;
    struct sockaddr_in address;
    
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(GUI_PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("GUI socket listening\n");
    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
    
        get_json_building_state(building, building_json, sizeof(building_json));
        
        char response[BUFFER_SIZE+200];

        snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s\n", 
        (int)(strlen(building_json)), building_json);

        pthread_mutex_lock(&building->building_lock);
        send(new_socket, response, sizeof(response), MSG_NOSIGNAL);
        pthread_mutex_unlock(&building->building_lock);

        close(new_socket);
        usleep(100000);
    }
    
    close(server_fd);

    pthread_exit(NULL);
}