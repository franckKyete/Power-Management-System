#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "server.h"

#define CLI_PORT 8080
#define GUI_PORT 8085

void *cli_socket(void *_building);
void *gui_socket(void *_building);

#endif