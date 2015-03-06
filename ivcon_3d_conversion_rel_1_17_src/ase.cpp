/******************************************************************************/
// Author:  John Burkardt 
// Change: 04 Mar 2007 Tomasz Lis adjusted to new core

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - 31-bit vertices support
// - 31-bit faces support, max face order 4
// - No meshes support
// - Full 32-bit materials support, per vertex
// - No textures
// - No texture UV
// - No camera/view support
// - No Environment or lights support
// - No shapes/lines support
// Format support note - writing:
// - Full 32-bit vertices support
// - Full 32-bit faces support, max face order 26
// - Full meshes support
// - Full 32-bit materials/textures support
// - Material support - colors, alpha, two textures per material
// - Texture UV supported (per vertex)
// - Cameras supported, no default view support
// - Environment and lights supported
// - No shapes/lines support

//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

struct ASE_READER
{
    //current level in hierarchical file structure
    long level;
    //names of levels from top to current
    array<NameItm> llevel;
    //num of faces before the reading
    unsigned long face_num_old;
    unsigned long vertex_num_old;
    // variables used to read normal vertices at right indexes
    int   iface_norm;
    int   ivert_norm;
    //parameters for parsing line readed from file
	int   nlbrack;
	int   nrbrack;
	char  word[LINE_MAX_LEN];
    char *next;
    ASE_READER(){ level=0;llevel[0]=NameItm();
                  nlbrack=0;nrbrack=0;face_num_old=0;
                  strcpy(word," ");next=word; };
};
/******************************************************************************/

bool IVCONV::ase_read_cfacelist_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_cfacelist_property identifies one line in CFACELIST subsection
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
				if ( leqi( rd.word, "{" ) )
                {
					return true;
				} else

				if ( leqi( rd.word, "}" ) )
                {
					rd.level = rd.nlbrack - rd.nrbrack;
					return true;
				} else

				if ( leqi( rd.word, "*MESH_CFACE" ) )
                {
                    if (debug)
                      printf ( "  ASE_READ_CFACELIST: MESH_CFACE tag found at line %lu\n",stats.text_num );
					return false;
				} else

				{
					stats.bad_num++;
                    if (debug)
                      printf ( "  ASE_READ_CFACELIST: Warning, bad data in MESH_CFACE at line %lu\n",stats.text_num );
					return false;
				}
}
/******************************************************************************/

bool IVCONV::ase_read_cvertlist_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_cvertlist_property identifies one line in CVERTLIST subsection
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
				if ( leqi( rd.word, "{" ) )
                {
					return true;
				} else

				if ( leqi( rd.word, "}" ) )
                {
					rd.level = rd.nlbrack - rd.nrbrack;
					return true;
				} else

				if ( leqi( rd.word, "*MESH_VERTCOL" ) )
                {
                    int   count=0;
                    int   width=0;
                    int   i=0;
					count += sscanf ( rd.next, "%d%n", &i, &width );
					rd.next += width;

					unsigned long vertidx = i + rd.vertex_num_old;
                    float rval=0.5;
                    float gval=0.5;
                    float bval=0.5;
					count += sscanf ( rd.next, "%f%n", &rval, &width );
					rd.next += width;
					
					count += sscanf ( rd.next, "%f%n", &gval, &width );
					rd.next += width;
					
					count += sscanf ( rd.next, "%f%n", &bval, &width );
					rd.next += width;
					
					if ((count==4) && (vertidx<vertex_num))
					{
                      vertex[vertidx].rgb = vec3(rval,gval,bval);
                    } else
                    {
                      if (debug)
                        printf ( "  ASE_READ_CVERTLIST: Warning, wrong parameters in color for vertex at line %lu\n",stats.text_num );
                      stats.bad_num++;
                    }
				} else

				{
					stats.bad_num++;
                    if (debug)
                      printf ( "  ASE_READ_CVERTLIST: Warning, bad data in MESH_CVERTLIST at line %lu\n",stats.text_num );
					return false;
				}
}
/******************************************************************************/

bool IVCONV::ase_read_tfacelist_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_tfacelist_property identifies one line in TFACELIST subsection
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
				if ( leqi( rd.word, "{" ) )
                {
					return true;
				} else

				if ( leqi( rd.word, "}" ) )
                {
					rd.level = rd.nlbrack - rd.nrbrack;
					return true;
				} else

				if ( leqi( rd.word, "*MESH_TFACE" ) )
                {
                    int   count=0;
                    int   width=0;
                    int   i=0;
					count += sscanf ( rd.next, "%d%n", &i, &width );
					rd.next += width;
					unsigned long faceidx = i + rd.face_num_old;
                    if ((faceidx >= face_num) || (count!=1))
                    {
                      stats.bad_num++;
                      if (debug)
                        printf ( "  ASE_READ_TFACELIST: Warning, wrong face index in MESH_TFACE section at line %lu\n",stats.text_num  );
                    }
                    //Skipping the line data
					return false;
				} else

				{
					stats.bad_num++;
                    if (debug)
                      printf ( "  ASE_READ_TFACELIST: Warning, bad data in MESH_TFACE_LIST at line %lu\n",stats.text_num );
					return false;
				}
}
/******************************************************************************/

bool IVCONV::ase_read_tvertlist_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_tvertlist_property identifies one line in TVERTLIST subsection
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
				if ( leqi( rd.word, "{" ) )
                {
					return true;
				} else

				if ( leqi( rd.word, "}" ) )
                {
					rd.level = rd.nlbrack - rd.nrbrack;
					return true;
				} else

				if ( leqi( rd.word, "*MESH_TVERT" )  )
                {
                    int   count=0;
                    int   width=0;
                    int   i=0;
					count += sscanf ( rd.next, "%d%n", &i, &width );
					rd.next += width;
					unsigned long faceidx = i + rd.face_num_old;
                    if ((faceidx >= face_num) || (count!=1))
                    {
                      stats.bad_num++;
                      if (debug)
                        printf ( "  ASE_READ_TVERTLIST: Warning, wrong face index %lu in MESH_TVERT section at line %lu\n",faceidx,stats.text_num  );
                      return false;
                    }
                    //Skipping the line data
					return false;
				} else

				{
					stats.bad_num++;
                    if (debug)
                      printf ( "  ASE_READ_TVERTLIST: Warning, bad data in MESH_TVERTLIST at line %lu\n",stats.text_num );
					return false;
				}
}
/******************************************************************************/

bool IVCONV::ase_read_facelist_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_facelist_property identifies one line in face list subsection
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
                //if (debug)
                //    printf ( "  ASE_READ_FACELIST: Identifying property \"%s\" at line %lu\n",rd.word,stats.text_num );
				if ( leqi( rd.word, "{" ) )
                {
					return true;
				} else

				if ( leqi( rd.word, "}" ) )
                {
					rd.level = rd.nlbrack - rd.nrbrack;
					return true;
				} else

				if ( leqi( rd.word, "*MESH_FACE" ) )
                {
					if (face_num>FACES_MAX)
					{
                        stats.bad_num++;
                        if (debug)
                          printf ( "  ASE_READ_FACELIST: Warning, faces limit exceeded; face definition skipped\n" );
                    }
                    face[face_num]=Face();
					unsigned long order = 0;

                        int   count=0;
                        int   width=0;
                        int   i=0;
                        char  word2[LINE_MAX_LEN];
						
						count += sscanf ( rd.next, "%d%n", &i, &width );
						rd.next = rd.next + width;

                        if ((i+rd.face_num_old) != face_num)
                        {
                          stats.bad_num++;
                          if (debug)
                          {
                            printf ( "  ASE_READ_FACELIST: Reading MESH_FACE indexed %d into face %lu\n",i,face_num );
                            printf ( "  ASE_READ_FACELIST: Warning, face %lu index has changed!\n",face_num );
                          }
                        }
						count += sscanf ( rd.next, "%s%n", word2, &width );
						rd.next = rd.next + width;
						count += sscanf ( rd.next, "%s%n", word2, &width );
						rd.next = rd.next + width;
						
						count += sscanf ( rd.next, "%d%n", &i, &width );
						rd.next = rd.next + width;
						unsigned long vertidx = i + rd.vertex_num_old;
                        face[face_num].vertices[order] = vertidx;
                        order++;
						
						count += sscanf ( rd.next, "%s%n", word2, &width );
						rd.next = rd.next + width;
						
						count += sscanf ( rd.next, "%d%n", &i, &width );
						rd.next = rd.next + width;
						face[face_num].vertices[order] = i + rd.vertex_num_old;
						order++;
						
						count += sscanf ( rd.next, "%s%n", word2, &width );
						rd.next = rd.next + width;
						
						count += sscanf ( rd.next, "%d%n", &i, &width );
						rd.next = rd.next + width;
						face[face_num].vertices[order] = i + rd.vertex_num_old;
						order++;
						
						count += sscanf ( rd.next, "%s%n", word2, &width );
						rd.next = rd.next + width;
						
						if ( leqi( word2, "D:" ) )
                        {
							count += sscanf ( rd.next, "%d%n", &i, &width );
							rd.next = rd.next + width;
							face[face_num].vertices[order] = i + rd.vertex_num_old;
							order++;
						}
					if ((count==9) || (count==10))
					{
                      face[face_num].order=order;
                    } else
					{
                      if (debug)
                        printf ( "  ASE_READ_FACELIST: Warning, wrong parameters count in face definition at line %lu\n",stats.text_num );
                      stats.bad_num++;
                      face[face_num].order=0; //mark bad face for deletion
                    }
                    face_num++;
					return false;
				} else

				{
					stats.bad_num++;
                    if (debug)
                      printf ( "  ASE_READ_FACELIST: Warning, bad data in MESH_FACE_LIST at line %lu\n",stats.text_num );
					return false;
				}
}
/******************************************************************************/

bool IVCONV::ase_read_vertxlist_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_vertxlist_property identifies one line in vertex list subsection
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
				if ( leqi( rd.word, "{" ) )
                {
					rd.vertex_num_old = vertex_num;
					return true;
				} else

				if ( leqi( rd.word, "}" ) )
                {
					rd.level = rd.nlbrack - rd.nrbrack;
					return true;
				} else

				if ( leqi( rd.word, "*MESH_VERTEX" ) )
                {
                    int   count=0;
                    int   width=0;
                    int   i=0;
                    float x=0.0,y=0.0,z=0.0;
					count += sscanf ( rd.next, "%d%n", &i, &width );
					rd.next = rd.next + width;
					
					count += sscanf ( rd.next, "%f%n", &x, &width );
					rd.next = rd.next + width;
					
					count += sscanf ( rd.next, "%f%n", &y, &width );
					rd.next = rd.next + width;
					
					count += sscanf ( rd.next, "%f%n", &z, &width );
					rd.next = rd.next + width;
					
					unsigned long vertidx = i + rd.vertex_num_old;
					
                    if (vertidx < VERTICES_MAX)
                    {
                       while (vertex_num <= vertidx)
                       {
                          vertex[vertex_num]=Vertex();
                          vertex_num++;
                       }
						vertex[i].cor3[0] =
							  transform_matrix[0][0] * x + transform_matrix[0][1] * y 
							+ transform_matrix[0][2] * z + transform_matrix[0][3];
						vertex[i].cor3[1] =
							  transform_matrix[1][0] * x + transform_matrix[1][1] * y 
							+ transform_matrix[1][2] * z + transform_matrix[1][3];
						vertex[i].cor3[2] =
							  transform_matrix[2][0] * x + transform_matrix[2][1] * y 
							+ transform_matrix[2][2] * z + transform_matrix[2][3];
                    } else
					{
                      if (debug)
                        printf ( "  ASE_READ_VERTXLIST: Warning, vertex limit exceeded, vertex not loaded.\n" );
                      stats.bad_num++;
                    }

					if ((count!=4))
					{
                      if (debug)
                        printf ( "  ASE_READ_VERTXLIST: Warning, wrong parameters count in vertex definition at line %lu\n",stats.text_num );
                      stats.bad_num++;
                    }
					return false;
				} else

				{
					stats.bad_num++;
                    if (debug)
                      printf ( "ASE_READ_VERTXLIST: Warning, bad data in MESH_VERTEX_LIST at line %lu\n",stats.text_num );
					return false;
				}
}
/******************************************************************************/

bool IVCONV::ase_read_meshnormals_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_meshnormals_property identifies one line in mesh normals subsection
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
				if ( leqi( rd.word, "{" ) )
                {
					return true;
				} else

				if ( leqi( rd.word, "}" ) )
                {
					rd.level = rd.nlbrack - rd.nrbrack;
					return true;
				} else

				if ( leqi( rd.word, "*MESH_FACENORMAL" ) )
                {
                    int   count=0;
                    int   width=0;
                    float x=0.0,y=0.0,z=0.0;
					count += sscanf ( rd.next, "%d%n", &(rd.iface_norm), &width );
					rd.next = rd.next + width;
					
					count += sscanf ( rd.next, "%f%n", &x, &width );
					rd.next = rd.next + width;
					
					count += sscanf ( rd.next, "%f%n", &y, &width );
					rd.next = rd.next + width;
					
					count += sscanf ( rd.next, "%f%n", &z, &width );
					rd.next = rd.next + width;

					rd.iface_norm += rd.face_num_old;
					rd.ivert_norm = 0; //for reading vertices of this face
					
					if (rd.iface_norm<face_num)
					{
                        face[rd.iface_norm].normal[0] = x;
                        face[rd.iface_norm].normal[1] = y;
                        face[rd.iface_norm].normal[2] = z;
                    } else
                    {
                      if (debug)
                        printf ( "  ASE_READ_MESHNORMALS: Warning, trying to read non-existing face normal at line %lu\n",stats.text_num );
                    }

					if ((count!=4))
					{
                      if (debug)
                        printf ( "  ASE_READ_MESHNORMALS: Warning, wrong parameters count in face normal vector definition at line %lu\n",stats.text_num );
                      stats.bad_num++;
                    }
					return false;
				} else

				if ( leqi( rd.word, "*MESH_VERTEXNORMAL" ) )
                {
                    int   count=0;
                    int   width=0;
                    int   i=0;
                    float x=0.0,y=0.0,z=0.0;
					count += sscanf ( rd.next, "%d%n", &i, &width );
					rd.next = rd.next + width;
					
					count += sscanf ( rd.next, "%f%n", &x, &width );
					rd.next = rd.next + width;
					
					count += sscanf ( rd.next, "%f%n", &y, &width );
					rd.next = rd.next + width;
					
					count += sscanf ( rd.next, "%f%n", &z, &width );
					rd.next = rd.next + width;
					
					unsigned long vertidx = 0;
                    if ( (rd.iface_norm<face_num) && (rd.ivert_norm<face[rd.iface_norm].order) )
                    {
                        vertidx = face[rd.iface_norm].vertices[rd.ivert_norm];
                        if (vertidx>vertex_num)
                        {
                            vertex[vertidx].normal[0] = x;
                            vertex[vertidx].normal[1] = y;
                            vertex[vertidx].normal[2] = z;
                        }
                        rd.ivert_norm++;
                        if ((count!=4))
                        {
                          if (debug)
                            printf ( "  ASE_READ_MESHNORMALS: Warning, wrong parameters count in vertex normal vector definition at line %lu\n",stats.text_num );
                          stats.bad_num++;
                        }
                    } else
					{
                      if (debug)
                        printf ( "  ASE_READ_MESHNORMALS: Warning, outranged index in normal vector definition at line %lu\n",stats.text_num );
                      stats.bad_num++;
                    }
					return false;
				} else

				{
					stats.bad_num++;
                    if (debug)
                      printf ( "ASE_READ_MESHNORMALS: Warning, bad data in MESH_NORMALS at line %lu\n",stats.text_num );
					return false;
				}
}
/******************************************************************************/

bool IVCONV::ase_read_scene_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_scene_property identifies one line in SCENE subsection
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
		  if ( leqi( rd.word, "{" ) )
          {
			  return true;
		  } else
		  if ( leqi( rd.word, "}" ) )
          {
			  rd.level = rd.nlbrack - rd.nrbrack;
			  return true;
		  } else
		  if ( leqi( rd.word, "*SCENE_AMBIENT_STATIC" ) )
          {
			  return false;
		  } else
		  if ( leqi( rd.word, "*SCENE_BACKGROUND_STATIC" ) )
          {
              if (debug)
                printf ( "  ASE_READ_SCENE: Found BACKGROUND_STATIC tag at line %lu\n",stats.text_num );
			  return false;
	      } else
		  if ( leqi( rd.word, "*SCENE_FILENAME" ) )
          {
              if (debug)
                printf ( "  ASE_READ_SCENE: Found FILENAME tag at line %lu\n",stats.text_num );
			  return false;
		  } else
		  if ( leqi( rd.word, "*SCENE_FIRSTFRAME" ) )
          {
			  return false;
		  } else
		  if ( leqi( rd.word, "*SCENE_FRAMESPEED" ) )
          {
			  return false;
		  } else
		  if ( leqi( rd.word, "*SCENE_LASTFRAME" ) )
          {
			  return false;
		  } else
		  if ( leqi( rd.word, "*SCENE_TICKSPERFRAME" ) )
          {
			  return false;
		  } else
		  {
			  stats.bad_num++;
              if (debug)
                printf ( "  ASE_READ_SCENE: Warning, bad data in SCENE tag at line %lu\n",stats.text_num );
			  return false;
		  }

}
/******************************************************************************/

bool IVCONV::ase_read_nodetm_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_nodetm_property identifies one line in NODETM subsection
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
				if ( leqi( rd.word, "{" ) )
                {
					tmat_init ( transform_matrix );
					return true;
				} else
				if ( leqi( rd.word, "}" ) )
                {
					rd.level = rd.nlbrack - rd.nrbrack;
					return true;
				} else
				if ( leqi( rd.word, "*INHERIT_POS" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*INHERIT_ROT" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*INHERIT_SCL" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*NODE_NAME" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*TM_POS" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*TM_ROTANGLE" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*TM_ROTAXIS" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*TM_ROW0" ) )
                {
                    int   count=0;
                    float temp=0.0;
                    int   width=0;
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[0][0] = temp;

					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[1][0] = temp;
					
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[2][0] = temp;

					if ((count!=3))
					{
                      if (debug)
                        printf ( "  ASE_READ_NODETM: Warning, wrong parameters count in transform matrix definition at line %lu\n",stats.text_num );
                      stats.bad_num++;
                    }
					return false;
				} else
				if ( leqi( rd.word, "*TM_ROW1" ) )
                {
                    int   count=0;
                    float temp=0.0;
                    int   width=0;
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[0][1] = temp;
					
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[1][1] = temp;
					
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[2][1] = temp;

					if ((count!=3))
					{
                      if (debug)
                        printf ( "  ASE_READ_NODETM: Warning, wrong parameters count in transform matrix definition at line %lu\n",stats.text_num );
                      stats.bad_num++;
                    }
					return false;
				} else
				if ( leqi( rd.word, "*TM_ROW2" ) )
                {
                    int   count=0;
                    float temp=0.0;
                    int   width=0;
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[0][2] = temp;
					
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[1][2] = temp;
					
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[2][2] = temp;

					if ((count!=3))
					{
                      if (debug)
                        printf ( "  ASE_READ_NODETM: Warning, wrong parameters count in transform matrix definition at line %lu\n",stats.text_num );
                      stats.bad_num++;
                    }
					return false;
				} else

				if ( leqi( rd.word, "*TM_ROW3" ) )
                {
                    int   count=0;
                    float temp=0.0;
                    int   width=0;
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[0][3] = temp;
					
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[1][3] = temp;
					
					count += sscanf ( rd.next, "%f%n", &temp, &width );
					rd.next += width;
					transform_matrix[2][3] = temp;

					if ((count!=3))
					{
                      if (debug)
                        printf ( "  ASE_READ_NODETM: Warning, wrong parameters count in transform matrix definition at line %lu\n",stats.text_num );
                      stats.bad_num++;
                    }
					return false;
				} else
				if ( leqi( rd.word, "*TM_SCALE" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*TM_SCALEAXIS" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*TM_SCALEAXISANG" ) )
                {
					return false;
				} else
				{
					stats.bad_num++;
                    if (debug)
                      printf ( "  ASE_READ_NODETM: Warning, bad data in NODE_TM tag at line %lu\n",stats.text_num );
					return false;
				}
}
/******************************************************************************/

bool IVCONV::ase_read_mesh_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_mesh_property identifies one line in GEOMOBJECTs MESH subsection
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
				if ( leqi( rd.word, "{" ) )
                {
					return true;
				} else

				if ( leqi( rd.word, "}" ) )
                {
					rd.level = rd.nlbrack - rd.nrbrack;
					return true;
				} else

				if ( leqi( rd.word, "*MESH_CFACELIST" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found MESH_CFACELIST tag at line %lu\n",stats.text_num );
					return true;
				} else
				if ( leqi( rd.word, "*MESH_CVERTLIST" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found MESH_CVERTLIST tag at line %lu\n",stats.text_num );
					return true;
				} else
				if ( leqi( rd.word, "*MESH_FACE_LIST" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found MESH_FACE_LIST tag at line %lu\n",stats.text_num );
					return true;
				} else
				if ( leqi( rd.word, "*MESH_NORMALS" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found MESH_NORMALS tag at line %lu\n",stats.text_num );
					return true;
				} else
				if ( leqi( rd.word, "*MESH_NUMCVERTEX" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*MESH_NUMCVFACES" ) )
                {
					return false;
				} else
				if ( leqi( rd.word, "*MESH_NUMFACES" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found NUMFACES tag at line %lu\n",stats.text_num );
					return false;
				} else
				if ( leqi( rd.word, "*MESH_NUMTVERTEX" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found NUMTVERTEX tag at line %lu\n",stats.text_num );
					return false;
				} else
				if ( leqi( rd.word, "*MESH_NUMTVFACES" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found NUMTVFACE tag at line %lu\n",stats.text_num );
					return false;
				} else
				if ( leqi( rd.word, "*MESH_NUMVERTEX" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found NUMVERTEX tag at line %lu\n",stats.text_num );
					return false;
				} else
				if ( leqi( rd.word, "*MESH_TFACELIST" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found TFACELIST tag at line %lu\n",stats.text_num );
					return true;
				} else
				if ( leqi( rd.word, "*MESH_TVERTLIST" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found TVERTLIST tag at line %lu\n",stats.text_num );
					return true;
				} else
				if ( leqi( rd.word, "*MESH_VERTEX_LIST" ) )
                {
                    if (debug)
                      printf ( "    ASE_READ_MESH: Found VERTEX_LIST tag at line %lu\n",stats.text_num );
					return true;
				} else
				if ( leqi( rd.word, "*TIMEVALUE" ) )
                {
					return false;
				} else
				{
					stats.bad_num++;
                    if (debug)
                      printf ( "    ASE_READ_MESH: Warning, bad data in MESH tag at line %lu\n",stats.text_num );
					return false;
				}
}
/******************************************************************************/

bool IVCONV::ase_read_geomobject_property(ASE_READER &rd)

/******************************************************************************/
/*
Purpose:
  ase_read_geomobject_property identifies one line in GEOMOBJECT section
    of AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis created from ASE_READ and improved
*/
{
				if ( leqi( rd.word, "{" ) )
                {
					return true;
				} else
				if ( leqi( rd.word, "}" ) )
                {
					rd.level = rd.nlbrack - rd.nrbrack;
					return true;
				} else
				//Here you can add reading GEOMOBJECT names
				if ( leqi( rd.word, "*NODE_NAME" ) )
                {
                    if (debug)
                      printf ( "  ASE_READ_GEOMOBJECT: Found NODE_NAME tag at line %lu\n",stats.text_num );
					return false;
				} else
				if ( leqi( rd.word, "*NODE_TM" ) )
                {
                    if (debug)
                      printf ( "  ASE_READ_GEOMOBJECT: Found NODE_TM tag at line %lu\n",stats.text_num );
					return true;
				} else
				if ( leqi( rd.word, "*MESH" ) )
                {
					//new geomobject (or new mesh) has new face/vertex numbering
                    rd.vertex_num_old = vertex_num;
                    rd.face_num_old = face_num;
                    if (debug)
                    {
                      printf ( "  ASE_READ_GEOMOBJECT: New MESH started at line %lu,\n",stats.text_num );
                      printf ( "          faces will be appended at index %lu\n",rd.face_num_old );
                    }
                    //Assuming that *MESH section is on the same level that this one
                    //  gives us better chances of properly reading the file
					return true;
				} else
				if ( leqi( rd.word, "*PROP_CASTSHADOW" ) )
                {
                    if (debug)
                      printf ( "  ASE_READ_GEOMOBJECT: Found CASTSHADOW tag at line %lu\n",stats.text_num );
					return false;
				} else
				if ( leqi( rd.word, "*PROP_MOTIONBLUR" ) )
                {
                    if (debug)
                      printf ( "  ASE_READ_GEOMOBJECT: Found PROP_MOTIONBLUR tag at line %lu\n",stats.text_num );
					return false;
				} else
				if ( leqi( rd.word, "*PROP_RECVSHADOW" ) )
                {
                    if (debug)
                      printf ( "  ASE_READ_GEOMOBJECT: Found PROP_RECVSHADOW tag at line %lu\n",stats.text_num );
					return false;
				} else
				if ( leqi( rd.word, "*MATERIAL_REF" ) )
                {
                    if (debug)
                      printf ( "  ASE_READ_GEOMOBJECT: Found MATERIAL_REF tag at line %lu\n",stats.text_num );
					return false;
				} else
				{
					stats.bad_num++;
                    if (debug)
                      printf ( "  ASE_READ_GEOMOBJECT: Warning, bad data in GEOMOBJECT tag at line %lu\n",stats.text_num );
					return false;
				}
}
/******************************************************************************/

bool IVCONV::ase_read ( FileIO *filein )

/******************************************************************************/
/*
Purpose:
  ASE_READ reads an AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 05 Mar 2007 Tomasz Lis divided into smaller functions
*/
{
    ASE_READER rd=ASE_READER();
	rd.level = 0;
	rd.llevel[0]=NameItm();
	strcpy( rd.llevel[0].name, "Top" );
	rd.vertex_num_old = vertex_num;
	rd.face_num_old = face_num;
	rd.nlbrack = 0;
	rd.nrbrack = 0;
	
	strcpy ( rd.word, " " );
	char  wordm1[LINE_MAX_LEN];
	strcpy ( wordm1, " " );

    stats.bad_num=0;

	//Read a line of text from the file.
	
	for ( ;; )
    {
		char input[LINE_MAX_LEN];
		if ( filein->fgets( input, LINE_MAX_LEN ) == NULL )
			break;

        //if (debug)
        //   printf ( "ASE_READ: Reading line %lu\n",stats.text_num );
		
		stats.text_num++;
		rd.next = input;

		int iword = 0;
        char  word1[LINE_MAX_LEN];

		//Read the next word from the line.
		for ( ;; )
        {
			strcpy ( wordm1, rd.word );
			strcpy ( rd.word, " " );
			
            int   width=0;
			int count = sscanf ( rd.next, "%s%n", rd.word, &width );
			rd.next = rd.next + width;
			if ( count <= 0 ) break;
			iword++;
			if ( iword == 1 ) strcpy ( word1, rd.word );

			//In case the new word is a bracket, update the bracket count.
			if ( strcmp ( rd.word, "{" ) == 0 )
            {
				rd.nlbrack++;
				rd.level = rd.nlbrack - rd.nrbrack;
				strcpy ( rd.llevel[rd.level].name, wordm1 );
			} else
		    if ( strcmp ( rd.word, "}" ) == 0 )
            {
				rd.nrbrack++;
				if ( rd.nlbrack < rd.nrbrack )
                {
					printf ( "\n" );
					printf ( "ASE_READ - Fatal error!\n" );
					printf ( "  Extraneous right bracket on line %lu\n", stats.text_num );
					printf ( "  Currently processing field: %s\n",rd.llevel[rd.level].name );
					return false;
				}
			}

			// *3DSMAX_ASCIIEXPORT  200
			if ( leqi( word1, "*3DSMAX_ASCIIEXPORT" ) )
            {
                if (debug)
                  printf ( "ASE_READ: 3DSMAX_ASCIIEXPORT tag found at line %lu\n",stats.text_num );
				break;
			} else

			// *COMMENT
			if ( leqi( word1, "*COMMENT" ) )
            {
                if (debug)
                  printf ( "ASE_READ: COMMENT tag found at line %lu\n",stats.text_num );
				break;
			} else

			// *GEOMOBJECT
			if ( leqi( rd.llevel[rd.level].name, "*GEOMOBJECT" ) )
            {
                  if (ase_read_geomobject_property(rd))
					continue;
                  else
                    break;
			} else

			// *MESH
			if ( leqi( rd.llevel[rd.level].name, "*MESH" ) )
            {
                  if (ase_read_mesh_property(rd))
					continue;
                  else
                    break;
            } else

			// *MESH_CFACELIST
			if ( leqi( rd.llevel[rd.level].name, "*MESH_CFACELIST" ) )
            {
              if (ase_read_cfacelist_property(rd))
		          continue;
              else
                  break;
			} else

			// *MESH_CVERTLIST
			  //Mesh vertex indices must be incremented by VERTEX_NUM_OLD before being stored
			  //in the internal array.
			if ( leqi( rd.llevel[rd.level].name, "*MESH_CVERTLIST" ) )
            {
              if (ase_read_cvertlist_property(rd))
		          continue;
              else
                  break;
			} else

			// *MESH_FACE_LIST
			//This coding assumes a face is always triangular or quadrilateral.
			if ( leqi( rd.llevel[rd.level].name, "*MESH_FACE_LIST" ) )
            {
              if (ase_read_facelist_property(rd))
		          continue;
              else
                  break;
			} else

			// *MESH_NORMALS
			if ( leqi( rd.llevel[rd.level].name, "*MESH_NORMALS" ) )
            {
              if (ase_read_meshnormals_property(rd))
		          continue;
              else
                  break;
			} else

			// *MESH_TFACELIST
			if ( leqi( rd.llevel[rd.level].name, "*MESH_TFACELIST" ) )
            {
                  if (ase_read_tfacelist_property(rd))
					continue;
                  else
                    break;
			} else

			// *MESH_TVERTLIST
			if ( leqi( rd.llevel[rd.level].name, "*MESH_TVERTLIST" ) )
            {
                  if (ase_read_tvertlist_property(rd))
					continue;
                  else
                    break;
			} else

			// *MESH_VERTEX_LIST
			if ( leqi( rd.llevel[rd.level].name, "*MESH_VERTEX_LIST" ) )
            {
                  if (ase_read_vertxlist_property(rd))
					continue;
                  else
                    break;
			} else
			// *NODE_TM
			  //Each node should start out with a default transformation matrix.
			if ( leqi( rd.llevel[rd.level].name, "*NODE_TM" ) )
            {
                  if (ase_read_nodetm_property(rd))
					continue;
                  else
                    break;
            } else

	  // *SCENE
      if ( leqi( rd.llevel[rd.level].name, "*SCENE" ) )
      {
          if (ase_read_scene_property(rd))
		      continue;
          else
              break;
      }

    }
	//End of loop reading words from the line.
  }
  //End of loop reading lines from input file.
  if (stats.bad_num!=0)
        printf ( "ASE_READ: Warning, %lu wrong lines found when reading.\n",stats.bad_num );
  return true;
}
/******************************************************************************/

unsigned long ase_write_trmatrix ( FileIO *fileout, char *objname,float transform_matrix[4][4] )

/******************************************************************************/
/*
Purpose:
  ASE_WRITE_TRMATRIX writes transition matrix for any object in format of AutoCAD ASE.
  
	Author: Tomasz Lis
	Modified: 05 Mar 2007
*/
{
	//Sub block NODE_TM:
    unsigned long ln_written=0;
	fileout->fprintf("  *NODE_TM {\n" );
	ln_written++;
	fileout->fprintf("    *NODE_NAME \"%s\"\n", objname);
	fileout->fprintf("    *INHERIT_POS %u %u %u\n",0,0,0 );
	fileout->fprintf("    *INHERIT_ROT %u %u %u\n",0,0,0 );
	fileout->fprintf("    *INHERIT_SCL %u %u %u\n",0,0,0 );
	ln_written+=4;
	fileout->fprintf("    *TM_ROW0 %f %f %f\n",1.0,0.0,0.0 );
	fileout->fprintf("    *TM_ROW1 %f %f %f\n",0.0,1.0,0.0 );
	fileout->fprintf("    *TM_ROW2 %f %f %f\n",0.0,0.0,1.0 );
	fileout->fprintf("    *TM_ROW3 %f %f %f\n",0.0,0.0,0.0 );
	ln_written+=4;
	fileout->fprintf("    *TM_POS %f %f %f\n",0.0,0.0,0.0 );
	fileout->fprintf("    *TM_ROTAXIS %f %f %f\n",0.0,0.0,0.0 );
	fileout->fprintf("    *TM_ROTANGLE %f\n",0.0);
	fileout->fprintf("    *TM_SCALE %f %f %f\n",1.0,1.0,1.0 );
	fileout->fprintf("    *TM_SCALEAXIS %f %f %f\n",0.0,0.0,0.0 );
	fileout->fprintf("    *TM_SCALEAXISANG %f\n",0.0);
	ln_written+=6;
	fileout->fprintf("  }\n" );
	ln_written++;
	return ln_written;
}
/******************************************************************************/

unsigned long IVCONV::ase_write_geomobject ( FileIO *fileout, unsigned long meshidx )

/******************************************************************************/
/*
Purpose:
  ASE_WRITE_MESH writes one mesh in format of AutoCAD ASE.
  
	Author: John Burkardt
	Modified: 30 Sep 1998
	Modified: 05 Mar 2007 Tomasz Lis extracted from ase_write and extended
*/
{
    //Begin the big geometry block.
	fileout->fprintf("*GEOMOBJECT {\n" );
	fileout->fprintf("  *NODE_NAME \"%s\"\n", mesh[meshidx].name);
	stats.text_num += 2;
    if (debug)
        printf ( "ASE_WRITE_GEOMOBJECT: Creating GEOMOBJECT for mesh \"%s\"\n", mesh[meshidx].name );
  
	//Sub block NODE_TM:
    stats.text_num += ase_write_trmatrix(fileout,mesh[meshidx].name,transform_matrix);

    //Preparing mesh to save
    array<unsigned long> vert_idx_arr;
    unsigned long vert_count;
    array<long> nvert_idx;
    unsigned long faces_count;
    get_mesh_vertices(vert_idx_arr,vert_count,nvert_idx,faces_count,meshidx);

	//Sub block MESH:
    //Items
	fileout->fprintf("  *MESH {\n" );
	fileout->fprintf("    *TIMEVALUE %u\n",0 );
	fileout->fprintf("    *MESH_NUMVERTEX %d\n", vert_count );
	fileout->fprintf("    *MESH_NUMFACES %d\n", faces_count );
	stats.text_num += 4;

  {	
	//Sub sub block MESH_VERTEX_LIST
	fileout->fprintf("    *MESH_VERTEX_LIST {\n" );
	stats.text_num++;
	unsigned long j;
	for ( j = 0; j < vert_count; ++j )
    {
        unsigned long vert_idx=vert_idx_arr[j];
		fileout->fprintf("      *MESH_VERTEX %d %f %f %f\n", j, vertex[vert_idx].cor3[0],
			vertex[vert_idx].cor3[1], vertex[vert_idx].cor3[2] );
		stats.text_num++;
	}
	fileout->fprintf("    }\n" );
	stats.text_num++;
  }
  { //Sub sub block MESH_FACE_LIST
    //Items MESH_FACE
	fileout->fprintf("    *MESH_FACE_LIST {\n" );
	stats.text_num++;
	unsigned int ivert;
	unsigned long face_nidx=0;
	unsigned long iface;
	for ( iface = 0; iface < face_num; iface++ )
	  if (face[iface].mesh==meshidx)
      {
        unsigned long order=face[iface].order;
        if ((debug)&&(order!=3)&&(order!=4))
            printf ( "    MESH_FACE_LIST: Warning, face %lu has strange order of %lu\n",iface,face[iface].order );
		unsigned int ivert;
		long msh_matr=get_face_major_material(iface);
        // writing face vertices indexes
		fileout->fprintf("      *MESH_FACE %5lu: ", face_nidx ); 
		for (ivert=0;ivert<order;ivert++)
		{
            if (ivert>25) break;
            unsigned long nv_idx=face[iface].vertices[ivert];
            long vert_idx=nvert_idx[nv_idx];
            if (vert_idx<0)
            {
              if (debug)
                printf ( "    MESH_FACE_LIST: Warning, lost vertex from face %lu\n",iface);
              vert_idx=0;
            }
			fileout->fprintf("%c: %5ld ", 'A'+(char)ivert, vert_idx ); 
        }
        // other vertices and face parameters
		for (ivert=0;ivert<order;ivert++)
		{
			fileout->fprintf("%c%c: %u ",'A'+(char)ivert,'A'+(char)((ivert+1)%order),1 );
        }
		fileout->fprintf("*MESH_SMOOTHING %u *MESH_MTLID %u\n",face[iface].smooth,msh_matr );
		stats.text_num++;
		face_nidx++;
      }
    fileout->fprintf("    }\n" );
	stats.text_num++;
  }	

	//Item MESH_NUMTVERTEX.
	fileout->fprintf("    *MESH_NUMTVERTEX %u\n",vert_count );
	stats.text_num++;
  {//Item MESH_TVERTLIST
	fileout->fprintf("    *MESH_TVERTLIST {\n" );
	stats.text_num++;
	unsigned long ivert;
	for ( ivert = 0; ivert < vert_count; ivert++ )
	{
        unsigned long vert_idx=vert_idx_arr[ivert];
        fileout->fprintf("      *MESH_TVERT %lu %f %f %f\n", ivert,
           vertex[vert_idx].tex_uv[0], vertex[vert_idx].tex_uv[1], 0.0005 ); 
    }
    fileout->fprintf("    }\n" );
	stats.text_num++;
  }

	//Item MESH_NUMTVFACES
	fileout->fprintf("    *MESH_NUMTVFACES %u\n",0 );
	stats.text_num++;
/*  {//Item MESH_TVERTLIST
	fileout->fprintf("    *MESH_TFACELIST {\n" );
	stats.text_num++;
	unsigned long iface;
	unsigned long face_nidx=0;
	for ( iface = 0; iface < face_num; iface++ )
	  if (face[iface].mesh==meshidx)
      {
        fileout->fprintf("      *MESH_TFACE %5lu %4lu %4lu %4lu\n", face_nidx, 0, 0, 0 ); 
        face_nidx++;
      }
    fileout->fprintf("    }\n" );
	stats.text_num++;
  }*/

    //Item NUMCVERTEX.
	fileout->fprintf("    *MESH_NUMCVERTEX %u\n",0 );
	stats.text_num++;

  { //Sub block MESH_NORMALS
    //Items MESH_FACENORMAL, MESH_VERTEXNORMAL (repeated)
	fileout->fprintf("    *MESH_NORMALS {\n" );
	stats.text_num++;
	unsigned long iface;
	unsigned long face_nidx=0;
	for ( iface = 0; iface < face_num; iface++ )
	  if (face[iface].mesh==meshidx)
      {
		fileout->fprintf("      *MESH_FACENORMAL %d %f %f %f\n", 
			face_nidx, face[iface].normal[0], face[iface].normal[1], face[iface].normal[2] );
		stats.text_num++;
        unsigned long order=face[iface].order;
        if (order>26) order=26;
        unsigned long ivert;
		for ( ivert = 0; ivert < order; ivert++ )
        {
            unsigned long vertidx=face[iface].vertices[ivert];
            long vert_nidx=nvert_idx[vertidx];
			fileout->fprintf("        *MESH_VERTEXNORMAL %d %f %f %f\n", 
				vert_nidx, vertex[vertidx].normal[0], 
				vertex[vertidx].normal[1], vertex[vertidx].normal[2] );
			stats.text_num++;
		}
        face_nidx++;
      }
	fileout->fprintf("    }\n" );
	stats.text_num++;
  }
	//Close the MESH object.
	fileout->fprintf("  }\n" );
	stats.text_num++;
    if (debug)
        printf ( "ASE_WRITE_GEOMOBJECT: MESH structure ends at line %lu\n", stats.text_num );

	//A few closing parameters.
	fileout->fprintf("  *PROP_MOTIONBLUR %u\n",0 );
    int shadow_val=0;
    if (mesh[meshidx].shadows) shadow_val=1;
	fileout->fprintf("  *PROP_CASTSHADOW %u\n",shadow_val );
	fileout->fprintf("  *PROP_RECVSHADOW %u\n",shadow_val );
	stats.text_num += 3;
    //default material
	fileout->fprintf("  *MATERIAL_REF %u\n",0 );
	stats.text_num++;
	//Close the GEOM object.
	fileout->fprintf("}\n" );
	stats.text_num ++;
}
/******************************************************************************/

unsigned long IVCONV::ase_write_material_map ( FileIO *fileout, long textr_idx )

/******************************************************************************/
/*
Purpose:
  ASE_WRITE_MATERIAL_MAP writes map properties to material information
   of AutoCAD ASE file.
  
	Author: Tomasz Lis
	Modified: 05 Mar 2007
*/
{
        fileout->fprintf("      *MAP_NAME \"%s\"\n",texture[textr_idx].name );
        fileout->fprintf("      *MAP_CLASS \"%s\"\n","Bitmap" );
        fileout->fprintf("      *MAP_AMOUNT %f\n",texture[textr_idx].intense );
        fileout->fprintf("      *BITMAP \"%s\"\n",texture[textr_idx].name );
        stats.text_num+=4;
        fileout->fprintf("      *MAP_TYPE %s\n","Screen" );
        fileout->fprintf("      *UVW_U_OFFSET %f\n",0.0 );
        fileout->fprintf("      *UVW_V_OFFSET %f\n",0.0 );
        fileout->fprintf("      *UVW_U_TILING %f\n",1.0 );
        fileout->fprintf("      *UVW_V_TILING %f\n",1.0 );
        fileout->fprintf("      *BITMAP_FILTER %s\n","Pyramidal" );
        stats.text_num+=6;
}
/******************************************************************************/

unsigned long IVCONV::ase_write_camera ( FileIO *fileout, unsigned long camidx )

/******************************************************************************/
/*
Purpose:
  ASE_WRITE_CAMERA writes one camera in format of AutoCAD ASE.
  
	Author: Tomasz Lis
	Modified: 05 Mar 2007
*/
{
  unsigned long text_num_base=stats.text_num;
  //Begin the light object.
  fileout->fprintf("*CAMERAOBJECT {\n" );
  fileout->fprintf("  *NODE_NAME \"%s\"\n", camera[camidx].name);
  stats.text_num += 2;
  fileout->fprintf("  *LIGHT_TYPE %s\n", "Free");
  stats.text_num++;
  if (debug)
      printf ( "ASE_WRITE_CAMERAOBJECT: Creating CAMERAOBJECT for camera %lu\n", camidx );
  //Sub block NODE_TM:
  stats.text_num += ase_write_trmatrix(fileout,camera[camidx].name,transform_matrix);
  //LIGHT_SETTINGS chunk
  fileout->fprintf("  *CAMERA_SETTINGS {\n");
  stats.text_num ++;
	fileout->fprintf("    *TIMEVALUE %u\n",0);
	stats.text_num++;
	fileout->fprintf("    *CAMERA_NEAR %f\n",0.0);
	stats.text_num++;
	fileout->fprintf("    *CAMERA_FAR %f\n",1000.0);
	stats.text_num++;
	fileout->fprintf("    *CAMERA_FOV %f\n",1.7);
	stats.text_num++;
	fileout->fprintf("    *CAMERA_TDIST %f\n",1.0);
	stats.text_num++;
  //Close the LIGHT_SETTINGS chunk.
  fileout->fprintf("  }\n" );
  //Close the LIGHTOBJECT chunk.
  fileout->fprintf("}\n" );
  stats.text_num ++;
  return stats.text_num-text_num_base;
}
/******************************************************************************/

unsigned long IVCONV::ase_write_light ( FileIO *fileout, unsigned long lightidx )

/******************************************************************************/
/*
Purpose:
  ASE_WRITE_LIGHT writes one light in format of AutoCAD ASE.
  
	Author: Tomasz Lis
	Modified: 05 Mar 2007
*/
{
    unsigned long text_num_base=stats.text_num;
    //Begin the light object.
	fileout->fprintf("*LIGHTOBJECT {\n" );
	fileout->fprintf("  *NODE_NAME \"%s\"\n", light[lightidx].name);
	stats.text_num += 2;
    char lighttype_val[LINE_MAX_LEN];
    if (light[lightidx].directional)
        strcpy(lighttype_val,"Target");
    else
        strcpy(lighttype_val,"Omni");
	fileout->fprintf("  *LIGHT_TYPE %s\n", lighttype_val);
	stats.text_num++;
    if (debug)
        printf ( "ASE_WRITE_LIGHTOBJECT: Creating LIGHTOBJECT for light %lu\n", lightidx );
	//Sub block NODE_TM:
    stats.text_num += ase_write_trmatrix(fileout,light[lightidx].name,transform_matrix);
    //Directional lights must have NODE_TM for target
    if (light[lightidx].directional)
    {
        char targetname_val[LINE_MAX_LEN];
        sprintf(targetname_val,"%s.Target",light[lightidx].name);
        stats.text_num += ase_write_trmatrix(fileout,targetname_val,transform_matrix);
    }
    //Other parameters
    char shadow_val[LINE_MAX_LEN];
    if (light[lightidx].shadows)
        strcpy(shadow_val,"On");
    else
        strcpy(shadow_val,"Off");
	fileout->fprintf("  *LIGHT_SHADOWS %s\n",shadow_val);
	stats.text_num++;
	fileout->fprintf("  *LIGHT_USELIGHT %u\n",1);
	stats.text_num++;
	fileout->fprintf("  *LIGHT_SPOTSHAPE %s\n","Circle");
	stats.text_num++;
	fileout->fprintf("  *LIGHT_USEGLOBAL %u\n",0);
	stats.text_num++;
	fileout->fprintf("  *LIGHT_ABSMAPBIAS %u\n",0);
	stats.text_num++;
	fileout->fprintf("  *LIGHT_OVERSHOOT %u\n",0);
	stats.text_num++;
    //LIGHT_SETTINGS chunk
	fileout->fprintf("  *LIGHT_SETTINGS {\n");
	stats.text_num ++;
	fileout->fprintf("    *TIMEVALUE %u\n",0);
	stats.text_num++;
	fileout->fprintf("    *LIGHT_COLOR %f %f %f\n",light[lightidx].rgb[0],
             light[lightidx].rgb[1],light[lightidx].rgb[2]);
	stats.text_num++;
	fileout->fprintf("    *LIGHT_INTENS %f\n",1.0);
	stats.text_num++;
	fileout->fprintf("    *LIGHT_ASPECT %f\n",-1.0);
	stats.text_num++;
	fileout->fprintf("    *LIGHT_TDIST %f\n",-1.0);
	stats.text_num++;
	fileout->fprintf("    *LIGHT_MAPBIAS %f\n",1.0);
	stats.text_num++;
	fileout->fprintf("    *LIGHT_MAPRANGE %f\n",4.0);
	stats.text_num++;
	fileout->fprintf("    *LIGHT_MAPSIZE %u\n",512);
	stats.text_num++;
	fileout->fprintf("    *LIGHT_RAYBIAS %f\n",0.0);
	stats.text_num++;
	fileout->fprintf("  }\n" );
	stats.text_num ++;

	//Close the LIGHTOBJECT chunk.
	fileout->fprintf("}\n" );
	stats.text_num ++;
    return stats.text_num-text_num_base;
}
/******************************************************************************/

unsigned long IVCONV::ase_write_material ( FileIO *fileout, unsigned long matridx )

/******************************************************************************/
/*
Purpose:
  ASE_WRITE_MATERIAL writes material information to an AutoCAD ASE file.
  
	Author: Tomasz Lis
	Modified: 05 Mar 2007
*/
{
    unsigned long text_num_base=stats.text_num;
	fileout->fprintf("  *MATERIAL %lu {\n",matridx );
	fileout->fprintf("    *MATERIAL_NAME \"%s\"\n",material[matridx].name );
	fileout->fprintf("    *MATERIAL_CLASS \"%s\"\n","Standard" );
    stats.text_num += 3;
	fileout->fprintf("    *MATERIAL_AMBIENT %f %f %f\n",material[matridx].ambient[0],
              material[matridx].ambient[1],material[matridx].ambient[2] );
	fileout->fprintf("    *MATERIAL_DIFFUSE %f %f %f\n",material[matridx].rgb[0],
              material[matridx].rgb[1],material[matridx].rgb[2] );
	fileout->fprintf("    *MATERIAL_SPECULAR %f %f %f\n",material[matridx].specular[0],
              material[matridx].specular[1],material[matridx].specular[2] );
    stats.text_num += 3;
	fileout->fprintf("    *MATERIAL_SHINE %f\n",0.3 );
	fileout->fprintf("    *MATERIAL_SHINESTRENGTH %f\n",0.3 );
	fileout->fprintf("    *MATERIAL_TRANSPARENCY %f\n",material[matridx].alpha );
	fileout->fprintf("    *MATERIAL_WIRESIZE %f\n",1.0 );
	fileout->fprintf("    *MATERIAL_SHADING \"%s\"\n","Blinn" );
	fileout->fprintf("    *MATERIAL_SELFILLUM %f\n",0.0 );
    stats.text_num += 6;
    long textr_idx=material[matridx].texture;
    if (textr_idx>=0)
    {
        fileout->fprintf("    *MAP_DIFFUSE {\n");
        stats.text_num++;
        ase_write_material_map(fileout,textr_idx);
        fileout->fprintf("    }\n" );
        stats.text_num++;
    }
    textr_idx=material[matridx].bump_texture;
    if (textr_idx>=0)
    {
        fileout->fprintf("    *MAP_BUMP {\n");
        stats.text_num++;
        ase_write_material_map(fileout,textr_idx);
        fileout->fprintf("    }\n" );
        stats.text_num++;
    }
	fileout->fprintf("  }\n" );
    stats.text_num++;
  return stats.text_num-text_num_base;
}
/******************************************************************************/

bool IVCONV::ase_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  ASE_WRITE writes all 3D scene graphics information to an AutoCAD ASE file.
  
	Author: John Burkardt
	Modified: 30 Sep 1998
	Modified: 05 Mar 2007 Tomasz Lis improved code
*/
{
  stats.text_num = 0;
  { //Write the header.
	fileout->fprintf ( "*3DSMAX_ASCIIEXPORT 200\n" );
	fileout->fprintf("*COMMENT \"%s, created by IVCON.\"\n", fileout_name );
	fileout->fprintf("*COMMENT \"Original data in %s\"\n", filein_name );
    stats.text_num += 3;
    if (debug)
        printf ( "ASE_WRITE - Header ends after %lu lines;\n", stats.text_num );
  }	
  { //Write the scene block.
	fileout->fprintf("*SCENE {\n" );
	fileout->fprintf("  *SCENE_FILENAME \"%s\"\n",filein_name );
	fileout->fprintf("  *SCENE_FIRSTFRAME %u\n",0 );
	fileout->fprintf("  *SCENE_LASTFRAME %u\n",100 );
	fileout->fprintf("  *SCENE_FRAMESPEED %u\n",30 );
	fileout->fprintf("  *SCENE_TICKSPERFRAME %u\n",160 );
	fileout->fprintf("  *SCENE_BACKGROUND_STATIC %f %f %f\n",env.rgb[0],env.rgb[1],env.rgb[2] );
	fileout->fprintf("  *SCENE_AMBIENT_STATIC %f %f %f\n",env.ambient_light[0],env.ambient_light[1],env.ambient_light[2] );
	fileout->fprintf("}\n" );
    stats.text_num += 9;
    if (debug)
        printf ( "ASE_WRITE - SCENE structure ends at line %lu\n", stats.text_num );
  }	
  { //Write the materials block
	fileout->fprintf("*MATERIAL_LIST {\n" );
	fileout->fprintf("  *MATERIAL_COUNT %lu\n",material_num );
    stats.text_num += 2;
    unsigned long matridx;
    for (matridx=0;matridx<material_num;matridx++)
        ase_write_material(fileout,matridx);
	fileout->fprintf("}\n" );
    stats.text_num ++;
    if (debug)
        printf ( "ASE_WRITE - MATERIALS structure ends at line %lu\n", stats.text_num );
  }	
  {
    unsigned long meshidx;
    for (meshidx=0;meshidx<mesh_num;meshidx++)
        ase_write_geomobject ( fileout, meshidx );
  }
  {
    unsigned long lightidx;
    for (lightidx=0;lightidx<light_num;lightidx++)
        ase_write_light( fileout, lightidx );
  }
  {
    unsigned long camidx;
    for (camidx=0;camidx<camera_num;camidx++)
        ase_write_camera( fileout, camidx );
  }
	//Report.
	printf ( "\n" );
	printf ( "ASE_WRITE - Wrote %d text lines;\n", stats.text_num );
	return true;
}
/******************************************************************************/
