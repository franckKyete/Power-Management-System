#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <asm-generic/socket.h>

#include "../shared_mem.h"

#define PORT 8082
#define BUFFER_SIZE 4096


void write_shared_memory(char *buffer, shared_memory *shm_ptr){

    int room_id, sensor_type;
    float value;
    char *line;

    line = strtok(buffer, "\n");
    while (line != NULL)
    {
        if(sscanf(line, "%d;%d;%f", &room_id, &sensor_type, &value) == 3){
            shm_ptr->values[(room_id*4) + sensor_type] = value;
        }
        line = strtok(NULL, "\n");
    }
}

void handle_client(int client_socket, shared_memory *shm_ptr){
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE-1, 0);

    if(bytes_received < 0){
        perror("Failed to receive");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';
    // printf("HTTP Request : \n%s\n", buffer);

    char *body = strstr(buffer, "\r\n\r\n");
    printf("Data : %s\n", body);
    if(body){
        body += 4;
        write_shared_memory(body, shm_ptr);
    }

    const char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Content-Length: 27\r\n"
        "\r\n"
        "Data received successfully.\n";
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
}

int main(){
    int shmid;
    shared_memory *shm_ptr;

    shmid = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
    if(shmid == -1){
        perror("shmget failed");
        exit(1);
    }
    shm_ptr = (shared_memory*)shmat(shmid, NULL, 0);
    if(shm_ptr == (shared_memory*)(-1)){
        perror("shmat failed");
        exit(1);
    }

    for (size_t i = 0; i < 80; i++)
    {
        shm_ptr->values[i] = 0.0;
    }
    shm_ptr->is_written = 1; // !!!!!!!!!!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!!!!!!!!!!!!

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0){
        perror("GUI server");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Failed to bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_socket, 10) < 0){
        perror("Failed to listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("GUI server running on http://localhost:%d\n", PORT);

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if(client_socket < 0){
            perror("Failed to accept");
            continue;
        }
        handle_client(client_socket, shm_ptr);
    }
    close(server_socket);
    return 0;
}