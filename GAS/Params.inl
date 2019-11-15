#pragma once
#include <string>

namespace P {

	const std::string csv_folder ("C:/Users/prive/Desktop/csv/");


	template <typename T> 
	struct MinMax {
		const T min, max;
		MinMax(T aMin, T aMax) : min(aMin), max(aMax) {}
	};

	const unsigned steps = 500;
	const unsigned burn_at_step = 5;


	const double area_beg = 0;
	const double area_end = 0.2;
	const double area_size = area_end - area_beg;

	const double max_x = 25;

	const MinMax<unsigned> particles_at_step(300, 500);
	const MinMax<double> particle_speed(0.02, 0.04);

	const double burn_radius = 0.035;
	const unsigned burn_time = 3;

	const int front_line_steps = 100;
	const double front_line_window = 0.02;

}