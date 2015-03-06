/******************************************************************************/
// Author:  John Burkardt 
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core

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

bool IVCONV::xgl_write ( FILE *fileout )

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
    Modified: 17 September 2001
*/
{
	int iface;
	int ivert;
	int j;
	float light_ambient_rgb[3];
	float light_diffuse_rgb[3];
	float light_direction[3];
	float light_specular_rgb[3];
	int material;
	float material_alpha;
	float material_amb_rgb[3];
	float material_diff_rgb[3];
	float material_emiss_rgb[3];
	float material_shine;
	float material_spec_rgb[3];
	int mesh;
	int mesh_num = 1;
	int meshidx;
	float transform_forward[3];
	float transform_position[3];
	float transform_scale[3];
	float transform_up[3];
	/*
	Make up some placeholder values for now.
	*/
	light_ambient_rgb[0] = 0.2;
	light_ambient_rgb[1] = 0.1;
	light_ambient_rgb[2] = 0.1;
	
	light_diffuse_rgb[0] = 0.1;
	light_diffuse_rgb[1] = 0.2;
	light_diffuse_rgb[2] = 0.1;
	
	light_direction[0] =   0.0;
	light_direction[1] =   0.0;
	light_direction[2] = 100.0;
	
	light_specular_rgb[0] = 0.1;
	light_specular_rgb[1] = 0.1;
	light_specular_rgb[2] = 0.2;
	
	material_alpha = 0.9;
	
	material_amb_rgb[0] = 0.1;
	material_amb_rgb[1] = 0.1;
	material_amb_rgb[2] = 0.1;
	
	material_diff_rgb[0] = 0.2;
	material_diff_rgb[1] = 0.1;
	material_diff_rgb[2] = 0.1;
	
	material_emiss_rgb[0] = 0.1;
	material_emiss_rgb[1] = 0.2;
	material_emiss_rgb[2] = 0.1;
	
	material_shine = 0.8;
	
	material_spec_rgb[0] = 0.1;
	material_spec_rgb[1] = 0.1;
	material_spec_rgb[2] = 0.2;
	
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
	
	mesh_num = 1;
	
	stats.text_num = 0;
	
	fprintf ( fileout, "<WORLD>\n" );
	fprintf ( fileout, "\n" );
	stats.text_num += 2;
	
	fprintf ( fileout, "  <BACKGROUND>\n" );
	fprintf ( fileout, "    <BACKCOLOR> %f, %f, %f </BACKCOLOR>\n", 
		env.rgb[0], env.rgb[1], env.rgb[2] );
	fprintf ( fileout, "  </BACKGROUND>\n" );
	fprintf ( fileout, "\n" );
	stats.text_num+=4;

	fprintf ( fileout, "  <LIGHTING>\n" );
	fprintf ( fileout, "    <AMBIENT> %f, %f, %f </AMBIENT>\n",
		light_ambient_rgb[0], light_ambient_rgb[1], light_ambient_rgb[2] );
	fprintf ( fileout, "    <DIRECTIONALLIGHT>\n" );
	fprintf ( fileout, "      <DIFFUSE> %f, %f, %f </DIFFUSE>\n",
		env.ambient_light[0], env.ambient_light[1], env.ambient_light[2] );
	fprintf ( fileout, "      <DIRECTION> %f, %f, %f </DIRECTION>\n",
		light_direction[0], light_direction[1], light_direction[2] );
	fprintf ( fileout, "      <SPECULAR> %f, %f, %f </SPECULAR>\n",
		light_specular_rgb[0], light_specular_rgb[1], light_specular_rgb[2] );
	fprintf ( fileout, "    </DIRECTIONALLIGHT>\n" );
	fprintf ( fileout, "  </LIGHTING>\n" );
	stats.text_num+=8;
	
	for ( mesh = 0; mesh < mesh_num; mesh++ )
    {
		
		fprintf ( fileout, "\n" );
		fprintf ( fileout, "  <MESH ID = \"%d\">\n", mesh );
		fprintf ( fileout, "\n" );
		stats.text_num += 3;
		
		for ( j = 0; j < vertex_num; ++j )
        {
			fprintf ( fileout, "    <P ID=\"%d\"> %f, %f, %f </P>\n", j,
				vertex[j].cor3[0], vertex[j].cor3[1], vertex[j].cor3[2] );
			stats.text_num++;
		}
		fprintf ( fileout, "\n" );
		stats.text_num++;

		for ( j = 0; j < vertex_num; ++j )
        {
			fprintf ( fileout, "    <N ID=\"%d\"> %f, %f, %f </N>\n", j,
				vertex[j].normal[0], vertex[j].normal[1], vertex[j].normal[2] );
			stats.text_num++;
		}
		
		for ( material = 0; material < material_num; material++ )
        {
			fprintf ( fileout, "\n" );
			fprintf ( fileout, "    <MAT ID=\"%d\">\n", material );
			fprintf ( fileout, "      <ALPHA> %f </ALPHA>\n", material_alpha );
			fprintf ( fileout, "      <AMB> %f, %f, %f </AMB>\n",
				material_amb_rgb[0], material_amb_rgb[1], material_amb_rgb[2] );
			fprintf ( fileout, "      <DIFF> %f, %f, %f </DIFF>\n",
				material_diff_rgb[0], material_diff_rgb[1], material_diff_rgb[2] );
			fprintf ( fileout, "      <EMISS> %f, %f, %f </EMISS>\n",
				material_emiss_rgb[0], material_emiss_rgb[1], material_emiss_rgb[2] );
			fprintf ( fileout, "      <SHINE> %f </SHINE>\n", material_shine );
			fprintf ( fileout, "      <SPEC> %f, %f, %f </SPEC>\n",
				material_spec_rgb[0], material_spec_rgb[1], material_spec_rgb[2] );
			fprintf ( fileout, "    </MAT>\n" );
			stats.text_num += 9;
		}
		
		fprintf ( fileout, "\n" );
		stats.text_num++;
		
		for ( iface = 0; iface < face_num; iface++ )
        {
			fprintf ( fileout, "    <F>\n" );
			fprintf ( fileout, "      <MATREF> %d </MATREF>\n", get_face_major_material(iface) );
			stats.text_num += 2;
			for ( ivert = 0; ivert < face[iface].order; ivert++ ) {
				fprintf ( fileout, 
					"      <FV%d><PREF> %d </PREF><NREF> %d </NREF></FV%d>\n",
					ivert+1, face[iface].vertices[ivert], face[iface].vertices[ivert], ivert+1 );
				stats.text_num++;
			}
			fprintf ( fileout, "    </F>\n" );
			stats.text_num++;
		}
		
		fprintf ( fileout, "  </MESH>\n" );
		stats.text_num++;
		
	}
	
	fprintf ( fileout, "\n" );
	stats.text_num++;
	
	for ( meshidx = 0; meshidx < mesh_num; meshidx++ )
    {
		fprintf ( fileout, "  <OBJECT>\n" );
		fprintf ( fileout, "    <TRANSFORM>\n" );
		fprintf ( fileout, "      <FORWARD> %f, %f, %f </FORWARD>\n",
			transform_forward[0], transform_forward[1], transform_forward[2] );
		fprintf ( fileout, "      <POSITION> %f, %f, %f </POSITION>\n",
			transform_position[0], transform_position[1], transform_position[2] );
		fprintf ( fileout, "'      <SCALE> %f, %f, %f </SCALE>\n",
			transform_scale[0], transform_scale[1], transform_scale[2] );
		fprintf ( fileout, "      <UP> %f, %f, %f </UP>\n",
			transform_up[0], transform_up[1], transform_up[2] );
		fprintf ( fileout, "    </TRANSFORM>\n" );
		mesh = 0;
		fprintf ( fileout, "    <MESHREF> %d </MESHREF>\n", mesh );
		fprintf ( fileout, "  </OBJECT>\n" );
		stats.text_num += 9;
	}
	
	fprintf ( fileout, "\n" );
	fprintf ( fileout, "</WORLD>\n" );
	stats.text_num += 2;
	
	/*
	Report.
	*/
	printf ( "\n" );
	printf ( "XGL_WRITE - Wrote %d text lines.\n", stats.text_num );
	
	return true;
}

