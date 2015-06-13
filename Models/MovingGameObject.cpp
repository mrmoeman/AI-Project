#include "MovingGameObject.h"


MovingGameObject::MovingGameObject(void)
{
	maxSpeed =0.1f;
}


MovingGameObject::~MovingGameObject(void)
{
}

void MovingGameObject::update()
{
	XMVECTOR position = getPosVec();
	XMVECTOR _velocity = getVelocityVec();

		  
	position += _velocity;
	setPosVec(position);
	
	//turn object to face velocity. May need to change if objects are not always facing the dirtection
	// we are moving in. Getting pushed back etc.
	//Rot.x = XMVectorGetZ(XMVector3Normalize(_velocity));
	//setRotVec(XMVECTOR(XMVector3Normalize(_velocity)));
	setRotVec(_velocity);
	//OutputDebugString("updating");
}

void MovingGameObject::setVelocity(XMFLOAT3 _velocity)
{
	velocity = _velocity;
}

void MovingGameObject::setVelocity(XMVECTOR _velocity)
{
	velocity.x = XMVectorGetX(_velocity);
	velocity.y = XMVectorGetY(_velocity);
	velocity.z = XMVectorGetZ(_velocity);
}

void MovingGameObject::setMaxSpeed(float _maxSpeed)
{
	maxSpeed = _maxSpeed;
}

XMVECTOR MovingGameObject::getVelocityVec()
{
	return XMVectorSet(velocity.x, velocity.y, velocity.z, 1);
}

float MovingGameObject::getMaxSpeed()
{
	return maxSpeed;
}

