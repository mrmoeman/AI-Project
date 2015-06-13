/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    Texture Mapping Shader for 3D Objects
*/


Texture2D colorMap : register( t0 );
SamplerState colorSampler : register( s0 );

Texture2D normalMap : register( t1 );
SamplerState normalSampler : register( s1 );


cbuffer cbChangesEveryFrame : register( b0 )
{
    matrix worldMatrix;
};

cbuffer cbNeverChanges : register( b1 )
{
    matrix viewMatrix;
};

cbuffer cbChangeOnResize : register( b2 )
{
    matrix projMatrix;
};

cbuffer cbCameraData : register( b3 )
{
    float3 cameraPos;
};

cbuffer cbswitch : register( b4 )
{
    float3 NormalOn;
};

struct VS_Input
{
    float4 pos  : POSITION;
    float2 tex0 : TEXCOORD0;
    float3 norm : NORMAL;
	float3 tang : TANGENT;
	float3 bitang : BITANGENT;
};

struct PS_Input
{
    float4 pos  : SV_POSITION;
    float2 tex0 : TEXCOORD0;
    float3 norm : NORMAL;
	float3 tang : TANGENT;
	float3 bitang : BITANGENT;
    float3 lightVec : TEXCOORD1;
	float3 viewVec : TEXCOORD2;
};


PS_Input VS_Main( VS_Input vertex )
{
    PS_Input vsOut = ( PS_Input )0;
    
	//vsOut.pos = mul( vertex.pos, worldMatrix );
	float4 worldPos = mul( vertex.pos, worldMatrix );

    //vsOut.pos = mul( vsOut.pos, viewMatrix );
	vsOut.pos = mul( worldPos, viewMatrix );

    vsOut.pos = mul( vsOut.pos, projMatrix );
    vsOut.tex0 = vertex.tex0;

	
	vsOut.norm = mul( vertex.norm, (float3x3)worldMatrix );
    vsOut.norm = normalize( vsOut.norm );

	vsOut.tang = mul( vertex.tang, (float3x3)worldMatrix );
    vsOut.tang = normalize( vsOut.tang );

	vsOut.bitang = mul( vertex.bitang, (float3x3)worldMatrix );
    vsOut.bitang = normalize( vsOut.bitang );

    //float3 lightPos = float3( 0.0f, 500.0f, 50.0f );
	float3 lightPos = float3( 0.0f, 500.0f, 500.0f );
    vsOut.lightVec = normalize( lightPos - worldPos );

    vsOut.viewVec = normalize( cameraPos - worldPos );
	
    return vsOut;
}

float4 PS_Main( PS_Input frag ) : SV_TARGET
{
    //return colorMap.Sample( colorSampler, frag.tex0 );
	float3 textureColor = colorMap.Sample( colorSampler, frag.tex0 );

	float3 normalM = normalMap.Sample( colorSampler, frag.tex0 );	////// gets normal map

	normalM = (2.0f*normalM) - 1.0f;	////////changes normal map range

	//Make sure tangent is completely orthogonal to normal
	//frag.tang = normalize(frag.tang - dot(frag.tang, frag.norm)*frag.norm);		//breaks?????

	//Create the "Texture Space"
	float3x3 texSpace = float3x3(frag.tang, frag.bitang, frag.norm);

	frag.norm = normalize(mul(normalM, texSpace)* frag.viewVec);		/// added * viewvec

	frag.norm = (frag.norm + 1)/2;		///makes normal between 0 and 1

	float3 ambientColor = float3( 0.5f, 0.5f, 0.5f );
    float3 lightColor = float3( 0.7f, 0.7f, 0.7f );

	
    float3 lightVec = normalize( frag.lightVec);

    float diffuseTerm = clamp( dot( frag.norm, lightVec ), 0.0f, 1.0f );
    float specularTerm = 0;

    if( diffuseTerm > 0.0f )
    {
        float3 viewVec = normalize( frag.viewVec );
        float3 halfVec = normalize( lightVec + viewVec );

        specularTerm = pow( saturate( dot( frag.norm, halfVec ) ), 25 ); 
    }

    float3 finalLightColor = ambientColor + lightColor * diffuseTerm + lightColor * specularTerm;

	float3 finalColor = finalLightColor*textureColor;

	if(NormalOn.x > 0){
		return float4( textureColor, 1.0f );
	}
	else{
		return float4( finalColor, 1.0f );
	}

}
