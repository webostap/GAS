#include "Swarm.h"
//#include <iostream>

namespace ps {

	Swarm::Swarm() { }
	
	void Swarm::Test(size_t size) {
		for (size_t i = 0; i < size; i++)
			for (size_t j = 0; j < size; j++)
				m_particle_list.emplace_back(i/100.,j/100.);
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
		for (size_t i = 0; i < count; ++i) {
			m_particle_list.emplace_back();
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

	}




} /* namespace ps */
