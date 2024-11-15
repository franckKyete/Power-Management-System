#include "../msg_queue.h"

int main(){

    int msgids[4];

    for(int i=0; i<4; i++){
        // Access the message queue
        msgids[i] = msgget(i, 0666 | IPC_CREAT);
        if (msgids[i] == -1) {
            perror("msgget failed");
            exit(1);
        }
    }

    for(int i=0; i<4; i++){
        if (msgctl(msgids[i], IPC_RMID, NULL) == -1) {
            perror("msgctl failed");
            exit(1);
        }       
    }

    printf("Done!\n");
    return 0;
}