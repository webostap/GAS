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
	//SDL_Event event;
	const Uint8* key_state;


	char c;


	ps::Segments main_swarm;
	ps::Segments::Segment* segment;
	//print_files(main_swarm);
	//clear_csv_files();
	//print_final(main_swarm, 2);
	//return 0;


	ps::Screen screen;
	bool burned = false;
	int ii = 0;

	int mouse_x, mouse_y;
	
	while (!screen.quit_program()) {



		if (!startTime) {
			// get the time in ms passed from the moment the program started
			startTime = SDL_GetTicks();
		}
		else {
			delta = endTime - startTime; // how many ms for a frame
		}

		// if less than 16ms, delay
		if (delta < timePerFrame) {
			SDL_Delay(timePerFrame - delta);
		}

		/*if (!burned && ii == P::burn_at_step) {
			burned = true;
			main_swarm.Lighter();
		}*/

		//main_swarm.Step();


		main_swarm.UpdateSegments();

		SDL_PumpEvents();
		if (SDL_GetMouseState(&mouse_x, &mouse_y) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			//printf("%f\t%f\n", P::screen_to_area_x(mouse_x), P::screen_to_area_y(mouse_y));
			segment = main_swarm.GetSegment(P::screen_to_area_x(mouse_x), P::screen_to_area_y(mouse_y));
			main_swarm.BurnSegment(*segment);
		}

		key_state = SDL_GetKeyboardState(NULL);
		if (key_state[SDL_SCANCODE_SPACE]) {
			main_swarm.LightsOut();
		}

		main_swarm.BurnParticles();

		main_swarm.MoveParticles();

		main_swarm.ClearParticleList();

		main_swarm.Fill_2();

		//main_swarm.Step();


		// Load current particle swarm.
		screen.load_swarm(main_swarm.all_list);

		// Update SDL window with new particle positions and colors.
		screen.update();



		// Manipulate particle positions for next iteration. 
		// Ticks are used to ensure consistent/proportional movement.
		//swarm.move(SDL_GetTicks());
		//std::cout<<SDL_GetTicks()<<std::endl;

		// Apply gaussian blur effect.
		//screen.box_blur();

		//if (endTime > 2000)
			//swarm.burn_step();


		// if delta is bigger than 16ms between frames, get the actual fps
		if (delta > timePerFrame) 
		{
			fps = 1000 / delta;
		}

		//printf("FPS is: %i \n", fps);

		startTime = endTime;
		endTime = SDL_GetTicks();

		//if (!ii)
		{
			//std::cout << main_swarm.all_list.size() << '\n';
		}
		if (burned) 
		{
			//_getch();
		}
		

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

