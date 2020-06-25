#include "HotNode.h"
#include "ColdNode.h"

HotNode::HotNode()
	:position(0.0f, 0.0f, 0.0f),
	velocity(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f),
	life(0.0f)
{
}

HotNode::HotNode(const HotNode &other)
	:position(other.position),
	velocity(other.velocity),
	scale(other.scale),
	life(other.life)
{
}

HotNode& HotNode::operator=(const HotNode &other)
{
	this->position = other.position;
	this->velocity = other.velocity;
	this->scale = other.scale;
	this->life = other.life;
	this->cold = other.cold;
	return *this;
}


HotNode::~HotNode()
{
}

void HotNode::Update(const float time_elapsed)
{
	life += time_elapsed;
	position = position + (velocity * time_elapsed);
	Vect4D z_axis(0.0f, -0.25f, 1.0f);
	Vect4D v(3, 4, 0);
	position.Cross(z_axis, v);
	v.norm(v);
	position = position + v * 0.05 * life;
	cold->rotation += cold->rotation_velocity * time_elapsed *2.01f;
}
