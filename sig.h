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
#include <time.h>
#include <errno.h>

#include "chat.h"

#ifndef SIG_HEADER
#define SIG_HEADER

void send_direct_msg(ChatInfo* chat_info, int pid, const char* msg);

void receive_msg(ChatInfo* chat_info);

int add_msg_to_text_db(void* text_db, const char* msg);

char* get_msg_from_text_db(void* text_db, int str_num); 

#endif
