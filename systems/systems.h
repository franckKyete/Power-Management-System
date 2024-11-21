#ifndef VENTILATION_H
#define VENTILATION_H

#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#include "../building.h"

void *ventilation(void *_building);
void *light(void *_building);
void *ac(void *_building);
void *power_source(void *_building);

#endif