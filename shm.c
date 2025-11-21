#include "shm.h"

const char* pid_db_name = "/my_chat_pid_db";
const char* text_db_name = "/my_chat_text_db";

void make_shmem(const char* name, int size, int* ret_fd, void** ret_ptr)
{
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, size);
    void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    *ret_fd = fd;
    *ret_ptr = ptr;
}   

void join_chat(ChatInfo* chat_info, int head_pid)
{   
    chat_info->head_pid = head_pid;
    
    chat_info->text_fd = shm_open(text_db_name, O_RDWR, 0666); //open already existing shmem
    chat_info->text_db = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, chat_info->text_fd, 0);

    chat_info->pid_fd = shm_open(pid_db_name, O_RDWR, 0666); //open already existing shmem
    chat_info->pid_db = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, chat_info->pid_fd, 0);
}
