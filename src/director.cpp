#include <iostream>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fstream>
#include "common.h"
#include "utils.h"

// Funkcja pomocnicza do zapisu stanu (zrzut pamięci do pliku)
void save_state(Warehouse *w)
{
    std::ofstream file(STATE_FILE, std::ios::out | std::ios::trunc);

    if (file.is_open())
    {
        file << w->countA << " "
             << w->countB << " "
             << w->countC << " "
             << w->countD << " "
             << w->current_capacity_used << std::endl;

        std::cout << "[DYREKTOR] Stan magazynu zapisany do pliku: " << STATE_FILE << std::endl;
        file.close();
    }
    else
    {
        std::cerr << "[DYREKTOR] Blad otwarcia pliku do zapisu stanu!" << std::endl;
    }
}

int main()
{
    std::cout << "[DYREKTOR] Rozpoczynam prace." << std::endl;

    int shmid = shmget(SHM_KEY, sizeof(Warehouse), 0600);
    if (shmid == -1)
    {
        perror("[DYREKTOR] Blad shmget");
        return 1;
    }

    Warehouse *warehouse = (Warehouse *)shmat(shmid, NULL, 0);
    if (warehouse == (void *)-1)
    {
        perror("[DYREKTOR] Blad shmat");
        return 1;
    }

    int semid = semget(SEM_KEY, 1, 0600);
    if (semid == -1)
    {
        perror("[DYREKTOR] Blad semget");
        return 1;
    }

    // TODO:
    // Komendy dyrektora

    std::cout << "[DYREKTOR] Koniec pracy." << std::endl;
    shmdt(warehouse);
    return 0;
}