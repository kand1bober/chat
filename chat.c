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
#include "shm.h"

void user(ChatInfo* chat_info);
void clean_stdin();

int main(int argc, char* argv[])
{
    char input[128];
    int to_join_pid;
    
    extern const char* pid_db_name;
    extern const char* text_db_name;
    ChatInfo chat_info ={.self_pid = getpid()};
    
    scanf("%s%d", input, &to_join_pid);
    if (!strcmp(input, "chat")) {
        if (to_join_pid == -1) {
            chat_info.head_pid = 0;
            make_shmem(pid_db_name, 4096, &chat_info.text_fd, &chat_info.text_db);
            make_shmem(text_db_name, 4096, &chat_info.pid_fd, &chat_info.pid_db);
            user(&chat_info);
        }
        else if (to_join_pid >= 0) {
            join_chat(&chat_info, to_join_pid);
            user(&chat_info);
        }
        else {
            printf("error in input\n");
            exit(0);
        }
    }
    else {
        printf("error in input\n");
        exit(0);
    }

    return 0;
}


void user(ChatInfo* chat_info)
{
    printf("I am new user, my pid: %d\n", getpid());
    char input[128];

    char msg[128];
    pid_t msg_receiver; 

    while (1) { //cycle of reading stdin
        receive_msg();

        scanf("%s", input);
        if (!strcmp(input, "tell")) {
            scanf("%d", &msg_receiver);            
            scanf("%s", msg);

            send_direct_msg(&chat_info, msg_receiver, msg);  
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
