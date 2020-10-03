#pragma once

#include "Params.hpp"
#include "Particle.h"
#include <list>
#include <vector>
#include <string>
#include <fmt/format.h>
#include <fstream>


namespace ps {

	class FrontLine
	{
		const Params* P;
	public:
		FrontLine(const Params& P_) : P(&P_) { Init(); }
		void Init(int steps, double window, double 
			tart, double area_end);
		void Init();


		void Calc(const std::vector <Particle*>& particle_list);
		void CalcError();

		void Calc2(const std::vector <Particle*>& particle_list);
		void FivePointStencil();
		void Print(unsigned num);

		struct point {
			double x, Vx, Vx2;
		};
		point* points{ 0 };

		struct front_line_point {
			double x=0, z=0, div = 0, sum = 0, diff2 = 0, div2 = 0;
			unsigned count = 0;
		};

		front_line_point* front_line_points{0};

		//double *x{ 0 }, *Vx{ 0 }, * Vx2{ 0 };

		int steps = 0;
		double window = 0;
		unsigned h;
		double area_start, area_end;
		double radius;
		double steps_start;
		double steps_end;
		double steps_area;
		double step_size;

		double w_percent;

		double avg = 0, _avg = 0;
		double deviation = 0, error = 0;

	public:
		~FrontLine() {
			delete[]points;
			delete[]front_line_points;
		}
	};

}