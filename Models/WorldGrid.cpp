#include "WorldGrid.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Enemy.h"

const int MAX_ENEMY = 100;

Enemy* EnemyArray[MAX_ENEMY] = {NULL};

const int MAX_CHEST = 8;

Chest* ChestArray[MAX_ENEMY] = {NULL};


WorldGrid::WorldGrid(int wid, int heig)
{
	width = wid;
	height = heig;

	basetype = new int[wid*heig];
	collidable = new bool[wid*heig];
	entitycollidable = new bool[wid*heig];
	PathChecked = new bool[wid*heig];

	MyPaths = new PathStruct[500];
	MyRooms = new PathStruct[500];
	SpreadingNodes = new Spreadstruct[1000];
	TempPositionArray = new NodeStruct[50];
	PathCalculationArray = new GridPathDataStruct[width*height];

	gridPosX = 0;
	gridPosY = 0;
	ExitPosX = 0;
	ExitPosY = 0;
	MovementTimer = 1;
	AttackTimer = 4;
	fullsize = false;
	NeedReload = false;
	floorCleared = false;

	floorNumber = 1;

	playerStrength = 2;
	playerHealth = 500;
	playerMaxHealth = playerHealth;
	bufferHealth = playerHealth;
	playerDefense = 1;
	Potions = 1;
	TestingWorldType = false;
	srand (time(NULL));
	WorldType = 3;
	while(WorldType == 3){
		WorldType = RanInt(0,5) + 1;
	}
	//WorldType = 5;

	MovementDirection = 0;
	SmoothXOffset = 0.0f;
	SmoothYOffset = 0.0f;
}

WorldGrid::~WorldGrid(void)
{
}

void WorldGrid::InitSnow()
{
	WorldType = 1;

	int randomx;
	int randomy;
	int centerpos;
	int mydirection;
	playerDirection = 1;
	//MyPaths = new PathStruct[500];
	//MyRooms = new PathStruct[500];
	//SpreadingNodes = new Spreadstruct[1000];
	//TempPositionArray = new NodeStruct[50];
	srand (time(NULL));
	floorCleared = false;

	//PathCalculationArray = new GridPathDataStruct[width*height];

	while(Paths<10){
	Paths = 0;
	Rooms = 0;
	SpreadingNum = 0;
	CurrentEnemyNumber = 0;

	
	for(int x = 0; x < 500; x++){
		MyRooms[x].Real = false;
		MyRooms[x].Position = 0;
		MyRooms[x].Direction = 0;
	}

	for(int x = 0; x < 500; x++){
		MyPaths[x].Real = false;
		MyPaths[x].Position = 0;
		MyPaths[x].Direction = 0;
	}

	for(int x = 0; x < 1000; x++){
		SpreadingNodes[x].Real = false;
		SpreadingNodes[x].Position = 0;
		SpreadingNodes[x].newType = 0;
		SpreadingNodes[x].oldType = 0;
		SpreadingNodes[x].checkType = 0;
	}

	for(int x = 0; x < width*height; x++){
		basetype[x] = 0;
		collidable[x] = 0;   
		entitycollidable[x] = 0;
		PathChecked[x] = 0;

		PathCalculationArray[x].Real = false;
	}
	randomx=RanInt(0, width);
	randomy=RanInt(0, height);
	mydirection = RanInt(0,4) + 1;
	centerpos = width * randomy + randomx;
	basetype[centerpos] = 2;
	NewPath(centerpos, RanInt(0,MaxLength(centerpos, mydirection)), mydirection);

	for (int PN = 0; PN < 500; PN++){
		if(MyPaths[PN].Real == true){
			NewPath(MyPaths[PN].Position, RanInt(0,MaxLength(MyPaths[PN].Position, MyPaths[PN].Direction)), MyPaths[PN].Direction);
			MyPaths[PN].Real = false;
		}
	}


	for (int RN = 0; RN < 500; RN++){
		if(MyRooms[RN].Real == true){
			NewRoom(MyRooms[RN].Position);
			MyRooms[RN].Real = false;
		}
	}



	}
	RimDetection(0, 1, 2);

	RimDetection(0, 4, 1);

	RimDetection(0, -1, 4);
	ReplaceAll(-1,4);

	RimDetection(2, 3, 1);

	AddSpreadingNodes(2, 5, 2, 95);

	SpreadNodes();

	Paths = 0;
	SpreadingNum = 0;

	//player spawn
	while(basetype[gridPosY*width + gridPosX] != 2){
		gridPosY = RanInt(0, height);
		gridPosX = RanInt(0, width);
	}

	//exit spawn
	while(basetype[ExitPosY*width + ExitPosX] != 3){
		ExitPosY = RanInt(0, height);
		ExitPosX = RanInt(0, width);
	}
	//collidable[ExitPosY*width + ExitPosX] = -1;

	PopulateEnemies(2);
	PopulateChests(3);
	
}

void WorldGrid::InitLava()
{
	WorldType = 2;

	int randomx;
	int randomy;
	int centerpos;
	int mydirection;
	playerDirection = 1;
	//MyPaths = new PathStruct[500];
	//MyRooms = new PathStruct[500];
	//SpreadingNodes = new Spreadstruct[1000];
	//TempPositionArray = new NodeStruct[50];
	srand (time(NULL));
	floorCleared = false;

	//PathCalculationArray = new GridPathDataStruct[width*height];

	while(Paths<10){
	Paths = 0;
	Rooms = 0;
	SpreadingNum = 0;
	CurrentEnemyNumber = 0;

	
	for(int x = 0; x < 500; x++){
		MyRooms[x].Real = false;
		MyRooms[x].Position = 0;
		MyRooms[x].Direction = 0;
	}

	for(int x = 0; x < 500; x++){
		MyPaths[x].Real = false;
		MyPaths[x].Position = 0;
		MyPaths[x].Direction = 0;
	}

	for(int x = 0; x < 1000; x++){
		SpreadingNodes[x].Real = false;
		SpreadingNodes[x].Position = 0;
		SpreadingNodes[x].newType = 0;
		SpreadingNodes[x].oldType = 0;
		SpreadingNodes[x].checkType = 0;
	}

	for(int x = 0; x < width*height; x++){
		basetype[x] = 0;
		collidable[x] = 0;   
		entitycollidable[x] = 0;
		PathChecked[x] = 0;

		PathCalculationArray[x].Real = false;
	}

	randomx=RanInt(0, width);
	randomy=RanInt(0, height);
	mydirection = RanInt(0,4) + 1;
	centerpos = width * randomy + randomx;
	basetype[centerpos] = 2;
	NewPath(centerpos, RanInt(0,MaxLength(centerpos, mydirection)), mydirection);

	for (int PN = 0; PN < 500; PN++){
		if(MyPaths[PN].Real == true){
			NewPath(MyPaths[PN].Position, RanInt(0,MaxLength(MyPaths[PN].Position, MyPaths[PN].Direction)), MyPaths[PN].Direction);
			MyPaths[PN].Real = false;
		}
	}


	for (int RN = 0; RN < 500; RN++){
		if(MyRooms[RN].Real == true){
			NewRoom(MyRooms[RN].Position);
			MyRooms[RN].Real = false;
		}
	}

	}

	RimDetection(0, 1, 2);
	RimDetection(2, 3, 1);
	ReplaceAll(0, 1);
	AddDots(2, 4, 98);

	//player spawn
	while(basetype[gridPosY*width + gridPosX] != 2){
		gridPosY = RanInt(0, height);
		gridPosX = RanInt(0, width);
	}

	//exit spawn
	while(basetype[ExitPosY*width + ExitPosX] != 3){
		ExitPosY = RanInt(0, height);
		ExitPosX = RanInt(0, width);
	}
	//collidable[ExitPosY*width + ExitPosX] = -1;

	PopulateEnemies(2);
	PopulateChests(3);
	RimDetection(0, 4, 1);
	
	Paths = 0;
	SpreadingNum = 0;
	
}

void WorldGrid::InitTemple()
{
	WorldType = 4;

	int randomx;
	int randomy;
	int centerpos;
	int mydirection;
	playerDirection = 1;
	//MyPaths = new PathStruct[500];
	//MyRooms = new PathStruct[500];
	//SpreadingNodes = new Spreadstruct[1000];
	//TempPositionArray = new NodeStruct[50];
	srand (time(NULL));
	floorCleared = false;
	Paths = 0;
	Rooms = 0;
	SpreadingNum = 0;
	CurrentEnemyNumber = 0;

	//PathCalculationArray = new GridPathDataStruct[width*height];

	for(int x = 0; x < width*height; x++){
		basetype[x] = 0;
		collidable[x] = 0;   
		entitycollidable[x] = 0;
		PathChecked[x] = 0;

		PathCalculationArray[x].Real = false;
	}

	

	LineEdge(4);
	RimDetection(0,7,4);
	ReplaceAll(7, 4);

	StartCellSimulation(2,5,0);
	SimulationStep(2, 5);
	SimulationStep(2, 5);
	SimulationStep(2, 5);
	SimulationStep(2, 5);
	FloodSimulation(2, 5);

	ReplaceAll(5,0);
	ReplaceAll(4,0);

	RimDetection(0,1,2);
	RimDetection(2,3,1);

	//player spawn
	while(basetype[gridPosY*width + gridPosX] != 2){
		gridPosY = RanInt(0, height);
		gridPosX = RanInt(0, width);
	}

	//exit spawn
	while(basetype[ExitPosY*width + ExitPosX] != 3){
		ExitPosY = RanInt(0, height);;
		ExitPosX = RanInt(0, width);;
	}
	//collidable[ExitPosY*width + ExitPosX] = -1;

	PopulateEnemies(2);
	PopulateChests(3);
	
	Paths = 0;
	SpreadingNum = 0;
	
}

void WorldGrid::InitSnowMaze(){

	WorldType = 3;

	int randomx;
	int randomy;
	int centerpos;
	int mydirection;
	playerDirection = 1;
	//MyPaths = new PathStruct[500];
	//MyRooms = new PathStruct[500];
	//SpreadingNodes = new Spreadstruct[1000];
	//TempPositionArray = new NodeStruct[50];
	srand (time(NULL));
	floorCleared = false;

	//PathCalculationArray = new GridPathDataStruct[width*height];

	Paths = 0;
	Rooms = 0;
	SpreadingNum = 0;
	CurrentEnemyNumber = 0;


	for(int x = 0; x < width*height; x++){
		basetype[x] = 0;
		collidable[x] = 0;   
		entitycollidable[x] = 0;
		PathChecked[x] = 0;

		PathCalculationArray[x].Real = false;
	}



	LineEdge(2);
	for(int i = 0; i < width/5; i++){
		RimDetection(0,7,2);
		ReplaceAll(7, 2);
	}
	GenerateRandomPath(0,3);

	ReplaceAll(2, 0);

	//player spawn
	while(basetype[gridPosY*width + gridPosX] != 3){
		gridPosY = RanInt(0, height);
		gridPosX = RanInt(0, width);
	}

	//exit spawn
	while(basetype[ExitPosY*width + ExitPosX] != 3){
		ExitPosY = RanInt(0, height);
		ExitPosX = RanInt(0, width);
	}
	//collidable[ExitPosY*width + ExitPosX] = -1;

	PopulateEnemies(3);
	PopulateChests(3);

	RimDetection(0, 1, 3);
	RimDetection(0, 4, 1);
	RimDetection(0, -1, 4);
	ReplaceAll(-1,4);
	
	Paths = 0;
	SpreadingNum = 0;


}

void WorldGrid::InitTemplelabyrinth(){

	WorldType = 5;

	int randomx;
	int randomy;
	int centerpos;
	int mydirection;
	playerDirection = 1;
	//MyPaths = new PathStruct[500];
	//MyRooms = new PathStruct[500];
	//SpreadingNodes = new Spreadstruct[1000];
	//TempPositionArray = new NodeStruct[50];
	srand (time(NULL));
	floorCleared = false;

	//PathCalculationArray = new GridPathDataStruct[width*height];

	Paths = 0;
	Rooms = 0;
	SpreadingNum = 0;
	CurrentEnemyNumber = 0;


	for(int x = 0; x < width*height; x++){
		basetype[x] = 0;
		collidable[x] = 0;   
		entitycollidable[x] = 0;
		PathChecked[x] = 0;

		PathCalculationArray[x].Real = false;
	}



	LineEdge(2);
	for(int i = 0; i < width/8; i++){
		RimDetection(0, 3, 2);
		ReplaceAll(3, 2);
	}

	GenerateRooms(0, 1, 200, 1000, 20);
	GenerateRandomPath(0,3);
	ReplaceAll(1, 3);
	FloodSimulation(3, 0);
	RemoveDeadEnds(3, 0);
	ReplaceAll(2, 0);
	ReplaceAll(3, 2);
	RimDetection(0, 1, 2);
	RimDetection(2, 3, 1);
	ReplaceAll(0, 7);




	//player spawn
	while(basetype[gridPosY*width + gridPosX] != 2){
		gridPosY = RanInt(0, height);
		gridPosX = RanInt(0, width);
	}

	//exit spawn
	while(basetype[ExitPosY*width + ExitPosX] != 3){
		ExitPosY = RanInt(0, height);
		ExitPosX = RanInt(0, width);
	}
	//collidable[ExitPosY*width + ExitPosX] = -1;

	PopulateEnemies(2);
	PopulateChests(3);
	
	Paths = 0;
	SpreadingNum = 0;


}

void WorldGrid::BuildBuffer(ID3D11Device* device,float TileX, float TileY)
{
	NeedReload = false;
	BuildPlayerBuffer(device,TileX,TileY);
	BuildExitBuffer(device,TileX,TileY);
	BuildHealthBuffer(device);
	BuildUIBuffer(device);


	for(int e = 0; e < CurrentEnemyNumber; e++){
		EnemyArray[e]->BuildBuffer(device, TileX, TileY);
	}

	for(int i = 0; i < MAX_CHEST; i++){
		ChestArray[i]->BuildBuffer(device, TileX, TileY);
	}

	MyTileX = TileX;
	MyTileY = TileY;

	VertexStruct* vertices = new VertexStruct[width*height*6];
	float TileWidth = 2/TileX;
	float TileHeight = 2/TileY;

	MyTileWidth = TileWidth;
	MyTileHeight = TileHeight;


	for( int h = 0; h < height; h++ )
	{
		for( int w = 0; w <width; w++ )
		{
			int position = ((h*width)+w)*6;
			
			vertices[position].position = XMFLOAT3( -1 + TileWidth*(w+1), 1 - TileHeight*h, 1.0f );
			vertices[position+1].position = XMFLOAT3( -1 + TileWidth*w, 1 - TileHeight*(h+1), 1.0f );
			vertices[position+2].position = XMFLOAT3( -1 + TileWidth*w, 1 - TileHeight*h, 1.0f );
			
			vertices[position+3].position = XMFLOAT3( -1 + TileWidth*(w+1), 1 - TileHeight*h, 1.0f );
			vertices[position+4].position = XMFLOAT3( -1 + TileWidth*(w+1), 1 - TileHeight*(h+1), 1.0f );
			vertices[position+5].position = XMFLOAT3( -1 + TileWidth*w, 1 - TileHeight*(h+1), 1.0f );
			
			float XDiff = 1.0f/20.0f;
			float YDiff = 0.05f;

			float XPos = 0;
			float YPos = 0;

			int CurrentPosition = (h*width)+w;


			//calculate surroundings
			int Up = 0;
			int Down = 0;
			int Left = 0;
			int Right = 0;
			int UpLeft = 0;
			int DownLeft = 0;
			int UpRight = 0;
			int DownRight = 0;
			if(CurrentPosition > width){
				Up = basetype[CurrentPosition - width];

				div_t TestUp;
				div_t TestUpLeft;
				div_t TestUpRight;
				TestUp = div (CurrentPosition - width, width);
				TestUpLeft = div (CurrentPosition - width - 1, width);
				TestUpRight = div (CurrentPosition - width + 1, width);
				if(TestUp.quot == TestUpLeft.quot){
					UpLeft = basetype[CurrentPosition - width - 1];
				}
				if(TestUp.quot == TestUpRight.quot){
					UpRight = basetype[CurrentPosition - width + 1];
				}
			}
			if(CurrentPosition < (height-1)*width){
				Down = basetype[CurrentPosition + width];

				div_t TestDown;
				div_t TestDownLeft;
				div_t TestDownRight;
				TestDown = div (CurrentPosition + width, width);
				TestDownLeft = div (CurrentPosition + width - 1, width);
				TestDownRight = div (CurrentPosition + width + 1, width);
				if(TestDown.quot == TestDownLeft.quot){
					DownLeft = basetype[CurrentPosition + width - 1];
				}
				if(TestDown.quot == TestDownRight.quot){
					DownRight = basetype[CurrentPosition + width + 1];
				}
			}

			div_t Test;
			div_t TestLeft;
			div_t TestRight;
			Test = div (CurrentPosition, width);
			TestLeft = div (CurrentPosition - 1, width);
			TestRight = div (CurrentPosition + 1, width);
			if(Test.quot == TestLeft.quot){
				Left = basetype[CurrentPosition - 1];
			}
			if(Test.quot == TestRight.quot){
				Right = basetype[CurrentPosition + 1];
			}


			if(WorldType == 1 || WorldType == 3){
			if(basetype[CurrentPosition] == 0){
				XPos = 10;
				YPos = 2;
			}
			if(basetype[CurrentPosition] == 1){  //WALLS
				collidable[CurrentPosition] = 1;
				XPos=5;
				YPos=8;

				//verticalwall
				if(Up == 1 && Down == 1 && Left != 1 && Right != 1){
					XPos=7;
					YPos=2;
				}
				//horizontalwall
				if(Up != 1 && Down != 1 && Left == 1 && Right == 1){
					XPos=5;
					YPos=6;
				}

				//3sides no top
				if(Up != 1 && Down == 1 && Left == 1 && Right == 1){
					XPos=8;
					YPos=4;
				}
				//3sides no bottom
				if(Up == 1 && Down != 1 && Left == 1 && Right == 1){
					XPos=9;
					YPos=5;
				}
				//3sides no left
				if(Up == 1 && Down == 1 && Left != 1 && Right == 1){
					XPos=8;
					YPos=5;
				}
				//3sides no right
				if(Up == 1 && Down == 1 && Left == 1 && Right != 1){
					XPos=9;
					YPos=4;
				}

				//corner no tl
				if(Up != 1 && Down == 1 && Left != 1 && Right == 1){
					XPos=4;
					YPos=7;
				}
				//corner no tr
				if(Up != 1 && Down == 1 && Left == 1 && Right != 1){
					XPos=6;
					YPos=7;
				}
				//corner no bl
				if(Up == 1 && Down != 1 && Left != 1 && Right == 1){
					XPos=4;
					YPos=9;
				}
				//corner no br
				if(Up == 1 && Down != 1 && Left == 1 && Right != 1){
					XPos=6;
					YPos=9;
				}

				//pointup
				if(Up != 1 && Down == 1 && Left != 1 && Right != 1){
					XPos=7;
					YPos=1;
				}
				//pointdown
				if(Up == 1 && Down != 1 && Left != 1 && Right != 1){
					XPos=7;
					YPos=3;
				}
				//pointleft
				if(Up != 1 && Down != 1 && Left != 1 && Right == 1){
					XPos=4;
					YPos=6;
				}
				//pointright
				if(Up != 1 && Down != 1 && Left == 1 && Right != 1){
					XPos=6;
					YPos=6;
				}

				//none
				if(Up != 1 && Down != 1 && Left != 1 && Right != 1){
					XPos=5;
					YPos=3;
				}
				//4sides
				if(Up == 1 && Down == 1 && Left == 1 && Right == 1){
					XPos=11;
					YPos=5;
				}

				

			}
			if(basetype[CurrentPosition] == 2){ //floor
				XPos=2;
				YPos=8;
			}
			if(basetype[CurrentPosition] == 3){
				XPos=1;
				YPos=8;
			}
			if(basetype[CurrentPosition] == 4){
				XPos=14;
				YPos=7;

				if(Up == 0 && Down != 0 && Left != 0 && Right != 0){
					XPos=14;
					YPos=6;
				}
				if(Up != 0 && Down == 0 && Left != 0 && Right != 0){
					XPos=14;
					YPos=8;
				}
				if(Up != 0 && Down != 0 && Left == 0 && Right != 0){
					XPos=13;
					YPos=7;
				}
				if(Up != 0 && Down != 0 && Left != 0 && Right == 0){
					XPos=15;
					YPos=7;
				}

				if(Up == 0 && Down != 0 && Left == 0 && Right != 0){
					XPos=13;
					YPos=6;
				}
				if(Up == 0 && Down != 0 && Left != 0 && Right == 0){
					XPos=15;
					YPos=6;
				}
				if(Up != 0 && Down == 0 && Left != 0 && Right == 0){
					XPos=15;
					YPos=8;
				}
				if(Up != 0 && Down == 0 && Left == 0 && Right != 0){
					XPos=13;
					YPos=8;
				}

				if(Up != 0 && Down != 0 && Left != 0 && Right != 0 && DownLeft == 0){
					XPos=5;
					YPos=4;
				}
				if(Up != 0 && Down != 0 && Left != 0 && Right != 0 && DownRight == 0){
					XPos=4;
					YPos=4;
				}
				if(Up != 0 && Down != 0 && Left != 0 && Right != 0 && UpRight == 0){
					XPos=4;
					YPos=5;
				}
				if(Up != 0 && Down != 0 && Left != 0 && Right != 0 && UpLeft == 0){
					XPos=5;
					YPos=5;
				}


				if(Up != 0 && Down == 0 && Left == 0 && Right == 0){
					XPos=10;
					YPos=2;
				}
				if(Up == 0 && Down != 0 && Left == 0 && Right == 0){
					XPos=10;
					YPos=2;
				}
				if(Up == 0 && Down == 0 && Left != 0 && Right == 0){
					XPos=10;
					YPos=2;
				}
				if(Up == 0 && Down == 0 && Left == 0 && Right != 0){
					XPos=10;
					YPos=2;
				}
				if(Up == 0 && Down == 0 && Left == 0 && Right == 0){
					XPos=10;
					YPos=2;
				}

				if(Up == 0 && Down == 0 && Left != 0 && Right != 0){
					XPos=10;
					YPos=2;
				}
				if(Up != 0 && Down != 0 && Left == 0 && Right == 0){
					XPos=10;
					YPos=2;
				}

			}

			if(basetype[CurrentPosition] == 5){
				XPos=10;
				YPos=12;

				if(Up != 5 && Down != 5 && Left == 5 && Right == 5){
					XPos=9;
					YPos=9;
				}
				if(Up == 5 && Down == 5 && Left != 5 && Right != 5){
					XPos=9;
					YPos=8;
				}

				if(Up == 5 && Down != 5 && Left == 5 && Right != 5){
					XPos=2;
					YPos=10;
				}
				if(Up == 5 && Down != 5 && Left != 5 && Right == 5){
					XPos=1;
					YPos=10;
				}
				if(Up != 5 && Down == 5 && Left == 5 && Right != 5){
					XPos=2;
					YPos=9;
				}
				if(Up != 5 && Down == 5 && Left != 5 && Right == 5){
					XPos=1;
					YPos=9;
				}

				if(Up != 5 && Down == 5 && Left == 5 && Right == 5){
					XPos=9;
					YPos=16;
				}
				if(Up == 5 && Down != 5 && Left == 5 && Right == 5){
					XPos=9;
					YPos=18;
				}
				if(Up == 5 && Down == 5 && Left != 5 && Right == 5){
					XPos=8;
					YPos=17;
				}
				if(Up == 5 && Down == 5 && Left == 5 && Right != 5){
					XPos=10;
					YPos=17;
				}

				if(Up != 5 && Down != 5 && Left != 5 && Right != 5){
					XPos=3;
					YPos=8;
				}

			}
			}


			if(WorldType == 2){
				if(basetype[CurrentPosition] == 1){
					XPos=5;
					YPos=6;
					collidable[CurrentPosition] = 1;


					//none
					if(Up != 1 && Down != 1 && Left != 1 && Right != 1){
						XPos=9;
						YPos=6;
					}

					//up
					if(Up == 1 && Down != 1 && Left != 1 && Right != 1){
						XPos=10;
						YPos=9;
					}
					//down
					if(Up != 1 && Down == 1 && Left != 1 && Right != 1){
						XPos=10;
						YPos=8;
					}
					//left
					if(Up != 1 && Down != 1 && Left == 1 && Right != 1){
						XPos=9;
						YPos=8;
					}
					//right
					if(Up != 1 && Down != 1 && Left != 1 && Right == 1){
						XPos=8;
						YPos=8;
					}

					//horizontal
					if(Up != 1 && Down != 1 && Left == 1 && Right == 1){
						XPos=9;
						YPos=5;
					}
					//Vertical
					if(Up == 1 && Down == 1 && Left != 1 && Right != 1){
						XPos=8;
						YPos=6;
					}

					//
					if(Up == 1 && Down != 1 && Left == 1 && Right != 1){
						XPos=6;
						YPos=7;
					}
					if(Up == 1 && Down != 1 && Left != 1 && Right == 1){
						XPos=4;
						YPos=7;
					}
					if(Up != 1 && Down == 1 && Left == 1 && Right != 1){
						XPos=6;
						YPos=5;
					}
					if(Up != 1 && Down == 1 && Left != 1 && Right == 1){
						XPos=4;
						YPos=5;
					}

					//
					if(Up != 1 && Down == 1 && Left == 1 && Right == 1){
						XPos=5;
						YPos=5;
					}
					if(Up == 1 && Down != 1 && Left == 1 && Right == 1){
						XPos=5;
						YPos=7;
					}
					if(Up == 1 && Down == 1 && Left != 1 && Right == 1){
						XPos=4;
						YPos=6;
					}
					if(Up == 1 && Down == 1 && Left == 1 && Right != 1){
						XPos=6;
						YPos=6;
					}


					//
					if(Up == 1 && Down == 1 && Left == 1 && Right == 1 && UpRight != 1){
						XPos=11;
						YPos=6;
					}
					if(Up == 1 && Down == 1 && Left == 1 && Right == 1 && UpLeft != 1){
						XPos=12;
						YPos=6;
					}
					if(Up == 1 && Down == 1 && Left == 1 && Right == 1 && DownRight != 1){
						XPos=11;
						YPos=5;
					}
					if(Up == 1 && Down == 1 && Left == 1 && Right == 1 && DownLeft != 1){
						XPos=12;
						YPos=5;
					} 

					//
					if(Up != 1 && Down == 1 && Left == 1 && Right == 1 && DownLeft != 1){
						XPos=12;
						YPos=7;
					} 
					if(Up != 1 && Down == 1 && Left == 1 && Right == 1 && DownRight != 1){
						XPos=11;
						YPos=7;
					} 
					if(Up == 1 && Down != 1 && Left == 1 && Right == 1 && UpLeft != 1){
						XPos=12;
						YPos=8;
					} 
					if(Up == 1 && Down != 1 && Left == 1 && Right == 1 && UpRight != 1){
						XPos=11;
						YPos=8;
					} 



					//
					if(Up == 1 && Down == 1 && Left == 1 && Right != 1 && DownLeft != 1){
						XPos=15;
						YPos=5;
					} 
					if(Up == 1 && Down == 1 && Left == 1 && Right != 1 && UpLeft != 1){
						XPos=15;
						YPos=7;
					} 
					if(Up == 1 && Down == 1 && Left != 1 && Right == 1 && DownRight != 1){
						XPos=13;
						YPos=5;
					} 
					if(Up == 1 && Down == 1 && Left != 1 && Right == 1 && UpRight != 1){
						XPos=13;
						YPos=7;
					} 


				}

				if(basetype[CurrentPosition] == 2){
					XPos=7;
					YPos=6;
				}

				if(basetype[CurrentPosition] == 3){
					XPos=7;
					YPos=7;
				}
				if(basetype[CurrentPosition] == 4){
					XPos=7;
					YPos=5;
				}
			}

			if(WorldType == 4  || WorldType == 5){
				if(basetype[CurrentPosition] == 0){
					XPos = 11;
					YPos = 11;
				}
				if(basetype[CurrentPosition] == 1){  //WALLS
					collidable[CurrentPosition] = 1;
					XPos = 11;
					YPos = 16;
					//none
					if(Up != 1 && Down != 1 && Left != 1 && Right != 1){
						XPos=13;
						YPos=18;
					} 

					if(Up == 1 && Down != 1 && Left != 1 && Right != 1){
						XPos=13;
						YPos=17;
					} 
					if(Up != 1 && Down == 1 && Left != 1 && Right != 1){
						XPos=13;
						YPos=15;
					} 

					if(Up != 1 && Down != 1 && Left == 1 && Right != 1){
						XPos=12;
						YPos=18;
					} 
					if(Up != 1 && Down != 1 && Left != 1 && Right == 1){
						XPos=10;
						YPos=18;
					} 

					//vertical
					if(Up == 1 && Down == 1 && Left != 1 && Right != 1){
						XPos=13;
						YPos=16;
					} 
					//horizontal
					if(Up != 1 && Down != 1 && Left == 1 && Right == 1){
						XPos=11;
						YPos=18;
					} 

					//corner
					if(Up != 1 && Down == 1 && Left == 1 && Right != 1){
						XPos=12;
						YPos=15;
					} 
					if(Up != 1 && Down == 1 && Left != 1 && Right == 1){
						XPos=10;
						YPos=15;
					} 
					if(Up == 1 && Down != 1 && Left == 1 && Right != 1){
						XPos=12;
						YPos=17;
					} 
					if(Up == 1 && Down != 1 && Left != 1 && Right == 1){
						XPos=10;
						YPos=17;
					} 

					//3 way
					if(Up != 1 && Down == 1 && Left == 1 && Right == 1){
						XPos=11;
						YPos=15;
					} 
					if(Up == 1 && Down != 1 && Left == 1 && Right == 1){
						XPos=11;
						YPos=17;
					} 
					if(Up == 1 && Down == 1 && Left != 1 && Right == 1){
						XPos=10;
						YPos=16;
					} 
					if(Up == 1 && Down == 1 && Left == 1 && Right != 1){
						XPos=12;
						YPos=16;
					} 



				}
				if(basetype[CurrentPosition] == 2 || basetype[CurrentPosition] == 7){ 
					XPos = 7;
					YPos = 5;

					if(Up == 7)Up = 2;
					if(Down == 7)Down = 2;
					if(UpLeft == 7)UpLeft = 2;
					if(DownLeft == 7)DownLeft = 2;
					if(UpRight == 7)UpRight = 2;
					if(DownRight == 7)DownRight = 2;
					if(Left == 7)Left = 2;
					if(Right == 7)Right = 2;



					if(Up != 2 && Down != 2 && Left != 2 && Right != 2){
						XPos=9;
						YPos=4;
					} 

					//
					if(Up != 2 && Down != 2 && Left == 2 && Right != 2){
						XPos=8;
						YPos=7;
					} 
					if(Up != 2 && Down != 2 && Left != 2 && Right == 2){
						XPos=6;
						YPos=7;
					} 
					if(Up == 2 && Down != 2 && Left != 2 && Right != 2){
						XPos=5;
						YPos=6;
					} 
					if(Up != 2 && Down == 2 && Left != 2 && Right != 2){
						XPos=5;
						YPos=4;
					} 

					if(Up == 2 && Down == 2 && Left != 2 && Right != 2){
						XPos=5;
						YPos=5;
					} 
					if(Up != 2 && Down != 2 && Left == 2 && Right == 2){
						XPos=7;
						YPos=7;
					} 

					//corners
					if(Up == 2 && Down != 2 && Left == 2 && Right != 2){
						XPos=8;
						YPos=6;
					} 
					if(Up == 2 && Down != 2 && Left != 2 && Right == 2){
						XPos=6;
						YPos=6;
					} 
					if(Up != 2 && Down == 2 && Left == 2 && Right != 2){
						XPos=8;
						YPos=4;
					} 
					if(Up != 2 && Down == 2 && Left != 2 && Right == 2){
						XPos=6;
						YPos=4;
					} 


					//sides
					if(Up != 2 && Down == 2 && Left == 2 && Right == 2){
						XPos=7;
						YPos=4;
					} 
					if(Up == 2 && Down != 2 && Left == 2 && Right == 2){
						XPos=7;
						YPos=6;
					} 
					if(Up == 2 && Down == 2 && Left != 2 && Right == 2){
						XPos=6;
						YPos=5;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right != 2){
						XPos=8;
						YPos=5;
					} 

					//corners with inner corners
					if(Up == 2 && Down != 2 && Left == 2 && Right != 2 && UpLeft != 2){
						XPos=10;
						YPos=7;
					} 
					if(Up == 2 && Down != 2 && Left != 2 && Right == 2 && UpRight != 2){
						XPos=9;
						YPos=7;
					} 
					if(Up != 2 && Down == 2 && Left == 2 && Right != 2 && DownLeft != 2){
						XPos=10;
						YPos=6;
					} 
					if(Up != 2 && Down == 2 && Left != 2 && Right == 2 && DownRight != 2){
						XPos=9;
						YPos=6;
					} 


					//sides with one corner
					if(Up != 2 && Down == 2 && Left == 2 && Right == 2 && DownLeft != 2){
						XPos=15;
						YPos=4;
					} 
					if(Up == 2 && Down != 2 && Left == 2 && Right == 2  && UpLeft != 2){
						XPos=15;
						YPos=8;
					} 
					if(Up == 2 && Down == 2 && Left != 2 && Right == 2 && UpRight != 2){
						XPos=12;
						YPos=7;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right != 2 && UpLeft != 2){
						XPos=16;
						YPos=7;
					} 


					if(Up != 2 && Down == 2 && Left == 2 && Right == 2 && DownRight != 2){
						XPos=13;
						YPos=4;
					} 
					if(Up == 2 && Down != 2 && Left == 2 && Right == 2  && UpRight != 2){
						XPos=13;
						YPos=8;
					} 
					if(Up == 2 && Down == 2 && Left != 2 && Right == 2 && DownRight != 2){
						XPos=12;
						YPos=5;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right != 2 && DownLeft != 2){
						XPos=16;
						YPos=5;
					} 




					//sides with 2 corners
					if(Up != 2 && Down == 2 && Left == 2 && Right == 2 && DownLeft != 2 && DownRight != 2){
						XPos=14;
						YPos=4;
					} 
					if(Up == 2 && Down != 2 && Left == 2 && Right == 2  && UpLeft != 2 && UpRight != 2){
						XPos=14;
						YPos=8;
					} 
					if(Up == 2 && Down == 2 && Left != 2 && Right == 2 && UpRight != 2 && DownRight != 2){
						XPos=12;
						YPos=6;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right != 2 && UpLeft != 2 && DownLeft != 2){
						XPos=16;
						YPos=6;
					} 


					//one inner corner
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2 && UpRight != 2){
						XPos=16;
						YPos=4;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2  && UpLeft != 2){
						XPos=12;
						YPos=4;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2 && DownRight != 2){
						XPos=16;
						YPos=8;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2 && DownLeft != 2){
						XPos=12;
						YPos=8;
					} 

					//two corners only
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2 && UpRight != 2 && UpLeft != 2){
						XPos=14;
						YPos=7;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2  && UpLeft != 2 && DownLeft !=2){
						XPos=15;
						YPos=6;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2 && DownRight != 2 && DownLeft != 2){
						XPos=14;
						YPos=5;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2 && DownRight != 2 && UpRight != 2){
						XPos=13;
						YPos=6;
					} 


					//three corners only
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2 && UpRight != 2 && UpLeft != 2 && DownLeft != 2){
						XPos=15;
						YPos=7;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2  && UpLeft != 2 && DownLeft !=2 && DownRight != 2){
						XPos=15;
						YPos=5;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2 && DownRight != 2 && DownLeft != 2 && UpRight != 2){
						XPos=13;
						YPos=5;
					} 
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2 && DownRight != 2 && UpRight != 2 && UpLeft != 2){
						XPos=13;
						YPos=7;
					} 

					//all corners
					if(Up == 2 && Down == 2 && Left == 2 && Right == 2 && DownRight != 2 && UpRight != 2 && UpLeft != 2 && DownLeft != 2){
						XPos=14;
						YPos=6;
					} 



				}
				if(basetype[CurrentPosition] == 3){ 
					XPos = 9;
					YPos = 4;
				}


			}


			vertices[position].texture = XMFLOAT2(XPos*XDiff,(YPos-1)*YDiff);
			vertices[position+1].texture = XMFLOAT2((XPos-1)*XDiff,YPos*YDiff);
			vertices[position+2].texture = XMFLOAT2((XPos-1)*XDiff,(YPos-1)*YDiff);
			
			vertices[position+3].texture = XMFLOAT2(XPos*XDiff,(YPos-1)*YDiff);
			vertices[position+4].texture = XMFLOAT2(XPos*XDiff,YPos*YDiff);
			vertices[position+5].texture = XMFLOAT2((XPos-1)*XDiff,YPos*YDiff);
			
		}
	}

	// create a D3D11_BUFFER_DESC struct that describes the buffer data
	D3D11_BUFFER_DESC vertex_desc;
	ZeroMemory( &vertex_desc, sizeof( vertex_desc ) );
	vertex_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_desc.ByteWidth = sizeof( VertexStruct) * (width*height*6);

	// create a D3D11_SUBRESOURCE_DATA struct to hold the buffer data
	D3D11_SUBRESOURCE_DATA resource_data;
	ZeroMemory( &resource_data, sizeof( resource_data ) );
	resource_data.pSysMem = vertices;
	
	// using the current device context, generate the vertex buffer
	HRESULT d3dResult = device->CreateBuffer( &vertex_desc, &resource_data, &g_vertexBuffer );

	// delete the Vertex array
	delete[] vertices;
	

}

void WorldGrid::BuildPlayerBuffer(ID3D11Device* device,float TileX, float TileY){
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

	if(playerDirection == 1){//up
		XPos = 1;
		YPos = 2;
	}
	if(playerDirection == 2){//down
		XPos = 1;
		YPos = 3;
	}
	if(playerDirection == 3){//left
		XPos = 1;
		YPos = 4;
	}
	if(playerDirection == 4){//right
		XPos = 1;
		YPos = 5;
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
	HRESULT d3dResult = device->CreateBuffer( &vertex_desc, &resource_data, &g_vertexPlayerBuffer );


	// delete the Vertex array
	delete[] vertices;

}

void WorldGrid::BuildExitBuffer(ID3D11Device* device,float TileX, float TileY){
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

	int XPos = 3;
	int YPos = 2;

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
	HRESULT d3dResult = device->CreateBuffer( &vertex_desc, &resource_data, &g_vertexExitBuffer );


	// delete the Vertex array
	delete[] vertices;

}

void WorldGrid::BuildHealthBuffer(ID3D11Device* device){
	bufferHealth = playerHealth;

	float BarPecentage = playerHealth/playerMaxHealth;
	float BarTotalWidth = 0.8f;
	float BarNewWidth = BarTotalWidth * BarPecentage;

	VertexStruct* vertices = new VertexStruct[6];

	vertices[0].position = XMFLOAT3( BarNewWidth/2, 0.95f , 1.0f );
	vertices[1].position = XMFLOAT3( -(BarNewWidth/2), 0.9f, 1.0f );
	vertices[2].position = XMFLOAT3( -(BarNewWidth/2), 0.95f, 1.0f );
			
	vertices[3].position = XMFLOAT3( BarNewWidth/2, 0.95f, 1.0f );
	vertices[4].position = XMFLOAT3( BarNewWidth/2, 0.9f, 1.0f );
	vertices[5].position = XMFLOAT3( -(BarNewWidth/2), 0.9f, 1.0f );

	float XDiff = 1.0f/20.0f;
	float YDiff = 0.05f;

	int XPos = 15;
	int YPos = 1;

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

void WorldGrid::BuildUIBuffer(ID3D11Device* device){
	bufferHealth = playerHealth;

	float BarPecentage = playerHealth/playerMaxHealth;
	float BarTotalWidth = 0.8f;
	float BarNewWidth = BarTotalWidth * BarPecentage;

	VertexStruct* vertices = new VertexStruct[18];
	float XDiff = 1.0f/20.0f;
	float YDiff = 0.05f;
	int XPos = 2;
	int YPos = 1;

	vertices[0].position = XMFLOAT3( -0.32f, 0.9f , 1.0f );
	vertices[1].position = XMFLOAT3( -0.42f, 0.8f, 1.0f );
	vertices[2].position = XMFLOAT3( -0.42f, 0.9f, 1.0f );	
	vertices[3].position = XMFLOAT3( -0.32f, 0.9f, 1.0f );
	vertices[4].position = XMFLOAT3( -0.32f, 0.8f, 1.0f );
	vertices[5].position = XMFLOAT3( -0.42f, 0.8f, 1.0f );
	XPos = 2;
	YPos = 1;
	vertices[0].texture = XMFLOAT2(XPos*XDiff,(YPos-1)*YDiff);
	vertices[1].texture = XMFLOAT2((XPos-1)*XDiff,YPos*YDiff);
	vertices[2].texture = XMFLOAT2((XPos-1)*XDiff,(YPos-1)*YDiff);	
	vertices[3].texture = XMFLOAT2(XPos*XDiff,(YPos-1)*YDiff);
	vertices[4].texture = XMFLOAT2(XPos*XDiff,YPos*YDiff);
	vertices[5].texture = XMFLOAT2((XPos-1)*XDiff,YPos*YDiff);

	vertices[6].position = XMFLOAT3( -0.22f, 0.9f , 1.0f );
	vertices[7].position = XMFLOAT3( -0.32f, 0.8f, 1.0f );
	vertices[8].position = XMFLOAT3( -0.32f, 0.9f, 1.0f );	
	vertices[9].position = XMFLOAT3( -0.22f, 0.9f, 1.0f );
	vertices[10].position = XMFLOAT3( -0.22f, 0.8f, 1.0f );
	vertices[11].position = XMFLOAT3( -0.32f, 0.8f, 1.0f );
	XPos = 3;
	YPos = 1;
	vertices[6].texture = XMFLOAT2(XPos*XDiff,(YPos-1)*YDiff);
	vertices[7].texture = XMFLOAT2((XPos-1)*XDiff,YPos*YDiff);
	vertices[8].texture = XMFLOAT2((XPos-1)*XDiff,(YPos-1)*YDiff);	
	vertices[9].texture = XMFLOAT2(XPos*XDiff,(YPos-1)*YDiff);
	vertices[10].texture = XMFLOAT2(XPos*XDiff,YPos*YDiff);
	vertices[11].texture = XMFLOAT2((XPos-1)*XDiff,YPos*YDiff);


	vertices[12].position = XMFLOAT3( -0.12f, 0.9f , 1.0f );
	vertices[13].position = XMFLOAT3( -0.22f, 0.8f, 1.0f );
	vertices[14].position = XMFLOAT3( -0.22f, 0.9f, 1.0f );	
	vertices[15].position = XMFLOAT3( -0.12f, 0.9f, 1.0f );
	vertices[16].position = XMFLOAT3( -0.12f, 0.8f, 1.0f );
	vertices[17].position = XMFLOAT3( -0.22f, 0.8f, 1.0f );
	XPos = 4;
	YPos = 1;
	vertices[12].texture = XMFLOAT2(XPos*XDiff,(YPos-1)*YDiff);
	vertices[13].texture = XMFLOAT2((XPos-1)*XDiff,YPos*YDiff);
	vertices[14].texture = XMFLOAT2((XPos-1)*XDiff,(YPos-1)*YDiff);	
	vertices[15].texture = XMFLOAT2(XPos*XDiff,(YPos-1)*YDiff);
	vertices[16].texture = XMFLOAT2(XPos*XDiff,YPos*YDiff);
	vertices[17].texture = XMFLOAT2((XPos-1)*XDiff,YPos*YDiff);



	// create a D3D11_BUFFER_DESC struct that describes the buffer data
	D3D11_BUFFER_DESC vertex_desc;
	ZeroMemory( &vertex_desc, sizeof( vertex_desc ) );
	vertex_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_desc.ByteWidth = sizeof( VertexStruct) * (18);

	// create a D3D11_SUBRESOURCE_DATA struct to hold the buffer data
	D3D11_SUBRESOURCE_DATA resource_data;
	ZeroMemory( &resource_data, sizeof( resource_data ) );
	resource_data.pSysMem = vertices;
	
	// using the current device context, generate the vertex buffer
	HRESULT d3dResult = device->CreateBuffer( &vertex_desc, &resource_data, &g_vertexUIBuffer );


	// delete the Vertex array
	delete[] vertices;

}

void WorldGrid::Render(ID3D11DeviceContext* deviceContext)
{
	// Puts the vertex and index buffers on the graphics pipeline
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexStruct); 
	offset = 0;

	//grid
	XMMATRIX scale_mat = XMMatrixScaling(1,1,1);
	XMMATRIX rotation_mat = XMMatrixRotationRollPitchYaw(0,0,0);
	XMMATRIX position_mat = XMMatrixTranslation(GetXPos() - SmoothXOffset, GetYPos()  - SmoothYOffset, 0);
	//XMMATRIX position_mat = XMMatrixTranslation(0, 1, 0);
	XMMATRIX world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );
	if(fullsize == false){
		deviceContext->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );
		deviceContext->VSSetConstantBuffers( 0, 1, &worldCB_ );
	}
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &g_vertexBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->Draw( width*height*6, 0 );


	//exit
	scale_mat = XMMatrixScaling(1,1,1);
	rotation_mat = XMMatrixRotationRollPitchYaw(0,0,0);
	if(fullsize == true){
		position_mat = XMMatrixTranslation((MyTileWidth * ExitPosX), -(MyTileHeight * ExitPosY), 0);
	}
	else{
		position_mat = XMMatrixTranslation((MyTileWidth * ExitPosX) + GetXPos() - SmoothXOffset, -(MyTileHeight * ExitPosY) + GetYPos()  - SmoothYOffset, 0);
	}
	world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );
	deviceContext->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &worldCB_ );

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &g_vertexExitBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->Draw(6, 0 );


	//chests
	for(int i = 0; i < MAX_CHEST; i++){
		ChestArray[i]->Render(deviceContext, MyTileWidth, MyTileHeight, GetXPos()  - SmoothXOffset, GetYPos()  - SmoothYOffset, fullsize);
	}

	//player
	scale_mat = XMMatrixScaling(1,1,1);
	rotation_mat = XMMatrixRotationRollPitchYaw(0,0,0);
	if(fullsize == true){
		position_mat = XMMatrixTranslation((MyTileWidth * (gridPosX)) + SmoothXOffset, -(MyTileHeight * gridPosY) + SmoothYOffset, 0);
	}
	else{
		position_mat = XMMatrixTranslation((MyTileWidth * (gridPosX)) + GetXPos(), -(MyTileHeight * gridPosY) + GetYPos(), 0);
	}
	world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );
	deviceContext->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &worldCB_ );

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &g_vertexPlayerBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->Draw(6, 0 );

	//enemies
	for(int e = 0; e < CurrentEnemyNumber; e++){
		if(EnemyArray[e]->GetHealth() > 0){
			EnemyArray[e]->Render(deviceContext, MyTileWidth, MyTileHeight, GetXPos()  - SmoothXOffset, GetYPos()  - SmoothYOffset, fullsize);
		}
	}



	//healthbar
	scale_mat = XMMatrixScaling(1,1,1);
	rotation_mat = XMMatrixRotationRollPitchYaw(0,0,0);
	position_mat = XMMatrixTranslation(0, 0, 0);
	world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );
	deviceContext->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &worldCB_ );

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &g_vertexHealthBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->Draw(6, 0 );


	//UI
	scale_mat = XMMatrixScaling(1,1,1);
	rotation_mat = XMMatrixRotationRollPitchYaw(0,0,0);
	position_mat = XMMatrixTranslation(0, 0, 0);
	world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );
	deviceContext->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &worldCB_ );

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &g_vertexUIBuffer, &stride, &offset);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->Draw(18, 0 );

	return;
}

void WorldGrid::SetFullsize(bool full, ID3D11Device* device,float TileX, float TileY){
	fullsize = full;
	BuildBuffer(device, TileX, TileY);
}

void WorldGrid::NewRoom(int position){

	int RoomHLength;
	int RoomHHeight;
	bool RoomSafe = true;

	RoomHLength = RanInt(1, 8);
	RoomHHeight = RanInt(1, 8);

	div_t Roomtest;
	div_t RoomPositiontest;
	RoomPositiontest = div (position, width);
	Roomtest = div (position - RoomHLength - 1, width);

	if(Roomtest.quot != RoomPositiontest.quot){
		RoomSafe = false;
	}
	if(RoomSafe == true){
		Roomtest = div (position + RoomHLength + 1, width);

		if(Roomtest.quot != RoomPositiontest.quot){
			RoomSafe = false;
		}

		if(RoomSafe == true){
			if(position - (RoomHHeight + 1) * width < 0){
				RoomSafe = false;
			}
			if(RoomSafe == true){
				if(position + (RoomHHeight + 1) * width > width*height){
				RoomSafe = false;
				}
			}
		}
	}
	if(RoomSafe == true){
		for(int h = 0; h < (RoomHHeight*2 + 1); h++){
			//basetype[(position - RoomHHeight * height) + h * width] = 1;
			int HPOS = (position - RoomHHeight * height) + h * width;
			for(int l = 0; l < (RoomHLength*2 + 1); l++){
				basetype[(HPOS - RoomHLength) + l] = 2;
			}
		}

		//basetype[position] = 1;
	}
}

void WorldGrid::NewPath(int position, int length, int direction){

	bool PathLast = false;

	for(int i = 0; i<length; i++){
		if(direction == 1){
			basetype[(position - i*width)] = 2;
		}
		if(direction == 2){
			basetype[(position + i*width)] = 2;
		}
		if(direction == 3){
			basetype[(position + i)] = 2;
		}
		if(direction == 4){
			basetype[(position - i)] = 2;
		}

		if(PathLast == false){

		//random path addition
		int Chance;
		float Chanceof;
		if (Paths == 0){
			Chanceof = 60;
		}
		else{
			Chanceof = 90;
		}

		Chance = RanInt(0,100);
		if(Chance > Chanceof){
			int mydirection;
			mydirection = RanInt(0,2)+1;
			int MyPos;
			if(direction == 1){
				MyPos=(position - i*width);
				mydirection+=2;
			}
			if(direction == 2){
				MyPos=(position + i*width);
				mydirection+=2;
			}
			if(direction == 3){
				MyPos=(position + i);
			}
			if(direction == 4){
				MyPos=(position - i);
			}
			if(Paths < 30){
				MyPaths[Paths].Real = true;
				MyPaths[Paths].Position = MyPos;
				MyPaths[Paths].Direction = mydirection;
				Paths++;
			}
			//NewPath(MyPos, RanInt(0,MaxLength(MyPos, mydirection)), mydirection);
			PathLast = true;
		}
		}
		else{
			PathLast = false;
		}

		//Random Room spawn
		int ChanceofRoom = 96;
		int RoomChance;
		RoomChance = RanInt(0, 100);
		int RoomPos;
		if(RoomChance > ChanceofRoom){

			if(direction == 1){
				RoomPos=(position - i*width);
			}
			if(direction == 2){
				RoomPos=(position + i*width);
			}
			if(direction == 3){
				RoomPos=(position + i);
			}
			if(direction == 4){
				RoomPos=(position - i);
			}

			if(Rooms < 50){
				MyRooms[Rooms].Real = true;
				MyRooms[Rooms].Position = RoomPos;
				MyRooms[Rooms].Direction = 1;
				Rooms++;
			}

		}

	}

}

void WorldGrid::RimDetection(int oldType, int newtype, int checkType){
	//WALL DETECTION
	for(int w = 0; w < width*height; w++){
		if(basetype[w] == checkType){

			if(w > width-1){
				if(basetype[w - width] == oldType){
					basetype[w - width] = newtype;
				}
				if(basetype[w - width+1] == oldType){
					basetype[w - width+1] = newtype;
				}
				if(basetype[w - width-1] == oldType){
					basetype[w - width-1] = newtype;
				}
			}

			if(w < width*(height-1)){
				if(basetype[w + width] == oldType){
					basetype[w + width] = newtype;
				}
				if(basetype[w + width+1] == oldType){
					basetype[w + width+1] = newtype;
				}
				if(basetype[w + width-1] == oldType){
					basetype[w + width-1] = newtype;
				}
			}

			div_t TestP;
			TestP = div(w, 120);

			div_t TestW;
			TestW = div(w-1, 120);
			if(TestP.quot == TestW.quot){
				if(basetype[w-1] == oldType){
					basetype[w-1] = newtype;
				}
			}
			TestW = div(w+1, 120);
			if(TestP.quot == TestW.quot){
				if(basetype[w+1] == oldType){
					basetype[w+1] = newtype;
				}
			}

		}
	}
}

void WorldGrid::ReplaceAll(int oldtype, int newType){
	for(int w = 0; w < width*height; w++){
		if(basetype[w] == oldtype){
			basetype[w] = newType;
		}
	}
}

void WorldGrid::AddSpreadingNodes(int old, int neww, int check, int chance){
	for(int w = 0; w < width*height; w++){
		if(basetype[w] == check){

			int MyChance;
			MyChance = RanInt(0,100);

			if(MyChance > chance){
				SpreadingNodes[SpreadingNum].Real = true;
				SpreadingNodes[SpreadingNum].Position = w;
				SpreadingNodes[SpreadingNum].checkType = check;
				SpreadingNodes[SpreadingNum].oldType = old;
				SpreadingNodes[SpreadingNum].newType = neww;
				//basetype[w] = 5;
				SpreadingNum++;

			}
		}
	}
}

void WorldGrid::SpreadNodes(){
	int ArrayPos;
	int SpreadChance = 60;
	int Chance;
	for (int NN = 0; NN < 1000; NN++){

		if(SpreadingNodes[NN].Real == true){
			
			for(int i = 0; i < 50; i++){
				TempPositionArray[i].Real = false;
			}
			ArrayPos = 0;
			TempPositionArray[ArrayPos].Position = SpreadingNodes[NN].Position;
			TempPositionArray[ArrayPos].Real = true;
			ArrayPos++;
			
			
			for(int p = 0; p < 50; p++){
				if(TempPositionArray[p].Real == true){
					if(basetype[TempPositionArray[p].Position] == SpreadingNodes[NN].checkType){
						basetype[TempPositionArray[p].Position] = SpreadingNodes[NN].newType;
						Chance = RanInt(0,100);
						if(Chance > SpreadChance && ArrayPos < 50){//up
							TempPositionArray[ArrayPos].Position = TempPositionArray[p].Position-width;
							TempPositionArray[ArrayPos].Real = true;
							ArrayPos++;
						}
						Chance = RanInt(0,100);
						if(Chance > SpreadChance && ArrayPos < 50){//down
							TempPositionArray[ArrayPos].Position = TempPositionArray[p].Position+width;
							TempPositionArray[ArrayPos].Real = true;
							ArrayPos++;
						}
						Chance = RanInt(0,100);
						if(Chance > SpreadChance && ArrayPos < 50){//left
							TempPositionArray[ArrayPos].Position = TempPositionArray[p].Position-1;
							TempPositionArray[ArrayPos].Real = true;
							ArrayPos++;
						}
						Chance = RanInt(0,100);
						if(Chance > SpreadChance && ArrayPos < 50){//right
							TempPositionArray[ArrayPos].Position = TempPositionArray[p].Position+1;
							TempPositionArray[ArrayPos].Real = true;
							ArrayPos++;
						}


						TempPositionArray[p].Real = false;
					}
				}

			}
			
		}
	}
}

void WorldGrid::PopulateEnemies(int CheckType){
	for(int w = 0; w < width*height; w++){
		if(basetype[w] == CheckType){
			if(CurrentEnemyNumber < MAX_ENEMY){
				int Chance = RanInt(0,100);
				if(Chance > 95){
					div_t Height;
					Height = div (w, width);
					if(w - Height.quot != gridPosX && Height.quot != gridPosY){
						EnemyArray[CurrentEnemyNumber] = new Enemy(width, height, w - Height.quot * width, Height.quot, floorNumber - 1);
						CurrentEnemyNumber++;
					}
				}
			}
		}
	}
}

void WorldGrid::AddDots(int oldType, int newType, int chanceNum){
	for(int w = 0; w < width*height; w++){
		if(basetype[w] == oldType){
			int Chance = RanInt(0,100);
				if(Chance > chanceNum){
					basetype[w] = newType;
				}
		}
	}
}

void WorldGrid::PopulateChests(int CheckType){
	for(int i = 0; i < MAX_CHEST; i++){

		int ChestX = 0;
		int ChestY = 0;
		
		while(basetype[ChestY*width + ChestX] != CheckType){
			ChestY = RanInt(0, height);
			ChestX = RanInt(0, width);
		}

		ChestArray[i]  = new Chest(width, height, ChestX, ChestY);

	}
}

void WorldGrid::RecalculateEntityCollisionGrid(){
	for(int x = 0; x < width*height; x++){
		entitycollidable[x] = 0;
	}

	entitycollidable[gridPosY * width + gridPosX] = 1;
	//entitycollidable[ExitPosY * width + ExitPosX] = 1;

	for(int e = 0; e < CurrentEnemyNumber; e++){
		if(EnemyArray[e]->GetHealth() > 0){
			entitycollidable[EnemyArray[e]->GetY() * width + EnemyArray[e]->GetX()] = 1;
		}
	}
}

int WorldGrid::MaxLength(int position, int direction){
	if(direction == 1){//up
		div_t MyDiv;
		MyDiv = div(position,width);
		return MyDiv.quot + 1;
	}
	else if(direction == 2){//down
		div_t MyDiv;
		MyDiv = div(position,width);
		return height - MyDiv.quot;
	}
	else if(direction == 3){//right
		div_t MyDiv;
		MyDiv = div(position,width);
		return ((MyDiv.quot + 1) * width) - position; //done
	}
	else if(direction == 4){//left
		div_t MyDiv;
		MyDiv = div(position,width);
		return position - MyDiv.quot*width + 1;
	}else{
		return 0;
	}

}

int WorldGrid::RanInt(int min, int max){

	return rand() %(max-min) + min;

}

void WorldGrid::GridUpdate(float dt){
	MovementTimer += 10 * dt;
	AttackTimer += 10 * dt;
	//MovementTimer = 1;

	if(playerHealth < 0){
		playerHealth = 0;
	}

	if(MovementTimer > 1){
		MovementTimer = 1;
	}
	
	if(MovementDirection == 1){
		SmoothXOffset = 0;
		SmoothYOffset = -(MyTileHeight *  (1 - MovementTimer));
	}
	if(MovementDirection == 2){
		SmoothXOffset = 0;
		SmoothYOffset = (MyTileHeight *  (1 - MovementTimer));
	}
	if(MovementDirection == 3){
		SmoothXOffset = (MyTileWidth*  (1 - MovementTimer));
		SmoothYOffset = 0;
	}
	if(MovementDirection == 4){
		SmoothXOffset = -(MyTileWidth*  (1 - MovementTimer));
		SmoothYOffset = 0;
	}

	if(SmoothXOffset < -MyTileWidth){
		SmoothXOffset = -MyTileWidth;
	}
	if(SmoothXOffset > MyTileWidth){
		SmoothXOffset = MyTileWidth;
	}
	if(SmoothYOffset > MyTileHeight){
		SmoothYOffset = MyTileHeight;
	}
	if(SmoothYOffset < -MyTileHeight){
		SmoothYOffset = -MyTileHeight;
	}
	


	if((gridPosY) * width + gridPosX == (ExitPosY) * width + ExitPosX){
		NeedReload = true;
		floorNumber++;
		if(TestingWorldType == false){
			WorldType = RanInt(0,5) + 1;
		}
		else{
			WorldType = WorldType;
		}
		//ReplaceAll(1, 0);

		for(int e = 0; e < CurrentEnemyNumber; e++){
			EnemyArray[e]->unloadAll();
			//EnemyArray[e] = NULL;
			//delete EnemyArray[e];
		}
		for(int i = 0; i < MAX_CHEST; i++){
			ChestArray[i]->unloadAll();
			//ChestArray[i] = NULL;
			//delete ChestArray[i];
		}

	}

	int EnemiesLeft = 0;

	RecalculateEntityCollisionGrid();
	//enemies
	for(int e = 0; e < CurrentEnemyNumber; e++){
		if(EnemyArray[e]->GetHealth() > 0){
		EnemiesLeft++;
		int u = basetype[((EnemyArray[e]->GetY() - 1) * width + EnemyArray[e]->GetX())];
		int d = basetype[((EnemyArray[e]->GetY() + 1) * width + EnemyArray[e]->GetX())];
		int l = basetype[((EnemyArray[e]->GetY()) * width + EnemyArray[e]->GetX() - 1)];
		int r = basetype[((EnemyArray[e]->GetY()) * width + EnemyArray[e]->GetX() + 1)];
		int ul = basetype[((EnemyArray[e]->GetY() - 1) * width + EnemyArray[e]->GetX() - 1)];
		int dl = basetype[((EnemyArray[e]->GetY() + 1) * width + EnemyArray[e]->GetX() - 1)];
		int ur = basetype[((EnemyArray[e]->GetY() - 1) * width + EnemyArray[e]->GetX() + 1)];
		int dr = basetype[((EnemyArray[e]->GetY() + 1) * width + EnemyArray[e]->GetX() + 1)];
		int c = basetype[((EnemyArray[e]->GetY()) * width + EnemyArray[e]->GetX())];
		if(entitycollidable[((EnemyArray[e]->GetY() - 1) * width + EnemyArray[e]->GetX())] == 1){
			u = 1;
		}
		if(entitycollidable[((EnemyArray[e]->GetY() + 1) * width + EnemyArray[e]->GetX())] == 1){
			d = 1;
		}
		if(entitycollidable[((EnemyArray[e]->GetY()) * width + EnemyArray[e]->GetX() - 1)] == 1){
			l = 1;
		}
		if(entitycollidable[((EnemyArray[e]->GetY()) * width + EnemyArray[e]->GetX() + 1)] == 1){
			r = 1;
		}
		if(entitycollidable[((EnemyArray[e]->GetY() - 1) * width + EnemyArray[e]->GetX() - 1) ] == 1){
			ul = 1;
		}
		if(entitycollidable[((EnemyArray[e]->GetY() + 1) * width + EnemyArray[e]->GetX() - 1)] == 1){
			dl = 1;
		}
		if(entitycollidable[((EnemyArray[e]->GetY() - 1) * width + EnemyArray[e]->GetX() + 1) ] == 1){
			ur = 1;
		}
		if(entitycollidable[((EnemyArray[e]->GetY() + 1) * width + EnemyArray[e]->GetX() + 1)] == 1){
			dr = 1;
		}

		EnemyArray[e]->UpdateSurroudings(u, d, l, r, ul, ur, dl, dr, c);

		if(EnemyArray[e]->RequiresPath() == true){
			CalculatePath(e);
		}


		EnemyArray[e]->EnemyUpdate(dt, gridPosY * width + gridPosX);

		if(EnemyArray[e]->getPlayerHit() == true){
			int DamageTaken = EnemyArray[e]->GetStrength() - playerDefense;
			if(DamageTaken > 0){
				playerHealth -= DamageTaken;
			}
			EnemyArray[e]->PlayerHitOff();
		}

		}
	}

	if(EnemiesLeft == 0 && floorCleared == false){
		floorCleared = true;
		playerStrength +=2;
		playerMaxHealth += 10;
		playerDefense += 2;
	}

}

bool WorldGrid::GetReload(){
	return NeedReload;
}

void WorldGrid::GridDown(ID3D11Device* device){
	playerDirection = 1;
	BuildPlayerBuffer(device, MyTileX, MyTileY);
	if(MovementTimer >= 1){
		if(gridPosY > 0){
			if(collidable[(gridPosY-1) * width + gridPosX] != 1 && entitycollidable[(gridPosY-1) * width + gridPosX] != 1){
				gridPosY--;
				MovementTimer = 0;
				for(int e = 0; e < CurrentEnemyNumber; e++){
					EnemyArray[e]->PlayerMoved();
					MovementDirection = 1;
				}
			}
		}
	}
}

void WorldGrid::GridUp(ID3D11Device* device){
	playerDirection = 2;
	BuildPlayerBuffer(device, MyTileX, MyTileY);
	if(MovementTimer >= 1){
		if(gridPosY < height){
			if(collidable[(gridPosY+1) * width + gridPosX] != 1  && entitycollidable[(gridPosY+1) * width + gridPosX] != 1){
				gridPosY++;
				MovementTimer = 0;
				for(int e = 0; e < CurrentEnemyNumber; e++){
					EnemyArray[e]->PlayerMoved();
					MovementDirection = 2;
				}
			}
		}
	}

}

void WorldGrid::GridLeft(ID3D11Device* device){
	playerDirection = 3;
	BuildPlayerBuffer(device, MyTileX, MyTileY);
	if(MovementTimer >= 1){
		if(gridPosX > 0){
			if(collidable[(gridPosY) * width + gridPosX-1] != 1  && entitycollidable[(gridPosY) * width + gridPosX-1] != 1){
				gridPosX--;
				MovementTimer = 0;
				for(int e = 0; e < CurrentEnemyNumber; e++){
					EnemyArray[e]->PlayerMoved();
					MovementDirection = 3;
				}
			}
		}
	}
}

void WorldGrid::GridRight(ID3D11Device* device){
	playerDirection = 4;
	BuildPlayerBuffer(device, MyTileX, MyTileY);
	if(MovementTimer >= 1){
		if(gridPosX < width){
			if(collidable[(gridPosY) * width + gridPosX+1] != 1  && entitycollidable[(gridPosY) * width + gridPosX + 1] != 1){
				gridPosX++;
				MovementTimer = 0;
				for(int e = 0; e < CurrentEnemyNumber; e++){
					EnemyArray[e]->PlayerMoved();
					MovementDirection = 4;
				}
			}
		}
	}
}

void WorldGrid::GridAttack(){
	if(AttackTimer > 8){

	int AttackX = 0;
	int AttackY = 0;
	if(playerDirection == 1){//up
		AttackX = gridPosX;
		AttackY = gridPosY - 1;
	}
	if(playerDirection == 2){//down
		AttackX = gridPosX;
		AttackY = gridPosY + 1;
	}
	if(playerDirection == 3){//left
		AttackX = gridPosX - 1;
		AttackY = gridPosY;
	}
	if(playerDirection == 4){//right
		AttackX = gridPosX + 1;
		AttackY = gridPosY;
	}


	for(int e = 0; e < CurrentEnemyNumber; e++){
		if(EnemyArray[e]->GetHealth() > 0){
			if(EnemyArray[e]->GetX() == AttackX){
				if(EnemyArray[e]->GetY() == AttackY){
					int DamageDone = playerStrength - EnemyArray[e]->GetDefense();
					if(DamageDone > 0){
						EnemyArray[e]->SetHealth(EnemyArray[e]->GetHealth() - DamageDone);
					}
				}
			}
		}
	}
	AttackTimer = 0;
	}
}

void WorldGrid::GridInteract(){
	for(int i = 0; i < MAX_CHEST; i++){
		if(ChestArray[i]->GetX() == gridPosX && ChestArray[i]->GetY() == gridPosY){
			if(ChestArray[i]->GetOpen() == false){
				ChestArray[i]->Open();
				if(ChestArray[i]->GetContents() == 0){
					playerMaxHealth += ChestArray[i]->GetAmount();
				}
				if(ChestArray[i]->GetContents() == 1){
					playerStrength += ChestArray[i]->GetAmount();
				}
				if(ChestArray[i]->GetContents() == 2){
					playerDefense += ChestArray[i]->GetAmount();
				}
				if(ChestArray[i]->GetContents() == 3){
					Potions ++;
				}
			}
		}
	}
}

void WorldGrid::GridPotion(){
	if(Potions > 0){
		Potions--;
		playerHealth+=100;
		if(playerHealth > playerMaxHealth){
			playerHealth = playerMaxHealth;
		}
	}
}

float WorldGrid::GetXPos(){
	return -(MyTileWidth * (gridPosX - 13));
}

float WorldGrid::GetYPos(){
	return (MyTileHeight * (gridPosY - 10));
}

ID3D11Buffer* const* WorldGrid::GetVertexBuffer() { return &g_vertexBuffer; }

void WorldGrid::CalculatePath(int e){
	if(EnemyArray[e]->GetHealth()>0){
	for(int x = 0; x < width*height; x++){
		PathCalculationArray[x].Real = false;
		PathCalculationArray[x].ParentArrayPosition = 0;
		PathCalculationArray[x].PositionX = 0;
		PathCalculationArray[x].PositionY = 0;
		PathChecked[x] = 0;
	}

	int playerPositionX = gridPosX;
	int playerPositionY = gridPosY;
	//int PathCalcState = 0; //0 - calculation, 1 - found, 2 - given up
	//int StartPos = EnemyPositionY * width + EnemyPositionX;


	//BFS pathfinding
	//we start with a point on our grid, and search all 4 squares around it, we then search around those squares and so on and so forth
	//so our start position is the position of our enemy
	//and the goal is the position of our player

	//first of all, lets add our start position to the queue
	PathCalculationArray[0].ParentArrayPosition = 0;
	PathCalculationArray[0].PositionX = EnemyArray[e]->GetX();
	PathCalculationArray[0].PositionY = EnemyArray[e]->GetY();
	PathCalculationArray[0].Real = 1;
	PathChecked[EnemyArray[e]->GetY() * width + EnemyArray[e]->GetX()] = 1;


	int CurrentQueuePosition = 0;
	int NumberinQueue = 1;
	TempPosition = 0;
	int PathCalcState = 0;

	int FinalArrayPosition = 0;

	while(PathCalcState == 0){

		TempPosition = PathCalculationArray[CurrentQueuePosition].PositionY * width + PathCalculationArray[CurrentQueuePosition].PositionX;
		if(PathCalculationArray[CurrentQueuePosition].Real == true){
		//above current position
		if(basetype[TempPosition - width] != 1){//we make sure above our current position isn't collidable
			if(PathChecked[TempPosition - width] == 0){//we make sure it hasn't already been added

				PathCalculationArray[NumberinQueue].ParentArrayPosition = CurrentQueuePosition;
				PathCalculationArray[NumberinQueue].PositionX = PathCalculationArray[CurrentQueuePosition].PositionX;
				PathCalculationArray[NumberinQueue].PositionY = PathCalculationArray[CurrentQueuePosition].PositionY - 1;
				PathCalculationArray[NumberinQueue].Real = 1;
				PathChecked[TempPosition - width] = 1;

				if((TempPosition - width) == (playerPositionY * width + playerPositionX)){
					PathCalcState = 1;
					FinalArrayPosition = NumberinQueue;
				}

				NumberinQueue++;
			}
		}

		//below current position
		if(basetype[TempPosition + width] != 1){//we make sure above our current position isn't collidable
			if(PathChecked[TempPosition + width] == 0){//we make sure it hasn't already been added

				PathCalculationArray[NumberinQueue].ParentArrayPosition = CurrentQueuePosition;
				PathCalculationArray[NumberinQueue].PositionX = PathCalculationArray[CurrentQueuePosition].PositionX;
				PathCalculationArray[NumberinQueue].PositionY = PathCalculationArray[CurrentQueuePosition].PositionY + 1;
				PathCalculationArray[NumberinQueue].Real = 1;
				PathChecked[TempPosition + width] = 1;

				if((TempPosition + width) == (playerPositionY * width + playerPositionX)){
					PathCalcState = 1;
					FinalArrayPosition = NumberinQueue;
				}

				NumberinQueue++;
			}
		}

		//left current position
		if(basetype[TempPosition - 1] != 1){//we make sure above our current position isn't collidable
			if(PathChecked[TempPosition - 1] == 0){//we make sure it hasn't already been added

				PathCalculationArray[NumberinQueue].ParentArrayPosition = CurrentQueuePosition;
				PathCalculationArray[NumberinQueue].PositionX = PathCalculationArray[CurrentQueuePosition].PositionX - 1;
				PathCalculationArray[NumberinQueue].PositionY = PathCalculationArray[CurrentQueuePosition].PositionY;
				PathCalculationArray[NumberinQueue].Real = 1;
				PathChecked[TempPosition - 1] = 1;

				if((TempPosition - 1) == (playerPositionY * width + playerPositionX)){
					PathCalcState = 1;
					FinalArrayPosition = NumberinQueue;
				}

				NumberinQueue++;
			}
		}

		//right current position
		if(basetype[TempPosition + 1] != 1){//we make sure above our current position isn't collidable
			if(PathChecked[TempPosition + 1] == 0){//we make sure it hasn't already been added

				PathCalculationArray[NumberinQueue].ParentArrayPosition = CurrentQueuePosition;
				PathCalculationArray[NumberinQueue].PositionX = PathCalculationArray[CurrentQueuePosition].PositionX + 1;
				PathCalculationArray[NumberinQueue].PositionY = PathCalculationArray[CurrentQueuePosition].PositionY;
				PathCalculationArray[NumberinQueue].Real = 1;
				PathChecked[TempPosition + 1] = 1;

				if((TempPosition +1) == (playerPositionY * width + playerPositionX)){
					PathCalcState = 1;
					FinalArrayPosition = NumberinQueue;
				}

				NumberinQueue++;
			}
		}
		}
		else{
			PathCalcState = 2;
		}

		CurrentQueuePosition++;
	}

	if(PathCalcState == 1){

	//with the pathfinding working correctly
	//we now need to pass the path to the enemy
	//we start at the our finalarrayposition and work our way back through the array following the parent array positions

	//to start with, lets find the length of our path
	int CurrentSearchPosition = FinalArrayPosition;
	int Length = 0;
	while (CurrentSearchPosition > 0){ //now we loop through our path to calculate the length of of the path
		CurrentSearchPosition = PathCalculationArray[CurrentSearchPosition].ParentArrayPosition;
		Length++;
	}
	EnemyArray[e]->SetPathLength(Length - 1); //now we tell the enemy how long its path is so we have an array thr right size for the path
	//we misnused 1 from the length as we dont want the enmy going to the same position as the player, only up to an adjoining position

	CurrentSearchPosition = FinalArrayPosition;//reset our starting position for our loop

	while (CurrentSearchPosition > 0){ //now we loop through our path again this time passing the positions into the enemies path array
		Length--;
		CurrentSearchPosition = PathCalculationArray[CurrentSearchPosition].ParentArrayPosition;
		EnemyArray[e]->AddPathPoint(Length - 1, PathCalculationArray[CurrentSearchPosition].PositionX, PathCalculationArray[CurrentSearchPosition].PositionY);
	}

	EnemyArray[e]->UpdatePath();
	}
	}
}

int WorldGrid::GetFloorNumber(){
	return floorNumber;
}

void WorldGrid::unloadAll(){

	MyPaths = NULL;
	MyRooms = NULL;
	SpreadingNodes = NULL;
	TempPositionArray = NULL;
	g_vertexBuffer->Release();
	g_vertexBuffer = NULL;
	g_vertexPlayerBuffer->Release();
	g_vertexPlayerBuffer = NULL;
	g_vertexExitBuffer->Release();
	g_vertexExitBuffer = NULL;
	g_vertexHealthBuffer->Release();
	g_vertexHealthBuffer = NULL;
	g_vertexUIBuffer->Release();
	g_vertexUIBuffer = NULL;
	worldCB_->Release();
	worldCB_ = NULL;
	basetype = NULL;
	collidable = NULL;
	entitycollidable = NULL;
	PathChecked = NULL;
	PathCalculationArray = NULL;

	for(int e = 0; e < CurrentEnemyNumber; e++){
		EnemyArray[e]->unloadAll();
		EnemyArray[e] = NULL;
		delete EnemyArray[e];
	}
	for(int i = 0; i < MAX_CHEST; i++){
		ChestArray[i]->unloadAll();
		ChestArray[i] = NULL;
		delete ChestArray[i];
	}

}

void WorldGrid::RebuildBuffer(ID3D11Device* device){
	for(int e = 0; e < CurrentEnemyNumber; e++){
		EnemyArray[e]->RebuildBuffer(device, MyTileX, MyTileY);
	}
	if(bufferHealth != playerHealth){
		BuildHealthBuffer(device);
	}
	for(int i = 0; i < MAX_CHEST; i++){
		ChestArray[i]->RebuildBuffer(device, MyTileX, MyTileY);
	}

}

int WorldGrid::GetStrength(){
	return playerStrength;
}

int WorldGrid::GetDefense(){
	return playerDefense;
}

int WorldGrid::GetPotions(){
	return Potions;
}

int WorldGrid::GetHealth(){
	return playerHealth;
}

int WorldGrid::GetMaxHealth(){
	return playerMaxHealth;
}

int WorldGrid::GetWorldType(){
	return WorldType;
}

void WorldGrid::LineEdge(int NewType){

	for(int i = 0; i < width; i ++){
		basetype[i] = NewType;
		basetype[i + width*(height-1)] = NewType;
	}
	for(int i = 0; i < height; i++){
		basetype[width*i] = NewType;
		basetype[width*i + width] = NewType;

	}

}

void WorldGrid::GenerateRandomPath(int oldType, int newType){

	int CurrentX = 0;
	int CurrentY = 0;
	bool pathDone = false;
	int NextX = 0;
	int NextY = 0;
	TempPosition = 0;
	int checked = 0;
	//exit spawn

	while(basetype[CurrentY*width + CurrentX] != oldType){
		CurrentX = RanInt(0, height);
		CurrentY = RanInt(0, width);
	}

	

	while(pathDone == false){

		basetype[(CurrentY) * width + CurrentX] = newType;

		PathCalculationArray[TempPosition].Real = true;
		PathCalculationArray[TempPosition].PositionX = CurrentX;
		PathCalculationArray[TempPosition].PositionY = CurrentY;
		PathCalculationArray[TempPosition].ParentArrayPosition = TempPosition-1;

		int RanDirection = RanInt(0,4) + 1;
		
		if(RanDirection == 1){//up first,d , l, r
			if(basetype[(CurrentY - 2) * width + CurrentX] == oldType){
				basetype[(CurrentY - 2) * width + CurrentX] = newType;
				basetype[(CurrentY - 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY - 2;
			}
			else if(basetype[(CurrentY + 2) * width + CurrentX] == oldType){
				basetype[(CurrentY + 2) * width + CurrentX] = newType;
				basetype[(CurrentY + 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY + 2;
			}
			else if(basetype[CurrentY * width + CurrentX - 2] == oldType){
				basetype[CurrentY * width + CurrentX - 2] = newType;
				basetype[CurrentY * width + CurrentX - 1] = newType;
				NextX = CurrentX - 2;
				NextY = CurrentY;
			}
			else if(basetype[CurrentY * width + CurrentX + 2] == oldType){
				basetype[CurrentY * width + CurrentX + 2] = newType;
				basetype[CurrentY * width + CurrentX + 1] = newType;
				NextX = CurrentX + 2;
				NextY = CurrentY;
			}
			else{
				pathDone = true;
			}
		}



		if(RanDirection == 2){//d, l, r, u
			if(basetype[(CurrentY + 2) * width + CurrentX] == oldType){
				basetype[(CurrentY + 2) * width + CurrentX] = newType;
				basetype[(CurrentY + 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY + 2;
			}
			else if(basetype[CurrentY * width + CurrentX - 2] == oldType){
				basetype[CurrentY * width + CurrentX - 2] = newType;
				basetype[CurrentY * width + CurrentX - 1] = newType;
				NextX = CurrentX - 2;
				NextY = CurrentY;
			}
			else if(basetype[CurrentY * width + CurrentX + 2] == oldType){
				basetype[CurrentY * width + CurrentX + 2] = newType;
				basetype[CurrentY * width + CurrentX + 1] = newType;
				NextX = CurrentX + 2;
				NextY = CurrentY;
			}
			else if(basetype[(CurrentY - 2) * width + CurrentX] == oldType){
				basetype[(CurrentY - 2) * width + CurrentX] = newType;
				basetype[(CurrentY - 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY - 2;
			}
			else{
				pathDone = true;
			}
		}

		if(RanDirection == 3){// l, r, u, d
			if(basetype[CurrentY * width + CurrentX - 2] == oldType){
				basetype[CurrentY * width + CurrentX - 2] = newType;
				basetype[CurrentY * width + CurrentX - 1] = newType;
				NextX = CurrentX - 2;
				NextY = CurrentY;
			}
			else if(basetype[CurrentY * width + CurrentX + 2] == oldType){
				basetype[CurrentY * width + CurrentX + 2] = newType;
				basetype[CurrentY * width + CurrentX + 1] = newType;
				NextX = CurrentX + 2;
				NextY = CurrentY;
			}
			else if(basetype[(CurrentY - 2) * width + CurrentX] == oldType){
				basetype[(CurrentY - 2) * width + CurrentX] = newType;
				basetype[(CurrentY - 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY - 2;
			}
			else if(basetype[(CurrentY + 2) * width + CurrentX] == oldType){
				basetype[(CurrentY + 2) * width + CurrentX] = newType;
				basetype[(CurrentY + 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY + 2;
			}
			else{
				pathDone = true;
			}
		}



		if(RanDirection == 4){// r, u, d, l
			if(basetype[CurrentY * width + CurrentX + 2] == oldType){
				basetype[CurrentY * width + CurrentX + 2] = newType;
				basetype[CurrentY * width + CurrentX + 1] = newType;
				NextX = CurrentX + 2;
				NextY = CurrentY;
			}
			else if(basetype[(CurrentY - 2) * width + CurrentX] == oldType){
				basetype[(CurrentY - 2) * width + CurrentX] = newType;
				basetype[(CurrentY - 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY - 2;
			}
			else if(basetype[(CurrentY + 2) * width + CurrentX] == oldType){
				basetype[(CurrentY + 2) * width + CurrentX] = newType;
				basetype[(CurrentY + 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY + 2;
			}
			else if(basetype[CurrentY * width + CurrentX - 2] == oldType){
				basetype[CurrentY * width + CurrentX - 2] = newType;
				basetype[CurrentY * width + CurrentX - 1] = newType;
				NextX = CurrentX - 2;
				NextY = CurrentY;
			}
			else{
				pathDone = true;
			}
		}


		CurrentX =  NextX;
		CurrentY = NextY;

		if(pathDone == false){
 			TempPosition++;
			checked++;
		}


	}

	bool mazeDone = false;
	int mazePathsCreated = 0;
	while(mazeDone == false){
		mazePathsCreated = 0;
		for(int i = TempPosition; i >= 0; i--){
			int MyTempArrayPosition = PathCalculationArray[i].PositionY * width + PathCalculationArray[i].PositionX;
			if(basetype[MyTempArrayPosition - (width*2)] == oldType || basetype[MyTempArrayPosition + (width*2)] == oldType || basetype[MyTempArrayPosition + 2] == oldType || basetype[MyTempArrayPosition -2] == oldType){
				//basetype[MyTempArrayPosition - (width*2)] = newType;
				//basetype[MyTempArrayPosition - (width)] = newType;
				SpreadRandomPath(oldType, newType, i);
				mazePathsCreated++;
			}
		}
		if(mazePathsCreated < 1){
			mazeDone = true;
		}
		
	}

	SpreadRandomPath(oldType, newType, TempPosition);

}

void WorldGrid::SpreadRandomPath(int oldType, int newType, int ArrayStartPosition){

	bool pathDone = false;
	int CurrentY = PathCalculationArray[ArrayStartPosition].PositionY;
	int CurrentX = PathCalculationArray[ArrayStartPosition].PositionX;
	int NextX = 0;
	int NextY = 0;

	while(pathDone == false){

		basetype[(CurrentY) * width + CurrentX] = newType;

		PathCalculationArray[TempPosition].Real = true;
		PathCalculationArray[TempPosition].PositionX = CurrentX;
		PathCalculationArray[TempPosition].PositionY = CurrentY;
		PathCalculationArray[TempPosition].ParentArrayPosition = TempPosition-1;

		int RanDirection = RanInt(0,4) + 1;
		
		if(RanDirection == 1){//up first,d , l, r
			if(basetype[(CurrentY - 2) * width + CurrentX] == oldType){
				basetype[(CurrentY - 2) * width + CurrentX] = newType;
				basetype[(CurrentY - 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY - 2;
			}
			else if(basetype[(CurrentY + 2) * width + CurrentX] == oldType){
				basetype[(CurrentY + 2) * width + CurrentX] = newType;
				basetype[(CurrentY + 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY + 2;
			}
			else if(basetype[CurrentY * width + CurrentX - 2] == oldType){
				basetype[CurrentY * width + CurrentX - 2] = newType;
				basetype[CurrentY * width + CurrentX - 1] = newType;
				NextX = CurrentX - 2;
				NextY = CurrentY;
			}
			else if(basetype[CurrentY * width + CurrentX + 2] == oldType){
				basetype[CurrentY * width + CurrentX + 2] = newType;
				basetype[CurrentY * width + CurrentX + 1] = newType;
				NextX = CurrentX + 2;
				NextY = CurrentY;
			}
			else{
				pathDone = true;
			}
		}



		if(RanDirection == 2){//d, l, r, u
			if(basetype[(CurrentY + 2) * width + CurrentX] == oldType){
				basetype[(CurrentY + 2) * width + CurrentX] = newType;
				basetype[(CurrentY + 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY + 2;
			}
			else if(basetype[CurrentY * width + CurrentX - 2] == oldType){
				basetype[CurrentY * width + CurrentX - 2] = newType;
				basetype[CurrentY * width + CurrentX - 1] = newType;
				NextX = CurrentX - 2;
				NextY = CurrentY;
			}
			else if(basetype[CurrentY * width + CurrentX + 2] == oldType){
				basetype[CurrentY * width + CurrentX + 2] = newType;
				basetype[CurrentY * width + CurrentX + 1] = newType;
				NextX = CurrentX + 2;
				NextY = CurrentY;
			}
			else if(basetype[(CurrentY - 2) * width + CurrentX] == oldType){
				basetype[(CurrentY - 2) * width + CurrentX] = newType;
				basetype[(CurrentY - 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY - 2;
			}
			else{
				pathDone = true;
			}
		}

		if(RanDirection == 3){// l, r, u, d
			if(basetype[CurrentY * width + CurrentX - 2] == oldType){
				basetype[CurrentY * width + CurrentX - 2] = newType;
				basetype[CurrentY * width + CurrentX - 1] = newType;
				NextX = CurrentX - 2;
				NextY = CurrentY;
			}
			else if(basetype[CurrentY * width + CurrentX + 2] == oldType){
				basetype[CurrentY * width + CurrentX + 2] = newType;
				basetype[CurrentY * width + CurrentX + 1] = newType;
				NextX = CurrentX + 2;
				NextY = CurrentY;
			}
			else if(basetype[(CurrentY - 2) * width + CurrentX] == oldType){
				basetype[(CurrentY - 2) * width + CurrentX] = newType;
				basetype[(CurrentY - 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY - 2;
			}
			else if(basetype[(CurrentY + 2) * width + CurrentX] == oldType){
				basetype[(CurrentY + 2) * width + CurrentX] = newType;
				basetype[(CurrentY + 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY + 2;
			}
			else{
				pathDone = true;
			}
		}



		if(RanDirection == 4){// r, u, d, l
			if(basetype[CurrentY * width + CurrentX + 2] == oldType){
				basetype[CurrentY * width + CurrentX + 2] = newType;
				basetype[CurrentY * width + CurrentX + 1] = newType;
				NextX = CurrentX + 2;
				NextY = CurrentY;
			}
			else if(basetype[(CurrentY - 2) * width + CurrentX] == oldType){
				basetype[(CurrentY - 2) * width + CurrentX] = newType;
				basetype[(CurrentY - 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY - 2;
			}
			else if(basetype[(CurrentY + 2) * width + CurrentX] == oldType){
				basetype[(CurrentY + 2) * width + CurrentX] = newType;
				basetype[(CurrentY + 1) * width + CurrentX] = newType;
				NextX = CurrentX;
				NextY = CurrentY + 2;
			}
			else if(basetype[CurrentY * width + CurrentX - 2] == oldType){
				basetype[CurrentY * width + CurrentX - 2] = newType;
				basetype[CurrentY * width + CurrentX - 1] = newType;
				NextX = CurrentX - 2;
				NextY = CurrentY;
			}
			else{
				pathDone = true;
			}
		}


		CurrentX =  NextX;
		CurrentY = NextY;

		if(pathDone == false){
 			TempPosition++;
		}


	}






}

void WorldGrid::StartCellSimulation(int hollowType, int SolidType, int Checktype){

	for(int i = 0; i < width*height; i++){
		if(basetype[i] == Checktype){
			int Chance = RanInt(0,100);
			if(Chance > 45){
				basetype[i] = SolidType;
			}
			else{
				basetype[i] = hollowType;
			}

		}
	}

}

void WorldGrid::SimulationStep(int hollowType, int SolidType){
	int DeathLimit = 4;
	int BirthLimit = 4;

	for(int i = 0; i < width*height; i++){
		if(basetype[i] != hollowType && basetype[i] != SolidType){
			PathCalculationArray[i].ParentArrayPosition = basetype[i];
		}
		else{
			int SolidNeighbours = 0;
			if(basetype[i + 1] == SolidType){
				SolidNeighbours++;
			}
			if(basetype[i - 1] == SolidType){
				SolidNeighbours++;
			}
			if(basetype[i + 1 + width] == SolidType){
				SolidNeighbours++;
			}
			if(basetype[i + 1 - width] == SolidType){
				SolidNeighbours++;
			}
			if(basetype[i - 1 - width] == SolidType){
				SolidNeighbours++;
			}
			if(basetype[i - 1 + width] == SolidType){
				SolidNeighbours++;
			}
			if(basetype[i - width] == SolidType){
				SolidNeighbours++;
			}
			if(basetype[i + width] == SolidType){
				SolidNeighbours++;
			}


			if(basetype[i] == hollowType && SolidNeighbours <= BirthLimit){
				PathCalculationArray[i].ParentArrayPosition = SolidType;
			}
			else{
				PathCalculationArray[i].ParentArrayPosition = hollowType;
			}
			if(basetype[i] == SolidType && SolidNeighbours >= DeathLimit){
				PathCalculationArray[i].ParentArrayPosition = hollowType;
			}
			else{
				PathCalculationArray[i].ParentArrayPosition = SolidType;
			}

		}
	}

	for(int i = 0; i < width*height; i++){
		basetype[i] = PathCalculationArray[i].ParentArrayPosition;
		PathCalculationArray[i].ParentArrayPosition = 0;
	}
}

void WorldGrid::FloodSimulation(int hollowType, int SolidType){
	int LargestFlood = 0;
	for(int x = 0; x < width*height; x++){
		PathChecked[x] = 0;
		PathCalculationArray[x].Real = false;
	}

	for(int i = 0; i < width*height; i++){
		if(basetype[i] == hollowType && PathChecked[i] == 0){
			int FloodNumber = 0;
			int FloodPosition = 0;

			PathCalculationArray[FloodNumber].Real = true;
			PathCalculationArray[FloodNumber].ParentArrayPosition = i;
			PathChecked[i] = 1;
			FloodNumber++;

			for(int f = 0; f < FloodNumber; f++){
				if(PathCalculationArray[f].Real == true){
					int TempFloodPosition = PathCalculationArray[f].ParentArrayPosition - width;
					//up
					if(basetype[TempFloodPosition] == hollowType && PathChecked[TempFloodPosition] == 0){
						PathCalculationArray[FloodNumber].ParentArrayPosition = TempFloodPosition;
						PathCalculationArray[FloodNumber].Real = true;
						PathChecked[TempFloodPosition] = 1;
						FloodNumber++;
					}
					//down
					TempFloodPosition = PathCalculationArray[f].ParentArrayPosition + width;
					if(basetype[TempFloodPosition] == hollowType && PathChecked[TempFloodPosition] == 0){
						PathCalculationArray[FloodNumber].ParentArrayPosition = TempFloodPosition;
						PathCalculationArray[FloodNumber].Real = true;
						PathChecked[TempFloodPosition] = 1;
						FloodNumber++;
					}
					//left
					TempFloodPosition = PathCalculationArray[f].ParentArrayPosition -1;
					if(basetype[TempFloodPosition] == hollowType && PathChecked[TempFloodPosition] == 0){
						PathCalculationArray[FloodNumber].ParentArrayPosition = TempFloodPosition;
						PathCalculationArray[FloodNumber].Real = true;
						PathChecked[TempFloodPosition] = 1;
						FloodNumber++;
					}
					//right
					TempFloodPosition = PathCalculationArray[f].ParentArrayPosition +1;
					if(basetype[TempFloodPosition] == hollowType && PathChecked[TempFloodPosition] == 0){
						PathCalculationArray[FloodNumber].ParentArrayPosition = TempFloodPosition;
						PathCalculationArray[FloodNumber].Real = true;
						PathChecked[TempFloodPosition] = 1;
						FloodNumber++;
					}
				}
			}


			for(int f = 0; f < FloodNumber; f++){
				PathCalculationArray[PathCalculationArray[f].ParentArrayPosition].PositionY = FloodNumber;
				PathCalculationArray[f].Real = false;
			}

			if(FloodNumber > LargestFlood){
				LargestFlood = FloodNumber;
			}
			FloodNumber = 0;


		}
		
	}


	for(int i = 0; i < width*height; i++){
		if(PathCalculationArray[i].PositionY < LargestFlood && basetype[i] == hollowType){
			basetype[i] = SolidType;
		}
	}
}

void WorldGrid::GenerateRooms(int oldType, int newType, int MaxRoomNumber, int MaxAttempts, int MaxSize){

	int RoomsDone = 0;
	int Attempts = 0;

	while(RoomsDone < MaxRoomNumber && Attempts < MaxAttempts){

		int TempMyX = 0;
		int TempMyY = 0;

		while(basetype[TempMyY*width + TempMyX] != oldType){
			TempMyX = RanInt(0, width);
			TempMyY = RanInt(0, height);
		}
		//basetype[TempMyY*width + TempMyX] = newType;
		//MaxRoomNumber++;

		int RoomWidth = RanInt(3, MaxSize);
		int RoomHeight = RanInt(3, MaxSize);
		bool RoomValid = true;

		for(int w = 0; w < RoomWidth + 2; w++){
			for(int h = 0; h < RoomHeight + 2; h++){
				int TempPosition = 0;

				TempPosition = ((TempMyY*width + TempMyX) + w + (h*width)) - RoomWidth/2 - (RoomHeight/2)*width - width - 1;
				if(basetype[TempPosition] != oldType){
					RoomValid = false;
				}

			}
		}

		if(RoomValid == true){
			for(int w = 0; w < RoomWidth; w++){
				for(int h = 0; h < RoomHeight; h++){
					int TempPosition = 0;
					TempPosition = ((TempMyY*width + TempMyX) + w + (h*width)) - RoomWidth/2 - (RoomHeight/2)*width;
					basetype[TempPosition] = newType;
				}
			}
			MaxRoomNumber++;
		}


		Attempts++;
	}

}

void WorldGrid::RemoveDeadEnds(int oldType, int newType){

	int DeadEndsRemoved = 1;
	bool Done = false;

	while(Done == false){
		DeadEndsRemoved = 0;

		for(int i = 0; i < width*height; i++){
			if(basetype[i] == oldType){
				int SolidNeighbours = 0;
				if(basetype[i + 1] == oldType){
					SolidNeighbours++;
				}
				if(basetype[i - 1] == oldType){
					SolidNeighbours++;
				}
				if(basetype[i - width] == oldType){
					SolidNeighbours++;
				}
				if(basetype[i + width] == oldType){
					SolidNeighbours++;
				}

				if(SolidNeighbours == 1){
					basetype[i] = newType;
					DeadEndsRemoved++;
				}

			}
		}


		if(DeadEndsRemoved == 0){
			Done = true;
		}


	}

}

void WorldGrid::SetConstantBuffers(ID3D11Device* device){
	for(int e = 0; e < CurrentEnemyNumber; e++){
		EnemyArray[e]->SetConstantBuffer(device);
	}

	//constant world matrix buffer

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT d3dResult = device->CreateBuffer( &constDesc, 0, &worldCB_ );
}
