#pragma once
#include "Params.inl"

#include <nlohmann/json.hpp>

namespace P {

	const double area_beg =  0;
	const double area_end =  10;

	const double scale = 400;
	const double DSR = L / scale;

	//////
	double burn_radius = 5 * DSR; 
	double burn_radius_2 = burn_radius * burn_radius;
	//////


	double (*stream_function)(double) = x2_stream;

	double base_speed = 20. * DSR;
	double burn_speed = 20.;
	double const_speed = burn_speed * DSR;

	int base_particles = 2000;

	int iterations = 3;
	double iterate_const = burn_speed / iterations;
	double iterate_speed = base_speed / iterations;
	int iterate_particles = (int)round(base_particles / iterations * P::stream_function(P::area_center));

	int burn_time = 5 * iterations;
	int sage_time = 2 * iterations;
	
	
	
	void read_params() {
		nlohmann::json j = nlohmann::json::parse(H::file_to_string("params.json"));

		burn_radius = j["burn_radius"] * DSR; 
		burn_radius_2 = burn_radius * burn_radius;

		base_particles = j["base_particles"];

		base_speed = j["base_speed"];
		base_speed*= burn_radius;

		burn_speed = j["burn_speed"];
		burn_speed/= burn_radius;
		//burn_speed/= scale/10/10;

		const_speed = j["const_speed"];
		const_speed*= DSR;

		iterations = j["iterations"];

		iterate_speed = base_speed / iterations;
		iterate_const = const_speed / iterations;
		iterate_particles = (int)round(base_particles * P::particle_speed(P::area_center) / burn_radius_2 * L / 10 * 3);

		burn_time = 5 * iterations;
		sage_time = 2 * iterations;
	}
	
}
