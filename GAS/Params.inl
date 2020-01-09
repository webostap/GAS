#pragma once
#include <string>

namespace P {

	const std::string csv_folder ("C:/Users/prive/Desktop/csv/");


	template <typename T> 
	struct Interval {
		const T min, max, mid;
		Interval(T aMin, T aMax) : min(aMin), max(aMax), mid((aMax - aMin) / 2) {}
	};

	const unsigned steps = 5000;
	const unsigned burn_at_step = 15;
	const unsigned light_count = 2000;




	const double area_beg = 0;
	const double area_end = 10;
	const double area_size = area_end - area_beg;
	const double L = area_size;


	const unsigned burn_index_steps = 100;
	const double burn_index_window = P::area_size / P::burn_index_steps;


	const double max_x = L*25;

	const Interval<unsigned> particles_at_step(200, 300);
	const unsigned base_particles = 100;

	const double particle_distribution_multiple = 0.1;
	const unsigned particle_distribution_steps = 10;


	//const double particle_count_multiple = 1.1;


	const Interval<double> particle_speed(L/40, L/20);

	//speed increases linearly as it approaches the center of the stream
	//speed = base_speed + P::particle_speed.mid * center_percentage * P::center_speed_increase;
	const double base_speed = 7.2 * L / 1000;
	const double center_speed_increase = 0.1;

	const double burn_radius = 15 * L / 1000;



	const unsigned burn_time = 3;

	//num of points in the front line
	const int front_line_steps = 100;

	//area that includes points from which we get the point of the front line 
	const double front_line_window = area_size / 20;


	const unsigned front_line_h = 10;



}