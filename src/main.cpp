#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include "common.h"

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

    // TODO: IPC (Pamięć, Semafory)
    // init_ipc();

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

    // TODO:
    // cleanup_ipc();

    return 0;
}