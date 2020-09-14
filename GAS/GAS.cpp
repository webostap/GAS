﻿#include <iostream>
#include <fstream>
#include <string>
#include <ctime> 
#include <cstdlib>
#include <unordered_map>

//#include "Parametrs.inl"
#include "Params.hpp"
#include "Screen.h"
#include "Segments.h"
#include "FrontLine.h"




void clear_csv_files(const ps::Params & P);



struct input {
	bool sdl_quit = 0,
		set_burn = 0,
		lights_out = 0,
		clear = 0,
		pause = 0,
		step = 0,
		toggle_fill = 0,
		print_step = 0,
		print_denisty = 0,
		clear_csv = 0;
};


#undef main
int main() {
	
	ps::Params params;
	params.Print();

	ps::Segments main_swarm(params);



	struct {
		bool move = true,
			pause = false,
			display_swarm = true,
			display_line = true,
			bold_points = false,
			update_line = true;
			
	} State;


	input Input;


	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 delta = 0;
	short fps = 30;
	short timePerFrame = 1000 / fps; // miliseconds
	SDL_Event e;
	//const Uint8* key_state;

	char buffer[] = "FPS: 00";


	ps::Segments::Segment* segment = main_swarm.GetSegment(0, 0);

	ps::Screen screen(params);
	screen.calc_refract_points();


		
	int print_step_counter = 0;

	std::ifstream check_file;
	for (;;) {
		check_file.open((params.csv_folder + "line.csv." + std::to_string(print_step_counter)).c_str());
		if (check_file.fail()) break;
		check_file.close();
		++print_step_counter;
	}


	int mouse_x, mouse_y;
	double burn_x, burn_y;



	bool even = false;
	bool key_pressed = false;
	
	int ii = 0;

	while (!Input.sdl_quit) {

		++ii;



		endTime = SDL_GetTicks();
		delta = endTime - startTime;
		startTime = endTime;

		if (delta < timePerFrame) {
			SDL_Delay(timePerFrame - delta);
		} else fps = 1000 / delta;


		if (ii % 10 == 0)
		{
			sprintf_s(buffer, "FPS: %d", fps);
			screen.SetTitle(buffer);
		}






		/*----- INPUT EVENTS ------*/

		Input = input();
		key_pressed = false;

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT: Input.sdl_quit = true;
				break;

			case SDL_KEYDOWN:
				key_pressed = true;
				switch (e.key.keysym.sym)
				{
					case SDLK_1: params.SetStream(1);
						break;
					case SDLK_2: params.SetStream(2);
						break;
					case SDLK_3: params.SetStream(3);
						break;
					case SDLK_4: params.SetStream(4);
						break;
					case SDLK_SPACE: Input.lights_out = true;
						break;
					case SDLK_DELETE: Input.clear = true;
						break;
					case SDLK_p: State.pause = !State.pause;
						std::cout << (State.pause ? "\n- Pause" : "\n- Play");
						break;
					case SDLK_s: Input.step = true;
						even = !even;
						break;
					case SDLK_q: State.bold_points = !State.bold_points;
						break;
					case SDLK_m: State.move = !State.move;
						std::cout << (State.move ? "\n- Move" : "\n- Freeze");
						break;
					case SDLK_w: State.display_swarm = !State.display_swarm;
						break;
					case SDLK_e: State.display_line = !State.display_line;
						break;
					case SDLK_l: State.update_line = !State.update_line;
						break;
					case SDLK_f: main_swarm.Toggle_Fill();
						std::cout << "\n- Toggle Fill";
						break;
					case SDLK_c: Input.print_step = true;
						break;
					case SDLK_d: Input.print_denisty = true;
						break;
					case SDLK_x: Input.clear_csv = true;
						break;
					case SDLK_u: 
						params.Read();
						params.Print();
						main_swarm.SetBurnRadius(params.burn_radius_cross);
						main_swarm.SetFillGrid(params.particles_dist);
						main_swarm.m_front_line.Init(); 

						screen.calc_refract_points();
						std::cout << main_swarm.size << " - size\n";
						break;
				}
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
			burn_x = params.screen_to_area_x(mouse_x);
			burn_y = params.screen_to_area_y(mouse_y);
			//printf("\n%i\t%i", mouse_x, mouse_y);
			//printf("\n%f\t%f", burn_x, burn_y);
			segment = main_swarm.GetSegment(burn_x, burn_y);

		}

		/* --- MAIN ACTION --- */

		//if (!State.pause || Input.step)
		{
			/*if (even = !even){

				screen.draw_circles(main_swarm.all_will_burn);
				screen.update();
			} else*/


			//for (int iterate = params.iterations; iterate; --iterate)
			{
				if (even || !State.pause)
				{


					if (!State.pause || Input.step)
					{

						if (State.move) {
							main_swarm.MoveParticles();
							main_swarm.Fill();
						}

						main_swarm.UpdateSegments();
					}

				} 
				if (!even || !State.pause) {

					//if (!State.pause || Input.step) main_swarm.UpdateSegments();

					//if (iterate == params.iterations) 
					{
						if (Input.set_burn) main_swarm.BurnSegment(segment);
						if (Input.lights_out) main_swarm.LightsOut();
						if (Input.clear) main_swarm.Clear();
					}

					if (!State.pause || Input.step)
					{

						main_swarm.CrossParticles();

						//main_swarm.RefractParticles();

						main_swarm.FinalLoop();
					}

				}


				/*main_swarm.CrossParticles();
				main_swarm.StepParticles();

				if (State.move)
				{
					main_swarm.MoveParticles();
					main_swarm.Fill_Sampling();
				}

				main_swarm.ClearParticleList();*/
			}

		}

		if (!State.pause || Input.step || key_pressed) {
			screen.clear();
			screen.draw_grid(main_swarm.grid_count_x, main_swarm.grid_count_z);

			if (State.display_swarm) screen.load_swarm(main_swarm.all_list, State.bold_points);
			//screen.box_blur();

			if (State.update_line && !State.pause || !even ) main_swarm.CalcLine();
			if (State.display_line) screen.draw_frontline(main_swarm.m_front_line.front_line_points, main_swarm.m_front_line.steps);
			screen.draw_refract_line();
			screen.update();
		}


		//}

		if (Input.clear_csv)
		{
			clear_csv_files(params);
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

void clear_csv_files(const ps::Params& P) {
	bool gas_out = 0, line_out = 0;

	for (int i = 0;; ++i)
	{
		if (!gas_out && std::remove((P.csv_folder + "gas.csv." + std::to_string(i)).c_str()) != 0) {
			gas_out = 1;
		}
		if (!line_out && std::remove((P.csv_folder + "line.csv." + std::to_string(i)).c_str()) != 0) {
			line_out = 1;
		}

		if (gas_out && line_out) return;

	}
}




