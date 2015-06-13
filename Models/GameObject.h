#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include "Dx11DemoBase.h"
#include "DisplayObject.h"
#include <xnamath.h>

//static int objNum=0;

#pragma once
class GameObject
{
	// ZS-->04/02/2015 added a DisplayObject*
	DisplayObject *m_display_object;
public:
	GameObject(void); 
	virtual ~GameObject(void);

	//void SetPos();
	void SetPos(XMFLOAT3 objPos);

	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z);
	void SetScale(float scale);
	void SetScale(float x, float y, float z);
	void setPosVec(XMVECTOR _pos);
	void setRotVec(XMVECTOR _rot);

	float getX() const;
	float getY() const;
	float getZ() const;

	float getRX() const;
	float getRY() const;
	float getRZ() const;

	float getSX() const;
	float getSY() const;
	float getSZ() const;

	XMVECTOR getPosVec() const;
	XMVECTOR getRotVec() const;
	//int getObjNum() const;

	//XMMATRIX world;
	XMFLOAT3 Pos;
	XMFLOAT3 Rot;
	XMFLOAT3 Scale;

	// ZS-->04/02/2015 added getter/setter for the DisplayObject*
	void SetDisplayObject( DisplayObject *display_object );
	DisplayObject * GetDisplayObject();
};

#endif

