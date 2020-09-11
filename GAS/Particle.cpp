#include "Particle.h"

namespace ps {

	
	/*Particle::Particle(coord_t _x, coord_t _z, coord_t _speed) : 
		x(_x), z(_z), speed(_speed){}*/

	Particle::Particle(coord_t _x, coord_t _z, coord_t _speed, coord_t _burn_radius) :
		x(_x), z(_z), speed(_speed), burn_radius(_burn_radius){}



	const bool Particle::isBurn() {
		return state == State::BURN;
	}

	void Particle::setBurn() {
		state = State::WARM;
		burn_counter = 1;
	}


	void Particle::Move() {
		z+= speed * (state != State::WAVE);
	}


	coord_t Particle::_x() const {
		return x;
	}

	coord_t Particle::_z() const {
		return z;
	}

	Particle::State Particle::getState() const {
		return state;
	}

	coord_t Particle::Distance(const Particle &p) const {
		return (x - p.x) * (x - p.x) + (z - p.z) * (z - p.z);
	}
	bool Particle::Cross(const Particle &p) const {
		return Distance(p) <= burn_radius_2;
	}
	bool Particle::CrossBurn(const Particle &burn_particle) const {
		return Distance(burn_particle) <= burn_radius_2;
	}

} /* namespace ps */
