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

        std::cout << "[DYREKTOR] Stan magazynu zapisany do pliku.\n";
        file.close();
    }
    else
    {
        std::cerr << "[DYREKTOR] Blad otwarcia pliku do zapisu stanu!" << std::endl;
    }
}

void print_menu()
{
    std::cout << "\n--- CENTRUM STEROWANIA FABRYKA ---\n";
    std::cout << " 1 - Polecenie_1: Zatrzymaj PRODUKCJE (Pracownicy STOP)\n";
    std::cout << " 2 - Polecenie_2: Zamknij MAGAZYN (Nikt nie wchodzi)\n";
    std::cout << " 3 - Polecenie_3: Zatrzymaj DOSTAWY (Dostawcy STOP)\n";
    std::cout << " 4 - Polecenie_4: KONIEC CALKOWITY (Zapisz i wyjdz)\n";
    std::cout << " s - Status magazynu\n";
    std::cout << "----------------------------------\n";
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

    sem_p(semid);
    warehouse->command = CMD_NONE;
    sem_v(semid);

    char choice;
    bool working = true;

    while (working)
    {
        print_menu();
        std::cin >> choice;

        sem_p(semid); // Blokujemy dostęp na czas zmiany rozkazów

        switch (choice)
        {
        case '1':
            warehouse->command = CMD_FACTORY_STOP;
            std::cout << "[DYREKTOR] Wyslano CMD_FACTORY_STOP. Pracownicy koncza zmiany.\n";
            break;
        case '2':
            warehouse->command = CMD_WAREHOUSE_STOP;
            std::cout << "[DYREKTOR] Wyslano CMD_WAREHOUSE_STOP. Magazyn zablokowany.\n";
            break;
        case '3':
            warehouse->command = CMD_SUPPLIER_STOP;
            std::cout << "[DYREKTOR] Wyslano CMD_SUPPLIER_STOP. Dostawcy wracaja do baz.\n";
            break;
        case '4':
            warehouse->command = CMD_FACTWARE_STOP;
            save_state(warehouse);
            std::cout << "[DYREKTOR] Wyslano CMD_FACTWARE_STOP. Koniec symulacji.\n";
            working = false; // Dyrektor też kończy
            break;
        case 's':
            std::cout << "STAN: A=" << warehouse->countA
                      << " B=" << warehouse->countB
                      << " C=" << warehouse->countC
                      << " D=" << warehouse->countD
                      << " [Zapelnienie: " << warehouse->current_capacity_used << "/" << MAX_CAPACITY << "]\n";
            break;
        default:
            std::cout << "Nieznana komenda.\n";
        }

        sem_v(semid);
    }

    std::cout << "[DYREKTOR] Koniec pracy." << std::endl;
    shmdt(warehouse);
    return 0;
}