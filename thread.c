#include <signal.h>

#include "thread.h"

void* receive_msgs_cycle(void* chat_info)
{
    while (1) {
        receive_msg(chat_info);
    }
}

void start_receive_msg(ChatInfo* chat_info)
{
    pthread_create(&chat_info->msg_receiver_tid, NULL, receive_msgs_cycle, chat_info);
}

void finish_receive_msgs(ChatInfo* chat_info)
{
    pthread_cancel(chat_info->msg_receiver_tid);
    pthread_join(chat_info->msg_receiver_tid, NULL);
}

//-----------------------------------------------

void start_receive_join_request(ChatInfo* chat_info)
{
    pthread_create(&chat_info->join_request_receiver, NULL, receive_join_request, chat_info);
}

void* receive_join_request(void* chat_info)
{
    while (1) {
        //create mask
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, JOIN_REQUEST);

        //receive msg
        siginfo_t info;
        struct timespec time = {.tv_sec = 0, .tv_nsec = 5000};
        int error;
        while (1) {
            error = sigtimedwait(&set, &info, &time);
            if (error < 0) {
                if (errno != EAGAIN) {
                    perror("sigtimedwait");
                    exit(1);
                }
                else {
                    continue;
                }
            }
            else {
                printf("join request received, sending approval to %d\n", info.si_value.sival_int);

                //sending approval
                union sigval data = {.sival_int = ((ChatInfo*)chat_info)->shmem_name_num};
                if (sigqueue(info.si_value.sival_int, JOIN_APPROVAL, data) == -1) {
                    perror("sigqueue");
                }

                break;
            }
        }
    }
}

void finish_receive_join_request(ChatInfo* chat_info)
{
    pthread_cancel(chat_info->join_request_receiver);
    pthread_join(chat_info->join_request_receiver, NULL);
}

//-----------------------------------------------

void start_receive_join_approval(ChatInfo* chat_info)
{
    pthread_t receiver_tid;
    
    //create mask
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, JOIN_APPROVAL);

    pthread_create(&receiver_tid, NULL, receive_join_approval, chat_info);

    pthread_join(receiver_tid, NULL);
}

void* receive_join_approval(void* chat_info)
{
    //create mask
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, JOIN_APPROVAL);

    //receive msg
    siginfo_t info;
    struct timespec time = {.tv_sec = 0, .tv_nsec = 5000};
    int error;
    while (1) {
        error = sigtimedwait(&set, &info, &time);
        if (error < 0) {
            if (errno != EAGAIN) {
                perror("sigtimedwait");
                exit(1);
            }
            else {
                continue;
            }
        }
        else {
            // printf("approval received, shmem head num: %d\n", info.si_value.sival_int);
            printf("approval received\n");
            ((ChatInfo*)chat_info)->shmem_name_num = info.si_value.sival_int;
            pthread_exit(NULL);
        }
    }
}

//-----------------------------------------------

void block_all_signals()
{
    //create mask
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, DIRECT_MSG);
    sigaddset(&set, JOIN_REQUEST);
    sigaddset(&set, JOIN_APPROVAL);

    // block signal from mask
    sigprocmask(SIG_BLOCK, &set, NULL);
}