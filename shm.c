#include "shm.h"

const char* pid_db_name = "/my_chat_pid_db";
const char* text_db_name = "/my_chat_text_db";

void make_shmem(const char* name, int* ret_fd, void** ret_ptr)
{
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, 4096);
    void *ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    int init = 0;
    memcpy(ptr, &init, sizeof(int));
tell 30869 am
    *ret_fd = fd;
    *ret_ptr = ptr;
}   

void join_shmem(const char* name, int* ret_fd, void** ret_ptr)
{       
    int fd = shm_open(name, O_RDWR, 0666); //open already existing shmem
    *ret_fd = fd;

    void* ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    *ret_ptr = ptr;
}

int add_pid_to_db(void* pid_db, int pid)
{
    int num;
    memcpy(&num, pid_db, sizeof(int)); //get pid counter

    int count = 0;
    memcpy((char*)pid_db + count * sizeof(int), &pid, sizeof(int)); //add new pid
 
    num++;
    memcpy(pid_db, &num, sizeof(int)); //increase pid counter
    return count; 
}

int get_pid_from_db(void* pid_db, int pid_num)
{
    int num;
    memcpy(&num, pid_db, sizeof(int));
    if (pid_num > num) {
        printf("invalid number of pid to take from pid data base\n");
        exit(1);
    }

    int res;
    memcpy(&res, (char*)pid_db + (pid_num + 1) * sizeof(int), sizeof(int));
    return res; 
}

void show_all_pids(ChatInfo* chat_info)
{
    int db_size;
    memcpy(&db_size, chat_info->pid_db, sizeof(int));

    printf("All pids:\n");
    for (int i = 0; i < db_size; i++) {
        printf("%2d: %d\n", i, get_pid_from_db(chat_info->pid_db, i));
    }
}
