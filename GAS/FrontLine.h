#pragma once

#include "Particle.h"
#include <list>
#include <string>
#include <fstream>


namespace ps {

	class FrontLine
	{
	public:
		FrontLine(int steps, double window, double area_start, double area_end);
		void Init(int steps, double window, double area_start, double area_end);

		void Calc(std::list <Particle*>& particle_list);
		void Print(unsigned num);

	private:

		struct front_line_point {
			double x = 0, y = 0, sum = 0;
			unsigned count = 0;
		};

		front_line_point* front_line_points;

		int steps;
		double radius;
		double steps_start;
		double steps_end;
		double steps_area;
		double step_size;

	};
}