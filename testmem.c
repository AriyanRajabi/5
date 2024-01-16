#include "types.h"
#include "stat.h"
#include "user.h"
#define MEM_ID 0

void write_to_shared_memory(char ch, int i)
{
    char *val = 0;
    open_sharedmem(MEM_ID, &val);
    acquire_sharedmem_lock(MEM_ID);
    // printf(1, "===val pointer in process %d: %p\n", i, val);
    val[i] = ch;
    printf(1, "---val string in process %d: %s\n", i, val);
    release_sharedmem_lock(MEM_ID);
    // close_sharedmem(MEM_ID);
}

int main(int argc, char *argv[])
{
    int num_child = 6;

    printf(1, "Starting shared memory test\n");
    char *val = 0;
    open_sharedmem(MEM_ID, &val);
    acquire_sharedmem_lock(MEM_ID);
    printf(1, "val pointer: %p\n", val);
    val[0] = 'a';
    printf(1, "val string: %s\n", val);
    release_sharedmem_lock(MEM_ID);
    for (int i = 0; i < num_child; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            write_to_shared_memory('a', i + 1);
            exit();
        }
    }
    sleep(200);
    printf(1, "val string final: %s\n", val);
    for (int i = 0; i < num_child; i++)
    {
        close_sharedmem(MEM_ID);
        wait();
    }

    close_sharedmem(MEM_ID);
    exit();
    /*
    int pid = fork();
    if (pid == 0)
    {
        write_to_shared_memory('b', 1);
        exit();
    }
    else if (pid > 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            write_to_shared_memory('c', 2);
            exit();
        }
        else if (pid > 0)
        {
            int pid = fork();
            if (pid == 0)
            {
                write_to_shared_memory('d', 3);
                exit();
            }
            else if (pid > 0)
            {
                wait();
                wait();
                wait();
                printf(1, "val string after: %s\n", val);
                close_sharedmem(MEM_ID);
            }
        }
    }*/
}