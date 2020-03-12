#pragma once
#include <string>
#include <cmath>



namespace P {

	template <typename T>
	struct Interval {
		const T min, max, mid;
		Interval(T aMin, T aMax) : min(aMin), max(aMax), mid((aMax - aMin) / 2.) {}
	};


	const std::string csv_folder ("C:/Users/prive/Desktop/csv/");

	const unsigned steps = 500;
	const unsigned burn_at_step = 5;
	const unsigned light_count = 2000; 
	const unsigned particles_sum = 592;

	const bool edge_burners = false;


	const double area_beg = 0;
	const double area_end = 10;
	const double area_size = area_end - area_beg;
	const double area_center = area_size / 2;
	const double L = area_size;


	static double center_percentage(const double x) {
		return 1 - fabs(1 - (x - P::area_beg) / P::area_size * 2);
	}


	const unsigned segment_count = 40;
	const double segment_size = area_size / segment_count;

	const unsigned burn_index_steps = 100;
	const double burn_index_window = P::area_size / P::burn_index_steps;


	const double max_x = L*25;

	const Interval<unsigned> particles_at_step(200, 300);


	const double DSR = L*40;
	const unsigned base_particles = 20;

	const double particle_distribution_multiple = 0.1;
	const unsigned particle_distribution_steps = 10;



	//const double particle_count_multiple = 1.1;


	const Interval<double> particle_speed(L/40, L/20);

	//speed increases linearly as it approaches the center of the stream
	//speed = base_speed + P::particle_speed.mid * center_percentage * P::center_speed_increase;


	const double center_speed_increase = .5;
	const double burn_radius =  7.5 * L / DSR;
	const double base_speed  =  25  * L / DSR;
	//const double base_speed = 3.7122 * L / DSR;


	static double stream_center(const double x) {
		return fabs(x - area_center);
	}

	static double speed_distribution(const double x) {
		return 0.25 * log(area_center + 1 - fabs(stream_center(x)) );
	}
	
	/*static double speed_distribution(const double x) {
		return 0.02 * (area_center * area_center - stream_center(x) * stream_center(x));
	}*/

	const double burn_radius_2 = burn_radius * burn_radius;



	//const unsigned particles_sum = base_particles * segment_count + base_particles * center_speed_increase * (segment_count / 2);
	
	//const unsigned particles_sum = 2594;


	const unsigned burn_time = 7;

	//num of points in the front line
	const int front_line_steps = 200;

	//area that includes points from which we get the point of the front line 
	const double front_line_window = area_size / 25;


	const Interval<double> front_line_h(4, 20);
	const bool front_line_horizontal = true;



}