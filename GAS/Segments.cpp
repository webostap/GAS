#include "Segments.h"
#include <string>
#include <fmt/format.h>
#include <cmath>



namespace ps {

	Segments::Segments() 
	{
		segment_beg[0] = P::area_beg;
		segment_end[0] = P::area_beg + P::segment_size;

		segment_cross_beg[0] = P::area_beg;
		segment_cross_end[0] = segment_end[0] - P::burn_radius;

		for (size_t si = 1; si < P::segment_count; si++)
		{
			segment_beg[si] = segment_beg[si - 1] + P::segment_size;
			segment_end[si] = segment_end[si - 1] + P::segment_size;

			segment_cross_beg[si] = segment_beg[si] + P::burn_radius;
			segment_cross_end[si] = segment_end[si] - P::burn_radius;
		}

		segment_cross_end[P::segment_count-1] = P::area_end;

	}

	const void Segments::PrintStep(size_t num)
	{

		std::ofstream csv(P::csv_folder + "gas.csv." + std::to_string(num));

		std::string output = "x,y,burn";

	
		for (auto& particle : all_list) {
			output += fmt::format("\n{},{},{}", particle._x(), particle._y(), particle.m_burn_counter);
		}
		
		csv << output;
		csv.close();


		m_front_line.Calc(all_will_burn);
		m_front_line.Print(num);

	}

	void Segments::Fill() {

		std::random_device rd_x;
		std::random_device rd_y;
		std::uniform_real_distribution<double> y_dist;
		std::uniform_real_distribution<double> x_dist;

		unsigned particles_at_step = P::base_particles;
		double particles_double = P::base_particles;
		double count_multipler = P::base_particles * P::center_speed_increase / (P::segment_count / 2);

		double window_beg = P::area_beg;
		double window_end = P::area_beg + P::segment_size;
		double p_y_cord, p_x_cord, p_speed, p_center_percentage;

		for (size_t si = 0; si < P::segment_count; si++)
		{

			y_dist = std::uniform_real_distribution<double> (window_beg, window_end);

			for (size_t pi = 0; pi < particles_at_step; ++pi) {

				p_y_cord = y_dist(rd_y);

				p_center_percentage = (1 - abs(1 - (p_y_cord - P::area_beg) / P::area_size * 2));
				p_speed = P::base_speed * (1 + p_center_percentage * P::center_speed_increase);

				x_dist = std::uniform_real_distribution<double> (0, p_speed);
				p_x_cord = x_dist(rd_x);
				
				all_list.emplace_back(p_x_cord, p_y_cord, p_speed);
			}

			window_beg += P::segment_size;
			window_end += P::segment_size;

			particles_double += (si < P::segment_count / 2) ? count_multipler : -count_multipler;
			particles_at_step = round(particles_double);
		}

	}
	

	void Segments::UpdateBurnFrom() {


		for (size_t si = 0; si < P::segment_count; si++)
		{ //walk over all segments

			burn_from[si] = burn_list[si].empty() ? 0 : burn_list[si].front()->_x();

			for (auto& burn_i : burn_list[si]) {
				if (burn_i->_x() < burn_from[si]) {
					burn_from[si] = burn_i->_x();
				}
			}
			burn_from[si]-= P::burn_radius;
		}

	}

	/*void Segments::StepParticles()
	{
		for (size_t si = 0; si < P::segment_count; si++)
		{ //walk over all segments
			for (auto& particle_i : particle_list[si])
			{ //walk over all particles in segment
				if (particle_i.getState() == Particle::State::OK)
				{ //if particle doesn't burn
					if (particle_i._x() >= burn_from[si])
					{ //if particle being close to burn line
						for (auto& burn_i : burn_list[si])
						{ //walk over all burning particles in current segment
							if (particle_i.Cross(*burn_i))
							{ //if particle cross with burning particle
								BurnParticle(si, particle_i);
								break;
							}
						}
					}
				}
				particle_i.Step();

			} //walk over all particles in segment
		}
	}*/

	void Segments::StepParticlesAll()
	{
		//for (size_t si = 0; si < P::segment_count; si++)
		{ //walk over all segments
			for (auto& particle_i : all_list)
			{ //walk over all particles in segment
				if (particle_i.getState() == Particle::State::OK)
				{ //if particle doesn't burn
					//if (particle_i._x() >= burn_from[si])
					{ //if particle being close to burn line
						for (auto& burn_i : all_burn)
						{ //walk over all burning particles in current segment
							if (particle_i.Cross(*burn_i))
							{ //if particle cross with burning particle
								BurnParticleAll(particle_i);
								break;
							}
						}
					}
				}
				particle_i.Step();

			} //walk over all particles in segment
		}
	}
	void Segments::StepParticles()
	{
		unsigned segment_index; //walk over all segments

		for (auto& particle_i : all_list)
		{ //walk over all particles in segment
			if (particle_i.getState() == Particle::State::OK)
			{ //if particle doesn't burn
				segment_index = GetSegmentIndex(particle_i);
				if (particle_i._x() >= burn_from[segment_index])
				{ //if particle being close to burn line
					for (auto& burn_i : burn_list[segment_index])
					{ //walk over all burning particles in current segment
						if (particle_i.Cross(*burn_i))
						{ //if particle cross with burning particle
							BurnParticle(particle_i);
							break;
						}
					}
				}
			}
			particle_i.Step();

		}
	}


	void Segments::ClearBurnList() {
		
		for (size_t si = 0; si < P::segment_count; si++)
		{
			if (!burn_list[si].empty())
			{
				auto burn_it = burn_list[si].begin();
				auto died_it = burn_it;
				while (burn_it != burn_list[si].end()) {
					if ((*burn_it)->getState() == ps::Particle::State::DIED) {
						died_it = burn_it;
						++burn_it;
						burn_list[si].erase(died_it);
					}
					else ++burn_it;
				}
			}

			burn_list[si].splice(burn_list[si].end(), will_burn_list[si]);
		}

	}
	void Segments::ClearBurnListAll() {

		if (!all_burn.empty())
		{
			auto burn_it = all_burn.begin();
			auto died_it = burn_it;
			while (burn_it != all_burn.end()) {
				if ((*burn_it)->getState() == ps::Particle::State::DIED) {
					died_it = burn_it;
					++burn_it;
					all_burn.erase(died_it);
				}
				else ++burn_it;
			}
		}
		all_burn.splice(all_burn.end(), all_will_burn);

	}

	void Segments::ClearParticleList() {

		auto particle_it = all_list.begin();
		auto died_it = particle_it;
		while (particle_it != all_list.end()) {
			if ((*particle_it).getState() == ps::Particle::State::DIED) {
				died_it = particle_it;
				++particle_it;
				all_list.erase(died_it);
			}
			else ++particle_it;
		}

	}



	unsigned Segments::GetSegmentIndex(Particle& p)
	{
		return floor((p._y() - P::area_beg) / P::area_size * P::segment_count);
	}

	void Segments::LighterAll() {
		int i = 0;
		for (auto& particle_i : all_list) {
			BurnParticleAll(particle_i);
			if (++i == 10) break;
		}

	}
	void Segments::Lighter() {
		int i = 0, limit = P::particles_sum;
		for (auto& particle_i : all_list) {
			BurnParticle(particle_i);
			if (++i == limit) break;
		}

	}


	void Segments::BurnParticleAll(Particle& particle) {

		particle.setBurn();

		all_will_burn.push_back(&particle);

	}

	void Segments::BurnParticle(Particle& particle) {

		particle.setBurn();

		unsigned segment_index = GetSegmentIndex(particle);

		will_burn_list[segment_index].push_back(&particle);
		all_will_burn.push_back(&particle);

		if (particle._y() <= segment_cross_beg[segment_index]) {
			will_burn_list[segment_index - 1].push_back(&particle);
		}
		if (particle._y() >= segment_cross_end[segment_index]) {
			will_burn_list[segment_index + 1].push_back(&particle);
		}

	}

	

	void Segments::Step() {

		all_will_burn.clear();

		UpdateBurnFrom();

		StepParticles();

		ClearBurnList();

		ClearParticleList();


	}

	void Segments::StepAll() {

		//UpdateBurnFrom();

		StepParticlesAll();

		ClearBurnListAll();

		ClearParticleList();

	}

}