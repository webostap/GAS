#pragma once
#include "Params.inl"

#include <nlohmann/json.hpp>

namespace P {

	const double burn_radius = 7.5 * L / DSR;

	double (*stream_function)(double) = x2_stream;

	double base_speed = .5;
	double burn_speed = 6;

	int base_particles = 2000;

	int iterations = 3;
	double iterate_speed = base_speed / iterations;
	double iterate_particles = base_particles / iterations;

	int burn_time = 5 * iterations;
	int sage_time = 2 * iterations;
	
	
	inline void read_params() {
		nlohmann::json j = nlohmann::json::parse(H::file_to_string("params.json"));
		base_particles	= j["base_particles"];
		base_speed		= j["base_speed"];
		iterations		= j["iterations"];

		iterate_speed = base_speed / iterations;
		iterate_particles = base_particles / iterations;

		burn_time = 5 * iterations;
		sage_time = 2 * iterations;
	}
	
}
