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
		void Init();

		void Calc(const std::vector <Particle*>& particle_list);
		void Calc2(const std::vector <Particle*>& particle_list);
		void FivePointStencil();
		void Print(unsigned num);

		struct front_line_point {
			double x=0, z=0, div = 0, sum = 0, diff2 = 0, div2 = 0;
			unsigned count = 0;
		};



		front_line_point* front_line_points{0};

		double *Vx{0}, *Vx2{0};

		int steps = 0;
		unsigned h;
		double area_start, area_end;
		double radius;
		double steps_start;
		double steps_end;
		double steps_area;
		double step_size;

		double w_percent;

	public:
		~FrontLine() {
			delete[]Vx;
			delete[]Vx2;
			delete[]front_line_points;
		}
	};

}