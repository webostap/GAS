#pragma once

#ifndef PARTICLE_SIMULATION_SCREEN_H
#define PARTICLE_SIMULATION_SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
//#include <SDL2/SDL2_gfxPrimitives.h>
#include "Particle.h"
#include <iostream>
#include <vector>
#include <forward_list>

namespace ps {

    class Screen {
    public:
        Screen();
        virtual ~Screen();
        void update();
        void box_blur();
        void draw_circles(std::vector <Particle*>& particle_list);
        void load_swarm(std::forward_list <Particle>& particle_list);
        bool quit_program();

        void SetTitle(const char*);

        SDL_Window* GetWindow() {
            return m_window;
        }

        void clear();
        void draw_grid(int x_count, int y_count);

    private:
        void init_SDL();
        void init_window();
        void init_renderer();
        void init_texture();
        void init_buffers();
        Uint32 get_uint32_color(Uint8, Uint8, Uint8);
        int x_to_pixel(double);
        int y_to_pixel(double);
        void draw_plus(int, int, Uint32);
        void set_pixel_color(int, int, Uint32);
        void set_pixel_color(int, int, Uint8, Uint8, Uint8);
        void get_avg_color(int, int, Uint8&, Uint8&, Uint8&);
        void draw_circle(double x, double y, double r, int steps, Uint32 color);


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
