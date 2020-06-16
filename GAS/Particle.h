#pragma once
#include "Params.inl"

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>


namespace ps {


	class Particle {

	public:


		enum class State { OK, WARM, BURN, SAGE, DIED };

		Particle(float x, float z, float speed);
		Particle(float x, float z, float speed, float burn_radius);
		virtual ~Particle() {};

		//get coordinates
		const float _x();
		const float _z();

		//get state
		const State getState();

		const bool isBurn();
		void setBurn();

		//void Step();
		void Step();
		void Move();
		const float Distance(const Particle &);
		const bool Cross(const Particle &);
		const bool CrossBurn(const Particle &);

		State state = State::OK;
		unsigned burn_counter = 0, warm_counter = 0, sage_counter = 0;

		float x, z, speed;
		float burn_radius = 0;
		float burn_radius_2 = burn_radius*burn_radius;
		//unsigned seg_x, seg_z;
	};

} /* namespace ps */

