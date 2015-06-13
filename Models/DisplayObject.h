////////////////////////////////////////////////////////////////////////////////
// Filename: displayobject.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _DISPLAYOBJECT_H_
#define _DISPLAYOBJECT_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "TextureClass.h"
#include "ModelClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: DisplayObject
////////////////////////////////////////////////////////////////////////////////
class DisplayObject
{
public:
	DisplayObject(void);
	~DisplayObject(void);

	bool Init(ModelClass* model, TextureClass* texture);

	ModelClass* GetModel();
	void SetModel( ModelClass* model_class );

	int GetVertexCount();

	TextureClass* GetTexture();
	void SetTexture( TextureClass* texture_class );

	bool Draw( ID3D11DeviceContext* deviceContext );

private:
	ModelClass* m_model;
	TextureClass* m_texture;
};

#endif

