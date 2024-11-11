// sender.c
#include "msg_queue.h"

int main() {
    int msgid;
    struct msg_buffer message;

    // Create or access the message queue
    msgid = msgget(CAPTEUR_CO2, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    // Prepare the message
    message.msg_type = 1; // Set message type
    strcpy(message.msg_text, "Hello from the sender!");

    // Send the message
    if (msgsnd(msgid, &message, sizeof(message.msg_text), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }

    printf("Message sent: %s\n", message.msg_text);
    return 0;
}
