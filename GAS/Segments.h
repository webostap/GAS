#pragma once
#include "Params.inl"
#include "Particle.h"
#include "FrontLine.h"
#include <list>
#include <vector>
#include <fstream>

namespace ps {

	class Segments {

	public:

	public:
		Segments();

		void Step();

		void Fill();
		void Fill_2();
		void Lighter();
		void UpdateBurnFrom();
		void BurnParticles();
		void BurnParticles_2();
		void BurnParticles_3();
		void MoveParticles();
		void ClearBurnList();
		void ClearParticleList();

		void BurnParticle(Particle &p);
		void BurnParticle_2(Particle &p);

		//void StepParticle(unsigned index, std::list<Particle>::iterator&);

		const void PrintStep(size_t);



		void ParticleInBurnSegment(Particle& particle, int seg_x, int seg_z);
		void ParticleInBurnSegment_2(Particle& particle, int seg_x, int seg_z);
		void ParticleInBurnSegment(Particle& particle);

		bool CheckSegmentBurn(int seg_x, int seg_z);


		unsigned GetSegmentIndex(Particle& p);
		unsigned GetParticleSegmentX(Particle& p);
		unsigned GetParticleSegmentZ(Particle& p);

		struct GridEl {
			double beg, end;
			bool has_burn = false;
			std::vector<Particle*> ok_list, burn_list;
		};

		GridEl** grid = new GridEl*[P::grid_count_x];
		GridEl* grid_mem = new GridEl[P::grid_count];

		std::vector<std::pair<int,int>> burn_segments;

		void UpdateSegments();
		void UpdateSegments_2();





		//private:

		double segment_beg[P::burn_index_steps];
		double segment_end[P::burn_index_steps];

		double segment_cross_beg[P::burn_index_steps];
		double segment_cross_end[P::burn_index_steps];

		double burn_from[P::burn_index_steps];

		std::list <Particle> all_list;
		std::list <Particle*> all_burn;
		std::list <Particle*> all_will_burn;


		//std::list <Particle> particle_list[P::segment_count];

		std::list <Particle*> burn_list[P::burn_index_steps];
		std::list <Particle*> will_burn_list[P::burn_index_steps];
		//std::vector<std::list<Particle>::iterator> died_list;


		FrontLine m_front_line = FrontLine(P::front_line_steps, P::front_line_window, P::area_beg, P::area_end);
	};


} /* namespace ps */

