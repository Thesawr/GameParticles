#include "ColdNode.h"



ColdNode::ColdNode()
	: rotation(0.0f), rotation_velocity(-0.5f)
{
}

ColdNode::ColdNode(const ColdNode &other)
	: rotation(other.rotation), rotation_velocity(other.rotation_velocity)
{
}

ColdNode & ColdNode::operator=(const ColdNode &other)
{
	this->rotation = other.rotation;
	this->rotation_velocity = other.rotation_velocity;
	return *this;
}


ColdNode::~ColdNode()
{
}