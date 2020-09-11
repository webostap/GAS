#include "Params.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>


ps::Params::Params() { 
	Read();
}


void ps::Params::Read()
{
	std::ifstream file("params.json");
	assert(file);
	std::string json((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	nlohmann::json j = nlohmann::json::parse(json);

	csv_folder = to_string(j["csv_folder"]);

	area_beg = j["area_beg"]; 
	area_end = j["area_end"]; 
	area_size = area_end - area_beg; 
	area_height = area_size / screen_width * screen_height;

	screen_proportion = screen_width / area_size;
	screen_x_proportion = area_size / screen_width;
	screen_y_proportion = area_height / screen_height;

	stream_beg = j["stream_beg"];
	stream_end = j["stream_end"];
	stream_width = stream_end - stream_beg;
	stream_radius = stream_width / 2;

	area_center = stream_beg + stream_radius;
	L = stream_width;

	scale = j["scale"];
	DSR = L / scale;



	burn_radius = (double)j["burn_radius"] * DSR;
	burn_radius_2 = burn_radius * burn_radius;

	base_particles = (int)j["base_particles"];
	particles_dist = burn_radius / (int)j["particles_dist"];

	//burn_radius_cross = burn_radius + burn_radius * (double)j["burn_fix"];
	burn_radius_cross = burn_radius * (1 + (pow(base_particles, (double)j["burn_fix"])));
	burn_radius_2_cross = burn_radius_cross * burn_radius_cross;

	emitter_begin = (double)j["emitter_begin"] * burn_radius;


	base_speed = (double)j["base_speed"] * burn_radius;

	burn_speed = (double)j["burn_speed"] / burn_radius;

	const_speed = (double)j["const_speed"] * burn_radius;

	iterations = (int)j["iterations"];

	iterate_speed = base_speed / iterations;
	iterate_const = const_speed / iterations;
	iterate_particles = (int)round(base_particles * particle_speed(area_center) / burn_radius_2 / M_PI * L);

	burn_time = (int)j["burn_time"] * iterations;
	sage_time = (int)j["sage_time"] * iterations;
	wave_time = (int)j["wave_time"] * iterations;

	front_line_steps = (int)j["front_line_steps"];
	front_line_window = area_size / (int)j["front_line_window"];
	front_line_h = (int)j["front_line_h"];

	refract_coef = (double)j["refract_coef"];
	refract_offset = (double)j["refract_offset"];

}

