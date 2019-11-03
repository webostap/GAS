#pragma once
#include "Particle.h"
#include <list>
#include <vector>
#include <string>

namespace ps {

	class Swarm {

	public:
		struct Params {
			std::string csvFolder;
		};

	private:
		Params m_params;

	public:
		Swarm(Params);
		void Erasing();
		void Test(size_t);
		void Fill(size_t);
		void Step();
		void ClearBurnList();
		void ClearParticleList();

		const void PrintStep(size_t);

		size_t size();


	//private:

		std::list <Particle> m_particle_list;
		std::list <Particle*> m_burn_list;
		std::list <Particle*> m_will_burn_list;

		std::vector<std::list<Particle>::iterator> m_died_list;
		//std::vector<std::list<Particle>::iterator> m_died_list;
	};


} /* namespace ps */

