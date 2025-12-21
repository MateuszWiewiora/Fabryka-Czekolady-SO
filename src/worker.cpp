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
        std::cerr << "[PRACOWNIK] Błąd: Brak argumentu - numeru stanowiska" << std::endl;
        return 1;
    }

    int id = std::stoi(argv[1]);

    std::cout << "[PRACOWNIK " << id << "] Na stanowisku pracy." << std::endl;

    int shmid = shmget(SHM_KEY, sizeof(Warehouse), 0600);
    if (shmid == -1)
    {
        perror("[PRACOWNIK] Blad shmget");
        return 1;
    }

    Warehouse *warehouse = (Warehouse *)shmat(shmid, NULL, 0);
    if (warehouse == (void *)-1)
    {
        perror("[PRACOWNIK] Blad shmat");
        return 1;
    }

    int semid = semget(SEM_KEY, 1, 0600);
    if (semid == -1)
    {
        perror("[PRACOWNIK] Blad semget");
        return 1;
    }

    while (warehouse->is_running)
    {
        // Czeka od 0.5s do 1.5s
        usleep((rand() % 1000000) + 500000);

        sem_p(semid);

        if (!warehouse->is_running)
        {
            sem_v(semid);
            break;
        }

        bool made_chocolate = false;

        // Logika dla Stanowiska 1: Potrzeba A + B + C
        if (id == 1)
        {
            if (warehouse->countA > 0 && warehouse->countB > 0 && warehouse->countC > 0)
            {
                warehouse->countA--;
                warehouse->countB--;
                warehouse->countC--;

                warehouse->current_capacity_used -= (1 + 1 + 2);
                made_chocolate = true;
            }
        }
        // Logika dla Stanowiska 2: Potrzeba A + B + D
        else if (id == 2)
        {
            if (warehouse->countA > 0 && warehouse->countB > 0 && warehouse->countD > 0)
            {
                warehouse->countA--;
                warehouse->countB--;
                warehouse->countD--;

                warehouse->current_capacity_used -= (1 + 1 + 3);
                made_chocolate = true;
            }
        }

        if (made_chocolate)
        {
            std::cout << "[PRACOWNIK " << id << "] Wyprodukowano czekolade! Stan mag: "
                      << warehouse->current_capacity_used << "/" << MAX_CAPACITY << std::endl;

            std::ofstream logfile(SIMULATION_LOG_FILE, std::ios::app);
            if (logfile.is_open())
            {
                logfile << "[PRACOWNIK " << id << "] Zrobiono czekolade. Stan A:" << warehouse->countA
                        << " B:" << warehouse->countB << " C:" << warehouse->countC
                        << " D:" << warehouse->countD << " (Zajete: " << warehouse->current_capacity_used << ")\n";
                logfile.close();
            }
        }
        // Jeśli brakuje składników, pracownik nic nie robi, po prostu zwalnia semafor

        sem_v(semid);
    }

    std::cout << "[PRACOWNIK " << id << "] Koniec zmiany." << std::endl;
    shmdt(warehouse);
    return 0;
}