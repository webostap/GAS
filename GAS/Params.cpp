#pragma once
#include "Params.inl"

#include <nlohmann/json.hpp>

namespace P {

	const double area_beg = -1;
	const double area_end =  1;

	const double scale = 400;
	const double DSR = L / scale;

	//////
	double burn_radius = 5 * DSR;
	double burn_radius_2 = burn_radius * burn_radius;

	double burn_radius_cross = burn_radius;
	double burn_radius_2_cross = burn_radius_2;
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

	double particles_dist = burn_radius * 0.5;

	int burn_time = 2 * iterations;
	int sage_time = 0 * iterations;
	
	
	
	void read_params() {
		nlohmann::json j = nlohmann::json::parse(H::file_to_string("params.json"));

		burn_radius = (double)j["burn_radius"] * DSR;
		burn_radius_2 = burn_radius * burn_radius;

		base_particles = j["base_particles"];
		particles_dist = burn_radius / j["particles_dist"];

		//burn_radius_cross = burn_radius + burn_radius * (double)j["burn_fix"];
		burn_radius_cross = burn_radius;
		burn_radius_cross+= burn_radius * (pow(base_particles, (double)j["burn_fix"]));
		burn_radius_2_cross = burn_radius_cross * burn_radius_cross;


		base_speed = (double)j["base_speed"];
		base_speed*= burn_radius;

		burn_speed = j["burn_speed"];
		burn_speed/= burn_radius;
		//burn_speed/= scale/10/10;

		const_speed = j["const_speed"];
		const_speed*= burn_radius;

		iterations = (int)j["iterations"];

		iterate_speed = base_speed / iterations;
		iterate_const = const_speed / iterations;
		iterate_particles = (int)round(base_particles * P::particle_speed(P::area_center) / burn_radius_2 / M_PI * L);

		burn_time = 1 * iterations;
		sage_time = 0 * iterations;
	}
	
}
