#include <iostream>
#include <string>
#include "common.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "[PRACOWNIK] Błąd: Brak argumentu - numeru stanowiska" << std::endl;
        return 1;
    }

    int id = std::stoi(argv[1]);
    std::cout << "[PRACOWNIK " << id << "] Na stanowisku pracy." << std::endl;
    return 0;
}