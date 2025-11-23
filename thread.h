#include <pthread.h>

#include "sig.h"

#ifndef THREAD_HEADER
#define THREAD_HEADER

void* receive_msgs_cycle(void* chat_info);

void start_receive_msg(ChatInfo* chat_info, pthread_t* receiver_tid);

void finish_receive_msgs(pthread_t receiver_tid);

//-----------------------------------------------

void start_receive_join_approval(ChatInfo* chat_info);

void* receive_join_approval(void* chat_info);

//-----------------------------------------------

void start_receive_join_request(ChatInfo* chat_info);

void finish_receive_join_request(ChatInfo* chat_info);

void* receive_join_request(void* chat_info);

#endif
