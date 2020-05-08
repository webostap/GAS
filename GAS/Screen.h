#pragma once

#ifndef PARTICLE_SIMULATION_SCREEN_H
#define PARTICLE_SIMULATION_SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include "Segments.h"
#include <iostream>

namespace ps {

    class Screen {
    public:
        Screen();
        virtual ~Screen();
        void update();
        void box_blur();
        void load_swarm(std::list <Particle>& particle_list);
        bool quit_program();

        void SetTitle(const char*);

        SDL_Window* GetWindow() {
            return m_window;
        }

    private:
        void init_SDL();
        void init_window();
        void init_renderer();
        void init_texture();
        void init_buffers();
        void set_pixel_color(int, int, Uint8, Uint8, Uint8);
        void get_avg_color(int, int, Uint8&, Uint8&, Uint8&);

    private:
        const static int SCREEN_WIDTH{ P::screen_width };
        const static int SCREEN_HEIGHT{ P::screen_height };
        SDL_Window* m_window{ nullptr };
        SDL_Renderer* m_renderer{ nullptr };
        SDL_Texture* m_texture{ nullptr };
        Uint32* m_main_buffer{ nullptr };
        Uint32* m_blur_buffer{ nullptr };
        SDL_Event m_event;
    };

} /* namespace ps */


#endif //PARTICLE_SIMULATION_SCREEN_H
