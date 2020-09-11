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
	template <typename T>
	class Singleton
	{
	public:
		static T& instance() {
			static T inst;
			return inst;
		}
		
	protected:
		Singleton() {};
	};


}

namespace P {

	extern void read_params();


	const std::string csv_folder("z:/csv/");

	const int screen_width = 600;
	const int screen_height = 900;
	const int screen_bottom_gap = 0;

	extern bool sdl_draw_plus;
	extern double front_line_bias;



	//const int steps = 100;
	//const int burn_at_step = 50;
	//const int light_count = 2000; 
	//const int particles_sum = 1000;



	const bool edge_burners = false;


	extern const double area_beg;
	extern const double area_end;
	extern const double area_size;
	extern double stream_beg;
	extern double stream_end;
	extern const double area_center;
	extern const double L;

	const double area_height = area_size / screen_width * screen_height;

	extern const double scale;
	extern const double DSR;


	static double from_center(const double x) {
		return x - area_center;
	}

	const double screen_proportion = screen_width / P::area_size;

	const double stream_width = stream_end - stream_beg;
	const double stream_radius = stream_width / 2;

	const double screen_x_proportion = area_size / screen_width;
	const double screen_y_proportion = area_height / screen_height;

	static double screen_to_area_x (const int x) {
		return x * screen_x_proportion + P::area_beg;
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






	//const double particle_distribution_multiple = 0.1;
	//const int particle_distribution_steps = 10;



	//const double particle_count_multiple = 1.1;



	//speed increases linearly as it approaches the center of the stream
	//speed = base_speed + P::particle_speed.mid * center_percentage * P::center_speed_increase;



	//const double center_speed_increase = .5;



	extern double burn_radius;
	extern double burn_radius_2;
	extern double burn_radius_cross;
	extern double burn_radius_2_cross;

	extern double(*stream_function)(double);

	extern double base_speed;
	extern double burn_speed;
	extern double const_speed;

	extern int base_particles;

	extern int iterations;
	extern double iterate_speed;
	extern double iterate_const;
	extern int iterate_particles;

	extern double particles_dist;

	extern double emitter_begin;




	//const int grid_count_x = (int)floor(area_size / burn_radius);
	//const int grid_count_z = (int)floor(area_height / burn_radius);
	//const int grid_count = grid_count_x * grid_count_z;

	//const double grid_count_x_percent = grid_count_x / area_size;
	//const double grid_count_z_percent = grid_count_z / area_height;



	//const int particles_sum = base_particles * segment_count + base_particles * center_speed_increase * (segment_count / 2);
	
	//const int particles_sum = 2594;


	extern int burn_time;
	extern int sage_time;
	extern int wave_time;


	extern int front_line_steps;
	extern double front_line_window;
	extern int front_line_h;





	static double linear_stream(const double x) {
		return 1 - fabs(from_center(x)) / stream_radius;
	}
	static double log_stream(const double x) {
		return log(stream_radius + 1 - fabs(from_center(x))) / log(stream_radius + 1);
	}
	static double x2_stream(const double x) {
		return 1 - from_center(x)*from_center(x) / stream_radius / stream_radius;
	}
	static double const_stream(const double x) {
		return .5;
	}




	static double particle_count (const double x) {
		return iterate_particles * stream_function(x) / segment_count;
	}

	static double system_speed (const double x) {
		return base_speed * stream_function(x) + const_speed;
	}
	static double burn_speed_fu (const double x) {
		return burn_speed * stream_function(x);
	}
	static double particle_speed (const double x) {
		return iterate_speed * stream_function(x) + iterate_const;
	}
	static double profile_speed (const double x) {
		return iterate_speed * stream_function(x);
	}


	extern double refract_coef;
	extern double refract_offset;
	static double refract_func(const double x) {
		return x * refract_coef + refract_offset;
	}





}