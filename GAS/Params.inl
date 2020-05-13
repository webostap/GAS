#pragma once
#include <string>
#define _USE_MATH_DEFINES 
#include <cmath>

#define M_PI 3.14159265358979323846



namespace P {


	const unsigned screen_width = 400;
	const unsigned screen_height = 600;
	const unsigned screen_bottom_gap = 0;


	template <typename T>
	struct Interval {
		const T min, max, mid;
		Interval(T aMin, T aMax) : min(aMin), max(aMax), mid((aMax - aMin) / 2.) {}
	};


	const std::string csv_folder ("z:/csv/");

	const unsigned steps = 100;
	const unsigned burn_at_step = 50;
	const unsigned light_count = 2000; 
	const unsigned particles_sum = 1000;

	const unsigned base_particles = 2000;


	const bool edge_burners = false;


	const double area_beg = 0;
	const double area_end = 10;
	const double area_size = area_end - area_beg;
	const double area_center = area_size / 2;

	const double area_height = area_size / screen_width * screen_height;


	const double stream_radius = 5;
	const double stream_width = stream_radius * 2;

	const double screen_x_proportion = area_size / screen_width;
	const double screen_y_proportion = area_height / screen_height;

	static double screen_to_area_x (const int x) {
		return x * screen_x_proportion;
	}
	static double screen_to_area_y (const int y) {
		return area_height - y * screen_y_proportion;
	}



	const double L = area_size;


	static double center_percentage(const double x) {
		return 1 - fabs(1 - (x - P::area_beg) / P::area_size * 2);
	}

	
	//FILL ZIGGURAT//
	const unsigned segment_count = 36;
	const double segment_size = area_size / segment_count;

	//const unsigned burn_index_steps = 80;
	//const double burn_index_window = P::area_size / P::burn_index_steps;


	const double max_x = 20.;



	const double DSR = L*40;

	const double particle_distribution_multiple = 0.1;
	const unsigned particle_distribution_steps = 10;



	//const double particle_count_multiple = 1.1;



	//speed increases linearly as it approaches the center of the stream
	//speed = base_speed + P::particle_speed.mid * center_percentage * P::center_speed_increase;


	const double center_speed_increase = .5;
	const double burn_radius = 7.5 * L / DSR;
	const double base_speed = .5;
	const double burn_speed = 6;


	const unsigned iterations = 3;
	const double iterate_speed = base_speed / iterations;
	const double iterate_particles = base_particles / iterations;


	
	/*static double speed_distribution(const double x) {
		return 0.02 * (area_center * area_center - stream_center(x) * stream_center(x));
	}*/

	const double burn_radius_2 = burn_radius * burn_radius;


	const unsigned grid_count_x = area_size / burn_radius;
	const unsigned grid_count_z = area_height / burn_radius;
	const unsigned grid_count = grid_count_x * grid_count_z;

	const double grid_count_x_percent = grid_count_x / area_size;
	const double grid_count_z_percent = grid_count_z / area_height;



	//const unsigned particles_sum = base_particles * segment_count + base_particles * center_speed_increase * (segment_count / 2);
	
	//const unsigned particles_sum = 2594;


	const unsigned burn_time = 5*iterations;
	const unsigned sage_time = 2*iterations;

	//num of points in the front line
	const int front_line_steps = 200;

	//area that includes points from which we get the point of the front line 
	const double front_line_window = area_size / 25;


	const Interval<int> front_line_h(12, 15);
	const bool front_line_horizontal = true;





	static double from_center(const double x) {
		return x - area_center;
	}



	static double const_stream(const double x) {
		return .5;
	}
	static double linear_stream(const double x) {
		return 1 - fabs(from_center(x)) / area_center;
	}
	static double log_stream(const double x) {
		return log(area_center + 1 - fabs(from_center(x))) / log(area_center + 1);
	}
	static double x2_stream(const double x) {
		return 1 - from_center(x) * from_center(x) / area_center / area_center;
	}



	static double stream_function(const double x) {
		return x2_stream(x);
	}

	static double particle_count (const double x) {
		return iterate_particles * stream_function(x) / segment_count;
	}

	static double system_speed (const double x) {
		return base_speed * stream_function(x);
	}
	static double particle_speed (const double x) {
		return iterate_speed * stream_function(x);
	}





}