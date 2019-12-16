#include "FrontLine.h"

namespace ps {

	FrontLine::FrontLine(int steps, double window, unsigned h, double area_start, double area_end) {
		Init(steps, window, h, area_start, area_end);
	}
		
	void FrontLine::Init(int a_steps, double window, unsigned a_h, double area_start, double area_end) {

		steps = a_steps;
		radius = window / 2;
		h = a_h;
		steps_start = area_start + radius;
		steps_end = area_end - radius;
		steps_area = steps_end - steps_start;
		step_size = steps_area / (steps - 1.);

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

		FivePointStencil();
	}

	void FrontLine::Print(unsigned num) {
		std::ofstream csv(P::csv_folder + "line.csv." + std::to_string(num));

		std::string output = "x,y,div";

		for (int i = 0; i < steps; ++i) {
			if (front_line_points[i].count) {
				output += fmt::format("\n{},{},{}", front_line_points[i].x, front_line_points[i].y, front_line_points[i].div);
				//output+= '\n' + std::to_string(front_line_points[i].x) + ',' + std::to_string(front_line_points[i].y) + ',' + std::to_string(front_line_points[i].div);
			}
		}
		csv << output;
		csv.close();
	}

	void FrontLine::FivePointStencil() {
		for (unsigned i = h*2; i < steps-h*2-1; ++i) {

			//skip point if it doesn't have 4 neighbors
			for (unsigned j = i - h*2; j <= i + h*2; j+= h) {
				if (!front_line_points[j].count) {
					continue;
				}
			}

			front_line_points[i].div = front_line_points[i-h*2].x - front_line_points[i+h*2].x +
				8 * (front_line_points[i+h].x - front_line_points[i-h].x);

			front_line_points[i].div /= h * step_size * 12;
			
		}
	}
}

