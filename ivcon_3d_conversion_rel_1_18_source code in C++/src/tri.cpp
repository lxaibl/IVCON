/******************************************************************************/
// Author:  John Burkardt 
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core

// Purpose:
//   Methods for support of a simple triangle format requested by Greg Hood

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - unknown
// Format support note - writing:
// - unknown

/*
   NOTE: support of this format is unfinished, it probably won't work.
*/
/*
Example:

12                    <-- Number of triangles

(x,y,z) and (nx,ny,nz) of normal vector at:

0.0 0.0 0.0 0.3 0.3 0.3   node 1 of triangle 1.
1.0 0.0 0.0 0.3 0.1 0.3   node 2 of triangle 1,
0.0 1.0 0.0 0.3 0.1 0.3   node 3 of triangle 1,
1.0 0.5 0.0 0.3 0.1 0.3   node 1 of triangle 2,
...
0.0 0.5 0.5 0.3 0.1 0.3   node 3 of triangle 12.
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

/******************************************************************************/

bool IVCONV::tria_read ( FileIO *filein )

/******************************************************************************/
/*
Purpose:
  TRIA_READ reads an ASCII triangle file.

    Modified: 22 June 1999

    Author: John Burkardt
*/
{
	float cvec[3];
	int   icor3;
	int   iface;
	int   iface_hi;
	int   iface_lo;
	int   ivert;
	int   face_num2;
	float r1;
	float r2;
	float r3;
	float r4;
	float r5;
	float r6;
	/*
	Get the number of triangles.
	*/
	char input[LINE_MAX_LEN];
	filein->fgets(input, LINE_MAX_LEN );
	stats.text_num++;
	sscanf ( input, "%d", &face_num2 );
	/*
	For each triangle:
	*/
	iface_lo = face_num;
	iface_hi = face_num + face_num2;
	
	for ( iface = iface_lo; iface < iface_hi; iface++ ) {
		
		// if ( iface < FACE_MAX ) 
		{
			face[iface].order = 3;
//			face[iface].material = 0;
		}
		/*
		For each face:
		*/
		for ( ivert = 0; ivert < face[iface].order; ivert++ ) {
			
			filein->fgets(input, LINE_MAX_LEN);
			stats.text_num++;
			sscanf ( input, "%e %e %e %e %e %e", &r1, &r2, &r3, &r4, &r5, &r6 ); 
			
			cvec[0] = r1;
			cvec[1] = r2;
			cvec[2] = r3;
			
			if ( vertex_num < 1000 ) {
				icor3 = -1;//!!!! rcol_find ( cor3, vertex_num, cvec );
			}
			else {
				icor3 = -1;
			}
			
			if ( icor3 == -1 ) {
				icor3 = vertex_num;
//				if ( vertex_num < COR3_MAX ) 
				{
					vertex[vertex_num].cor3= cvec;
				}
				vertex_num = vertex_num + 1;
			}
			else {
				stats.dup_num++;
			}
			
			// if ( iface < FACE_MAX ) 
			{
				
				face[iface].vertices[ivert] = icor3;
				vertex[icor3].material = 0;
				vertex[icor3].normal[0] = r4;
				vertex[icor3].normal[1] = r5;
				vertex[icor3].normal[2] = r6;
			}
			
		}
	}
	face_num += face_num2;
	
	return true;
}
/**********************************************************************/

bool IVCONV::tria_write ( FileIO *fileout )

/**********************************************************************/

/*
Purpose:

TRIA_WRITE writes the graphics data to an ASCII "triangle" file.

Discussion:

This is just a private format that Greg Hood requested from me.

Example:

12                    <-- Number of triangles

(x,y,z) and (nx,ny,nz) of normal vector at:

0.0 0.0 0.0 0.3 0.3 0.3   node 1 of triangle 1.
1.0 0.0 0.0 0.3 0.1 0.3   node 2 of triangle 1,
0.0 1.0 0.0 0.3 0.1 0.3   node 3 of triangle 1,
1.0 0.5 0.0 0.3 0.1 0.3   node 1 of triangle 2,
...
0.0 0.5 0.5 0.3 0.1 0.3   node 3 of triangle 12.

Modified:

10 June 1999

Author:

John Burkardt
*/
{
	int face2[3];
	int icor3;
	int iface;
	int jlo;
	int k;
	int face_num2;
	float nx;
	float ny;
	float nz;
	float x;
	float y;
	float z;
	
	stats.text_num = 0;
	/*
	Determine the number of triangular faces.
	*/
	face_num2 = 0;
	for ( iface = 0; iface < face_num; iface++ ) {
		for ( jlo = 0; jlo < face[iface].order - 2; jlo ++ ) {
			face_num2 = face_num2 + 1;
		}
	}
	
	fileout->fprintf("%d\n",face_num2);
	stats.text_num++;
	/*
	Do the next face.
	*/
	for ( iface = 0; iface < face_num; iface++ ) {
	/*
	Break the face up into triangles, anchored at node 1.
		*/
		for ( jlo = 0; jlo < face[iface].order - 2; jlo ++ ) {
			
			face2[0] = face[iface].vertices[    0];
			face2[1] = face[iface].vertices[jlo+1];
			face2[2] = face[iface].vertices[jlo+2];
			
			for ( k = 0; k < 3; ++k ) {
				
				icor3 = face2[k];
				
				x = vertex[icor3].cor3[0];
				y = vertex[icor3].cor3[1];
				z = vertex[icor3].cor3[2];
				
				nx = vertex[icor3].normal[0];
				ny = vertex[icor3].normal[1];
				nz = vertex[icor3].normal[2];
				
				fileout->fprintf("%f %f %f %f %f %f\n", x, y, z, nx, ny, nz );
				
				stats.text_num++;
				
			}
			
		}
		
	}
	/*
	Report.
	*/
	printf ( "\n" );
	printf ( "TRIA_WRITE - Wrote %d text lines.\n", stats.text_num );
	
	return true;
}
/******************************************************************************/

bool IVCONV::trib_read ( FileIO *filein )

/******************************************************************************/

/*
Purpose:

TRIB_READ reads a binary triangle file.

Example:

4 byte int = number of triangles

For each triangular face:

3 4-byte floats = coordinates of first node;
3 4-byte floats = components of normal vector at first node;
3 4-byte floats = coordinates of second node;
3 4-byte floats = components of normal vector at second node;
3 4-byte floats = coordinates of third node;
3 4-byte floats = components of normal vector at third node.

Modified:

22 June 1999

Author:

John Burkardt
*/
{
	float cvec[3];
	int icor3;
	int i;
	int iface;
	int iface_hi;
	int iface_lo;
	int ivert;
	int face_num2;
	/* 
	Read the number of triangles in the file.
	*/
	face_num2 = filein->long_int_read();
	stats.bytes_num += 4;
	/*
	For each (triangular) face,
    read the coordinates and normal vectors of three vertices,
	*/
	iface_lo = face_num;
	iface_hi = face_num + face_num2;
	
	for ( iface = iface_lo; iface < iface_hi; iface++ ) {
		
		// if ( iface < FACE_MAX ) 
		{
			face[iface].order = 3;
//			face[iface].material = 0;
		}
		
		for ( ivert = 0; ivert < face[iface].order; ivert++ ) {
			
			for ( i = 0; i < 3; ++i ) {
				cvec[i] = filein->float_read();
				stats.bytes_num += 4;
			}
			
			if ( vertex_num < 1000 ) {
				icor3 = -1;//!! rcol_find ( cor3, vertex_num, cvec );
			}
			else {
				icor3 = -1;
			}
			
			if ( icor3 == -1 ) {
				icor3 = vertex_num;
//				if ( vertex_num < COR3_MAX ) 
				{
					vertex[vertex_num].cor3 = cvec;
				}
				vertex_num = vertex_num + 1;
			}
			else
            {
				stats.dup_num++;
			}
			
			//if ( iface < FACE_MAX ) 
			{
				
				face[iface].vertices[ivert] = icor3;
				vertex[icor3].material = 0;
				
				for ( i = 0; i < 3; ++i ) {
					vertex[icor3].normal[i] = filein->float_read();
					stats.bytes_num += 4;
				}
				
			}
			
		}
	}
	
	face_num += face_num2;
	
	return true;
}
/**********************************************************************/

bool IVCONV::trib_write ( FileIO *fileout )

/**********************************************************************/

/*
Purpose:

TRIB_WRITE writes the graphics data to a binary "triangle" file.

Discussion:

This is just a private format that Greg Hood requested from me.

Example:

12   Number of triangles
0.0  x at node 1, triangle 1,
0.0  y at node 1, triangle 1,
0.0  z at node 1, triangle 1,
0.3  nx at node 1, triangle 1,
0.3  ny at node 1, triangle 1,
0.3  nz at node 1, triangle 1.
1.0  x at node 2, triangle 1,
...
0.7  nz at node 3, triangle 1.
1.2  x at node 1, triangle 2,
...
0.3  nz at node 3, triangle 2.
9.3  x at node 1, triangle 3,
...
0.3  nz at node 3, triangle 12.

Modified:

16 June 1999

Author:

John Burkardt
*/
{
	int face2[3];
	int icor3;
	int iface;
	int jlo;
	int k;
	int face_num2;
	float nx;
	float ny;
	float nz;
	float x;
	float y;
	float z;
	
	stats.bytes_num = 0;
	/*
	Determine the number of triangular faces.
	*/
	face_num2 = 0;
	for ( iface = 0; iface < face_num; iface++ ) {
		for ( jlo = 0; jlo < face[iface].order - 2; jlo ++ ) {
			face_num2 = face_num2 + 1;
		}
	}
	
	stats.bytes_num += fileout->long_int_write ( face_num2 );
	/*
	Do the next face.
	*/
	for ( iface = 0; iface < face_num; iface++ ) {
	/*
	Break the face up into triangles, anchored at node 1.
		*/
		for ( jlo = 0; jlo < face[iface].order - 2; jlo ++ ) {
			
			face2[0] = face[iface].vertices[    0];
			face2[1] = face[iface].vertices[jlo+1];
			face2[2] = face[iface].vertices[jlo+2];
			
			for ( k = 0; k < 3; ++k ) {
				
				icor3 = face2[k];
				
				x = vertex[icor3].cor3[0];
				y = vertex[icor3].cor3[1];
				z = vertex[icor3].cor3[2];
				
				nx = vertex[icor3].normal[0];
				ny = vertex[icor3].normal[1];
				nz = vertex[icor3].normal[2];
				
				stats.bytes_num += fileout->float_write ( x );
				stats.bytes_num += fileout->float_write( y );
				stats.bytes_num += fileout->float_write( z );
				stats.bytes_num += fileout->float_write( nx );
				stats.bytes_num += fileout->float_write( ny );
				stats.bytes_num += fileout->float_write( nz );
				
			}
			
		}
		
	}
	/*
	Report.
	*/
	printf ( "\n" );
	printf ( "TRIB_WRITE - Wrote %lu bytes.\n", stats.bytes_num );
	
	return true;
}
/******************************************************************************/

