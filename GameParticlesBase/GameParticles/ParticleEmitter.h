// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Math\Matrix.h"
#include "Math\Vect4D.h"
#include "Particle.h"

#include <list>

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void SpawnParticle();
	void update();
	void draw();

	void addParticleToList(Particle *p);
	void removeParticleFromList(Particle *p);

	void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);

private:

	Vect4D	start_position;
	Vect4D	start_velocity;

	double	max_life;
	int		max_particles;
	int		last_active_particle;
	double	spawn_frequency;
	double	last_spawn;
	double	last_loop;


	// added for speed efficiency
	int bufferCount;
	Particle *headParticle;
	Vect4D	vel_variance;
	Vect4D	pos_variance;
	double	scale_variance;
	std::list<Particle> drawBuffer;
	std::list<Particle> particle_list;

};

#endif // PARTICLEEMITTER_H
