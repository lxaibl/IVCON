/******************************************************************************/
// Author:  John Burkardt
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core

// Purpose:
//   Methods for support of Advanced Visual Systems (AVS) Unstructured Cell Data

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - No read support
// Format support note - writing:
// - unknown

/*
   NOTE: support of this format is unfinished, it probably won't work.
*/
/*
Examples:

#  cube.ucd created by IVREAD.
#
#  Material RGB to hue map:
#
#  material    R    G  B   Alpha     Hue
#
#    0   0.94  0.70  0.15  1.000   0.116
#    1   0.24  0.70  0.85  1.000   0.541
#    2   0.24  0.00  0.85  1.000   0.666
#
#  The node data is
#    node # / material # / RGBA / Hue
#
8  6  6  0  0
0  0.0  0.0  0.0
1  1.0  0.0  0.0
2  1.0  1.0  0.0
3  0.0  1.0  0.0
4  0.0  0.0  1.0
5  1.0  0.0  1.0
6  1.0  1.0  1.0
7  0.0  1.0  1.0
0  0  quad  0  1  2  3
1  0  quad  0  4  5  1
2  0  quad  1  5  6  2
3  0  quad  2  6  7  3
4  0  quad  3  7  4  0
5  0  quad  4  7  6  5
3  1 4 1
material, 0...2
RGBA, 0-1/0-1/0-1/0-1
Hue, 0-1
0  0  0.94  0.70  0.15  1.0  0.116
1  0  0.94  0.70  0.15  1.0  0.116
2  0  0.94  0.70  0.15  1.0  0.116
3  0  0.94  0.70  0.15  1.0  0.116
4  1  0.24  0.70  0.85  1.0  0.541
5  1  0.24  0.70  0.85  1.0  0.541
6  2  0.24  0.24  0.85  0.0  0.666
7  2  0.24  0.24  0.85  0.0  0.666
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

/******************************************************************************/

bool IVCONV::ucd_write ( FILE *fileout )

/******************************************************************************/
/*
Purpose:
  UCD_WRITE writes graphics data to an AVS UCD file.

Author: John Burkardt
Modified: 22 May 1999
*/
{
	float a;
	float b;
	float g;
	float h;
	int i;
	int j;
	float r;
	
	stats.text_num = 0;
	
	fprintf ( fileout, "#  %s created by IVREAD.\n", fileout_name );
	fprintf ( fileout, "#\n" );
	stats.text_num += 2;

    // Write RGB-TO-HUE comment
	fprintf ( fileout, "#  Material RGB to Hue map:\n" );
	fprintf ( fileout, "#\n" );
	fprintf ( fileout, "#  material    R    G      B     Alpha  Hue\n" );
	fprintf ( fileout, "#\n" );
	stats.text_num += 4;
	for ( j = 0; j < material_num; ++j )
    {
		r = material[j].rgb[0];
		g = material[j].rgb[1];
		b = material[j].rgb[2];
		a = material[j].alpha;
		h = rgb_to_hue ( r, g, b );
		fprintf ( fileout, "#  %d %f %f %f %f %f\n", j, r, g, b, a, h );
		stats.text_num++;
	}
	
	fprintf ( fileout, "#\n" );
	stats.text_num++;

    // Write node data
	fprintf ( fileout, "#  The node data is\n" );
	fprintf ( fileout, "#    node # / material # / RGBA / Hue\n" );
	fprintf ( fileout, "#\n" );
	stats.text_num += 3;
	
	fprintf ( fileout, "%d %d 6 0 0\n", vertex_num, face_num );
	stats.text_num++;
	
	for ( j = 0; j < vertex_num; ++j )
    {
		fprintf ( fileout, "%d %f %f %f\n", j, vertex[j].cor3[0], vertex[j].cor3[1],
			vertex[j].cor3[2] );
		stats.text_num++;
	}

	// NOTE: UCD only accepts triangles and quadrilaterals, not higher order
	// polygons.  We would need to break polygons up to proceed.
	for ( j = 0; j < face_num; ++j )
    {
		
		fprintf ( fileout, "%d %d", j, get_face_major_material(j) );
		
		if ( face[j].order == 3 ) {
			fprintf ( fileout, " tri" );
		}
		else if ( face[j].order == 4 ) {
			fprintf ( fileout, " quad" );
		}
		else {
			fprintf ( fileout, " ???" );
		}
		
		for ( i = 0; i < face[j].order; ++i ) {
			fprintf ( fileout, "%d", face[j].vertices[i] );
		}
		fprintf ( fileout, "\n" );
		stats.text_num++;
		
	}
	
	fprintf ( fileout, "3  1  4  1\n" );
	fprintf ( fileout, "material, 0...%d\n", material_num - 1 );
	fprintf ( fileout, "RGBA, 0-1/0-1/0-1/0-1\n" );
	fprintf ( fileout, "Hue, 0-1\n" );
	stats.text_num += 4;
	
	for ( j = 0; j < vertex_num; ++j )
    {
        int imat;
		imat = vertex[j].material;
		r = material[imat].rgb[0];
		g = material[imat].rgb[1];
		b = material[imat].rgb[2];
		a = material[imat].alpha;
		h = rgb_to_hue ( r, g, b );
		
		fprintf ( fileout, "%d %d %f %f %f %f %f\n", j, imat, r, g, b, a, h );
		stats.text_num++;
	}

	// Report.
	printf ( "\n" );
	printf ( "UCD_WRITE: Wrote %d text lines.\n", stats.text_num );
	
	return true;
}
/******************************************************************************/

