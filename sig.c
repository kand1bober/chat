#include "sig.h"

#define DIRECT_MSG SIGRTMIN + 1
#define ALL_MSG    SIGRTMIN + 2
#define NO_MSG 0
#define MSG 1

volatile sig_atomic_t msg_state = NO_MSG;

void direct_msg_handler()
{
    // msg_state = MSG;
}

void receive_msg()
{
    //set custom handler
    struct sigaction sa = {
        .sa_sigaction = direct_msg_handler, 
        .sa_flags = SA_SIGINFO
    };
    sigaction(DIRECT_MSG, &sa, NULL);

    //create mask
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, DIRECT_MSG);

    // block signal from mask
    sigprocmask(SIG_BLOCK, &set, NULL);

    //receive msg
    siginfo_t info;
    struct timespec time = {.tv_sec = 0, .tv_nsec = 5000};
    int error = sigtimedwait(&set, &info, &time);
    if (error < 0) {
        if (errno != EAGAIN) {
            perror("sigtimedwait");
            exit(1);
        }
        else {
            printf("no msg\n");
        }
    }
    else {
        printf("msg received\n");
    }
}

void send_direct_msg(pid_t pid, const char* msg)
{
    union sigval data;
    data.sival_ptr = NULL;

    // printf("sending msg to: %d\n", pid);

    if (sigqueue(pid, DIRECT_MSG, data) == -1) {
        perror("sigqueue");
    }
}
