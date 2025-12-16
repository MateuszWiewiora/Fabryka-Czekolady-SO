#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <vector>
#include "common.h"
#include "utils.h"

void run_process(const char *path, const char *name, const char *arg = nullptr)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("Błąd fork");
        exit(1);
    }
    else if (pid == 0)
    {
        execl(path, path, arg, NULL);
        perror("Błąd execl");
        exit(1);
    }
    std::cout << "[MAIN] Uruchomiono proces: " << name << " (PID: " << pid << ")" << std::endl;
}

int main()
{
    std::cout << "[MAIN] --- Start Symulacji Fabryki Czekolady ---" << std::endl;

    // Tworzenie Pamięci Dzielonej
    int shmid = create_shm(SHM_KEY, sizeof(Warehouse));
    std::cout << "[MAIN] Pamiec dzielona utworzona. ID: " << shmid << std::endl;

    // Podpięcie się pod pamięć, żeby wyzerować Magazyn na start
    // shmat (Shared Memory Attach) zwraca wskaźnik void*, rzutujemy go na Warehouse*
    Warehouse *warehouse = (Warehouse *)shmat(shmid, NULL, 0);

    if (warehouse == (void *)-1)
    {
        perror("[MAIN] Blad shmat (przylaczanie)");
        exit(1);
    }

    warehouse->countA = 0;
    warehouse->countB = 0;
    warehouse->countC = 0;
    warehouse->countD = 0;
    warehouse->current_capacity_used = 0;
    warehouse->is_running = true;
    std::cout << "[MAIN] Magazyn wyzerowany i gotowy." << std::endl;

    // Odpięcie się od pamięci (Main już jej nie potrzebuje do pracy)
    shmdt(warehouse);

    int semid = create_sem(SEM_KEY, 1);
    std::cout << "[MAIN] Semafor utworzony. ID: " << semid << std::endl;

    run_process("./director", "Dyrektor");

    run_process("./worker", "Pracownik 1", "1");
    run_process("./worker", "Pracownik 2", "2");

    run_process("./supplier", "Dostawca A", "A");
    run_process("./supplier", "Dostawca B", "B");
    run_process("./supplier", "Dostawca C", "C");
    run_process("./supplier", "Dostawca D", "D");

    while (wait(NULL) > 0)
        ;

    std::cout << "[MAIN] Wszystkie procesy zakończone. Koniec symulacji." << std::endl;

    remove_shm(shmid);
    remove_sem(semid);

    return 0;
}