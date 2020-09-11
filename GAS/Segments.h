#pragma once
#include "Params.hpp"
//#include "Parametrs.inl"
//#include "Particle.h"
#include "FrontLine.h"
#include <forward_list>
//#include <list>
#include <vector>
#include <fstream>
#include <random>

typedef std::forward_list <ps::Particle> ParticleList;

namespace ps {

	class Segments {


	public:

		const Params* P;
		Segments(const Params&);



		int size = 0;

		double area_beg = P->area_beg, area_end = P->area_end;


		double burn_radius = P->burn_radius_cross;
		double burn_radius_2 = P->burn_radius_2_cross;

		double burn_radius_center = 0, burn_radius_center_2 = 0;

		int grid_count_x, grid_count_z, grid_count;
		double grid_count_x_percent, grid_count_z_percent;


	public:
		bool is_burn = false;
		//void LoadParams(const Params&);
		//void UpdateParams();
		
		void Step();
		void FinalLoop();

		//void Fill_Ziggurat();
		void Fill_Sampling();
		void Fill_Grid();

		void SetFillGrid(double);

		double particles_dist, *last_particles = 0;
		int fill_grid_count;


		void (Segments::*fill_func)(void) = 0;
		void Fill();
		void Toggle_Fill();
		bool _fill_one = 0;

		int Line_Count();

		//void Lighter();
		void CrossParticles();
		void StepParticles();
		void MoveParticles();
		void ClearParticleList();

		void CreateParticle(double x_cord, double z_cord, double p_speed);
		void MoveParticle(Particle *p);
		void StepParticle(Particle *p);
		void BurnParticle(Particle *p);

		void RefractParticles();


		const void PrintStep(int);
		const void PrintLine(int);
		const void CalcLine();

		void LightsOut();
		void Clear();

		const void Density_Grid();
		const void Density_Radius();
		const void Max_Radius();


		bool CheckSegmentBurn(int seg_x, int seg_z);


		//double(*stream_func)(double) = P::x2_stream;

		const double particle_speed(const double x);


		struct Segment {
			//double beg, end;
			bool has_burn = false, refract = false;
			std::vector<Particle*> ok_list, burn_list;
		};

		Segment **grid = 0, *grid_mem = 0;

		std::vector<std::pair<int,int>> burn_segments;


		void SetBurnRadius(double _burn_radius);
		void SetGrid(double seg_size);
		void UpdateSegments();
		void ClearSegments();

		int GetSegmentX(const double x);
		int GetSegmentZ(const double z);
		Segment* GetSegment(const double x, const double z);

		void ParticleInBurnSegment(Particle* particle, int seg_x, int seg_z);
		void BurnSegment(Segment*);



		ParticleList all_list;
		//std::list <Particle*> all_burn;
		std::vector <Particle*> all_will_burn;

		~Segments() {
			delete[]grid;
			delete[]grid_mem;
			delete[]last_particles;
		}




		FrontLine m_front_line = FrontLine(P);
	};


} /* namespace ps */

