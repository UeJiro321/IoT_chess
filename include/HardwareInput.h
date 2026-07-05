#ifndef _HARDWARE_INPUT_H_
#define _HARDWARE_INPUT_H_

#include <cstdint>
#include "Core.h"

class HardwareInput {
public:
    HardwareInput();
    ~HardwareInput();

    MoveCommand getMove();

    void gameStart();
    void notifyDone(bool success);
    void clearDone();

private:
    MoveCommand decodeCommand(uint32_t data);

private:
    int fd;
    void* virtual_base;
    volatile uint32_t* h2p_control;
};

#endif // _HARDWARE_INPUT_H_
