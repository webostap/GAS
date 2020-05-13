#pragma once
#include "Params.inl"

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

namespace ps {


	class Particle {

	public:


		enum class State { OK, WARM, BURN, SAGE, DIED };

		Particle(double x, double z, double speed);
		virtual ~Particle() {};

		//get coordinates
		const double _x();
		const double _z();

		//get state
		const State getState();

		const bool isBurn();
		void setBurn();

		//void Step();
		void Step();
		void Move();
		const double Distance(const Particle &);
		const bool Cross(const Particle &);

		State state = State::OK;
		unsigned burn_counter = 0, warm_counter = 0, sage_counter = 0;

		double x, z, speed;
		//unsigned seg_x, seg_z;
	};

} /* namespace ps */

