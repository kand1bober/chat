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

#ifndef SIG_HEADER
#define SIG_HEADER

void send_direct_msg(pid_t pid, const char* msg);
void receive_msg();

#endif
