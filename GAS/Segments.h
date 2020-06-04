#pragma once
//#include "Params.inl"
//#include "Particle.h"
#include "FrontLine.h"
#include <list>
#include <vector>
#include <fstream>

namespace ps {

	class Segments {

	/*public:

		struct Params {
			int base_particles;
			double base_speed;
			double burn_radius;

			int iterations;	
		};*/

	/*int base_particles;
	double burn_radius;
	double base_speed;

	int iterations;*/

	//double iterate_speed;
	//double iterate_particles;

	/*int burn_time = 5 * iterations;
	int sage_time = 2 * iterations;*/

	/////////

	/*double burn_radius_2 = burn_radius * burn_radius;

	int grid_count_x = P::area_size / burn_radius;
	int grid_count_z = P::area_height / burn_radius;
	int grid_count = grid_count_x * grid_count_z;

	double grid_count_x_percent = grid_count_x / P::area_size;
	double grid_count_z_percent = grid_count_z / P::area_height;*/

	//Params params;

	public:
		bool is_burn = false;

		Segments();
		//Segments(const Params&);
		//void LoadParams(const Params&);
		//void UpdateParams();

		void Step();
		void FinalLoop(bool move = true);

		//void Fill_Ziggurat();
		void Fill_Sampling();

		int Line_Count();

		//void Lighter();
		void CrossParticles();
		void StepParticles();
		void MoveParticles();
		void ClearParticleList();

		void BurnParticle(Particle *p);


		const void PrintStep(int);
		const void PrintLine(int);

		void LightsOut();

		const void Density_Grid();
		const void Density_Radius();


		bool CheckSegmentBurn(int seg_x, int seg_z);


		//double(*stream_func)(double) = P::x2_stream;

		const double particle_speed(const double x);


		struct Segment {
			//double beg, end;
			bool has_burn = false;
			std::vector<Particle*> ok_list, burn_list;
		};

		Segment** grid = new Segment*[P::grid_count_x];
		Segment* grid_mem = new Segment[P::grid_count];

		std::vector<std::pair<int,int>> burn_segments;


		void UpdateSegments();
		void ClearSegments();

		int GetSegmentX(const double x);
		int GetSegmentZ(const double z);
		Segment* GetSegment(const double x, const double z);

		void ParticleInBurnSegment(Particle* particle, int seg_x, int seg_z);
		void BurnSegment(Segment*);



		std::list <Particle> all_list;
		//std::list <Particle*> all_burn;
		std::list <Particle*> all_will_burn;




		FrontLine m_front_line = FrontLine(P::front_line_steps, P::front_line_window, P::area_beg, P::area_end);
	};


} /* namespace ps */

