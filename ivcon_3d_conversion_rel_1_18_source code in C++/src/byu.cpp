/******************************************************************************/
// Author:  John Burkardt
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core

// Purpose:
//   Metthods for support of Movie.BYU surface geometry files

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - Full 32-bit vertices support
// - Full 32-bit faces support, face order limited to about LINE_MAX_LEN/10
// - One mesh supported (possible to extend)
// - No materials support
// - No textures support
// - No texture UV supported
// - No default view or cameras supported
// - No environment or lights support
// - No shapes supported
// Format support note - writing:
// - Full 32-bit vertices support
// - Full 32-bit faces support, unlimited face order
// - One mesh supported (possible to extend)
// - No materials support
// - No textures support
// - No texture UV supported
// - No default view or cameras supported
// - No environment or lights support
// - No shapes supported

/*
Discussion:
  A Movie.BYU surface geometry file contains 4 groups of data.

  The first group of data is a single line, containing 4 integers,
  each one left justified in 8 columns.  The integers are:

  PART_NUM, VERTEX_NUM, POLY_NUM, EDGE_NUM,

  that is, the number of parts or objects, the number of vertices or nodes,
  the number of polygons or faces, and the number of edges.

  The second group of data is a single line, containing 2 integers,
  each one left justified in 8 columnes.  The integers are:

  POLY1, POLY2,

  the starting and ending polygon numbers.  Presumably, this means
  that the polygons are labeled POLY1, POLY1+1, ..., POLY2, comprising
  a total of POLY_NUM polygons.

  The third group is the X, Y and Z coordinates of all the vertices.
  These may be written using a FORTRAN format of 6E12.5, which
  crams two sets of (X,Y,Z) data onto each line, with each real value
  written in an exponential format with 5 places after the decimal.
  However, it is generally possible to write the XYZ coordinate data
  for each vertex on a separate line.

  The fourth group defines the polygons in terms of the vertex indices.
  For each polygon, the vertices that make up the polygon are listed in
  counterclockwise order.  The last vertex listed is given with a negative
  sign to indicate the end of the list.  All the vertices for all the
  polygons are listed one after the other, using a format that puts
  up to 10 left-justified integers on a line, with each integer occupying
  8 spaces.

Example:

1       8       6      24
1       6
0.00000E+00 0.00000E+00 0.00000E+00
1.00000E+00 0.00000E+00 0.00000E+00
1.00000E+00 2.00000E+00 0.00000E+00
0.00000E+00 2.00000E+00 0.00000E+00
0.00000E+00 0.00000E+00 1.00000E+00
1.00000E+00 0.00000E+00 1.00000E+00
1.00000E+00 2.00000E+00 1.00000E+00
0.00000E+00 2.00000E+00 1.00000E+00
4       3       2      -1
5       6       7      -8
1       5       8      -4
4       8       7      -3
3       7       6      -2
2       6       5      -1
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

/******************************************************************************/

bool IVCONV::byu_read ( FileIO *filein )

/******************************************************************************/
/*
Purpose:
  BYU_READ reads graphics data from a Movie.BYU surface geometry file.

  Author: John Burkardt
  Modified: 24 May 2001
*/
{
	stats.text_num = 0;

    if (debug)
        printf ( "BYU_READ: reading header line\n" );
	char input[LINE_MAX_LEN];
	if ( filein->fgets ( input, LINE_MAX_LEN ) == NULL )
    {
        if (debug)
            printf ( "BYU_READ: Error, no header line!\n" );
		return false;
	}
	stats.text_num++;
	unsigned long vertex_num_new=0;
	unsigned long face_num_new=0;
	unsigned long edge_num=0;
	unsigned long part_num=0;
	long count = sscanf ( input, "%lu %lu %lu %lu", &part_num,&vertex_num_new,&face_num_new,&edge_num );
    if (count<4)
    {
      if (debug)
          printf ( "BYU_READ: Warning, unrecognized header data!\n");
    }

	unsigned long poly1=1;
	unsigned long poly2=face_num_new;
	if ( filein->fgets ( input, LINE_MAX_LEN ) == NULL )
    {
        if (debug)
            printf ( "BYU_READ: Warning, no polygons range line\n" );
	} else
	{
        stats.text_num++;
        long count = sscanf ( input, "%lu %lu", &poly1, &poly2 );
        if (count<2)
        {
          if (debug)
            printf ( "BYU_READ: Warning, unrecognized polygons range line\n" );
        }
    }
    unsigned long mesh_idx=mesh_num;
    mesh[mesh_idx]=Mesh();
	mesh_num++;
	
    if (debug)
        printf ( "BYU_READ: reading vertices\n" );
    unsigned long vertex_num_base=vertex_num;
	unsigned long vert_idx;
	for ( vert_idx=0; vert_idx<vertex_num_new; vert_idx++ )
    {
		float x=0.0,y=0.0,z=0.0;
		if ( filein->fgets(input,LINE_MAX_LEN) == NULL )
        {
            if (debug)
                printf ( "BYU_READ: Warning, no data when reading vertex %lu\n",vert_idx );
		}
		else
		{
            stats.text_num++;
            long count = sscanf ( input, "%f %f %f", &x,&y,&z );
            if ((count<3)||isnan(x)||isnan(y)||isnan(z))
            {
                if (debug)
                    printf ( "BYU_READ: Warning, vertex data unrecognized!\n" );
        		if (isnan(x)) x=0.0;
        		if (isnan(y)) y=0.0;
        		if (isnan(z)) z=0.0;
        	}
        }
        vertex[vertex_num] = Vertex();
		vertex[vertex_num].cor3 = vec3(x,y,z);
		vertex_num++;
	}

    if (debug)
        printf ( "BYU_READ: reading faces\n" );
	unsigned long iface;
	for ( iface=0; iface<face_num_new; iface++ )
    {
		if ( filein->fgets(input,LINE_MAX_LEN) == NULL )
        {
            if (debug)
                printf ( "BYU_READ: Warning, no data when reading face %lu\n",iface );
			continue;
		}
		stats.text_num++;
		face[face_num]=Face();
		face[face_num].mesh=mesh_idx;
		char *next = input;
		bool face_end=false;
		unsigned long ivert=0;
		do
        {
            long ival=0;
            int width;
			long count = sscanf ( next, "%ld%n", &ival, &width );
			next+=width;
			if (count<1)
            {
                if (debug)
                  printf ( "BYU_READ: Warning, data not recognized when reading face %lu vertex\n",iface );
                  break;
			}
			if ( ival>0 )
			{
                face[face_num].vertices[ivert]= ival-1+vertex_num_base;
            }
			else
			{
                face[face_num].vertices[ivert]=-ival-1+vertex_num_base;
                face_end=true;
            }
            ivert++;
        } while (!face_end);
		face[face_num].order=ivert;
        face_num++;
	}
	
    // Report.
	printf ( "\n" );
	printf ( "BYU_READ - Read %lu text lines.\n", stats.text_num );
	
	return true;
}
/******************************************************************************/

bool IVCONV::byu_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  BYU_WRITE writes out the graphics data as a Movie.BYU surface geometry file.

  Author: John Burkardt
  Modified: 24 May 2001
  Modified: 18 Mar 2007 Tomasz Lis
*/
{
	stats.text_num = 0;

	unsigned long edge_num = 0;
    {
      unsigned long iface;
      for (iface=0; iface<face_num; iface++ )
        edge_num+=face[iface].order;
	}
	
	unsigned long part_num = 1;
	fileout->fprintf("%lu %lu %lu %lu\n", part_num, vertex_num, face_num, edge_num );
	stats.text_num++;
    //Starting and ending face numbers
	fileout->fprintf("%d %lu\n",1,face_num);
	stats.text_num++;

    //Writing vertices	
    if (debug)
        printf ( "BYU_WRITE: writing %lu vertices\n", vertex_num );
    unsigned long vert_idx;
	for ( vert_idx=0; vert_idx<vertex_num; vert_idx++ )
    {
        vec3 coord=vec3(vertex[vert_idx].cor3);
		fileout->fprintf("%f %f %f\n", coord[0],coord[1],coord[2]);
		stats.text_num++;
	}

    if (debug)
        printf ( "BYU_WRITE: writing %lu faces\n", face_num );
	unsigned long iface;
	for ( iface = 0; iface < face_num; iface++ )
    {
        unsigned long ivert;
		for ( ivert = 0; ivert < face[iface].order; ivert++ )
		{
			long jp = face[iface].vertices[ivert] + 1;
			if ( ivert == face[iface].order-1 )
				jp = -jp;
			fileout->fprintf("%ld ",jp);
		}
		fileout->fprintf ( "\n" );
		stats.text_num++;
	}

	// Report.
	printf ( "\n" );
	printf ( "BYU_WRITE - Wrote %d text lines.\n", stats.text_num );
	return true;
}
/******************************************************************************/
