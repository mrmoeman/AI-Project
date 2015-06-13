////////////////////////////////////////////////////////////////////////////////
// Filename: TextureClass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "TextureClass.h"


TextureClass::TextureClass(void)
{
	m_texture = NULL;
}

TextureClass::~TextureClass(void)
{
}

bool TextureClass::Init(ID3D11Device* device, char* filename)
{
	HRESULT result;

	// Load the texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureClass::Release()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = NULL;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}

void TextureClass::SetTexture(ID3D11ShaderResourceView* texture)
{
	m_texture = texture;
}

void TextureClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &m_texture);
}
