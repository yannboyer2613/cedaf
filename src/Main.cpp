#include <SDL_render.h>
#include <SDL_video.h>
#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>
#include "Consts.hpp"
#include "VM.hpp"
#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cerr << "[ERROR] No ROM provided !\n";
        std::cout << "[INFO] Usage : ./cedaf my_game.rom/my_game.ch8\n";
        return EXIT_FAILURE;
    }

    std::string romPath = std::string(argv[1]);

    VM chip8VM;

    try {
        chip8VM.InitAudio();
        chip8VM.LoadROM(romPath);
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("CEDAF - Chip8VM by Yann BOYER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cerr << "[ERROR] Unable to initialize the window !\n";
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (renderer == nullptr) {
        std::cerr << "[ERROR] Unable to initialize the renderer !\n";
        return EXIT_FAILURE;
    }

    uint8_t divCycles = 0;

    bool isRunning = true;

    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                default: break;
            }
        }

        try {
            chip8VM.DoSingleCPUCycle();
            divCycles++;
        } catch (std::exception& e) {
            std::cerr << e.what() << "\n";
            std::exit(EXIT_FAILURE);
        }


        // Drawing.
        for (uint8_t y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
            for (uint8_t x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
                SDL_Rect pixel = { x * PIXEL_SCALE_FACTOR, y * PIXEL_SCALE_FACTOR, PIXEL_SCALE_FACTOR, PIXEL_SCALE_FACTOR };
                if (chip8VM.RT_IsPixelOn(x, y))
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                else
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer, &pixel);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer); // Prevent slowdowns...

        if (divCycles == TIMER_CLOCK_DIVISION) {
            try {
                chip8VM.UpdateCPUTimers();
                divCycles = 0;
            } catch (std::exception& e) {
                std::cerr << e.what() << "\n";
                std::exit(EXIT_FAILURE);
            }
        }

        std::this_thread::sleep_for(std::chrono::microseconds(CPU_CLOCK_DELAY));
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
