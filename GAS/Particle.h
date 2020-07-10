#pragma once
#include "Params.inl"

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

typedef double coord_t;


namespace ps {


	class Particle {

	public:


		enum class State { OK, WARM, BURN, SAGE, DIED };

		//Particle(coord_t x, coord_t z, coord_t speed);
		Particle(coord_t x, coord_t z, coord_t speed, coord_t burn_radius);
		virtual ~Particle() {};

		//get coordinates
		const coord_t _x();
		const coord_t _z();

		//get state
		const State getState();

		const bool isBurn();
		void setBurn();

		//void Step();
		void Step();
		void Move();
		const coord_t Distance(const Particle &);
		const bool Cross(const Particle &);
		const bool CrossBurn(const Particle &);

		State state = State::OK;
		unsigned burn_counter = 0, warm_counter = 0, sage_counter = 0;

		coord_t x, z, speed;
		coord_t burn_radius = 0;
		coord_t burn_radius_2 = burn_radius*burn_radius;
		//unsigned seg_x, seg_z;
	};

} /* namespace ps */

