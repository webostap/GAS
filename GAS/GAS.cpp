#include <iostream>
#include <fstream>
#include <string>
//#include <ctime> 
//#include <cstdlib>

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

struct state {
	bool move = true,
		pause = 0,
		display_swarm = true,
		display_line = true,
		bold_points = false,
		blur = false,
		update_line = true;

};


#undef main
int main() {
	
	ps::Params params;

	ps::Segments main_swarm(params);
	ps::FrontLine front_line(params);
	ps::Screen screen(params);

	ps::Segments::Segment* segment = main_swarm.GetSegment(0, 0);

	params.Print();




	state State;
	input Input;
	bool key_pressed = false;


	Uint32 startTime = 0;
	Uint32 endTime = 0;
	Uint32 delta = 0;
	short fps = 30;
	short timePerFrame = 1000 / fps; // miliseconds
	SDL_Event e;
	//const Uint8* key_state;

	char buffer[] = "FPS: 00";


		
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



	
	int ii = -1;
	while (!Input.sdl_quit) {
		++ii;
		startTime = SDL_GetTicks();



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
				switch (e.key.keysym.sym)
				{
					case SDLK_1: params.SetStream(1); break;
					case SDLK_2: params.SetStream(2); break;
					case SDLK_3: params.SetStream(3); break;
					case SDLK_4: params.SetStream(4); break;
					case SDLK_SPACE: Input.lights_out = true; key_pressed = 1; break;
					case SDLK_DELETE: Input.clear = true; key_pressed = 1; break;
					case SDLK_p: State.pause = !State.pause;
						std::cout << (State.pause ? "\n- Pause" : "\n- Play");
						break;
					case SDLK_s: Input.step = true; key_pressed = 1; break;
					case SDLK_q: State.bold_points = !State.bold_points; key_pressed = 1; break;
					case SDLK_b: State.blur = !State.blur; key_pressed = 1; break;
					case SDLK_m: State.move = !State.move;
						std::cout << (State.move ? "\n- Move" : "\n- Freeze");
						break;
					case SDLK_w: State.display_swarm = !State.display_swarm; key_pressed = 1; break;
					case SDLK_e: State.display_line = !State.display_line; key_pressed = 1; break;
					case SDLK_l: State.update_line = !State.update_line; key_pressed = 1; break;
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
						key_pressed = 1;
						params.Read();
						params.Print();
						main_swarm.Update();
						front_line.Init();
						screen.calc_refract_points();
						std::cout << main_swarm.size() << " - size\n";
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
			key_pressed = true;
			Input.set_burn = true;
			burn_x = params.screen_to_area_x(mouse_x);
			burn_y = params.screen_to_area_y(mouse_y);
			segment = main_swarm.GetSegment(burn_x, burn_y);

		}

		/* --- MAIN ACTION --- */

		
		
		/*if (even = !even){

			screen.draw_circles(main_swarm.all_will_burn);
			screen.update();
		} else*/

		//for (int iterate = params.iterations; iterate; --iterate)
		{

			
			if (!State.pause || Input.step ) {
				if (State.move) {
					main_swarm.MoveParticles();
					main_swarm.Fill();
				}
				main_swarm.UpdateSegments();
			}

			if (Input.set_burn) main_swarm.BurnSegment(segment);
			if (Input.lights_out) main_swarm.LightsOut();
			if (Input.clear) main_swarm.Clear();

			if (!State.pause || Input.step) {
				main_swarm.CrossParticles();
				main_swarm.FinalLoop();
			}

		}



		if (!State.pause || Input.step || key_pressed)
		{

			screen.clear();
			if (State.display_swarm) screen.load_swarm(main_swarm.all_list, State.bold_points);
			if (State.blur) screen.box_blur();
			screen.UpdateTexture();

			screen.draw_grid(main_swarm.grid_count_x, main_swarm.grid_count_z);


			if (State.display_line) {
				if (State.update_line) front_line.Calc(main_swarm.all_will_burn);
				screen.draw_frontline(front_line.front_line_points, front_line.steps);
			}

			//screen.draw_refract_line();
			screen.Render();
		}



		if (Input.clear_csv)
		{
			clear_csv_files(params);
			print_step_counter = 0;
			std::cout << "\nclear files";
		}


		if (Input.print_denisty) {
			main_swarm.Density_Grid();
			main_swarm.Density_Radius();
			main_swarm.Max_Radius();
			std::cout << "\nprint - denisty";
		}


		if (Input.print_step)
		{
			main_swarm.Print(print_step_counter);
			front_line.Print(print_step_counter);
			std::cout << "\nprint - " << print_step_counter;
			++print_step_counter;
		}

		/* --- END OF MAIN ACTION --- */

		endTime = SDL_GetTicks();
		delta = endTime - startTime;

		startTime = endTime;

		if (delta < timePerFrame) {
			SDL_Delay(timePerFrame - delta);
			fps = 30;
		}
		else fps = 1000 / delta;


		if (ii % 10 == 0)
		{
			sprintf_s(buffer, "FPS: %d", fps);
			screen.SetTitle(buffer);
			//std::cout << buffer;
		}


	}

	return 0;


}


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




