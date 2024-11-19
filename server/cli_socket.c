#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "cli_socket.h"
#include <asm-generic/socket.h>



void *cli_socket(void *_building){

    Building *building = (Building *)_building;

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
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening\n");

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        // valread = read(new_socket, buffer, 1024 - 1);
        // printf("%s\n", buffer);
        pthread_mutex_lock(&building->building_lock);
        send(new_socket, building, sizeof(Building), 0);
        pthread_mutex_unlock(&building->building_lock);
        // printf("message sent\n");
        sleep(1);
    }
    
    

    close(new_socket);
    close(server_fd);

    pthread_exit(NULL);
}