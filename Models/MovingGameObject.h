#pragma once
#include "gameobject.h"
class MovingGameObject :
	public GameObject
{
public:
	MovingGameObject(void);
	~MovingGameObject(void);

	void update();
	void setVelocity(XMFLOAT3 _velocity);
	void setVelocity(XMVECTOR _velocity);
	void setMaxSpeed(float _maxSpeed);
	XMVECTOR getVelocityVec();
	float getMaxSpeed();
private:
	XMFLOAT3 velocity;
	float maxSpeed;
};

