#include <iostream>
#include "common.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "[DOSTAWCA] Błąd: Brak argumentu!" << std::endl;
        return 1;
    }

    char type = argv[1][0];
    std::cout << "[DOSTAWCA " << type << "] Rozpoczynam dostawy." << std::endl;

    return 0;
}