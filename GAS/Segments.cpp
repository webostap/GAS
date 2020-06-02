#include "Segments.h"
#include <string>
#include <fmt/format.h>
#include <cmath>



namespace ps {

	//Segments::Segments(const Params& aParams) : params(aParams)
	Segments::Segments()
	{
		//UpdateParams();

		for (size_t i = 0; i < P::grid_count_x; i++) {
			grid[i] = (grid_mem + i * P::grid_count_z);
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

				//if (P::edge_burners && (si == 0 || si == P::segment_count - 1)) {
					//BurnParticle(all_list.back());
				//}
			}

			window_beg += P::segment_size;
			window_end += P::segment_size;

			
		}

	}*/
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
				all_list.emplace_back(p_x_cord, p_z_cord, p_speed);
			}
		}
		/*int pi = P::base_particles;
		while (pi)
		{
			p_x_cord = dist_x(rd);
			p_z_cord = dist_z(rd);
			p_speed = particle_speed(p_x_cord);

			if (p_z_cord < p_speed) {
				all_list.emplace_back(p_x_cord, p_z_cord, p_speed);
				--pi;
			}
		}*/


	}

	int Segments::Line_Count()
	{
		int sum = 0;
		for (int i = 0; i < P::grid_count_x; i++)
		{
			sum+= (int)grid[i][0].ok_list.size();
		}
		return sum;
	}


	


	void Segments::CrossParticles()
	{

		if (!is_burn) return;

		all_will_burn.clear();

		for (int j = 0; j < P::grid_count_z; j++)
		{
			for (int i = 0; i < P::grid_count_x; i++)
			{
				if (CheckSegmentBurn(i, j))
				{
					for (auto& particle_i : grid[i][j].ok_list)
					{
						ParticleInBurnSegment(particle_i, i, j);
					}
				}
			}
		}
		
		
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

	void Segments::ParticleInBurnSegment(Particle* particle, int seg_x, int seg_z)
	{
		for (size_t i = seg_x ? seg_x - 1 : 0; i < (seg_x < P::grid_count_x - 1 ? seg_x + 2 : P::grid_count_x); i++)
		{
			for (size_t j = seg_z ? seg_z - 1 : 0; j < (seg_z < P::grid_count_z - 1 ? seg_z + 2 : P::grid_count_z); j++)
			{
				if (grid[i][j].has_burn) for (auto& burn_i : grid[i][j].burn_list)
				{
					if (particle->Cross(*burn_i))
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
		auto died_it = particle_it;
		while (particle_it != end) {
			if (particle_it->state == Particle::State::DIED) {
				died_it = particle_it;
				++particle_it;
				all_list.erase(died_it);
			}
			else ++particle_it;
		}

	}


	

	int Segments::GetSegmentX(const double x)
	{
		return (int)floor((x - P::area_beg) * P::grid_count_x_percent);
	}
	int Segments::GetSegmentZ(const double z)
	{
		return (int)floor(z * P::grid_count_z_percent);
	}
	Segments::Segment* Segments::GetSegment(const double x, const double z)
	{
		return &grid[GetSegmentX(x)][GetSegmentZ(z)];
	}


	void Segments::ClearSegments()
	{
		for (size_t i = 0; i < P::grid_count; i++)
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


	}


}