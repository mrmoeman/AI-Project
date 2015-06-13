#include "Bullet.h"


Bullet::Bullet(void)
{
	MovingGameObject::setMaxSpeed(0.03);
}


Bullet::~Bullet(void)
{
}

Bullet::Bullet(float _posX, float _posY, float _posZ)
{

	SetPos(_posX, _posY, _posZ);
}


void Bullet::update()
{
	MovingGameObject::update();

	
}

