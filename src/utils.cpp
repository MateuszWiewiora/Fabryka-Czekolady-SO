#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
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

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int create_sem(key_t key, int value)
{
    int semid = semget(key, 1, 0600 | IPC_CREAT);

    if (semid == -1)
    {
        perror("Blad tworzenia semafora");
        exit(1);
    }

    union semun arg;
    arg.val = value;

    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("Blad ustawiania wartosci semafora");
        exit(1);
    }

    return semid;
}

void remove_sem(int semid)
{
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("Blad usuwania semafora");
    }
    else
    {
        std::cout << "Semafor usuniety." << std::endl;
    }
}