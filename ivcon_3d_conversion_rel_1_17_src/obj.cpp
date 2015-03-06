/******************************************************************************/
// Author:  John Burkardt
// Change: 02 Mar 2007 Tomasz Lis adjusted to new IVCONV class, minor fixes

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - Full 32-bit vertices support
// - Full 32-bit faces support, unlimited face order
// - No meshes support (but possible to add)
// - No materials support (but possible to add)
// - No textures support
// - No Texture UV support
// - No camera/view support
// - No Environment or lights support
// - Partial shapes support (only one type of lines; possible to extend)
// Format support note - writing:
// - Full 32-bit vertices support
// - Full 32-bit faces support, unlimited face order
// - No meshes support (but possible to add)
// - No materials support (but possible to add)
// - No textures support
// - No Texture UV support
// - No camera/view support
// - No Environment or lights support
// - Partial shapes support (possible to extend)

/*
Example:

#  magnolia.obj

mtllib ./vp.mtl

g
v -3.269770 -39.572201 0.876128
v -3.263720 -39.507999 2.160890
...
v 0.000000 -9.988540 0.000000
g stem
s 1
usemtl brownskn
f 8 9 11 10
f 12 13 15 14
...
f 788 806 774

*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

/******************************************************************************/

bool IVCONV::obj_read ( FILE *filein )

/******************************************************************************/
/*
Purpose:

OBJ_READ reads a Wavefront OBJ file.

    Author: John Burkardt
    Modified: 20 Oct 1998
    Modified: 02 Mar 2007 Tomasz Lis
*/
{
	//Initialize. 
	unsigned long vertex_normal_num = 0;
	unsigned long line_dex_num = 0;
	array<unsigned long>  line_dex;
	unsigned long lines_at_start = shape_num;
	array<vec3>  normal_temp;

	//Read the next line of the file into INPUT. 
	char input[LINE_MAX_LEN];
	while ( fgets ( input, LINE_MAX_LEN, filein ) != NULL )
    {
		stats.text_num++;
		//Advance to the first nonspace character in INPUT. 
        char *next;
		for ( next = input; *next != '\0' && isspace(*next); next++ ) {}

		//Skip blank lines and comments. 
		if ( *next == '\0' ) continue;
		
		if ( (*next=='#') || (*next=='$') )
        {
			stats.comment_num++;
			continue;
		}
        char  token[LINE_MAX_LEN];
        unsigned int width;
		//Extract the first word in this line. 
		sscanf ( next, "%s%n", token, &width );
		//Set NEXT to point to just after this token. 
		next = next + width;

		// BEVEL
		// Bevel interpolation.
		if ( leqi ( token, "BEVEL" )  )
        {
			continue;
		} else

		// BMAT
		// Basis matrix.
		if ( leqi ( token, "BMAT" )  )
        {
			continue;
		} else

		// C_INTERP
		// Color interpolation.
		if ( leqi ( token, "C_INTERP" )  )
        {
			continue;
		} else

		// CON
		// Connectivity between free form surfaces.
		if ( leqi ( token, "CON" )  )
        {
			continue;
		} else

		// CSTYPE
		// Curve or surface type.
		if ( leqi ( token, "CSTYPE" )  )
        {
			continue;
		} else

		// CTECH
		// Curve approximation technique.
		if ( leqi ( token, "CTECH" )  )
        {
			continue;
		} else

		// CURV
		// Curve.
		if ( leqi ( token, "CURV" )  )
        {
			continue;
		} else

		// CURV2
		// 2D curve.
		if ( leqi ( token, "CURV2" )  )
        {
			continue;
		} else

		// D_INTERP
		// Dissolve interpolation.
		if ( leqi ( token, "D_INTERP" )  )
        {
			continue;
		} else

		// DEG
		// Degree.
		if ( leqi ( token, "DEG" )  )
        {
			continue;
		} else

		// END
		// End statement.
		if ( leqi ( token, "END" )  )
        {
			continue;
		} else

		/*  
		F V1 V2 V3
		or
		F V1/VT1/VN1 V2/VT2/VN2 ...
		or
		F V1//VN1 V2//VN2 ...
		
		  Face.
		  A face is defined by the vertices.
		  Optionally, slashes may be used to include the texture vertex
		  and vertex normal indices.
		  
			OBJ line node indices are 1 based rather than 0 based.
			So we have to decrement them before loading them into FACE.
		*/
		if ( leqi ( token, "F" )  )
        {
			unsigned int ivert = 0;
			face[face_num]=Face();
			face[face_num].order = 0;
			//Read each item in the F definition as a token,
			//and then take it apart.
			for ( ;; )
            {
                char  token2[LINE_MAX_LEN];
				int count = sscanf ( next, "%s%n", token2, &width );
				next = next + width;
				if ( count != 1 ) break;
                unsigned int node;
				count = sscanf ( token2, "%d%n", &node, &width );
                char *next2;
				next2 = token2 + width;
				if ( count != 1 ) break;
				// if ( ivert < ORDER_MAX && face_num < FACE_MAX ) 
				{
                    unsigned long vertidx = node-1;
					face[face_num].vertices[ivert] = vertidx;
					//vertex[vertidx].material = 0;
					face[face_num].order++;
				} 
				//If there's a slash, skip to the next slash, and extract the
				//index of the normal vector.
				if ( *next2 == '/' )
				{
                char *next3;
					for ( next3 = next2 + 1; next3 < token2 + LINE_MAX_LEN; next3++ )
						if ( *next3 == '/' )
                        {
							next3 = next3 + 1;
							count = sscanf ( next3, "%d%n", &node, &width );
							node = node - 1;
							if ( 0 <= node && node < vertex_normal_num )
								for ( int i = 0; i < 3; ++i )
                                {
                                    unsigned long vertidx = face[face_num].vertices[ivert];
									vertex[vertidx].normal[i] = normal_temp[node][i];
								}
							break;
						}
               }
				ivert = ivert + 1;
			} //end for (...)
			++face_num;
		} else
		
		// G  
		// Group name.
		if ( leqi ( token, "G" )  )
        {
			continue;
		} else

		// HOLE
		// Inner trimming hole.
		if ( leqi ( token, "HOLE" )  )
        {
			continue;
		} else

		// L  
		// I believe OBJ line node indices are 1 based rather than 0 based.
		// So we have to decrement them before loading them into LINE_DEX.
		if ( leqi(token,"L" ) )
        {
			shape[shape_num]=Shape();
            shape[shape_num].rgb = vec3(0.9,0.9,0.9);
            shape[shape_num].order=0;
			for ( ;; )
            {
                unsigned int node;
				int count = sscanf ( next, "%d%n", &node, &width );
				next = next + width;
				if ( count != 1 )
					break;
                line_dex[line_dex_num]=node-1;
                line_dex_num++;
				shape[shape_num].order++;
			} //end for ( ;; )
			shape_num++;
		} else
		
		// LOD
		// Level of detail.
		if ( leqi ( token, "LOD" )  )
        {
			continue;
		} else

		// MG
		// Merging group.
		if ( leqi ( token, "MG" )  )
        {
			continue;
		} else

		// MTLLIB
		// Material library.
		if ( leqi ( token, "MTLLIB" )  )
        {
			continue;
		} else

		// O
		// Object name.
		if ( leqi ( token, "O" )  )
        {
			continue;
		} else

		// P
		// Point.
		if ( leqi ( token, "P" )  )
        {
			continue;
		} else

		// PARM
		// Parameter values.
		if ( leqi ( token, "PARM" )  )
        {
			continue;
		} else

		// S  
		// Smoothing group
		if ( leqi ( token, "S" )  )
        {
			continue;
		} else

		// SCRV
		// Special curve.
		if ( leqi ( token, "SCRV" )  )
        {
			continue;
		} else

		// SHADOW_OBJ
		// Shadow casting.
		if ( leqi ( token, "SHADOW_OBJ" )  )
        {
			continue;
		} else

		// SP
		// Special point.
		if ( leqi ( token, "SP" )  )
        {
			continue;
		} else

		// STECH
		// Surface approximation technique.
		if ( leqi ( token, "STECH" )  )
        {
			continue;
		} else

		// STEP
		// Stepsize.
		if ( leqi ( token, "CURV" )  )
        {
			continue;
		} else

		// SURF
		// Surface.
		if ( leqi ( token, "SURF" )  )
        {
			continue;
		} else

		// TRACE_OBJ
		// Ray tracing.
		if ( leqi ( token, "TRACE_OBJ" )  )
        {
			continue;
		} else

		// TRIM
		// Outer trimming loop.
		if ( leqi ( token, "TRIM" )  )
        {
			continue;
		} else

		// USEMTL  
		// Material name.
		if ( leqi ( token, "USEMTL" )  )
        {
			continue;
		} else
		
		// V X Y Z W
		// Geometric vertex.
		// W is optional, a weight for rational curves and surfaces.
		// The default for W is 1.
		if ( leqi ( token, "V" )  )
        {
            float r1,r2,r3;
			sscanf ( next, "%e %e %e", &r1, &r2, &r3 );
			vertex[vertex_num]=Vertex();
			vertex[vertex_num].cor3=vec3(r1,r2,r3);
			vertex_num++;
		} else

		// VN
		// Vertex normals.
		if ( leqi ( token, "VN" )  )
        {
            float r1,r2,r3;
			sscanf ( next, "%e %e %e", &r1, &r2, &r3 );
            normal_temp[vertex_normal_num] = vec3(r1,r2,r3);
			vertex_normal_num++;
		} else

		// VT
		// Vertex texture.
		if ( leqi ( token, "VT" )  ) {
			continue;
		} else

		// VP
		// Parameter space vertices.
		if ( leqi ( token, "VP" )  ) {
			continue;
		} else

		// Unrecognized  
		{
			stats.bad_num++;
		}
		
  } // end while ( fgets ( input, ...

  //Now, let's take care of the lines points coordinates
  // (we must convert vertices to shape coordinates)
  {
    array<Vertex> lnvert;
    for (unsigned long ivert=0;ivert<line_dex_num;ivert++)
    {
        lnvert[ivert]=Vertex();
        unsigned long vertidx=line_dex[ivert];
        if (vertidx<vertex_num)
            lnvert[ivert]=vertex[vertidx];
    }
    line_vertices_to_shape_coords(lnvert,line_dex_num,lines_at_start);
  }
  return true;
}
/******************************************************************************/

bool IVCONV::obj_write ( FILE *fileout )

/******************************************************************************/
/*
Purpose:

OBJ_WRITE writes a Wavefront OBJ file.

    Author: John Burkardt
    Modified: 01 September 1998
    Modified: 02 Mar 2007 Tomasz Lis
*/
{
	//Initialize. 
	stats.text_num = 0;
	float w = 1.0;
	unsigned long line_dex_num = 0;
    array<Vertex> lnvert;
    shape_coords_to_line_vertices(lnvert,line_dex_num,0);
	
	fprintf ( fileout, "# %s created by IVCON.\n", fileout_name );
	fprintf ( fileout, "# Original data in %s.\n", filein_name );
	fprintf ( fileout, "\n" );
//	fprintf ( fileout, "g %s\n", object_name );
	fprintf ( fileout, "\n" );
	stats.text_num+=4;

	// V: vertex coordinates. 
	unsigned long i;
	for ( i = 0; i < vertex_num; i++ )
    {
		fprintf ( fileout, "v %f %f %f %f\n", 
			vertex[i].cor3[0], vertex[i].cor3[1], vertex[i].cor3[2], w );
		stats.text_num++;
	}
    // Additional vertices from lines definitions
	for ( unsigned long i = 0; i < line_dex_num; i++ )
    {
		fprintf ( fileout, "v %f %f %f %f\n", 
			lnvert[i].cor3[0], lnvert[i].cor3[1], lnvert[i].cor3[2], w );
		stats.text_num++;
	}
	
	// VN: Vertex face normal vectors. 
	if ( face_num > 0 )
    {
		fprintf ( fileout, "\n" );
		stats.text_num++;
	}
	for ( unsigned long iface = 0; iface < face_num; iface++ )
		for ( unsigned int ivert = 0; ivert < face[iface].order; ivert++ )
        {
			unsigned long vertidx = face[iface].vertices[ivert];
			fprintf ( fileout, "vn %f %f %f\n", vertex[vertidx].normal[0],
				vertex[vertidx].normal[1], vertex[vertidx].normal[2] );
			stats.text_num++;
		}

	// F: faces. 
	if ( face_num > 0 )
    {
		fprintf ( fileout, "\n" );
		stats.text_num++;
	}
	unsigned long indexvn = 0;
	for ( unsigned long iface = 0; iface < face_num; iface++ )
    {
		fprintf ( fileout, "f" );
		for ( unsigned int ivert = 0; ivert < face[iface].order; ivert++ )
        {
			indexvn++;
			fprintf ( fileout, " %d//%d", face[iface].vertices[ivert]+1, indexvn );
		}
		fprintf ( fileout, "\n" );
		stats.text_num++;
	}

	// L: lines. 
	if ( shape_num > 0 )
    {
		fprintf ( fileout, "\n" );
		stats.text_num++;
	}
    unsigned long vertidx = vertex_num;
	for ( unsigned long i = 0; i < shape_num; i++ )
      if (shape[i].order>0)
      {
          fprintf ( fileout, "l" );
          for ( unsigned long k = 0; k < shape[i].order; k++ )
          {
              if (vertidx < vertex_num+line_dex_num)
                  fprintf ( fileout, " %lu", vertidx+1 );
              vertidx++;
          }
          fprintf ( fileout, "\n" );
          stats.text_num++;
          if (vertidx >= vertex_num+line_dex_num) break;
      }
	
	fprintf ( fileout, "\n" );
	stats.text_num++;

	// Report.
	printf ( "\n" );
	printf ( "OBJ_WRITE - Wrote %d text lines.\n", stats.text_num );
	
	return true;
}
/******************************************************************************/

