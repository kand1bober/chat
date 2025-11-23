#include "sig.h"

void receive_msg(ChatInfo* chat_info)
{
    //create mask
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, DIRECT_MSG);

    //receive msg
    siginfo_t info;
    struct timespec time = {.tv_sec = 1, .tv_nsec = 0};
    int error = sigtimedwait(&set, &info, &time);
    if (error < 0) {
        if (errno != EAGAIN) {
            perror("sigtimedwait");
            exit(1);
        }
        else {
            // printf(".");
            // fflush(stdout);
        }
    }
    else {
        printf("msg received: '%s'\n", get_msg_from_text_db(chat_info->text_db, info.si_value.sival_int));
        fflush(stdout);
    }
}

void send_direct_msg(ChatInfo* chat_info, int pid, const char* msg)
{
    union sigval data;
    data.sival_int = add_msg_to_text_db(chat_info->text_db, msg); //send number of msg in text_db
    // printf("sending msg number: %d\n", data.sival_int);

    if (sigqueue(pid, DIRECT_MSG, data) == -1) {
        perror("sigqueue");
    }
}

int add_msg_to_text_db(void* text_db, const char* msg)
{
    int num;
    memcpy(&num, text_db, sizeof(int));

    char* pos = ((char*)text_db + sizeof(int));
    int count = 0;
    while (count < num) {
        if (*pos == 0) {
            count++;
        }
        pos++;
    }
    memcpy(pos, msg, strlen(msg) + 1); //add new str

    count++; 
    memcpy(text_db, &count, sizeof(int)); //increase str counter
    return count; 
}

char* get_msg_from_text_db(void* text_db, int str_num)
{
    int num;
    memcpy(&num, text_db, sizeof(int));
    if (str_num > num) {
        printf("invalid nuber of string to take from text data base\n");
        exit(1);
    }

    char* pos = ((char*)text_db + sizeof(int));
    int count = 0;
    while (count < (str_num - 1)) {
        if (*pos == 0) { 
            count++;
        }
        pos++;
    }
    
    return pos;
}
