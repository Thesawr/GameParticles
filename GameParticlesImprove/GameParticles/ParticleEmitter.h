// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Math\Matrix.h"
#include "Math\Vect4D.h"
//#include "Particle.h"
#include "HotNode.h"

#include <list>

class ParticleEmitter
{
public:
	ParticleEmitter();
	ParticleEmitter(const ParticleEmitter&);
	ParticleEmitter& operator=(const ParticleEmitter&);
	~ParticleEmitter();

	void SpawnParticle();
	void update();
	void draw();

	void addParticleToList(HotNode *p);
	void removeParticleFromList(HotNode *p);

	void Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc);

private:

	Vect4D	start_position;
	Vect4D	start_velocity;
	Vect4D	vel_variance;
	Vect4D	pos_variance;

	float	max_life;
	float	spawn_frequency;
	float	last_spawn;
	float	last_loop;
	float	scale_variance;
	int		max_particles;
	int		last_active_particle;
	HotNode *headParticle;

};

#endif // PARTICLEEMITTER_H
