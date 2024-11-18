CC = gcc
CFLAGS = -Wall

BUILDDIR = build


# server
SERVER = $(BUILDDIR)/server

# sensors
CO2_SENSOR = $(BUILDDIR)/co2_sensor
PRESENCE_SENSOR = $(BUILDDIR)/presence_sensor
TEMPERATURE_SENSOR = $(BUILDDIR)/temperature_sensor

# cli
CLI = $(BUILDDIR)/cli

all: $(SERVER) $(CO2_SENSOR) $(PRESENCE_SENSOR) $(TEMPERATURE_SENSOR) $(CLI)