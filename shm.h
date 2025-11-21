#define _POSIX_C_SOURCE 200809L

#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "chat.h"

#ifndef SHMEM_HEADER
#define SHMEM_EDER

void make_shmem(const char* name, int size, int* ret_fd, void** ret_ptr);
void join_chat(ChatInfo* chat_info, int head_pid);

#endif
