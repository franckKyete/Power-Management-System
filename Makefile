CC = gcc
CFLAGS = -Wall -g

# Output binaries in the build directory
BUILD_DIR = build
SERVER = $(BUILD_DIR)/server
CLI = $(BUILD_DIR)/cli
GUI = $(BUILD_DIR)/gui
CO2_SENSOR = $(BUILD_DIR)/co2_sensor
PRESENCE_SENSOR = $(BUILD_DIR)/presence_sensor
TEMP_SENSOR = $(BUILD_DIR)/temperature_sensor

# Directories
SRC_DIR = .
GUI_DIR = GUI
CLI_DIR = CLI
SENSORS_DIR = sensors
SERVER_DIR = server
SYSTEMS_DIR = systems

# Source files
SERVER_SRC = $(SERVER_DIR)/server.c $(SERVER_DIR)/power_meter.c $(SERVER_DIR)/cli_socket.c $(SERVER_DIR)/gui_socket.c $(SRC_DIR)/building.c $(SYSTEMS_DIR)/chauffage.c $(SYSTEMS_DIR)/eclairage.c $(SYSTEMS_DIR)/energy_source.c $(SYSTEMS_DIR)/ventilation.c

CO2_SENSOR_SRC = $(SENSORS_DIR)/co2_sensor.c
TEMP_SENSOR_SRC = $(SENSORS_DIR)/temperature_sensor.c
PRESENCE_SENSOR_SRC = $(SENSORS_DIR)/presence_sensor.c
CLI_SRC = $(CLI_DIR)/cli_socket.c $(CLI_DIR)/cli.c
GUI_SRC = $(GUI_DIR)/gui_server.c
# SYSTEMS_SRC = $(SYSTEMS_DIR)/chauffage.c $(SYSTEMS_DIR)/eclairage.c $(SYSTEMS_DIR)/energy_source.c $(SYSTEMS_DIR)/ventilation.c

# Object files
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CO2_SENSOR_OBJ = $(CO2_SENSOR_SRC:.c=.o)
TEMP_SENSOR_OBJ = $(TEMP_SENSOR_SRC:.c=.o)
PRESENCE_SENSOR_OBJ = $(PRESENCE_SENSOR_SRC:.c=.o)
CLI_OBJ = $(CLI_SRC:.c=.o)
GUI_OBJ = $(GUI_SRC:.c=.o)
# SYSTEMS_OBJ = $(SYSTEMS_SRC:.c=.o)

# Targets
all: $(SERVER) $(CLI) $(CO2_SENSOR) $(PRESENCE_SENSOR) $(TEMP_SENSOR) $(GUI)

$(SERVER): $(SERVER_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -o $(SERVER)

$(CLI): $(CLI_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CLI_OBJ) -o $(CLI)

$(GUI): $(GUI_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(GUI_OBJ) -o $(GUI)

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
	rm -f $(SERVER_OBJ) $(CLI_OBJ) $(CO2_SENSOR_OBJ) $(PRESENCE_SENSOR_OBJ) $(TEMP_SENSOR_OBJ) $(GUI_OBJ)
	rm -rf $(BUILD_DIR)

.PHONY: all clean
