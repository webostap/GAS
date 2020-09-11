#pragma once

#define M_PI 3.14159265358979323846
#include <cmath>
#include <string>
#include <cassert>
namespace ps {
	class Params;
}

class ps::Params {

public:

	Params();
	void Read();

	const int screen_width = 600;
	const int screen_height = 900;
	const int screen_bottom_gap = 0;

	std::string csv_folder;

	double screen_proportion, screen_x_proportion, screen_y_proportion;

	double area_beg, area_end, area_size, area_center, area_height;
	double stream_beg, stream_end, stream_width, stream_center, stream_radius;
	double L, scale, DSR;


	double burn_radius, burn_radius_2;
	double burn_radius_cross, burn_radius_2_cross;

	double base_speed, burn_speed, const_speed;

	double emitter_begin;
	double base_particles, particles_dist;

	int iterations;
	double iterate_burn_radius;
	double iterate_speed;
	double iterate_const;
	int iterate_particles;

	int burn_time, sage_time, wave_time;


	int front_line_steps, front_line_h;
	double front_line_window;


	double refract_coef, refract_offset;






	double stream_function(double x) const {
		return (this->*stream_function_p)(x);
	}

	void SetStream(int i) {
		assert(i && i <= 4);
		stream_function_p = streams[i];
	}

private:
	typedef double(Params::* stream)(double) const;
	stream streams[5]{
		0,
		&Params::linear_stream,
		&Params::log_stream,
		&Params::x2_stream,
		&Params::const_stream
	};
	stream stream_function_p = streams[3];

	double linear_stream(const double x) const {
		return 1 - fabs(from_center(x)) / stream_radius;
	}

	double log_stream(const double x) const {
		return log(stream_radius + 1 - fabs(from_center(x))) / log(stream_radius + 1);
	}
	double x2_stream(const double x) const {
		return 1 - from_center(x) * from_center(x) / stream_radius / stream_radius;
	}
	double const_stream(const double x) const  {
		return .5;
	}


public:
	double from_center(const double x) const{
		return x - area_center;
	}
	double screen_to_area_x(const int x) const {
		return x * screen_x_proportion + area_beg;
	}
	double screen_to_area_y(const int y) const {
		return area_height - y * screen_y_proportion;
	}
	double center_percentage(const double x) const {
		return 1 - fabs(1 - (x - area_beg) / area_size * 2);
	}
	double system_speed(const double x) const {
		return base_speed * stream_function(x) + const_speed;
	}
	double burn_speed_fu(const double x) const {
		return burn_speed * stream_function(x);
	}
	double profile_speed(const double x) const {
		return iterate_speed * stream_function(x);
	}
	double particle_speed(const double x) const {
		return profile_speed(x) + iterate_const;
	}

	double refract_func(const double x) const {
		return x * refract_coef + refract_offset;
	}




};

