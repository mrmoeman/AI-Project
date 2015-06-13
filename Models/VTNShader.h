////////////////////////////////////////////////////////////////////////////////
// Filename: VTNShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _VTNSHADER_H_
#define _VTNSHADER_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <xnamath.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "LightClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: LightShaderClass
////////////////////////////////////////////////////////////////////////////////
class VTNShader
{
private:

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection; float padding;
	};

public:
	VTNShader();
	~VTNShader();

	bool Init( ID3D11Device* device, 
		char* vs_file, char* vs_function, char* vs_model, 
		char* ps_file, char* ps_function, char* ps_model );
	void Release();

	void UpdateLight( LightClass * light );
	bool Render( ID3D11DeviceContext* device_context, int num_verts, XMMATRIX &world_matrix, XMMATRIX &view_matrix, XMMATRIX &projection_matrix );

private:
	void OutputShaderErrorMessage( ID3D10Blob* error_message, char* shader_file_name );

private:
	ID3D11VertexShader* m_vertexShader;	// vertex shader on G-Card
	ID3D11PixelShader* m_pixelShader;	// pixel shader on G-Card
	ID3D11InputLayout* m_layout;		// layout of vertices
	ID3D11SamplerState* m_sampleState;	// texture sampler
	ID3D11Buffer* m_matrixBuffer;		// world/view/projection matrices on G-Card
	ID3D11Buffer* m_lightBuffer;		// ambient light data on G-Card

	LightClass *m_light; // ambient light
};

#endif




