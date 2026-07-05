#include "HardwareInput.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "../../hps_0.h"


#ifndef NEW_COMPONENT_0_BASE
#define NEW_COMPONENT_0_BASE 0x3000
#endif

#define HW_REGS_BASE (ALT_STM_OFST)
#define HW_REGS_SPAN (0x04000000)
#define HW_REGS_MASK (HW_REGS_SPAN - 1)

HardwareInput::HardwareInput()
    : fd(-1),
      virtual_base(nullptr),
      h2p_control(nullptr)
{
    fd = open("/dev/mem", O_RDWR | O_SYNC);

    if (fd == -1) {
        std::cerr << "ERROR: could not open /dev/mem" << std::endl;
        return;
    }

    virtual_base = mmap(
        nullptr,
        HW_REGS_SPAN,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        HW_REGS_BASE
    );

    if (virtual_base == MAP_FAILED) {
        std::cerr << "ERROR: mmap() failed" << std::endl;
        close(fd);
        fd = -1;
        virtual_base = nullptr;
        return;
    }

    h2p_control = reinterpret_cast<volatile uint32_t*>(
        static_cast<char*>(virtual_base) +
        (
            static_cast<unsigned long>(ALT_LWFPGASLVS_OFST + NEW_COMPONENT_0_BASE)
            & static_cast<unsigned long>(HW_REGS_MASK)
        )
    );
}

HardwareInput::~HardwareInput()
{
    if (h2p_control != nullptr) {
        uint32_t data = *h2p_control;
        data &= ~(0x1 << 12);  // clear start bit
        data &= ~(0x3 << 10);  // clear done bits
        *h2p_control = data;
    }

    if (virtual_base != nullptr) {
        if (munmap(virtual_base, HW_REGS_SPAN) != 0) {
            std::cerr << "ERROR: munmap() failed" << std::endl;
        }
    }

    if (fd != -1) {
        close(fd);
    }
}

void HardwareInput::gameStart()
{
    if (h2p_control == nullptr) {
        return;
    }

    uint32_t data = *h2p_control;

    // 全HPS制御ビットを初期状態へ
    data &= ~(0x1 << 12);  // start = 0
    data &= ~(0x3 << 10);  // done = 00
    *h2p_control = data;

    usleep(1000);

    // start pulse
    data |= (0x1 << 12);
    *h2p_control = data;

    usleep(1000);

    // 次回起動のため start を戻す
    data &= ~(0x1 << 12);
    *h2p_control = data;
}

void HardwareInput::clearDone(){
    if (h2p_control == nullptr) {
        return;
    }

    uint32_t data = *h2p_control;
    data &= ~(0x3 << 10); // Clear bits 11 and 10
    *h2p_control = data;
}

MoveCommand HardwareInput::getMove()
{
    // 読み取り前に[11:10]bitが00であることを確認する
    MoveCommand cmd{};

    if (h2p_control == nullptr) {
        std::cerr << "ERROR: hardware register is not mapped" << std::endl;
        return cmd;
    }

    uint32_t data = *h2p_control;

    cmd = decodeCommand(data);

    return cmd;
}

MoveCommand HardwareInput::decodeCommand(uint32_t data)
{
    MoveCommand cmd{};

    cmd.timeup = static_cast<bool>((data >> 9) & 0x1);
    cmd.turn   = static_cast<bool>((data >> 8) & 0x1);
    cmd.select = static_cast<bool>((data >> 7) & 0x1);
    cmd.enter  = static_cast<bool>((data >> 6) & 0x1);

    cmd.y = 7 - static_cast<int>((data >> 3) & 0x7);
    cmd.x = static_cast<int>(data & 0x7);
    
    return cmd;
}

void HardwareInput::notifyDone(bool success)
{
    if (h2p_control == nullptr) {
        return;
    }

    uint32_t data = *h2p_control;
    data &= ~(0x3 << 10); // Clear bits 11 and 10
    if(success) {
        data |= (0x1 << 10);  // Set bit 10 to indicate success
    }else{
        data |= (0x1 << 11); // Set bit 11 to indicate failure
    }
    *h2p_control = data;
}