#pragma once
#include "MovingGameObject.h"
class Bullet :
	public MovingGameObject
{
public:
	Bullet(void);
	~Bullet(void);

	Bullet(float _posX, float _posY, float _posZ);

	void update();

	//XMVECTOR velocity;

	// ZS-->04/02/2015 -> 
	// ALEX
	// I noticed that the bullet doesn't move if the tank is not moving
	// Is it just how the bullets are being instantiated in ModelsDemo.cpp ?
};

