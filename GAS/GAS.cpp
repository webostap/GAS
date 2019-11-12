// GAS.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include "Swarm.h"



int main() {



	ps::Swarm::Params swarm_params;
	swarm_params.csvFolder = "C:/Users/prive/Documents/csv/";





	//remove all files from csv directory
	for (size_t i = 0; ; ++i) {
		if (std::remove((swarm_params.csvFolder + "gas.csv." + std::to_string(i)).c_str()) != 0) {
			std::cout << i;
			break;
		}
	}


	ps::Swarm main_swarm = ps::Swarm(swarm_params);
	//main_swarm.Test(10);

	//auto first_partile = main_swarm.m_particle_list.begin();


	for (size_t i = 0; i < 300; i++)
	{

		main_swarm.Step();
		main_swarm.PrintStep(i);

		if (i == 20) {

			(*main_swarm.m_particle_list.begin()).setBurn();
			main_swarm.m_burn_list.push_back(&*(main_swarm.m_particle_list.begin()));
		}

		if (!main_swarm.m_particle_list.size()) break;

		//std::cout << main_swarm.m_burn_list.size();
		//std::cin.get();
	}
}
