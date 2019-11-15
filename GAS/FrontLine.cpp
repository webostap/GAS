#include "FrontLine.h"

namespace ps {

	FrontLine::FrontLine(int steps, double window, double area_start, double area_end) {
		Init(steps, window, area_start, area_end);
	}
		
	void FrontLine::Init(int aSteps, double window, double area_start, double area_end) {

		steps = aSteps;
		radius = window / 2;
		steps_start = area_start + radius;
		steps_end = area_end - radius;
		steps_area = steps_end - steps_start;
		step_size = steps_area / (aSteps - 1);

		front_line_points = new front_line_point[steps];

		for (int i = 0; i < steps; ++i) {
			front_line_points[i].y = steps_start + step_size * i;
		}
	}


	void FrontLine::Calc(std::list <Particle*>& particle_list) {

		for (int i = 0; i < steps; ++i) {
			front_line_points[i].sum = front_line_points[i].count = 0;
		}

		for (const auto& particle : particle_list) {
			//auto particle = *particle_it;

			int beg_i = (int)ceil((particle->_y() - steps_start - radius) / step_size);
			int end_i = (int)((particle->_y() - steps_start + radius) / step_size);
			if (beg_i < 0) beg_i = 0;
			if (end_i > steps - 1) end_i = steps - 1;

			for (int i = beg_i; i < end_i; ++i) {
				front_line_points[i].sum += particle->_x();
				front_line_points[i].count++;
			}

		}

		for (int i = 0; i < steps; ++i) {
			front_line_points[i].x = front_line_points[i].sum / front_line_points[i].count;
		}
	}

	void FrontLine::Print(unsigned num) {
		std::ofstream csv(P::csv_folder + "line.csv." + std::to_string(num));

		csv << "x,y";

		for (int i = 0; i < steps; ++i) {
			if (front_line_points[i].count) {
				csv << '\n' << front_line_points[i].x << ',' << front_line_points[i].y;
			}
		}

		csv.close();
	}
}