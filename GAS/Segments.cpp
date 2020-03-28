#include "Segments.h"
#include <string>
#include <fmt/format.h>
#include <cmath>



namespace ps {

	Segments::Segments() 
	{

		for (size_t i = 0; i < P::grid_count_x; i++) {
			grid[i] = (grid_mem + i * P::grid_count_z);
		}







		segment_beg[0] = P::area_beg;
		segment_end[0] = P::area_beg + P::burn_index_window;

		segment_cross_beg[0] = P::area_beg;
		segment_cross_end[0] = segment_end[0] - P::burn_radius;

		for (size_t si = 1; si < P::burn_index_steps; si++)
		{
			segment_beg[si] = segment_beg[si - 1] + P::burn_index_window;
			segment_end[si] = segment_end[si - 1] + P::burn_index_window;

			segment_cross_beg[si] = segment_beg[si] + P::burn_radius;
			segment_cross_end[si] = segment_end[si] - P::burn_radius;
		}

		segment_cross_end[P::burn_index_steps -1] = P::area_end;

	}

	const void Segments::PrintStep(size_t num)
	{

		std::ofstream csv(P::csv_folder + "gas.csv." + std::to_string(num));

		std::string output = "x,z,burn";

	
		for (auto& particle : all_list) {
			output += fmt::format("\n{},{},{}", particle._x(), particle._z(), particle.burn_counter);
		}
		
		csv << output;
		csv.close();


		m_front_line.Calc(all_will_burn);
		m_front_line.Print(num);

	}

	void Segments::Fill() {

		std::random_device rd;
		std::uniform_real_distribution<double> x_dist, z_dist;

		double particles_double; 
		int particles_at_step;

		double window_beg = P::area_beg;
		double window_end = P::area_beg + P::segment_size;
		double p_x_cord, p_z_cord, p_speed;

		for (size_t si = 0; si < P::segment_count; si++)
		{

			particles_double = P::particle_count(window_beg + P::segment_size/2.);
			particles_at_step = round(particles_double);

			x_dist = std::uniform_real_distribution<double> (window_beg, window_end);

			for (size_t pi = 0; pi < particles_at_step; ++pi) {

				p_x_cord = x_dist(rd);

				p_speed = P::particle_speed(p_x_cord);

				z_dist = std::uniform_real_distribution<double> (0, p_speed);
				p_z_cord = z_dist(rd);
				
				all_list.emplace_back(p_x_cord, p_z_cord, p_speed);

				if (P::edge_burners && (si == 0 || si == P::segment_count - 1)) {
					BurnParticle(all_list.back());
				}
			}

			window_beg += P::segment_size;
			window_end += P::segment_size;

			
		}

	}
	void Segments::Fill_2() {

		std::random_device rd;
		std::uniform_real_distribution<double> dist_x(P::area_beg, P::area_end), dist_z(0, P::particle_speed(P::area_center));

		double p_x_cord, p_z_cord, p_speed;

		for (size_t pi = 0; pi < P::base_particles; ++pi) {

			p_x_cord = dist_x(rd);
			p_z_cord = dist_z(rd);
			p_speed = P::particle_speed(p_x_cord);

			if (p_z_cord < p_speed) {
				all_list.emplace_back(p_x_cord, p_z_cord, p_speed);
			}
			else --pi;

		}

	}
	

	void Segments::UpdateBurnFrom() {


		for (size_t si = 0; si < P::burn_index_steps; si++)
		{ //walk over all segments

			burn_from[si] = burn_list[si].empty() ? P::area_height : burn_list[si].front()->_z();

			for (auto& burn_i : burn_list[si]) {
				if (burn_i->_z() < burn_from[si]) {
					burn_from[si] = burn_i->_z();
				}
			}
			burn_from[si]-= P::burn_radius;
		}

	}


	void Segments::BurnParticles()
	{
		unsigned burn_index; //walk over all segments

		for (auto& particle_i : all_list)
		{ //walk over all particles in segment
			if (particle_i.getState() == Particle::State::OK)
			{ //if particle doesn't burn
				burn_index = GetSegmentIndex(particle_i);

				if (particle_i._z() >= burn_from[burn_index])
				{ //if particle being close to burn line
					for (auto& burn_i : burn_list[burn_index])
					{ //walk over all burning particles in current segment
						if (/*(*burn_i).m_burn_counter > 1 && */particle_i.Cross(*burn_i))
						{ //if particle cross with burning particle
							BurnParticle(particle_i);
							break;
						}
					}
				}
			}
		}
	}

	void Segments::BurnParticles_2()
	{

		//for (auto& p : all_list)
		//{
			//ParticleInBurnSegment(p);
		//}

		if (burn_segments.empty()) return;

		
		for (size_t j = 0; j < P::grid_count_z; j++)
		{
			for (size_t i = 0; i < P::grid_count_x; i++)
			{
				if (CheckSegmentBurn(i, j))
				{
					for (auto& particle_i : grid[i][j].ok_list)
					{
						ParticleInBurnSegment(*particle_i, i, j);
					}
				}
			}
		}
		
		
		/*for (auto& burn_segment : burn_segments)
		{

			int seg_x = burn_segment.first;
			int seg_z = burn_segment.second;

			for (auto& particle_i : grid[seg_x][seg_z].ok_list)
			{
				ParticleInBurnSegment(*particle_i, seg_x, seg_z);
			}
		}*/

		/*for (size_t j = 0; j < P::grid_count_z; j++)
		{
			for (size_t i = 0; i < P::grid_count_x; i++)
			{
				for (auto& burn_i : grid[i][j].burn_list)
				{
					ParticleInBurnSegment_2(*burn_i, i, j);
				}
			}
		}*/
	}


	bool Segments::CheckSegmentBurn(int seg_x, int seg_z)
	{

		for (size_t i = seg_x ? seg_x - 1 : 0; i < (seg_x < P::grid_count_x - 1 ? seg_x + 2 : P::grid_count_x); i++)
		{
			for (size_t j = seg_z ? seg_z - 1 : 0; j < (seg_z < P::grid_count_z - 1 ? seg_z + 2 : P::grid_count_z); j++)
			{
				if (grid[i][j].has_burn)
				{
					return true;
				}
			}
		}
		return false;

	}

	void Segments::ParticleInBurnSegment_2(Particle& burn_i, int seg_x, int seg_z)
	{
		for (size_t i = seg_x ? seg_x - 1 : 0; i < (seg_x < P::grid_count_x - 1 ? seg_x + 2 : P::grid_count_x); i++)
		{
			for (size_t j = seg_z ? seg_z - 1 : 0; j < (seg_z < P::grid_count_z - 1 ? seg_z + 2 : P::grid_count_z); j++)
			{
				for (auto& particle_i : grid[i][j].ok_list)
				{
					if (burn_i.Cross(*particle_i))
					{
						BurnParticle_2(*particle_i);
					}
				}
			}
		}
	}
	void Segments::ParticleInBurnSegment(Particle& particle, int seg_x, int seg_z)
	{
		for (size_t i = seg_x ? seg_x - 1 : 0; i < (seg_x < P::grid_count_x - 1 ? seg_x + 2 : P::grid_count_x); i++)
		{
			for (size_t j = seg_z ? seg_z - 1 : 0; j < (seg_z < P::grid_count_z - 1 ? seg_z + 2 : P::grid_count_z); j++)
			{
				if (grid[i][j].has_burn) for (auto& burn_i : grid[i][j].burn_list)
				{
					if (particle.Cross(*burn_i))
					{
						BurnParticle_2(particle);
						return;
					}
				}
			}
		}
	}
	void Segments::ParticleInBurnSegment(Particle& p)
	{
		for (size_t i = p.seg_x ? p.seg_x - 1 : 0; i < (p.seg_x < P::grid_count_x - 1 ? p.seg_x + 2 : P::grid_count_x); i++)
		{
			for (size_t j = p.seg_z ? p.seg_z - 1 : 0; j < (p.seg_z < P::grid_count_z - 1 ? p.seg_z + 2 : P::grid_count_z); j++)
			{
				for (auto& burn_i : grid[i][j].burn_list)
				{
					if (p.Cross(*burn_i))
					{
						BurnParticle_2(p);
						return;
					}
				}
			}
		}
	}

	void Segments::MoveParticles()
	{
		for (auto& particle_i : all_list) { 

			particle_i.Step();
			particle_i.Move();
		}
	}


	void Segments::ClearBurnList() {
		
		for (size_t si = 0; si < P::burn_index_steps; si++)
		{
			if (!burn_list[si].empty())
			{
				auto burn_it = burn_list[si].begin();
				auto died_it = burn_it;
				while (burn_it != burn_list[si].end()) {
					if ((*burn_it)->getState() != Particle::State::BURN) {
						died_it = burn_it;
						++burn_it;
						burn_list[si].erase(died_it);
					}
					else ++burn_it;
				}
			}
			if (!will_burn_list[si].empty())
			{
				auto burn_it = will_burn_list[si].begin();
				auto died_it = burn_it;
				while (burn_it != will_burn_list[si].end()) {
					if ((*burn_it)->getState() != Particle::State::BURN) {
						died_it = burn_it;
						++burn_it;
						will_burn_list[si].erase(died_it);
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
			if ((*particle_it).getState() == Particle::State::DIED) {
				died_it = particle_it;
				++particle_it;
				all_list.erase(died_it);
			}
			else ++particle_it;
		}

	}



	unsigned Segments::GetSegmentIndex(Particle& p)
	{
		return floor((p._x() - P::area_beg) / P::area_size * P::burn_index_steps);
	}

	unsigned Segments::GetParticleSegmentX(Particle& p)
	{
		return floor(p.x * P::grid_count_x_percent);
	}
	unsigned Segments::GetParticleSegmentZ(Particle& p)
	{
		return floor(p.z * P::grid_count_z_percent);
	}



	void Segments::UpdateSegments()
	{
		burn_segments.clear();

		for (size_t i = 0; i < P::grid_count; i++)
		{
			grid_mem[i].has_burn = false;
			grid_mem[i].burn_list.clear();
			grid_mem[i].ok_list.clear();
		}

		for (auto& p : all_list) {
			unsigned seg_x = GetParticleSegmentX(p);
			unsigned seg_z = GetParticleSegmentZ(p);

			if (p.isBurn()) {
				if (!grid[seg_x][seg_z].has_burn) {
					burn_segments.emplace_back(seg_x,seg_z);
				}
				grid[seg_x][seg_z].has_burn = 1;
				grid[seg_x][seg_z].burn_list.push_back(&p);
			}
			else grid[seg_x][seg_z].ok_list.push_back(&p);
		}
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

		unsigned burn_index = GetSegmentIndex(particle);

		will_burn_list[burn_index].push_back(&particle);
		all_will_burn.push_back(&particle);

		if (particle._x() <= segment_cross_beg[burn_index]) {
			will_burn_list[burn_index - 1].push_back(&particle);
		}
		if (particle._x() >= segment_cross_end[burn_index]) {
			will_burn_list[burn_index + 1].push_back(&particle);
		}

	}
	void Segments::BurnParticle_2(Particle& particle) {
		particle.setBurn();
		all_will_burn.push_back(&particle);
	}

	

	void Segments::Step() {

		all_will_burn.clear();

		//UpdateBurnFrom();

		UpdateSegments();

		BurnParticles_2();

		MoveParticles();

		//ClearBurnList();

		ClearParticleList();

		Fill_2();
	}

}