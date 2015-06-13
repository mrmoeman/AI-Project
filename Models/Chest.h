#ifndef _CHEST_H_
#define _CHEST_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>

class Chest
{

	private:
	ID3D11Buffer *g_vertexBuffer;
	ID3D11Buffer* worldCB_;

	int XPos;
	int YPos;
	int width;
	int height;
	bool Opened;
	bool bufferOpened;

	int Contents; //0 - health up, 1 - strength up, 2 - defense up, 3 - health potion
	int Amount;

	struct VertexStruct
		{
			XMFLOAT3 position;
			XMFLOAT2 texture;
		};


	public:

	Chest(int wid, int heig, int MyX, int MyY);
	Chest();
	~Chest(void);

	int GetX();
	int GetY();
	int RanInt(int min, int max);

	void BuildBuffer(ID3D11Device* device,float TileX, float TileY);
	void Render(ID3D11DeviceContext* deviceContext, float TileWidth, float TileHeight, float GetX, float GetY, bool fullsize);
	void RebuildBuffer(ID3D11Device* device,float TileX, float TileY);
	void unloadAll();
	void Open();

	int GetContents();
	int GetAmount();
	bool GetOpen();

};

#endif