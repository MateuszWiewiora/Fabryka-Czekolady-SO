#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>

int create_shm(key_t key, size_t size); // tworzenie shared_mem i zwracanie jego ID

void remove_shm(int shmid);

int create_sem(key_t key, int value); // tworzenie semaforow i ust ich wartosci poczatkowej

void remove_sem(int semid);

#endif