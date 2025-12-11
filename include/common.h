#ifndef COMMON_H
#define COMMON_H

#define MAX_CAPACITY 100
#define SHM_KEY 062004
#define SEM_KEY 180604
#define MSG_KEY 182004

#define SIMULATION_LOG_FILE "simulation_log.txt"
#define STATE_FILE "warehouse_state.dat"

struct Warehouse
{
    int countA;
    int countB;
    int countC;
    int countD;
    int current_capacity_used;
    bool is_running;
};

enum DirectorCommand
{
    CMD_NONE = 0,
    CMD_FACTORY_STOP = 1,
    CMD_WAREHOUSE_STOP = 2,
    CMD_SUPPLIER_STOP = 3,
    CMD_FACTWARE_STOP = 4
};

#endif