/******************************************************************************/
// Author:  John Burkardt
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core
// Change: 31 Mar 2008 Tomasz Lis fixed and described

// Purpose:
//   Methods for support of  Persistence of Vision files

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - No read support
// Format support note - writing:
// - Full vertices support
// - Full faces support, max face order 3 (auto reduced on save)
// - No meshes support (may be possible to add - not sure)
// - No materials support (may be possible to add)
// - No textures supported
// - No texture UV supported
// - No camera/view support
// - No Environment or lights support
// - No shapes support
/*
Example:

// cone.pov created by IVCON.
// Original data in cone.iv

#version 3.0
#include "colors.inc"
#include "shapes.inc"
global_settings { assumed_gamma 2.2 }

camera {
right < 4/3, 0, 0>
up < 0, 1, 0 >
sky < 0, 1, 0 >
angle 20
location < 0, 0, -300 >
look_at < 0, 0, 0>
}

light_source { < 20, 50, -100 > color White }

background { color SkyBlue }

#declare RedText = texture {
pigment { color rgb < 0.8, 0.2, 0.2> }
finish { ambient 0.2 diffuse 0.5 }
}

#declare BlueText = texture {
pigment { color rgb < 0.2, 0.2, 0.8> }
finish { ambient 0.2 diffuse 0.5 }
}
mesh {
smooth_triangle {
< 0.29, -0.29, 0.0>, < 0.0, 0.0, -1.0 >,
< 38.85, 10.03, 0.0>, < 0.0, 0.0, -1.0 >,
< 40.21, -0.29, 0.0>, <  0.0, 0.0, -1.0 >
texture { RedText } }
...
smooth_triangle {
<  0.29, -0.29, 70.4142 >, < 0.0,  0.0, 1.0 >,
<  8.56,  -2.51, 70.4142 >, < 0.0,  0.0, 1.0 >,
<  8.85, -0.29, 70.4142 >, < 0.0,  0.0, 1.0 >
texture { BlueText } }
}
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

/******************************************************************************/

bool IVCONV::pov_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  POV_WRITE writes graphics information to a POV file.

Author: John Burkardt
Modified: 08 October 1998
*/
{
	int i;
	int j;
	int jj;
	int jlo;
	int k;
	
	stats.text_num = 0;
	fileout->fprintf(  "// %s created by IVCON.\n", fileout_name );
	fileout->fprintf(  "// Original data in %s.\n", filein_name );
	fileout->fprintf( "\n" );
	stats.text_num += 3;

	// Initial declarations.
	fileout->fprintf( "#version 3.0\n" );
	fileout->fprintf( "#include \"colors.inc\"\n" );
	fileout->fprintf( "#include \"shapes.inc\"\n" );
	fileout->fprintf( "global_settings { assumed_gamma 2.2 }\n" );
	fileout->fprintf( "\n" );
	stats.text_num+=4;

	fileout->fprintf( "camera {\n" );
	fileout->fprintf( " right < 4/3, 0, 0>\n" );
	fileout->fprintf( " up < 0, 1, 0 >\n" );
	fileout->fprintf( " sky < 0, 1, 0 >\n" );
	fileout->fprintf( " angle 20\n" );
	fileout->fprintf( " location < 0, 0, -300 >\n" );
	fileout->fprintf( " look_at < 0, 0, 0>\n" );
	fileout->fprintf( "}\n" );
	fileout->fprintf( "\n" );
	stats.text_num+=9;

	fileout->fprintf( "light_source { < 20, 50, -100 > color White }\n" );
	fileout->fprintf( "\n" );
	stats.text_num+=2;
	fileout->fprintf( "background { color SkyBlue }\n" );
	fileout->fprintf( "\n" );
	stats.text_num+=2;

	// Declare RGB textures.
	fileout->fprintf( "#declare RedText = texture {\n" );
	fileout->fprintf( "  pigment { color rgb < 0.8, 0.2, 0.2> }\n" );
	fileout->fprintf( "  finish { ambient 0.2 diffuse 0.5 }\n" );
	fileout->fprintf( "}\n" );
	fileout->fprintf( "\n" );
	stats.text_num+=5;

	fileout->fprintf( "#declare GreenText = texture {\n" );
	fileout->fprintf( "  pigment { color rgb < 0.2, 0.8, 0.2> }\n" );
	fileout->fprintf( "  finish { ambient 0.2 diffuse 0.5 }\n" );
	fileout->fprintf( "}\n" );
	fileout->fprintf( "\n" );
	stats.text_num+=5;

	fileout->fprintf( "#declare BlueText = texture {\n" );
	fileout->fprintf( "  pigment { color rgb < 0.2, 0.2, 0.8> }\n" );
	fileout->fprintf( "  finish { ambient 0.2 diffuse 0.5 }\n" );
	fileout->fprintf( "}\n" );
	fileout->fprintf( "\n" );
	stats.text_num+=5;

	// Write one big object.
	fileout->fprintf(  "mesh {\n" );
	stats.text_num++;

	// Do the next face.
	for ( i = 0; i < face_num; ++i )
    {
	// Break the face up into triangles, anchored at node 1.
		for ( jlo = 0; jlo < face[i].order - 2; jlo++ )
        {
			fileout->fprintf( "  smooth_triangle {\n" );
			stats.text_num++;
			
			for ( j = jlo; j < jlo + 3; ++j )
            {
				if ( j == jlo )
					jj = 0;
				else
					jj = j;
				
				k = face[i].vertices[jj];
				
				fileout->fprintf( "<%f, %f, %f>, <%f, %f, %f>",
					vertex[k].cor3[0], vertex[k].cor3[1], vertex[k].cor3[2],
					vertex[k].normal[0],
					vertex[k].normal[1],
					vertex[k].normal[2] );
				
				if ( j < jlo + 2 )
					fileout->fprintf( ",\n" );
				else
					fileout->fprintf( "\n" );
				stats.text_num++;
				
			}
			
			if (i%6 == 1 )
            {
				fileout->fprintf( "texture { RedText } }\n" );
			} else
            if ( i%2 == 0 )
            {
				fileout->fprintf( "texture { BlueText } }\n" );
			} else
            {
				fileout->fprintf( "texture { GreenText } }\n" );
			}
			stats.text_num++;
			
		}
		
	}
	
	fileout->fprintf( "}\n" );
	stats.text_num++;

	// Report.
	printf ( "\n" );
	printf ( "POV_WRITE - Wrote %lu text lines.\n", stats.text_num );
	
	return true;
}
/******************************************************************************/

