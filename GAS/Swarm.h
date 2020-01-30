#pragma once
#include "Params.inl"
#include "Particle.h"
#include "FrontLine.h"
#include <list>
#include <vector>
#include <fstream>

namespace ps {

	class Swarm {

	public:

	public:
		Swarm();
		void Erasing();
		void Test(size_t);
		void Fill(size_t);
		void Fill_New(size_t);
		void Lighter(size_t);
		void Step();
		void Step_New();
		void ClearBurnList();
		void ClearParticleList();

		int GetBurnIndex(double y);
		void BurnParticle(std::list<Particle>::iterator&);
		void StepParticle(std::list<Particle>::iterator&);

		const void PrintStep(size_t);

		size_t size();

		double m_burn_from = 0;
		void UpdateBurnFrom();
		void UpdateBurnFrom_New();
		void UpdateBurnIndex();


	//private:
		double burn_from [P::segment_count];
		std::list <Particle> particle_list [P::segment_count];
		std::list <Particle*> burn_list [P::segment_count];
		std::list <Particle*> will_burn_list [P::segment_count];
		std::vector<std::list<Particle>::iterator> died_list[P::segment_count];


		std::list <Particle> m_particle_list;
		std::list <Particle*> m_burn_list;
		std::list <Particle*> m_will_burn_list;

		std::vector<std::list<Particle>::iterator> m_died_list;
		
		
		double m_burn_index[P::burn_index_steps];

		//std::vector<std::list<Particle>::iterator> m_died_list;

		FrontLine m_front_line = FrontLine(P::front_line_steps, P::front_line_window, P::front_line_h, P::area_beg, P::area_end);
	};


} /* namespace ps */

