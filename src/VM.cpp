#include "VM.hpp"
#include "Consts.hpp"
#include <fstream>
#include <vector>

void VM::InitAudio() {
    m_audioPlayer.Init();
}

void VM::LoadROM(const std::string& romPath) {
    std::ifstream romFile(romPath);

    if (!romFile.is_open())
        throw std::runtime_error("[ERROR] Unable to open the ROM file ! :(");

    romFile.seekg(0, std::ios::end);
    std::size_t romBufferLen = romFile.tellg();
    romFile.seekg(0, std::ios::beg);

    std::vector<uint8_t> romBufferData;

    romBufferData.reserve(romBufferLen);
    romFile.read(reinterpret_cast<char*>(romBufferData.data()), romBufferLen);
    romFile.close();

    // Load FONTSET before anything else.
    for (std::size_t idx = 0; idx < FONTSET_SIZE; idx++) {
        m_memory.Write(idx, FONTSET[idx]);
    }

    for (std::size_t idx = 0; idx < romBufferLen; idx++) {
        m_memory.Write(idx + PROCESSOR_INTERNAL_PROGRAM_COUNTER_START, romBufferData[idx]);
    }
}

bool VM::RT_IsPixelOn(uint8_t x, uint8_t y) {
    return m_renderTable.IsPixelOn(x, y);
}

void VM::UpdateCPUTimers() {
    m_cpu.UpdateTimers(m_audioPlayer);
}

void VM::DoSingleCPUCycle() {
    uint16_t opcode = m_cpu.FetchNextOpcode(m_memory);
    m_cpu.DecodeAndExecute(opcode, m_memory, m_renderTable);
}
