/******************************************************************************/
// Author:  Tomasz Lis
// Created: 02 Mar 2007

// Purpose:
//    Reads/writes Bullfrog Syndicate Wars 3D structures format - map files
//    (must be renamed to *.swm)

// Comment:
//   Thanks to Marco Castrucci for his library (sw_maprd) for reading maps.

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - Max vertices per mesh 65535, total vertices unlimited
// - Max faces 65535, max face order 4
// - 32767 meshes supported
// - Max materials/textures 6
// - No support for colors or alpha; one texture per material
// - Texture UV supported (per-face)
// - No camera/view support
// - No Environment support
// - No shapes/lines support
// Format support note - writing:
// - No writing supported

//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

#include "sw_maprd.h"
#include "stdarg.h"

/******************************************************************************/

bool IVCONV::swm_read_mesh ( unsigned int src_mesh,CMapReader *mr,int mat_base)

/******************************************************************************/
/*
Purpose:
    Reads mesh for single building in Syndicate Wars 3D structures format
*/
{
  //Create mesh for storing faces
  unsigned long mesh_idx=mesh_num;
  mesh[mesh_idx]=Mesh();
  sprintf( mesh[mesh_idx].name,"%s%03u",DEFAULT_ITEM_NAME,src_mesh);
  if ( debug )
      printf ( "  SWM_READ_MESH: creating mesh %s\n",mesh[mesh_idx].name);
  mesh_num++;

  CMapReader::SMesh *mr_mesh = &mr->m_meshes[src_mesh];
  vec3 mesh_pos=vec3((float)mr_mesh->posX,(float)mr_mesh->posY,(float)mr_mesh->posZ);

  {   //upload tri faces
      unsigned long face_idx;
      unsigned long face_num_delta=mr_mesh->numTriFaces;
      unsigned long first_face=mr_mesh->firstTriFace;
      if ( debug )
        printf ( "    SWM_READ_MESH: reading %lu tri-faces\n",face_num_delta);
      for (face_idx=first_face;face_idx<first_face+face_num_delta;face_idx++)
      {
          //filling trivial elements in face structure
          face[face_num]=Face();
          face[face_num].order=3;
          face[face_num].mesh=mesh_idx;
          //preparing vertices indexes
          unsigned long vertidx[4];
          vertidx[0] = mr->m_triFaces[face_idx].v0;
          vertidx[1] = mr->m_triFaces[face_idx].v1;
          vertidx[2] = mr->m_triFaces[face_idx].v2;
          //preparing uv mapping coords
          unsigned int uv_face_idx = mr->m_triFaces[face_idx].uvFace;
          if (uv_face_idx >= mr->m_header.numTriUVFaces)
          {
            printf("    SWM_READ_MESH: Warning, mesh %u TriUVFace index %u is bigger than the number of TriUVFaces!\n",src_mesh,uv_face_idx);
            uv_face_idx = mr->m_header.numTriUVFaces-1;
          }
          unsigned char texNum = (unsigned char)mr->m_triUVFaces[uv_face_idx].texture;
          float uv[3][2];       //texcoords values
          uv[0][0] =       (float)mr->m_triUVFaces[uv_face_idx].u0 / 255.f;
          uv[0][1] = 1.f - (float)mr->m_triUVFaces[uv_face_idx].v0 / 255.f;
          uv[1][0] =       (float)mr->m_triUVFaces[uv_face_idx].u1 / 255.f;
          uv[1][1] = 1.f - (float)mr->m_triUVFaces[uv_face_idx].v1 / 255.f;
          uv[2][0] =       (float)mr->m_triUVFaces[uv_face_idx].u2 / 255.f;
          uv[2][1] = 1.f - (float)mr->m_triUVFaces[uv_face_idx].v2 / 255.f;
          //uploading vertices, filling vertices coords and uv mappings
          unsigned int idxMap[] = {0,1,2};
          for(unsigned int ivert=0;ivert<3;ivert++)
          {
              if(vertidx[ivert] >= mr->m_header.numVertices)
              {
                printf("  SWM_READ_MESH: Warning, mesh %u Vertex index %lu is bigger than the number of vertices!\n",src_mesh,vertidx[ivert]);
                vertidx[ivert] = mr->m_header.numVertices-1;
              }
              vertex[vertex_num]=Vertex();
              vertex[vertex_num].cor3[0] = mesh_pos[0] + (float)mr->m_vertices[ vertidx[ivert] ].x;
              vertex[vertex_num].cor3[1] = mesh_pos[2] + (float)mr->m_vertices[ vertidx[ivert] ].z;
              vertex[vertex_num].cor3[2] = mesh_pos[1] + (float)mr->m_vertices[ vertidx[ivert] ].y;
              vertex[vertex_num].material=mat_base+texNum;
              vertex[vertex_num].tex_uv=vec2(uv[ivert][0],uv[ivert][1]);
              face[face_num].vertices[idxMap[ivert]]=vertex_num;
              vertex_num++;
          }
          //if ( debug )
          //    printf ( "  SWM_READ_MESH tri-face %lu created with vertices %lu %lu %lu\n",face_num,
          //        face[face_num].vertices[0],face[face_num].vertices[1],face[face_num].vertices[2]);
          face_num++;
      }
  }
  {   //upload quad faces
      unsigned long face_idx;
      unsigned long face_num_delta=mr_mesh->numQuadFaces;
      unsigned long first_face=mr_mesh->firstQuadFace;
      if ( debug )
        printf ( "  SWM_READ_MESH: reading %lu quad-faces\n",face_num_delta);
      for (face_idx=first_face;face_idx<first_face+face_num_delta;face_idx++)
      {
          //filling trivial elements in face structure
          face[face_num]=Face();
          face[face_num].order=4;
          face[face_num].mesh=mesh_idx;
          //preparing vertices indexes
          unsigned long vertidx[4];
          vertidx[0] = mr->m_quadFaces[face_idx].v0;
          vertidx[1] = mr->m_quadFaces[face_idx].v1;
          vertidx[2] = mr->m_quadFaces[face_idx].v2;
          vertidx[3] = mr->m_quadFaces[face_idx].v3;
          //preparing uv mapping coords
          unsigned int uv_face_idx = mr->m_quadFaces[face_idx].uvFace;
          if (uv_face_idx >= mr->m_header.numQuadUVFaces)
          {
            printf("  SWM_READ_MESH: Warning, mesh %u QuadUVFace index %u is bigger than the number of QuadUVFaces!\n",src_mesh,uv_face_idx);
            uv_face_idx = mr->m_header.numQuadUVFaces-1;
          }
          unsigned char texNum = (unsigned char)mr->m_quadUVFaces[uv_face_idx].texture;
          float uv[4][2];       //texcoords values
          uv[0][0] =       (float)mr->m_quadUVFaces[uv_face_idx].u0 / 255.f;
          uv[0][1] = 1.f - (float)mr->m_quadUVFaces[uv_face_idx].v0 / 255.f;
          uv[1][0] =       (float)mr->m_quadUVFaces[uv_face_idx].u1 / 255.f;
          uv[1][1] = 1.f - (float)mr->m_quadUVFaces[uv_face_idx].v1 / 255.f;
          uv[2][0] =       (float)mr->m_quadUVFaces[uv_face_idx].u2 / 255.f;
          uv[2][1] = 1.f - (float)mr->m_quadUVFaces[uv_face_idx].v2 / 255.f;
          uv[3][0] =       (float)mr->m_quadUVFaces[uv_face_idx].u3 / 255.f;
          uv[3][1] = 1.f - (float)mr->m_quadUVFaces[uv_face_idx].v3 / 255.f;
          //uploading vertices, filling vertices coords and uv mappings
          unsigned int idxMap[4] = {0,1,3,2};
          for(unsigned int ivert=0;ivert<4;ivert++)
          {
              if(vertidx[ivert] >= mr->m_header.numVertices)
              {
                printf("  SWM_READ_MESH: Warning, mesh %u Vertex index %lu is bigger than the number of vertices!\n",src_mesh,vertidx[ivert]);
                vertidx[ivert] = mr->m_header.numVertices-1;
              }
              vertex[vertex_num]=Vertex();
              vertex[vertex_num].cor3[0] = mesh_pos[0] + (float)mr->m_vertices[ vertidx[ivert] ].x;
              vertex[vertex_num].cor3[1] = mesh_pos[2] + (float)mr->m_vertices[ vertidx[ivert] ].z;
              vertex[vertex_num].cor3[2] = mesh_pos[1] + (float)mr->m_vertices[ vertidx[ivert] ].y;
              vertex[vertex_num].material=mat_base+texNum;
              vertex[vertex_num].tex_uv=vec2(uv[ivert][0],uv[ivert][1]);
              face[face_num].vertices[idxMap[ivert]]=vertex_num;
              vertex_num++;
          }
          //if ( debug )
          //    printf ( "  SWM_READ_MESH quad-face %lu created with vertices %lu %lu %lu %lu\n",face_num,
          //        face[face_num].vertices[0],face[face_num].vertices[1],face[face_num].vertices[2],face[face_num].vertices[3]);
          face_num++;
      }
  }
  return true;
}

/******************************************************************************/

bool IVCONV::swm_read ( FILE *filein )

/*****************************************************************************/
/*
Purpose:
    Reads Bullfrog Syndicate Wars 3D structures format - map files
    (must be renamed to *.swm)
*/
{
  CMapReader *mr = new CMapReader();
  if(!(mr->Read(filein)))
  {
    delete mr;
    return false;
  }

  //Setup textures and materials
  int mat_base=material_num;
  swv_read_materials();

  //Create mesh for storing terrain faces
  unsigned long mesh_idx=mesh_num;
  mesh[mesh_idx]=Mesh();
  sprintf( mesh[mesh_idx].name,"%s%03u","trr",0);
  if ( debug )
      printf ( "  SWM_READ_TERRAIN: creating mesh %s\n",mesh[mesh_idx].name);
  mesh_num++;

  const int rows_num=128;
  const int cols_num=128;
  unsigned long vertex_num_base=vertex_num;
  if ( debug )
      printf ( "    SWM_READ_TERRAIN: creating %lu terrain vertices\n",(unsigned long)rows_num*(unsigned long)cols_num);
  { //importing terrain vertices
	float fMul = 256.f;	//map width and depth multiplier
	float fMulY = 8.f;	//map vertex height multiplier
    for(unsigned int r=0; r<rows_num; r++)		//rows
      for(unsigned int c=0; c<cols_num; c++)	//columns
      {
        vertex[vertex_num]=Vertex();
        vertex[vertex_num].cor3 = vec3(((float)c)*fMul,((float)r)*fMul,0.0);
		vertex[vertex_num].cor3[2] = (float)mr->m_terrainCells[r*128 + c].vertHeight * fMulY;
        vertex_num++;
      }
  }
  //Prepare structure for reading "per face" UV mapping
  array<FaceUV> face_uv;
  unsigned long face_num_base=face_num;
  if ( debug )
      printf ( "    SWM_READ_TERRAIN: creating %lu terrain faces\n",(unsigned long)(rows_num-1)*(unsigned long)(cols_num-1));
  { //creating terrain faces
    for(unsigned int r=0; r<(rows_num-1); r++)		//rows
      for(unsigned int c=0; c<(cols_num-1); c++)	//columns
      {
			//cell vertex ordering
			// 2----3
			// |    |
			// 1----0
        unsigned int face_uv_index_raw = mr->m_terrainCells[r*128 + c].uvFace;
        unsigned int face_uv_index = face_uv_index_raw & 0x3FFF;
        if(face_uv_index >= mr->m_header.numQuadUVFaces)
        { //this error is true only on the orbital maps, where these quads are occluded by the map objects
			face_uv_index = mr->m_header.numQuadUVFaces-1;
        }
        unsigned char texNum = mr->m_quadUVFaces[face_uv_index].texture;
        vec2 uv[4];
        uv[0][0] =       (float)mr->m_quadUVFaces[face_uv_index].u0 / 255.f;
        uv[0][1] = 1.f - (float)mr->m_quadUVFaces[face_uv_index].v0 / 255.f;
        uv[1][0] =       (float)mr->m_quadUVFaces[face_uv_index].u1 / 255.f;
        uv[1][1] = 1.f - (float)mr->m_quadUVFaces[face_uv_index].v1 / 255.f;
        uv[2][0] =       (float)mr->m_quadUVFaces[face_uv_index].u2 / 255.f;
        uv[2][1] = 1.f - (float)mr->m_quadUVFaces[face_uv_index].v2 / 255.f;
        uv[3][0] =       (float)mr->m_quadUVFaces[face_uv_index].u3 / 255.f;
        uv[3][1] = 1.f - (float)mr->m_quadUVFaces[face_uv_index].v3 / 255.f;

        unsigned int idxMap[4] = {3,2,1,0};
        face[face_num]=Face();
        face[face_num].vertices[idxMap[0]]=vertex_num_base + (r+1)*128+(c);
        face[face_num].vertices[idxMap[1]]=vertex_num_base + (r+1)*128+(c+1);
        face[face_num].vertices[idxMap[2]]=vertex_num_base + (r)*128+(c+1);
        face[face_num].vertices[idxMap[3]]=vertex_num_base + (r)*128+(c);
        face[face_num].order=4;
        face[face_num].mesh=mesh_idx;
        face_uv[face_num-face_num_base]=FaceUV();
        face_uv[face_num-face_num_base].material=mat_base+texNum;
        face_uv[face_num-face_num_base].order=face[face_num].order;
          for(unsigned int ivert=0;ivert<4;ivert++)
              face_uv[face_num-face_num_base].tex_uv[ivert]=vec2(uv[idxMap[ivert]][0],uv[idxMap[ivert]][1]);
        face_num++;
      }
  }
  //Convert terrain UV mapping to "per vertex"
  face_to_vertex_uv(face_uv,face_num_base,face_num-face_num_base);

  { //Exporting building and structure meshes
	for(mesh_idx=1; mesh_idx<mr->m_header.numMeshes; mesh_idx++)
      swm_read_mesh ( mesh_idx,mr,mat_base);
  }

  delete mr;
  return true;
}
/*****************************************************************************/
