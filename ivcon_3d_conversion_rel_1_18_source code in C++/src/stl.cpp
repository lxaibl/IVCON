/******************************************************************************/
// Author:  John Burkardt 
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core

// Purpose:
//   Methods for support of Stereolithography files

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - unknown
// Format support note - writing:
// - unknown

/*
   NOTE: support of this format is unfinished, it probably won't work.
*/
/*
STLA Examples:

solid MYSOLID
facet normal 0.4 0.4 0.2
outerloop
vertex  1.0 2.1 3.2
vertex  2.1 3.7 4.5
vertex  3.1 4.5 6.7
endloop
endfacet
...
facet normal 0.2 0.2 0.4
outerloop
vertex  2.0 2.3 3.4
vertex  3.1 3.2 6.5
vertex  4.1 5.5 9.0
endloop
endfacet
endsolid MYSOLID
*/
/*
STLB Example:

80 byte string = header containing nothing in particular

4 byte int = number of faces

For each face:

3 4-byte floats = components of normal vector to face;
3 4-byte floats = coordinates of first node;
3 4-byte floats = coordinates of second node;
3 4-byte floats = coordinates of third and final node;
2-byte int = attribute, whose value is 0.
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

/******************************************************************************/

bool IVCONV::stla_read ( FILE *filein )

/******************************************************************************/

/*
Purpose:
  STLA_READ reads an ASCII STL (stereolithography) file.

    Author: John Burkardt
    Modified: 20 Oct 1998
*/
{
	int   count;
	int   i;
	int   icor3;
	int   ivert;
	char *next;
	float r1;
	float r2;
	float r3;
	float r4;
	float temp[3];
	char  token[LINE_MAX_LEN];
	int   width;
	/*
	Read the next line of the file into INPUT. 
	*/
	char input[LINE_MAX_LEN];

	while ( fgets ( input, LINE_MAX_LEN, filein ) != NULL ) {
		
		stats.text_num++;
		/*
		Advance to the first nonspace character in INPUT. 
		*/
		for ( next = input; *next != '\0' && isspace(*next); next++ ) {
		}
		/*
		Skip blank lines and comments. 
		*/
		if ( *next == '\0' || *next == '#' || *next == '!' || *next == '$' ) {
			continue;
		}
		/*
		Extract the first word in this line. 
		*/
		sscanf ( next, "%s%n", token, &width );
		/*
		Set NEXT to point to just after this token. 
		*/
		next = next + width;
		/*
		FACET
		*/
		if ( leqi ( token, "facet" )  ) {
		/* 
		Get the XYZ coordinates of the normal vector to the face. 
			*/
			sscanf ( next, "%*s %e %e %e", &r1, &r2, &r3 );  
			
			// if ( face_num < FACE_MAX ) 
			{
				face[face_num].normal = vec3(r1,r2,r3);
			}
			
			fgets ( input, LINE_MAX_LEN, filein );
			stats.text_num++;
			
			ivert = 0;
			
			for ( ;; ) {
				
				fgets ( input, LINE_MAX_LEN, filein );
				stats.text_num++;
				
				count = sscanf ( input, "%*s %e %e %e", &r1, &r2, &r3 );
				
				if ( count != 3 ) {
					break;
				}
				
				temp[0] = r1;
				temp[1] = r2;
				temp[2] = r3;
				
				if ( vertex_num < 1000 ) {
					icor3 = -1;//!!!!!! rcol_find ( cor3, vertex_num, temp );
				}
				else {
					icor3 = -1;
				}
				
				if ( icor3 == -1 ) {
					
					icor3 = vertex_num;
					
//					if ( vertex_num < COR3_MAX ) 
					{
						vertex[vertex_num].cor3 = temp;
					}
					vertex_num = vertex_num + 1;
				}
				else {
					stats.dup_num++;
				}
				
				// if ( ivert < ORDER_MAX && face_num < FACE_MAX ) 
				{
					face[face_num].vertices[ivert] = icor3;
					vertex[icor3].material = 0;
					vertex[icor3].normal = face[face_num].normal;
				}
				
				ivert = ivert + 1;
			} 
			
			fgets ( input, LINE_MAX_LEN, filein );
			stats.text_num++;
			
			// if ( face_num < FACE_MAX ) 
			{
				face[face_num].order = ivert;
			} 
			
			++face_num;
			
		}
		/*
		COLOR 
		*/
		
		else if ( leqi ( token, "color" )  ) {
			sscanf ( next, "%*s %f %f %f %f", &r1, &r2, &r3, &r4 );
		}
		/* 
		SOLID 
		*/
		else if ( leqi ( token, "solid" )  ) {
			mesh_num++;
		}
		/* 
		ENDSOLID 
		*/
		else if ( leqi ( token, "endsolid" )  ) {
		}
		/* 
		Unexpected or unrecognized. 
		*/
		else {
			printf ( "\n" );
			printf ( "STLA_READ - Fatal error!\n" );
			printf ( "  Unrecognized first word on line.\n" );
			return false;
		}
		
  }
  return true;
}
/******************************************************************************/

bool IVCONV::stla_write ( FILE *fileout )

/******************************************************************************/

/*
Purpose:
  STLA_WRITE writes an ASCII STL (stereolithography) file.
  
  Discussion:
    The polygons in an STL file should only be triangular.  This routine 
    will try to automatically decompose higher-order polygonal faces into 
    suitable triangles, without actually modifying the internal graphics 
    data.
		  
    Author: John Burkardt
    Modified: 01 September 1998
*/
{
	int icor3;
	int iface;
	int jvert;
	int face_num2;
	/*
	Initialize.
	*/
	stats.text_num = 0;
	face_num2 = 0;
	
	fprintf ( fileout, "solid MYSOLID created by IVCON, original data in %s\n", 
		filein_name );
	
	stats.text_num++;
	
	for ( iface = 0; iface < face_num; iface++ ) {
		
		for ( jvert = 2; jvert < face[iface].order; jvert++ ) {
			
			face_num2 = face_num2 + 1;
			
			fprintf ( fileout, "  facet normal %f %f %f\n", 
				face[iface].normal[0], face[iface].normal[1], face[iface].normal[2] );
			
			fprintf ( fileout, "    outer loop\n" );
			
			icor3 = face[iface].vertices[0];
			fprintf ( fileout, "      vertex %f %f %f\n", 
				vertex[icor3].cor3[0], vertex[icor3].cor3[1], vertex[icor3].cor3[2] );
			
			icor3 = face[iface].vertices[jvert-1];
			fprintf ( fileout, "      vertex %f %f %f\n", 
				vertex[icor3].cor3[0], vertex[icor3].cor3[1], vertex[icor3].cor3[2] );
			
			icor3 = face[iface].vertices[jvert];
			fprintf ( fileout, "      vertex %f %f %f\n", 
				vertex[icor3].cor3[0], vertex[icor3].cor3[1], vertex[icor3].cor3[2] );
			
			fprintf ( fileout, "    endloop\n" );
			fprintf ( fileout, "  endfacet\n" );
			stats.text_num += 7;
		}
	}
	
	fprintf ( fileout, "endsolid MYSOLID\n" );
	stats.text_num++;
	/*
	Report.
	*/
	printf ( "\n" );
	printf ( "STLA_WRITE - Wrote %d text lines.\n", stats.text_num );
	
	if ( face_num != face_num2 ) {
		printf ( "  Number of faces in original data was %d.\n", face_num );
		printf ( "  Number of triangular faces in decomposed data is %d.\n",
			face_num2 );
	}
	
	return true;
}
/******************************************************************************/

bool IVCONV::stlb_read ( FileIO *filein )

/******************************************************************************/
/*
Purpose:
  STLB_READ reads a binary STL (stereolithography) file.

    Author: John Burkardt
    Modified: 24 May 1999
*/
{
	short int attribute = 0;
	char c;
	float cvec[3];
	int icor3;
	int i;
	int iface;
	int ivert;
	/* 
	80 byte Header.
	*/
	for ( i = 0; i < 80; ++i ) {
		c = filein->fgetc();
		if ( debug ) {
			printf ( "%d\n", c );
		}
		stats.bytes_num++;
	}
	/*
	Number of faces.
	*/
	face_num = filein->long_int_read();
	stats.bytes_num += 4;
	/*
	For each (triangular) face,
    components of normal vector,
    coordinates of three vertices,
    2 byte "attribute".
	*/
	for ( iface = 0; iface < face_num; iface++ ) {
		
		face[iface].order = 3;
//		face[iface].material = 0;
		
		for ( i = 0; i < 3; ++i ) {
			face[iface].normal[i] = filein->float_read();
			stats.bytes_num += 4;
		}
		
		for ( ivert = 0; ivert < face[iface].order; ivert++ )
        {
			
			for ( i = 0; i < 3; ++i ) {
				cvec[i] = filein->float_read();
				stats.bytes_num += 4;
			}
			
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
					vertex[vertex_num].cor3 = cvec;
				}
				vertex_num = vertex_num + 1;
			}
			else {
				stats.dup_num++;
			}
			
			face[iface].vertices[ivert] = icor3;
			
		}
		attribute = filein->short_int_read();
		if ( debug ) {
			printf ( "ATTRIBUTE = %d\n", attribute );
		}
		stats.bytes_num += 2;
	}
	
	return true;
}
/******************************************************************************/

bool IVCONV::stlb_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  STLB_WRITE writes a binary STL (stereolithography) file.

Discussion:
  The polygons in an STL file should only be triangular.  This routine 
  will try to automatically decompose higher-order polygonal faces into 
  suitable triangles, without actually modifying the internal graphics 
  data.

Author: John Burkardt
Modified: 24 May 1999
*/
{
	short int attribute = 0;
	char c;
	int i;
	int icor3;
	int iface;
	int jvert;
	int face_num2;

	// 80 byte Header.
	for ( i = 0; i < 80; ++i ) {
		c = ' ';
		stats.bytes_num += fileout->fputc(c);
	}
	/*
	Number of faces.
	*/
	face_num2 = 0;
	for ( iface = 0; iface < face_num; iface++ ) {
		face_num2 = face_num2 + face[iface].order - 2;
	}
	
	stats.bytes_num += fileout->long_int_write(face_num2);
	/*
	For each (triangular) face,
    components of normal vector,
    coordinates of three vertices,
    2 byte "attribute".
	*/
	for ( iface = 0; iface < face_num; iface++ )
    {
		for ( jvert = 2; jvert < face[iface].order; jvert++ )
        {
			for ( i = 0; i < 3; ++i )
				stats.bytes_num += fileout->float_write( face[iface].normal[i] );

			icor3 = face[iface].vertices[0];
			for ( i = 0; i < 3; ++i )
				stats.bytes_num += fileout->float_write( vertex[icor3].cor3[i] );

			icor3 = face[iface].vertices[jvert-1];
			for ( i = 0; i < 3; ++i )
				stats.bytes_num += fileout->float_write( vertex[icor3].cor3[i] );

			icor3 = face[iface].vertices[jvert];
			for ( i = 0; i < 3; ++i )
				stats.bytes_num += fileout->float_write( vertex[icor3].cor3[i] );

			stats.bytes_num += fileout->short_int_write(attribute);
		}
	}
	// Report.
	printf ( "\n" );
	printf ( "STLB_WRITE - Wrote %lu bytes.\n", stats.bytes_num );
	
	if ( face_num != face_num2 )
    {
		printf ( "  Number of faces in original data was %lu.\n", face_num );
		printf ( "  Number of triangular faces in decomposed data is %d.\n",
			face_num2 );
	}
	return true;
}
