#include <iostream>
#include <fstream>
#include <ctime> 
#include "Screen.h"
#include "Segments.h"

void print_files(ps::Segments& swarm);
void print_final(ps::Segments&, int);
void clear_csv_files();


#undef main
int main() {



	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 delta = 0;
	short fps = 60;
	short timePerFrame = 1000 / fps; // miliseconds
	SDL_Event e;
	const Uint8* key_state;

	char buffer[] = "FPS: 00";


	char c;


	ps::Segments main_swarm;
	ps::Segments::Segment* segment = main_swarm.GetSegment(0, 0);
	//print_files(main_swarm);
	//clear_csv_files();
	//print_final(main_swarm, 2);
	//return 0;


	ps::Screen screen;
	bool sdl_quit = false, 
		set_burn = false, 
		lights_out = false, 
		pause = false;
	int ii = 0;

	int mouse_x, mouse_y;
	double burn_x, burn_y;

	
	while (!sdl_quit) {


		if (!startTime) startTime = SDL_GetTicks();
		else delta = endTime - startTime;

		if (delta < timePerFrame) SDL_Delay(timePerFrame - delta);


		/*----- INPUT EVENTS ------*/

		sdl_quit = false;
		set_burn = false;
		lights_out = false;

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT: sdl_quit = true;
				break;

			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_1: main_swarm.stream_func = P::linear_stream;
						break;
					case SDLK_2: main_swarm.stream_func = P::log_stream;
						break;
					case SDLK_3: main_swarm.stream_func = P::x2_stream;
						break;
					case SDLK_4: main_swarm.stream_func = P::const_stream;
						break;
					case SDLK_SPACE: lights_out = true;
						break;
					case SDLK_p: pause = !pause;
						break;
				}
				break;
				
			break;
			}
			// break SDL_PollEvent
			if (sdl_quit) break;
		}


		// break sdl loop if window closed //
		if (sdl_quit) break;
		///////////////////


		if (SDL_BUTTON(SDL_BUTTON_LEFT) && SDL_GetMouseState(&mouse_x, &mouse_y)) {

			set_burn = true;
			burn_x = P::screen_to_area_x(mouse_x);
			burn_y = P::screen_to_area_y(mouse_y);
			segment = main_swarm.GetSegment(burn_x, burn_y);

		}

		if (!pause) // main action
		{
			//for (int iterate = 0; iterate < P::iterations; ++iterate)
			{

				if (lights_out) main_swarm.LightsOut();

				main_swarm.UpdateSegments();

				if (set_burn) main_swarm.BurnSegment(segment);

				main_swarm.CrossParticles();
				main_swarm.MoveParticles();
				main_swarm.ClearParticleList();
				main_swarm.Fill_Sampling();
			}


			screen.load_swarm(main_swarm.all_list);
			screen.update();

			// Apply gaussian blur effect.
			//screen.box_blur();

		}




		if (delta > timePerFrame) fps = 1000 / delta;


		if (ii % 10 == 0) 
		{
			sprintf_s(buffer, "FPS: %d", fps);
			screen.SetTitle(buffer);
		}
		

		startTime = endTime;
		endTime = SDL_GetTicks();


		

		++ii;
	}

	return 0;


	

}

void print_files(ps::Segments &swarm) {

	for (size_t i = 0; i < P::steps; i++)
	{

		if (i == P::burn_at_step) swarm.Lighter();

		swarm.Step();

		if (i % 10 == 0)
		{
			swarm.PrintStep(i / 10);
			//main_swarm.PrintStep(i);
		}

		std::cout << i << ")\t" << swarm.all_list.size() << '\n';
	}
}
void print_final(ps::Segments &swarm, int num = 0) {

	for (size_t i = 0; i < P::steps; i++)
	{
		if (i == P::burn_at_step) swarm.Lighter();
		swarm.Step();
	}
	swarm.PrintStep(num);
}

void clear_csv_files() {
	bool gas_out = 0, line_out = 0;

	for (size_t i = 0;; ++i)
	{
		if (!gas_out && std::remove((P::csv_folder + "gas.csv." + std::to_string(i)).c_str()) != 0) {
			gas_out = 1;
		}
		if (!line_out && std::remove((P::csv_folder + "line.csv." + std::to_string(i)).c_str()) != 0) {
			line_out = 1;
		}

		if (gas_out && line_out) return;

	}
}

