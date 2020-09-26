#include "FrontLine.h"
#include <iostream>

namespace ps {

		
	void FrontLine::Init() {
		Init(P->front_line_steps, P->front_line_window, P->area_beg, P->area_end);
	}
	void FrontLine::Init(int a_steps, double _window, double _area_start, double _area_end) {
		area_start = _area_start; 
		area_end = _area_end;
		radius = _window / 2;
		steps_start = area_start + radius;
		steps_end = area_end - radius;
		steps_area = steps_end - steps_start;
		step_size = steps_area / (a_steps - 1.);

		w_percent = a_steps / (area_end - area_start);


		if (steps != a_steps) 
		{
			steps = a_steps;

			delete[]points;
			points = new point[steps];

			delete[]front_line_points;
			front_line_points = new front_line_point[steps];

		}


		if (steps != a_steps || window != _window) {
			for (int i = 0; i < steps; ++i) {
				points[i].x = steps_start + step_size * i;
			}
		}

		///////////


		for (int i = 0; i < steps; ++i) {
			points[i].Vx = P->system_speed(points[i].x) * P->burn_speed;
			points[i].Vx2 = points[i].Vx * points[i].Vx;
		}

		/*for (size_t i = 0; i < steps; i++)
		{
			Vx[i] = P->system_speed(front_line_points[i].x) * P->burn_speed;
			Vx2[i] = Vx[i] * Vx[i];
		}*/


	}


	void FrontLine::Calc(const std::vector <Particle*>& particle_list) {


		for (int i = 0; i < steps; ++i) {
			front_line_points[i] = {};
			front_line_points[i].x = points[i].x;
		}

		for (const auto& particle : particle_list) {
			//auto particle = *particle_it;

			int beg_i = (int) ceil((particle->_x() - steps_start - radius) / step_size);
			int end_i =	(int)floor((particle->_x() - steps_start + radius) / step_size);
			beg_i *= beg_i > 0;
			end_i = end_i * (end_i < steps) + (end_i > steps - 1) * (steps - 1);
			//if (end_i > steps - 1) end_i = steps - 1;

			for (int i = beg_i; i <= end_i; ++i) {
				front_line_points[i].sum += particle->_z();
				front_line_points[i].count++;
			}

		}

		for (int i = 0; i < steps; ++i) {
			if (front_line_points[i].count) {
				front_line_points[i].z += front_line_points[i].sum / front_line_points[i].count;
				front_line_points[i].z -= P->system_speed(points[i].x) / 2;
			}
		}

		FivePointStencil();
	}

	void FrontLine::Calc2(const std::vector <Particle*>& particle_list) {

		for (int i = 0; i < steps; ++i) {
			//Vx[i] = P->system_speed(front_line_points[i].x) * P->burn_speed;
			//Vx2[i] = Vx[i] * Vx[i];
			front_line_points[i].sum = front_line_points[i].count = 0;
			front_line_points[i].z = 0;
		}

		for (const auto& particle : particle_list) {

			int w = (int)floor((particle->_x() - area_start) * w_percent);
			if (particle->_z() < front_line_points[w].z)
			{
				front_line_points[w].count = 1;
				front_line_points[w].z = particle->_z();
				//front_line_points[w].x = particle->_x();
			}

		}

		/*for (int i = 0; i < steps; ++i) {
			front_line_points[i].z = front_line_points[i].sum / front_line_points[i].count;
		}*/

		FivePointStencil();
	}

	void FrontLine::Print(unsigned num) {
		std::ofstream csv(P->csv_folder + "line.csv." + std::to_string(num));
		if (!csv) {
			std::cout << "sheeet";
		}

		std::string output = "x,z,div,div2,Vx2,diff2";

		for (int i = 0; i < steps; ++i) {
			if (front_line_points[i].count) {
				output += fmt::format("\n{},{},{},{},{},{}", points[i].x, front_line_points[i].z, front_line_points[i].div, front_line_points[i].div2, points[i].Vx2, front_line_points[i].diff2);
				//output+= '\n' + std::to_string(front_line_points[i].x) + ',' + std::to_string(front_line_points[i].y) + ',' + std::to_string(front_line_points[i].div);
			}
		}
		csv << output;
		csv.close();
	}

	void FrontLine::FivePointStencil() {
		int h_div = P->front_line_h;// , point_i;
		bool no_neighbors;

		for (int i = P->front_line_h*2; i < steps - P->front_line_h * 2; ++i) {



			/*h_div = (steps / 4. - fabs(fabs(i - (steps-1) / 2.) - steps / 4.)) / 2;
			if (h_div < P->front_line_h.min) h_div = P->front_line_h.min;
			if (h_div > P->front_line_h.max) h_div = P->front_line_h.max;*/

			//std::cout << i+1 << "\t) " << h_div << "\n";
			//continue;

			//skip point if it doesn't have 4 neighbors
			no_neighbors = false;
			for (int j = i - h_div *2; j <= i + h_div *2; j+= h_div) {
				if (!front_line_points[j].count) {
					no_neighbors = true;
					front_line_points[i].div = front_line_points[i].div2 = front_line_points[i].diff2 = 0;
					break;
				}
			}
			if (no_neighbors) continue;

			front_line_points[i].div = front_line_points[i - h_div *2].z - front_line_points[i+ h_div *2].z +
				8 * (front_line_points[i+ h_div].z - front_line_points[i- h_div].z);
			front_line_points[i].div /= h_div * step_size * 12;
			front_line_points[i].div2 = front_line_points[i].div * front_line_points[i].div + 1;

			front_line_points[i].diff2 = (
				- front_line_points[i + h_div * 2].z
				+ 16 * front_line_points[i + h_div].z
				- 30 * front_line_points[i].z
				+ 16 * front_line_points[i - h_div].z
				- front_line_points[i - h_div * 2].z
			) / (12 * h_div * h_div * step_size * step_size);



		}

		//exit(0);
	}
}

