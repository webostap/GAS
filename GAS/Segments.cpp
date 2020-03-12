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

		double particles_double = P::base_particles;
		unsigned particles_at_step = round(particles_double);
		double count_multipler = P::base_particles * P::center_speed_increase / (P::segment_count / 2);

		double window_beg = P::area_beg;
		double window_end = P::area_beg + P::segment_size;
		double p_y_cord, p_x_cord, p_speed;

		for (size_t si = 0; si < P::segment_count; si++)
		{

			particles_double = P::speed_distribution(window_beg + P::segment_size/2)*P::base_particles;
			particles_at_step = round(particles_double * particles_double + 5);

			y_dist = std::uniform_real_distribution<double> (window_beg, window_end);

			for (size_t pi = 0; pi < particles_at_step; ++pi) {

				p_y_cord = y_dist(rd_y);

				p_speed = P::speed_distribution(p_y_cord);

				x_dist = std::uniform_real_distribution<double> (0, p_speed);
				p_x_cord = x_dist(rd_x);
				
				all_list.emplace_back(p_x_cord, p_y_cord, p_speed);

				if (P::edge_burners && (si == 0 || si == P::segment_count - 1))
				{
					BurnParticle(all_list.back());
				}
			}

			window_beg += P::segment_size;
			window_end += P::segment_size;

			
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


	void Segments::BurnParticles()
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
						if (/*(*burn_i).m_burn_counter > 1 && */particle_i.Cross(*burn_i))
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
	void Segments::MoveParticles()
	{
		for (auto& particle_i : all_list) { 
			particle_i.Move();
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


	void Segments::Lighter() {
		int i = 0, limit = P::particles_sum;
		for (auto& particle_i : all_list) {
			BurnParticle(particle_i);
			if (++i == limit) break;
		}

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

		BurnParticles();

		ClearBurnList();

		ClearParticleList();

		MoveParticles();

	}

}