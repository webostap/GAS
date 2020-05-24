#pragma once

#include "Params.inl"

namespace P {
	const double burn_radius = 7.5 * L / DSR;

	double base_speed = .5;
	double burn_speed = 6;

	int base_particles = 2000;

	int iterations = 3;
	double iterate_speed = base_speed / iterations;
	double iterate_particles = base_particles / iterations;

	int burn_time = 5 * iterations;
	int sage_time = 2 * iterations;
}
