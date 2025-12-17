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

void sem_p(int semid)
{
    struct sembuf buf;
    buf.sem_num = 0; // Indeks semafora
    buf.sem_op = -1; // Operacja: Zmniejsz o 1
    buf.sem_flg = 0;

    if (semop(semid, &buf, 1) == -1)
    {
        perror("Blad zamykania semafora");
        exit(1);
    }
}

void sem_v(int semid)
{
    struct sembuf buf;
    buf.sem_num = 0;
    buf.sem_op = 1; // Zwiększ o 1
    buf.sem_flg = 0;

    if (semop(semid, &buf, 1) == -1)
    {
        perror("Blad otwierania semafora");
        exit(1);
    }
}

void clean_up(int shmid, int semid)
{
    std::cout << "[UTILS] --- Rozpoczynam sprzatanie zasobow ---" << std::endl;

    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("[UTILS] Blad usuwania pamieci");
    }
    else
    {
        std::cout << "[UTILS] Pamiec dzielona usunieta." << std::endl;
    }

    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("[UTILS] Blad usuwania semafora");
    }
    else
    {
        std::cout << "[UTILS] Semafor usuniety." << std::endl;
    }

    std::cout << "[UTILS] Sprzatanie zakonczone." << std::endl;
}