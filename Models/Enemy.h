#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <xnamath.h>

class Enemy
{
	private:

		int XPos;
		int YPos;

		int *PathX;
		int *PathY;
		int PathLength;
		float Health;
		float MaxHealth;
		float Bufferhealth;

		int enemyStrength;
		int enemyDefense;
		bool PlayerHit;

		int enemyDirection;
		int bufferDirection;

		int CurrentPathPoint;

		int width;
		int height;

		int state;
		float timer;
		bool NeedPath;
		//0 idle
		//1 wander
		//2 chase
		//3 attack
		//4 flee

		float MovementTimer;
		float SmoothXOffset;
		float SmoothYOffset;

		int Type;
		float Speed;
		int FleeHealth;
		int PlayerRanger;


		ID3D11Buffer *g_vertexBuffer;
		ID3D11Buffer *g_vertexHealthBuffer;
		ID3D11Buffer* worldCB_;

		struct VertexStruct
		{
			XMFLOAT3 position;
			XMFLOAT2 texture;
		};

		void SwitchtoIdle();
		void SwitchtoWander();
		void SwitchtoChase(int playerposition);
		void SwitchChasetoIdle(int playerposition);
		void SwitchtoAttack(int playerposition);
		void SwitchAttacktoChase(int playerposition);
		void SwitchtoFlee(int playerposition);
		void SwitchFleetoAttack();
		void Wander();
		void Chase();
		void Attack(int playerposition);
		void Flee(int playerposition);



		int Up;
		int Down;
		int Left;
		int Right;
		int UpLeft;
		int DownLeft;
		int UpRight;
		int DownRight;
		int Center;


	public:

		Enemy(int wid, int heig, int MyX, int MyY, int FloorNumber);
		Enemy();
		~Enemy(void);

		void BuildBuffer(ID3D11Device* device,float TileX, float TileY);
		void BuildHealthBuffer(ID3D11Device* device,float TileX, float TileY);
		void EnemyUpdate(float dt, int playerposition);
		void Render(ID3D11DeviceContext* deviceContext, float TileWidth, float TileHeight, float GetX, float GetY, bool fullsize);
		int RanInt(int min, int max);
		int GetX();
		int GetY();
		bool RequiresPath();

		void UpdateSurroudings(int u, int d, int l, int r, int ul, int ur, int dl, int dr, int c);

		void UpdatePath();
		void PlayerMoved();

		void SetPathLength(int length);
		void AddPathPoint(int Position, int X, int Y);

		float GetHealth();
		void SetHealth(int NewHealth);
		void unloadAll();

		bool getPlayerHit();
		int GetDefense();
		int GetStrength();
		void PlayerHitOff();

		void RebuildBuffer(ID3D11Device* device,float TileX, float TileY);
		void SetConstantBuffer(ID3D11Device* device);


};

#endif