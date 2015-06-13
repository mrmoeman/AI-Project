#include "Chest.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Chest::Chest(int wid, int heig, int MyX, int MyY){
	XPos =  MyX;
	YPos = MyY;
	width = wid;
	height = heig;
	Opened = false;
	bufferOpened = Opened;

	Contents = RanInt(0,4);
	Amount = RanInt(0,3) + 1;
}

Chest::Chest(){}

Chest::~Chest(void){}

void Chest::BuildBuffer(ID3D11Device* device,float TileX, float TileY){

	bufferOpened = Opened;

	float TileWidth = 2/TileX;
	float TileHeight = 2/TileY;

	VertexStruct* vertices = new VertexStruct[6];

	vertices[0].position = XMFLOAT3( -1 + TileWidth, 1 , 1.0f );
	vertices[1].position = XMFLOAT3( -1, 1 - TileHeight, 1.0f );
	vertices[2].position = XMFLOAT3( -1, 1, 1.0f );
			
	vertices[3].position = XMFLOAT3( -1 + TileWidth, 1, 1.0f );
	vertices[4].position = XMFLOAT3( -1 + TileWidth, 1 - TileHeight, 1.0f );
	vertices[5].position = XMFLOAT3( -1, 1 - TileHeight, 1.0f );

	float XDiff = 1.0f/20.0f;
	float YDiff = 0.05f;

	int XPos;
	int YPos;
	if(Opened == false){
		XPos = 7;
		YPos = 11;
	}
	if(Opened == true){
		XPos = 7;
		YPos = 12;
	}


	vertices[0].texture = XMFLOAT2(XPos*XDiff,(YPos-1)*YDiff);
	vertices[1].texture = XMFLOAT2((XPos-1)*XDiff,YPos*YDiff);
	vertices[2].texture = XMFLOAT2((XPos-1)*XDiff,(YPos-1)*YDiff);
			
	vertices[3].texture = XMFLOAT2(XPos*XDiff,(YPos-1)*YDiff);
	vertices[4].texture = XMFLOAT2(XPos*XDiff,YPos*YDiff);
	vertices[5].texture = XMFLOAT2((XPos-1)*XDiff,YPos*YDiff);



	// create a D3D11_BUFFER_DESC struct that describes the buffer data
	D3D11_BUFFER_DESC vertex_desc;
	ZeroMemory( &vertex_desc, sizeof( vertex_desc ) );
	vertex_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_desc.ByteWidth = sizeof( VertexStruct) * (6);

	// create a D3D11_SUBRESOURCE_DATA struct to hold the buffer data
	D3D11_SUBRESOURCE_DATA resource_data;
	ZeroMemory( &resource_data, sizeof( resource_data ) );
	resource_data.pSysMem = vertices;
	
	// using the current device context, generate the vertex buffer
	HRESULT d3dResult = device->CreateBuffer( &vertex_desc, &resource_data, &g_vertexBuffer );


	// delete the Vertex array
	delete[] vertices;

	//constant world matrix buffer

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = device->CreateBuffer( &constDesc, 0, &worldCB_ );

}

void Chest::Render(ID3D11DeviceContext* deviceContext, float TileWidth, float TileHeight, float GetX, float GetY, bool fullsize){

	// Puts the vertex and index buffers on the graphics pipeline
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexStruct); 
	offset = 0;

	XMMATRIX position_mat = XMMatrixTranslation(0, 0, 0);
	XMMATRIX scale_mat = XMMatrixScaling(1,1,1);
	XMMATRIX rotation_mat = XMMatrixRotationRollPitchYaw(0,0,0);
	if(fullsize == true){
		position_mat = XMMatrixTranslation((TileWidth * XPos), -(TileHeight * YPos), 0);
	}
	else{
		position_mat = XMMatrixTranslation((TileWidth * XPos) + GetX , -(TileHeight * YPos) + GetY, 0);
	}
	XMMATRIX world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );
	deviceContext->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &worldCB_ );

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->Draw(6, 0 );

}

void Chest::unloadAll(){
	g_vertexBuffer->Release();
	g_vertexBuffer = NULL;
	worldCB_->Release();
	worldCB_ = NULL;
}

void Chest::RebuildBuffer(ID3D11Device* device,float TileX, float TileY){
	if(bufferOpened != Opened){
		BuildBuffer(device, TileX,  TileY);
	}
}

void Chest::Open(){
	Opened = true;
}

int Chest::GetX(){
	return XPos;
}

int Chest::GetY(){
	return YPos;
}

int Chest::RanInt(int min, int max){

	return rand() %(max-min) + min;

}

int Chest::GetContents(){
	return Contents;
}

int Chest::GetAmount(){
	return Amount;
}

bool Chest::GetOpen(){
	return Opened;
}
	