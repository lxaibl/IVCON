/******************************************************************************/
// Author:  John Burkardt 
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core
// Change: 31 May 2008 Tomasz Lis verified and fixed

// Purpose:
//   Methods for support of the XGL format, based on the XML language
//   and OpenGl graphics

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - No read support
// Format support note - writing:
// - Full 32-bit vertices support
// - Full 32-bit faces support, unlimited face order
// - Meshes supported (untested)
// - Materials supported
// - No textures support
// - No texture UV support
// - No camera/view support
// - Environment color and lights supported
// - No shapes/lines support

/*
Example:

<WORLD>

<BACKGROUND>
<BACKCOLOR> 0.1, 0.1, 0.1 </BACKCOLOR>
</BACKGROUND>

<LIGHTING>
<AMBIENT> 0.2, 0.1, 0.1 </AMBIENT>
<DIRECTIONALLIGHT>
<DIFFUSE> 0.1, 0.2, 0.1 </DIFFUSE>
<DIRECTION> 0, 0, 100 </DIRECTION>
<SPECULAR> 0.1, 0.1, 0.2 </SPECULAR>
</DIRECTIONALLIGHT>
</LIGHTING>

<MESH ID = "0">

<P ID="0"> -0.5, -0.5, 1 </P>
<P ID="1"> 0.5, -0.5, 1 </P>
<P ID="2"> 0.5, 0.5, 1 </P>
<P ID="3"> -0.5, 0.5, 1 </P>
<P ID="4"> 0.5, -0.5, 0 </P>
<P ID="5"> -0.5, -0.5, 0 </P>
<P ID="6"> -0.5, 0.5, 0 </P>
<P ID="7"> 0.5, 0.5, 0 </P>

<N ID="0"> -0.408248, -0.408248, 0.816497 </N>
<N ID="1"> 0.666667, -0.666667, 0.333333 </N>
<N ID="2"> 0.408248, 0.408248, 0.816497 </N>
<N ID="3"> -0.666667, 0.666667, 0.333333 </N>
<N ID="4"> 0.408248, -0.408248, -0.816497 </N>
<N ID="5"> -0.666667, -0.666667, -0.333333 </N>
<N ID="6"> -0.408248, 0.408248, -0.816497 </N>
<N ID="7"> 0.666667, 0.666667, -0.333333 </N>

<MAT ID="0">
<ALPHA> 0.9 </ALPHA>
<AMB> 0.1, 0.1, 0.1 </AMB>
<DIFF> 0.2, 0.1, 0.1 </DIFF>
<EMISS> 0.1, 0.2, 0.1 </EMISS>
<SHINE> 0.8 </SHINE>
<SPEC> 0.1, 0.1, 0.2 </SPEC>
</MAT>

<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 0 </PREF><NREF> 0 </NREF></FV1>
<FV2><PREF> 1 </PREF><NREF> 1 </NREF></FV2>
<FV3><PREF> 2 </PREF><NREF> 2 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 0 </PREF><NREF> 0 </NREF></FV1>
<FV2><PREF> 2 </PREF><NREF> 2 </NREF></FV2>
<FV3><PREF> 3 </PREF><NREF> 3 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 4 </PREF><NREF> 4 </NREF></FV1>
<FV2><PREF> 5 </PREF><NREF> 5 </NREF></FV2>
<FV3><PREF> 6 </PREF><NREF> 6 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 4 </PREF><NREF> 4 </NREF></FV1>
<FV2><PREF> 6 </PREF><NREF> 6 </NREF></FV2>
<FV3><PREF> 7 </PREF><NREF> 7 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 5 </PREF><NREF> 5 </NREF></FV1>
<FV2><PREF> 0 </PREF><NREF> 0 </NREF></FV2>
<FV3><PREF> 3 </PREF><NREF> 3 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 5 </PREF><NREF> 5 </NREF></FV1>
<FV2><PREF> 3 </PREF><NREF> 3 </NREF></FV2>
<FV3><PREF> 6 </PREF><NREF> 6 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 1 </PREF><NREF> 1 </NREF></FV1>
<FV2><PREF> 4 </PREF><NREF> 4 </NREF></FV2>
<FV3><PREF> 7 </PREF><NREF> 7 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 1 </PREF><NREF> 1 </NREF></FV1>
<FV2><PREF> 7 </PREF><NREF> 7 </NREF></FV2>
<FV3><PREF> 2 </PREF><NREF> 2 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 5 </PREF><NREF> 5 </NREF></FV1>
<FV2><PREF> 4 </PREF><NREF> 4 </NREF></FV2>
<FV3><PREF> 1 </PREF><NREF> 1 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 5 </PREF><NREF> 5 </NREF></FV1>
<FV2><PREF> 1 </PREF><NREF> 1 </NREF></FV2>
<FV3><PREF> 0 </PREF><NREF> 0 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 3 </PREF><NREF> 3 </NREF></FV1>
<FV2><PREF> 2 </PREF><NREF> 2 </NREF></FV2>
<FV3><PREF> 7 </PREF><NREF> 7 </NREF></FV3>
</F>
<F>
<MATREF> 0 </MATREF>
<FV1><PREF> 3 </PREF><NREF> 3 </NREF></FV1>
<FV2><PREF> 7 </PREF><NREF> 7 </NREF></FV2>
<FV3><PREF> 6 </PREF><NREF> 6 </NREF></FV3>
</F>
</MESH>

<OBJECT>
<TRANSFORM>
<FORWARD> 0, 0, 0 </FORWARD>
<POSITION> 0, 0, 0 </POSITION>
<SCALE> 1, 1, 1 </SCALE>
<UP> 1, 1, 1 </UP>
</TRANSFORM>
<MESHREF> 0 </MESHREF>
</OBJECT>

</WORLD>
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

/******************************************************************************/

bool IVCONV::xgl_write_bkgnd ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  XGL_WRITE_BKGND writes background data to a XGL file.

Author: John Burkardt
Modified: 31 May 2008 Tomasz Lis created from XGL_WRITE
*/
{
	fileout->fprintf("  <BACKGROUND>\n" );
	fileout->fprintf("    <BACKCOLOR> %f, %f, %f </BACKCOLOR>\n", 
		env.rgb[0], env.rgb[1], env.rgb[2] );
	fileout->fprintf("  </BACKGROUND>\n" );
	fileout->fprintf("\n" );
	stats.text_num+=4;
}
/******************************************************************************/

bool IVCONV::xgl_write_lghtng ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  XGL_WRITE_LGHTNG writes lightning data to a XGL file.

Author: John Burkardt
Modified: 31 May 2008 Tomasz Lis created from XGL_WRITE
*/
{
	fileout->fprintf("  <LIGHTING>\n" );
	fileout->fprintf("    <AMBIENT> %f, %f, %f </AMBIENT>\n",
		env.ambient_light[0], env.ambient_light[1], env.ambient_light[2] );
	stats.text_num+=2;
    unsigned int light_idx;
    for (light_idx=0;light_idx<light_num;light_idx++)
    {
        fileout->fprintf("    <DIRECTIONALLIGHT>\n" );
        fileout->fprintf("      <DIFFUSE> %f, %f, %f </DIFFUSE>\n",
            light[light_idx].rgb[0], light[light_idx].rgb[1], light[light_idx].rgb[2]);
        fileout->fprintf("      <DIRECTION> %f, %f, %f </DIRECTION>\n",
            light[light_idx].target[0], light[light_idx].target[1], light[light_idx].target[2]);
        fileout->fprintf("      <SPECULAR> %f, %f, %f </SPECULAR>\n",
            light[light_idx].rgb[0], light[light_idx].rgb[1], light[light_idx].rgb[2]);
        fileout->fprintf("    </DIRECTIONALLIGHT>\n" );
        stats.text_num+=5;
    }
	fileout->fprintf("  </LIGHTING>\n" );
	stats.text_num++;
}
/******************************************************************************/

bool IVCONV::xgl_write_material ( FileIO *fileout, unsigned long mat_idx )

/******************************************************************************/
/*
Purpose:
  XGL_WRITE_MATERIAL writes material data to a XGL file.

Author: John Burkardt
Modified: 31 May 2008 Tomasz Lis created from XGL_WRITE
*/
{
    fileout->fprintf("    <MAT ID=\"%d\">\n", mat_idx );
    fileout->fprintf("      <ALPHA> %f </ALPHA>\n", material[mat_idx].alpha );
    fileout->fprintf("      <AMB> %f, %f, %f </AMB>\n",
        material[mat_idx].ambient[0], material[mat_idx].ambient[1], material[mat_idx].ambient[2] );
    fileout->fprintf("      <DIFF> %f, %f, %f </DIFF>\n",
        material[mat_idx].rgb[0], material[mat_idx].rgb[1], material[mat_idx].rgb[2] );
    fileout->fprintf("      <EMISS> %f, %f, %f </EMISS>\n",
        0.0f, 0.0f, 0.0f );
    fileout->fprintf("      <SHINE> %f </SHINE>\n", material[mat_idx].shiness );
    fileout->fprintf("      <SPEC> %f, %f, %f </SPEC>\n",
        material[mat_idx].specular[0], material[mat_idx].specular[1], material[mat_idx].specular[2] );
    fileout->fprintf("    </MAT>\n" );
    stats.text_num += 8;
}
/******************************************************************************/

bool IVCONV::xgl_write_mesh ( FileIO *fileout, unsigned long mesh_idx )

/******************************************************************************/
/*
Purpose:
  XGL_WRITE_MESH writes a mesh data to a XGL file.

Author: John Burkardt
Modified: 31 May 2008 Tomasz Lis created from XGL_WRITE
*/
{
    fileout->fprintf("  <MESH ID = \"%d\">\n", mesh_idx );
    fileout->fprintf("\n" );
    stats.text_num += 2;

    //Preparing mesh to save
    array<unsigned long> vert_idx_arr;
    unsigned long vert_count;
    array<long> nvert_idx;
    unsigned long faces_count;
    get_mesh_vertices(vert_idx_arr,vert_count,nvert_idx,faces_count,mesh_idx);

    unsigned long j;
    for ( j = 0; j < vert_count; j++ )
    {
        unsigned long vert_idx=vert_idx_arr[j];
        fileout->fprintf("    <P ID=\"%d\"> %f, %f, %f </P>\n", vert_idx,
            vertex[vert_idx].cor3[0], vertex[vert_idx].cor3[1], vertex[vert_idx].cor3[2] );
        stats.text_num++;
    }
    fileout->fprintf("\n" );
    stats.text_num++;

    for ( j = 0; j < vert_count; j++ )
    {
        unsigned long vert_idx=vert_idx_arr[j];
        fileout->fprintf("    <N ID=\"%d\"> %f, %f, %f </N>\n", vert_idx,
				vertex[vert_idx].normal[0], vertex[vert_idx].normal[1], vertex[vert_idx].normal[2] );
        stats.text_num++;
    }
    unsigned int material_idx;
    for ( material_idx = 0; material_idx < material_num; material_idx++ )
    {
        fileout->fprintf("\n" );
        stats.text_num++;
        xgl_write_material(fileout,material_idx);
    }
		
    fileout->fprintf("\n" );
    stats.text_num++;
    unsigned long iface;
    for ( iface = 0; iface < face_num; iface++ )
    {
        fileout->fprintf("    <F>\n" );
        fileout->fprintf("      <MATREF> %d </MATREF>\n", get_face_major_material(iface) );
        stats.text_num += 2;
        unsigned long ivert;
        for ( ivert = 0; ivert < face[iface].order; ivert++ )
        {
            unsigned long nv_idx=face[iface].vertices[ivert];
            long vert_idx=nvert_idx[nv_idx];
            if (vert_idx<0)
            {
              if (debug)
                printf ( "    XGL_WRITE_MESH: Warning, lost vertex from face %lu\n",iface);
              vert_idx=0;
            }
            fileout->fprintf("      <FV%lu><PREF> %ld </PREF><NREF> %ld </NREF></FV%lu>\n",
                ivert+1, vert_idx, vert_idx, ivert+1 );
            stats.text_num++;
        }
			fileout->fprintf("    </F>\n" );
			stats.text_num++;
    }
		
    fileout->fprintf("  </MESH>\n" );
    stats.text_num++;
}
/******************************************************************************/

bool IVCONV::xgl_write_object ( FileIO *fileout, unsigned long mesh_idx )

/******************************************************************************/
/*
Purpose:
  XGL_WRITE_OBJECT writes object data to a XGL file.

Author: John Burkardt
Modified: 31 May 2008 Tomasz Lis created from XGL_WRITE
*/
{
	float transform_forward[3];
	float transform_position[3];
	float transform_scale[3];
	float transform_up[3];

	transform_forward[0] = 0.0;
	transform_forward[1] = 0.0;
	transform_forward[2] = 0.0;
	
	transform_position[0] = 0.0;
	transform_position[1] = 0.0;
	transform_position[2] = 0.0;
	
	transform_scale[0] = 1.0;
	transform_scale[1] = 1.0;
	transform_scale[2] = 1.0;
	
	transform_up[0] = 1.0;
	transform_up[1] = 1.0;
	transform_up[2] = 1.0;

		fileout->fprintf("  <OBJECT>\n" );
		fileout->fprintf("    <TRANSFORM>\n" );
		fileout->fprintf("      <FORWARD> %f, %f, %f </FORWARD>\n",
			transform_forward[0], transform_forward[1], transform_forward[2] );
		fileout->fprintf("      <POSITION> %f, %f, %f </POSITION>\n",
			transform_position[0], transform_position[1], transform_position[2] );
		fileout->fprintf("'      <SCALE> %f, %f, %f </SCALE>\n",
			transform_scale[0], transform_scale[1], transform_scale[2] );
		fileout->fprintf("      <UP> %f, %f, %f </UP>\n",
			transform_up[0], transform_up[1], transform_up[2] );
		fileout->fprintf("    </TRANSFORM>\n" );
		fileout->fprintf("    <MESHREF> %d </MESHREF>\n", mesh_idx );
		fileout->fprintf("  </OBJECT>\n" );
		stats.text_num += 9;
}
/******************************************************************************/

bool IVCONV::xgl_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  XGL_WRITE writes an XGL file.

Discussion:
  Two corrections to the routine were pointed out by
  Mike Phillips, msphil@widowmaker.com, on 17 September 2001,
  and are gratefully acknowledged.

Reference:
  XGL specification at http://www.xglspec.org/

Author: John Burkardt
Modified: 17 Sep 2001
Modified: 31 May 2008 Tomasz Lis divided into smaller functions, fixed
*/
{
	int mesh_idx;
	
	stats.text_num = 0;
	
	fileout->fprintf("<WORLD>\n" );
	fileout->fprintf("\n" );
	stats.text_num += 2;
	
    xgl_write_bkgnd(fileout);
    xgl_write_lghtng(fileout);

	for ( mesh_idx = 0; mesh_idx < mesh_num; mesh_idx++ )
    {
		fileout->fprintf("\n" );
		stats.text_num++;
		xgl_write_mesh(fileout,mesh_idx);
	}
	
	for ( mesh_idx = 0; mesh_idx < mesh_num; mesh_idx++ )
    {
		fileout->fprintf("\n" );
		stats.text_num++;
        xgl_write_object(fileout,mesh_idx);
	}
	
	fileout->fprintf("\n" );
	fileout->fprintf("</WORLD>\n" );
	stats.text_num += 2;
	
	// Report.
	printf ( "\n" );
	printf ( "XGL_WRITE: Wrote %d text lines.\n", stats.text_num );
	
	return true;
}

