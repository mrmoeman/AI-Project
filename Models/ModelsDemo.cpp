/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    3D Models Demo - Demonstrates loading a model from an .OBJ file.
*/


#include "ModelsDemo.h"
#include <xnamath.h>
#include <stdio.h>
#include "objLoader.h"
#include "Bullet.h"

//char* MODEL_NAME = "building smaller scale.obj";
//char* MODEL_NAME = "138k.obj";
char* TEXTURE_NAME = "Image/Concrete_texture.jpg";
char* MODEL_NAME = "Obj/tank.obj";
int numberOfTanks =10;
MovingGameObject tank[10];


//////////////////////////
WorldGrid* MyGrid = NULL;
//////////////////////////


// ZS-->09/02/2015 -> tank example
TextureClass* tank_texture = NULL;
ModelClass* tank_model = NULL;
DisplayObject* tank_display_object = NULL;

// ZS-->09/02/2015 -> moon example
TextureClass* moon_texture = NULL;
ModelClass* moon_model = NULL;
DisplayObject* moon_display_object = NULL;
GameObject* moon_game_object = NULL;

//Bullet testing variable ALEX OWEN 26/01/15
Bullet bullet;

struct VertexPos
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
    XMFLOAT3 norm;
	XMFLOAT3 tang;
	XMFLOAT3 bitang;
};

struct TextVertexPos
{
    XMFLOAT3 pos;
    XMFLOAT2 tex0;
};

ModelsDemo::ModelsDemo( ) : textureMapVS_( 0 ), textureMapPS_( 0 ),textTextureMapVS_( 0 ), textTextureMapPS_( 0 ),
							inputLayout_( 0 ), textInputLayout_(0),textVertexBuffer_(0),textColorMapSampler_( 0 ),
                            vertexBuffer_( 0 ), colorMap_( 0 ), textColorMap_(0), colorMapSampler_( 0 ),
                            viewCB_( 0 ), projCB_( 0 ), worldCB_( 0 ), camPosCB_( 0 ), totalVerts_( 0 ),
							gameState_(START_MENU), pauseMenuSelection(RETURN), displayFPS(false)
{
    ZeroMemory( &controller1State_, sizeof( XINPUT_STATE ) );
    ZeroMemory( &prevController1State_, sizeof( XINPUT_STATE ) );
	ZeroMemory(&mouseCurrState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mousePrevState, sizeof(DIMOUSESTATE));

	m_alphaEnableBlendingState = 0;
	m_alphaDisableBlendingState = 0;
	m_depthStencilState = 0;
	m_depthDisabledStencilState = 0;
	frameTime_=0;
	fps_=0;

	xpos = 0;
	ypos = 0;
	fullsize = false;
}

ModelsDemo::~ModelsDemo( )
{

}

bool ModelsDemo::LoadContent( )
{

	//////////////////////////////GRID STUFF///////////////////////
	MyGrid = new WorldGrid(120,120);
	//MyGrid->InitSnow();
	//MyGrid->BuildBuffer(d3dDevice_, 26.0f, 20.0f);	//26 20



    ID3DBlob* vsBuffer = 0;
	////////shaders start
    bool compileResult = CompileD3DShader( "TextureMap.fx", "VS_Main", "vs_4_0", &vsBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling the vertex shader!" );
        return false;
    }

    HRESULT d3dResult;

    d3dResult = d3dDevice_->CreateVertexShader( vsBuffer->GetBufferPointer( ),
        vsBuffer->GetBufferSize( ), 0, &textureMapVS_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the vertex shader!" );

        if( vsBuffer )
            vsBuffer->Release( );

        return false;
    }

    D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    unsigned int totalLayoutElements = ARRAYSIZE( solidColorLayout );

    d3dResult = d3dDevice_->CreateInputLayout( solidColorLayout, totalLayoutElements,
        vsBuffer->GetBufferPointer( ), vsBuffer->GetBufferSize( ), &inputLayout_ );

    //vsBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the input layout!" );
        return false;
    }

    ID3DBlob* psBuffer = 0;

    compileResult = CompileD3DShader( "TextureMap.fx", "PS_Main", "ps_4_0", &psBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling pixel shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreatePixelShader( psBuffer->GetBufferPointer( ),
        psBuffer->GetBufferSize( ), 0, &textureMapPS_ );

    //psBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating pixel shader!" );
        return false;
    }

	//////////////////////////////////////////////////////////////////normal shaders
	////////shaders start
    compileResult = CompileD3DShader( "NormalMap.fx", "VS_Main", "vs_4_0", &vsBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling the vertex shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreateVertexShader( vsBuffer->GetBufferPointer( ),
        vsBuffer->GetBufferSize( ), 0, &normalTextureMapVS_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the vertex shader!" );

        if( vsBuffer )
            vsBuffer->Release( );

        return false;
    }


    d3dResult = d3dDevice_->CreateInputLayout( solidColorLayout, totalLayoutElements,
        vsBuffer->GetBufferPointer( ), vsBuffer->GetBufferSize( ), &inputLayout_ );

    //vsBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the input layout!" );
        return false;
    }

    compileResult = CompileD3DShader( "NormalMap.fx", "PS_Main", "ps_4_0", &psBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling pixel shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreatePixelShader( psBuffer->GetBufferPointer( ),
        psBuffer->GetBufferSize( ), 0, &normalTextureMapPS_ );

    //psBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating pixel shader!" );
        return false;
    }




////////////////////////////////////////////text 2d shaders////////////////////////////////////////////////////////////

	//ID3DBlob* vsBuffer = 0;
	vsBuffer = 0;

    compileResult = CompileD3DShader( "TextTextureMap.fx", "VS_Main", "vs_4_0", &vsBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling the vertex shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreateVertexShader( vsBuffer->GetBufferPointer( ),
        vsBuffer->GetBufferSize( ), 0, &textTextureMapVS_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the vertex shader!" );

        if( vsBuffer )
            vsBuffer->Release( );

        return false;
    }

    D3D11_INPUT_ELEMENT_DESC textSolidColorLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    totalLayoutElements = ARRAYSIZE( textSolidColorLayout );

    d3dResult = d3dDevice_->CreateInputLayout( textSolidColorLayout, totalLayoutElements,
        vsBuffer->GetBufferPointer( ), vsBuffer->GetBufferSize( ), &textInputLayout_ );

    vsBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating the input layout!" );
        return false;
    }

    //ID3DBlob* psBuffer = 0;
	psBuffer = 0;

    compileResult = CompileD3DShader( "TextTextureMap.fx", "PS_Main", "ps_4_0", &psBuffer );

    if( compileResult == false )
    {
        DXTRACE_MSG( "Error compiling pixel shader!" );
        return false;
    }

    d3dResult = d3dDevice_->CreatePixelShader( psBuffer->GetBufferPointer( ),
        psBuffer->GetBufferSize( ), 0, &textTextureMapPS_ );

    psBuffer->Release( );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Error creating pixel shader!" );
        return false;
    }
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////texture
	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
	"fontEX.png", 0, 0, &textColorMap_, 0 );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to load the texture image!" );
        return false;
    }
	///////////////////
	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
	"tilemaptemplate.png", 0, 0, &GridColorMap_, 0 );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to load the texture image!" );
        return false;
    }


	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
	"tilemaplava.png", 0, 0, &GridColorMapLava_, 0 );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to load the texture image!" );
        return false;
    }


	d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
	"tilemaptemple.png", 0, 0, &GridColorMapTemple_, 0 );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to load the texture image!" );
        return false;
    }
	///////////////////

	D3D11_SAMPLER_DESC textColorMapDesc;
    ZeroMemory( &textColorMapDesc, sizeof( textColorMapDesc ) );
    textColorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    textColorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    textColorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    textColorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    textColorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    textColorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

    d3dResult = d3dDevice_->CreateSamplerState( &textColorMapDesc, &textColorMapSampler_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create color map sampler state!" );
        return false;
    }

    D3D11_BUFFER_DESC textVertexDesc;
    ZeroMemory( &textVertexDesc, sizeof( textVertexDesc ) );
    textVertexDesc.Usage = D3D11_USAGE_DYNAMIC;
    textVertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    textVertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    const int sizeOfSprite = sizeof( TextVertexPos ) * 6;
    const int maxLetters = 24;

    textVertexDesc.ByteWidth = sizeOfSprite * maxLetters;

    d3dResult = d3dDevice_->CreateBuffer( &textVertexDesc, 0, &textVertexBuffer_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create vertex buffer!" );
        return false;
    }

	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    

    d3dResult = D3DX11CreateShaderResourceViewFromFile( d3dDevice_,
        "Image/decal.dds", 0, 0, &colorMap_, 0 );
		//"41.jpg", 0, 0, &colorMap_, 0 );
		//TEXTURE_NAME, 0, 0, &colorMap_, 0 );



    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to load the texture image!" );
        return false;
    }



    D3D11_SAMPLER_DESC colorMapDesc;
    ZeroMemory( &colorMapDesc, sizeof( colorMapDesc ) );
    colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    //colorMapDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

    d3dResult = d3dDevice_->CreateSamplerState( &colorMapDesc, &colorMapSampler_ );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to create color map sampler state!" );
        return false;
    }


    D3D11_BUFFER_DESC constDesc;
	ZeroMemory( &constDesc, sizeof( constDesc ) );
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof( XMMATRIX );
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &viewCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &projCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &worldCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }

	constDesc.ByteWidth = sizeof( XMFLOAT4 );

    d3dResult = d3dDevice_->CreateBuffer( &constDesc, 0, &camPosCB_ );

	if( FAILED( d3dResult ) )
    {
        return false;
    }
	///////////////////////////////////////////////////////////////////////////////////////////////////////
/*		D3D11_BLEND_DESC blendDesc2;
    ZeroMemory( &blendDesc2, sizeof( blendDesc2 ) );
    blendDesc2.RenderTarget[0].BlendEnable = TRUE;
	
	blendDesc2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc2.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    d3dDevice_->CreateBlendState( &blendDesc2, &alphaBlendState_ );
	//d3dDevice_->CreateBlendState( &BlendState, &alphaBlendState_ );
    d3dContext_->OMSetBlendState( alphaBlendState_, blendFactor, 0xFFFFFFFF );*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	d3dResult = d3dDevice_->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if(FAILED(d3dResult))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	d3dResult = d3dDevice_->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if(FAILED(d3dResult))
	{
		return false;
	}

    projMatrix_ = XMMatrixPerspectiveFovLH( XM_PIDIV4, 800.0f / 600.0f, 0.01f, 10000.0f );
    projMatrix_ = XMMatrixTranspose( projMatrix_ );

    camera_.SetDistance( 12.0f, 4.0f, 20.0f );

    return true;
}

void ModelsDemo::UnloadContent( )
{
    if( colorMapSampler_ ) colorMapSampler_->Release( );
	if( textColorMapSampler_ ) textColorMapSampler_->Release( );
    if( colorMap_ ) colorMap_->Release( );
	if( textColorMap_ ) textColorMap_->Release( );
    if( textureMapVS_ ) textureMapVS_->Release( );
    if( textTextureMapPS_ ) textTextureMapPS_->Release( );
    if( textTextureMapVS_ ) textTextureMapVS_->Release( );
    if( textureMapPS_ ) textureMapPS_->Release( );
    if( inputLayout_ ) inputLayout_->Release( );
	if( textInputLayout_ ) textInputLayout_->Release( );
	
    if( vertexBuffer_ ) vertexBuffer_->Release( );
	if( textVertexBuffer_ ) textVertexBuffer_->Release( );
    if( viewCB_ ) viewCB_->Release( );
    if( projCB_ ) projCB_->Release( );
    if( worldCB_ ) worldCB_->Release( );
	if( camPosCB_ ) camPosCB_->Release( );

	if(m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if(m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}


    colorMapSampler_ = 0;
	textColorMapSampler_ = 0;
    colorMap_ = 0;
	textColorMap_ = 0;
    textureMapVS_ = 0;
    textureMapPS_ = 0;
	textTextureMapVS_ = 0;
    textTextureMapPS_ = 0;
    inputLayout_ = 0;
	textInputLayout_ = 0;
	//changed vertexBuffer_ = 0; 
    vertexBuffer_ = 0;
	textVertexBuffer_ = 0;
    viewCB_ = 0;
    projCB_ = 0;
    worldCB_ = 0;

	MyGrid->unloadAll();
	delete MyGrid;
	//MyGrid = NULL;

	if ( tank_model ) { tank_model->Release(); tank_model = NULL; }
	if ( tank_texture ) { tank_texture->Release(); tank_texture = NULL; }
	if ( tank_display_object ) { delete tank_display_object; tank_display_object = NULL; }
}

void ModelsDemo::TurnOnAlphaBlending()
{
	float blendFactor[4];
	

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn on the alpha blending.
	d3dContext_->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

void ModelsDemo::TurnOffAlphaBlending()
{
	float blendFactor[4];
	

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn off the alpha blending.
	d3dContext_->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void ModelsDemo::TurnZBufferOn()
{
	d3dContext_->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}

void ModelsDemo::TurnZBufferOff()
{
	d3dContext_->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

void ModelsDemo::SetGameState(GameStates state)
{
	gameState_ = state;
}
GameStates ModelsDemo::GetGameState()
{
	return gameState_;
}

void ModelsDemo::Update( float dt )
{


	if(gameState_ == RUN)
		{

			
		}

	float moveSpeed=0.0001f;
	float moveSpeed2=5.0f;
	float zoom = 0.0;
	float xRotation=0.0;
	float yRotation=0.0;

	//moveSpeed=0.001f;
	float moveLeftRight =0.0;
	float moveBackForward =0.0;

	if(gameState_ == START_MENU)
	{

		if(!(keystate[DIK_SPACE] & 0x80)&&(keyPrevState[DIK_SPACE] & 0x80))
		{
			gameState_ = RUN;
			MyGrid = new WorldGrid(120,120);
			if(MyGrid->GetWorldType() == 1){
				MyGrid->InitSnow();
			}
			if(MyGrid->GetWorldType() == 2){
				MyGrid->InitLava();
			}
			if(MyGrid->GetWorldType() == 3){
				MyGrid->InitSnowMaze();
			}
			if(MyGrid->GetWorldType() == 4){
				MyGrid->InitTemple();
			}
			if(MyGrid->GetWorldType() == 5){
				MyGrid->InitTemplelabyrinth();
			}
			MyGrid->BuildBuffer(d3dDevice_, 25.0f, 20.0f);	//26 20
			MyGrid->SetConstantBuffers(d3dDevice_);
		}

		if(!(keystate[DIK_ESCAPE] & 0x80)&&(keyPrevState[DIK_ESCAPE] & 0x80))
		{
			PostQuitMessage(0);
		}


	}

	if(gameState_ == PAUSED)
	{
		
		if(!(keystate[DIK_ESCAPE] & 0x80)&&(keyPrevState[DIK_ESCAPE] & 0x80))
		{
			//PostQuitMessage(0);
			gameState_ = RUN;
		}
		if((keystate[DIK_RETURN] & 0x80)&&(pauseMenuSelection == RETURN))
		{
			//PostQuitMessage(0);
			gameState_ = RUN;
		}
		if((keystate[DIK_RETURN] & 0x80)&&(pauseMenuSelection == PLAY_MOVIE))
		{
			//PostQuitMessage(0);
			gameState_ = INTRO_MOVIE_REPLAY;
		}
		
		if((keystate[DIK_RETURN] & 0x80)&&(pauseMenuSelection == QUIT))
		{
			gameState_ = START_MENU;
			//PostQuitMessage(0);
		}
		
		if((!(keystate[DIK_RETURN] & 0x80)&&(keyPrevState[DIK_RETURN] & 0x80))
			&&(pauseMenuSelection == FPS))
		{
			displayFPS = !displayFPS;
		}
		
		if(
			(!(keystate[DIK_DOWN] & 0x80)&&(keyPrevState[DIK_DOWN] & 0x80))
			||
			(!(keystate[DIK_S] & 0x80)&&(keyPrevState[DIK_S] & 0x80))
			)
		{
			pauseMenuSelection++;

		}
		if(
			(!(keystate[DIK_UP] & 0x80)&&(keyPrevState[DIK_UP] & 0x80))
			||
			(!(keystate[DIK_W] & 0x80)&&(keyPrevState[DIK_W] & 0x80))
			)

		{
			pauseMenuSelection--;
		}


	}

	if(gameState_ == RUN)
	{
		

		float MoveSpeedX = -(1.0f/20.0f);
		float MoveSpeedY = -(1.0f/25.0f);
		if(!(keystate[DIK_R] & 0x80)&&(keyPrevState[DIK_R] & 0x80))
		{
			MyGrid->GridPotion();
		}

		if((keystate[DIK_LEFT] & 0x80)||(keystate[DIK_A] & 0x80))
		{
			MyGrid->GridLeft(d3dDevice_);
			//xpos-=MoveSpeedX;
		}
		if((keystate[DIK_RIGHT] & 0x80)||(keystate[DIK_D] & 0x80))
		{
			MyGrid->GridRight(d3dDevice_);
			//xpos+=MoveSpeedX;
		}
	
		if((keystate[DIK_DOWN] & 0x80)||(keystate[DIK_S] & 0x80))
		{
			MyGrid->GridUp(d3dDevice_);
			//ypos-=MoveSpeedY;
		}
		if((keystate[DIK_UP] & 0x80)||(keystate[DIK_W] & 0x80))
		{
			MyGrid->GridDown(d3dDevice_);
			//ypos+=MoveSpeedY;
		}
		if((keystate[DIK_E] & 0x80))
		{
			MyGrid->GridAttack();
		}
		if(keystate[DIK_SUBTRACT] & 0x80)
		{
			if(fullsize == false){
				fullsize = true;
				MyGrid->SetFullsize(true, d3dDevice_, MyGrid->GetWidth(), MyGrid->GetHeight());
			}
		}
		if(keystate[DIK_ADD] & 0x80)
		{
			if(fullsize == true){
				fullsize = false;
				MyGrid->SetFullsize(false, d3dDevice_, 25.0f, 20.0f);
			}
		}

		if(keystate[DIK_P] & 0x80)
		{
			if(MyGrid->GetWorldType() == 1){
				MyGrid->InitSnow();
			}
			if(MyGrid->GetWorldType() == 2){
				MyGrid->InitLava();
			}
			if(MyGrid->GetWorldType() == 3){
				MyGrid->InitSnowMaze();
			}
			if(MyGrid->GetWorldType() == 4){
				MyGrid->InitTemple();
			}
			if(MyGrid->GetWorldType() == 5){
				MyGrid->InitTemplelabyrinth();
			}
			if(fullsize == false){
				MyGrid->BuildBuffer(d3dDevice_, 25.0f, 20.0f);	//26 20
			}
			if(fullsize == true){
				MyGrid->BuildBuffer(d3dDevice_, MyGrid->GetWidth(), MyGrid->GetHeight());
			}
			MyGrid->SetConstantBuffers(d3dDevice_);
		}

		if(keystate[DIK_Q] & 0x80)
		{
			MyGrid->GridInteract();
		}
	
		if(!(keystate[DIK_ESCAPE] & 0x80)&&(keyPrevState[DIK_ESCAPE] & 0x80))
		{
			//PostQuitMessage(0);
			gameState_ = PAUSED;
		}
	
	
	
		if((mouseCurrState.lX != mousePrevState.lX) || (mouseCurrState.lY != mousePrevState.lY))
		{
			yRotation += mousePrevState.lX * 0.005f;
			//yRotation += mousePrevState.lX * dt*2.0f;//mouse movement already based on time

			xRotation += mouseCurrState.lY * 0.005f;
			//xRotation += mouseCurrState.lY * dt*2.0f;/mouse movement already based on time
		
			if(xRotation>XM_PI/8)
			{
				xRotation=XM_PI/8;
			}

			if(xRotation<-(XM_PI/8))
			{
				xRotation=-(XM_PI/8);
			}

			mousePrevState = mouseCurrState;
		}


		camera_.Move(moveLeftRight, moveBackForward);

		camera_.ApplyZoom(zoom);
		camera_.ApplyRotation(xRotation,yRotation);

		MyGrid->GridUpdate(dt);
		MyGrid->RebuildBuffer(d3dDevice_);

		if(MyGrid->GetHealth() == 0){
			gameState_ = START_MENU;
		}

		if(MyGrid->GetReload() == true){
			if(MyGrid->GetWorldType() == 1){
				MyGrid->InitSnow();
			}
			if(MyGrid->GetWorldType() == 2){
				MyGrid->InitLava();
			}
			if(MyGrid->GetWorldType() == 3){
				MyGrid->InitSnowMaze();
			}
			if(MyGrid->GetWorldType() == 4){
				MyGrid->InitTemple();
			}
			if(MyGrid->GetWorldType() == 5){
				MyGrid->InitTemplelabyrinth();
			}
			if(fullsize == false){
				MyGrid->BuildBuffer(d3dDevice_, 25.0f, 20.0f);	//26 20
			}
			if(fullsize == true){
				MyGrid->BuildBuffer(d3dDevice_, MyGrid->GetWidth(), MyGrid->GetHeight());
			}
			MyGrid->SetConstantBuffers(d3dDevice_);
		}




	}

    memcpy( &prevController1State_, &controller1State_, sizeof( XINPUT_STATE ) );
}

bool ModelsDemo::DrawString( char* message, float startX, float startY )
{
	//float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    //d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );


    // Size in bytes for a single sprite.
    const int sizeOfSprite = sizeof( TextVertexPos ) * 6;

    // Demo's dynamic buffer setup for max of 24 letters.
    const int maxLetters = 24;

    int length = strlen( message );

    // Clamp for strings too long.
    if( length > maxLetters )
        length = maxLetters;

    // Char's width on screen.
    float charWidth = 32.0f / 800.0f;

    // Char's height on screen.
    float charHeight = 32.0f / 640.0f;
    
    // Char's texel width.
    //float texelWidth = 32.0f / 864.0f;
	float texelWidth = 32.0f / 3072.0f;

    // verts per-triangle (3) * total triangles (2) = 6.
    const int verticesPerLetter = 6;

    D3D11_MAPPED_SUBRESOURCE mapResource;
    HRESULT d3dResult = d3dContext_->Map( textVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to map resource!" );
        return false;
    }

    // Point to our vertex buffer's internal data.
    TextVertexPos *spritePtr = ( TextVertexPos* )mapResource.pData;

	const int indexSpace = static_cast<char>( ' ' );
    const int indexA = static_cast<char>( 'A' );
    const int indexZ = static_cast<char>( 'Z' );
	const int indexSquare = static_cast<char>( 127 );

    for( int i = 0; i < length; ++i )
    {
        float thisStartX = startX + ( charWidth * static_cast<float>( i ) );
        float thisEndX = thisStartX + charWidth;
        float thisEndY = startY + charHeight;

        spritePtr[0].pos = XMFLOAT3( thisEndX,   thisEndY, 1.0f );
        spritePtr[1].pos = XMFLOAT3( thisEndX,   startY,   1.0f );
        spritePtr[2].pos = XMFLOAT3( thisStartX, startY,   1.0f );
        spritePtr[3].pos = XMFLOAT3( thisStartX, startY,   1.0f );
        spritePtr[4].pos = XMFLOAT3( thisStartX, thisEndY, 1.0f );
        spritePtr[5].pos = XMFLOAT3( thisEndX,   thisEndY, 1.0f );

        int texLookup = 0;
        int letter = static_cast<char>( message[i] );

        //if( letter < indexA || letter > indexZ )
        if( letter < indexSpace || letter > indexSquare )
		{
            // Grab one index past Z, which is a blank space in the texture.
           // texLookup = ( indexZ - indexA ) + 1;
			texLookup = indexSquare;
        }
        else
        {
            // A = 0, B = 1, Z = 25, etc.
            //texLookup = ( letter - indexA );
			texLookup = ( letter - indexSpace ); 
        }

        float tuStart = 0.0f + ( texelWidth * static_cast<float>( texLookup ) );
        float tuEnd = tuStart + texelWidth;

        spritePtr[0].tex0 = XMFLOAT2( tuEnd, 0.0f );
        spritePtr[1].tex0 = XMFLOAT2( tuEnd, 1.0f );
        spritePtr[2].tex0 = XMFLOAT2( tuStart, 1.0f );
        spritePtr[3].tex0 = XMFLOAT2( tuStart, 1.0f );
        spritePtr[4].tex0 = XMFLOAT2( tuStart, 0.0f );
        spritePtr[5].tex0 = XMFLOAT2( tuEnd, 0.0f );

        spritePtr += 6;
    }

    d3dContext_->Unmap( textVertexBuffer_, 0 );
    d3dContext_->Draw( 6 * length, 0 );

    return true;
}

bool ModelsDemo::DrawGameObject( GameObject * game_object )
{
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	// THESE OPERATIONS ARE NOT ENCAPSULATED YET !!!

	// activate the vertex layout on the graphics card
	d3dContext_->IASetInputLayout( inputLayout_ );

	// activate the vertex shader on the graphics card
	d3dContext_->VSSetShader( textureMapVS_, 0, 0 );
	
	// activate the pixel shader on the graphics card
	d3dContext_->PSSetShader( textureMapPS_, 0, 0 );

	// activate the texture sampler on the graphics card
	d3dContext_->PSSetSamplers( 0, 1, &colorMapSampler_ );

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	// get the display object from the game object
	DisplayObject * display_object = game_object->GetDisplayObject();

	// activate the GameObject's vertex buffer on the graphics card
	display_object->GetModel()->Render( d3dContext_ );

	// activate the GameObject's texture buffer on the graphics card
	display_object->GetTexture()->Render( d3dContext_ );

	// create matrices to create a single world matrix for the GameObject's transform
	XMMATRIX scale_mat = XMMatrixScaling(game_object->getSX(), game_object->getSY(), game_object->getSZ());
	XMMATRIX rotation_mat = XMMatrixRotationRollPitchYaw(game_object->getRX(), game_object->getRY(), game_object->getRZ());
	XMMATRIX position_mat = XMMatrixTranslation(game_object->getX(), game_object->getY(), game_object->getZ());
	
	// 1) scale 
	// 2) rotate 
	// 3) position
	XMMATRIX world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	// THESE OPERATIONS ARE NOT ENCAPSULATED YET !!!

	// update the world matrix on the graphics card
	d3dContext_->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );

	// set the world matrix on the vertex shader
	d3dContext_->VSSetConstantBuffers( 0, 1, &worldCB_ );

	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	d3dContext_->Draw( display_object->GetModel()->GetVertexCount(), 0 );

	return true;
}

void ModelsDemo::Render( )
{
    if( d3dContext_ == 0 )
        return;


	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float redColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	if(MyGrid->GetWorldType() == 1 || MyGrid->GetWorldType() == 3){
		clearColor[0] = 1.0f;
		clearColor[1] = 0.98f;
		clearColor[2] = 0.98f;
	}
	if(MyGrid->GetWorldType() == 2){
		clearColor[0] =  0.007f;
		clearColor[1] = 0.18f;
		clearColor[2] = 0.37f;
	}
	if(MyGrid->GetWorldType() == 4  || MyGrid->GetWorldType() == 5){
		clearColor[0] =  0.10f;
		clearColor[1] = 0.42f;
		clearColor[2] = 0.63f;
	}




    d3dContext_->ClearRenderTargetView( renderTargetViewMap, redColor );										///////clears render target 
	d3dContext_->ClearRenderTargetView( backBufferTarget_, clearColor );				
    d3dContext_->ClearDepthStencilView( depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	d3dContext_->OMSetRenderTargets( 1, &renderTargetViewMap, depthStencilView_ );			///sets render target					NOT WORKING FOR SOME BLOODY REASON............
	
	unsigned int stride = sizeof( VertexPos );
	unsigned int offset = 0;
	
	

	if((gameState_ == RUN)||(gameState_==PAUSED))
	{
		//RenderPassOne();
		RenderPassThree();
	}

	

	RenderPassTwo();

    swapChain_->Present( 0, 0 );
}
//George Anderson
/////draws a rectangle to screen
bool ModelsDemo::DrawRect()
{



	//////////////////
    D3D11_MAPPED_SUBRESOURCE mapResource;
    HRESULT d3dResult = d3dContext_->Map( textVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource );

    if( FAILED( d3dResult ) )
    {
        DXTRACE_MSG( "Failed to map resource!" );
        return false;
    }

    // Point to our vertex buffer's internal data.
    TextVertexPos *spritePtr = ( TextVertexPos* )mapResource.pData;



        spritePtr[0].pos = XMFLOAT3( -1.0f,   1.0f, 1.0f );
        spritePtr[1].pos = XMFLOAT3( 1.0f,   1.0f,   1.0f );
        spritePtr[2].pos = XMFLOAT3( -1.0f, -1.0f,   1.0f );
        spritePtr[3].pos = XMFLOAT3( 1.0f, 1.0f,   1.0f );
        spritePtr[4].pos = XMFLOAT3( 1.0f, -1.0f, 1.0f );
        spritePtr[5].pos = XMFLOAT3( -1.0f,   -1.0f, 1.0f );
		

        spritePtr[0].tex0 = XMFLOAT2( 0.0f, 0.0f );
        spritePtr[1].tex0 = XMFLOAT2( 1.0f, 0.0f );
        spritePtr[2].tex0 = XMFLOAT2( 0.0f, 1.0f );
        spritePtr[3].tex0 = XMFLOAT2( 1.0f, 0.0f );
        spritePtr[4].tex0 = XMFLOAT2( 1.0f, 1.0f );
        spritePtr[5].tex0 = XMFLOAT2( 0.0f, 1.0f );
    

    d3dContext_->Unmap( textVertexBuffer_, 0 );
    d3dContext_->Draw( 6, 0 );

	

    return true;
}

/////Models
bool ModelsDemo::RenderPassOne(){

	unsigned int stride = sizeof( VertexPos );
	unsigned int offset = 0;

	//TurnZBufferOff();

		XMMATRIX worldMat = XMMatrixIdentity( );
		worldMat = XMMatrixTranspose( worldMat );

		XMMATRIX viewMat = camera_.GetViewMatrix( );
		viewMat = XMMatrixTranspose( viewMat );

		XMFLOAT3 cameraPos = camera_.GetPosition( );

		//d3dContext_->UpdateSubresource( worldCB_, 0, 0, &worldMat, 0, 0 );
		d3dContext_->UpdateSubresource( viewCB_, 0, 0, &viewMat, 0, 0 );
		d3dContext_->UpdateSubresource( projCB_, 0, 0, &projMatrix_, 0, 0 );
		d3dContext_->UpdateSubresource( camPosCB_, 0, 0, &cameraPos, 0, 0 );

		//d3dContext_->VSSetConstantBuffers( 0, 1, &worldCB_ );
		d3dContext_->VSSetConstantBuffers( 1, 1, &viewCB_ );
		d3dContext_->VSSetConstantBuffers( 2, 1, &projCB_ );
		d3dContext_->VSSetConstantBuffers( 3, 1, &camPosCB_ );

		d3dContext_->Draw( totalVerts_, 0 );

		/////////////////////////////////////////geometry settings//////////////////////////////

	// ZS-->09/02/2015 -> draw the tanks
		for(int i = 0; i < numberOfTanks; i++)
		{
			GameObject* game_object = &tank[i];
			DrawGameObject( game_object );
		}

		// ZS-->09/02/2015 -> draw bullet
		//GameObject* game_object = &bullet;
		//DrawGameObject( game_object );

		// ZS-->09/02/2015 -> draw moon
		DrawGameObject( moon_game_object );

		//MyGrid->Render(d3dContext_);
		/////NORMAL MAPPING

		
		//d3dContext_->UpdateSubresource( worldCB_, 0, 0, &worldMat, 0, 0 );
		d3dContext_->UpdateSubresource( viewCB_, 0, 0, &viewMat, 0, 0 );
		d3dContext_->UpdateSubresource( projCB_, 0, 0, &projMatrix_, 0, 0 );
		d3dContext_->UpdateSubresource( camPosCB_, 0, 0, &cameraPos, 0, 0 );

		//d3dContext_->VSSetConstantBuffers( 0, 1, &worldCB_ );
		d3dContext_->VSSetConstantBuffers( 1, 1, &viewCB_ );
		d3dContext_->VSSetConstantBuffers( 2, 1, &projCB_ );
		d3dContext_->VSSetConstantBuffers( 3, 1, &camPosCB_ );


		d3dContext_->VSSetShader( normalTextureMapVS_, 0, 0 );
		d3dContext_->PSSetShader( normalTextureMapPS_, 0, 0 );
		d3dContext_->PSSetSamplers( 0, 1, &colorMapSampler_ );
		////////////////////terrain////////////////////////////////
		worldMat = XMMatrixIdentity( );
		worldMat = XMMatrixTranspose( worldMat );
		d3dContext_->UpdateSubresource( worldCB_, 0, 0, &worldMat, 0, 0 );
		d3dContext_->VSSetConstantBuffers( 0, 1, &worldCB_ );
		d3dContext_->IASetVertexBuffers( 0, 1, &vertexBufferTerrain_, &stride, &offset );

		d3dContext_->Draw( 6, 0 );

		return true;
}
////Text
bool ModelsDemo::RenderPassTwo(){
	d3dContext_->OMSetRenderTargets( 1, &backBufferTarget_, NULL );
	unsigned int stride = sizeof( VertexPos );
	unsigned int offset = 0;


		// create matrices to create a single world matrix for the GameObject's transform
		XMMATRIX scale_mat = XMMatrixScaling(1,1,1);
		XMMATRIX rotation_mat = XMMatrixRotationRollPitchYaw(0,0,0);
		XMMATRIX position_mat = XMMatrixTranslation(0, 0, 0);
		XMMATRIX  world_mat = XMMatrixTranspose( scale_mat * rotation_mat * position_mat );
		d3dContext_->UpdateSubresource( worldCB_, 0, 0, &world_mat, 0, 0 );
		d3dContext_->VSSetConstantBuffers( 0, 1, &worldCB_ );





		TurnZBufferOff();
		TurnOnAlphaBlending();
	
		stride = sizeof( TextVertexPos );
		offset = 0;

		d3dContext_->IASetInputLayout( textInputLayout_ );
		d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );
		d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		d3dContext_->VSSetShader( textTextureMapVS_, 0, 0 );
		d3dContext_->PSSetShader( textTextureMapPS_, 0, 0 );


		d3dContext_->PSSetShaderResources( 0, 1, &textColorMap_ );
		d3dContext_->PSSetSamplers( 0, 1, &textColorMapSampler_ );

		////////////////////////////////////////////////////////
		//MyGrid->Render(d3dContext_);

		d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );///////
		if (gameState_ == START_MENU)
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////
		DrawString( "PRESS SPACE to START", -0.4f, 0.0f );
		DrawString( "PRESS ESC to QUIT", -0.35f, -0.1f );
		////////////////////////////////////////////////////////////////////////////////////////////////
	}
	char output[24];
	if(gameState_ == RUN){
		sprintf_s(output, "Floor:%d",MyGrid->GetFloorNumber());
		DrawString( output, -0.1f, 0.8f);

		sprintf_s(output, "%d/%d",MyGrid->GetHealth(), MyGrid->GetMaxHealth());
		DrawString( output, -0.1f, 0.9f);

		sprintf_s(output, "%d",MyGrid->GetPotions());
		DrawString( output, -0.2f, 0.8f);

		sprintf_s(output, "%d",MyGrid->GetStrength());
		DrawString( output, -0.3f, 0.8f);

		sprintf_s(output, "%d",MyGrid->GetDefense());
		DrawString( output, -0.4f, 0.8f);
	}

	if((gameState_ == RUN)&&(displayFPS==true))
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		
		//sprintf_s(output, "FPS:%d",fps_);
		sprintf_s(output, "FPS:%d",fps_);

		DrawString( output, -0.9f, 0.83f);

		//sprintf_s(output, "Frame Time:%d", MyGrid->GetHeight());

		//DrawString( output, -0.9f, 0.6f);
		////////////////////////////////////////////////////////////////////////////////////////////////
	}




	if (gameState_ == PAUSED)
	{
		/////////////////////////////////////////////TEXT//////////////////////////////////////////////

		DrawString( "GAME PAUSED", -0.2f, 0.0f );
		if (pauseMenuSelection == RETURN)
		{
			DrawString( "->Return to Game<-", -0.33f, -0.1f );
		}
		else
		{
			DrawString( "Return to Game", -0.25f, -0.1f );
		}

		if (pauseMenuSelection == FPS)
		{
			if(displayFPS==true)
			{
				DrawString( "->Display FPS: ON<-",-0.35f, -0.2f );
			}
			else
			{
				DrawString( "->Display FPS:OFF<-",-0.35f, -0.2f );
			}
		}
		else
		{
			if(displayFPS==true)
			{
				DrawString( "Display FPS: ON",-0.27f, -0.2f );
			}
			else
			{
				DrawString( "Display FPS:OFF",-0.27f, -0.2f );
			}
		}
		
		if (pauseMenuSelection == PLAY_MOVIE)
		{
			DrawString( "->Play the Movie<-", -0.33f, -0.3f );
		}
		else
		{
			DrawString( "Play the Movie", -0.25f, -0.3f );
		}

		if (pauseMenuSelection == QUIT)
		{
			DrawString( "->Return to Menu<-", -0.33f, -0.4f );
		}
		else
		{
			DrawString( "Return to Menu", -0.25f, -0.4f );
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
	}



		
	
	
		TurnOffAlphaBlending();
		TurnZBufferOn();

		return true;
}
////Rectangle for secondary target view
bool ModelsDemo::RenderPassThree(){
	d3dContext_->OMSetRenderTargets( 1, &backBufferTarget_, NULL );
	unsigned int stride = sizeof( VertexPos );
	unsigned int offset = 0;


	TurnZBufferOff();
	TurnOnAlphaBlending();
	
	stride = sizeof( TextVertexPos );
	offset = 0;

	d3dContext_->IASetInputLayout( textInputLayout_ );
	d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );
	d3dContext_->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	d3dContext_->VSSetShader( textTextureMapVS_, 0, 0 );
	d3dContext_->PSSetShader( textTextureMapPS_, 0, 0 );

	//d3dContext_->ClearRenderTargetView( renderTargetViewMap, redColor );	/////
	d3dContext_->PSSetShaderResources( 0, 1, &shaderResourceViewMap  );
    d3dContext_->PSSetSamplers( 0, 1, &textColorMapSampler_ );
	d3dContext_->IASetVertexBuffers( 0, 1, &textVertexBuffer_, &stride, &offset );
	//d3dContext_->IASetVertexBuffers( 0, 1, &vertexBufferRect_, &stride, &offset );
	//d3dContext_->Draw( 6, 0 );


	if(MyGrid->GetWorldType() == 1 || MyGrid->GetWorldType() == 3){
		d3dContext_->PSSetShaderResources( 0, 1, &GridColorMap_ );
	}
	if(MyGrid->GetWorldType() == 2){
		d3dContext_->PSSetShaderResources( 0, 1, &GridColorMapLava_ );
	}
	if(MyGrid->GetWorldType() == 4  || MyGrid->GetWorldType() == 5){
		d3dContext_->PSSetShaderResources( 0, 1, &GridColorMapTemple_ );
	}



	MyGrid->Render(d3dContext_);

	return true;
}