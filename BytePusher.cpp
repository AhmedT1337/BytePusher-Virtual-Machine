#include <fstream>

#include "BytePusher.hpp"

BytePusher::BytePusher()
{
    memory.resize(MEMORY_SIZE);
    pc = 0;
    keypad = 0;
    
}

BytePusher::~BytePusher()
{
    
}

void BytePusher::LoadROM(char const* filename){
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streampos size = file.tellg();
    char* buffer = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();
    for (long i = 0; i<size; ++i){
        memory[i] = buffer[i];
    }
    delete[] buffer;
}

void BytePusher::ByteBytePush(){
    uint32_t A = (memory[pc] << 16) | (memory[pc + 1] << 8) | (memory[pc + 2]);
    uint32_t B = (memory[pc + 3] << 16) | (memory[pc + 4] << 8) | (memory[pc + 5]);
    uint32_t C = (memory[pc + 6] << 16) | (memory[pc + 7] << 8) | (memory[pc + 8]);
    
    memory[B] = memory[A];
    pc = C;
    
}

void BytePusher::InnerLoop(){
    pc = (memory[2] << 16) | (memory[3] << 8) | (memory[4]);
    int i = 65536;
    do {
        ByteBytePush();
    } while (--i);
}

void BytePusher::Tick(){
    memory[0] = keypad >> 8;
    memory[1] = keypad & 0xFF;
    InnerLoop();
    pixelPage = memory[5];
    audioPage = (memory[6] << 8) | memory[7];

}