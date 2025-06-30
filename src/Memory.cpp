#include "Memory.hpp"
#include <stdexcept>
#include <format>

Memory::Memory() {
    m_memory.fill(0x0);
}

void Memory::Write(uint16_t addr, uint8_t data) {
    if (addr > TOTAL_MEMORY_LEN - 1)
        throw std::runtime_error(std::format("[ERROR] Memory Error : WRITE command is out of range ! ADDR -> {:#06x}", addr));

    m_memory[addr] = data;
}

uint8_t Memory::Read(uint16_t addr) {
    if (addr > TOTAL_MEMORY_LEN - 1)
        throw std::runtime_error(std::format("[ERROR] Memory Error : READ command is out of range ! ADDR -> {:#06x}", addr));

    return m_memory[addr];
}
