/******************************************************************************/
// Author:  John Burkardt 
// Change: 02 Mar 2007 Tomasz Lis some text displayed

// Purpose:
//   Metthods for support of simple ASCII files

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - Full 32-bit vertices support
// - No faces support
// - No meshes support
// - No materials
// - No textures
// - No texture UV
// - No camera/view support
// - No Environment or lights support
// - No shapes/lines support
// Format support note - writing:
// - Full 32-bit vertices support
// - No faces support
// - No meshes support
// - No materials
// - No textures
// - No texture UV
// - No camera/view support
// - No Environment or lights support
// - No shapes/lines support

//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

/*****************************************************************************/

bool IVCONV::asc_read ( FileIO *filein )

/*****************************************************************************/
/*
Purpose:

ASC_READ reads points stored in ASCII

8.59816       5.55317      -3.05561,
8.59816       2.49756      0.000000E+00,
...etc...
2.48695       2.49756      -3.05561,
*/
{
  if ( debug )
      printf ( "ASC_READ: Reading point coordinates\n" );
	for ( ;; )
    {
		char input[LINE_MAX_LEN];
		if ( filein->fgets(input,LINE_MAX_LEN) == NULL ) break;
		stats.text_num++;
		float x=0,y=0,z=0;
		sscanf(input, "%e %e %e", &x,&y,&z );
		if (isnan(x)||isnan(y)||isnan(z))
		{
          if ( debug )
            printf ( "ASC_READ: One of vertices skipped because of NaN coordinate.\n" );
          continue;
        }
		vertex[vertex_num]=Vertex();
		vertex[vertex_num].cor3=vec3(x,y,z);
		vertex_num++;
	}
  if ( debug )
      printf ( "ASC_READ - found %lu vertices.\n",vertex_num );
  return true;
}
/*****************************************************************************/

bool IVCONV::asc_write ( FileIO *fileout )

/*****************************************************************************/
/*
Purpose:

ASC_WRITE writes points (vertices coordinates) to ASCII

*/
{
    if ( debug )
      printf ( "ASC_WRITE: writing vertex coordinates\n");
    unsigned long cor3_count=0;
	for (unsigned long vertidx = 0; vertidx < vertex_num; vertidx++ )
    {
        vec3 coords=vec3( vertex[vertidx].cor3);
		fileout->fprintf("%f %f %f\n", coords[0],coords[1],coords[2]);
        cor3_count++;
		stats.text_num++;
	}
    if ( debug )
      printf ( "ASC_WRITE: writing line points coordinates\n");
	for (unsigned long shapeidx = 0; shapeidx < shape_num; shapeidx++ )
      for (unsigned long pointidx = 0; pointidx < shape[shapeidx].order; pointidx++ )
      {
        vec3 coords=vec3(shape[shapeidx].cor3[pointidx]);
		fileout->fprintf("%f %f %f\n", coords[0],coords[1],coords[2]);
        cor3_count++;
		stats.text_num++;
      }
    printf ( "ASC_WRITE - wrote %lu point coordinates.\n",cor3_count );
	return true;
}
/*****************************************************************************/
