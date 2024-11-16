// receiver.c
#include "msg_queue.h"

int main() {
    int msgid;
    sensor_msg message;

    // Access the message queue
    msgid = msgget(CAPTEUR_TEMPERATURE, 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Receive the message
    if (msgrcv(msgid, &message, sizeof(message.value), 0, 0) == -1) {
        perror("msgrcv failed");
        exit(1);
    }

    printf("Message received: %f\n", message.value);

    // Optional: Delete the message queue after receiving
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(1);
    }

    return 0;
}
