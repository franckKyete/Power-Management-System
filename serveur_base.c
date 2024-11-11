// receiver.c
#include "msg_queue.h"

int main() {
    int msgid;
    struct msg_buffer message;

    // Access the message queue
    msgid = msgget(CAPTEUR_CO2, 0666);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Receive the message
    if (msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0) == -1) {
        perror("msgrcv failed");
        exit(1);
    }

    printf("Message received: %s\n", message.msg_text);

    // Optional: Delete the message queue after receiving
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(1);
    }

    return 0;
}
