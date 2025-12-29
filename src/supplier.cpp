#include <iostream>
#include <unistd.h> // sleep, usleep
#include <sys/shm.h>
#include <sys/sem.h>
#include <ctime>
#include <cstdlib> // rand, srand
#include <fstream>
#include "common.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "[DOSTAWCA] Blad: Brak typu dostawcy!" << std::endl;
        return 1;
    }
    char type = argv[1][0]; // A, B, C lub D

    // rozmiary towaru:
    int size = 1;
    if (type == 'C')
        size = 2;
    if (type == 'D')
        size = 3;

    srand(time(NULL) ^ getpid());

    std::cout << "[DOSTAWCA " << type << "] Rozpoczynam prace. Towar zajmuje: " << size << std::endl;

    // podlaczenie ipc
    int shmid = shmget(SHM_KEY, sizeof(Warehouse), 0600);
    if (shmid == -1)
    {
        perror("[DOSTAWCA] Blad shmget");
        return 1;
    }

    Warehouse *warehouse = (Warehouse *)shmat(shmid, NULL, 0);
    if (warehouse == (void *)-1)
    {
        perror("[DOSTAWCA] Blad shmat");
        return 1;
    }

    int semid = semget(SEM_KEY, 1, 0600);
    if (semid == -1)
    {
        perror("[DOSTAWCA] Blad semget");
        return 1;
    }

    while (true)
    {
        // Symulacja czasu dostawy (losowo od 100ms do 1s)
        usleep((rand() % 900000) + 100000);

        sem_p(semid);

        if (warehouse->command == CMD_SUPPLIER_STOP ||
            warehouse->command == CMD_FACTWARE_STOP ||
            warehouse->command == CMD_WAREHOUSE_STOP)
        {
            sem_v(semid);
            break;
        }

        if (warehouse->current_capacity_used + size <= MAX_CAPACITY)
        { // spr czy jest miejsce w magazynie

            if (type == 'A')
                warehouse->countA++;
            else if (type == 'B')
                warehouse->countB++;
            else if (type == 'C')
                warehouse->countC++;
            else if (type == 'D')
                warehouse->countD++;

            warehouse->current_capacity_used += size;

            std::cout << "[DOSTAWCA " << type << "] Dostarczono. Stan mag: "
                      << warehouse->current_capacity_used << "/" << MAX_CAPACITY << std::endl;

            std::ofstream logfile(SIMULATION_LOG_FILE, std::ios::app);
            if (logfile.is_open())
            {
                logfile << "[DOSTAWCA " << type << "] Dodano towar. Stan A:" << warehouse->countA
                        << " B:" << warehouse->countB << " C:" << warehouse->countC
                        << " D:" << warehouse->countD << " (Zajete: " << warehouse->current_capacity_used << ")\n";
                logfile.close();
            }
        }
        else
        {
            std::cout << "[DOSTAWCA " << type << "] Magazyn PELNY! Czekam..." << std::endl;
        }

        sem_v(semid);
    }

    // odpiecie sie od ipc
    std::cout << "[DOSTAWCA " << type << "] Koniec pracy." << std::endl;
    shmdt(warehouse);

    return 0;
}