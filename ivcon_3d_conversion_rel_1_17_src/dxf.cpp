/******************************************************************************/
// Author:  John Burkardt
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core

// Purpose:
//   Metthods for support of AutoCAD DXF files

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - Full 32-bit vertices support
// - Full 32-bit faces support, max face order 10
// - No meshes support
// - No materials support
// - No textures support
// - No texture UV supported
// - No default view or cameras supported
// - No environment or lights support
// - Shapes supported
// Format support note - writing:
// - Full 32-bit vertices support
// - Full 32-bit faces support, max face order 10
// - No meshes support
// - No materials support
// - No textures support
// - No texture UV supported
// - No default view or cameras supported
// - No environment or lights support
// - Shapes supported

/*
	Examples:

      0
	  SECTION
      2
	  HEADER
	  999
	  diamond.dxf created by IVREAD.
	  999
	  Original data in diamond.obj.
      0
	  ENDSEC
      0
	  SECTION
      2
	  TABLES
      0
	  ENDSEC
      0
	  SECTION
      2
	  BLOCKS
      0
	  ENDSEC
      0
	  SECTION
      2
	  ENTITIES
      0
	  LINE
      8
	  0
	  10
      0.00  (X coordinate of beginning of line.)
	  20
      0.00  (Y coordinate of beginning of line.)
	  30
      0.00  (Z coordinate of beginning of line.)
	  11
      1.32  (X coordinate of end of line.)
	  21
      1.73  (Y coordinate of end of line.)
	  31
      2.25  (Z coordinate of end of line.)
      0
	  3DFACE
      8
	  Cube
	  10
	  -0.50  (X coordinate of vertex 1)
	  20
	  0.50  (Y coordinate of vertex 1)
	  30
      1.0  (Z coordinate of vertex 1)
	  11
	  0.50  (X coordinate of vertex 2)
	  21
	  0.50  (Y coordinate of vertex 2)
	  31
      1.0  (Z coordinate of vertex 2)
	  12
	  0.50  (X coordinate of vertex 3)
	  22
	  0.50  (Y coordinate of vertex 3)
	  32
	  0.00  (Z coordinate of vertex 3)
      0
	  ENDSEC
      0
	  EOF
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

typedef enum DXF_READTYPE
{ rt_none,rt_line,rt_face, };

/******************************************************************************/

bool IVCONV::dxf_read ( FileIO *filein )

/******************************************************************************/
/*
Purpose:
  DXF_READ reads an AutoCAD DXF file.
  
  Modified: 23 May 1999
  Author: John Burkardt
*/
{
	DXF_READTYPE lmode=rt_none;
	unsigned long ivert=0;   //vertex index inside face or line
	unsigned long itm_idx=0; //face or line index

    vec3 cvec=vec3();
    unsigned int cvec_set=0;
	// Read the next two lines of the file into INPUT1 and INPUT2. 
	for ( ;; )
    {
      int   code;
      char  input1[LINE_MAX_LEN];
      {
          // INPUT1 should contain a single integer, which tells what INPUT2
          // will contain.
          if ( filein->fgets(input1,LINE_MAX_LEN) == NULL )
            break;
          stats.text_num++;
          int   width;
          long count;
          count = sscanf ( input1, "%d%n", &code, &width );
          if ( count < 1 )
          {
              break;
          }
      }
      char  input2[LINE_MAX_LEN];
      {
		// Read the second line, and interpret it according to the code.
		if ( filein->fgets(input2,LINE_MAX_LEN) == NULL )
        {
			break;
		}
		stats.text_num++;
      }
		
      if ( code == 0 )
      {
			if ( strncmp( input2, "LINE", 4 ) == 0 )
            {
				lmode = rt_line;
				ivert = 0;
				cvec_set=0;
                shape[shape_num]=Shape();
                itm_idx=shape_num;
                shape_num++;
			} else
            if ( strncmp( input2, "3DFACE", 6 ) == 0 )
            {
				lmode = rt_face;
				ivert = 0;
				cvec_set=0;
                face[face_num]=Face();
                itm_idx=face_num;
                face_num++;
			}
      } else
      {
            unsigned int cpos1;
            for (cpos1=0; (input1[cpos1]==' ')||(input1[cpos1]=='\t') ; cpos1++) 
			{};
			
			if ( (input1[cpos1]>='1') && (input1[cpos1]<='3') )
            {
                float rval=0.0;
                long count;
                int width;
				count = sscanf ( input2, "%e%n", &rval, &width );
                cvec[input1[cpos1]-'1'] = rval;
				cvec_set++;
                if (cvec_set>=3)
                {
                    switch (lmode)
                    {
                    case rt_face: // add face node
                        vertex[vertex_num]=Vertex();
                        vertex[vertex_num].cor3 = vec3(cvec);
                        face[itm_idx].vertices[ivert]=vertex_num;
                        ivert++;
                        face[itm_idx].order=ivert;
                        vertex_num++;
                        break;
                    case rt_line: // add line node
                        shape[itm_idx].cor3[ivert] = vec3(cvec);
                        ivert++;
                        shape[itm_idx].order=ivert;
                        break;
                    default: // add free vertex
                        vertex[vertex_num]=Vertex();
                        vertex[vertex_num].cor3 = vec3(cvec);
                        ivert++;
                        vertex_num++;
                        break;
                    }
                    cvec_set=0;
                }
			}
      } // end if (code==0) else
    }//end for (...)
  
  return true;
}
/******************************************************************************/

bool IVCONV::dxf_write_shape(FileIO *fileout,unsigned long shape_idx)

/******************************************************************************/
/*
Purpose:
  dxf_write_shape writes one shape into DXF file.
  
  Author: Tomasz Lis
  Modified: 18 Mar 2007
*/
{
    long order=shape[shape_idx].order;
    if (order<=1) return false;
    order--;
    unsigned long icor3=0;
    while (icor3<order)
    {
        fileout->fprintf("  %d\n",0 );
        fileout->fprintf("LINE\n" );
        fileout->fprintf("  %d\n",8 );
        fileout->fprintf("  %d\n",0 );
        stats.text_num+=4;

        unsigned int coord_count;
        for (coord_count=0;coord_count<2;coord_count++)
        {
          vec3 coord=vec3(shape[shape_idx].cor3[icor3]);
          for (unsigned int i=0;i<3;i++)
          {
            fileout->fprintf(" %u%u\n",i+1,coord_count );
            fileout->fprintf("%f\n", coord[i] );
            stats.text_num+=2;
          }
          icor3++;
        }
    }
    return true;
}
/******************************************************************************/

bool IVCONV::dxf_write_face(FileIO *fileout,unsigned long face_idx)

/******************************************************************************/
/*
Purpose:
  dxf_write_face writes one face into DXF file.
  
  Author: Tomasz Lis
  Modified: 18 Mar 2007
*/
{
    long order=face[face_idx].order;
    if (order>10) order=10;
    fileout->fprintf("  %d\n",0 );
    fileout->fprintf("3DFACE\n" );
    fileout->fprintf("  %d\n",8 );
    fileout->fprintf("  Cube\n" );
    stats.text_num += 4;

    unsigned long ivert;
    for ( ivert = 0; ivert < order; ivert++ )
    {
        unsigned long vert_idx=face[face_idx].vertices[ivert];
        vec3 coord=vec3(vertex[vert_idx].cor3);
        for (unsigned int i=0;i<3;i++)
        {
          fileout->fprintf("%u%lu\n",i+1,ivert );
          fileout->fprintf("%f\n", coord[i] );
          stats.text_num += 2;
        }
    }
  return true;
}
/******************************************************************************/

bool IVCONV::dxf_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  DXF_WRITE writes graphics information to an AutoCAD DXF file.

  Modified: 16 May 1999
  Author: John Burkardt
*/
{
	// Initialize. 
	stats.text_num = 0;
	
    if (debug)
        printf ( "DXF_WRITE: writing HEADER section\n" );
	fileout->fprintf("  %d\n",0 );
	fileout->fprintf("SECTION\n" );
	fileout->fprintf("  %d\n",2 );
	fileout->fprintf("HEADER\n" );
	fileout->fprintf("%d\n",999 );
	fileout->fprintf("%s created by IVCON.\n", fileout_name );
	fileout->fprintf("%d\n",999 );
	fileout->fprintf("Original data in %s.\n", filein_name );
	fileout->fprintf("  %d\n",0 );
	fileout->fprintf("ENDSEC\n" );
	stats.text_num+=10;
	
    if (debug)
        printf ( "DXF_WRITE: writing TABLES section\n" );
	fileout->fprintf("  %d\n",0 );
	fileout->fprintf("SECTION\n" );
	fileout->fprintf("  %d\n",2 );
	fileout->fprintf("TABLES\n" );
	fileout->fprintf("  %d\n",0 );
	fileout->fprintf("ENDSEC\n" );
	stats.text_num += 6;
	
    if (debug)
        printf ( "DXF_WRITE: writing BLOCKS section\n" );
	fileout->fprintf("  %d\n",0 );
	fileout->fprintf("SECTION\n" );
	fileout->fprintf("  %d\n",2 );
	fileout->fprintf("BLOCKS\n" );
	fileout->fprintf("  %d\n",0 );
	fileout->fprintf("ENDSEC\n" );
	stats.text_num += 6;
	
    if (debug)
        printf ( "DXF_WRITE: writing ENTITIES section\n" );
	fileout->fprintf("  %d\n",0 );
	fileout->fprintf("SECTION\n" );
	fileout->fprintf("  %d\n",2 );
	fileout->fprintf("ENTITIES\n" );
	stats.text_num += 4;

	// Handle lines.
	unsigned long shape_idx;
	for ( shape_idx = 0; shape_idx < shape_num; shape_idx++ )
	  dxf_write_shape(fileout,shape_idx);

	// Handle faces.
	// (If FACE_ORDER is greater than 10, you're sure to have problems here)
	unsigned long face_idx;
	for ( face_idx = 0; face_idx < face_num; face_idx++ )
	  dxf_write_face(fileout,face_idx);

	fileout->fprintf("  %d\n",0 );
	fileout->fprintf("ENDSEC\n" );
	fileout->fprintf("  %d\n",0 );
	fileout->fprintf("EOF\n" );
	stats.text_num += 4;

	// Report.
	printf ( "\n" );
	printf ( "DXF_WRITE - Wrote %d text lines.\n", stats.text_num );
	
	return true;
}
