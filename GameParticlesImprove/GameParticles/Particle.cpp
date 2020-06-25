// ----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------

#include "Particle.h"
#include "Matrix.h"

Particle::Particle()
	: life(0.0f),
	position(0.0f, 0.0f, 0.0f),
	velocity(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f),
	rotation(0.0f),
	rotation_velocity(-0.5f)
{
}

Particle::Particle(const Particle &other)
	: life(other.life),
	position(other.position),
	velocity(other.velocity),
	scale(other.scale),
	rotation(other.rotation),
	rotation_velocity(other.rotation_velocity)
{
}

Particle & Particle::operator=(const Particle &other)
{
	this->life = other.life;
	this->position = other.position;
	this->velocity = other.velocity;
	this->scale = other.scale;
	this->rotation = other.rotation;
	this->rotation_velocity = other.rotation_velocity;
	return *this;
}

Particle::~Particle()
{
	// nothing to do
}

//void Particle::CopyDataOnly(Particle *p)
//{
//	// copy the data only
//	// this way of copying data is more efficient that element by element
//	this->position = p->position;
//	this->velocity = p->velocity;
//	this->scale = p->scale;
//	this->rotation = p->rotation;
//	this->rotation_velocity = p->rotation_velocity;
//	this->life = p->life;
//}

void Particle::Update(const double& time_elapsed)
{
	//// Rotate the matrices
	//Vect4D tmp_Row0;
	//tmp_Row0 = curr_Row0;
	//prev_Row0 = tmp_Row0;

	//Vect4D tmp_Row1;
	//tmp_Row1 = curr_Row1;
	//prev_Row1 = tmp_Row1;

	//Vect4D tmp_Row2;
	//tmp_Row2 = curr_Row2;
	//prev_Row2 = tmp_Row2;

	//Vect4D tmp_Row3;
	//tmp_Row3 = curr_Row3;
	//prev_Row3 = tmp_Row3;

	//Matrix tmp;

	//tmp.set(Matrix::MATRIX_ROW_0, &this->diff_Row0);
	//tmp.set(Matrix::MATRIX_ROW_1, &this->diff_Row1);
	//tmp.set(Matrix::MATRIX_ROW_2, &this->diff_Row2);
	//tmp.set(Matrix::MATRIX_ROW_3, &this->diff_Row3);

	//double MatrixScale = tmp.Determinant();

	// serious math below - magic secret sauce
	life += time_elapsed;
	position = position + (velocity * time_elapsed);
	Vect4D z_axis(0.0f, -0.25f, 1.0f);
	Vect4D v(3, 4, 0);
	position.Cross(z_axis, v);
	v.norm(v);
	position = position + v * 0.05 * life;

	//if (MatrixScale > 1.0)
	//{
	//	MatrixScale = 1.0 / MatrixScale;
	//};

	rotation = rotation + rotation_velocity * time_elapsed *2.01;
}


// End of file


