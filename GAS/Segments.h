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
		void Lighter();
		void UpdateBurnFrom();
		void BurnParticles();
		void MoveParticles();
		void ClearBurnList();
		void ClearParticleList();

		void BurnParticle(Particle &p);

		//void StepParticle(unsigned index, std::list<Particle>::iterator&);

		const void PrintStep(size_t);



		unsigned GetSegmentIndex(Particle& p);




		//private:

		double segment_beg[P::segment_count];
		double segment_end[P::segment_count];

		double segment_cross_beg[P::segment_count];
		double segment_cross_end[P::segment_count];

		double burn_from[P::segment_count];

		std::list <Particle> all_list;
		std::list <Particle*> all_burn;
		std::list <Particle*> all_will_burn;


		std::list <Particle> particle_list[P::segment_count];

		std::list <Particle*> burn_list[P::segment_count];
		std::list <Particle*> will_burn_list[P::segment_count];
		//std::vector<std::list<Particle>::iterator> died_list;


		FrontLine m_front_line = FrontLine(P::front_line_steps, P::front_line_window, P::area_beg, P::area_end);
	};


} /* namespace ps */

