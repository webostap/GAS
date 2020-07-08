#include "Particle.h"

namespace ps {

	
	Particle::Particle(coord_t _x, coord_t _z, coord_t _speed) : 
		x(_x), z(_z), speed(_speed){}

	//Particle::Particle(coord_t _x, coord_t _z, coord_t _speed, coord_t _burn_radius) : 
		//x(_x), z(_z), speed(_speed), burn_radius(_burn_radius){}



	const bool Particle::isBurn() {
		return state == State::BURN;
	}

	void Particle::setBurn() {
		state = State::WARM;
		burn_counter = 1;
	}


	void Particle::Step() {

		switch (state)
		{
		case State::WARM:

			if (++warm_counter >= P::iterations)
			{
				state = State::BURN;
				//++burn_counter;
			}
			break;

		case State::BURN:

			if (++burn_counter > P::burn_time)
			{
				state = P::sage_time ? State::SAGE : State::DIED;
			}
			break;

		case State::SAGE:

			if (++sage_counter >= P::sage_time)
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


	const coord_t Particle::_x() {
		return x;
	}

	const coord_t Particle::_z() {
		return z;
	}

	const Particle::State Particle::getState() {
		return state;
	}

	inline const coord_t Particle::Distance(const Particle &p)
	{
		return (x - p.x) * (x - p.x) + (z - p.z) * (z - p.z);
	}
	const bool Particle::Cross(const Particle &p) {
		return Distance(p) <= P::burn_radius_2_cross;
	}
	const bool Particle::CrossBurn(const Particle &burn_particle) {
		return Distance(burn_particle) <= P::burn_radius_2_cross;
	}

} /* namespace ps */
