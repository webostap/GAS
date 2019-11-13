#include "Swarm.h"
#include <fstream>


namespace ps {

	Swarm::Swarm(Params aParams) : m_params(aParams) {}

	const void Swarm::PrintStep(size_t num)
	{
		//if (!m_particle_list.size()) return;

		std::ofstream csv(m_params.csvFolder + "gas.csv." + std::to_string(num));

		csv << "x,y,burn";

		for (auto& particle : m_particle_list) {
			csv << '\n' << particle._x() << ','
				<< particle._y() << ','
				<< particle.m_burn_counter;
		}

		csv.close();
	}
	
	void Swarm::Test(size_t size) {

		std::random_device rd;
		std::uniform_real_distribution<double> y_dist(0, 1);

		for (size_t i = 0; i < size; i++)
			for (size_t j = 0; j < size; j++)
				m_particle_list.emplace_back(0, y_dist(rd));
	}

	void Swarm::Erasing() {
		for (size_t i = 0; i < 10; i++)
			for (size_t j = 0; j < 10; j++)
				m_particle_list.emplace_back(i,j);

		for (auto it = m_particle_list.begin(); it != m_particle_list.end(); ++it)
			m_died_list.push_back(it);
	
		ClearParticleList();
	}


	void Swarm::Fill(size_t count) {
		std::random_device rd;
		std::uniform_real_distribution<double> y_dist(0, 0.2);
		for (size_t i = 0; i < count; ++i) {
			m_particle_list.emplace_back(0, y_dist(rd));
		}
	}

	size_t Swarm::size() {
		return m_particle_list.size();
	}



	void Swarm::ClearParticleList() {

		for (auto& it : m_died_list) {
			m_particle_list.erase(it);
		}

		m_died_list.clear();
	}


	void Swarm::ClearBurnList() {
		if (m_died_list.empty()) return;

		auto burn_it = m_burn_list.begin();
		auto died_it = burn_it;
		while (burn_it != m_burn_list.end()) {
			if ((*burn_it)->getState() == ps::Particle::State::DIED) {
				died_it = burn_it;
				++burn_it;
				m_burn_list.erase(died_it);
			}
			else ++burn_it;
		}

	}


	void Swarm::Step() {


		std::random_device rd;
		std::uniform_int_distribution<int> count_dist(100, 200);

		Fill(count_dist(rd));


		size_t step_num = 0;
		for (auto it = m_particle_list.begin(); it != m_particle_list.end(); ++it)
		{

			//if particle doesn't burn check cross with burned particles
			if ((*it).getState() == Particle::State::OK) {
				for (auto& burn_it : m_burn_list)
					if ((*it).Cross(*burn_it)) {
						(*it).setBurn();
						m_will_burn_list.push_back(&*it);
						break;
					}
			}

			(*it).Step();

			if ((*it).getState() == Particle::State::DIED) {
				m_died_list.push_back(it);
			}

		}
		ClearBurnList();
		ClearParticleList();

		m_burn_list.merge(m_will_burn_list);

		//PrintStep(step_num);
		//++step_num;
	}




} /* namespace ps */
