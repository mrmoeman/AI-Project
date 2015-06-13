#include "GameObject.h"


GameObject::GameObject(void)
{
	//objNum ++;
	//Pos = XMFLOAT3(0.0,0.0,0.0);
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);

	// ZS-->04/02/2015 initialise DisplayObject as NULL
	m_display_object == NULL;
}

GameObject::~GameObject(void)
{
}

//int GameObject::getObjNum ( ) const
//{
//	return objNum;
//}

//void GameObject::SetPos()
//{

//}



void GameObject::SetPos(XMFLOAT3 objPos)
{
	Pos = objPos;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

void GameObject::SetPos(float x, float y, float z)
{
	Pos.x = x;
	Pos.y = y;
	Pos.z = z;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

void GameObject::SetRot(float x, float y, float z)
{
	Rot.x = x;
	Rot.y = y;
	Rot.z = z;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

void GameObject::SetScale(float scale)
{
	Scale.x = scale;
	Scale.y = scale;
	Scale.z = scale;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

void GameObject::SetScale(float x, float y, float z)
{
	Scale.x = x;
	Scale.y = y;
	Scale.z = z;
	//world = XMMatrixTranslation(Pos.x, Pos.y, Pos.z);
}

//ALEX OWEN - 26/01/15 - set the position with a vector
void GameObject::setPosVec(XMVECTOR _pos)
{
	Pos.x = XMVectorGetX(_pos);
	Pos.y = XMVectorGetY(_pos);
	Pos.z = XMVectorGetZ(_pos);

}


//ALEX OWEN - 26/01/15 - set the position with a vector
void GameObject::setRotVec(XMVECTOR _rot)
{
	Rot.x = XMVectorGetX(_rot);
	Rot.y = XMVectorGetY(_rot);
	Rot.z = XMVectorGetZ(_rot);

}



float GameObject::getX() const
{
	return Pos.x;
}

float GameObject::getY() const
{
	return Pos.y;
}

float GameObject::getZ() const
{
	return Pos.z;
}

float GameObject::getRX() const
{
	return Rot.x;
}

float GameObject::getRY() const
{
	return Rot.y;
}

float GameObject::getRZ() const
{
	return Rot.z;
}

float GameObject::getSX() const
{
	return Scale.x;
}

float GameObject::getSY() const
{
	return Scale.y;
}

float GameObject::getSZ() const
{
	return Scale.z;
}


//ALEX OWEN - 26/01/15 - return position as vector
XMVECTOR GameObject::getPosVec() const
{
	return XMVectorSet(Pos.x, Pos.y, Pos.z, 1);
}


//ALEX OWEN - 26/01/15 - return rotation as vector
XMVECTOR GameObject::getRotVec() const
{
	return XMVectorSet(Rot.x, Rot.y, Rot.z, 1);
}

// ZS-->04/02/2015 DisplayObject setter
void GameObject::SetDisplayObject( DisplayObject * display_object )
{
	m_display_object = display_object;
}

// ZS-->04/02/2015 DisplayObject getter
DisplayObject * GameObject::GetDisplayObject()
{
	return m_display_object;
}



