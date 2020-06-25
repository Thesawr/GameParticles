#ifndef HOTNODE_H
#define HOTNODE_H

class ColdNode;
#include "Vect4D.h"

class HotNode
{
public:
	friend class ParticleEmitter;

	HotNode();
	HotNode(const HotNode&);
	HotNode& operator = (const HotNode&);
	~HotNode();
	
	void Update(const float time_elapsed);

private:

	Vect4D	position;
	Vect4D	velocity;
	Vect4D	scale;
	float	life;
	HotNode *next;
	HotNode *prev;
	ColdNode *cold;
};

#endif


