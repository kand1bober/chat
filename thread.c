#include <signal.h>

#include "thread.h"

void* receive_msgs_cycle(void* chat_info)
{
    while (1) {
        receive_msg(chat_info);
    }
}

void start_receive_msg(ChatInfo* chat_info, pthread_t* receiver_tid)
{
    //create mask
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, DIRECT_MSG);

    // block signal from mask
    sigprocmask(SIG_BLOCK, &set, NULL);

    pthread_create(receiver_tid, NULL, receive_msgs_cycle, chat_info);
}

void finish_receive_msgs(pthread_t receiver_tid)
{
    pthread_cancel(receiver_tid);
}

