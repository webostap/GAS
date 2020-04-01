#pragma once
#include "Params.inl"
#include "Particle.h"
#include "FrontLine.h"
#include <list>
#include <vector>
#include <fstream>

namespace ps {

	class Segments {

	public:
		bool is_burn = false;

	public:
		Segments();

		void Step();

		void Fill_Ziggurat();
		void Fill_Sampling();

		void Lighter();
		void BurnParticles();
		void MoveParticles();
		void ClearParticleList();

		void BurnParticle(Particle *p);


		const void PrintStep(size_t);

		void LightsOut();


		bool CheckSegmentBurn(int seg_x, int seg_z);


		double(*stream_func)(double) = P::log_stream;

		const double particle_speed(const double x);


		struct Segment {
			double beg, end;
			bool has_burn = false;
			std::vector<Particle*> ok_list, burn_list;
		};

		Segment** grid = new Segment*[P::grid_count_x];
		Segment* grid_mem = new Segment[P::grid_count];

		std::vector<std::pair<int,int>> burn_segments;


		void UpdateSegments();
		void ClearSegments();

		unsigned GetSegmentX(const double x);
		unsigned GetSegmentZ(const double z);
		Segment* GetSegment(const double x, const double z);

		void ParticleInBurnSegment(Particle* particle, int seg_x, int seg_z);
		void BurnSegment(Segment*);



		std::list <Particle> all_list;
		//std::list <Particle*> all_burn;
		std::list <Particle*> all_will_burn;




		FrontLine m_front_line = FrontLine(P::front_line_steps, P::front_line_window, P::area_beg, P::area_end);
	};


} /* namespace ps */

