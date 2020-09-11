#pragma once
#include "Parametrs.inl"

#include <nlohmann/json.hpp>

namespace P {

	const double area_beg = -1;
	const double area_end =  1;
	double stream_beg = -0.75;
	double stream_end =  0.25;
	const double area_size = area_end - area_beg;
	const double area_center = stream_width / 2 + stream_beg;
	const double L = stream_width;

	const double scale = 500;
	const double DSR = L / scale;

	//////
	double burn_radius = 5 * DSR;
	double burn_radius_2 = burn_radius * burn_radius;

	double burn_radius_cross = burn_radius;
	double burn_radius_2_cross = burn_radius_2;
	//////


	double (*stream_function)(double) = const_stream;

	double base_speed = 20. * DSR;
	double burn_speed = 20.;
	double const_speed = burn_speed * DSR;

	int base_particles = 2000;

	int iterations = 3;
	double iterate_const = burn_speed / iterations;
	double iterate_speed = base_speed / iterations;
	int iterate_particles = (int)round(base_particles / iterations * P::stream_function(P::area_center));

	double particles_dist = burn_radius * 0.5;

	int burn_time = 1 * iterations;
	int sage_time = 0 * iterations;
	int wave_time = 10;


	int front_line_steps = 300;
	double front_line_window = area_size / 25;
	int front_line_h = 10;

	bool sdl_draw_plus = false;
	double front_line_bias = burn_radius / 2;
	
	double emitter_begin;
	double refract_coef = 0;
	double refract_offset = 0;
	
	void read_params() {
		nlohmann::json j = nlohmann::json::parse(H::file_to_string("params.json"));

		burn_radius = (double)j["burn_radius"] * DSR;
		burn_radius_2 = burn_radius * burn_radius;

		base_particles = j["base_particles"];
		particles_dist = burn_radius / j["particles_dist"];

		//burn_radius_cross = burn_radius + burn_radius * (double)j["burn_fix"];
		burn_radius_cross = burn_radius;
		burn_radius_cross*= 1 + (pow(base_particles, (double)j["burn_fix"]));
		burn_radius_2_cross = burn_radius_cross * burn_radius_cross;

		emitter_begin = (double)j["emitter_begin"] * burn_radius;


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

		burn_time = (int)j["burn_time"] * iterations;
		sage_time = (int)j["sage_time"] * iterations;
		wave_time = (int)j["wave_time"] * iterations;

		front_line_steps = (int)j["front_line_steps"];
		front_line_window = area_size / (double)j["front_line_window"];
		front_line_h = (int)j["front_line_h"];

		refract_coef = (double)j["refract_coef"];
		refract_offset = (double)j["refract_offset"];
	}
	
}
