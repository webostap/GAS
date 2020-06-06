#include "Particle.h"

#include <iostream>

namespace ps {


	Particle::Particle(double _x, double _z, double _speed) : 
		x(_x), z(_z), speed(_speed){}

	Particle::Particle(double _x, double _z, double _speed, double _burn_radius = P::burn_radius) : 
		x(_x), z(_z), speed(_speed), burn_radius(_burn_radius), burn_radius_2(burn_radius*burn_radius){}



	const bool Particle::isBurn() {
		return state == State::BURN;
	}

	void Particle::setBurn() {
		state = State::WARM;
	}


	void Particle::Step() {

		switch (state)
		{
		case State::WARM:

			if (++warm_counter == P::iterations)
			{
				state = State::BURN;
				++burn_counter;
			}
			break;

		case State::BURN:

			if (++burn_counter == P::burn_time)
			{
				state = State::SAGE;
				++sage_counter;
			}
			break;

		case State::SAGE:

			if (++sage_counter == P::sage_time)
			{
				state = State::DIED;
			}
			break;

		default:
			break;
		}



	}
	void Particle::Move() {

		z+= speed;

		if (z >= P::area_height) state = State::DIED;
	}



	const double Particle::_x() {
		return x;
	}

	const double Particle::_z() {
		return z;
	}

	const Particle::State Particle::getState() {
		return state;
	}

	inline const double Particle::Distance(const Particle &p)
	{
		return (x - p.x)*(x - p.x) + (z - p.z)*(z - p.z);

	}
	const bool Particle::Cross(const Particle &p) {
		return Distance(p) < P::burn_radius_2;
	}
	const bool Particle::CrossBurn(const Particle &burn_particle) {
		return Distance(burn_particle) < burn_particle.burn_radius_2;
	}

} /* namespace ps */
