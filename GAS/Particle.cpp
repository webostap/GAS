#include "Particle.h"

namespace ps {

	// Particle constructor. Assigns particle initial position (center of window), as well
	// as giving it a unique speed (-1 to 1) and a unique angular direction (0 to 2 pi)
	Particle::Particle() : m_x_cord(0), m_y_cord(0) {
		initialize();
	}


	void Particle::initialize() {
		std::random_device rd;
		std::uniform_real_distribution<double> speed_dist(0.02, 0.03);
		//std::uniform_real_distribution<double> angle_dist(0, 2 * M_PI);


		// Generate random particle speed. Speed is squared causing
		// particle distribution to be exponential instead of linear.
		m_speed = speed_dist(rd);

		// Generate random particle direction between 0 and 2 Pi.
		//m_direction = angle_dist(rd);
	}

	Particle::Particle(double x, double y) : m_x_cord(x), m_y_cord(y) {
		//initialize(x,y);
		std::random_device rd;
		std::uniform_real_distribution<double> speed_dist(P::particle_speed.min, P::particle_speed.max);
		m_speed = speed_dist(rd) + (P::area_size / 2 - abs(P::area_size / 2 - m_y_cord))/10;
	}


	const bool Particle::isBurn()
	{
		return m_state == State::BURN;
	}

	void Particle::setBurn() {
		m_state = State::BURN;
	}


	void Particle::Step() {

		if(m_x_cord >= P::max_x) {
			m_state = State::DIED;
			return;
		}


		if (m_state == State::BURN) ++m_burn_counter;

		if (m_burn_counter == P::burn_time) {
			m_state = State::DIED;
			return;
		}

		++m_steps;

		m_x_cord += m_speed;


		/*m_direction += (m_steps * 0.005);

		m_x_vector = m_speed * sin(m_direction);
		m_y_vector = m_speed * cos(m_direction);

		m_x_cord += m_x_vector * m_steps;
		m_y_cord += m_y_vector * m_steps;*/

	}



	const double Particle::_x()
	{
		return m_x_cord;
	}

	const double Particle::_y()
	{
		return m_y_cord;
	}

	const Particle::State Particle::getState()
	{
		return m_state;
	}

	const double Particle::Distance(const Particle &p)
	{
		return (m_x_cord - p.m_x_cord)*(m_x_cord - p.m_x_cord) + (m_y_cord - p.m_y_cord)*(m_y_cord - p.m_y_cord);
	}
	const bool Particle::Cross(const Particle &p) {
		return Distance(p) < P::burn_radius * P::burn_radius;
	}

} /* namespace ps */
