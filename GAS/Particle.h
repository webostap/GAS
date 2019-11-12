#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

namespace ps {


	const int BURN_TIME = 5;
	const double BURN_RADIUS = 0.06;

	class Particle {

	public:


		enum class State { OK, BURN, DIED };

		Particle();
		Particle(double x, double y);
		virtual ~Particle() {};

		//get coordinates
		const double _x();
		const double _y();

		//get state
		const State getState();

		const bool isBurn();
		void setBurn();

		void Step();
		const double Distance(const Particle &);
		const bool Cross(const Particle &);

	private:
		State m_state = State::OK;
		size_t m_burn_counter = 0;
		size_t m_steps = 0;

		double m_x_cord, m_y_cord;
		double m_x_vector, m_y_vector;

		double m_speed;
		double m_direction;

		void initialize();
	};

} /* namespace ps */

