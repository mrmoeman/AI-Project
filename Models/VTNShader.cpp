////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "VTNShader.h"

//////////////
// INCLUDES //
//////////////
#include <D3DX11async.h>	// for compiling shaders
#include <DxErr.h>			// for handling errors
#include <fstream>			// for outputting error to a file

using namespace std;

VTNShader::VTNShader()
{
	m_vertexShader = NULL;
	m_pixelShader = NULL;
	m_layout = NULL;
	m_sampleState = NULL;
	m_matrixBuffer = NULL;
	m_lightBuffer = NULL;
	m_light = NULL;
}

VTNShader::~VTNShader()
{
}

void VTNShader::Release()
{
	// Release the light constant buffer.
	if(m_light)
	{
		delete m_light;
		m_light = 0;
	}

	// Release the light constant buffer.
	if(m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the matrix constant buffer.
	if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the sampler state.
	if(m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}

void VTNShader::UpdateLight( LightClass * light )
{
	if (!m_light) m_light = new LightClass();
	m_light->SetAmbientColor( light->GetAmbientColor() );
	m_light->SetDiffuseColor( light->GetDiffuseColor() );
	m_light->SetDirection( light->GetDirection() );
}

bool VTNShader::Init( ID3D11Device* device, 
		char* vs_file, char* vs_function, char* vs_model, 
		char* ps_file, char* ps_function, char* ps_model )
{
	HRESULT result;
	ID3D10Blob* errorMessage = NULL;
	ID3D10Blob* vertexShaderBuffer = NULL;
	ID3D10Blob* pixelShaderBuffer = NULL;

	// descriptive elements
	D3D11_INPUT_ELEMENT_DESC inputLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile
	( 
		vs_file,						// name of file
		NULL,							// macro defines					
		NULL,							// includes
		vs_function,					// name of function
		vs_model,						// shader model
		D3D10_SHADER_ENABLE_STRICTNESS, // flags 1
		0,								// flags 2
		NULL,							// thread pump						
		&vertexShaderBuffer,			// shader blob
		&errorMessage,					// error blob
		NULL							// HRESULT
	);

	if(FAILED(result))
	{
		// If the shader failed to compile it should have written something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage( errorMessage, vs_file );
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			std::string estr = "Error!! Could not find the vertex shader file: ";
			estr += vs_file;
			DXTRACE_MSG( estr );
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile
	( 
		ps_file,						// name of file
		NULL,							// macro defines					
		NULL,							// includes
		ps_function,					// name of function
		ps_model,						// shader model
		D3D10_SHADER_ENABLE_STRICTNESS, // flags 1
		0,								// flags 2
		NULL,							// thread pump						
		&vertexShaderBuffer,			// shader blob
		&errorMessage,					// error blob
		NULL							// HRESULT
	);

	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage( errorMessage, ps_file );
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			std::string estr = "Error!! Could not find the pixel shader file: ";
			estr += vs_file;
			DXTRACE_MSG( estr );
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		DXTRACE_MSG( "Error!! Could not create the vertex shader" );
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
	{
		DXTRACE_MSG( "Error!! Could not create the pixel shader" );
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	inputLayout[0].SemanticName = "POSITION";
	inputLayout[0].SemanticIndex = 0;
	inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[0].InputSlot = 0;
	inputLayout[0].AlignedByteOffset = 0;
	inputLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[0].InstanceDataStepRate = 0;

	inputLayout[1].SemanticName = "TEXCOORD";
	inputLayout[1].SemanticIndex = 0;
	inputLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputLayout[1].InputSlot = 0;
	inputLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[1].InstanceDataStepRate = 0;

	inputLayout[2].SemanticName = "NORMAL";
	inputLayout[2].SemanticIndex = 0;
	inputLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[2].InputSlot = 0;
	inputLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(inputLayout) / sizeof(inputLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout
	(
		inputLayout,
		numElements, 
		vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), 
		&m_layout
	);

	if(FAILED(result))
	{
		DXTRACE_MSG( "Error!! Could not create the input layout" );
		return false;
	}

	// Release the vertex & pixel shader buffers
	vertexShaderBuffer->Release(); vertexShaderBuffer = NULL;
	pixelShaderBuffer->Release(); pixelShaderBuffer = NULL;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f; // default
	samplerDesc.MaxAnisotropy = 1; // default
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;// D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if(FAILED(result))
	{
		DXTRACE_MSG( "Error!! Could not create the texture sampler state" );
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
	{
		DXTRACE_MSG( "Error!! Could not create the texture sampler state" );
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void VTNShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, char* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release(); errorMessage = NULL;

	// Output a message
	DXTRACE_MSG( "Error compiling shader.  Check shader-error.txt for message." );

	return;
}

bool VTNShader::Render( ID3D11DeviceContext* device_context, int num_verts, XMMATRIX &world_matrix, XMMATRIX &view_matrix, XMMATRIX &projection_matrix )
{
	// make sure the light has been initialised
	if (!m_light) return false;

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;

	//////////////
	// MATRICES //
	//////////////

	// Lock the constant buffer so it can be written to.
	result = device_context->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = world_matrix;
	dataPtr->view = view_matrix;
	dataPtr->projection = projection_matrix;

	// Unlock the constant buffer.
	device_context->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	device_context->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	////////////
	// LIGHTS //
	////////////

	// Lock the light constant buffer so it can be written to.
	result = device_context->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr2->ambientColor = m_light->GetAmbientColor();
	dataPtr2->diffuseColor = m_light->GetDiffuseColor();
	dataPtr2->lightDirection = m_light->GetDirection();
	dataPtr2->padding = 0.0f;

	// Unlock the constant buffer.
	device_context->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	device_context->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	///////////////////
	// VERTEX LAYOUT //
	///////////////////

	// Set the vertex input layout.
	device_context->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	device_context->VSSetShader(m_vertexShader, NULL, 0);
	device_context->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	device_context->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangles.
	device_context->Draw( num_verts, 0 );

	return true;
}