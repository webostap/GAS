#include "FrontLine.h"
#include <iostream>

namespace ps {

	FrontLine::FrontLine(int steps, double window, double area_start, double area_end) {
		Init(steps, window, area_start, area_end);
	}
		
	void FrontLine::Init(int a_steps, double window, double area_start, double area_end) {

		steps = a_steps;
		radius = window / 2;
		steps_start = area_start + radius;
		steps_end = area_end - radius;
		steps_area = steps_end - steps_start;
		step_size = steps_area / (steps - 1.);

		front_line_points = new front_line_point[steps];

		for (int i = 0; i < steps; ++i) {
			front_line_points[i].x = steps_start + step_size * i;
		}

		///////////
		Vx = new double[steps];
		Vx2 = new double[steps];

		for (size_t i = 0; i < steps; i++)
		{
			Vx[i] = P::particle_speed(front_line_points[i].x) * P::burn_speed;
			Vx2[i] = Vx[i] * Vx[i];
		}

	}


	void FrontLine::Calc(std::list <Particle*>& particle_list) {

		for (int i = 0; i < steps; ++i) {
			front_line_points[i].sum = front_line_points[i].count = 0;
		}

		for (const auto& particle : particle_list) {
			//auto particle = *particle_it;

			int beg_i = (int)ceil((particle->_x() - steps_start - radius) / step_size);
			int end_i =	(int)	 ((particle->_x() - steps_start + radius) / step_size);
			if (beg_i < 0) beg_i = 0;
			if (end_i > steps - 1) end_i = steps - 1;

			for (int i = beg_i; i <= end_i; ++i) {
				front_line_points[i].sum += particle->_z();
				front_line_points[i].count++;
			}

		}

		for (int i = 0; i < steps; ++i) {
			front_line_points[i].z = front_line_points[i].sum / front_line_points[i].count;
		}

		FivePointStencil();
	}

	void FrontLine::Print(unsigned num) {
		std::ofstream csv(P::csv_folder + "line.csv." + std::to_string(num));

		std::string output = "x,z,div,div2,V";

		for (int i = 0; i < steps; ++i) {
			if (front_line_points[i].count) {
				output += fmt::format("\n{},{},{},{},{}", front_line_points[i].x, front_line_points[i].z, front_line_points[i].div, front_line_points[i].div2, Vx[i]);
				//output+= '\n' + std::to_string(front_line_points[i].x) + ',' + std::to_string(front_line_points[i].y) + ',' + std::to_string(front_line_points[i].div);
			}
		}
		csv << output;
		csv.close();
	}

	void FrontLine::FivePointStencil() {
		int h_div = P::front_line_h.max, point_i;
		bool no_neighbors = false;

		for (int i = P::front_line_h.min*2; i < steps - P::front_line_h.min * 2; ++i) {



			h_div = (steps / 4. - fabs(fabs(i - (steps-1) / 2.) - steps / 4.)) / 2;
			if (h_div < P::front_line_h.min) h_div = P::front_line_h.min;
			if (h_div > P::front_line_h.max) h_div = P::front_line_h.max;

			//std::cout << i+1 << "\t) " << h_div << "\n";
			//continue;

			//skip point if it doesn't have 4 neighbors
			no_neighbors = false;
			for (int j = i - h_div *2; j <= i + h_div *2; j+= h_div) {
				if (!front_line_points[j].count) {
					no_neighbors = true;
				}
			}
			if (no_neighbors) continue;

			front_line_points[i].div = front_line_points[i - h_div *2].z - front_line_points[i+ h_div *2].z +
				8 * (front_line_points[i+ h_div].z - front_line_points[i- h_div].z);

			front_line_points[i].div /= h_div * step_size * 12;

			front_line_points[i].div2 = fabs(front_line_points[i].div) + .5;

			front_line_points[i].diff2 = front_line_points[i].div * front_line_points[i].div+1;

		}

		//exit(0);
	}
}

