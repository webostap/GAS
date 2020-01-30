#pragma once
#include <string>

namespace P {

	const std::string csv_folder ("C:/Users/prive/Desktop/csv/");


	template <typename T> 
	struct Interval {
		const T min, max, mid;
		Interval(T aMin, T aMax) : min(aMin), max(aMax), mid((aMax - aMin) / 2) {}
	};

	const unsigned steps = 100;
	const unsigned burn_at_step = 100;
	const unsigned light_count = 2000;




	const double area_beg = 0;
	const double area_end = 10;
	const double area_size = area_end - area_beg;
	const double L = area_size;


	const unsigned segment_count = 10;
	const double segment_size = area_size / segment_count;

	const unsigned burn_index_steps = 100;
	const double burn_index_window = P::area_size / P::burn_index_steps;


	const double max_x = L*25;

	const Interval<unsigned> particles_at_step(200, 300);


	const double DSR = L * 50;
	const unsigned base_particles = 3;

	const double particle_distribution_multiple = 0.1;
	const unsigned particle_distribution_steps = 10;



	//const double particle_count_multiple = 1.1;


	const Interval<double> particle_speed(L/40, L/20);

	//speed increases linearly as it approaches the center of the stream
	//speed = base_speed + P::particle_speed.mid * center_percentage * P::center_speed_increase;

	const double center_speed_increase = 2;
	const double burn_radius = 7.5  * L / DSR;
	const double base_speed  = 3.71 * L / DSR * (1 - center_speed_increase / 25);



	const unsigned particles_sum = P::base_particles * P::segment_count + P::base_particles * P::center_speed_increase * (P::segment_count / 2);


	const unsigned burn_time = 3;

	//num of points in the front line
	const int front_line_steps = 100;

	//area that includes points from which we get the point of the front line 
	const double front_line_window = area_size / 20;


	const unsigned front_line_h = 10;



}