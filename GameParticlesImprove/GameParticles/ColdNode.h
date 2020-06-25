#ifndef COLDNODE_H
#define COLDNODE_H

#include "Vect4D.h"

class ColdNode
{
public:
	friend class HotParticle;
	friend class ParticleEmitter;

	ColdNode();
	ColdNode(const ColdNode&);
	ColdNode& operator = (const ColdNode&);
	~ColdNode();

	float	rotation;
	float	rotation_velocity;
};

#endif

