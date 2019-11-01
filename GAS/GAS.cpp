// GAS.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Swarm.h"


int main()
{

	ps::Swarm main_swarm = ps::Swarm();

	
	main_swarm.Test(100);

	auto first_partile = main_swarm.m_particle_list.begin();
	(*first_partile).setBurn();
	main_swarm.m_burn_list.push_back(&*first_partile);


	for (size_t i = 0; i < 100; i++)
	{
		main_swarm.Step();

		
		std::cout << main_swarm.m_burn_list.size();
		//std::cin.get();
	}
}
