#pragma once

#ifndef PARTICLE_SIMULATION_SCREEN_H
#define PARTICLE_SIMULATION_SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Params.hpp"
#include "Particle.h"
#include "Frontline.h"
#include <iostream>
#include <vector>
#include <forward_list>

namespace ps {

    class Screen {
        const Params* P;
    public:
        Screen(const Params&);
        virtual ~Screen();
        void UpdateTexture();
        void Render();
        void box_blur();
        void draw_circles(std::vector <Particle*>& particle_list);
        void load_swarm(std::forward_list <Particle>& particle_list, bool sdl_draw_plus = false);

        void calc_refract_points();
        void draw_refract_line();
        const int refract_points_count = 100;
        SDL_Point refract_points[100] = {0};

        void draw_frontline(const FrontLine::front_line_point*, int size);
        //void foreach_particles(std::forward_list <Particle>& particle_list);
        bool quit_program();

        void SetTitle(const char*);

        SDL_Window* GetWindow() {
            return m_window;
        }

        void clear();
        void draw_grid(int x_count, int y_count);
        void draw_grid2(int x_count, int y_count);

    private:
        void init_SDL();
        void init_window();
        void init_renderer();
        void init_texture();
        void init_buffers();
        Uint32 get_uint32_color(Uint8, Uint8, Uint8, Uint8 = 255);
        int x_to_pixel(double);
        int y_to_pixel(double);
        void draw_plus(int, int, Uint32);
        void set_pixel_color(int, int, Uint32);
        void set_pixel_color(int, int, Uint8, Uint8, Uint8);
        void get_avg_color(int, int, Uint8&, Uint8&, Uint8&);
        void draw_circle(double x, double y, double r, int steps, Uint32 color);


    private:
        const int SCREEN_WIDTH{ P->screen_width };
        const int SCREEN_HEIGHT{ P->screen_height };
        SDL_Window* m_window{ nullptr };
        SDL_Renderer* m_renderer{ nullptr };
        SDL_Texture* m_texture{ nullptr };
        SDL_Color* SDL_Color_buffer{ nullptr };
        Uint32* m_main_buffer{ nullptr };
        Uint32* m_blur_buffer{ nullptr };
        SDL_Event m_event;
    };

} /* namespace ps */


#endif //PARTICLE_SIMULATION_SCREEN_H
