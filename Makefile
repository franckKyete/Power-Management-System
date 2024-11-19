CC = gcc
CFLAGS = -Wall -g

# Output binaries in the build directory
BUILD_DIR = build
SERVER = $(BUILD_DIR)/server
CLI = $(BUILD_DIR)/cli
CO2_SENSOR = $(BUILD_DIR)/co2_sensor
PRESENCE_SENSOR = $(BUILD_DIR)/presence_sensor
TEMP_SENSOR = $(BUILD_DIR)/temperature_sensor

# Directories
SRC_DIR = .
CLI_DIR = CLI
SENSORS_DIR = sensors
SERVER_DIR = server

# Source files
SERVER_SRC = $(SERVER_DIR)/server.c $(SERVER_DIR)/power_meter.c $(SERVER_DIR)/cli_socket.c $(SRC_DIR)/building.c
CO2_SENSOR_SRC = $(SENSORS_DIR)/co2_sensor.c
TEMP_SENSOR_SRC = $(SENSORS_DIR)/temperature_sensor.c
PRESENCE_SENSOR_SRC = $(SENSORS_DIR)/presence_sensor.c
CLI_SRC = $(CLI_DIR)/cli_socket.c $(CLI_DIR)/cli.c

# Object files
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CO2_SENSOR_OBJ = $(CO2_SENSOR_SRC:.c=.o)
TEMP_SENSOR_OBJ = $(TEMP_SENSOR_SRC:.c=.o)
PRESENCE_SENSOR_OBJ = $(PRESENCE_SENSOR_SRC:.c=.o)
CLI_OBJ = $(CLI_SRC:.c=.o)

# Targets
all: $(SERVER) $(CLI) $(CO2_SENSOR) $(PRESENCE_SENSOR) $(TEMP_SENSOR)

$(SERVER): $(SERVER_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -o $(SERVER)

$(CLI): $(CLI_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CLI_OBJ) -o $(CLI)

$(CO2_SENSOR): $(CO2_SENSOR_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CO2_SENSOR_OBJ) -o $(CO2_SENSOR)

$(PRESENCE_SENSOR): $(PRESENCE_SENSOR_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(PRESENCE_SENSOR_OBJ) -o $(PRESENCE_SENSOR)

$(TEMP_SENSOR): $(TEMP_SENSOR_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TEMP_SENSOR_OBJ) -o $(TEMP_SENSOR)

# Clean object files and executables
clean:
	rm -f $(SERVER_OBJ) $(CLI_OBJ) $(CO2_SENSOR_OBJ) $(PRESENCE_SENSOR_OBJ) $(TEMP_SENSOR_OBJ)
	rm -rf $(BUILD_DIR)

.PHONY: all clean
