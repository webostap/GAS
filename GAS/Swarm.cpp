#include "Swarm.h"
#include <string>
#include <fmt/format.h>
#include <omp.h>



namespace ps {

	Swarm::Swarm() {}

	const void Swarm::PrintStep(size_t num)
	{
		//if (!m_particle_list.size()) return;

		std::ofstream csv(P::csv_folder + "gas.csv." + std::to_string(num));

		std::string output = "x,y,burn";

		//double print_from = m_burn_from - P::particle_speed.max;

		for (auto& particle : m_particle_list) {
			//if (particle._x() >= print_from) {
				output += fmt::format("\n{},{},{}", particle._x(), particle._y(), particle.m_burn_counter);
			//}
		}
		csv << output;
		csv.close();

		m_front_line.Print(num);
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
		std::uniform_real_distribution<double> y_dist(P::area_beg, P::area_end);
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

	void Swarm::UpdateBurnFrom() {

		if (!m_burn_list.empty()) {
			m_burn_from = m_burn_list.front()->_x();
		}
		for (auto& burn_i : m_burn_list) {
			if (burn_i->_x() < m_burn_from) {
				m_burn_from = burn_i->_x();
			}
		}

	}

	void Swarm::StepParticle(std::list<Particle>::iterator&p) {
		//if particle doesn't burn check cross with burned particles
		if ((*p).getState() == Particle::State::OK && (*p)._x() >= m_burn_from - P::burn_radius) {
			for (auto& burn_it : m_burn_list)
				if ((*p).Cross(*burn_it)) {
					(*p).setBurn();
					m_will_burn_list.push_back(&(*p));
					break;
				}
		}

		(*p).Step();

		if ((*p).getState() == Particle::State::DIED) {
			m_died_list.push_back(p);
		}
	}


	void Swarm::Step() {


		UpdateBurnFrom();



		//size_t step_num = 0;

		for (auto it = m_particle_list.begin(); it != m_particle_list.end(); ++it)
		{
			StepParticle(it);
		}



		m_front_line.Calc(m_will_burn_list);


		ClearBurnList();
		ClearParticleList();

		m_burn_list.merge(m_will_burn_list);

		//PrintStep(step_num);
		//++step_num;
	}




} /* namespace ps */
