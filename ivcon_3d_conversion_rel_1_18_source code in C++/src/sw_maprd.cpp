//--------------------------------------------------------------------------------------
// File: sw_maprd.cpp
//
// Desc: Syndicate Wars Map Reader class
//
// Written by Marco Castrucci <mmacleod@tiscali.it> in 2006
//--------------------------------------------------------------------------------------

#include "sw_maprd.h"
#include <memory.h>
#include <assert.h>

extern void ErrorMessage(const char *fmt,...);

extern u8 ReadU8(FILE *fp);
extern s8 ReadS8(FILE *fp);
extern u16 ReadU16(FILE *fp);
extern s16 ReadS16(FILE *fp);

//------------------------------------------------------------
CMapReader::CMapReader()
{
	memset(&m_header, 0, sizeof(SHeader));
	m_terrainCells = NULL;
	m_quadUVFaces = NULL;
	m_triUVFaces = NULL;
	m_vertices = NULL;
	m_triFaces = NULL;
	m_meshes = NULL;
	m_blockG = NULL;
	m_blockH = NULL;
	m_blockI = NULL;
	m_quadFaces = NULL;
	m_blockK = NULL;
	m_navipoints = NULL;
	m_blockM = NULL;
	m_blockN = NULL;
	m_blockO = NULL;
	m_blockP = NULL;
	m_blockQ = NULL;
	m_blockR = NULL;
}

CMapReader::~CMapReader()
{
	if(m_terrainCells)	delete [] m_terrainCells;

	if(m_quadUVFaces)	delete [] m_quadUVFaces;

	if(m_triUVFaces)	delete [] m_triUVFaces;

	if(m_vertices)	delete [] m_vertices;

	if(m_triFaces)	delete [] m_triFaces;

	if(m_meshes)	delete [] m_meshes;

	if(m_blockG)	delete [] m_blockG;

	if(m_blockH)	delete [] m_blockH;

	if(m_blockI)	delete [] m_blockI;

	if(m_quadFaces)	delete [] m_quadFaces;

	if(m_blockK)	delete [] m_blockK;

	if(m_navipoints)	delete [] m_navipoints;

	if(m_blockM)	delete [] m_blockM;

	if(m_blockN)	delete [] m_blockN;

	if(m_blockO)	delete [] m_blockO;

	if(m_blockP)	delete [] m_blockP;

	if(m_blockQ)	delete [] m_blockQ;

	if(m_blockR)	delete [] m_blockR;
}

bool CMapReader::Read(FILE *fp)
{
	ReadHeader(fp);
	
	ReadTerrainCells(fp);
	
	if(CheckBlockEnd(0x00,fp) == false)
		return false;

	ReadQuadUVFaces(fp); SkipExtraData(800,18,fp);

	if(CheckBlockEnd(0x01,fp) == false)
		return false;

	ReadTriUVFaces(fp); SkipExtraData(800,16,fp);

	if(CheckBlockEnd(0x02,fp) == false)
		return false;

	ReadVertices(fp); SkipExtraData(2000,10,fp);

	if(CheckBlockEnd(0x03,fp) == false)
		return false;

	ReadTriFaces(fp); SkipExtraData(2000,32,fp);

	if(CheckBlockEnd(0x04,fp) == false)
		return false;

	ReadMeshes(fp); SkipExtraData(120,36,fp);

	if(CheckBlockEnd(0x05,fp) == false)
		return false;

	ReadBlockG(fp); SkipExtraData(4000,6,fp);

	if(CheckBlockEnd(0x06,fp) == false)
		return false;
	
	ReadBlockH(fp); SkipExtraData(10,32,fp);

	if(CheckBlockEnd(0x07,fp) == false)
		return false;

	ReadBlockI(fp); SkipExtraData(3500,16,fp);

	if(CheckBlockEnd(0x08,fp) == false)
		return false;

	ReadQuadFaces(fp); SkipExtraData(2000,40,fp);

	if(CheckBlockEnd(0x09,fp) == false)
		return false;

	ReadBlockK(fp); SkipExtraData(50,54,fp);

	if(CheckBlockEnd(0x0A,fp) == false)
		return false;

	ReadNavipoints(fp); SkipExtraData(50,36,fp);

	if(CheckBlockEnd(0x0B,fp) == false)
		return false;

	ReadBlockM(fp); SkipExtraData(50,36,fp);

	if(CheckBlockEnd(0x0C,fp) == false)
		return false;

	ReadBlockN(fp); SkipExtraData(1000,6,fp);

	if(CheckBlockEnd(0x0D,fp) == false)
		return false;

	ReadBlockO(fp); SkipExtraData(600,14,fp);

	if(CheckBlockEnd(0x0E,fp) == false)
		return false;

	ReadBlockP(fp);

	if(CheckBlockEnd(0x0F,fp) == false)
		return false;

	ReadBlockQ(fp);

	if(CheckBlockEnd(0x10,fp) == false)
		return false;

	ReadBlockR(fp);

	if(CheckBlockEnd(0x11,fp) == false)
		return false;

	return true;
}

void CMapReader::ReadHeader(FILE *fp)
{
	m_header.un0 = ReadU16(fp);
	m_header.un1 = ReadU16(fp);
	m_header.numQuadUVFaces = ReadU16(fp);
	m_header.numTriUVFaces = ReadU16(fp);
	m_header.numVertices = ReadU16(fp);
	m_header.numTriFaces = ReadU16(fp);
	m_header.numMeshes = ReadU16(fp);
	m_header.numBlockG = ReadU16(fp);
	m_header.numBlockH = ReadU16(fp);
	m_header.numBlockI = ReadU16(fp);
	m_header.numQuadFaces = ReadU16(fp);
	m_header.numBlockK = ReadU16(fp);
	m_header.numNavipoints = ReadU16(fp);
	m_header.numBlockM = ReadU16(fp);
	m_header.numBlockN = ReadU16(fp);
	m_header.numBlockO = ReadU16(fp);
	m_header.numBlockP = ReadU16(fp);
	m_header.numBlockQ = ReadU16(fp);
	m_header.numBlockR = ReadU16(fp);
}

void CMapReader::ReadTerrainCells(FILE *fp)
{
	int num = 128*128;

	m_terrainCells = new STerrainCell[num];
	memset(m_terrainCells, 0, sizeof(STerrainCell)*num);

	STerrainCell *c = m_terrainCells;
	for(int i=0; i<num; i++, c++)
	{
		c->uvFace = ReadU16(fp);

		c->un0 = ReadU16(fp);
		c->un1 = ReadU16(fp);

		c->vertHeight = ReadS16(fp);

		c->un2 = ReadU16(fp);
		c->un3 = ReadU16(fp);
		c->un4 = ReadU16(fp);
		c->un5 = ReadU16(fp);
		c->un6 = ReadU16(fp);
	}
}

void CMapReader::ReadQuadUVFaces(FILE *fp)
{
	m_quadUVFaces = new SQuadUVFace[m_header.numQuadUVFaces];
	memset(m_quadUVFaces, 0, sizeof(SQuadUVFace)*m_header.numQuadUVFaces);

	SQuadUVFace *f = m_quadUVFaces;
	for(int i=0; i<m_header.numQuadUVFaces; i++, f++)
	{
		f->u0 = ReadU8(fp);	f->v0 = ReadU8(fp);
		f->u1 = ReadU8(fp);	f->v1 = ReadU8(fp);
		f->u2 = ReadU8(fp);	f->v2 = ReadU8(fp);
		f->u3 = ReadU8(fp);	f->v3 = ReadU8(fp);

		f->texture = ReadU16(fp);

		f->pad[0] = ReadU16(fp);
		f->pad[1] = ReadU16(fp);
		f->pad[2] = ReadU16(fp);
		f->pad[3] = ReadU16(fp);
	}
}

void CMapReader::ReadTriUVFaces(FILE *fp)
{
	m_triUVFaces = new STriUVFace[m_header.numTriUVFaces];
	memset(m_triUVFaces, 0, sizeof(STriUVFace)*m_header.numTriUVFaces);

	STriUVFace *f = m_triUVFaces;
	for(int i=0; i<m_header.numTriUVFaces; i++, f++)
	{
		f->u0 = ReadU8(fp);	f->v0 = ReadU8(fp);
		f->u1 = ReadU8(fp);	f->v1 = ReadU8(fp);
		f->u2 = ReadU8(fp);	f->v2 = ReadU8(fp);

		f->texture = ReadU16(fp);

		f->pad[0] = ReadU16(fp);
		f->pad[1] = ReadU16(fp);
		f->pad[2] = ReadU16(fp);
		f->pad[3] = ReadU16(fp);
	}
}

void CMapReader::ReadVertices(FILE *fp)
{
	m_vertices = new SVertex[m_header.numVertices];
	memset(m_vertices, 0, sizeof(SVertex)*m_header.numVertices);

	SVertex *v = m_vertices;
	for(u32 i=0; i<m_header.numVertices; i++, v++)
	{
		v->un0 = ReadU8(fp);
		v->un1 = ReadU8(fp);

		v->x = ReadS16(fp);
		v->y = ReadS16(fp);
		v->z = ReadS16(fp);

		v->un2 = ReadU8(fp);
		v->un3 = ReadU8(fp);
	}
}

void CMapReader::ReadTriFaces(FILE *fp)
{
	m_triFaces = new STriFace[m_header.numTriFaces];
	memset(m_triFaces, 0, sizeof(STriFace)*m_header.numTriFaces);

	STriFace *f = m_triFaces;
	for(u32 i=0; i<m_header.numTriFaces; i++, f++)
	{
		f->v0 = ReadU16(fp);
		f->v1 = ReadU16(fp);
		f->v2 = ReadU16(fp);

		f->uvFace = ReadU16(fp);

		f->un0 = ReadU16(fp);
		f->un1 = ReadU16(fp);
		f->un2 = ReadU16(fp);
		f->un3 = ReadU16(fp);
		f->un4 = ReadU16(fp);
		f->un5 = ReadU16(fp);
		f->un6 = ReadU16(fp);
		f->un7 = ReadU16(fp);
		f->un8 = ReadU16(fp);
		f->un9 = ReadU16(fp);
		f->un10 = ReadU16(fp);
		f->un11 = ReadU16(fp);
	}
}

void CMapReader::ReadMeshes(FILE *fp)
{
	m_meshes = new SMesh[m_header.numMeshes];
	memset(m_meshes, 0, sizeof(SMesh)*m_header.numMeshes);

	SMesh *m = m_meshes;
	for(u32 i=0; i<m_header.numMeshes; i++, m++)
	{
		m->firstTriFace = ReadU16(fp);
		m->numTriFaces = ReadU16(fp);

		m->un0 = ReadU16(fp);

		m->firstQuadFace = ReadU16(fp);
		m->numQuadFaces = ReadU16(fp);

		m->un1 = ReadU16(fp);
		m->un2 = ReadU16(fp);
		
		m->posY = ReadS16(fp);
		
		m->un3 = ReadU16(fp);
		m->un4 = ReadU16(fp);

		m->posX = ReadS16(fp);
		m->posZ = ReadS16(fp);

		m->firstVertexIndex = ReadU16(fp);
		m->lastVertexIndex = ReadU16(fp);

		m->un5 = ReadU16(fp);
		m->un6 = ReadU16(fp);
		m->un7 = ReadU16(fp);
		m->un8 = ReadU16(fp);
	}
}

void CMapReader::ReadBlockG(FILE *fp)
{
	m_blockG = new SBlockG[m_header.numBlockG];
	memset(m_blockG, 0, sizeof(SBlockG)*m_header.numBlockG);

	SBlockG *b = m_blockG;
	for(u32 i=0; i<m_header.numBlockG; i++, b++)
	{
		b->un0 = ReadU16(fp);
		b->un1 = ReadU16(fp);
		b->un2 = ReadU16(fp);
	}
}

void CMapReader::ReadBlockH(FILE *fp)
{
	m_blockH = new SBlockH[m_header.numBlockH];
	memset(m_blockH, 0, sizeof(SBlockH)*m_header.numBlockH);

	SBlockH *b = m_blockH;
	for(u32 i=0; i<m_header.numBlockH; i++, b++)
	{
		b->un0 = ReadU16(fp);
		b->un1 = ReadU16(fp);
		b->un2 = ReadU16(fp);
		b->un3 = ReadU16(fp);
		b->un4 = ReadU16(fp);
		b->un5 = ReadU16(fp);
		b->un6 = ReadU16(fp);
		b->un7 = ReadU16(fp);
		b->un8 = ReadU16(fp);
		b->un9 = ReadU16(fp);
		b->un10 = ReadU16(fp);
		b->un11 = ReadU16(fp);
		b->un12 = ReadU16(fp);
		b->un13 = ReadU16(fp);
		b->un14 = ReadU16(fp);
		b->un15 = ReadU16(fp);
	}
}

void CMapReader::ReadBlockI(FILE *fp)
{
	m_blockI = new SBlockI[m_header.numBlockI];
	memset(m_blockI, 0, sizeof(SBlockI)*m_header.numBlockI);

	SBlockI *b = m_blockI;
	for(u32 i=0; i<m_header.numBlockI; i++, b++)
	{
		b->un0 = ReadU16(fp);
		b->un1 = ReadU16(fp);
		b->un2 = ReadU16(fp);
		b->un3 = ReadU16(fp);
		b->un4 = ReadU16(fp);
		b->un5 = ReadU16(fp);
		b->un6 = ReadU16(fp);
		b->un7 = ReadU16(fp);
	}
}

void CMapReader::ReadQuadFaces(FILE *fp)
{
	m_quadFaces = new SQuadFace[m_header.numQuadFaces];
	memset(m_quadFaces, 0, sizeof(SQuadFace)*m_header.numQuadFaces);

	SQuadFace *f = m_quadFaces;
	for(u32 i=0; i<m_header.numQuadFaces; i++, f++)
	{
		f->v0 = ReadU16(fp);
		f->v1 = ReadU16(fp);
		f->v2 = ReadU16(fp);
		f->v3 = ReadU16(fp);

		f->uvFace = ReadU16(fp);

		f->un0 = ReadU16(fp);
		f->un1 = ReadU16(fp);
		f->un2 = ReadU16(fp);
		f->un3 = ReadU16(fp);
		f->un4 = ReadU16(fp);
		f->un5 = ReadU16(fp);
		f->un6 = ReadU16(fp);
		f->un7 = ReadU16(fp);
		f->un8 = ReadU16(fp);
		f->un9 = ReadU16(fp);
		f->un10 = ReadU16(fp);
		f->un11 = ReadU16(fp);
		f->un12 = ReadU16(fp);
		f->un13 = ReadU16(fp);
		f->un14 = ReadU16(fp);
	}
}

void CMapReader::ReadBlockK(FILE *fp)
{
	m_blockK = new SBlockK[m_header.numBlockK];
	memset(m_blockK, 0, sizeof(SBlockK)*m_header.numBlockK);

	SBlockK *b = m_blockK;
	for(u32 i=0; i<m_header.numBlockK; i++, b++)
	{
		for(u32 j=0; j<27; j++)
			b->un[j] = ReadU16(fp);
	}
}

void CMapReader::ReadNavipoints(FILE *fp)
{
	m_navipoints = new SNavipoint[m_header.numNavipoints];
	memset(m_navipoints, 0, sizeof(SNavipoint)*m_header.numNavipoints);

	SNavipoint *p = m_navipoints;
	for(u32 i=0; i<m_header.numNavipoints; i++, p++)
	{
		p->posX = ReadS16(fp);
		p->posY = ReadS16(fp);
		p->posZ = ReadS16(fp);

		p->un0 = ReadU16(fp);
		p->un1 = ReadU16(fp);
		p->un2 = ReadU16(fp);
		p->un3 = ReadU16(fp);
		p->un4 = ReadU16(fp);
		p->un5 = ReadU16(fp);
		p->un6 = ReadU16(fp);
		p->un7 = ReadU16(fp);
		p->un8 = ReadU16(fp);
		p->un9 = ReadU16(fp);

		p->pad0[0] = ReadU16(fp);
		p->pad0[1] = ReadU16(fp);
		p->pad0[2] = ReadU16(fp);
		p->pad0[3] = ReadU16(fp);
		p->pad0[4] = ReadU16(fp);
	}
}

void CMapReader::ReadBlockM(FILE *fp)
{
	m_blockM = new SBlockM[m_header.numBlockM];
	memset(m_blockM, 0, sizeof(SBlockM)*m_header.numBlockM);

	SBlockM *b = m_blockM;
	for(u32 i=0; i<m_header.numBlockM; i++, b++)
	{
		for(u32 j=0; j<10; j++)
			b->un[j] = ReadU16(fp);

		for(u32 j=0; j<8; j++)
			b->pad[j] = ReadU16(fp);
	}
}

void CMapReader::ReadBlockN(FILE *fp)
{
	m_blockN = new SBlockN[m_header.numBlockN];
	memset(m_blockN, 0, sizeof(SBlockN)*m_header.numBlockN);

	SBlockN *p = m_blockN;
	for(u32 i=0; i<m_header.numBlockN; i++, p++)
	{
		p->un0 = ReadU16(fp);
		p->un1 = ReadU16(fp);
		p->un2 = ReadU16(fp);
	}
}

void CMapReader::ReadBlockO(FILE *fp)
{
	m_blockO = new SBlockO[m_header.numBlockO];
	memset(m_blockO, 0, sizeof(SBlockO)*m_header.numBlockO);

	SBlockO *p = m_blockO;
	for(u32 i=0; i<m_header.numBlockO; i++, p++)
	{
		p->pos0X = ReadS16(fp);
		p->pos0Y = ReadS16(fp);
		p->pos0Z = ReadS16(fp);
		p->pos1X = ReadS16(fp);
		p->pos1Y = ReadS16(fp);
		p->pos1Z = ReadS16(fp);
		p->un0 = ReadU16(fp);
	}
}

void CMapReader::ReadBlockP(FILE *fp)
{
	m_blockP = new SBlockP[m_header.numBlockP];
	memset(m_blockP, 0, sizeof(SBlockP)*m_header.numBlockP);

	SBlockP *p = m_blockP;
	for(u32 i=0; i<m_header.numBlockP; i++, p++)
	{
		p->un0 = ReadU16(fp);
		p->un1 = ReadU16(fp);
	}
}

void CMapReader::ReadBlockQ(FILE *fp)
{
	m_blockQ = new SBlockQ[m_header.numBlockQ];
	memset(m_blockQ, 0, sizeof(SBlockQ)*m_header.numBlockQ);

	SBlockQ *p = m_blockQ;
	for(u32 i=0; i<m_header.numBlockQ; i++, p++)
	{
		p->un0 = ReadU8(fp);
		p->un1 = ReadU8(fp);
	}
}

void CMapReader::ReadBlockR(FILE *fp)
{
	m_blockR = new SBlockR[m_header.numBlockR];
	memset(m_blockR, 0, sizeof(SBlockR)*m_header.numBlockR);

	SBlockR *p = m_blockR;
	for(u32 i=0; i<m_header.numBlockR; i++, p++)
	{
		p->un0 = ReadU16(fp);
		p->un1 = ReadU16(fp);
		p->un2 = ReadU16(fp);
		p->un3 = ReadU16(fp);
		p->un4 = ReadU16(fp);
		p->un5 = ReadU16(fp);
		p->un6 = ReadU16(fp);
		p->un7 = ReadU16(fp);
	}
}

bool CMapReader::CheckBlockEnd(u16 data,FILE *fp)
{
	if(ReadU16(fp) == data)
		return true;
	else
		return false;
}

void CMapReader::SkipExtraData(u16 num,u16 size,FILE *fp)
{
	fseek(fp,num*size,SEEK_CUR);
}
//------------------------------------------------------------




CMapReader *sw_maprd_read(const char *_szFilePath)
{
	FILE *fp;
	fp=fopen(_szFilePath,"rb");
	if(fp == 0)
	{
		return NULL;
	}

	CMapReader *mr = new CMapReader();
	if(mr->Read(fp) == true)
	{
		fclose(fp);

		return mr;
	}else
	{
		fclose(fp);
		delete mr;
		return NULL;
	}
}
