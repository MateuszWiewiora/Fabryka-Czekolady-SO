#include <iostream>
#include "common.h"
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "[PRACOWNIK] Błąd: Brak argumentu - numeru stanowiska"
    }

    int id = std::stoi(argv[1]);
    std::cout << "[PRACOWNIK " << id << "] Na stanowisku pracy." << std::endl;
    return 0;
}