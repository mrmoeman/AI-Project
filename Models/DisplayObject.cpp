#include "DisplayObject.h"

DisplayObject::DisplayObject(void)
{
	m_model = NULL;
	m_texture = NULL;
}

DisplayObject::~DisplayObject(void)
{
}

bool DisplayObject::Init(ModelClass* model, TextureClass* texture)
{
	if (!model || !texture) return false;

	m_model = model;
	m_texture = texture;

	return true;
}

ModelClass* DisplayObject::GetModel() { return m_model; }
void DisplayObject::SetModel( ModelClass* model_class ) { m_model = model_class; }
int DisplayObject::GetVertexCount() { return m_model->GetVertexCount(); }

TextureClass* DisplayObject::GetTexture() { return m_texture; }
void DisplayObject::SetTexture( TextureClass* texture_class ) { m_texture = texture_class; }

bool DisplayObject::Draw( ID3D11DeviceContext* deviceContext )
{
	return true;
};