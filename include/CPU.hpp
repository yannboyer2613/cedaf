#ifndef CPU_HPP
#define CPU_HPP

#include "Audio.hpp"
#include "Memory.hpp"
#include "RenderTable.hpp"
#include <cstdint>
#include <array>
#include <vector>

enum class KeyState {
    KeyPressed,
    KeyNotPressed,
};

class CPU {
    public:
        CPU();

        void UpdateTimers(Audio& audioPlayer);
        uint16_t FetchNextOpcode(Memory& memory);
        void DecodeAndExecute(uint16_t opcode, Memory& memory, RenderTable& renderTable);
    private:
        static constexpr uint8_t REGISTERS_COUNT = 16;
        static constexpr uint8_t KEYS_COUNT = 16;

        std::array<uint8_t, REGISTERS_COUNT> m_regs;
        std::vector<uint16_t> m_stack;
        std::array<KeyState, KEYS_COUNT> m_keys;

        uint16_t m_indexRegister;
        uint8_t m_delayTimer;
        uint8_t m_soundTimer;

        uint16_t m_programCounter;
};

#endif
