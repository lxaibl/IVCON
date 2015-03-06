/******************************************************************************/
// Author:  John Burkardt
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core
// Change: 31 Mar 2008 Tomasz Lis fixed file reading routine

// Purpose:
//   Methods for support of Evans and Sutherland Digistar II VLA files for planetariums

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - No vertices support (but line points supported)
// - No faces support
// - No meshes support
// - No materials support
// - No textures support
// - No texture UV support
// - No cameras or default view support
// - No environment or lights supported
// - 31-bit shapes/lines support (restricted by SHAPES_MAX and SHAPE_ORDER_MAX)
// Format support note - writing:
// - No vertices support (but line points supported)
// - No faces support
// - No meshes support
// - No materials support
// - No textures support
// - No texture UV support
// - No cameras or default view support
// - No environment or lights supported
// - Full 32-bit shapes/lines support
/*
	Examples:

	  set comment cube.vla created by IVREAD
	  set comment Original data in cube.iv.
	  set comment
	  set intensity EXPLICIT
	  set parametric NON_PARAMETRIC
	  set filecontent LINES
	  set filetype NEW
	  set depthcue 0
	  set defaultdraw stellar
	  set coordsys RIGHT
	  set author IVREAD
	  set site Buhl Planetarium
	  set library_id UNKNOWN
	  P   8.59816       5.55317      -3.05561       1.00000
	  L   8.59816       2.49756      0.000000E+00   1.00000
	  L   8.59816       2.49756      -3.05561       1.00000
	  L   8.59816       5.55317      -3.05561       1.00000
	  P   8.59816       5.55317      0.000000E+00   1.00000
	  ...etc...
	  L   2.48695       2.49756      -3.05561       1.00000
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

/******************************************************************************/

bool IVCONV::vla_read ( FileIO *filein )

/******************************************************************************/
/*
Purpose:
  VLA_READ reads a VLA file. The VLA format is used by
   the Evans and Sutherland  Digistar II Planetarium projection system.
  
  Author: John Burkardt
  Modified: 31 Mar 2007
*/
{
	int   icor3;
	char *next;
	char  token[LINE_MAX_LEN];
	int   width;
	long shape_idx=-1; //line index

	// Initialize. 
	stats.text_num = 0;

	// Read the next line of the file into INPUT. 
	char input[LINE_MAX_LEN];
	while ( filein->fgets(input,LINE_MAX_LEN) != NULL )
    {
		stats.text_num++;

		// Advance to the first nonspace character in INPUT.
		for ( next = input; *next != '\0' && isspace(*next); next++ )
        { }

		// Skip blank lines and comments. 
		if ( *next == '\0' || *next == ';' )
			continue;

		// Extract the first word in this line. 
		sscanf ( next, "%s%n", token, &width );

		// Set NEXT to point to just after this token. 
		next = next + width;

		// SET (ignore) 
		if ( leqi ( token, "set" )  )
        {
		} else

		// P (begin a line)
		// L (continue a line) 
        if ( leqi ( token, "P" )  || leqi ( token, "L")  )
        {
			if ( leqi ( token, "P" ) )
            {
				if (shape_num<SHAPES_MAX)
                {
                    shape[shape_num]=Shape();
                    shape_idx=shape_num;
                    shape_num++;
				} else
				{
                    printf ( "VLA_READ: Reached limit of shapes. Breaking read.\n");
                    printf ( "  Some shapes have been skipped!\n");
                    break;
                }
			}
			
            float r1;
            float r2;
            float r3;
			sscanf ( next, "%e %e %e", &r1, &r2, &r3 );
			
			if (shape_idx<0)
            {
                printf ( "VLA_READ: Warning - wrong start of first line; fixed.\n");
                shape[shape_num]=Shape();
                shape_idx=shape_num;
                shape_num++;
            }
            int point_idx=shape[shape_idx].order;
            if (point_idx<SHAPE_ORDER_MAX)
            {
                shape[shape_idx].order++;
                shape[shape_idx].cor3[point_idx][0]=r1;
                shape[shape_idx].cor3[point_idx][1]=r2;
                shape[shape_idx].cor3[point_idx][2]=r3;
            }
		} else
		// Unexpected or unrecognized. 
		{
			printf ( "\n" );
			printf ( "VLA_READ - Fatal error!\n" );
			printf ( "  Unrecognized first word on line %d.\n",stats.text_num );
			return false;
		}
		
	}
	return true;
}
/******************************************************************************/

bool IVCONV::vla_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  VLA_WRITE writes internal graphics information to a VLA file.

Discussion:
  Comments begin with a semicolon in column 1.
  The X, Y, Z coordinates of points begin with a "P" to
  denote the beginning of a line, and "L" to denote the
  continuation of a line.  The fourth entry is intensity, which 
  should be between 0.0 and 1.0.

  Author: John Burkardt
  Modified: 22 May 1999
*/
{
	//Initialize. 
	stats.text_num = 0;
	
	fileout->fprintf( "set comment %s created by IVCON.\n", fileout_name );
	fileout->fprintf( "set comment Original data in %s.\n", filein_name );
	fileout->fprintf( "set comment\n" );
	stats.text_num+=3;
	fileout->fprintf( "set intensity %s\n","EXPLICIT" );
	fileout->fprintf( "set parametric %s\n","NON_PARAMETRIC" );
	fileout->fprintf( "set filecontent %s\n","LINES" );
	fileout->fprintf( "set filetype %s\n","NEW" );
	stats.text_num+=4;
	fileout->fprintf( "set depthcue %u\n",0 );
	fileout->fprintf( "set defaultdraw %s\n","stellar" );
	fileout->fprintf( "set coordsys %s\n","RIGHT" );
	fileout->fprintf( "set author %s\n","IVCON" );
	fileout->fprintf( "set site %s\n","Buhl Planetarium" );
	fileout->fprintf( "set library_id %s\n","UNKNOWN" );
	stats.text_num+=6;
    if ( debug )
      printf ( "  VLA_WRITE: File header ends at line %lu\n",stats.text_num);
	
	float intense = 1.0;
	unsigned long shape_idx;
	for ( shape_idx = 0; shape_idx < shape_num; shape_idx++ )
    {
        unsigned long cor3_idx;
        unsigned long line_order=shape[shape_idx].order;
        char c = 'P';
        for ( cor3_idx = 0; cor3_idx < line_order; cor3_idx++ )
        {
          vec3 ln_cor3=vec3(shape[shape_idx].cor3[cor3_idx]);
          fileout->fprintf( "%c % 11f % 11f % 11f % 11f\n", 
              c, ln_cor3[0], ln_cor3[1], ln_cor3[2], intense );
          stats.text_num++;
          c = 'L';
        }
        if ( debug )
          printf ( "  VLA_WRITE: Shape %lu ends at line %lu\n",shape_idx,stats.text_num);
	}

	// Report.
	printf ( "\n" );
	printf ( "VLA_WRITE - Wrote %d text lines.\n", stats.text_num );
	return true;
}

/**********************************************************************/

