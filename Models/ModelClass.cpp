////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ModelClass.h"
#include "objLoader.h"


ModelClass::ModelClass(void)
{
	m_vertexBuffer =  NULL;
}

ModelClass::~ModelClass(void)
{
}

bool ModelClass::Init(ID3D11Device* device, char* obj_file_name)
{
	bool result;

	// create an ObjModel object to load the model from the file
	// the ObjModel object will be automatically destroyed 
	ObjModel obj_model;

	// try to load the ObjModel data from the file
	result = obj_model.LoadOBJ( obj_file_name );
	if (!result) return false;

	// get the total number of vertices from the ObjModel object
	m_vertexCount = obj_model.GetTotalVerts( );

	// create an array of Vertex structs to hold the converted model data
	VertexType * vertices = new VertexType[m_vertexCount];

	// get pointers to the arrays within the ObjModel object
	float* v_ptr = obj_model.GetVertices( );
	float* tc_ptr = obj_model.GetTexCoords( );
	float* n_ptr = obj_model.GetNormals( );
	float* t_ptr = obj_model.GetTangents( );	////GA
	float* b_ptr = obj_model.GetBitangents( );	////GA

	// loop through the ObjModel object arrays to initialise the Vertex array
	for( int i = 0; i < m_vertexCount; i++ )
	{
		// vertices
		vertices[i].position = XMFLOAT3( *(v_ptr + 0), *(v_ptr + 1), *(v_ptr + 2) );
		v_ptr += 3;

		// texture coordinates
		vertices[i].texture = XMFLOAT2( *(tc_ptr + 0), *(tc_ptr + 1) );
		tc_ptr += 2;
		
		// normals
		vertices[i].normal = XMFLOAT3( *(n_ptr + 0), *(n_ptr + 1), *(n_ptr + 2) );
		n_ptr += 3;

		//tangents GA
		vertices[i].tangent = XMFLOAT3( *(t_ptr + 0), *(t_ptr + 1), *(t_ptr + 2) );
		t_ptr += 3;

		//bitangents GA
		vertices[i].bitangent = XMFLOAT3( *(b_ptr + 0), *(b_ptr + 1), *(b_ptr + 2) );
		b_ptr += 3;
	}

	// create a D3D11_BUFFER_DESC struct that describes the buffer data
	D3D11_BUFFER_DESC vertex_desc;
	ZeroMemory( &vertex_desc, sizeof( vertex_desc ) );
	vertex_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_desc.ByteWidth = sizeof( VertexType ) * m_vertexCount;

	// create a D3D11_SUBRESOURCE_DATA struct to hold the buffer data
	D3D11_SUBRESOURCE_DATA resource_data;
	ZeroMemory( &resource_data, sizeof( resource_data ) );
	resource_data.pSysMem = vertices;
	
	// using the current device context, generate the vertex buffer
	HRESULT d3dResult = device->CreateBuffer( &vertex_desc, &resource_data, &m_vertexBuffer );

	// delete the Vertex array
	delete[] vertices;

	// return false if the buffer generation failed
	if( FAILED( d3dResult ) )
	{
		return false;
	}
	
	// the buffer was successfully generated so return true
	return true;
}

void ModelClass::Release(void)
{
	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}

	return;
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Puts the vertex and index buffers on the graphics pipeline

	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

ID3D11Buffer* const* ModelClass::GetVertexBuffer() { return &m_vertexBuffer; }

int ModelClass::GetVertexCount(void) { return m_vertexCount; }