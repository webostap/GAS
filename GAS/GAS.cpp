// GAS.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

// compile with: /openmp
#include <iostream>
#include <fstream>
#include <omp.h>
#include "Swarm.h"



int main() {






	//remove all files from csv directory
	bool one_out = 0;
	for (size_t i = 0; ; ++i) {
		if (std::remove((P::csv_folder + "gas.csv." + std::to_string(i)).c_str()) != 0 ||
			std::remove((P::csv_folder + "line.csv." + std::to_string(i)).c_str()) != 0) {
			if (one_out) break;
			else one_out = 1;
		}
	}


	ps::Swarm main_swarm = ps::Swarm();
	//main_swarm.Test(10);

	//auto first_partile = main_swarm.m_particle_list.begin();


	for (size_t i = 0; i < P::steps; i++)
	{


		std::random_device rd;
		std::uniform_int_distribution<int> count_dist(P::particles_at_step.min, P::particles_at_step.max);

		//if (!i) 
			main_swarm.Fill(count_dist(rd));

		//main_swarm.PrintStep(i);

		main_swarm.Step();
		main_swarm.PrintStep(i);

		if (i == P::burn_at_step) {

			(*main_swarm.m_particle_list.begin()).setBurn();
			main_swarm.m_burn_list.push_back(&*(main_swarm.m_particle_list.begin()));
		}

		if (!main_swarm.m_particle_list.size()) break;

		std::cout << i << ") " << main_swarm.m_particle_list.size() << '\n';
		//if (i%10 == 0) std::cin.get();
		//std::cout << i << '\n';
	}
}
