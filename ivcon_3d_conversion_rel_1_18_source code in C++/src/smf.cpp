/******************************************************************************/
// Author:  John Burkardt
// Change: 03 July 1999 John Burkardt
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core
// Change: 18 Apr 2008 Tomasz Lis fixed format support

// Purpose:
//   Methods for support of Michael Garland's format for QSLIM

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - Full 32-bit vertices support
// - Full 32-bit faces support, face order limited to about LINE_MAX_LEN/10
// - Full meshes support (one level only - no sub-meshes)
// - Materials supported (color and texture)
// - One texture supported
// - Texture UV supported (per-vertex; per-face is untested)
// - No default view or cameras supported
// - No environment or lights support
// - No shapes supported
// Format support note - writing:
// - Full 32-bit vertices support
// - Full 32-bit faces support, unlimited face order
// - No meshes support
// - Limited materials support (only color and texture)
// - One texture supported
// - Texture UV supported (per-vertex)
// - No camera/view support
// - No Environment or lights support
// - No shapes support
/*
	Example:

	  #SMF2.0
	  #  cube_face.smf
	  #  This example demonstrates how an RGB color can be assigned to
	  #  each face of an object.
	  #
	  # First, define the geometry of the cube.
	  #
	  v 0.0  0.0  0.0
	  v 1.0  0.0  0.0
	  v 0.0  1.0  0.0
	  v 1.0  1.0  0.0
	  v 0.0  0.0  1.0
	  v 1.0  0.0  1.0
	  v 0.0  1.0  1.0
	  v 1.0  1.0  1.0
	  f 1 4 2
	  f 1 3 4
	  f 5 6 8
	  f 5 8 7
	  f 1 2 6
	  f 1 6 5
	  f 2 4 8
	  f 2 8 6
	  f 4 3 7
	  f 4 7 8
	  f 3 1 5
	  f 3 5 7
	  #
	  #  Colors will be bound 1 per face.
	  #
	  bind c face
	  c 1.0  0.0  0.0
	  c 1.0  0.0  0.0
	  c 0.0  1.0  0.0
	  c 0.0  1.0  0.0
	  c 0.0  0.0  1.0
	  c 0.0  0.0  1.0
	  c 1.0  1.0  0.0
	  c 1.0  1.0  0.0
	  c 0.0  1.0  1.0
	  c 0.0  1.0  1.0
	  c 1.0  0.0  1.0
	  c 1.0  0.0  1.0
	  #
	  #  Normal vectors will be bound 1 per face.
	  #
	  bind n face
	  n  0.0   0.0  -1.0
	  n  0.0   0.0  -1.0
	  n  0.0   0.0   1.0
	  n  0.0   0.0   1.0
	  n  0.0  -1.0   0.0
	  n  0.0  -1.0   0.0
	  n  1.0   0.0   0.0
	  n  1.0   0.0   0.0
	  n  0.0   1.0   0.0
	  n  0.0   1.0   0.0
	  n -1.0   0.0   0.0
	  n -1.0   0.0   0.0
	  #
	  #  Texture coordinate pairs will be bound 1 per face.
	  #
	  bind r face
	  r  0.0   0.0
	  r  0.0   0.1
	  r  0.0   0.2
	  r  0.0   0.3
	  r  0.1   0.0
	  r  0.1   0.1
	  r  0.1   0.2
	  r  0.1   0.3
	  r  0.2   0.0
	  r  0.2   0.1
	  r  0.2   0.2
	  r  0.2   0.3
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"
#include <cstring>

/******************************************************************************/

enum SMF_BINDING {
    BND_UNDEFINED,
    BND_PER_VERTEX,
    BND_PER_FACE,
};

/******************************************************************************/

bool IVCONV::smf_read ( FileIO *filein )

/******************************************************************************/
/*
Purpose:
  SMF_READ reads an SMF file.

    Author: John Burkardt
    Modified: 03 July 1999
*/
{
    short material_binding,normal_binding,texture_binding;
    //Prepare structure for reading "per face" UV mapping
    array<FaceUV> face_uv;
    long mesh_idx;
    unsigned long color_idx;
    unsigned long normal_idx;
    unsigned long uvmap_idx;
	long texture_idx;
	unsigned long material_base;
	unsigned long vertex_base;
	unsigned long face_base;
	int level;
	int vertex_correction;

	char *next;
	int   width;
    char cnr[LINE_MAX_LEN];
	char  token[LINE_MAX_LEN];
	char  token2[LINE_MAX_LEN];
	material_binding=BND_UNDEFINED;
	normal_binding=BND_UNDEFINED;
	texture_binding=BND_UNDEFINED;
	vertex_correction = 0;
    material_base = material_num;
    vertex_base = vertex_num;
    face_base = face_num;
	level = 0;
	color_idx=0;
	normal_idx=0;
	uvmap_idx=0;
	mesh_idx=-1;
	texture_idx=-1;

	// Read the next line of the file into INPUT.
	stats.text_num = 0;
	char input[LINE_MAX_LEN];

	while ( filein->fgets(input,LINE_MAX_LEN) != NULL )
    {
		stats.text_num++;
		if ( debug )
			printf ( "SMF_READ: Reading line %u\n", stats.text_num );

		// Advance to the first nonspace character in INPUT.
		for ( next = input; *next != '\0' && isspace(*next); next++ )
        { }
		// Skip blank lines.
		if ( *next == '\0' )
        { continue; }
		// Skip comment lines.
		if ( (*next == '#') || (*next == '$') )
        {
			stats.comment_num++;
			continue;
		}

		// Extract the first word in this line.
		sscanf ( next, "%s%n", token, &width );
		// Set NEXT to point to just after this token.
		next += width;

        // BEGIN
        // Reset the transformation matrix to identity.
        // Node numbering starts at zero again.  (Really, this is level based)
        // (Really should define a new transformation matrix, and concatenate.)
        // (Also, might need to keep track of level.)
		if ( leqi(token,"BEGIN") )
        {
			level++;
			// Update materials added in last block
			unsigned int material_idx;
			for (material_idx=material_base;material_idx<material_num;material_idx++)
                material[material_idx].texture=texture_idx;
            material_base = material_num;
            //Convert material mapping to "per vertex"
            if (material_binding==BND_PER_FACE)
                face_to_vertex_material(face_uv,face_base,face_num-face_base);
            //Convert UV mapping to "per vertex"
            if (texture_binding==BND_PER_FACE)
                face_to_vertex_uv(face_uv,face_base,face_num-face_base);
            // Create new mesh
			mesh[mesh_num]=Mesh();
			mesh_idx=mesh_num;
			mesh_num++;
			// Clear the block-dependent variables
			vertex_base = vertex_num;
			face_base = face_num;
            color_idx=0;
            normal_idx=0;
            uvmap_idx=0;
            texture_idx=-1;
			tmat_init ( transform_matrix );
		} else

        // BIND [c|n|r] [vertex|face]
        // Specify the binding for RGB color, Normal, or Texture.
        // Options are "vertex" or "face"
		if ( leqi( token, "BIND" )  )
        {
            char type[LINE_MAX_LEN];
			sscanf ( next, "%s%n", cnr, &width );
			next += width;
			sscanf ( next, "%s%n", type, &width );
			next += width;
			if ( debug )
				printf ( "SMF_READ: Bind - CNR=%s, TYPE=%s\n",cnr,type );

			if ( leqi(cnr,"C") )
            {
                if ( leqi(type,"VERTEX") )
                {
                    material_binding=BND_PER_VERTEX;
                } else
                if ( leqi(type,"FACE") )
                {
                    material_binding=BND_PER_FACE;
                } else
                {
                    stats.bad_num++;
                    printf ( "SMF_READ: Unrecognized material binding in line %u (ignored).\n",stats.text_num );
                }
			} else
			if ( leqi(cnr,"N") )
            {
                if ( leqi(type,"VERTEX") )
                {
                    normal_binding=BND_PER_VERTEX;
                } else
                if ( leqi(type,"FACE") )
                {
                    normal_binding=BND_PER_FACE;
                } else
                {
                    stats.bad_num++;
                    printf ( "SMF_READ: Unrecognized normal binding in line %u (ignored).\n",stats.text_num );
                }
			} else
			if ( leqi ( cnr, "R" )  )
            {
                if ( leqi(type,"VERTEX") )
                {
                    texture_binding=BND_PER_VERTEX;
                } else
                if ( leqi(type,"FACE") )
                {
                    texture_binding=BND_PER_FACE;
                } else
                {
                    stats.bad_num++;
                    printf ( "SMF_READ: Unrecognized texture binding in line %u (ignored).\n",stats.text_num );
                }
			} else
			{
                stats.bad_num++;
                printf ( "SMF_READ: Unrecognized binding in line %u (ignored).\n",stats.text_num );
            }
		} else

        // C <r> <g> <b>
        // Specify an RGB color, with R, G, B between 0.0 and 1.0.
		if ( leqi(token,"C") )
        {
            int count=1;
            float r,g,b;
            if (count==1)
                count = sscanf ( next, "%f%n", &r, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &g, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &b, &width );
			next += width;
            if (count!=1)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read color in line %u (ignored).\n",stats.text_num );
                continue;
            }
            // Set up a temporary material (R,G,B,1.0).
            // Add the material to the material database, or find the index of
            // a matching material already in.
            // Assign the material of the node or face to this index.
            unsigned int material_idx;
			material[material_num]=Material(r,g,b);
			material_idx = material_num;
			material_num++;
			if (material_binding==BND_PER_FACE)
            {
              // Make sure we won't initialize the FaceUV twice
              if (color_idx>=uvmap_idx)
                face_uv[color_idx]=FaceUV();
              face_uv[color_idx].material=material_idx;
			} else
			if (material_binding==BND_PER_VERTEX)
            {
              if (vertex_base+color_idx < vertex_num)
              {
                vertex[vertex_base+color_idx].material=material_idx;
              } else
              {
                stats.bad_num++;
                printf ( "SMF_READ: More colors than vertices in line %u (ignored).\n",stats.text_num );
              }
			} else
            {
                stats.bad_num++;
				printf ( "SMF_READ: Color error - Material binding undefined in line %u (ignored).\n",stats.text_num );
			}
			color_idx++;
		} else

        // END
        // Drop down a level.
		if ( leqi(token,"END") )
        {
			level--;
			if ( level < 0 )
            {
				printf ( "\n" );
				printf ( "SMF_READ - Fatal error!\n" );
				printf ( "  More END statements than BEGINs!\n" );
				return false;
			}
			// Update materials added in last block
			unsigned int material_idx;
			for (material_idx=material_base;material_idx<material_num;material_idx++)
                material[material_idx].texture=texture_idx;
            material_base = material_num;
            //Convert material mapping to "per vertex"
            if (material_binding==BND_PER_FACE)
                face_to_vertex_material(face_uv,face_base,face_num-face_base);
            //Convert UV mapping to "per vertex"
            if (texture_binding==BND_PER_FACE)
                face_to_vertex_uv(face_uv,face_base,face_num-face_base);
			// Back to previous mesh
			mesh_idx--;
            color_idx=0;
            normal_idx=0;
            uvmap_idx=0;
		} else

        // F V1 V2 V3
        // Face.
        // A face is defined by the vertices.
        // Node indices are 1 based rather than 0 based.
        // So we have to decrement them before loading them into FACE.
        // Note that vertex indices start back at 0 each time a BEGIN is entered.
        // The strategy here won't handle nested BEGIN's, just one at a time.
		if ( leqi(token,"F") )
        {
            if (face_num>=FACES_MAX)
            {
                stats.bad_num++;
                if (stats.bad_num<16)
                  printf ( "SMF_READ: Faces limit reached in line %u (ignored).\n",stats.text_num );
                continue;
            }
			unsigned long face_idx;
			face[face_num] = Face();
			face_idx=face_num;
			face_num++;
			if (mesh_idx<0)
			{
                mesh[mesh_num]=Mesh();
                mesh_idx=mesh_num;
                mesh_num++;
            }
            face[face_num].mesh=mesh_idx;
			// Read each item in the F definition as a token, and then
			// take it apart.
            int ivert;
			for (ivert=0;ivert<ORDER_MAX;ivert++ )
            {
                int count=1;
                int node;
				if (count==1)
                    count = sscanf ( next, "%s%n", token2, &width );
				next += width;
				if (count==1)
                    count = sscanf ( token2, "%d%n", &node, &width );
				if (count!=1)
                    break;
                unsigned long vertex_idx=node + vertex_base - 1 + vertex_correction;
                if (vertex_idx<vertex_num)
                {
                    face[face_idx].vertices[ivert] = vertex_idx;
                    face[face_idx].order++;
                } else
                {
                    printf ( "SMF_READ: Warning - face definition had nonexisting vertex.\n" );
                }
			}
			if (ivert==ORDER_MAX)
                printf ( "SMF_READ: Warning - max order reached, face truncated in line %u.\n",stats.text_num );
            else
            if ((debug)&&(face[face_idx].order<3))
                printf ( "SMF_READ: Warning - face order smaller than 3 in line %u.\n",stats.text_num );
		} else

        // N <x> <y> <z>
        // Specify a normal vector.
		if ( leqi ( token, "N" )  )
        {
            int count=1;
            float x,y,z;
            if (count==1)
                count = sscanf ( next, "%f%n", &x, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &y, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &z, &width );
			next += width;
            if (count!=1)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read normal vector in line %u (ignored).\n",stats.text_num );
                continue;
            }
			if (normal_binding==BND_PER_FACE)
            {
                unsigned long face_idx=face_base+normal_idx;
                if (face_idx<face_num)
                {
                    face[face_idx].normal=vec3(x,y,z);
                } else
                {
                    stats.bad_num++;
                    printf ( "SMF_READ: Normal vector for nonexisting face in line %u (ignored).\n",stats.text_num );
                }
			} else
			if (normal_binding==BND_PER_VERTEX)
            {
                unsigned long vertex_idx=vertex_base+normal_idx;
                if (vertex_idx<vertex_num)
                {
                    vertex[vertex_idx].normal=vec3(x,y,z);
                } else
                {
                    stats.bad_num++;
                    printf ( "SMF_READ: Normal vector for nonexisting vertex in line %u (ignored).\n",stats.text_num );
                }
			} else
            {
                stats.bad_num++;
                if (stats.bad_num<16)
                  printf ( "SMF_READ: Normal vector error - binding undefined in line %u (ignored).\n",stats.text_num );
			}
            normal_idx++;
		} else

		// R <u> <v>
		// Specify a texture coordinate.
		if ( leqi(token,"R") )
        {
            int count=1;
            float u,v;
            if (count==1)
                count = sscanf ( next, "%f%n", &u, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &v, &width );
			next += width;
            if (count!=1)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read UV mapping in line %u (ignored).\n",stats.text_num );
                continue;
            }

			if (texture_binding==BND_PER_FACE)
            {
                // Make sure we won't initialize the FaceUV twice
                if (uvmap_idx>=color_idx)
                    face_uv[uvmap_idx]=FaceUV();
                face_uv[uvmap_idx].tex_uv[face_uv[uvmap_idx].order]=vec2(u,v);
                face_uv[uvmap_idx].order++;
			} else
			if (texture_binding==BND_PER_VERTEX)
            {
                unsigned long vertex_idx=vertex_base+uvmap_idx;
                if (vertex_idx<vertex_num)
                {
                    vertex[vertex_idx].tex_uv=vec2(u,v);
                } else
                {
                    stats.bad_num++;
                    printf ( "SMF_READ: UV mapping for nonexisting vertex in line %u (ignored).\n",stats.text_num );
                }
			} else
            {
                stats.bad_num++;
                if (stats.bad_num<16)
                  printf ( "SMF_READ: UV mapping error - binding undefined in line %u (ignored).\n",stats.text_num );
			}
			uvmap_idx++;
		} else

		// ROT [x|y|z] <theta>
		if ( leqi(token,"ROT") )
        {
            int count=1;
            float angle;
            char  axis;
            if (count==1)
                sscanf ( next, "%c%n", &axis, &width );
			next += width;
            if (count==1)
                sscanf ( next, "%f%n", &angle, &width );
			next += width;
            if (count!=1)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read rotation in line %u (ignored).\n",stats.text_num );
                continue;
            }
			tmat_rot_axis ( transform_matrix, transform_matrix, angle, axis );
		} else

		// SCALE <sx> <sy> <sz>
		if ( leqi(token,"SCALE") )
        {
            int count=1;
            float x,y,z;
            if (count==1)
                count = sscanf ( next, "%f%n", &x, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &y, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &z, &width );
			next += width;
            if (count!=1)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read scale in line %u (ignored).\n",stats.text_num );
                continue;
            }
			tmat_scale ( transform_matrix, transform_matrix, x, y, z );
		} else

		// SET VERTEX_CORRECTION <i>
		// Specify increment to add to vertex indices in file.
		if ( leqi(token,"SET")  )
        {
            int count=1;
            int vcorrct;
            if (count==1)
                count = sscanf ( next, "%s%n", cnr, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%d%n", &vcorrct, &width );
			next += width;
            if (count!=1)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read vertex correction in line %u (ignored).\n",stats.text_num );
                vertex_correction=0;
                continue;
            }
			vertex_correction=vcorrct;
		} else

		// T_SCALE <dx> <dy>
		// Specify a scaling to texture coordinates.
		if ( leqi(token,"T_SCALE") )
        {
            int count=1;
            float dx,dy;
            if (count==1)
                count = sscanf ( next, "%f%n", &dx, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &dy, &width );
			next += width;
            if (count!=1)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read texture scale in line %u (ignored).\n",stats.text_num );
                continue;
            }
            //TODO: store the scaling
		} else

        // T_TRANS <dx> <dy>
        // Specify a translation to texture coordinates.
		if ( leqi(token,"T_TRANS") )
        {
            int count=1;
            float dx,dy;
            if (count==1)
                count = sscanf ( next, "%f%n", &dx, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &dy, &width );
			next += width;
            if (count!=1)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read texture translation in line %u (ignored).\n",stats.text_num );
                continue;
            }
            //TODO: store the value
		} else

        // TEX <filename>
        // Specify a filename containing the texture.
		if ( leqi(token,"TEX") )
        {
            int count=1;
            int vcorrct;
            if (count==1)
                count = sscanf ( next, "%s%n", cnr, &width );
            if (count!=1)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read texture name in line %u (ignored).\n",stats.text_num );
                continue;
            }
            texture[texture_num]=Texture();
            texture_num++;
            texture_idx=texture_num;
            strncpy(texture[texture_idx].name,cnr,LINE_MAX_LEN);
            // TODO: set materials to this texture
		} else

		// TRANS <dx> <dy> <dz>
		if ( leqi(token,"TRANS") )
        {
            int count=1;
            float x,y,z;
            if (count==1)
                count = sscanf ( next, "%f%n", &x, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &y, &width );
			next += width;
            if (count==1)
                count = sscanf ( next, "%f%n", &z, &width );
			next += width;
            if (count!=1)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read transform vector in line %u (ignored).\n",stats.text_num );
                continue;
            }
			tmat_trans ( transform_matrix, transform_matrix, x, y, z );
		} else

		// V <X> <Y> <Z>
		// Geometric vertex.
		if ( leqi(token,"V") )
        {
            if (vertex_num>=VERTICES_MAX)
            {
                stats.bad_num++;
                if (stats.bad_num<10)
                  printf ( "SMF_READ: Vertex limit reached in line %u (ignored).\n",stats.text_num );
                continue;
            }
            int count;
            float xvec[3];
			count = sscanf ( next, "%e %e %e",&(xvec[0]),&(xvec[1]),&(xvec[2]));
            if (count!=3)
            {
                stats.bad_num++;
                printf ( "SMF_READ: Couldn't read vertex coords in line %u (ignored).\n",stats.text_num );
                continue;
            }
            vertex[vertex_num]=Vertex();
			vertex[vertex_num].cor3=vec3(xvec[0],xvec[1],xvec[2]);
            // Apply current transformation matrix.
            // Right now, we can only handle one matrix, not a stack of
            // matrices representing nested BEGIN/END's.
			tmat_mxp ( transform_matrix, xvec, xvec );
			vertex_num++;
		} else

		// Unrecognized keyword.
		{
			stats.bad_num++;
			if ( stats.bad_num <= 10 )
            {
				printf ( "SMF_READ: Bad data in line %u (ignored).\n", stats.text_num );
			}
		}

  }

  // Update materials added in last block
  unsigned int material_idx;
  for (material_idx=material_base;material_idx<material_num;material_idx++)
    material[material_idx].texture=texture_idx;
  //Convert material mapping to "per vertex"
  if (material_binding==BND_PER_FACE)
    face_to_vertex_material(face_uv,face_base,face_num-face_base);
  //Convert UV mapping to "per vertex"
  if (texture_binding==BND_PER_FACE)
    face_to_vertex_uv(face_uv,face_base,face_num-face_base);
  if ( debug )
      printf ( "SMF_READ: Finished analyzing file lines.\n" );
  return true;
}
/******************************************************************************/

bool IVCONV::smf_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  SMF_WRITE writes graphics information to an SMF file.

    Author: John Burkardt
    Modified: 18 Apr 2008
*/
{
	// Initialize.
	stats.text_num = 0;

	fileout->fprintf( "#$SMF 2.0\n" );
	fileout->fprintf( "#$vertices %lu\n", vertex_num );
	fileout->fprintf( "#$faces %lu\n", face_num );
	fileout->fprintf( "#\n" );
	stats.text_num += 4;

	fileout->fprintf( "# %s created by IVCON.\n", fileout_name );
	fileout->fprintf( "# Original data in %s.\n", filein_name );
	fileout->fprintf( "#\n" );
	stats.text_num += 3;

	// V: vertex coordinates.
    unsigned long vertex_idx;
	for ( vertex_idx = 0; vertex_idx < vertex_num; vertex_idx++ )
    {
		fileout->fprintf( "v %f %f %f\n",
			vertex[vertex_idx].cor3[0], vertex[vertex_idx].cor3[1], vertex[vertex_idx].cor3[2] );
		stats.text_num++;
	}

	// F: faces.
	if ( face_num > 0 )
    {
		fileout->fprintf( "\n" );
		stats.text_num++;
        unsigned long iface;
        for ( iface = 0; iface < face_num; iface++ )
        {
            fileout->fprintf( "f" );
            unsigned long ivert;
            for ( ivert = 0; ivert < face[iface].order; ivert++ )
            {
                unsigned long vertidx=face[iface].vertices[ivert];
                fileout->fprintf( " %lu", vertidx+1 );
            }
            fileout->fprintf( "\n" );
            stats.text_num++;
        }
	}

	// Material binding.
	fileout->fprintf( "bind c vertex\n" );
	stats.text_num++;
	// Material RGB values at each node.
	unsigned long icor3;
	long texture_idx=0;
	for ( icor3 = 0; icor3 < vertex_num; icor3++ )
    {
        unsigned long imat;
		imat = vertex[icor3].material;
		fileout->fprintf( "c %f %f %f\n", material[imat].rgb[0], material[imat].rgb[1], material[imat].rgb[2]);
		if (material[imat].texture>=0)
		    texture_idx=material[imat].texture;
		stats.text_num++;
	}

	// Normal binding.
	fileout->fprintf( "bind n vertex\n" );
	stats.text_num++;
	// Normal vector at each node.
	for ( icor3 = 0; icor3 < vertex_num; icor3++ )
    {
		fileout->fprintf( "n %f %f %f\n", vertex[icor3].normal[0],
			vertex[icor3].normal[1], vertex[icor3].normal[2] );
		stats.text_num++;
	}

	if (texture_idx>texture_num)
	    texture_idx=0;
	if (texture_num>0)
    {
        // Texture filename.
		fileout->fprintf( "tex %s\n", texture[texture_idx].name );
		stats.text_num++;
		// Texture binding.
		fileout->fprintf( "bind r vertex\n" );
		stats.text_num++;
		// Texture coordinates at each node.
		for ( icor3 = 0; icor3 < vertex_num; icor3++ )
        {
			fileout->fprintf( "r %f %f\n", vertex[icor3].tex_uv[0],
				vertex[icor3].tex_uv[1] );
			stats.text_num++;
		}

	}

	// Report.
	printf ( "\n" );
	printf ( "SMF_WRITE - Wrote %d text lines.\n", stats.text_num );

	return true;
}
