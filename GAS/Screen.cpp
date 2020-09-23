#include "Screen.h"

namespace ps {

    Screen::Screen(const Params &P_) : P(&P_) {
        calc_refract_points();
        // Initialize all required SDL functionality and create SDL objects.
        init_SDL();
        init_window();
        init_renderer();
        init_texture();
        init_buffers();
    }

    Screen::~Screen() {
        // Destroy all SDL related objects.
        delete[] m_main_buffer;
        delete[] m_blur_buffer;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyTexture(m_texture);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    void Screen::init_SDL() {
        // Initialize SDL library. Returns 0 if successful.
        // Must be called before using any SDL functionality.
        if (SDL_Init(SDL_INIT_VIDEO)) {
            std::cout << "SDL_Init Error: \n" << SDL_GetError() << std::endl;
            exit(1);
        }
    }


    void Screen::init_window() {
        // Creates an SDL window struct. Returns NULL on failure.
        m_window = SDL_CreateWindow(
            "Particle Simulation",       // Window title
            SDL_WINDOWPOS_UNDEFINED,     // Initial x position
            SDL_WINDOWPOS_UNDEFINED,     // Initial y position
            SCREEN_WIDTH,                // Width, in pixels
            SCREEN_HEIGHT,               // Height, in pixels
            SDL_WINDOW_ALLOW_HIGHDPI     // Increases resolution
        );

        // Check if window creation failed.
        if (!m_window) {
            std::cout << "SDL_CreateWindow Error: \n" << SDL_GetError() << std::endl;
            exit(2);
        }
    }


    void Screen::init_renderer() {
        // Creates an SDL renderer struct: a rendering context relative to the state/information
        // of the associated window. Textures to be rendered pass through this renderer and when
        // done rendering are sent to the window to be displayed.
        m_renderer = SDL_CreateRenderer(
            m_window,                      // Window associated with the renderer.
            -1,                            // Index of rendering driver to initialize, -1 to use first available.
            SDL_RENDERER_PRESENTVSYNC |    // Synchronize rendering with window refresh rate.
            SDL_RENDERER_ACCELERATED);     // Allow renderer to use hardware acceleration.

        // Check if renderer creation failed.
        if (!m_renderer) {
            std::cout << "SDL_CreateRenderer Error: \n" << SDL_GetError() << std::endl;
            SDL_DestroyWindow(m_window);
            exit(3);
        }

    }


    void Screen::init_texture() {
        // Creates an SDL texture struct: a set of pixel data to be sent to the renderer.
        m_texture = SDL_CreateTexture(
            m_renderer,                    // The rendering context that this texture will pass through.
            SDL_PIXELFORMAT_RGBA8888,      // The format of the pixel color information, RGBA 8 bits per channel.
            SDL_TEXTUREACCESS_STATIC,      // How the texture is accessed when updated.
            SCREEN_WIDTH,                  // Texture width in pixels, equals associated window width.
            SCREEN_HEIGHT                  // Texture height in pixels, equals associated window height.
        );

        // Check if texture creation failed.
        if (!m_texture) {
            std::cout << "SDL_CreateTexture Error: \n" << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
            exit(3);
        }
    }


    void Screen::init_buffers() {
        // Initialize main_buffer and blur_buffer memory. During runtime, buffers are loaded with
        // particle objects and then fed to the SDL texture to be rendered within the SDL window.
        m_main_buffer = new Uint32[SCREEN_HEIGHT * SCREEN_WIDTH];
        memset(m_main_buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(Uint32));

        m_blur_buffer = new Uint32[SCREEN_HEIGHT * SCREEN_WIDTH];
        memset(m_blur_buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(Uint32));

    }

    void Screen::UpdateTexture() {
        SDL_UpdateTexture(
            m_texture,                      // Texture to be updated.
            NULL,                           // Area in texture to update, NULL for entire area.
            m_main_buffer,                  // Updated pixel data being sent to the texture.
            SCREEN_WIDTH * sizeof(Uint32)   // Number of bytes in a row of pixel data.
        );

        // Copies the newly updated texture into the renderer.
        SDL_RenderCopy(
            m_renderer,    // The renderer to be updated.
            m_texture,     // The source texture.
            NULL,          // Amount of texture to be copied, NULL for entire texture.
            NULL           // Amount of renderer to be updated, NULL for entire renderer.
        );
    }


    void Screen::Render() {
        SDL_RenderPresent(m_renderer);
    }

    void Screen::clear() {
        //auto background = get_uint32_color(255, 255, 255);
        auto background = get_uint32_color(0, 0, 0, 100);
        //auto background = SDL_Color{0,0,0,0};
        for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; ++i) {
            m_main_buffer[i] = background;
        }

    }






    void Screen::load_swarm(std::forward_list <Particle>& particle_list, bool sdl_draw_plus) {
        Uint32 red_color = get_uint32_color(125, 50, 50);
        Uint32 sage_color = get_uint32_color(60, 60, 60);
        Uint32 blue_color = get_uint32_color(50, 50, 125);
        Uint32 white_color = get_uint32_color(125, 125, 125);
        Uint32 color = white_color;



        int red = 0, green = 0, blue = 0;

        for (auto& particle : particle_list) {
            if (particle._x() > SCREEN_WIDTH) continue;

            int x = x_to_pixel(particle.x);
            int y = y_to_pixel(particle.z);


            switch (particle.getState())
            {

            case Particle::State::OK:
                red = 100, green = 100, blue = 255;
                color = blue_color;
                break;

            case Particle::State::WAVE:

                //filledCircleRGBA(m_renderer, x, y, 7, 255, 255, 0, 100);
                draw_circle(particle.x, particle.z, P->burn_radius * particle.wave_counter / P->iterations / 5, get_uint32_color(200, 200, 200));
                /*aacircleRGBA(m_renderer, x, y, 
                    P->screen_proportion * P->burn_radius * particle.wave_counter / P->iterations / 5,
                    255, 255, 255, 255);*/
                //continue;
                break;

            case Particle::State::WARM:
                red = 255; green = 100; blue = 100;
                color = red_color;
                color = white_color;
                //color = blue_color;
                break;

            case Particle::State::BURN:
                red = 255; green = 100; blue = 100;
                color = red_color;

                //color = white_color;
                //draw_circle(particle.x, particle.z, P->burn_radius, get_uint32_color(50, 200, 50));
                    
                break;

            case Particle::State::SAGE:
                color = sage_color;
                break;
                //if (P->sage_time == 0) continue;
                red = 200 - particle.sage_counter * 20;
                green = 100 - particle.sage_counter * 10;
                blue = 100 - particle.sage_counter * 10;

                int lim = 40;

                red = red < lim ? lim : red;
                green = green < lim ? lim : green,
                blue = blue < lim ? lim : blue;
                break;
            }


            if (particle.burn_counter == 1) {
                /*red = 255; green = 255, blue = 255;
                color = white_color;*/
            }

            /*if (particle.burn_counter > 1) {
                continue;
            }*/

            //SDL_SetRenderDrawColor(m_renderer, red, green, blue, SDL_ALPHA_OPAQUE);
            //SDL_RenderDrawPoint(m_renderer, x, y);

            //color = get_uint32_color(red, green, blue);
            set_pixel_color(x, y, color);
            if(sdl_draw_plus) draw_plus(x, y, color);

        }
        //SDL_RenderPresent(m_renderer);
    }

    void Screen::Resize(int w, int h)
    {
        SDL_SetWindowSize(m_window, w, h);
    }

    void Screen::calc_refract_points() {

        for (int i = 0; i < refract_points_count; i++)
        {
            double x_cord = P->area_beg + P->area_size * i / refract_points_count;
            refract_points[i].x = x_to_pixel(x_cord);
            refract_points[i].y = y_to_pixel(P->refract_func(x_cord));
        }
    }

    void Screen::draw_refract_line()
    {
        thickLineRGBA(m_renderer,
            x_to_pixel(P->area_beg),
            y_to_pixel(P->refract_func(P->area_beg)),
            x_to_pixel(P->area_end),
            y_to_pixel(P->refract_func(P->area_end)),
             3, 255,255,255,255 );

        //SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        //SDL_RenderDrawLines(m_renderer, refract_points, refract_points_count);

    }

    void Screen::draw_frontline(const FrontLine::front_line_point* front_line_points, int points_count)
    {
        //double bias = P->front_line_bias;

        SDL_Point* sdl_points = new SDL_Point[points_count];
        int sdl_points_count = 0;

        for (int i = 0; i < points_count; i++)
        {
            if (front_line_points[i].z > 0)
            {
                sdl_points[sdl_points_count].x = x_to_pixel(front_line_points[i].x);
                sdl_points[sdl_points_count].y = y_to_pixel(front_line_points[i].z);
                sdl_points_count++;

            }

        }

        SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLines(m_renderer, sdl_points, sdl_points_count);

        //aacircleColor(m_renderer, 300, 400, 100, get_uint32_color(255, 255, 255));
        //circleColor(m_renderer, 300, 550, 100, get_uint32_color(255, 255, 255));
        /*circleRGBA(m_renderer, 300, 500, 100, 255,255,255,100);
        circleRGBA(m_renderer, 300, 550, 100, 255,255,255,100);
        circleRGBA(m_renderer, 300, 600, 100, 255,255,255,100);*/
        //SDL_RenderPresent(m_renderer);
    }

    void Screen::draw_circles(std::vector <Particle*>& particle_list) {


        int red = 200, green = 100, blue = 100;


        for (auto& particle : particle_list) {
            if (particle->_x() > SCREEN_WIDTH) continue;

            int x = x_to_pixel(particle->x);
            int y = y_to_pixel(particle->z);


            Uint32 color = get_uint32_color(red, green, blue);
            set_pixel_color(x, y, color);
            //draw_plus(x, y, color);
            draw_circle(particle->x, particle->z, P->burn_radius, get_uint32_color(50, 200, 50));


        }




    }


    void Screen::box_blur() {
        // Main_buffer and blur_buffer are swapped, blur is applied to blur_buffer, and then
        // during each next iteration buffers are swapped again to display the blur on screen.
        std::swap(m_main_buffer, m_blur_buffer);

        // Iterates through each pixel in the buffer, gets average color of surrounding pixels,
        // and applies average surrounding color to each pixel. Creates a gaussian blur effect.
        for (int y = 0; y < SCREEN_HEIGHT; ++y) {
            for (int x = 0; x < SCREEN_WIDTH; ++x) {
                Uint8 red{ 0 }, green{ 0 }, blue{ 0 };
                get_avg_color(x, y, red, green, blue);
                set_pixel_color(x, y, red, green, blue);
            }
        }
    }


    void Screen::get_avg_color(int x, int y, Uint8& red, Uint8& green, Uint8& blue) {
        int red_total{ 0 }, green_total{ 0 }, blue_total{ 0 };

        // Totals the color values of the input pixel (x, y) and the 8 pixels that surround it.
        for (int row = -1; row <= 1; ++row) {
            for (int col = -1; col <= 1; ++col) {
                int current_x = x + col,
                    current_y = y + row;

                // Only grab values of pixels that are actually within the window
                if (current_x >= 0 && current_x < SCREEN_WIDTH &&
                    current_y >= 0 && current_y < SCREEN_HEIGHT) {

                    // Get the color value of the current pixel
                    Uint32 color = m_blur_buffer[current_x + (current_y * SCREEN_WIDTH)];

                    // Bit shift RGB values of pixel color into color totals.
                    red_total += static_cast<Uint8>(color >> 24);
                    green_total += static_cast<Uint8>(color >> 16);
                    blue_total += static_cast<Uint8>(color >> 8);
                }
            }
        }
        // Averages the color values of the input pixel (x, y) and its 8 surrounding pixels.
        // Changing the denominator here can create interesting visual effects.
        red = static_cast<Uint8>(red_total / 9);
        green = static_cast<Uint8>(green_total / 9);
        blue = static_cast<Uint8>(blue_total / 9);
    }

    void Screen::draw_circle(double x0, double y0, double r, Uint32 color)
    {
        int steps = ceil(r * P->screen_proportion * M_PI );
        double deg = 0, deg_step = M_PI * 2 / steps;
        double x, y;
        
        for (int i = 0; i < steps; i++)
        {
            x = x0 + r*sin(deg);
            y = y0 + r*cos(deg);
            deg += deg_step;
            if (y > P->refract_func(x)) {
                set_pixel_color(x_to_pixel(x), y_to_pixel(y), color);
            }
        }

    }

    void Screen::draw_grid(int x_count, int y_count)
    {
        //auto color = get_uint32_color(0, 0, 255, 100); 
        
        for (int i = 1; i < x_count; i++)
        {
            int x = (int)round(SCREEN_WIDTH / (double)x_count * i);
            vlineRGBA(m_renderer, x, 0, SCREEN_HEIGHT, 60,60,90,100);
        }
        for (int i = 1; i < y_count; i++)
        {
            int y = (int)round(SCREEN_HEIGHT / (double)y_count * i);
            hlineRGBA(m_renderer, 0, SCREEN_WIDTH, y, 60, 60, 90, 100);
        }
    }
    void Screen::draw_grid2(int x_count, int y_count)
    {
        //auto color = get_uint32_color(255,255,255);
        //auto color = get_uint32_color(40,200,40);
        //auto color = get_uint32_color(15, 15, 30);
        auto color = get_uint32_color(30, 30, 45);
        
        for (int i = 1; i < x_count; i++)
        {
            int x = (int)round(SCREEN_WIDTH / (double)x_count * i);
            for (int j = 0; j < SCREEN_HEIGHT; j++)
            {
                set_pixel_color(x, j, color);
            }
        }
        for (int i = 1; i < y_count; i++)
        {
            int y = (int)round(SCREEN_HEIGHT / (double)y_count * i);
            for (int j = 0; j < SCREEN_WIDTH; j++)
            {
                set_pixel_color(j, y, color);
            }
        }
    }

    Uint32 Screen::get_uint32_color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {

        Uint32 color{ 0 };

        // Bit shift colors into proper positions within color (RGBA)
        color += red;
        color <<= 8;
        color += green;
        color <<= 8;
        color += blue;
        color <<= 8;
        color += alpha;

        return color;
    }

    int Screen::x_to_pixel(double x)
    {
        return static_cast<int>((x - P->area_beg) * P->screen_proportion);
    }

    int Screen::y_to_pixel(double y)
    {
        return SCREEN_HEIGHT - P->screen_bottom_gap - static_cast<int>(y * P->screen_proportion);
    }


    void Screen::draw_plus(int x, int y, Uint32 color) {

        if (x < 0 || x >= ps::Screen::SCREEN_WIDTH || y < 0 || y >= ps::Screen::SCREEN_HEIGHT)
            return;

        if (x > 0) {
            m_main_buffer[(x - 1) + (y * SCREEN_WIDTH)] = color;
        }
        if (x < SCREEN_WIDTH - 2) {
            m_main_buffer[(x + 1) + (y * SCREEN_WIDTH)] = color;
        }
        if (y > 0) {
            m_main_buffer[x + ((y - 1) * SCREEN_WIDTH)] = color;
        }
        if (y < SCREEN_HEIGHT - 2) {
            m_main_buffer[x + ((y + 1) * SCREEN_WIDTH)] = color;
        }
    }


    void Screen::set_pixel_color(int x, int y, Uint32 color) {

        // Ignore particles with coordinates outside the boundaries of the SDL window
        if (x < 0 || x >= ps::Screen::SCREEN_WIDTH || y < 0 || y >= ps::Screen::SCREEN_HEIGHT)
            return;

        m_main_buffer[x + (y * SCREEN_WIDTH)] = color;

    }


    void Screen::set_pixel_color(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {

        // Ignore particles with coordinates outside the boundaries of the SDL window
        if (x < 0 || x >= ps::Screen::SCREEN_WIDTH || y < 0 || y >= ps::Screen::SCREEN_HEIGHT)
            return;


        Uint32 color = get_uint32_color(red, green, blue);
        set_pixel_color(x, y, color);
        
    }


    bool Screen::quit_program() {
        // Check for SDL events. If the window is closed, quit the program.
        while (SDL_PollEvent(&m_event)) {
            if (m_event.type == SDL_QUIT)
                return true;
        }
        return false;
    }

	void Screen::SetTitle(const char* title)
	{
        SDL_SetWindowTitle(m_window, title);
	}

} /* namespace ps */
