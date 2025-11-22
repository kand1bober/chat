
#ifndef CHAT_HEADER
#define CHAT_HEADER

typedef struct {
    char* shmem_head_num; //pid of creator of shmem, used to create id for shmem: id = "chat_shmem_<head_pid>"

    void* pid_db; //ptr to shared memory with all pids 
                  //zero elem is number of pids
    int pid_fd;

    void* text_db; //ptr to shared memory with text 
                   //first 4 bytes is integer number of all records 
    int text_fd;   //file descriptor of shared memory with text

    pid_t self_pid; 
    pid_t head_pid; //pid of process which current process was initially connected to
} ChatInfo;

#endif
