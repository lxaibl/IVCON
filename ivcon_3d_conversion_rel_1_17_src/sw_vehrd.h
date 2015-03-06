//--------------------------------------------------------------------------------------
// File: sw_vehrd.h
//
// Desc: Syndicate Wars Vehicle Reader class
//
// Written by Marco Castrucci <mmacleod@tiscali.it> in 2006
//--------------------------------------------------------------------------------------

#ifndef SW_VEHRD_H
#define SW_VEHRD_H

#include <stdio.h>

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short int u16;
typedef signed short int s16;
typedef unsigned long int u32;
typedef signed long int s32;

class CVehiclesReader
{
public:
	CVehiclesReader();
	~CVehiclesReader();
	void Read(FILE *fp);

	struct SHeader	// 18 bytes
	{
		u16 un0;	// "un" means unknown
		u16 un1;
		
		u16 numVertices;	// number of objects in block [A]

		u16 numTriFaces;	// number of Triangular faces; number of objects in block [B]

		u16 numQuadFaces;	// number of Quadrate faces; number of objects in block [C]
	
		u16 numMeshes;		// not sure if it's a 16 or 8 bit

		u16 numQuadUVFaces;	// number of Quadrate UV Faces faces; number of objects in block [E]
		
		u16 numTriUVFaces;	// number of Triangular UV Faces faces; number of objects in block [F]

		u16 pad0;
	};

	struct SVertex	// 10 bytes
	{
		u8 un0;	// some index number??
		u8 un1;	// usually 00
		
		s16 x;
		s16 y;
		s16 z;

		u8 un2;	// usually 0
		u8 un3; // flags??
	};

	struct STriFace	// 32 bytes
	{
		u16 v0;		// vertex index
		u16 v1;
		u16 v2;

		u16 uvFace;	// uv face index

		u16 un0;
		u16 un1;
		u16 un2;

		u16 pad0[3];

		u16 un3;

		u16 pad1[2];

		u16 un4;

		u16 pad2[2];
	};

	struct SQuadFace	// 40 bytes
	{
		u16 v0;	// vertex index
		u16 v1;
		u16 v2;
		u16 v3;

		u16 uvFace;	// uv face index

		u16 un0;
		u16 un1;
		u16 un2;

		u16 pad0[4];

		u16 un3;

		u16 pad1[3];
		
		u16 un4;

		u16 pad2[3];
	};

	struct SMesh	// 36 bytes
	{
		u16 firstTriFace;	// index of first face of the mesh in the Tri faces arrays
		u16 numTriFaces;	// num Tri faces that this mesh has

		u16 pad0;

		u16 firstQuadFace;	// index of first face of the mesh in the Quad faces arrays
		u16 numQuadFaces;	// num Quad faces that this mesh has

		u16 pad1[7];

		u16 firstVertexIndex;	// index of first vertex
		u16 lastVertexIndex;	// index of last vertex (actually only (lastVertexIndex-1) index is used)

		u16 un0;	// usually it's 0x3 ??

		u16 pad2[3];
	};

	struct SQuadUVFace	// 18 bytes
	{
		u8 u0,v0;	// uv coord
		u8 u1,v1;
		u8 u2,v2;
		u8 u3,v3;

		u16 texture;	// texture number, for example, 1 means TEX01.DAT

		u16 pad[4];
	};

	struct STriUVFace	// 16 byte
	{
		u8 u0,v0;	// uv coord
		u8 u1,v1;
		u8 u2,v2;

		u16 texture;	// texture number, for example, 1 means TEX01.DAT

		u16 pad[3];

		u16 un0;
	};

	SHeader m_header;
	SVertex *m_vertices;
	STriFace *m_triFaces;
	SQuadFace *m_quadFaces;
	SMesh *m_meshes;
	SQuadUVFace *m_quadUVFaces;
	STriUVFace *m_triUVFaces;

private:

	void ReadHeader(FILE *fp);
	void ReadVertices(FILE *fp);
	void ReadTriFaces(FILE *fp);
	void ReadQuadFaces(FILE *fp);
	void ReadMeshes(FILE *fp);
	void ReadQuadUVFaces(FILE *fp);
	void ReadTriUVFaces(FILE *fp);
};

CVehiclesReader *sw_vehrd_read(const char *_szFilePath);

#endif	//SW_VEHRD_H
