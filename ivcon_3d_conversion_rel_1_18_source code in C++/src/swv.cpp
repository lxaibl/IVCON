/******************************************************************************/
// Author:  Tomasz Lis
// Created: 02 Mar 2007

// Purpose:
//    Reads/writes Bullfrog Syndicate Wars 3D structures format - vehicles file primveh.obj
//    (must be renamed to *.swv)

// Comment:
//   Thanks to Marco Castrucci for his great job on reworking 3D data format from SW,
//   and well-written library (sw_vehrd) for reading vehicle files

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - Max vertices 65535
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

#include "sw_vehrd.h"
#include "stdarg.h"

/******************************************************************************/

bool IVCONV::swv_read_materials( )

/*****************************************************************************/
/*
Purpose:
    Creates materials with textures from Bullfrog's game Syndicate Wars
*/
{
  //Setup textures
  int base_txtr=texture_num;
  for (unsigned int i=0;i<6;i++)
  {
    texture[texture_num]=Texture();
    sprintf(texture[texture_num].name,"TEX%02uC.PNG",i);
    texture[texture_num].intense=1.0f;
    if ( debug )
        printf ( "  SW_READ_MATERIALS creating texture entry for %s\n",texture[texture_num].name);
    texture_num++;
    texture[texture_num]=Texture();
    sprintf(texture[texture_num].name,"TEX%02uM.PNG",i);
    texture[texture_num].intense=1.0f;
    texture_num++;
  }
  //Now materials
  for (unsigned int i=0;i<6;i++)
  {
    material[material_num]=Material();
    material[material_num].rgb=vec3(.5,.5,.5);
    material[material_num].ambient=vec3(.4,.4,.4);
    material[material_num].specular=vec3(.8,.8,.8);
    sprintf(material[material_num].name,"%s%02u",DEFAULT_MATERIAL_NAME,i);
    material[material_num].texture=base_txtr+2*i;
    material[material_num].mask_texture=base_txtr+2*i+1;
    material[material_num].alpha=0.0f;
    if ( debug )
        printf ( "  SW_READ_MATERIALS creating material entry %u\n",i);
    material_num++;
  }
}
/******************************************************************************/

bool IVCONV::swv_read_mesh ( unsigned int src_mesh,CVehiclesReader *vr,array<FaceUV> &face_uv,int car_mat,unsigned long vertex_num_base,unsigned long face_num_base )

/******************************************************************************/
/*
Purpose:
    Reads mesh for single vehicle in Syndicate Wars 3D structures format
*/
{
  //Create mesh for storing faces
  unsigned long mesh_idx=mesh_num;
  mesh[mesh_idx]=Mesh();
  sprintf( mesh[mesh_idx].name,"%s%03u","car",src_mesh);
  if ( debug )
      printf ( "  SWV_READ_MESH creating mesh %s\n",mesh[mesh_idx].name);
  mesh_num++;

  // Prepare for reading face data
  CVehiclesReader::SMesh *vr_mesh = &vr->m_meshes[src_mesh];

  {
      //upload tri faces
      unsigned long face_idx;
      unsigned long face_num_delta=vr_mesh->numTriFaces;
      unsigned long first_face=vr_mesh->firstTriFace;
      for (face_idx=first_face;face_idx<first_face+face_num_delta;face_idx++)
      {
          face[face_num]=Face();
          face[face_num].order=3;
          face[face_num].mesh=mesh_idx;
          //preparing vertices indexes
          unsigned long vertidx[3];
          vertidx[0]=vr->m_triFaces[face_idx].v0;
          vertidx[1]=vr->m_triFaces[face_idx].v1;
          vertidx[2]=vr->m_triFaces[face_idx].v2;
          unsigned int uv_face_idx = vr->m_triFaces[face_idx].uvFace;
          unsigned char texNum = (unsigned char)vr->m_triUVFaces[uv_face_idx].texture;
          face_uv[face_num-face_num_base]=FaceUV();
          face_uv[face_num-face_num_base].material=car_mat+texNum;
          face_uv[face_num-face_num_base].order=face[face_num].order;
          float uv[3][2];       //texcoords values
          uv[0][0] =       (float)vr->m_triUVFaces[uv_face_idx].u0 / 255.f;
          uv[0][1] = 1.f - (float)vr->m_triUVFaces[uv_face_idx].v0 / 255.f;
          uv[1][0] =       (float)vr->m_triUVFaces[uv_face_idx].u1 / 255.f;
          uv[1][1] = 1.f - (float)vr->m_triUVFaces[uv_face_idx].v1 / 255.f;
          uv[2][0] =       (float)vr->m_triUVFaces[uv_face_idx].u2 / 255.f;
          uv[2][1] = 1.f - (float)vr->m_triUVFaces[uv_face_idx].v2 / 255.f;
          unsigned int idxMap[3] = {0,1,2};
          for(unsigned int ivert=0;ivert<3;ivert++)
          {
              face_uv[face_num-face_num_base].tex_uv[ivert]=vec2(uv[idxMap[ivert]][0],uv[idxMap[ivert]][1]);
              face[face_num].vertices[ivert]=vertex_num_base+vertidx[idxMap[ivert]];
          }
          if ( debug )
              printf ( "  SWV_READ_MESH tri-face %lu created with vertices %lu %lu %lu\n",face_num,
                  face[face_num].vertices[0],face[face_num].vertices[1],face[face_num].vertices[2]);
          face_num++;
      }
  }

  {
      //upload quad faces
      unsigned long face_idx;
      unsigned long face_num_delta=vr_mesh->numQuadFaces;
      unsigned long first_face=vr_mesh->firstQuadFace;

      for (face_idx=first_face;face_idx<first_face+face_num_delta;face_idx++)
      {
          //filling trivial elements in face structure
          face[face_num]=Face();
          face[face_num].order=4;
          face[face_num].mesh=mesh_idx;
          face_uv[face_num-face_num_base]=FaceUV();
          face_uv[face_num-face_num_base].order=face[face_num].order;
          //preparing vertices indexes
          unsigned long vertidx[4];
          vertidx[0] = vr->m_quadFaces[face_idx].v0;
          vertidx[1] = vr->m_quadFaces[face_idx].v1;
          vertidx[2] = vr->m_quadFaces[face_idx].v2;
          vertidx[3] = vr->m_quadFaces[face_idx].v3;
          //preparing uv mapping coords
          unsigned int uv_face_idx = vr->m_quadFaces[face_idx].uvFace;
          unsigned char texNum = (unsigned char)vr->m_quadUVFaces[uv_face_idx].texture;
          float uv[4][2];       //texcoords values
          uv[0][0] =       (float)vr->m_quadUVFaces[uv_face_idx].u0 / 255.f;
          uv[0][1] = 1.f - (float)vr->m_quadUVFaces[uv_face_idx].v0 / 255.f;
          uv[1][0] =       (float)vr->m_quadUVFaces[uv_face_idx].u1 / 255.f;
          uv[1][1] = 1.f - (float)vr->m_quadUVFaces[uv_face_idx].v1 / 255.f;
          uv[2][0] =       (float)vr->m_quadUVFaces[uv_face_idx].u2 / 255.f;
          uv[2][1] = 1.f - (float)vr->m_quadUVFaces[uv_face_idx].v2 / 255.f;
          uv[3][0] =       (float)vr->m_quadUVFaces[uv_face_idx].u3 / 255.f;
          uv[3][1] = 1.f - (float)vr->m_quadUVFaces[uv_face_idx].v3 / 255.f;
          //filling vertices coords and uv mappings
          unsigned int idxMap[4] = {0,1,3,2};
          for(unsigned int ivert=0;ivert<4;ivert++)
          {
              face_uv[face_num-face_num_base].tex_uv[ivert]=vec2(uv[idxMap[ivert]][0],uv[idxMap[ivert]][1]);
              face[face_num].vertices[ivert]=vertex_num_base+vertidx[idxMap[ivert]];
          }
          face_uv[face_num-face_num_base].material=car_mat+texNum;
          if ( debug )
              printf ( "  SWV_READ_MESH quad-face %lu created with vertices %lu %lu %lu %lu\n",face_num,
                  face[face_num].vertices[0],face[face_num].vertices[1],face[face_num].vertices[2],face[face_num].vertices[3]);
          face_num++;
      }
  }
}
/******************************************************************************/

bool IVCONV::swv_read ( FILE *filein )

/*****************************************************************************/
/*
Purpose:
    Reads Bullfrog Syndicate Wars 3D structures format - vehicles file primveh.obj
    (must be renamed to *.swv)
*/
{
  //Read vehicle in reader
  CVehiclesReader *vr = new CVehiclesReader();
  vr->Read(filein);

  //Setup textures and materials
  int car_mat=material_num;
  swv_read_materials();

  //upload vertices
  unsigned long vertex_num_base=vertex_num;
  unsigned long vertex_num_delta=vr->m_header.numVertices;
  unsigned long vertex_idx;
  if ( debug )
      printf ( "  SWV_READ creating %lu vertices\n",vertex_num_delta);
  for (vertex_idx=0;vertex_idx<vertex_num_delta;vertex_idx++)
  {
      vertex[vertex_num]=Vertex();
      vertex[vertex_num].cor3[0] = (float)vr->m_vertices[ vertex_idx ].x;
      vertex[vertex_num].cor3[1] = (float)vr->m_vertices[ vertex_idx ].z;
      vertex[vertex_num].cor3[2] = (float)vr->m_vertices[ vertex_idx ].y;
      vertex_num++;
  }

  //Prepare structure for reading "per face" UV mapping
  array<FaceUV> face_uv;
  unsigned long face_num_base=face_num;

  //Reading meshes
  unsigned int src_mesh;
  unsigned int meshes_count=vr->m_header.numMeshes;
  for (src_mesh=0;src_mesh<meshes_count;src_mesh++)
      swv_read_mesh(src_mesh,vr,face_uv,car_mat,vertex_num_base,face_num_base);

  //Convert UV mapping to "per vertex"
  face_to_vertex_uv(face_uv,face_num_base,face_num-face_num_base);
  delete vr;
  return true;
}
/*****************************************************************************/

void ErrorMessage(const char *fmt,...)

/*****************************************************************************/
// Error Message
{
	char message[1024];
	va_list list;

	va_start(list,fmt);
	vsnprintf(message,1024,fmt,list);
	va_end(list);
    
    printf("    SW_xxxRD: %s\n",message);

/*
	switch( MessageBox(NULL,message,TEXT("ERROR"),MB_ABORTRETRYIGNORE|MB_ICONERROR|MB_TASKMODAL|MB_SETFOREGROUND) )
	{
		case IDABORT: raise(SIGABRT); break;
		case IDCANCEL: break;
		case IDRETRY: __asm { int 3 }; break;
	}*/
}

