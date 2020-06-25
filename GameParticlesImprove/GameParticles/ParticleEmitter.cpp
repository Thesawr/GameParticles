// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//-----------------------------------------------------------------------------

#include "DO_NOT_MODIFY\Timer.h"
#include "DO_NOT_MODIFY\GlobalTimer.h"
#include "DO_NOT_MODIFY\OpenGLInterface.h"

#include <assert.h>

#define UNUSED_VAR(v) ((void *)v)
#include "ParticleEmitter.h"
#include "Settings.h"
#include "HotNode.h"
#include "ColdNode.h"

static const unsigned char squareColors[] =
{
	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
	200,  200,  0,  255,
};

static const float squareVertices[] =
{
	-0.015,  -0.015, 0.0,
	 0.015,  -0.015, 0.0,
	-0.015,   0.015, 0.0,
	 0.015,   0.015, 0.0,
};

ParticleEmitter::ParticleEmitter()
	: start_position(0.0, 0.0, 0.0),
	start_velocity(0.0, 1.0, 0.0),
	max_life(MAX_LIFE),
	max_particles(NUM_PARTICLES),
	spawn_frequency(0.0000001),
	last_spawn(globalTimer::getTimerInSec()),
	last_loop(globalTimer::getTimerInSec()),
	last_active_particle(-1),
	//particle_list(NUM_PARTICLES),
	vel_variance(1.0, 4.0, 0.4),
	pos_variance(1.0, 1.0, 1.0),
	scale_variance(2.5),
	headParticle(0)
{
	// nothing to do
}

ParticleEmitter::ParticleEmitter(const ParticleEmitter &other)
	: start_position(other.start_position),
	start_velocity(other.start_velocity),
	max_life(other.max_life),
	max_particles(other.max_particles),
	spawn_frequency(other.spawn_frequency),
	last_spawn(other.last_spawn),
	last_loop(other.last_loop),
	last_active_particle(other.last_active_particle),
	vel_variance(other.vel_variance),
	pos_variance(other.pos_variance),
	scale_variance(other.scale_variance),
	headParticle(other.headParticle)
{
}

ParticleEmitter & ParticleEmitter::operator=(const ParticleEmitter &other)
{
	start_position = other.start_position;
	start_velocity = other.start_velocity;
	max_life = other.max_life;
	max_particles = other.max_particles;
	spawn_frequency = other.spawn_frequency;
	last_spawn = other.last_spawn;
	last_loop = other.last_loop;
	last_active_particle = other.last_active_particle;
	vel_variance = other.vel_variance;
	pos_variance = other.pos_variance;
	scale_variance = other.scale_variance;
	headParticle = other.headParticle;
	return *this;
}

ParticleEmitter::~ParticleEmitter()
{
	// do nothing
}

void ParticleEmitter::SpawnParticle()
{
	// create another particle if there are ones free
	if (last_active_particle < max_particles - 1)
	{

		// create new particle
		/*Particle *newParticle = new Particle();*/
		HotNode *newParticle = new HotNode();
		ColdNode *newCold = new ColdNode();
		newParticle->cold = newCold;

		// initialize the particle
		newParticle->life = 0.0f;
		newParticle->position = start_position;
		newParticle->velocity = start_velocity;
		newParticle->scale = Vect4D(2.0, 2.0, 2.0, 1.0);

		// apply the variance
		this->Execute(newParticle->position, newParticle->velocity, newParticle->scale);

		// increment count
		last_active_particle++;

		// add to list
		this->addParticleToList(newParticle);

	}
}

void ParticleEmitter::update()
{
	// get current time
	float current_time = globalTimer::getTimerInSec();

	// spawn particles
	float time_elapsed = current_time - last_spawn;

	// update
	while (spawn_frequency < time_elapsed)
	{
		// spawn a particle
		this->SpawnParticle();
		// adjust time
		time_elapsed -= spawn_frequency;
		// last time
		last_spawn = current_time;
	}

	// total elapsed
	time_elapsed = current_time - last_loop;

	/*Particle *p = this->headParticle;*/
	HotNode *p = this->headParticle;
	// walk the particles

	while (p != 0)
	{
		// call every particle and update its position 
		p->Update(time_elapsed);

		// if it's live is greater that the max_life 
		// and there is some on the list
		// remove node
		if ((p->life > max_life) && (last_active_particle > 0))
		{
			// particle to remove
			/*Particle *s = p;*/
			HotNode *s = p;

			// need to squirrel it away.
			p = p->next;

			// remove last node
			this->removeParticleFromList(s);

			// update the number of particles
			last_active_particle--;
		}
		else
		{
			// increment to next point
			p = p->next;
		}
	}

	last_loop = current_time;
}

void ParticleEmitter::addParticleToList(HotNode * p)
{
	assert(p);
	if (this->headParticle == 0)
	{ // first on list
		this->headParticle = p;
		p->next = 0;
		p->prev = 0;
	}
	else
	{ // add to front of list
		headParticle->prev = p;
		p->next = headParticle;
		p->prev = 0;
		headParticle = p;
	}
}

void ParticleEmitter::removeParticleFromList(HotNode * p)
{
	// make sure we are not screwed with a null pointer
	assert(p);

	if (p->prev == 0 && p->next == 0)
	{ // only one on the list
		this->headParticle = 0;
	}
	else if (p->prev == 0 && p->next != 0)
	{ // first on the list
		p->next->prev = 0;
		this->headParticle = p->next;
	}
	else if (p->prev != 0 && p->next == 0)
	{ // last on the list 
		p->prev->next = 0;
	}
	else//( p->next != 0  && p->prev !=0 )
	{ // middle of the list
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}

	// bye bye
	delete p;
}

void ParticleEmitter::draw()
{
	// get the camera matrix from OpenGL
	// need to get the position
	Matrix cameraMatrix;

	// get the camera matrix from OpenGL
	glGetDoublev(GL_MODELVIEW_MATRIX, reinterpret_cast<double*>(&cameraMatrix));

	// get the position from this matrix
	Vect4D camPosVect;
	cameraMatrix.get(Matrix::MATRIX_ROW_3, &camPosVect);

	// OpenGL goo... don't worrry about this
	glVertexPointer(3, GL_FLOAT, 0, squareVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, squareColors);
	glEnableClientState(GL_COLOR_ARRAY);

	// camera position
	Matrix transCamera;
	transCamera.setTransMatrix(&camPosVect);

	HotNode *it = headParticle;
	while (it != 0)
	{
		//Temporary matrix
		Matrix tmp;

		// particle position
		Matrix transParticle;
		transParticle.setTransMatrix(&it->position);

		// rotation matrix
		Matrix rotParticle;
		/*rotParticle.setRotZMatrix(it->rotation);*/
		rotParticle.setRotZMatrix(it->cold->rotation);

		// scale Matrix
		Matrix scaleMatrix;
		scaleMatrix.setScaleMatrix(&it->scale);

		// total transformation of particle
		tmp = scaleMatrix * transCamera * transParticle * rotParticle * scaleMatrix;

		// set the transformation matrix
		glLoadMatrixd(reinterpret_cast<double*>(&(tmp)));

		// draw the trangle strip
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		it = it->next;
	}
}

void ParticleEmitter::Execute(Vect4D& pos, Vect4D& vel, Vect4D& sc)
{
	// Add some randomness...

	// Ses it's ugly - I didn't write this so don't bitch at me
	// Sometimes code like this is inside real commerical code ( so now you know how it feels )

	// x - variance
	float var = static_cast<float>(rand() % 1000) * 0.001f;
	float sign = static_cast<float>(rand() % 2);
	float *t_pos = reinterpret_cast<float*>(&pos);
	float *t_var = &pos_variance[x];
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// y - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// z - variance
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	// x  - add velocity
	t_pos = &vel[x];
	t_var = &vel_variance[x];
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// y - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// z - add velocity
	var = static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);
	t_pos++;
	t_var++;
	if (sign == 0)
	{
		var *= -1.0;
	}
	*t_pos += *t_var * var;

	// correct the sign
	var = 2.0f * static_cast<float>(rand() % 1000) * 0.001f;
	sign = static_cast<float>(rand() % 2);

	if (sign == 0)
	{
		var *= -1.0;
	}
	sc = sc * var;
}


// End of file