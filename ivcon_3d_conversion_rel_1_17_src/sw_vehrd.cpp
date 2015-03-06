//--------------------------------------------------------------------------------------
// File: sw_vehrd.cpp
//
// Desc: Syndicate Wars Vehicle Reader class
//
// Written by Marco Castrucci <mmacleod@tiscali.it> in 2006
//--------------------------------------------------------------------------------------

#include "sw_vehrd.h"
#include <memory.h>
#include <assert.h>

extern void ErrorMessage(const char *fmt,...);

u8 ReadU8(FILE *fp)
{
	u8 data;
	
	if(fread(&data,sizeof(u8),1,fp) != 1)
	{
		ErrorMessage("u8 read error");
	}

	return data;
}

s8 ReadS8(FILE *fp)
{
	s8 data;
	
	if(fread(&data,sizeof(s8),1,fp) != 1)
	{
		ErrorMessage("s8 read error");
	}

	return data;
}

u16 ReadU16(FILE *fp)
{
	u16 data;
	
	if(fread(&data,sizeof(u16),1,fp) != 1)
	{
		ErrorMessage("u16 read error");
	}

	return data;
}

s16 ReadS16(FILE *fp)
{
	s16 data;
	
	if(fread(&data,sizeof(s16),1,fp) != 1)
	{
		ErrorMessage("s16 read error");
	}

	return data;
}


//------------------------------------------------------------
CVehiclesReader::CVehiclesReader()
{
	memset(&m_header, 0, sizeof(SHeader));
	m_vertices = NULL;
	m_triFaces = NULL;
	m_quadFaces = NULL;
	m_meshes = NULL;
	m_quadUVFaces = NULL;
	m_triUVFaces = NULL;
}

CVehiclesReader::~CVehiclesReader()
{
	if(m_vertices)	delete [] m_vertices;

	if(m_triFaces)	delete [] m_triFaces;

	if(m_quadFaces)	delete [] m_quadFaces;

	if(m_meshes)	delete [] m_meshes;

	if(m_quadUVFaces)	delete [] m_quadUVFaces;

	if(m_triUVFaces)	delete [] m_triUVFaces;
}

void CVehiclesReader::Read(FILE *fp)
{
	ReadHeader(fp);
	ReadVertices(fp);
	ReadTriFaces(fp);
	ReadQuadFaces(fp);
	ReadMeshes(fp);
	ReadQuadUVFaces(fp);
	ReadTriUVFaces(fp);
}

void CVehiclesReader::ReadHeader(FILE *fp)
{
	//syndicate wars primveh.obj header: 6600 0000 5C07 CD01 9904 2800 E603 6201 0000

	m_header.un0 = ReadU16(fp);
	m_header.un1 = ReadU16(fp);
	
	m_header.numVertices = ReadU16(fp);

	m_header.numTriFaces = ReadU16(fp);

	m_header.numQuadFaces = ReadU16(fp);
	
	m_header.numMeshes = ReadU16(fp);

	m_header.numQuadUVFaces = ReadU16(fp);
		
	m_header.numTriUVFaces = ReadU16(fp);

	m_header.pad0 = ReadU16(fp);
}

void CVehiclesReader::ReadVertices(FILE *fp)
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

void CVehiclesReader::ReadTriFaces(FILE *fp)
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

		f->pad0[0] = ReadU16(fp);
		f->pad0[1] = ReadU16(fp);
		f->pad0[2] = ReadU16(fp);

		f->un3 = ReadU16(fp);

		f->pad1[0] = ReadU16(fp);
		f->pad1[1] = ReadU16(fp);

		f->un4 = ReadU16(fp);

		f->pad2[0] = ReadU16(fp);
		f->pad2[1] = ReadU16(fp);
	}
}

void CVehiclesReader::ReadQuadFaces(FILE *fp)
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

		f->pad0[0] = ReadU16(fp);
		f->pad0[1] = ReadU16(fp);
		f->pad0[2] = ReadU16(fp);
		f->pad0[3] = ReadU16(fp);

		f->un3 = ReadU16(fp);

		f->pad1[0] = ReadU16(fp);
		f->pad1[1] = ReadU16(fp);
		f->pad1[2] = ReadU16(fp);
		
		f->un4 = ReadU16(fp);

		f->pad2[0] = ReadU16(fp);
		f->pad2[1] = ReadU16(fp);
		f->pad2[2] = ReadU16(fp);
	}
}

void CVehiclesReader::ReadMeshes(FILE *fp)
{
	m_meshes = new SMesh[m_header.numMeshes];
	memset(m_meshes, 0, sizeof(SMesh)*m_header.numMeshes);

	SMesh *m = m_meshes;
	for(u32 i=0; i<m_header.numMeshes; i++, m++)
	{
		m->firstTriFace = ReadU16(fp);
		m->numTriFaces = ReadU16(fp);

		m->pad0 = ReadU16(fp);;

		m->firstQuadFace = ReadU16(fp);
		m->numQuadFaces = ReadU16(fp);

		m->pad1[0] = ReadU16(fp);
		m->pad1[1] = ReadU16(fp);
		m->pad1[2] = ReadU16(fp);
		m->pad1[3] = ReadU16(fp);
		m->pad1[4] = ReadU16(fp);
		m->pad1[5] = ReadU16(fp);
		m->pad1[6] = ReadU16(fp);

		m->firstVertexIndex = ReadU16(fp);
		m->lastVertexIndex = ReadU16(fp);

		m->un0 = ReadU16(fp);

		m->pad2[0] = ReadU16(fp);
		m->pad2[1] = ReadU16(fp);
		m->pad2[2] = ReadU16(fp);
	}
}

void CVehiclesReader::ReadQuadUVFaces(FILE *fp)
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

void CVehiclesReader::ReadTriUVFaces(FILE *fp)
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
		
		f->un0 = ReadU16(fp);
	}
}
//------------------------------------------------------------


void __debug_out(CVehiclesReader *vr)
{
	FILE *fp;	fp=fopen("debug.txt","wt");

	fprintf(fp,"-------- FILE HEADER --------\n");
	fprintf(fp,"\tNum Meshes: %d\n",vr->m_header.numMeshes);
	fprintf(fp,"\tNum Vertices: %d\n",vr->m_header.numVertices);
	fprintf(fp,"\tNum Quad Faces: %d\n",vr->m_header.numQuadFaces);
	fprintf(fp,"\tNum QuadUV Faces: %d\n",vr->m_header.numQuadUVFaces);
	fprintf(fp,"\tNum Tri Faces: %d\n",vr->m_header.numTriFaces);
	fprintf(fp,"\tNum TriUV Faces: %d\n",vr->m_header.numTriUVFaces);
	fprintf(fp,"--------------------------------\n");

	fprintf(fp,"\n\n");

	fprintf(fp,"-------- MESH HEADERS --------\n");
	fprintf(fp,"[n]  [1st Tri] [n °Tri]   [1st Quad] [n° Quad]   [1st Vert] [last Vert]\n");
	for(u32 i=0; i<vr->m_header.numMeshes; i++)
	{
		CVehiclesReader::SMesh *m = &vr->m_meshes[i];

		fprintf(fp, "%2d     %4d     %4d       %4d       %4d         %4d      %4d\n", i, m->firstTriFace,m->numTriFaces, m->firstQuadFace,m->numQuadFaces, m->firstVertexIndex,m->lastVertexIndex);
	}
	fprintf(fp,"--------------------------------\n");

	fprintf(fp,"\n\n");

	fprintf(fp,"-------- VERTICES ------------\n");
	fprintf(fp,"  [n]    [un0]    [un1]\n");
	for(u32 i=0; i<vr->m_header.numVertices; i++)
	{
		CVehiclesReader::SVertex *v = &vr->m_vertices[i];

		fprintf(fp, "%4d     %4d     %4d\n", i, v->un0, v->un1);
	}
	fprintf(fp,"--------------------------------\n");

	fclose(fp);
}

CVehiclesReader *sw_vehrd_read(const char *_szFilePath)
{
	FILE *fp;
	fp=fopen(_szFilePath,"rb");

	if(fp == 0)
	{
		ErrorMessage("Cannot open vehicles file: %s",_szFilePath);
		return NULL;
	}

	CVehiclesReader *vr = new CVehiclesReader();
	vr->Read(fp);
	fclose(fp);

//	__debug_out(vr);

	return vr;
}
