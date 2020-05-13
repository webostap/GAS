#pragma once

#include "Particle.h"
#include <list>
#include <string>
#include <fmt/format.h>
#include <fstream>


namespace ps {

	class FrontLine
	{
	public:
		FrontLine(int steps, double window, double area_start, double area_end);
		void Init(int steps, double window, double area_start, double area_end);

		void Calc(const std::list <Particle*>& particle_list);
		void FivePointStencil();
		void Print(unsigned num);

	private:

		struct front_line_point {
			double x, z, div = 0, sum = 0, diff2 = 0, div2 = 0;
			unsigned count = 0;
		};

		front_line_point* front_line_points;

		double* Vx, * Vx2;

		int steps;
		unsigned h;
		double radius;
		double steps_start;
		double steps_end;
		double steps_area;
		double step_size;

	};
}