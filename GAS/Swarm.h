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
		void Step();
		void ClearBurnList();
		void ClearParticleList();

		void StepParticle(std::list<Particle>::iterator&);

		const void PrintStep(size_t);

		size_t size();

		double m_burn_from = 0;
		void UpdateBurnFrom();


	//private:

		std::list <Particle> m_particle_list;
		std::list <Particle*> m_burn_list;
		std::list <Particle*> m_will_burn_list;

		std::vector<std::list<Particle>::iterator> m_died_list;
		//std::vector<std::list<Particle>::iterator> m_died_list;

		FrontLine m_front_line = FrontLine(P::front_line_steps, P::front_line_window, P::front_line_h, P::area_beg, P::area_end);
	};


} /* namespace ps */

