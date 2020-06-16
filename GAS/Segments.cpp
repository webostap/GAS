#include "Segments.h"
#include <string>
#include <fmt/format.h>
#include <cmath>
#include <iostream>



namespace ps {

	//Segments::Segments(const Params& aParams) : params(aParams)
	Segments::Segments()
	{
		//UpdateParams();

		SetGrid(P::burn_radius);
		SetFillGrid(P::particles_dist);
		Toggle_Fill();

		/*for (int i = 0; i < P::grid_count_x; i++) {
			grid[i] = (grid_mem + i * P::grid_count_z);
		}*/

	}

	void Segments::Toggle_Fill() {

		fill_func = (_fill_one = !_fill_one) ? &Segments::Fill_Sampling : &Segments::Fill_Grid;
	}

	void Segments::Fill() {
		(this->*fill_func)();
	}

	void Segments::SetBurnRadius(double _burn_radius)
	{
		burn_radius = _burn_radius;
		SetGrid(burn_radius);
	}

	void Segments::SetGrid(double seg_size)
	{
		grid_count_x = (int)floor(P::area_size / seg_size);
		grid_count_z = (int)floor(P::area_height / seg_size);
		grid_count = grid_count_x * grid_count_z;

		grid_count_x_percent = grid_count_x / P::area_size;
		grid_count_z_percent = grid_count_z / P::area_height;

		delete[]grid;
		delete[]grid_mem;

		grid = new Segment* [grid_count_x];
		grid_mem = new Segment [grid_count];

		for (size_t i = 0; i < grid_count_x; i++) {
			grid[i] = grid_mem + i * grid_count_z;
		}

	}
	void Segments::SetFillGrid(double dist) {
		particles_dist = dist;
		fill_grid_count = (int)floor(P::area_size / particles_dist);
		delete[]last_particles;
		last_particles = new double[fill_grid_count];
		for (int i = 0; i < fill_grid_count; i++) {
			last_particles[i] = 0;
		}
	}

	/*void Segments::LoadParams(const Params& aParams)
	{
		params = aParams;
		UpdateParams();
	}
	void Segments::UpdateParams()
	{
		iterate_speed = params.base_speed / params.iterations;
		iterate_particles = params.base_particles / params.iterations;
	}*/

	const double Segments::particle_speed(const double x) {
		return P::iterate_speed * P::stream_function(x);
	}

	const void Segments::PrintStep(int num)
	{
		//all_will_burn.clear();

		std::string output;

		output += "#base_particles " + std::to_string(P::base_particles);

		output += "\nx,z,burn";
		for (auto& particle : all_list) {
			output += fmt::format("\n{},{},{}", particle._x(), particle._z(), particle.burn_counter);

			/*if (particle.burn_counter == 1 || particle.getState() == Particle::State::WARM)
			{
				all_will_burn.push_back(&particle);
			}*/
		}

		std::ofstream csv(P::csv_folder + "gas.csv." + std::to_string(num));
		csv << output;
		csv.close();


		/*m_front_line.Calc(all_will_burn);
		m_front_line.Print(num);*/

	}
	const void Segments::PrintLine(int num)
	{

		m_front_line.Calc(all_will_burn);
		m_front_line.Print(num);

	}



	/*void Segments::Fill_Ziggurat() {

		std::random_device rd;
		std::uniform_real_distribution<double> x_dist, z_dist;

		double particles_double; 
		int particles_at_step;

		double window_beg = P::area_beg;
		double window_end = P::area_beg + P::segment_size;
		double p_x_cord, p_z_cord, p_speed;

		for (int si = 0; si < P::segment_count; si++)
		{

			particles_double = P::particle_count(window_beg + P::segment_size/2.);
			particles_at_step = round(particles_double);

			x_dist = std::uniform_real_distribution<double> (window_beg, window_end);

			for (int pi = 0; pi < particles_at_step; ++pi) {

				p_x_cord = x_dist(rd);

				p_speed = P::particle_speed(p_x_cord);

				z_dist = std::uniform_real_distribution<double> (0, p_speed);
				p_z_cord = z_dist(rd);
				
				all_list.emplace_back(p_x_cord, p_z_cord, p_speed);

				//if (P::edge_burners && (si == 0 || si == P::segment_count - 1)) {
					//BurnParticle(all_list.back());
				//}
			}

			window_beg += P::segment_size;
			window_end += P::segment_size;

			
		}

	}*/

	void Segments::Fill_Grid() {

		double x_cord = P::area_beg, x_dist = particles_dist;

		for (int i = 0; i < fill_grid_count; i++)
		{
			double p_speed = P::particle_speed(x_cord);
			for (double z_cord = last_particles[i] - particles_dist; z_cord >= 0; z_cord -= particles_dist)
			{
				all_list.emplace_front(x_cord, z_cord, p_speed, burn_radius);
				last_particles[i] = z_cord;
			}
			x_cord += x_dist;
			last_particles[i]+= p_speed;
		}


	}
	

	void Segments::Fill_Sampling() {

		std::random_device rd;
		std::uniform_real_distribution<double> dist_x(P::area_beg, P::area_end), dist_z(0, P::particle_speed(P::area_center));

		double p_x_cord, p_z_cord, p_speed;

		for (int pi = P::iterate_particles; pi; --pi)
		{
			p_x_cord = dist_x(rd);
			p_z_cord = dist_z(rd);
			p_speed = P::particle_speed(p_x_cord);

			if (p_z_cord < p_speed) {
				all_list.emplace_front(p_x_cord, p_z_cord, p_speed, burn_radius);
			}
		}

	}

	int Segments::Line_Count()
	{
		int sum = 0;
		for (int xi = 0; xi < grid_count_x; xi++)
		{
			sum+= (int)grid[xi][0].ok_list.size();
		}
		return sum;
	}


	


	void Segments::CrossParticles()
	{

		if (!is_burn) return;

		all_will_burn.clear();

		for (int zi = 0; zi < grid_count_z; zi++)
		{
			for (int i = 0; i < grid_count_x; i++)
			{
				if (CheckSegmentBurn(i, zi))
				{
					for (auto& particle_i : grid[i][zi].ok_list)
					{
						ParticleInBurnSegment(particle_i, i, zi);
					}
				}
			}
		}
		
		
	}


	bool Segments::CheckSegmentBurn(int seg_x, int seg_z)
	{

		for (int xi = seg_x ? seg_x - 1 : 0; xi < (seg_x < grid_count_x - 1 ? seg_x + 2 : grid_count_x); xi++)
		{
			for (int zi = seg_z ? seg_z - 1 : 0; zi < (seg_z < grid_count_z - 1 ? seg_z + 2 : grid_count_z); zi++)
			{
				if (grid[xi][zi].has_burn)
				{
					return true;
				}
			}
		}
		return false;

	}

	void Segments::ParticleInBurnSegment(Particle* particle, int seg_x, int seg_z)
	{
		for (int i = seg_x ? seg_x - 1 : 0; i < (seg_x < grid_count_x - 1 ? seg_x + 2 : grid_count_x); i++)
		{
			for (int zi = seg_z ? seg_z - 1 : 0; zi < (seg_z < grid_count_z - 1 ? seg_z + 2 : grid_count_z); zi++)
			{
				if (grid[i][zi].has_burn) for (auto& burn_i : grid[i][zi].burn_list)
				{
					if (particle->CrossBurn(*burn_i))
					{
						BurnParticle(particle);
						return;
					}
				}
			}
		}
	}

	void Segments::StepParticles()
	{
		for (auto& particle_i : all_list) { 

			particle_i.Step();
			//particle_i.Move();
		}
	}
	void Segments::MoveParticles()
	{
		for (auto& particle_i : all_list) { 

			//particle_i.Step();
			particle_i.Move();
		}
	}



	void Segments::ClearParticleList() {

		auto particle_it = all_list.begin();
		const auto end = all_list.end();
		auto prev_it = all_list.before_begin();


		while (particle_it != end) {
			if (particle_it->state == Particle::State::DIED) {
				++particle_it;
				all_list.erase_after(prev_it);
			}
			else {
				prev_it = particle_it; 
				++particle_it;
			}
		}




		//auto particle_it = all_list.begin();
		//const auto end = all_list.end();
		//auto died_it = particle_it;
		//while (particle_it != end) {
		//	if (particle_it->state == Particle::State::DIED) {
		//		died_it = particle_it;
		//		++particle_it;
		//		all_list.erase(died_it);
		//	}
		//	else ++particle_it;
		//}

	}


	

	int Segments::GetSegmentX(const double x)
	{
		return (int)floor((x - P::area_beg) * grid_count_x_percent);
	}
	int Segments::GetSegmentZ(const double z)
	{
		return (int)floor(z * grid_count_z_percent);
	}
	Segments::Segment* Segments::GetSegment(const double x, const double z)
	{
		return &grid[GetSegmentX(x)][GetSegmentZ(z)];
	}


	void Segments::ClearSegments()
	{
		for (int i = 0; i < grid_count; i++)
		{
			grid_mem[i].has_burn = false;
			grid_mem[i].burn_list.clear();
			grid_mem[i].ok_list.clear();
		}
	}


	void Segments::UpdateSegments()
	{
		is_burn = false;
		burn_segments.clear();
		ClearSegments();

		for (auto& p : all_list) {

			unsigned seg_x = GetSegmentX(p.x);
			unsigned seg_z = GetSegmentZ(p.z);
			Segment *segment = &grid[seg_x][seg_z];
			//Segment *segment = GetSegment(p.x, p.z);

			if (p.state == Particle::State::BURN) {
				burn_segments.emplace_back(seg_x, seg_z);
				is_burn = 1;
				segment->has_burn = 1;
				segment->burn_list.push_back(&p);
			}
			else if (p.state == Particle::State::OK) {
				segment->ok_list.push_back(&p);
			}
		}
	}



	//void Segments::Lighter() {
	//	int i = 0, limit = P::particles_sum;
	//	for (auto& particle_i : all_list) {
	//		BurnParticle(&particle_i);
	//		if (++i == limit) break;
	//	}

	//}



	void Segments::LightsOut() {
		for (auto& p : all_list) {
			if (p.state != Particle::State::OK)
			{
				p.state = Particle::State::SAGE;
			}
		}

		for (auto& xz : burn_segments)
		{
			Segment* segment = &grid[xz.first][xz.second];
			/*for (auto& particle : segment->burn_list)
			{
				particle->state = Particle::State::SAGE;
			}*/
			segment->burn_list.clear();
		}
	}
	const void Segments::Density_Grid()
	{
		std::string output = "count";
		int size = 0;
		for (int zi = 0; zi < grid_count_z; zi++)
		{
			for (int xi = 0; xi < grid_count_x; xi++)
			{
				size = (int)grid[xi][zi].ok_list.size();
				if (size) {
					output += fmt::format("\n{}", size);
				}
				
			}
		}

		std::ofstream csv(P::csv_folder + "d_grid.csv");
		csv << output;
		csv.close();
	}
	const void Segments::Density_Radius()
	{
		std::string output = "count";
		int crossed = 0;
		for (int zi = 1; zi < grid_count_z-1; zi++)
		{
			for (int xi = 1; xi < grid_count_x-1; xi++)
			{
				for (auto& particle_1 : grid[xi][zi].ok_list)
				{
					crossed = 0;
					for (int i = xi - 1; i < xi + 2; i++)
					{
						for (int j = zi - 1; j < zi + 2; j++)
						{
							for (auto& particle_2 : grid[i][j].ok_list)
							{
								if (particle_1->Cross(*particle_2))
								{
									++crossed;
								}
							}
						}
					}
					output+= fmt::format("\n{}", crossed);
				}
				
			}
		}

		std::ofstream csv(P::csv_folder + "d_radius.csv");
		csv << output;
		csv.close();
	}
	const void Segments::Max_Radius()
	{
		std::string output = "count";
		double max = 0, distance;
		for (int zi = 1; zi < grid_count_z-1; zi++)
		{
			for (int xi = 1; xi < grid_count_x-1; xi++)
			{
				for (auto& particle_1 : grid[xi][zi].ok_list)
				{
					max = 0;
					for (int i = xi - 1; i < xi + 2; i++)
					{
						for (int j = zi - 1; j < zi + 2; j++)
						{
							for (auto& particle_2 : grid[i][j].ok_list)
							{
								if (particle_1->Cross(*particle_2))
								{
									distance = particle_1->Distance(*particle_2);
									if (distance < P::burn_radius_2 && distance > max)
									{
										max = distance;
									}
								}
							}
						}
					}
					output+= fmt::format("\n{}", sqrt(max));
				}
				
			}
		}

		std::ofstream csv(P::csv_folder + "d_distance.csv");
		csv << output;
		csv.close();
	}
	/*void Segments::LightsOut_OLD()
	{
		//all_will_burn.clear();
		is_burn = false;
		for (auto& xz : burn_segments)
		{
			Segment* segment = &grid[xz.first][xz.second];
			for (auto& particle : segment->burn_list)
			{
				particle->state = Particle::State::SAGE;
			}
			segment->burn_list.clear();
		}
	}*/

	void Segments::BurnParticle(Particle* particle) {
		particle->setBurn();
		//all_will_burn.push_back(particle);
	}

	void Segments::BurnSegment(Segment* segment) {
		for (auto& particle : segment->ok_list) {
			BurnParticle(particle);
		}
	}

	

	void Segments::Step() {

		UpdateSegments();

		CrossParticles();

		MoveParticles();

		ClearParticleList();

		Fill_Sampling();
	}

	void Segments::FinalLoop(bool move) {

		all_will_burn.clear();

		auto particle_it = all_list.begin();
		const auto end = all_list.end();
		auto prev_it = all_list.before_begin();

		while (particle_it != end) {

			particle_it->Step();

			if (particle_it->state == Particle::State::DIED) {
				++particle_it;
				all_list.erase_after(prev_it);
				continue;
			}

			prev_it = particle_it;

			if (particle_it->burn_counter == 1 || particle_it->state == Particle::State::WARM)
			{
				all_will_burn.push_back(&*particle_it);
			}

			if (move) particle_it->Move();

			++particle_it;
		} // endLoop


	}
	/*void Segments::FinalLoop(bool move) {

		all_will_burn.clear();

		auto particle_it = all_list.begin();
		const auto end = all_list.end();
		auto died_it = particle_it;

		while (particle_it != end) {

			particle_it->Step();

			if (particle_it->state == Particle::State::DIED) {
				died_it = particle_it;
				++particle_it;
				all_list.erase(died_it);
				continue;
			}

			if (particle_it->burn_counter == 1 || particle_it->state == Particle::State::WARM)
			{
				all_will_burn.push_back(&*particle_it);
			}

			if (move) particle_it->Move();

			++particle_it;
		} // endLoop


	}*/


}