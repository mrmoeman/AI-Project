#include "Enemy.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Enemy::Enemy(int wid, int heig, int MyX, int MyY, int FloorNumber){
	FloorNumber++;
	width = wid;
	height = heig;
	XPos = MyX;
	YPos = MyY;

	state = 0;
	timer = (RanInt(0,1000))/1000.0f;
	MovementTimer = 1;

	Up = 0;
	Down = 0;
	Left = 0;
	Right = 0;
	UpLeft = 0;
	DownLeft = 0;
	UpRight = 0;
	DownRight = 0;
	Center = 0;
	NeedPath = false;
	enemyDirection = 1;
	bufferDirection = 1;

	CurrentPathPoint = 0;

	Speed = 1;
	FleeHealth = 4;
	Health = 7;
	enemyDefense = 0;
	enemyStrength = 2;

	Type = RanInt(0,3);
	if(Type == 0){
		Speed = 1;
		FleeHealth = 4;
		PlayerRanger = 10;
		Health = 7 + FloorNumber*3;
		enemyDefense = (FloorNumber-1) * 2;
		enemyStrength = 2 * FloorNumber;
	}
	if(Type == 1){
		Speed = 0.5f;
		FleeHealth = 0;
		PlayerRanger = 20;
		Health = (7 + FloorNumber*3)*2;
		enemyDefense = (FloorNumber-1) * 2.5;
		enemyStrength = FloorNumber;
	}
	if(Type == 2){
		Speed = 2;
		FleeHealth = 3;
		PlayerRanger = 5;
		Health = (7 + FloorNumber*3)/2;
		enemyDefense = FloorNumber-1;
		enemyStrength = 3 * FloorNumber;
	}

	
	MaxHealth = Health;
	Bufferhealth = Health;
	PlayerHit = false;

}

Enemy::Enemy(){}

Enemy::~Enemy(void){}

void Enemy::EnemyUpdate(float dt, int playerposition){

	int PlayerX;
	int PlayerY;
	div_t PY;
	PY = div(playerposition, width);
	PlayerY = PY.quot;
	PlayerX = playerposition - PlayerY*width;
	if(XPos == PlayerX && YPos == PlayerY){
		Health = 0;
	}
	if(Center == 1){
		Health = 0;
	}

	timer += 5*Speed * dt;
	MovementTimer += 5*Speed * dt;
	if(timer >= 1){
		if(state == 0){
			SwitchtoWander();
			SwitchtoChase(playerposition);
		}

		if(state == 1){
			SwitchtoIdle();
			SwitchtoChase(playerposition);
			Wander();
		}

		if(state == 2){
			SwitchChasetoIdle(playerposition);
			SwitchtoAttack(playerposition);
			SwitchtoFlee(playerposition);
			if(state == 2){
				Chase();
			}
		}

		if(state == 3){
			SwitchChasetoIdle(playerposition);
			SwitchAttacktoChase(playerposition);
			SwitchtoFlee(playerposition);
			if(state == 3){
				Attack(playerposition);
			}
		}
		if(state == 4){
			SwitchChasetoIdle(playerposition);
			SwitchFleetoAttack();
			if(state == 4){
				Flee(playerposition);
			}
		}

		timer = 0;
	}

	if(MovementTimer > 1){
		MovementTimer = 1;
	}
}

void Enemy::Wander(){
	int chance;
	chance = RanInt(0,100);
	if(chance > 20 && chance <= 40){ //up
		if(Up != 1){
			YPos --;
			enemyDirection = 1;
			MovementTimer = 0;
		}
	}
	if(chance > 40 && chance <= 60){ //down
		if(Down != 1){
			YPos++;
			enemyDirection = 2;
			MovementTimer = 0;
		}
	}
	if(chance > 60 && chance <= 80){ //Left
		if(Left != 1){
			XPos--;
			enemyDirection = 3;
			MovementTimer = 0;
		}
	}
	if(chance > 80 && chance <= 100){ //Left
		if(Right != 1){
			XPos++;
			enemyDirection = 4;
			MovementTimer = 0;
		}
	}
	
}

void Enemy::Chase(){
	if(NeedPath == false){

	int direction = 0;
	if(XPos > PathX[CurrentPathPoint] && YPos == PathY[CurrentPathPoint]){
		direction = 3; //left
		enemyDirection = direction;
	}
	if(XPos < PathX[CurrentPathPoint] && YPos == PathY[CurrentPathPoint]){
		direction = 4; //right
		enemyDirection = direction;
	}
	if(XPos == PathX[CurrentPathPoint] && YPos > PathY[CurrentPathPoint]){
		direction = 1; //up
		enemyDirection = direction;
	}
	if(XPos == PathX[CurrentPathPoint] && YPos < PathY[CurrentPathPoint]){
		direction = 2; //down
		enemyDirection = direction;
	}
	bool goToNext = false;
	if(direction == 1 && Up!= 1){
		goToNext = true;
	}
	if(direction == 2 && Down!= 1){
		goToNext = true;
	}
	if(direction == 3 && Left!= 1){
		goToNext = true;
	}
	if(direction == 4 && Right!= 1){
		goToNext = true;
	}

	if(goToNext == true){
		XPos = PathX[CurrentPathPoint];
		YPos = PathY[CurrentPathPoint];
		MovementTimer = 0;
			if(CurrentPathPoint < PathLength - 1){
				CurrentPathPoint++;
			
			}
		}
	}
}

void Enemy::Attack(int playerposition){
	int PlayerX;
	int PlayerY;
	div_t PY;
	PY = div(playerposition, width);
	PlayerY = PY.quot;
	PlayerX = playerposition - PlayerY*width;

	if(XPos == PlayerX && YPos == PlayerY + 1){//up
		enemyDirection = 1;
		PlayerHit = true;
	}
	if(XPos == PlayerX && YPos == PlayerY - 1){//down
		enemyDirection = 2;
		PlayerHit = true;
	}
	if(XPos == PlayerX + 1 && YPos == PlayerY){//left
		enemyDirection = 3;
		PlayerHit = true;
	}
	if(XPos == PlayerX - 1 && YPos == PlayerY){//right
		enemyDirection = 4;
		PlayerHit = true;
	}


}

void Enemy::SwitchtoIdle(){
		int chance;
		chance = RanInt(0,100);
		if(chance > 90){
			state = 0;
		}
}

void Enemy::SwitchtoWander(){
		int chance;
		chance = RanInt(0,100);
		if(chance > 90){
			state = 1;
		}
}

void Enemy::SwitchtoChase(int playerposition){
	int PlayerX;
	int PlayerY;
	div_t PY;
	PY = div(playerposition, width);
	PlayerY = PY.quot;
	PlayerX = playerposition - PlayerY*width;

	if(PlayerX - XPos < PlayerRanger && PlayerX - XPos > -PlayerRanger){
		if(PlayerY - YPos < PlayerRanger && PlayerY - YPos > -PlayerRanger){
			state = 2;
			NeedPath = true;
		}
	}

}

void Enemy::SwitchChasetoIdle(int playerposition){
	int PlayerX;
	int PlayerY;
	div_t PY;
	PY = div(playerposition, width);
	PlayerY = PY.quot;
	PlayerX = playerposition - PlayerY*width;

	if(PlayerX - XPos > PlayerRanger || PlayerX - XPos < -PlayerRanger){
		state = 0;
		NeedPath = false;
	}
	if(PlayerY - YPos > PlayerRanger || PlayerY - YPos < -PlayerRanger){
		state = 0;
		NeedPath = false;
	}
}

void Enemy::SwitchtoAttack(int playerposition){
	int PlayerX;
	int PlayerY;
	div_t PY;
	PY = div(playerposition, width);
	PlayerY = PY.quot;
	PlayerX = playerposition - PlayerY*width;


	if(PlayerX - XPos <2 && PlayerX - XPos > -2 && YPos == PlayerY){
			state = 3;
			NeedPath = false;
	}
	if(PlayerY - YPos < 2 && PlayerY - YPos > -2  && XPos == PlayerX){
		state = 3;
		NeedPath = false;
	}

}

void Enemy::SwitchAttacktoChase(int playerposition){

	bool InAttackRange = false;
	int PlayerX;
	int PlayerY;
	div_t PY;
	PY = div(playerposition, width);
	PlayerY = PY.quot;
	PlayerX = playerposition - PlayerY*width;

	if(XPos == PlayerX && YPos == PlayerY + 1){//up
		InAttackRange = true;
	}
	if(XPos == PlayerX && YPos == PlayerY - 1){//down
		InAttackRange = true;
	}
	if(XPos == PlayerX + 1 && YPos == PlayerY){//left
		InAttackRange = true;
	}
	if(XPos == PlayerX - 1 && YPos == PlayerY){//right
		InAttackRange = true;
	}

	if(InAttackRange == false){
		SwitchtoChase(playerposition);
	}

}

void Enemy::SwitchtoFlee(int playerposition){
	if(FleeHealth > 0){
		if(Health < MaxHealth/FleeHealth){
			if(Up != 1 || Down != 1 || Left != 1 || Right !=1){
				state = 4;
				NeedPath = false;
			}
		}
	}
}

void Enemy::SwitchFleetoAttack(){
	if(Up == 1 && Down == 1 && Left == 1 && Right == 1){
		state = 3;
	}
}

void Enemy::Flee(int playerposition){

	int RanDirection;
	RanDirection = RanInt(0,4);

	int PlayerX;
	int PlayerY;
	div_t PY;
	PY = div(playerposition, width);
	PlayerY = PY.quot;
	PlayerX = playerposition - PlayerY*width;

	int DistanceFromPlayer;
	int CurrentDistanceFromPlayer;
	int TempX;
	int TempY;
	bool Done = false;

	TempX = XPos - PlayerX;
	TempY = YPos - PlayerY;
	if(TempX < 0){
		TempX = TempX * -1;
	}
	if(TempY < 0){
		TempY = TempY * -1;
	}
	CurrentDistanceFromPlayer = TempY + TempX;

	if(RanDirection == 0){//check up first, then down, left, right
		if(Done == false){//up
			TempX = XPos - PlayerX;
			TempY = (YPos - 1) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer  && Up!= 1){
				YPos--;
				enemyDirection = 1;
				MovementTimer = 0;
				Done = true;
			}
		}

		if(Done == false){//down
			TempX = XPos - PlayerX;
			TempY = (YPos + 1) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Down!= 1){
				YPos++;
				enemyDirection = 2;
				MovementTimer = 0;
				Done = true;
			}
		}

		if(Done == false){//left
			TempX = (XPos - 1) - PlayerX;
			TempY = (YPos) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Left!= 1){
				enemyDirection = 3;
				MovementTimer = 0;
				XPos--;
				Done = true;
			}
		}

		if(Done == false){//right
			TempX = (XPos + 1) - PlayerX;
			TempY = (YPos) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Right!= 1){
				enemyDirection = 4;
				MovementTimer = 0;
				XPos++;
				Done = true;
			}
		}
	}


	if(RanDirection == 1){//check down first, then left, right, up
		if(Done == false){//down
			TempX = XPos - PlayerX;
			TempY = (YPos + 1) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Down!= 1){
				YPos++;
				enemyDirection = 2;
				MovementTimer = 0;
				Done = true;
			}
		}

		if(Done == false){//left
			TempX = (XPos - 1) - PlayerX;
			TempY = (YPos) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Left!= 1){
				enemyDirection = 3;
				MovementTimer = 0;
				XPos--;
				Done = true;
			}
		}

		if(Done == false){//right
			TempX = (XPos + 1) - PlayerX;
			TempY = (YPos) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Right!= 1){
				enemyDirection = 4;
				MovementTimer = 0;
				XPos++;
				Done = true;
			}
		}

		if(Done == false){//up
			TempX = XPos - PlayerX;
			TempY = (YPos - 1) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer  && Up!= 1){
				YPos--;
				enemyDirection = 1;
				MovementTimer = 0;
				Done = true;
			}
		}
	}


	if(RanDirection == 2){//check left first right, up, down
		if(Done == false){//left
			TempX = (XPos - 1) - PlayerX;
			TempY = (YPos) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Left!= 1){
				enemyDirection = 3;
				MovementTimer = 0;
				XPos--;
				Done = true;
			}
		}

		if(Done == false){//right
			TempX = (XPos + 1) - PlayerX;
			TempY = (YPos) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Right!= 1){
				enemyDirection = 4;
				MovementTimer = 0;
				XPos++;
				Done = true;
			}
		}

		if(Done == false){//up
			TempX = XPos - PlayerX;
			TempY = (YPos - 1) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer  && Up!= 1){
				YPos--;
				enemyDirection = 1;
				MovementTimer = 0;
				Done = true;
			}
		}

		if(Done == false){//down
			TempX = XPos - PlayerX;
			TempY = (YPos + 1) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Down!= 1){
				YPos++;
				enemyDirection = 2;
				MovementTimer = 0;
				Done = true;
			}
		}
	}


	if(RanDirection == 3){//check right first up, down, left
		if(Done == false){//right
			TempX = (XPos + 1) - PlayerX;
			TempY = (YPos) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Right!= 1){
				enemyDirection = 4;
				MovementTimer = 0;
				XPos++;
				Done = true;
			}
		}

		if(Done == false){//up
			TempX = XPos - PlayerX;
			TempY = (YPos - 1) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer  && Up!= 1){
				YPos--;
				enemyDirection = 1;
				MovementTimer = 0;
				Done = true;
			}
		}

		if(Done == false){//down
			TempX = XPos - PlayerX;
			TempY = (YPos + 1) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Down!= 1){
				YPos++;
				enemyDirection = 2;
				MovementTimer = 0;
				Done = true;
			}
		}

		if(Done == false){//left
			TempX = (XPos - 1) - PlayerX;
			TempY = (YPos) - PlayerY ;
			if(TempX < 0){
				TempX = TempX * -1;
			}
			if(TempY < 0){
				TempY = TempY * -1;
			}
			DistanceFromPlayer = TempY + TempX;
			if(DistanceFromPlayer > CurrentDistanceFromPlayer && Left!= 1){
				enemyDirection = 3;
				MovementTimer = 0;
				XPos--;
				Done = true;
			}
		}
	}

}

void Enemy::BuildBuffer(ID3D11Device* device,float TileX, float TileY){
	bufferDirection = enemyDirection;

	BuildHealthBuffer(device, TileX,  TileY);

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
	int YPos ;
	if(enemyDirection == 1){//up
		XPos = 1 + Type;
		YPos = 17;
	}
	if(enemyDirection == 2){//down
		XPos = 1 + Type;
		YPos = 18;
	}
	if(enemyDirection == 3){//left
		XPos = 1 + Type;
		YPos = 19;
	}
	if(enemyDirection == 4){//right
		XPos = 1 + Type;
		YPos = 20;
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

	

}

void Enemy::BuildHealthBuffer(ID3D11Device* device,float TileX, float TileY){

	Bufferhealth = Health;

	float BarPercentage = (Health/MaxHealth);

	float TileWidth = 2/TileX;
	float TileHeight = 2/TileY;

	float newWidth = TileWidth * BarPercentage;
	float newHeight = TileHeight/10;

	float offsetWidth = (TileWidth - newWidth)/2;

	VertexStruct* vertices = new VertexStruct[6];

	vertices[0].position = XMFLOAT3( -1 + newWidth  + offsetWidth, 1 , 1.0f );
	vertices[1].position = XMFLOAT3( -1 + offsetWidth, 1 - newHeight, 1.0f );
	vertices[2].position = XMFLOAT3( -1 + offsetWidth, 1, 1.0f );
			
	vertices[3].position = XMFLOAT3( -1 + newWidth  + offsetWidth, 1, 1.0f );
	vertices[4].position = XMFLOAT3( -1 + newWidth  + offsetWidth, 1 - newHeight, 1.0f );
	vertices[5].position = XMFLOAT3( -1 + offsetWidth, 1 - newHeight, 1.0f );

	float XDiff = 1.0f/20.0f;
	float YDiff = 0.05f;

	int XPos = 15;
	int YPos = 1 ;

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
	HRESULT d3dResult = device->CreateBuffer( &vertex_desc, &resource_data, &g_vertexHealthBuffer );


	// delete the Vertex array
	delete[] vertices;
}

void Enemy::Render(ID3D11DeviceContext* deviceContext, float TileWidth, float TileHeight, float GetX, float GetY, bool fullsize){
	if(Health > 0){
	// Puts the vertex and index buffers on the graphics pipeline
	unsigned int stride;
	unsigned int offset;


	
	if(enemyDirection == 1){
		SmoothXOffset = 0;
		SmoothYOffset = -(TileHeight *  (1 - MovementTimer));
	}
	if(enemyDirection == 2){
		SmoothXOffset = 0;
		SmoothYOffset = (TileHeight *  (1 - MovementTimer));
	}
	if(enemyDirection == 3){
		SmoothXOffset = (TileWidth*  (1 - MovementTimer));
		SmoothYOffset = 0;
	}
	if(enemyDirection == 4){
		SmoothXOffset = -(TileWidth*  (1 - MovementTimer));
		SmoothYOffset = 0;
	}

	if(SmoothXOffset < -TileWidth){
		SmoothXOffset = -TileWidth;
	}
	if(SmoothXOffset > TileWidth){
		SmoothXOffset = TileWidth;
	}
	if(SmoothYOffset > TileHeight){
		SmoothYOffset = TileHeight;
	}
	if(SmoothYOffset < -TileHeight){
		SmoothYOffset = -TileHeight;
	}



	// Set vertex buffer stride and offset.
	stride = sizeof(VertexStruct); 
	offset = 0;

	XMMATRIX position_mat = XMMatrixTranslation(0, 0, 0);
	XMMATRIX scale_mat = XMMatrixScaling(1,1,1);
	XMMATRIX rotation_mat = XMMatrixRotationRollPitchYaw(0,0,0);
	if(fullsize == true){
		position_mat = XMMatrixTranslation((TileWidth * XPos) + SmoothXOffset, -(TileHeight * YPos)  + SmoothYOffset, 0);
	}
	else{
		position_mat = XMMatrixTranslation((TileWidth * XPos) + GetX + SmoothXOffset, -(TileHeight * YPos) + GetY + SmoothYOffset, 0);
	}
	XMMATRIX world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );
	deviceContext->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &worldCB_ );

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->Draw(6, 0 );

	if(Health < MaxHealth){
	position_mat = XMMatrixTranslation(0, 0, 0);
	scale_mat = XMMatrixScaling(1,1,1);
	rotation_mat = XMMatrixRotationRollPitchYaw(0,0,0);
	if(fullsize == true){
		position_mat = XMMatrixTranslation((TileWidth * XPos) + SmoothXOffset, -(TileHeight * YPos)  + SmoothYOffset, 0);
	}
	else{
		position_mat = XMMatrixTranslation((TileWidth * XPos) + GetX + SmoothXOffset, -(TileHeight * YPos) + GetY + SmoothYOffset, 0);
	}
	world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );
	deviceContext->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &worldCB_ );

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &g_vertexHealthBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->Draw(6, 0 );
	}
	}

}

int Enemy::RanInt(int min, int max){

	return rand() %(max-min) + min;

}

int Enemy::GetX(){
	return XPos;
}

int Enemy::GetY(){
	return YPos;
}

bool Enemy::RequiresPath(){
	return NeedPath;
}

void Enemy::UpdateSurroudings(int u, int d, int l, int r, int ul, int ur, int dl, int dr, int c){

	Up = u;
	Down = d;
	Left = l;
	Right = r;
	UpLeft = ul;
	DownLeft = dl;
	UpRight = ur;
	DownRight = dr;
	Center = c;

}

void Enemy::UpdatePath(){
	NeedPath = false;
}

void Enemy::PlayerMoved(){
	if(state == 2){
		NeedPath = true;
	}
}

void Enemy::SetPathLength(int length){
	PathLength = length;
	PathX = new int[PathLength];
	PathY = new int[PathLength];
}

void Enemy::AddPathPoint(int Position, int X, int Y){
	PathX[Position] = X;
	PathY[Position] = Y;
	PathLength = PathLength;
	CurrentPathPoint = 0;
}

float Enemy::GetHealth(){
	return Health;
}

void Enemy::SetHealth(int NewHealth){
	Health = NewHealth;
}

void Enemy::unloadAll(){
	g_vertexBuffer->Release();
	g_vertexBuffer = NULL;
	g_vertexHealthBuffer->Release();
	g_vertexHealthBuffer = NULL;
	worldCB_->Release();
	worldCB_ = NULL;
	PathY = NULL;
	PathX = NULL;
}

void Enemy::RebuildBuffer(ID3D11Device* device,float TileX, float TileY){
	if(bufferDirection != enemyDirection){
		BuildBuffer(device, TileX,  TileY);
	}
	if(Bufferhealth != Health){
		BuildHealthBuffer(device, TileX,  TileY);
	}
}

bool Enemy::getPlayerHit(){
	return PlayerHit;
}

int Enemy::GetDefense(){
	return enemyDefense;
}
		
int Enemy::GetStrength(){
	return enemyStrength;
}

void Enemy::PlayerHitOff(){
	PlayerHit = false;
}

void Enemy::SetConstantBuffer(ID3D11Device* device){
	//constant world matrix buffer

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT d3dResult = device->CreateBuffer( &constDesc, 0, &worldCB_ );
}