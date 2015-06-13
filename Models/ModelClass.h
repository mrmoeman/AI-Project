////////////////////////////////////////////////////////////////////////////////
// Filename: ModelClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;	////GA
		XMFLOAT3 bitangent;	////GA
	};

public:
	ModelClass(void);
	~ModelClass(void);

	bool Init(ID3D11Device* device, char* modelFilename);
	void Release(void);

	ID3D11Buffer* const* GetVertexBuffer();
	int GetVertexCount(void);

	void Render(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer *m_vertexBuffer;
	int m_vertexCount;;
};

#endif