#pragma once
#include <cstdint>
#include <vector>

#define MEMORY_SIZE 0x1000008

class BytePusher{
    private:
    public:
    std::vector<uint8_t> memory;
    uint32_t pc{};
    uint32_t opcode{};
    uint16_t keypad{};
    uint8_t pixelPage{};
    uint16_t audioPage{};
    void LoadROM(char const* filename);
    void ByteBytePush();
    void InnerLoop();
    void Tick();
    BytePusher();
    ~BytePusher();
};