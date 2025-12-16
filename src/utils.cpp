#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib> // exit
#include <cstdio>  // perror
#include "utils.h"

int create_shm(key_t key, size_t size)
{
    int shmid = shmget(key, size, 0600 | IPC_CREAT);

    if (shmid == -1)
    {
        perror("Blad tworzenia pamieci dzielonej");
        exit(1);
    }
    return shmid;
}

void remove_shm(int shmid)
{
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("Blad usuwania pamieci dzielonej");
    }
    else
    {
        std::cout << "Pamiec dzielona usunieta." << std::endl;
    }
}