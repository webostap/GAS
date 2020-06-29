﻿#include <iostream>
#include <fstream>
#include <string>
#include <ctime> 
#include <cstdlib>

#include "Screen.h"
#include "Segments.h"




void print_speed() {
	std::cout << "\n-------------\n";
	std::cout << P::stream_function(P::area_center) * P::base_speed << " - base speed\n";
	std::cout << P::particle_speed(P::area_center) << " - max delta\n";
	std::cout << P::burn_radius << " - burn radius\n";
	std::cout << P::burn_radius_2 << " - burn radius 2\n";
	std::cout << P::burn_radius_2_cross << " - burn radius 2 fix\n";
	std::cout << P::burn_speed << " - burn speed\n";
}

void clear_csv_files();



class input : public H::Singleton<input>
{ friend class Singleton<input>;

public:
	bool sdl_quit = 0,
		set_burn = 0,
		lights_out = 0,
		pause = 0,
		step = 0,
		toggle_fill = 0,
		print_step = 0,
		print_denisty = 0,
		clear_csv = 0;
private:
	input() {}
};


#undef main
int main() {



	P::read_params();
	print_speed();

	ps::Segments main_swarm;


	struct {
		bool move = true,
			pause = false;
			
	} State;


	input Input = input::instance();


	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 delta = 0;
	short fps = 30;
	short timePerFrame = 1000 / fps; // miliseconds
	SDL_Event e;
	//const Uint8* key_state;

	char buffer[] = "FPS: 00";


	ps::Segments::Segment* segment = main_swarm.GetSegment(0, 0);

	ps::Screen screen;


		
	int ii = 0, print_step_counter = 0;

	std::ifstream check_file;
	for (;;) {
		check_file.open((P::csv_folder + "line.csv." + std::to_string(print_step_counter)).c_str());
		if (check_file.fail()) break;
		check_file.close();
		++print_step_counter;
	}


	int mouse_x, mouse_y;
	double burn_x, burn_y;

	/*----- HELP ------*/

	/*std::ifstream help_file("help.txt");
	std::string help_text((std::istreambuf_iterator<char>(help_file)),
		std::istreambuf_iterator<char>());
	help_file.close();*/

	std::string help_text = H::file_to_string("help.txt");
	//std::cout << help_text;

	/* ---- */


	bool even = false;
	
	while (!Input.sdl_quit) {


		if (!startTime) startTime = SDL_GetTicks();
		else delta = endTime - startTime;

		if (delta < timePerFrame) SDL_Delay(timePerFrame - delta);


		/*----- INPUT EVENTS ------*/

		Input = input::instance();

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT: Input.sdl_quit = true;
				break;

			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_1: P::stream_function = P::linear_stream;
						P::read_params();
						break;
					case SDLK_2: P::stream_function = P::log_stream;
						P::read_params();
						break;
					case SDLK_3: P::stream_function = P::x2_stream;
						P::read_params();
						break;
					case SDLK_4: P::stream_function = P::const_stream;
						P::read_params();
						break;
					case SDLK_SPACE: Input.lights_out = true;
						break;
					case SDLK_p: State.pause = !State.pause;
						break;
					case SDLK_s: Input.step = true;
						break;
					case SDLK_m: State.move = !State.move;
						break;
					case SDLK_f: main_swarm.Toggle_Fill();
						std::cout << "\nToggle Fill";
						break;
					case SDLK_c: Input.print_step = true;
						break;
					case SDLK_d: Input.print_denisty = true;
						break;
					case SDLK_x: Input.clear_csv = true;
						break;
					case SDLK_u: 
						P::read_params();
						main_swarm.SetBurnRadius(P::burn_radius);
						main_swarm.SetFillGrid(P::particles_dist);
						print_speed();
						//std::cout << P::area_size / main_swarm.Line_Count() << " - L / N\n";
						//std::cout << P::burn_radius / P::area_size * main_swarm.Line_Count() << " - r / d\n";
						//std::cout << P::area_size / P::burn_radius << " - must be\n";
						//std::cout << main_swarm.Line_Count() << " - line\n";
						//std::cout << main_swarm.all_list.size() << " - size\n";
						//main_swarm.LoadParams(swarm_params);
						break;
				}

				//P::read_params();
				break;
				
			break;
			}
			// break SDL_PollEvent
			if (Input.sdl_quit) break;
		}


		// break sdl loop if window closed //
		if (Input.sdl_quit) break;
		///////////////////


		if (SDL_BUTTON(SDL_BUTTON_LEFT) && SDL_GetMouseState(&mouse_x, &mouse_y)) {

			Input.set_burn = true;
			burn_x = P::screen_to_area_x(mouse_x);
			burn_y = P::screen_to_area_y(mouse_y);
			//printf("\n%i\t%i", mouse_x, mouse_y);
			//printf("\n%f\t%f", burn_x, burn_y);
			segment = main_swarm.GetSegment(burn_x, burn_y);

		}

		/* --- MAIN ACTION --- */

		if (!State.pause || State.pause && Input.step)
		{
			if (0&&(even = !even) ){

				screen.draw_circles(main_swarm.all_will_burn);
				screen.update();
			}

		else {


			for (int iterate = P::iterations; iterate/* == P::iterations*/; --iterate)
			{
				main_swarm.UpdateSegments();

				if (Input.set_burn) main_swarm.BurnSegment(segment);
				if (Input.lights_out) main_swarm.LightsOut();

				main_swarm.CrossParticles();
				main_swarm.FinalLoop(State.move);
				if (State.move) main_swarm.Fill();


				/*main_swarm.CrossParticles();
				main_swarm.StepParticles();

				if (State.move)
				{
					main_swarm.MoveParticles();
					main_swarm.Fill_Sampling();
				}

				main_swarm.ClearParticleList();*/
			}

			screen.clear();

			screen.draw_grid(main_swarm.grid_count_x, main_swarm.grid_count_z);

			screen.load_swarm(main_swarm.all_list);
			//screen.box_blur();

			screen.update();

			/* Apply gaussian blur effect. */

		}
		}

		if (Input.clear_csv)
		{
			clear_csv_files();
			print_step_counter = 0;
			std::cout << "\nclear files";
		}


		if (Input.print_denisty)
		{

			main_swarm.Density_Grid();
			main_swarm.Density_Radius();
			main_swarm.Max_Radius();

			std::cout << "\nprint - denisty";

		}


		if (Input.print_step)
		{
			//main_swarm.PrintStep(print_step_counter);

			main_swarm.PrintLine(print_step_counter);

			std::cout << "\nprint - " << print_step_counter;

			++print_step_counter;
		}

		/* --- END OF MAIN ACTION --- */




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

//void print_files(ps::Segments &swarm) {
//
//	for (int i = 0; i < P::steps; i++)
//	{
//
//		if (i == P::burn_at_step) swarm.Lighter();
//
//		swarm.Step();
//
//		if (i % 10 == 0)
//		{
//			swarm.PrintStep(i / 10);
//			//main_swarm.PrintStep(i);
//		}
//
//		std::cout << i << ")\t" << swarm.all_list.size() << '\n';
//	}
//}
//void print_final(ps::Segments &swarm, int num = 0) {
//
//	for (int i = 0; i < P::steps; i++)
//	{
//		if (i == P::burn_at_step) swarm.Lighter();
//		swarm.Step();
//	}
//	swarm.PrintStep(num);
//}

void clear_csv_files() {
	bool gas_out = 0, line_out = 0;

	for (int i = 0;; ++i)
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




