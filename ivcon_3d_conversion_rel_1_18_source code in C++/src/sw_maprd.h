//--------------------------------------------------------------------------------------
// File: sw_maprd.h
//
// Desc: Syndicate Wars Map Reader class
//
// Written by Marco Castrucci <mmacleod@tiscali.it> in 2006
//--------------------------------------------------------------------------------------

#ifndef SW_MAPRD_H
#define SW_MAPRD_H

#include <cstdio>

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short int u16;
typedef signed short int s16;
typedef unsigned long int u32;
typedef signed long int s32;

class CMapReader
{
public:
	CMapReader();
	~CMapReader();
	bool Read(FILE *fp);

	struct SHeader	// 38 bytes
	{
		u16 un0;	// "un" means unknown
		u16 un1;
		u16 numQuadUVFaces;
		u16 numTriUVFaces;
		u16 numVertices;
		u16 numTriFaces;
		u16 numMeshes;
		u16 numBlockG;
		u16 numBlockH;
		u16 numBlockI;
		u16 numQuadFaces;
		u16 numBlockK;
		u16 numNavipoints;
		u16 numBlockM;
		u16 numBlockN;
		u16 numBlockO;
		u16 numBlockP;
		u16 numBlockQ;
		u16 numBlockR;
	};

	struct STerrainCell		// 18 bytes
	{
		u16 uvFace;			// quad uv map index
		u16 un0;
		u16 un1;
		s16 vertHeight;		// vertex height (this value must be multiplied by 8)
		u16 un2;
		u16 un3;
		u16 un4;
		u16 un5;
		u16 un6;
	};

	struct SQuadUVFace	// 18 bytes
	{
		u8 u0,v0;		// uv coord
		u8 u1,v1;
		u8 u2,v2;
		u8 u3,v3;

		u16 texture;	// texture number, for example, 1 means TEX01.DAT

		u16 pad[4];
	};

	struct STriUVFace	// 16 byte
	{
		u8 u0,v0;		// uv coord
		u8 u1,v1;
		u8 u2,v2;

		u16 texture;	// texture number, for example, 1 means TEX01.DAT

		u16 pad[4];
	};

	struct SVertex	// 10 bytes
	{
		u8 un0;
		u8 un1;

		s16 x;
		s16 y;
		s16 z;

		u8 un2;
		u8 un3;
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
		u16 un3;
		u16 un4;
		u16 un5;
		u16 un6;
		u16 un7;
		u16 un8;
		u16 un9;
		u16 un10;
		u16 un11;
	};

	struct SMesh	// 36 bytes
	{
		u16 firstTriFace;	// index of first face of the mesh in the Tri faces arrays
		u16 numTriFaces;	// num Tri faces that this mesh has

		u16 un0;

		u16 firstQuadFace;	// index of first face of the mesh in the Quad faces arrays
		u16 numQuadFaces;	// num Quad faces that this mesh has

		u16 un1;
		u16 un2;

		s16 posY;

		u16 un3;
		u16 un4;

		s16 posX;
		s16 posZ;

		u16 firstVertexIndex;	// index of first vertex
		u16 lastVertexIndex;	// index of last vertex (actually only (lastVertexIndex-1) index is used)

		u16 un5;
		u16 un6;
		u16 un7;
		u16 un8;
	};

	struct SBlockG	// 6 bytes
	{
		u16 un0;
		u16 un1;
		u16 un2;
	};

	struct SBlockH	// 32 bytes
	{
		u16 un0;
		u16 un1;
		u16 un2;
		u16 un3;
		u16 un4;
		u16 un5;
		u16 un6;
		u16 un7;
		u16 un8;
		u16 un9;
		u16 un10;
		u16 un11;
		u16 un12;
		u16 un13;
		u16 un14;
		u16 un15;
	};

	struct SBlockI	// 16 bytes
	{
		u16 un0;
		u16 un1;
		u16 un2;
		u16 un3;
		u16 un4;
		u16 un5;
		u16 un6;
		u16 un7;
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
		u16 un3;
		u16 un4;
		u16 un5;
		u16 un6;
		u16 un7;
		u16 un8;
		u16 un9;
		u16 un10;
		u16 un11;
		u16 un12;
		u16 un13;
		u16 un14;
	};

	struct SBlockK	// 54 bytes
	{
		u16 un[27];
	};

	struct SNavipoint	// 36 bytes
	{
		s16 posX;
		s16 posY;
		s16 posZ;

		u16 un0;
		u16 un1;
		u16 un2;
		u16 un3;
		u16 un4;
		u16 un5;
		u16 un6;
		u16 un7;
		u16 un8;
		u16 un9;

		u16 pad0[5];
	};

	struct SBlockM	// 36 bytes
	{
		u16 un[10];
		u16 pad[8];
	};

	struct SBlockN	// 6 bytes
	{
		u16 un0;
		u16 un1;
		u16 un2;
	};

	struct SBlockO	// 14 bytes
	{
		s16 pos0X;
		s16 pos0Y;
		s16 pos0Z;
		s16 pos1X;
		s16 pos1Y;
		s16 pos1Z;
		u16 un0;
	};

	struct SBlockP	// 4 bytes
	{
		u16 un0;
		u16 un1;
	};

	struct SBlockQ	// 2 bytes
	{
		u8 un0;
		u8 un1;
	};

	struct SBlockR	// 16 bytes
	{
		u16 un0;
		u16 un1;
		u16 un2;
		u16 un3;
		u16 un4;
		u16 un5;
		u16 un6;
		u16 un7;
	};

	SHeader m_header;
	STerrainCell *m_terrainCells;
	SQuadUVFace *m_quadUVFaces;
	STriUVFace *m_triUVFaces;
	SVertex *m_vertices;
	STriFace *m_triFaces;
	SMesh *m_meshes;
	SBlockG *m_blockG;
	SBlockH *m_blockH;
	SBlockI *m_blockI;
	SQuadFace *m_quadFaces;
	SBlockK *m_blockK;
	SNavipoint *m_navipoints;
	SBlockM *m_blockM;
	SBlockN *m_blockN;
	SBlockO *m_blockO;
	SBlockP *m_blockP;
	SBlockQ *m_blockQ;
	SBlockR *m_blockR;

private:
	bool CheckBlockEnd(u16 data,FILE *fp);
	void SkipExtraData(u16 num,u16 size,FILE *fp);

	void ReadHeader(FILE *fp);
	void ReadTerrainCells(FILE *fp);
	void ReadQuadUVFaces(FILE *fp);
	void ReadTriUVFaces(FILE *fp);
	void ReadVertices(FILE *fp);
	void ReadTriFaces(FILE *fp);
	void ReadMeshes(FILE *fp);
	void ReadBlockG(FILE *fp);
	void ReadBlockH(FILE *fp);
	void ReadBlockI(FILE *fp);
	void ReadQuadFaces(FILE *fp);
	void ReadBlockK(FILE *fp);
	void ReadNavipoints(FILE *fp);
	void ReadBlockM(FILE *fp);
	void ReadBlockN(FILE *fp);
	void ReadBlockO(FILE *fp);
	void ReadBlockP(FILE *fp);
	void ReadBlockQ(FILE *fp);
	void ReadBlockR(FILE *fp);
};

CMapReader *sw_maprd_read(const char *_szFilePath);

#endif	//SW_MAPRD_H
