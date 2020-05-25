#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#define _USE_MATH_DEFINES 
#include <cmath>

#define M_PI 3.14159265358979323846


namespace H {
	static std::string file_to_string(const char* file_name)
	{
		std::ifstream file(file_name);
		return std::string((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
	}
}

namespace P {


	extern void read_params();


	template <typename T>
	struct Interval {
		const T min, max, mid;
		Interval(T aMin, T aMax) : min(aMin), max(aMax), mid((aMax - aMin) / 2.) {}
	};

	const std::string csv_folder("z:/csv/");

	const int screen_width = 400;
	const int screen_height = 600;
	const int screen_bottom_gap = 0;



	//const int steps = 100;
	//const int burn_at_step = 50;
	//const int light_count = 2000; 
	//const int particles_sum = 1000;



	const bool edge_burners = false;


	const double area_beg = 0;
	const double area_end = 10;
	const double area_size = area_end - area_beg;
	const double area_center = area_size / 2;

	const double area_height = area_size / screen_width * screen_height;

	const double L = area_size;


	static double from_center(const double x) {
		return x - area_center;
	}



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





	static double center_percentage(const double x) {
		return 1 - fabs(1 - (x - P::area_beg) / P::area_size * 2);
	}

	
	//FILL ZIGGURAT//
	const int segment_count = 36;
	const double segment_size = area_size / segment_count;

	//const int burn_index_steps = 80;
	//const double burn_index_window = P::area_size / P::burn_index_steps;





	const double DSR = L*40;

	//const double particle_distribution_multiple = 0.1;
	//const int particle_distribution_steps = 10;



	//const double particle_count_multiple = 1.1;



	//speed increases linearly as it approaches the center of the stream
	//speed = base_speed + P::particle_speed.mid * center_percentage * P::center_speed_increase;



	//const double center_speed_increase = .5;
	extern const double burn_radius;
	const double burn_radius_2 = burn_radius * burn_radius;

	extern double(*stream_function)(double);

	extern double base_speed;
	extern double burn_speed;

	extern int base_particles;

	extern int iterations;
	extern double iterate_speed;
	extern double iterate_particles;




	const int grid_count_x = area_size / burn_radius;
	const int grid_count_z = area_height / burn_radius;
	const int grid_count = grid_count_x * grid_count_z;

	const double grid_count_x_percent = grid_count_x / area_size;
	const double grid_count_z_percent = grid_count_z / area_height;



	//const int particles_sum = base_particles * segment_count + base_particles * center_speed_increase * (segment_count / 2);
	
	//const int particles_sum = 2594;


	extern int burn_time;
	extern int sage_time;

	//num of points in the front line
	const int front_line_steps = 200;

	//area that includes points from which we get the point of the front line 
	const double front_line_window = area_size / 25;


	const Interval<int> front_line_h(12, 15);
	const bool front_line_horizontal = true;





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