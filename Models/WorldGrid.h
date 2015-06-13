#ifndef _WORLDGRID_H_
#define _WORLDGRID_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>
#include "Enemy.h"
#include "Chest.h"

class WorldGrid
{
	private:

	ID3D11Buffer *g_vertexBuffer;
	ID3D11Buffer *g_vertexPlayerBuffer;
	ID3D11Buffer *g_vertexExitBuffer;
	ID3D11Buffer *g_vertexHealthBuffer;
	ID3D11Buffer *g_vertexUIBuffer;
	ID3D11Buffer* worldCB_;
	int *basetype;
	bool *collidable;
	bool *entitycollidable;
	int width;
	int height;
	int Paths;
	int Rooms;
	int SpreadingNum;
	int playerDirection;
	int MovementDirection;
	float SmoothXOffset;
	float SmoothYOffset;

	float MyTileWidth;
	float MyTileHeight;

	float MyTileX;
	float MyTileY;
	bool NeedReload;

	int ExitPosX;
	int ExitPosY;

	int WorldType; //1 - snow, 2 - lava
	
	int gridPosX;
	int gridPosY;
	int CurrentEnemyNumber;
	bool fullsize;

	int TempPosition;
	int floorNumber;

	int playerStrength;
	float playerHealth;
	float playerMaxHealth;
	float bufferHealth;
	int playerDefense;
	int Potions;
	bool floorCleared;

	struct PathStruct
	{
		bool Real;
		int Position;
		int Direction; ///up 1, down 2, right 3, left 4
	};

	struct Spreadstruct
	{
		bool Real;
		int Position;
		int newType;
		int oldType;
		int checkType;
	};

	struct NodeStruct
	{
		bool Real;
		int Position;
	};

	struct GridPathDataStruct
	{
		int ParentArrayPosition;
		int PositionX;
		int PositionY;
		bool Real;
	};

	PathStruct* MyPaths;
	PathStruct* MyRooms;
	Spreadstruct* SpreadingNodes;
	NodeStruct* TempPositionArray;

	GridPathDataStruct* PathCalculationArray;

	bool *PathChecked;

	struct VertexStruct
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	WorldGrid(int wid, int heig);
	~WorldGrid(void);

	void InitSnow();
	void InitLava();
	void InitTemple();
	void InitSnowMaze();
	void InitTemplelabyrinth();
	void BuildBuffer(ID3D11Device* device,float TileX, float TileY);
	void BuildPlayerBuffer(ID3D11Device* device,float TileX, float TileY);
	void BuildExitBuffer(ID3D11Device* device,float TileX, float TileY);
	void BuildHealthBuffer(ID3D11Device* device);
	void BuildUIBuffer(ID3D11Device* device);
	void Render(ID3D11DeviceContext* deviceContext);
	void NewRoom(int position);
	void NewPath(int position, int length, int direction);
	void RimDetection(int oldType, int newtype, int checkType);
	void ReplaceAll(int oldtype, int newType);
	void GridUpdate(float dt);
	void AddSpreadingNodes(int old, int neww, int check, int chance);
	void SpreadNodes();
	void PopulateEnemies(int CheckType);
	void PopulateChests(int CheckType);
	void RecalculateEntityCollisionGrid();
	void CalculatePath(int e);
	void AddDots(int oldType, int newType, int chanceNum);
	int MaxLength(int position, int direction);
	int RanInt(int min, int max);
	void LineEdge(int NewType);
	void GenerateRandomPath(int oldType, int newType);
	void SpreadRandomPath(int oldType, int newType, int ArrayStartPosition);
	void StartCellSimulation(int hollowType, int SolidType, int Checktype);
	void SimulationStep(int hollowType, int SolidType);
	void FloodSimulation(int hollowType, int SolidType);
	void GenerateRooms(int oldType, int newType, int MaxRoomNumber, int MaxAttempts, int MaxSize);
	void RemoveDeadEnds(int oldType, int newType);

	float GetXPos();
	float GetYPos();
	bool GetReload();
	int GetFloorNumber();
	float MovementTimer;
	float AttackTimer;

	int GetStrength();
	int GetDefense();
	int GetPotions();
	int GetHealth();
	int GetMaxHealth();
	bool TestingWorldType;

	void GridInteract();
	void GridAttack();
	void GridPotion();
	void GridDown(ID3D11Device* device);
	void GridUp(ID3D11Device* device);
	void GridLeft(ID3D11Device* device);
	void GridRight(ID3D11Device* device);
	void SetFullsize(bool full, ID3D11Device* device,float TileX, float TileY);
	void SetConstantBuffers(ID3D11Device* device);

	void unloadAll();
	void RebuildBuffer(ID3D11Device* device);

	int GetWorldType();

	ID3D11Buffer* const* GetVertexBuffer();
	ID3D11Buffer* const* GetVertexPlayerBuffer();

	int    GetWidth() { return width; }
	int    GetHeight() { return height; }

};

#endif