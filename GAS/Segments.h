#pragma once
#include "Params.hpp"

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

		const Params* P;

	public:

		Segments(const Params&);
		void Update();


		ParticleList all_list;
		std::vector <Particle*> all_will_burn;


	

	public:

		void Fill();
		void Toggle_Fill();


		void CrossParticles();
		void StepParticles();
		void MoveParticles();
		void ClearParticleList();

		void LightsOut();
		void Clear();

		void FinalLoop();


		const void Print(int);


		int Line_Count();
		const void Density_Grid();
		const void Density_Radius();
		const void Max_Radius();


		void RefractParticles();




		void Fill_Sampling();
		void Fill_Grid();
	private:

		bool _fill_one = 0;
		//void (Segments::* fill_func)() = 0;


		void ResetFillGrid();
		double* last_particles = 0;
		int fill_grid_count;

	private:



		void CreateParticle(double x_cord, double z_cord, double p_speed);
		void MoveParticle(Particle *p);
		void StepParticle(Particle *p);
		void BurnParticle(Particle *p);




	public:

		struct Segment {
			bool has_burn = 0, refract = 0;
			std::vector<Particle*> ok_list, burn_list;
		};
		
		Segment* GetSegment(const double x, const double z);
		void BurnSegment(Segment*);
		void UpdateSegments();


		int grid_count_x, grid_count_z, grid_count;


	private:

		Segment **grid = 0, *grid_mem = 0;

		double grid_count_x_percent, grid_count_z_percent;



		void SetSegmentsGrid(double);
		void ClearSegments();

		int GetSegmentX(const double x);
		int GetSegmentZ(const double z);

		void ParticleInBurnSegment(Particle* particle, int seg_x, int seg_z);
		bool CheckSegmentBurn(int seg_x, int seg_z);


		std::vector<std::pair<int, int>> burn_segments;



	private:
		int particles_count = 0;
	public:
		int size() const {
			return particles_count;
		}

		~Segments() {
			delete[]grid;
			delete[]grid_mem;
			delete[]last_particles;
		}




	};


} /* namespace ps */

