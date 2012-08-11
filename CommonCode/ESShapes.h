#ifndef _ESSHAPES_H_
#define _ESSHAPES_H_
//
//    Utility cass for generating shapes
//
#include <vector>
#include <cmath>
#include "geomUtils.h"
namespace geomUtils
{
static const float cubeVerts[] = 
	{   -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f
	};
const static int cubeVertsSize = sizeof(cubeVerts) / sizeof(cubeVerts[0]);

const static float cubeNormals[] = 
	{    0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
	};
const static int cubeNormalsSize = sizeof(cubeNormals) / sizeof(cubeNormals[0]);

const static float cubeTex[] = 
	{ 	0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 
	};
const static int cubeTexSize = sizeof(cubeTex) / sizeof(cubeTex[0]);

const static short cubeIndices[] = 
	{   
		0, 2, 1, 0, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10,
		8, 10, 11, 12, 15, 14, 12, 14, 13, 16, 17, 18, 16, 18, 19, 20,
		23, 22, 20, 22, 21 
	};
const static int cubeIndicesSize = sizeof(cubeIndices) / sizeof(cubeIndices[0]);

struct geomData
{
	geomData() : mNumIndices(0){}
	tVectorOfFloats mVertices;
	tVectorOfFloats mNormals;
	tVectorOfFloats mTexCoords;
	tVectorOfShorts mIndices;
	int mNumIndices;
};
inline void genTriangle(geomData& obj)
{
    obj.mVertices.push_back(0.0f);
    obj.mVertices.push_back(1.0f);
    obj.mVertices.push_back(0.0f);
    obj.mVertices.push_back(1.0f);
    obj.mVertices.push_back(1.0f);
    obj.mVertices.push_back(-1.0f);
    obj.mVertices.push_back(0.0f);
    obj.mVertices.push_back(1.0f);
    obj.mVertices.push_back(-1.0f);
    obj.mVertices.push_back(-1.0f);
    obj.mVertices.push_back(0.0f);
    obj.mVertices.push_back(1.0f);
}
    
inline
int genSphere(int numSlices, float radius, geomData& obj) 
{
		int i;
		int j;
		int numParallels = numSlices;
		int numVertices = (numParallels + 1) * (numSlices + 1);
		int numIndices = numParallels * numSlices * 6;
		float angleStep = ((2.0f * (float) PI) / numSlices);

		// Allocate memory for buffers
		obj.mVertices.resize(numVertices * 3 * 4);
		obj.mNormals.resize(numVertices * 3 * 4);
		obj.mTexCoords.resize(numVertices * 2 * 4);
		obj.mIndices.resize(numIndices * 2);

		for (i = 0; i < numParallels + 1; i++) {
			for (j = 0; j < numSlices + 1; j++) {
				int vertex = (i * (numSlices + 1) + j) * 3;
				obj.mVertices[vertex + 0] = (float) (radius	* sin(angleStep * (float) i) * sin(angleStep * (float) j));
				obj.mVertices[vertex + 1] = (float) (radius * cos(angleStep * (float) i));
				obj.mVertices[vertex + 2] = (float) (radius	* sin(angleStep * (float) i) * cos(angleStep * (float) j));

				obj.mNormals[vertex + 0] = obj.mVertices[vertex + 0] / radius;
				obj.mNormals[vertex + 1] = obj.mVertices[vertex + 1] / radius;
				obj.mNormals[vertex + 2] = obj.mVertices[vertex + 2] / radius;

				int texIndex = (i * (numSlices + 1) + j) * 2;
				obj.mTexCoords[texIndex + 0] = (float) j / (float) numSlices;
				obj.mTexCoords[texIndex + 1] = (1.0f - (float) i) / (float) (numParallels - 1);
			}
		}

		int index = 0;
		for (i = 0; i < numParallels; i++) {
			for (j = 0; j < numSlices; j++) {
				obj.mIndices[index++] = (short) (i * (numSlices + 1) + j);
				obj.mIndices[index++] = (short) ((i + 1) * (numSlices + 1) + j);
				obj.mIndices[index++] =	(short) ((i + 1) * (numSlices + 1) + (j + 1));
				obj.mIndices[index++] = (short) (i * (numSlices + 1) + j);
				obj.mIndices[index++] = (short) ((i + 1) * (numSlices + 1) + (j + 1));
				obj.mIndices[index++] = (short) (i * (numSlices + 1) + (j + 1));
			}
		}
		obj.mNumIndices = numIndices;

		return numIndices;
	}

	inline
	int genCube(float scale, geomData& obj) 
	{
		int numVertices = 24;
		int numIndices = 36;

		// Allocate memory for buffers
		obj.mVertices.resize(numVertices * 3 * 4);
		obj.mNormals.resize(numVertices * 3 * 4);
		obj.mTexCoords.resize(numVertices * 2 * 4);
		obj.mIndices.resize(numIndices * 2);
		obj.mVertices.insert(obj.mVertices.begin(), cubeVerts, cubeVerts+cubeVertsSize);
		for (size_t i = 0; i < numVertices * 3; i++) 
			obj.mVertices[i] *= scale;
		
		obj.mNormals.insert(obj.mNormals.begin(), cubeNormals, cubeNormals+cubeNormalsSize);
		obj.mTexCoords.insert(obj.mTexCoords.begin(), cubeTex, cubeTex+cubeTexSize);
		obj.mIndices.insert(obj.mIndices.begin(), cubeIndices, cubeIndices+cubeIndicesSize);
		return numIndices;
	}
};
#endif
