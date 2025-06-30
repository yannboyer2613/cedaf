#include "RenderTable.hpp"
#include <format>

RenderTable::RenderTable() {
    m_renderTable.fill(PixelState::PixelOff);
}

bool RenderTable::IsPixelOn(uint8_t x, uint8_t y) {
    if (x > CHIP8_SCREEN_WIDTH || y > CHIP8_SCREEN_HEIGHT)
        throw std::runtime_error(std::format("[ERROR] Invalid coordinates : X -> {} Y -> {}", x, y));

    return m_renderTable[y * CHIP8_SCREEN_WIDTH + x] == PixelState::PixelOn;
}

void RenderTable::ChangePixelStateTo(uint8_t x, uint8_t y, PixelState newState) {
    if (x > CHIP8_SCREEN_WIDTH || y > CHIP8_SCREEN_HEIGHT)
        throw std::runtime_error(std::format("[ERROR] Invalid coordinates : X -> {} Y -> {}", x, y));

    m_renderTable[y * CHIP8_SCREEN_WIDTH + x] = newState;
}

void RenderTable::Clear() {
    for (std::size_t y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
        for (std::size_t x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
            m_renderTable[y * CHIP8_SCREEN_WIDTH + x] = PixelState::PixelOff;
        }
    }
}
