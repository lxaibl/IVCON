/******************************************************************************/
// Author:  John Burkardt
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core
// Change: 31 Mar 2008 Tomasz Lis fixed and described

// Purpose:
//   Methods for support of TECPLOT files

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - No read support
// Format support note - writing:
// - Full vertices support
// - Full faces support, max face order 3 (auto reduced on save)
// - No meshes support, only faces
// - Limited materials support - ambient color only
// - No textures supported
// - No texture UV supported
// - No camera/view support
// - No Environment or lights support
// - No shapes support
/*
Example:

TITLE = "cube.tec created by IVCON."
VARIABLES = "X", "Y", "Z", "R", "G", "B"
ZONE T="TRIANGLES", N=8, E=12, F=FEPOINT, ET=TRIANGLE
0.0 0.0 0.0 0.0 0.0 0.0
1.0 0.0 0.0 1.0 0.0 0.0
1.0 1.0 0.0 1.0 1.0 0.0
0.0 1.0 0.0 0.0 1.0 0.0
0.0 0.0 1.0 0.0 0.0 1.0
1.0 0.0 1.0 1.0 0.0 1.0
1.0 1.0 1.0 1.0 1.0 1.0
0.0 1.0 1.0 0.0 1.0 1.0
1 4 2
2 4 3
1 5 8
1 2 5
2 6 5
2 3 6
3 7 6
3 4 7
4 8 7
4 1 8
5 6 8
6 7 8
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

/******************************************************************************/

bool IVCONV::tec_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  TEC_WRITE writes graphics information to a TECPLOT file.

Discussion:
  The file format used is appropriate for 3D finite element surface
  zone data.  Polygons are decomposed into triangles where necessary.

Author: John Burkardt
Modified: 31 Mar 2008 Tomasz Lis
*/
{
	float b;
	int face2[3];
	float g;
	int icor3;
	int iface;
	int imat;
	int j;
	int face_num2;
	float r;

	// Determine the number of triangular faces.
	face_num2 = 0;
	for ( iface = 0; iface < face_num; iface++ )
    {
		for ( j = 0; j < face[iface].order - 2; ++j )
			face_num2 = face_num2 + 1;
	}
	
	stats.text_num = 0;
	
	fileout->fprintf( "TITLE = \"%s created by IVCON.\"\n", fileout_name );
	fileout->fprintf( "VARIABLES = \"X\", \"Y\", \"Z\", \"R\", \"G\", \"B\"\n" );
	fileout->fprintf( "ZONE T=\"%s\", N=%d, E=%d, F=%s, ET=%s\n",
        "TRIANGLES",vertex_num, face_num2,"FEPOINT","TRIANGLE" );
	
	stats.text_num += 3;

	// Write out X, Y, Z, R, G, B per node.
	for ( icor3 = 0; icor3 < vertex_num; icor3++ )
    {
		imat = vertex[icor3].material;
		r = material[imat].ambient[0];
		g = material[imat].ambient[1];
		b = material[imat].ambient[2];
		fileout->fprintf( "%f %f %f %f %f %f\n", vertex[icor3].cor3[0], vertex[icor3].cor3[1], vertex[icor3].cor3[2], r, g, b );
		stats.text_num++;
	}

	// Do the next face.
	for ( iface = 0; iface < face_num; iface++ )
    {
        // Break the face up into triangles, anchored at node 1.
		for ( j = 0; j < face[iface].order - 2; ++j )
        {
			face2[0] = face[iface].vertices[  0] + 1;
			face2[1] = face[iface].vertices[j+1] + 1;
			face2[2] = face[iface].vertices[j+2] + 1;
			fileout->fprintf( "%d %d %d\n", face2[0], face2[1], face2[2] );
			stats.text_num++;
		}
		
	}
	// Report.
	printf ( "\n" );
	printf ( "TEC_WRITE - Wrote %lu text lines.\n", stats.text_num );
	
	return true;
}
/******************************************************************************/
