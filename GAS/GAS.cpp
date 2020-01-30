#include <iostream>
#include <fstream>
#include <ctime> 
#include "Segments.h"



int main() {

	//std::cout << P::particles_sum;
	//return 0;

	//remove all files from csv directory
	bool gas_out = 0, line_out = 0;

	for (size_t i = 0;; ++i) 
	{
		if (!gas_out && std::remove((P::csv_folder + "gas.csv." + std::to_string(i)).c_str()) != 0) {
			gas_out = 1;
		}
		if (!line_out && std::remove((P::csv_folder + "line.csv." + std::to_string(i)).c_str()) != 0) {
			line_out = 1;
		}

		if (gas_out && line_out) break;

	}


	ps::Segments main_swarm = ps::Segments();
	//main_swarm.Test(10);

	//auto first_partile = main_swarm.m_particle_list.begin();
	//double point;

	double start_time = clock();


	for (size_t i = 0; i < P::steps; i++)
	{

		main_swarm.Fill();

		if (i == P::burn_at_step) {
			main_swarm.Lighter();
		}

		main_swarm.Step();

		//if (i % 10 == 0) 
		{
			main_swarm.PrintStep(i);
			//main_swarm.PrintStep(i / 10);
		}


		std::cout << i << '\n';
	}

	std::cout << clock() - start_time;
}
