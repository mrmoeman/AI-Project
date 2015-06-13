/*
    Beginning DirectX 11 Game Programming
    By Allen Sherrod and Wendy Jones

    ObjModel - Used to represent an OBJ model.
*/


#include<fstream>
#include<vector>
#include<string>
#include"objLoader.h"
#include"TokenStream.h"


ObjModel::ObjModel( )
{
    vertices_ = 0;
    normals_ = 0;
    texCoords_ = 0;
    totalVerts_ = 0;
}

      
ObjModel::~ObjModel( )
{
    Release( );
}


void ObjModel::Release( )
{
    totalVerts_ = 0;

    if( vertices_ != 0 ) delete[] vertices_;
    if( normals_ != 0 ) delete[] normals_;
    if( texCoords_ != 0 ) delete[] texCoords_;

    vertices_ = 0;
    normals_ = 0;
    texCoords_ = 0;
}


bool ObjModel::LoadOBJ( char *fileName )
{
    std::ifstream fileStream;
    int fileSize = 0;

    fileStream.open( fileName, std::ifstream::in );
   
    if( fileStream.is_open( ) == false )
        return false;

    fileStream.seekg( 0, std::ios::end );
    fileSize = ( int )fileStream.tellg( );
    fileStream.seekg( 0, std::ios::beg );

    if( fileSize <= 0 )
        return false;

    char *buffer = new char[fileSize];

    if( buffer == 0 )
        return false;

    memset( buffer, '\0', fileSize );

    TokenStream tokenStream, lineStream, faceStream;
    std::string tempLine, token;

    fileStream.read( buffer, fileSize );
    tokenStream.SetTokenStream( buffer );

    delete[] buffer;

    tokenStream.ResetStream( );

    std::vector<float> verts, norms, texC;
    std::vector<int> faces;

    char lineDelimiters[2] = { '\n', ' ' };

    while( tokenStream.MoveToNextLine( &tempLine ) )
    {
        lineStream.SetTokenStream( ( char* )tempLine.c_str( ) );
        tokenStream.GetNextToken( 0, 0, 0 );

        if( !lineStream.GetNextToken( &token, lineDelimiters, 2 ) )
            continue;

        if( strcmp( token.c_str( ), "v" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            verts.push_back( ( float )atof( token.c_str( ) ) );

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            verts.push_back( ( float )atof( token.c_str( ) ) );

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            verts.push_back( ( float )atof( token.c_str( ) ) );
        }
        else if( strcmp( token.c_str( ), "vn" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            norms.push_back( ( float )atof( token.c_str( ) ) );

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            norms.push_back( ( float )atof( token.c_str( ) ) );

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            norms.push_back( ( float )atof( token.c_str( ) ) );
        }
        else if( strcmp( token.c_str( ), "vt" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            texC.push_back( ( float )atof( token.c_str( ) ) );

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            texC.push_back( ( float )atof( token.c_str( ) ) );
        }
        else if( strcmp( token.c_str( ), "f" ) == 0 )
        {
            char faceTokens[3] = { '\n', ' ', '/' };
            std::string faceIndex;

            faceStream.SetTokenStream( ( char* )tempLine.c_str( ) );
            faceStream.GetNextToken( 0, 0, 0 );

            for( int i = 0; i < 3; i++ )
            {
                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );

                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );

                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );
            }
        }
        else if( strcmp( token.c_str( ), "#" ) == 0 )
        {
            int a = 0;
            int b = a;
        }

        token[0] = '\0';
    }

    // "Unroll" the loaded obj information into a list of triangles.

    int vIndex = 0, nIndex = 0, tIndex = 0;
    int numFaces = ( int )faces.size( ) / 9;

    totalVerts_ = numFaces * 3;

    vertices_ = new float[totalVerts_ * 3];			///1D arrays of size total verts * 3
	tangents_ = new float[totalVerts_ * 3];
	bitangents_ = new float[totalVerts_ * 3];

    if( ( int )norms.size( ) != 0 )
    {
        normals_ = new float[totalVerts_ * 3];
    }

    if( ( int )texC.size( ) != 0 )
    {
        texCoords_ = new float[totalVerts_ * 2];
    }

	//George Anderson
	//vertex, normal, and texture arrays are organised by traingles
	//so first 9 pieces of data in vertex refer to the position of the three vertices for the first triangle
	//first 6 pieces of data in texture array refer to the UV for each corresponding vertex for the first triangle
	//first 9 pieces of data normal array refer to the normals for each corresponding vertex for the first triangle

	//each triangle needs a tangent and a bitangent
	//which are calculated on object loading (preferably done before we export the object to our own format)
	//calculated as such where T = tangent, B = bitangent, deltapos are vector 3f position of two vertexs, deltaUV are floats for the texture
	//T and B are both Vector3f
	//deltaPos1 = deltaUV1.x * T + deltaUV1.y * B
	//deltaPos2 = deltaUV2.x * T + deltaUV2.y * B


    for( int f = 0; f < ( int )faces.size( ); f+=3 )
    {
        vertices_[vIndex + 0] = verts[( faces[f + 0] - 1 ) * 3 + 0];
        vertices_[vIndex + 1] = verts[( faces[f + 0] - 1 ) * 3 + 1];
        vertices_[vIndex + 2] = verts[( faces[f + 0] - 1 ) * 3 + 2];
        vIndex += 3;

        if(texCoords_)
        {
            texCoords_[tIndex + 0] = texC[( faces[f + 1] - 1 ) * 2 + 0];
            texCoords_[tIndex + 1] = texC[( faces[f + 1] - 1 ) * 2 + 1];
            tIndex += 2;
        }

        if(normals_)
        {
            normals_[nIndex + 0] = norms[( faces[f + 2] - 1 ) * 3 + 0];
            normals_[nIndex + 1] = norms[( faces[f + 2] - 1 ) * 3 + 1];
            normals_[nIndex + 2] = norms[( faces[f + 2] - 1 ) * 3 + 2];
            nIndex += 3;
        }
    }

	// calculating and storing the tangent and bitangent data
	for (int Tri = 0; Tri <= totalVerts_ / 3; Tri++){
		
	float vector1[3], vector2[3], tangent[3], binormal[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;
	// Calculate the two vectors for this face.
	vector1[0] = vertices_[(Tri*9) + 3] - vertices_[(Tri*9)];
	vector1[1] = vertices_[(Tri*9) + 4] - vertices_[(Tri*9) + 1];
	vector1[2] = vertices_[(Tri*9) + 5] - vertices_[(Tri*9) + 2];

	vector2[0] = vertices_[(Tri*9) + 6] - vertices_[(Tri*9)];
	vector2[1] = vertices_[(Tri*9) + 7] - vertices_[(Tri*9) + 1];
	vector2[2] = vertices_[(Tri*9) + 8] - vertices_[(Tri*9) + 2];

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = texCoords_[(Tri*6) + 2] - texCoords_[(Tri*6)];
	tvVector[0] = texCoords_[(Tri*6) + 3] - texCoords_[(Tri*6) + 1];

	tuVector[1] = texCoords_[(Tri*6) + 4] - texCoords_[(Tri*6)];
	tvVector[1] = texCoords_[(Tri*6) + 5] - texCoords_[(Tri*6) + 1];

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent[0] = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent[1] = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent[2] = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal[0] = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal[1] = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal[2] = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent[0] * tangent[0]) + (tangent[1] * tangent[1]) + (tangent[2] * tangent[2]));

	// Normalize the normal and then store it
	tangent[0] = tangent[0] / length;
	tangent[1] = tangent[1] / length;
	tangent[2] = tangent[2] / length;

	// Calculate the length of this normal.
	length = sqrt((binormal[0] * binormal[0]) + (binormal[1] * binormal[1]) + (binormal[2] * binormal[2]));
			
	// Normalize the normal and then store it
	binormal[0] = binormal[0] / length;
	binormal[1] = binormal[1] / length;
	binormal[2] = binormal[2] / length;


	tangents_[Tri] = tangent[0];
	tangents_[Tri + 3] = tangent[0];
	tangents_[Tri + 6] = tangent[0];

	tangents_[Tri + 1] = tangent[1];
	tangents_[Tri + 4] = tangent[1];
	tangents_[Tri + 7] = tangent[1];

	tangents_[Tri + 2] = tangent[2];
	tangents_[Tri + 5] = tangent[2];
	tangents_[Tri + 8] = tangent[2];

	bitangents_[Tri] = binormal[0];
	bitangents_[Tri + 3] = binormal[0];
	bitangents_[Tri + 6] = binormal[0];

	bitangents_[Tri + 1] = binormal[1];
	bitangents_[Tri + 4] = binormal[1];
	bitangents_[Tri + 7] = binormal[1];

	bitangents_[Tri + 2] = binormal[2];
	bitangents_[Tri + 5] = binormal[2];
	bitangents_[Tri + 8] = binormal[2];

	}


    verts.clear( );
    norms.clear( );
    texC.clear( );
    faces.clear( );

    return true;
}