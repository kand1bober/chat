#include <pthread.h>

#ifndef CHAT_HEADER
#define CHAT_HEADER

typedef struct {
    int shmem_name_num; //pid of creator of shmem, used to create id for shmem: id = "chat_shmem_<head_pid>"

    void* pid_db; //ptr to shared memory with all pids 
                  //zero elem is number of pids
    int pid_fd;

    void* text_db; //ptr to shared memory with text 
                   //first 4 bytes is integer number of all records 
    int text_fd;   //file descriptor of shared memory with text

    pid_t self_pid; 
    pid_t head_pid; //pid of process which current process was initially connected to

    pthread_t join_request_receiver; //thread, that receives join requests, works till the 'bye' function
} ChatInfo;

void user(ChatInfo* chat_info);

void tell(ChatInfo* chat_info);

void say(ChatInfo* chat_info);

void quit_chat(ChatInfo* chat_info);

void make_shmem_name(ChatInfo* chat_info, char* res, const char* base);

void join_chat(ChatInfo* chat_info);

void start_joining_others(ChatInfo* chat_info);

void finish_joining_others(ChatInfo* chat_info);

void block_all_signals();

#endif
