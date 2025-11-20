#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <spawn.h>
#include <stdbool.h>

#include "sig.h"

void user();
void clean_stdin();

int main(int argc, char* argv[])
{
    char input[128];
    
    scanf("%s", input);
    
    while (1) { //cycle of reading stdin

        if (!strcmp(input, "chat")) {
            user();
            break;
        }
        else {
            printf("error in input\n");
            exit(0);
        }

        clean_stdin();
        scanf("%s", input);
    }

    return 0;
}


void user()
{
    printf("I am new user, my pid: %d\n", getpid());
    char input[128];

    char msg[128];
    pid_t msg_receiver; 

    while (1) { //cycle of reading stdin
        receive_msg();

        scanf("%s", input);
        if (!strcmp(input, "tell")) {
            scanf("%s", input);
            msg_receiver = atoi(input);
            
            scanf("%s", input);
            strcpy(msg, input);

            send_direct_msg(msg_receiver, msg);  
        }
        else if (!strcmp(input, "say")) {
            //всем
        }
        else if (!strcmp(input, "bye")) {
            return;
        }
        else {
            printf("error in input\n");
            exit(0);
        }
    }
}

void clean_stdin()
{
    int c;
    do {
        c = getchar();
        // printf("%d\n", c);
    }
    while (c != '\n');
}
