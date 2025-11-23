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

#include "chat.h"
#include "thread.h"
#include "shm.h"

const char* kTextShmemNameBase = "my_chat_text_shmem_";
const char* kPidShmemNameBase =  "my_chat_pid_shmem_";

int main(int argc, char* argv[])
{
    char input[128];
    int to_join_pid;
    
    ChatInfo chat_info ={.self_pid = getpid()};

    pthread_t join_request_receiver, join_approval_receiver;
    
    block_all_signals();

    scanf("%s%d", input, &to_join_pid);
    if (!strcmp(input, "chat")) {
        if (to_join_pid == -1 || to_join_pid >= 0) {
            make_shmem_name(&chat_info, chat_info.text_db_name, kTextShmemNameBase);
            make_shmem_name(&chat_info, chat_info.pid_db_name, kPidShmemNameBase);

            if (to_join_pid == -1) {
                chat_info.head_pid = chat_info.self_pid;
                chat_info.shmem_name_num = chat_info.self_pid;

                make_shmem(chat_info.text_db_name, &chat_info.text_fd, &chat_info.text_db);
                make_shmem(chat_info.pid_db_name, &chat_info.pid_fd, &chat_info.pid_db);
                
                add_pid_to_db(chat_info.pid_db, chat_info.self_pid);

                start_joining_others(&chat_info);            
                user(&chat_info);
            }
            else if (to_join_pid >= 0) {
                chat_info.head_pid = to_join_pid;       
                join_chat(&chat_info);

                join_shmem(chat_info.text_db_name, &chat_info.text_fd, &chat_info.text_db);
                join_shmem(chat_info.pid_db_name, &chat_info.pid_fd, &chat_info.pid_db);

                add_pid_to_db(chat_info.pid_db, chat_info.self_pid);

                start_joining_others(&chat_info);
                user(&chat_info);
            }
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

    start_receive_msg(chat_info);

    while (1) { //cycle of reading stdin

        do {
            errno = 0;
            scanf("%s", input);
        }
        while (errno == EINTR);

        if (!strcmp(input, "tell")) {
            tell(chat_info);
        }
        else if (!strcmp(input, "say")) {
            say(chat_info);
        }
        else if (!strcmp(input, "bye")) {
            quit_chat(chat_info);
            return;
        }
        else if (!strcmp(input, "list"))
        {
            show_all_pids(chat_info);
        }
        else {
            printf("input: '%s'\n", input);
            printf("error in input\n");

            quit_chat(chat_info);
            exit(0);
        }
    }
}


void tell(ChatInfo* chat_info)
{
    char msg[128];
    pid_t msg_receiver; 

    scanf("%d", &msg_receiver);            
    scanf("%s", msg);

    send_direct_msg(chat_info, msg_receiver, msg);  
}

void say(ChatInfo* chat_info)
{
    char msg[128];
    scanf("%s", msg);

    int pid_db_size;
    memcpy(&pid_db_size, chat_info->pid_db, sizeof(int));

    int pid;
    for (int i = 0; i < pid_db_size; i++) {
        pid = get_pid_from_db(chat_info->pid_db, i);
        if (pid != chat_info->self_pid) {
            send_direct_msg(chat_info, pid, msg);
        }
    }
}

void make_shmem_name(ChatInfo* chat_info, char* res, const char* base)
{
    sprintf(res, "%s%d", base, chat_info->shmem_name_num);
}

void join_chat(ChatInfo* chat_info)
{
    //sending join request
    union sigval data = {.sival_int = chat_info->self_pid};
    if (sigqueue(chat_info->head_pid, JOIN_REQUEST, data) == -1) {
        perror("sigqueue");
    }

    start_receive_join_approval(chat_info);
}

void start_joining_others(ChatInfo* chat_info)
{
    start_receive_join_request(chat_info);
}

void finish_joining_others(ChatInfo* chat_info)
{
    finish_receive_join_request(chat_info);
}
