#pragma once
#include "Params.inl"

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

namespace ps {


	class Particle {

	public:


		enum class State { OK, BURN, DIED };

		Particle();
		Particle(double y);
		Particle(double x, double y);
		Particle(double x, double y, double speed);
		virtual ~Particle() {};

		//get coordinates
		const double _x();
		const double _y();

		//get state
		const State getState();

		const bool isBurn();
		void setBurn();

		//void Step();
		void Step();
		void Move();
		const double Distance(const Particle &);
		const bool Cross(const Particle &);

		State m_state = State::OK;
		size_t m_burn_counter = 0;
		size_t m_steps = 0;

		double m_x_cord, m_y_cord, m_cords;
		//double m_x_vector, m_y_vector;

		double m_speed;
		//double m_direction;

		void initialize();
	};

} /* namespace ps */

