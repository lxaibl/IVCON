/******************************************************************************/
// Author:  John Burkardt 
// Created: 23 Sep 1998
// Change:  15 Mar 2003 Philippe Guglielmetti correct material handling
//		  according to http://www.the-labs.com/Blender/3DS-details.html
// Change: 01 Mar 2007 Tomasz Lis material saving, fixed texture reading

//	Discussion:
//	  The style of functions in this file may seem obsolete or unclear for some
//    people. There is a lot better 3DS library in c++ - lib3ds. You may ask:
//    why won't you use this library? And the answer is simple: to be different.
//    This code is based on completely different conception. Also, it will be
//    better if there will be more than one 3DS support function for open-source
//    applications.

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - Max vertices 65535
// - Max faces 65535, max face order 3
// - Full meshes support
// - Max materials/textures 65535; texture name must be in 8.3 format
// - Full material support - colors, alpha, one texture per material
// - Texture UV supported
// - Default view point supported, possible to add cameras support
// - No Environment or lights support (possible to add)
// - No shapes/lines support (but possible to add)
// Format support note - writing:
// - Max vertices 65535
// - Max faces 65535, max face order 3
// - Full meshes support; free vertices are exported too
// - Max materials/textures 65535; texture name must be in 8.3 format
// - Full material support - colors, alpha, two textures per material
// - Texture UV supported
// - Default view point supported, possible to add cameras support
// - No Environment or lights support (possible to add)
// - No shapes/lines support (but possible to add)

//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.

/******************************************************************************/

#include "ivconv.h"
#include "ofile.h"	// file from 3D Studio Max API

typedef struct {
    unsigned short maptype;
    float pos[3];
    float matrix[4][4];
    float scale;
    float tile[2];
    float planar_size[2];
    float cylinder_height;
} TDS_TEXTURE_MAP_DATA;

typedef struct {
    unsigned short chunk;
    unsigned long size;
    unsigned long cur;
} TDS_CHUNK;

/******************************************************************************/

int tds_write_limited_string ( FileIO *fileout, char *string, int maxlen )

/******************************************************************************/
{
	if (string==NULL)
	{
      fileout->fputc('\0');
      return 1;
    }
	int cpos;
	for (cpos=0; cpos<maxlen; cpos++)
    {
		fileout->fputc(string[cpos]);
		if (string[cpos]==0) return (cpos+1);
	}
    fileout->fputc('\0');
	return (cpos+1);
}
/******************************************************************************/

int tds_write_string ( FileIO *fileout, char *string )

/******************************************************************************/
{
	return tds_write_limited_string(fileout,string,128);
}
/******************************************************************************/

int tds_write_u_short_int ( FileIO *fileout, unsigned short int short_int_val )

/******************************************************************************/
/*
    Author: John Burkardt
    Modified: 14 Oct 1998
*/
{
	union {
		unsigned short int yint;
		char ychar[2];
	} y;
	
	y.yint = short_int_val;
	
	if ( byte_swap ) {
		fileout->fputc( y.ychar[1] );
		fileout->fputc( y.ychar[0] );
	}
	else {
		fileout->fputc ( y.ychar[0] );
		fileout->fputc ( y.ychar[1] );
	}
	
	return 2;
}
/******************************************************************************/

int tds_color_write(FileIO *fileout, vec3 color)

/******************************************************************************/
{
    unsigned short short_int_val;
    long lcolor24 = 6 + 3;
    long llincol24 = 6 + 3;
    int bytes_num=0;

    short_int_val = (unsigned short)COLOR_24;
	bytes_num += fileout->short_int_write ( short_int_val );
	bytes_num += fileout->long_int_write ( lcolor24 );
	fileout->fputc ( (unsigned char)floor(255.0*color[0]+0.5) );
	fileout->fputc ( (unsigned char)floor(255.0*color[1]+0.5) );
	fileout->fputc ( (unsigned char)floor(255.0*color[2]+0.5) );
	bytes_num+=3;

    short_int_val = (unsigned short)LIN_COLOR_24;
	bytes_num += fileout->short_int_write ( short_int_val );
	bytes_num += fileout->long_int_write ( llincol24 );
	fileout->fputc ( (unsigned char)floor(255.0*color[0]+0.5) );
	fileout->fputc ( (unsigned char)floor(255.0*color[1]+0.5) );
	fileout->fputc ( (unsigned char)floor(255.0*color[2]+0.5) );
	bytes_num+=3;

    return bytes_num;
}
/******************************************************************************/

unsigned long tds_read_unknown_section ( FileIO *filein )

/******************************************************************************/
{	
	unsigned long current_pointer = filein->ftell() - 2;
	unsigned long temp_pointer = filein->long_uint_read();
    if ( debug )
        printf ( "            Skipped section of %lu bytes\n",temp_pointer );
	long pointer = current_pointer + temp_pointer;
	filein->fseek(pointer, SEEK_SET);
	return ( temp_pointer );
}
/******************************************************************************/

unsigned long tds_read_color (FileIO *filein, unsigned short selector, vec3& rgb_val)

/******************************************************************************/
//! @return number of bytes read from file
{
	long color_chsize = filein->long_uint_read();
	if (color_chsize<6) color_chsize=6;
	switch ( selector ) {
	case COLOR_F:
		{
			if ( debug ) printf ( "        COLOR_F color definition section tag of %04X\n", selector );
			for (int i = 0; i < 3; ++i ) rgb_val[i] = filein->float_read();
			if ( debug ) printf ( "        RGB_VAL = %f %f %f\n", rgb_val[0], rgb_val[1], rgb_val[2] );
			return 4 + 3*4;
		}
	case COLOR_24:
		{
			if ( debug ) printf ( "        COLOR_24 24 bit color definition section tag of %04X\n", selector );
			unsigned char true_c_val[3];
			for (int i = 0; i < 3; ++i ) 
			{
				true_c_val[i] = filein->fgetc();
				rgb_val[i] = float(true_c_val[i])/255.0f;
			}
			if ( debug ) printf ( "        TRUE_C_VAL = %d %d %d\n", true_c_val[0], true_c_val[1], true_c_val[2] );
			return 4 + 3;
		}
	case LIN_COLOR_24:
		{
			if ( debug ) printf ( "        LIN_COLOR_24 24 bit color definition section tag of %04X\n", selector );
			unsigned char true_c_val[3];
			for (int i = 0; i < 3; ++i ) 
			{
				true_c_val[i] = filein->fgetc();
				rgb_val[i] = float(true_c_val[i])/255.0f;
			}
			if ( debug ) printf ( "        TRUE_C_VAL = %d %d %d\n", true_c_val[0], true_c_val[1], true_c_val[2] );
			return 4 + 3;
		}
	default:
        if ( debug ) printf ( "        Warning, junk COLOR definition section tag of %04X\n", selector );
        //Seeking to end of the section
        filein->fseek(color_chsize-6,SEEK_CUR);
		return 4 + 0;
	}
}
/******************************************************************************/

unsigned long tds_read_mat_percentage_chunk (FileIO *filein, float &val)

/******************************************************************************/
// @return number of bytes read from file
{
	unsigned long current_pointer = filein->ftell() - 2;
	unsigned long chunks_size = filein->long_uint_read();
    unsigned short subchunk_type = filein->short_uint_read();
    // subchunk should be INT_PERCENTAGE
    if (subchunk_type!=0x0030)
    {
        unsigned long current_pointer = filein->ftell() - 2;
        unsigned long temp_pointer = filein->long_uint_read();
        if ( debug )
          printf ( "           Warning, not INT_PERCENTAGE section; skipped %lu bytes\n",chunks_size );
        long pointer = current_pointer + chunks_size;
        filein->fseek(pointer, SEEK_SET );
        return chunks_size;
    } else
    {
	unsigned long subchunk_size = filein->long_uint_read();
    unsigned short shiness = filein->short_uint_read();
    val = ((float)shiness)/100.0;
    }
	return chunks_size;
}
/******************************************************************************/

unsigned long tds_read_color_chunk ( FileIO *filein, vec3& rgb)

/******************************************************************************/
//! @return number of bytes read from file
{
	long chunks_size = filein->long_uint_read();
	long teller = 6;
	
	while (teller < chunks_size)
    {
		unsigned short color_type = filein->short_uint_read();
		teller+=2+tds_read_color(filein,color_type,rgb);
	}	
	return chunks_size;
}
/******************************************************************************/

unsigned long tds_read_ambient_section ( FileIO *filein )

/******************************************************************************/
{
	vec3 rgb_val;
	
	unsigned long current_pointer = filein->ftell() - 2;	
	unsigned long temp_pointer=tds_read_color_chunk(filein,rgb_val);
	long pointer = current_pointer + temp_pointer;
	filein->fseek(pointer, SEEK_SET );
	
	return temp_pointer;
}
/******************************************************************************/

unsigned long tds_read_background_section ( FileIO *filein )

/******************************************************************************/
{
	vec3 rgb_val;
	
	unsigned long current_pointer = filein->ftell() - 2;	
	unsigned long temp_pointer=tds_read_color_chunk(filein,rgb_val);
	long pointer = current_pointer + temp_pointer;
	filein->fseek(pointer, SEEK_SET );
	
	return temp_pointer;
}
/******************************************************************************/

unsigned long tds_read_boolean ( unsigned char *boolean, FileIO *filein )

/******************************************************************************/

{
	unsigned long current_pointer;
	long      pointer;
	unsigned long temp_pointer;
	
	current_pointer = filein->ftell() - 2;
	temp_pointer = filein->long_uint_read();
	
	*boolean = filein->fgetc();
	
	pointer = ( long ) ( current_pointer + temp_pointer );
	filein->fseek(pointer, SEEK_SET );
	
	return ( temp_pointer );
}
/******************************************************************************/

unsigned long tds_read_camera_section ( FileIO *filein,unsigned long camera_idx )

/******************************************************************************/
{
	float               camera_eye[3];
	float               camera_focus[3];
	unsigned long   current_pointer;
	float               lens;
	long            pointer;
	float               rotation;
	unsigned long   temp_pointer;
	unsigned short int  u_short_int_val;
	
	current_pointer = filein->ftell() - 2;
	temp_pointer = filein->long_uint_read();
	
	camera_eye[0] = filein->float_read();
	camera_eye[1] = filein->float_read();
	camera_eye[2] = filein->float_read();
	
	camera_focus[0] = filein->float_read();
	camera_focus[1] = filein->float_read();
	camera_focus[2] = filein->float_read();
	
	rotation = filein->float_read();
	lens = filein->float_read();
	
	if ( debug ) {
		printf ( " Found camera viewpoint at XYZ = %f %f %f\n",
			camera_eye[0], camera_eye[1], camera_eye[2] );
		printf ( "     Found camera focus coordinates at XYZ = %f %f %f\n",
			camera_focus[0], camera_focus[1], camera_focus[2] );
		printf ( "     Rotation of camera is:  %f\n", rotation );
		printf ( "     Lens in used camera is: %f mm\n", lens );
	}
	
	if ( ( temp_pointer-38 ) > 0 )
    {
		if ( debug )
			printf ( "          Found extra camera sections\n" );
		
		u_short_int_val = filein->short_uint_read();
		
		if ( u_short_int_val == CAM_SEE_CONE ) {
			if ( debug ) {
				printf ( "          CAM_SEE_CONE\n" );
			}
			tds_read_unknown_section ( filein );
		}
		
		u_short_int_val = filein->short_uint_read();
		
		if ( u_short_int_val == CAM_RANGES ) {
			if ( debug ) {
				printf ( "          CAM_RANGES\n" );
			}
			tds_read_unknown_section ( filein );
		}
		
	}
	
	pointer = ( long ) ( current_pointer + temp_pointer );
	filein->fseek(pointer, SEEK_SET );
	
	return ( temp_pointer );
}

/******************************************************************************/

int tds_read_name ( FileIO *filein, char *temp_name )

/******************************************************************************/
{
	unsigned char  letter;
	unsigned int   teller;

	teller = 0;
	letter = filein->fgetc();

	//Could be a dummy object.  
	if ( letter == 0 )
        {
		sprintf ( temp_name,"%s%03d",DEFAULT_ITEM_NAME,0 );
		return (-1); 
	}
	
	temp_name[teller] = letter;
	teller+=1;
	
	do {
		letter = filein->fgetc();
		temp_name[teller] = letter;
		teller+=1;
	} while ( ( letter != 0 ) && ( teller < 12 ) );
	
	temp_name[teller-1] = 0;
	
	if ( debug ) {
		printf ( "      TDS_READ_NAME found name: \"%s\"\n", temp_name );
	}
	
	return 0;
}
/******************************************************************************/

int tds_read_long_name ( FileIO *filein, char *temp_name )

/******************************************************************************/
{
	unsigned char  letter;
	unsigned int   teller;
	
	teller = 0;
	letter = filein->fgetc();

	//Could be a dummy object. 
	if ( letter == 0 ) {
		printf ( "        TDS_READ_LONG_NAME string not found, using default\n");
		sprintf ( temp_name,"%s%03d",DEFAULT_ITEM_NAME,0 );
		return -1; 
	}
	
	temp_name[teller] = letter;
	teller+=1;
	
	do {
		letter = filein->fgetc();
		temp_name[teller] = letter;
		teller+=1;
	} while ( letter != 0 );
	
	temp_name[teller-1] = 0;
	
	if ( debug ) {
		printf ( "        TDS_READ_LONG_NAME found name: \"%s\"\n", temp_name );
	}
	
	return teller;
}
/******************************************************************************/

unsigned long tds_read_spot_section ( FileIO *filein )

/******************************************************************************/
{
	unsigned long  current_pointer;
	float              falloff;
	float              hotspot;
	long           pointer;
	float              target[4];
	unsigned long  temp_pointer;
	
	current_pointer = filein->ftell() - 2;
	temp_pointer = filein->long_uint_read();
	
	target[0] = filein->float_read();
	target[1] = filein->float_read();
	target[2] = filein->float_read();
	hotspot = filein->float_read();
	falloff = filein->float_read();
	
	if ( debug ) {
		printf ( "      The target of the spot is XYZ = %f %f %f\n",
			target[0], target[1], target[2] );
		printf ( "      The hotspot of this light is %f\n", hotspot );
		printf ( "      The falloff of this light is %f\n", falloff );
	}
	
	pointer = ( long ) ( current_pointer + temp_pointer );
	
	filein->fseek(pointer, SEEK_SET );
	
	return ( temp_pointer );
}
/******************************************************************************/

unsigned long tds_read_light_section ( FileIO *filein,unsigned long light_idx )

/******************************************************************************/
/*
Purpose:
  Reads N_DIRECT_LIGHT section in 3DS file
*/
{
	unsigned char   boolean;
	unsigned long   current_pointer;
	bool            end_found = false;
	float           light_coors[3];
	long            pointer;
	vec3            rgb_val;
	unsigned long   teller;
	unsigned long   temp_pointer;
	unsigned short int temp_int;
	
	current_pointer = filein->ftell() - 2;
	temp_pointer = filein->long_uint_read();
	teller = 6;
	
	light_coors[0] = filein->float_read();
	light_coors[1] = filein->float_read();
	light_coors[2] = filein->float_read();

	teller+=3 * 4;  //sizeof(float)=4
	
	if ( debug ) {
		printf ( "     LIGHT_SECTION: Found light at coordinates XYZ = %f %f %f\n",
			light_coors[0], light_coors[1], light_coors[2] );
	}

	while (!end_found)
    {
		temp_int = filein->short_uint_read();
		
		switch ( temp_int ) {
		case COLOR_F:
		case COLOR_24:
			if ( debug )
				printf ( "      LIGHT_SECTION: COLOR section: %04X\n", temp_int );
            teller+=2;
			teller+=tds_read_color(filein,temp_int,rgb_val);
			break;
		case 0x4610:
			if ( debug  )
				printf ( "      LIGHT_SECTION: DL_SPOTLIGHT section tag of %04X\n", temp_int );
            teller+=2;
			teller+=tds_read_spot_section ( filein );
			break;
		case 0x4620:
			if ( debug )
				printf ( "      LIGHT_SECTION: DL_OFF section: %04X\n", temp_int );
            teller+=2;
			teller+=tds_read_boolean ( &boolean, filein );
			if ( debug )
            {
				if (boolean)
					printf ( "        Light is on\n" );
				else
					printf ( "        Light is off\n" );
			}
			break;
		case 0x4625:
			if (debug)
				printf("      LIGHT_SECTION: DL_ATTENUATE section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4627:
			if (debug)
				printf("      LIGHT_SECTION: DL_RAYSHAD section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4630:
			if (debug)
				printf("      LIGHT_SECTION: DL_SHADOWED section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4640:
			if (debug)
				printf("      LIGHT_SECTION: DL_LOCAL_SHADOW section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4641:
			if (debug)
				printf("      LIGHT_SECTION: DL_LOCAL_SHADOW2 section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4650:
			if (debug)
				printf("      LIGHT_SECTION: DL_SEE_CONE section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4651:
			if (debug)
				printf("      LIGHT_SECTION: DL_SPOT_RECTANGULAR section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4652:
			if (debug)
				printf("      LIGHT_SECTION: DL_SPOT_OVERSHOOT section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4653:
			if (debug)
				printf("      LIGHT_SECTION: DL_SPOT_PROJECTOR section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4654:
			if (debug)
				printf("      LIGHT_SECTION: DL_EXCLUDE section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4655:
			if (debug)
				printf("      LIGHT_SECTION: DL_RANGE section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4656:
			if (debug)
				printf("      LIGHT_SECTION: DL_SPOT_ROLL section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4657:
			if (debug)
				printf("      LIGHT_SECTION: DL_SPOT_ASPECT section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4658:
			if (debug)
				printf("      LIGHT_SECTION: DL_RAY_BIAS section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x4659:
			if (debug)
				printf("      LIGHT_SECTION: DL_INNER_RANGE section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x465A:
			if (debug)
				printf("      LIGHT_SECTION: DL_OUTER_RANGE section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		case 0x465B:
			if (debug)
				printf("      LIGHT_SECTION: DL_MULTIPLIER section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section ( filein );
			break;
		default:
			if ( debug  )
				printf ( "      LIGHT_SECTION: Warning, junk tag of %04X\n",temp_int);
            teller+=2;
			break;
		}
		
		if ( (filein->ftell()-current_pointer) >= temp_pointer )
			end_found = true;
        else
          if (filein->feof())
          {
            printf ( "  LIGHT_SECTION: Error, unexpected file end\n");
            break;
          }
	}
	
	pointer = ( long ) ( current_pointer + temp_pointer );
	filein->fseek(pointer, SEEK_SET );
	
	return ( temp_pointer );
}
/******************************************************************************/

unsigned long IVCONV::tds_read_obj_section ( FileIO *filein, unsigned long mesh_idx,array<NameItm> &face_material_names)

/******************************************************************************/
/*
Purpose:
  tds_read_obj_section reads the section containing face data.

Comments:
  Thanks to John F Flanagan for some suggested corrections.

	Modified: 27 February 2007 Tomasz Lis material reading and meshes support
*/
{
  unsigned long   current_pointer;
  unsigned long   temp_pointer;

  unsigned long   arr_temp_pointer;
  unsigned long   arr_curr_pointer;

  unsigned long   teller; 
  unsigned char   end_found = false;

  unsigned long   cor3_num_base;
  unsigned long   cor3_num_inc;
  unsigned long   face_num_base;
  unsigned long   face_num_inc;
	
  current_pointer = filein->ftell() - 2;
  temp_pointer = filein->long_uint_read();
  teller = 6;
  cor3_num_base = vertex_num;
  cor3_num_inc = 0;
  face_num_base = face_num;
  face_num_inc = 0;
  TDS_TEXTURE_MAP_DATA map_data;
  char temp_name[FNAME_LEN];
	
  while (!end_found)
  {
      unsigned int i,j;
      unsigned short int color_index;
      unsigned int  chunk_tag;
      chunk_tag = filein->short_uint_read();
      if ( debug )
          printf ( "       TDS_READ_OBJ_SECTION reading tag at %06X\n", filein->ftell() );
		
      switch ( chunk_tag )
      {
		case 0x4000:
			if ( debug )
				printf ( "        Warning, junk NAMED_OBJECT section tag inside N_TRI_OBJECT section\n");
            teller+=2;
			//teller+=tds_read_unknown_section ( filein );
			break;
		case 0x4100:
			if ( debug )
				printf ( "        Warning, junk N_TRI_OBJECT section tag inside N_TRI_OBJECT section\n");
            teller+=2;
			//teller+=tds_read_unknown_section ( filein );
			break;
		case 0x4110:
			if ( debug )
				printf ( "        POINT_ARRAY section tag of %04X\n",chunk_tag);
			arr_curr_pointer = filein->ftell() - 2;
			arr_temp_pointer = filein->long_uint_read();
            cor3_num_base = vertex_num;
			cor3_num_inc =  ( int ) filein->short_uint_read();
			for ( i = vertex_num; i < vertex_num + cor3_num_inc; ++i ) {
				vertex[i].cor3[0] = filein->float_read();
				vertex[i].cor3[1] = filein->float_read();
				vertex[i].cor3[2] = filein->float_read();
			}
			vertex_num += cor3_num_inc;
			teller+=arr_temp_pointer;
			break;
		case 0x4111:
			if ( debug )
				printf ( "        POINT_FLAG_ARRAY faces (2) section tag of %04X\n",chunk_tag);
			teller+=tds_read_unknown_section ( filein );
			break;
		case FACE_ARRAY:
			if ( debug )
				printf ( "        FACE_ARRAY section tag of %04X\n",chunk_tag);
            teller+=2;
			arr_curr_pointer = filein->ftell() - 2;
			arr_temp_pointer = filein->long_uint_read();
            teller+=4;
            face_num_base = face_num;
			face_num_inc = filein->short_uint_read();
            teller+=2;
			for ( i = face_num; i < face_num + face_num_inc; ++i )
            {
                //Initialize faces. We can do this, because the chunks below FACE_ARRAY
                // are really sub-chunks of it, so there is no way a chunk could
                // write some data to new faces before this initialization.
                face[i]=Face();
				face[i].mesh = mesh_idx;
				face[i].vertices[0] = filein->short_uint_read() + cor3_num_base;
				face[i].vertices[1] = filein->short_uint_read() + cor3_num_base;
				face[i].vertices[2] = filein->short_uint_read() + cor3_num_base;
				face[i].order = 3;
				face[i].flags = filein->short_uint_read();
                teller+=4*2;
			}
            face_num += face_num_inc;
			if ( debug )
                printf ( "        FACE_ARRAY should end at %06X; its tag ends at %06X\n",
                           arr_curr_pointer+arr_temp_pointer,filein->ftell());
			break;

        //  TL: Chunks below are normally sub-chunks for FACE_ARRAY...
        //  .. but if we treat them as equal-level chunks, nothing bad will happen,
        //  so we're doing so.
		case 0x4150:
			if ( debug )
				printf ( "        SMOOTH_GROUP section tag of %04X\n", chunk_tag );
            teller+=2;
			arr_curr_pointer = filein->ftell() - 2;
			arr_temp_pointer = filein->long_uint_read();
            teller+=4;
            for ( i = face_num_base; i < face_num_base + face_num_inc; i++ )
            {
               face[i].smooth = filein->long_uint_read();
               // I don't think smoothing has something to do with vertices indexes
               // so I removed adding cor3_num_base. Before was:
               //face[i].smooth = (int) filein->long_uint_read() + cor3_num_base;
               teller+=4;
            }
			if ((debug)&&((arr_curr_pointer+arr_temp_pointer)!=filein->ftell()))
				printf ( "        SMOOTH_GROUP claims to end at %06X, ends at %06X\n",arr_curr_pointer+arr_temp_pointer,filein->ftell());
			break;
		case MSH_MAT_GROUP:
            {
			if ( debug )
				printf ( "        MSH_MAT_GROUP section tag of %04X\n",chunk_tag);
            teller+=2;
			arr_curr_pointer = filein->ftell() - 2;
			arr_temp_pointer = filein->long_uint_read();
            teller+=4;
            teller+=tds_read_long_name(filein,temp_name);
            unsigned long faces=filein->short_uint_read();
            teller+=2;
            for (i=0; i<faces; ++i)
            {
              unsigned long index=face_num_base + filein->short_uint_read();
              teller+=2;
              // We cannot set material indexes here, because we have only string names,
              // and materials may not be defined yet. Just storing names for higher function.
              strncpy(face_material_names[index].name, temp_name, LINE_MAX_LEN);
            }
			if ((debug)&&((arr_curr_pointer+arr_temp_pointer)!=filein->ftell()))
				printf ( "        MSH_MAT_GROUP claims to end at %06X, ends at %06X\n",arr_curr_pointer+arr_temp_pointer,filein->ftell());
			};break;
		case 0x4140:
			if ( debug )
				printf ( "        TEX_VERTS section tag of %04X\n",chunk_tag);
			teller+=tds_read_tex_verts_section ( filein );
			break;
		case 0x4160:
            {
			if ( debug )
				printf ( "        MESH_MATRIX section tag of %04X\n",chunk_tag);
            teller+=2;
			filein->long_uint_read();
            teller+=4;
			for ( j = 0; j < 4; ++j )
				for ( i = 0; i < 3; ++i )
                {
					transform_matrix[j][i] = filein->float_read();
                    teller+=4;
				}
			transform_matrix[0][3] = 0.0;
			transform_matrix[1][3] = 0.0;
			transform_matrix[2][3] = 0.0;
			transform_matrix[3][3] = 0.0;
			};break;
		case 0x4165:
			if ( debug )
				printf ( "        MESH_COLOR section tag of %04X\n", chunk_tag );
            teller+=2;
			arr_curr_pointer = filein->ftell() - 2;
			arr_temp_pointer = filein->long_uint_read();
            teller+=4;
			if ( arr_temp_pointer == 7 )
            {
				color_index = filein->fgetc();
				teller++;
			} else
            {
				color_index = filein->short_uint_read();
				teller+=2;
			} 
			if ( debug )
				printf ( "        Color index set to %d\n", color_index );
			break;
		case 0x4170:
            {
			if ( debug )
				printf ( "        MESH_TEXTURE_INFO section tag of %04X\n",chunk_tag);
			arr_curr_pointer = filein->ftell() - 2;
			arr_temp_pointer = filein->long_uint_read();
            teller+=2+4;
            for (i=0; i<2; ++i)
                map_data.tile[i]=filein->float_read();
            for (i=0; i<3; ++i)
                map_data.pos[i]=filein->float_read();
            map_data.scale=filein->float_read();
            teller+=2*4 + 3*4 + 4;
            tmat_init(map_data.matrix);
            for (i=0; i<4; i++)
              for (j=0; j<3; j++)
              {
                map_data.matrix[i][j]=filein->float_read();
                teller+=4;
              }
            for (i=0; i<2; ++i)
                map_data.planar_size[i]=filein->float_read();
            map_data.cylinder_height=filein->float_read();
            teller+=2*4 + 4;
			if ((debug)&&((arr_curr_pointer+arr_temp_pointer)!=filein->ftell()))
				printf ( "        MESH_TEXTURE_INFO claims to end at %06X, ends at %06X\n",
                          arr_curr_pointer+arr_temp_pointer,filein->ftell());
			//teller+=tds_read_unknown_section ( filein );
			};break;
		case 0x0000:
            teller+=2;
			if ( debug )
				printf ( "        FACE_ARRAY section end tag of %04X\n",chunk_tag);
			break;
		default:
			if ( debug )
				printf ( "        Warning, junk subsection tag of %04X in OBJ section\n",chunk_tag);
            teller+=2;
			break;
      } //end switch(...)

      if ( filein->ftell() >= current_pointer+temp_pointer )
        end_found = true;
      else
        if (filein->feof())
        {
            printf ( "  OBJ_SECTION: Error, unexpected file end\n");
            break;
        }
  }

  arr_curr_pointer = ( long ) ( current_pointer + temp_pointer );
  if ( debug )
      printf ( "       TDS_READ_OBJ_SECTION ends at %06X, reading moved to %06X\n",filein->ftell(),arr_curr_pointer);
  filein->fseek(arr_curr_pointer, SEEK_SET );
  
  return ( temp_pointer );
}

/******************************************************************************/

unsigned long IVCONV::tds_read_object_section ( FileIO *filein,array<NameItm> &face_material_names )

/******************************************************************************/
/*
Purpose:

  TDS_READ_OBJECT_SECTION reads an object (mesh).
  
	Author: John Burkardt
	Modified: 03 Mar 2007 Tomasz Lis - mesh name saving, multiple meshes support
*/
{
	unsigned long   current_pointer;
	unsigned long   temp_pointer;
	unsigned long   teller;
    char temp_name[FNAME_LEN];

	current_pointer = filein->ftell() - 2;
	temp_pointer = filein->long_uint_read();
	teller = 6;

	// read and save the name
	int             int_val;
	int_val = tds_read_name ( filein,temp_name );
	if (int_val==-1)
    {
		if ( debug )
			printf ( "      TDS_READ_OBJECT_SECTION: Dummy object name found\n" );
		sprintf( temp_name,"%s%03u",DEFAULT_ITEM_NAME,0);
	}
	
	bool end_found = false;
	while ( !end_found)
    {
        unsigned short  temp_int;
		temp_int = filein->short_uint_read();
		teller+=2;
		
		switch ( temp_int )
        {
		case 0x4700:
			if ( debug )
				printf ( "      N_CAMERA section tag of %04X\n", temp_int );
            camera[camera_num]=Camera();
            unsigned long camera_idx=camera_num;
            camera_num++;
            strcpy ( camera[camera_idx].name, temp_name );
			teller+=tds_read_camera_section ( filein,camera_idx );
			break;
		case 0x4600:
			if ( debug )
				printf ( "      N_DIRECT_LIGHT section tag of %04X\n", temp_int );
            light[light_num]=Light();
            unsigned long light_idx=light_num;
            light_num++;
            strcpy ( light[light_idx].name, temp_name );
			teller+=tds_read_light_section ( filein,light_idx );
			break;
		case 0x4100:
          {
			if ( debug )
				printf ( "      N_TRI_OBJECT section tag of %04X\n", temp_int );
            mesh[mesh_num]=Mesh();
            unsigned long mesh_idx=mesh_num;
            mesh_num++;
            strcpy ( mesh[mesh_idx].name, temp_name );
			teller+=tds_read_obj_section ( filein,mesh_idx,face_material_names );
		  };break;
		case 0x4010: 
			if ( debug )
				printf ( "      OBJ_HIDDEN section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x4012: 
			if ( debug )
				printf ( "      OBJ_DOESNT_CAST section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		default:
			if ( debug  )
				printf ( "        Warning, junk subsection tag of %04X in OBJECT_SECTION\n",temp_int);
			break;
		} //end switch(...)
		
    if ( (filein->ftell()-current_pointer) >= temp_pointer )
        end_found = true;
    else
        if (filein->feof())
        {
            printf ( "  OBJECT_SECTION: Error, unexpected file end\n");
            break;
        }
	}

  unsigned long pointer = current_pointer + temp_pointer;
  filein->fseek(pointer, SEEK_SET );
  return temp_pointer;
}
/******************************************************************************/

unsigned long tds_read_matdef_section ( FileIO *filein, char* mat_name)

/******************************************************************************/
{
	unsigned long  current_pointer = filein->ftell() - 2;
	unsigned long temp_pointer = filein->long_uint_read();
    char temp_name[FNAME_LEN];
	
	int teller = tds_read_long_name ( filein ,temp_name);
	
	if ( teller == -1 ) {
		if ( debug ) {
			printf ( "      TDS_READ_MATDEF_SECTION: No material name found\n" );
		}
	}
	else {
		strcpy ( mat_name, temp_name );
		if ( debug ) {
			printf ( "      TDS_READ_MATDEF_SECTION: Material name %s\n", mat_name );
		}
	}
	
	long pointer = current_pointer + temp_pointer;
	filein->fseek(pointer, SEEK_SET );
	
	return ( temp_pointer );
}
/******************************************************************************/

unsigned long IVCONV::tds_read_texmap_section ( FileIO *filein )

/******************************************************************************/
/*
Purpose:
  TDS_READ_TEXMAP_SECTION gets the texture name from the MAP_TEXMAP or other
   MAT_xxxMAP section.

  Author: John Burkardt
  Modified: 24 Feb 2007 Tomasz Lis
*/
{
	unsigned long  current_pointer;
	long           pointer;
	int                teller;
	unsigned long  temp_pointer;
    char temp_name[FNAME_LEN];
	
	int curr_texture=texture_num;
	texture_num++;
	
	current_pointer = filein->ftell() - 2;
	temp_pointer = filein->long_uint_read();
	
	filein->short_uint_read();
	filein->short_uint_read();
	filein->short_uint_read();
	filein->short_uint_read();
	

	// This next short int should equal A300.
	filein->short_uint_read();
	filein->long_uint_read();

	// Now read the TEXMAP file name.
	teller = tds_read_long_name ( filein,temp_name );
	
	if ( teller == -1 ) {
        strcpy ( texture[curr_texture].name, "" );
		if ( debug ) {
			printf ( "      MAP_xxxMAP: no texture name found\n" );
		}
	}
	else {
		strcpy ( texture[curr_texture].name, temp_name );
		if ( debug ) {
			printf ( "      MAP_xxxMAP: texture name %s\n", texture[curr_texture].name );
		}
	}
	
	pointer = ( long ) ( current_pointer + temp_pointer );
	filein->fseek(pointer, SEEK_SET );
	
	return ( temp_pointer );
}
/******************************************************************************/

unsigned long IVCONV::tds_read_material_section ( FileIO *filein )

/******************************************************************************/
{
	unsigned long   temp_pointer;
	unsigned long   teller;
	bool end_found = false;
	
	unsigned long current_pointer = filein->ftell() - 2;
	
	temp_pointer = filein->long_uint_read();
	teller = 6;
	
	while ( !end_found )
    {
        unsigned short int  temp_int;
		temp_int = filein->short_uint_read();
		teller+=2;
		if ( debug )
			printf ( "    TDS_READ_MATERIAL_SECTION reading tag at %06X\n", filein->ftell() );
		
		switch ( temp_int ) {
			
		case 0xa000:
			if ( debug ) {
				printf ( "     MAT_NAME definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_matdef_section ( filein,material[material_num].name);
			break;
		case 0xa010:
			if ( debug ) {
				printf ( "     MAT_AMBIENT definition section tag of %04X\n", temp_int );
			}
			teller+=tds_read_color_chunk(filein,material[material_num].ambient);
			break;
		case 0xa020:
			if ( debug ) {
				printf ( "     MAT_DIFFUSE definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_color_chunk(filein,material[material_num].rgb);
			break;
		case 0xa030:
			if ( debug ) {
				printf ( "     MAT_SPECULAR definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_color_chunk(filein,material[material_num].specular);
			break;
		case 0xa040:
			if ( debug ) {
				printf ( "     MAT_SHININESS definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_mat_percentage_chunk(filein,material[material_num].shiness);
			break;
		case 0xa041:
			if ( debug ) {
				printf ( "     MAT_SHIN2PCT definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa042:
			if ( debug ) {
				printf ( "     MAT_SHIN3PCT definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa050:
			if ( debug ) {
				printf ( "     MAT_TRANSPARENCY definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_mat_percentage_chunk(filein,material[material_num].alpha);
			break;
		case 0xa052:
			if ( debug ) {
				printf ( "     MAT_XPFALL definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa053:
			if (debug)
				printf("     MAT_REFBLUR definition section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa080:
			if (debug)
				printf("     MAT_SELF_ILLUM definition section tag of %04X\n",temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa081:
			if (debug)
				printf ( "     MAT_TWO_SIDE definition section tag of %04X\n",temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa082:
			if (debug)
				printf ( "     MAT_DECAL definition section tag of %04X\n",temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa083:
			if (debug)
				printf("     MAT_ADDITIVE definition section tag of %04X\n",temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa084:
			if ( debug ) {
				printf ( "     MAT_SELF_ILPCT definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa085:
			if ( debug ) {
				printf ( "     MAT_WIRE definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa086:
			if ( debug ) {
				printf ( "     MAT_SUPERSMP definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa087:
			if ( debug ) {
				printf ( "     MAT_WIRESIZE definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa088:
			if ( debug ) {
				printf ( "     MAT_FACEMAP definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa08a:
			if ( debug ) {
				printf ( "     MAT_XPFALLIN definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa08c:
			if ( debug ) {
				printf ( "     MAT_PHONGSOFT definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa08e:
			if ( debug ) {
				printf ( "     MAT_WIREABS definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa100:
			if ( debug ) {
				printf ( "     MAT_SHADING definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa200:
			if ( debug ) {
				printf ( "     MAT_TEXMAP definition section tag of %04X\n", 
					temp_int );
			}
			material[material_num].texture=texture_num;
			teller+=tds_read_texmap_section ( filein );
			break;
		case 0xa204:
			if ( debug ) {
				printf ( "     MAT_SPECMAP definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa210:
			if ( debug ) {
				printf ( "     MAT_OPACMAP definition section tag of %04X\n", 
					temp_int );
			}
			material[material_num].mask_texture=texture_num;
			teller+=tds_read_texmap_section ( filein );
			break;
		case 0xa220:
			if ( debug ) {
				printf ( "     MAT_REFLMAP definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xa230:
			if ( debug ) {
				printf ( "     MAT_BUMPMAP definition section tag of %04X\n", 
					temp_int );
			}
			material[material_num].bump_texture=texture_num;
			teller+=tds_read_texmap_section ( filein );
			break;
		case 0xa353:
			if ( debug ) {
				printf ( "     MAT_MAP_TEXBLUR definition section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		default:
			if ( debug )
				printf ( "  Warning, junk subsection tag of %04X in MATERIALS section\n", temp_int );
			break;	
		}	
        if ( (filein->ftell()-current_pointer) >= temp_pointer )
            end_found = true;
        else
          if (filein->feof())
          {
            printf ( "  MATERIALS_SECTION: Error, unexpected file end\n");
            break;
          }
	}
	material_num++;

	long pointer = current_pointer + temp_pointer;
	filein->fseek(pointer, SEEK_SET );
	return ( temp_pointer );
}
/******************************************************************************/

unsigned long tds_read_vp_section ( FileIO *filein, int *views_read )

/******************************************************************************/
{
	unsigned int       attribs;
	unsigned long  current_pointer;
	int                i;
	int                int_val;
	long           pointer;
	unsigned int       port;
	unsigned long  temp_pointer;
    char temp_name[FNAME_LEN];
	char *viewports[11] = {
		"Bogus",
			"Top",
			"Bottom",
			"Left",
			"Right",
			"Front",
			"Back",
			"User",
			"Camera",
			"Light",
			"Disabled"
	};
	
	*views_read = *views_read + 1;
	
	current_pointer = filein->ftell() - 2;
	temp_pointer = filein->long_uint_read();
	
	attribs = filein->short_uint_read();
	
	if ( attribs == 3 ) {
		if ( debug ) {
			printf ( "<Snap> active in viewport\n" );
		}
	}
	
	if ( attribs == 5 ) {
		if ( debug ) {
			printf ( "<Grid> active in viewport\n" );
		}
	}
	/* 
	Read 5 INTS to get to the viewport information. 
	*/
	for ( i = 1; i < 6; ++i ) {
		filein->short_uint_read(); 
	}
	
	port = filein->short_uint_read();
	/*
	Find camera section. 
	*/
	if ( ( port == 0xffff ) || ( port == 0 ) ) {
		
		for ( i = 0; i < 12; ++i ) {
			filein->short_uint_read();
		}
		
		int_val = tds_read_name (filein,temp_name );
		
		if ( int_val == -1 ) {
			if ( debug ) {
				printf ( "      No Camera name found\n" );
			}
		}
		
		port = 0x0008;
	}
	
	if ( debug ) {
		printf ( "Reading [%s] information with tag:%d\n", viewports[port], port );
	}
	
	pointer = ( long ) ( current_pointer + temp_pointer );
	
	filein->fseek(pointer, SEEK_SET ); 
	
	return ( temp_pointer );
}
/******************************************************************************/

unsigned long tds_read_view_section ( FileIO *filein, int *views_read )

/******************************************************************************/
{
	unsigned long   current_pointer;
	bool end_found = false;
	long            pointer;
	unsigned short int  temp_int;
	unsigned long   temp_pointer;
	unsigned long   teller;
	
	current_pointer = filein->ftell() - 2;
	temp_pointer = filein->long_uint_read();
	teller = 6;
	
	while (!end_found) {
		
		temp_int = filein->short_uint_read();
		teller+=2;
		if ( debug )
			printf ( "     TDS_READ_VIEW_SECTION reading tag at %06X\n", filein->ftell() );
		
		switch ( temp_int ) {
		case 0x7012:
			if ( debug ) {
				printf ( "     VIEWPORT_DATA_3 section tag of %04X\n", temp_int );
			}
			teller+=tds_read_vp_section ( filein, views_read );
			break;
		case 0x7011:
			if ( debug ) {
				printf ( "     VIEWPORT_DATA section tag of %04X\n", temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x7020:
			if ( debug ) {
				printf ( "     VIEWPORT_SIZE section tag of %04X\n", temp_int );
			}
			teller+=tds_read_vp_section ( filein, views_read );
			break;
		default:
			if ( debug  )
				printf ( "      Warning, junk VIEW_SECTION tag of %04X\n",temp_int);
			break;
		}
		
        if ( (filein->ftell()-current_pointer) >= temp_pointer )
            end_found = true;
		if ( *views_read > 3 )
			end_found = true;

          if ((!end_found)&&(filein->feof()))
          {
            printf ( "  VIEW_SECTION: Error, unexpected file end\n");
            break;
          }
	}
	
	pointer = ( long ) ( current_pointer + temp_pointer );
	
	filein->fseek(pointer, SEEK_SET );
	
	return ( temp_pointer );
}
/******************************************************************************/

unsigned long IVCONV::tds_read_edit_section ( FileIO *filein, int *views_read)

/******************************************************************************/
/*
Purpose:
  TDS_READ_EDIT_SECTION reads edit_section of 3DS file
  
  Modified: 18 Sep 1998
  Modified: 05 Mat 2007 Tomasz Lis some error handling
*/
{
	unsigned long   chunk_length;
	unsigned long   current_pointer;
	bool end_found = false;
	long            pointer;
	unsigned long   teller;
	unsigned short int  temp_int;

    //Prepare structure for reading "per face" material names
    //(if this NAMED_OBJECT contains any faces...)
    array<NameItm> face_material_names;
    unsigned long face_num_base=face_num;

	current_pointer = filein->ftell() - 2;
	chunk_length = filein->long_uint_read();
	teller = 6;
	
	while ( !end_found)
    {
		temp_int = filein->short_uint_read();
		teller+=2;
		if ( debug )
			printf ( "    TDS_READ_EDIT_SECTION reading tag at %06X\n", filein->ftell() );
		
		switch ( temp_int ) {
		case 0x1100:
			if ( debug ) {
				printf ( "    BIT_MAP section tag of %04X\n", temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x1201:
			if ( debug ) {
				printf ( "    USE_SOLID_BGND section tag of %04X\n", temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x1300:
			if ( debug )
				printf ( "    V_GRADIENT section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case LO_SHADOW_BIAS:
			if ( debug )
				printf ( "    LO_SHADOW_BIAS section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case SHADOW_MAP_SIZE:
			if ( debug )
				printf ( "    SHADOW_MAP_SIZE section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case SHADOW_FILTER:
			if ( debug )
				printf ( "    SHADOW_FILTER section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x1500:
			if ( debug )
				printf ( "    O_CONSTS section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x2200:
			if ( debug )
				printf ( "    FOG section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x2201:
			if ( debug )
				printf ( "    USE_FOG section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x2210:
			if ( debug )
				printf ( "    FOG_BGND section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x2300:
			if ( debug )
				printf ( "    DISTANCE_CUE section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x2302:
			if ( debug )
				printf ( "    LAYER_FOG section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x3000:
			if ( debug )
				printf ( "    DEFAULT_VIEW section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x2100:
			if ( debug ) {
				printf ( "    AMBIENT_LIGHT section tag of %04X\n", temp_int );
			}
			teller+=tds_read_ambient_section ( filein );
			break;
		case 0x1200:
			if ( debug ) {
				printf ( "    SOLID_BGND section tag of %04X\n", temp_int );
			}
			teller+=tds_read_background_section ( filein );
			break;
		case 0x0100:
			if ( debug ) {
				printf ( "    MASTER_SCALE section tag of %04X\n", temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x3d3e:
			if ( debug ) {
				printf ( "    MESH_VERSION section tag of %04X\n", temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0xafff:
			if ( debug ) {
				printf ( "    MAT_ENTRY section tag of %04X\n", temp_int );
			}
			teller+=tds_read_material_section ( filein );
			break;
		case 0x4000:
			if ( debug ) {
				printf ( "    NAMED_OBJECT section tag of %04X\n", temp_int );
			}
			teller+=tds_read_object_section(filein,face_material_names);
			break;
		case 0x7001:
			if ( debug ) {
				printf ( "    VIEWPORT_LAYOUT section tag of %04X\n", 
					temp_int );
			}
			teller+=tds_read_view_section ( filein, views_read );
			break;
		case 0x7012:
			if ( debug ) {
				printf ( "    VIEWPORT_DATA_3 section tag of %04X\n", temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x7011:
			if ( debug ) {
				printf ( "    VIEWPORT_DATA section tag of %04X\n", temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		case 0x7020:
			if ( debug ) {
				printf ( "    VIEWPORT_SIZE section tag of %04X\n", temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;
		default:
			if ( debug )
				printf ( "    Warning, junk EDIT_SECTION tag of %04X\n",temp_int);
			break;
    }
	
    if ( (filein->ftell()-current_pointer) >= chunk_length )
        end_found = true;
    else
    if (filein->feof())
    {
        printf ( "TDS_READ_EDIT_SECTION: Error, unexpected file end\n");
        break;
    }
  }

  //At last, we can set material infos to vertices!
  //UV mapping is already set, so we're skipping it.
  array<FaceUV> face_uv;
  unsigned long face_uv_num; //there should be face_uv_num==face_num_inc
  clear_faces_uv(face_uv,face_num_base,face_uv_num);
  //set material indexes in face_uv
  face_materials_to_indexes(face_material_names,face_uv,face_uv_num);
  //Now copy indexes to vertices
  face_to_vertex_material(face_uv,face_num_base,face_uv_num);

  pointer = ( long ) ( current_pointer + chunk_length );
  filein->fseek(pointer, SEEK_SET );
  return ( chunk_length );
}
/******************************************************************************/

unsigned long IVCONV::tds_read_keyframe_objdes_section ( FileIO *filein )

/******************************************************************************/
/*

Modified: 21 September 1998
*/
{
	unsigned long   chunk_size;
	unsigned long   current_pointer;
	unsigned char   end_found = false;
	long            pointer;
	unsigned short int  temp_int;
	unsigned long   temp_pointer;
	unsigned long   teller;
	unsigned long   u_long_int_val;
	unsigned short int  u_short_int_val;
	
	current_pointer = filein->ftell() - 2;
	temp_pointer = filein->long_uint_read();
	teller = 6;
	
	while (!end_found)
    {
		temp_int = filein->short_uint_read();
		teller+=2;
		
		switch ( temp_int )
        {
		case 0xb010:
			if (debug)
				printf("      OBJDES_SECTION: NODE_HDR section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb011:
			if (debug)
				printf("      OBJDES_SECTION: INSTANCE_NAME section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb012:
			if (debug)
				printf("      OBJDES_SECTION: PRESCALE section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb013:
            {
			if (debug)
				printf("      OBJDES_SECTION: PIVOT section tag of %04X\n",temp_int);
			chunk_size = filein->long_uint_read();
			view.pivot[0] = filein->float_read();
			view.pivot[1] = filein->float_read();
			view.pivot[2] = filein->float_read();
			teller+=12;
			};break;

		case 0xb014:
			if (debug)
				printf("      OBJDES_SECTION: BOUNDBOX section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb015:
			if (debug)
				printf("      OBJDES_SECTION: MORPH_SMOOTH section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb020:
            {
			if (debug)
				printf("      OBJDES_SECTION: POS_TRACK_TAG section tag of %04X\n",temp_int);
			chunk_size = filein->long_uint_read();
			if ( debug )
				printf ( "        Chunk size is %d\n", chunk_size );
			u_short_int_val = filein->short_uint_read();
			u_short_int_val = filein->short_uint_read();
			u_short_int_val = filein->short_uint_read();
			u_short_int_val = filein->short_uint_read();
			u_short_int_val = filein->short_uint_read();
			u_short_int_val = filein->short_uint_read();
			u_short_int_val = filein->short_uint_read();
			u_short_int_val = filein->short_uint_read();
			u_long_int_val = filein->long_uint_read();

			view.origin[0] = filein->float_read();
			view.origin[1] = filein->float_read();
			view.origin[2] = filein->float_read();
			teller+=32;
			};break;

		case 0xb021:
			if (debug)
				printf("      OBJDES_SECTION: ROT_TRACK_TAG section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb022:
			if (debug)
				printf("      OBJDES_SECTION: SCL_TRACK_TAG section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb023:
			if (debug)
				printf("      OBJDES_SECTION: FOV_TRACK_TAG section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb024:
			if (debug)
				printf("      OBJDES_SECTION: ROLL_TRACK_TAG section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb025:
			if (debug)
				printf("      OBJDES_SECTION: COL_TRACK_TAG section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb026:
			if (debug)
				printf("      OBJDES_SECTION: MORPH_TRACK_TAG section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb027:
			if (debug)
				printf("      OBJDES_SECTION: HOT_TRACK_TAG section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb028:
			if (debug)
				printf("      OBJDES_SECTION: FALL_TRACK_TAG section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb029:
			if (debug)
				printf("      OBJDES_SECTION: HIDE_TRACK_TAG section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;

		case 0xb030:
			if ( debug ) {
				printf ( "      OBJDES_SECTION: NODE_ID section tag of %04X\n", temp_int );
			}
			teller+=tds_read_unknown_section ( filein );
			break;

		default: 
			if ( debug  )
				printf ( "      OBJDES_SECTION: Warning, junk section tag of %04X\n",temp_int);
			//teller+=tds_read_unknown_section ( filein );
			break;
		}
		
        if ( (filein->ftell()-current_pointer) >= temp_pointer )
            end_found = true;
        else
          if (filein->feof())
          {
            printf ( "  KEYFRAME_OBJDES_SECTION: Error, unexpected file end\n");
            break;
          }
	}
	
	pointer = ( long ) ( current_pointer+temp_pointer );
	filein->fseek(pointer, SEEK_SET );
	
	return ( temp_pointer );
}

/******************************************************************************/

unsigned long IVCONV::tds_read_keyframe_section ( FileIO *filein, int *views_read )

/******************************************************************************/
/*
Purpose:
  Reads KFDATA section of B000 in 3DS file.
*/
{
	unsigned long   current_pointer;
	bool end_found = false;
	long            pointer;
	unsigned short int  temp_int;
	unsigned long   temp_pointer;
	unsigned long   teller;
	
	current_pointer = filein->ftell() - 2;
	temp_pointer = filein->long_uint_read();
	teller = 6;
	
	while (!end_found)
    {
		temp_int = filein->short_uint_read();
		teller+=2;
		
		switch ( temp_int )
        {
		case 0x7001:
             //TL: I have no idea why reading of this section is here. This may be an error.
			if ( debug )
				printf ( "    KFDATA_SECTION: VIEWPORT_LAYOUT main definition section tag of %04X\n",
					temp_int );
			teller+=tds_read_view_section ( filein, views_read );
			break;

		case 0xb001:
			if (debug)
				printf ( "    KFDATA_SECTION: AMBIENT_NODE_TAG object description section tag of %04X\n",
					temp_int);
			teller+=tds_read_keyframe_objdes_section (filein);
			break;

		case 0xb002:
			if (debug)
				printf ( "    KFDATA_SECTION: OBJECT_NODE_TAG object description section tag of %04X\n",
					temp_int);
			teller+=tds_read_keyframe_objdes_section (filein);
			break;

		case 0xb003:
			if (debug)
				printf ( "    KFDATA_SECTION: CAMERA_NODE_TAG object description section tag of %04X\n",
					temp_int);
			teller+=tds_read_keyframe_objdes_section (filein);
			break;

		case 0xb004:
			if (debug)
				printf ( "    KFDATA_SECTION: TARGET_NODE_TAG object description section tag of %04X\n",
					temp_int);
			teller+=tds_read_keyframe_objdes_section (filein);
			break;

		case 0xb005:
			if (debug)
				printf ( "    KFDATA_SECTION: LIGHT_NODE_TAG object description section tag of %04X\n",
					temp_int);
			teller+=tds_read_keyframe_objdes_section (filein);
			break;

		case 0xb006:
			if (debug)
				printf ( "    KFDATA_SECTION: L_TARGET_NODE_TAG object description section tag of %04X\n",
					temp_int);
			teller+=tds_read_keyframe_objdes_section (filein);
			break;

		case 0xb007:
			if (debug)
				printf ( "    KFDATA_SECTION: SPOTLIGHT_NODE_TAG object description section tag of %04X\n",
					temp_int);
			teller+=tds_read_keyframe_objdes_section (filein);
			break;

		case 0xb008:
			if ( debug )
				printf ( "    KFDATA_SECTION: KFSEG frames section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;

		case 0xb009:
			if ( debug )
				printf ( "    KFDATA_SECTION: KFCURTIME section tag of %04X\n", temp_int );
			teller+=tds_read_unknown_section ( filein );
			break;

		case 0xb00a:
			if (debug)
				printf("    KFDATA_SECTION: KFHDR section tag of %04X\n",temp_int);
			teller+=tds_read_unknown_section(filein);
			break;
		default: 
			if ( debug  )
				printf ( "      KFDATA_SECTION: Warning, junk section tag of %04X\n",temp_int);
			//teller+=tds_read_unknown_section(filein);
			teller+=2;
			break;
		}

        if ( (filein->ftell()-current_pointer) >= temp_pointer )
            end_found = true;
        else
          if (filein->feof())
          {
            printf ( "  KFDATA_SECTION: Error, unexpected file end\n");
            break;
          }
	}//end while (...)
	
	pointer = ( long ) ( current_pointer + temp_pointer );
	filein->fseek(pointer, SEEK_SET );
	
	return ( temp_pointer );
}
/******************************************************************************/

unsigned long IVCONV::tds_read_tex_verts_section ( FileIO *filein )

/******************************************************************************/
/*
Purpose:

  TDS_READ_TEX_VERTS_SECTION reads the texture UV mapping data for vertices.
  
	Discussion:
	
	  The texture vertex data seems to be associated with nodes.  This routine
	  distributes that data to vertices (nodes as they make up a particular
	  face).
	  
    Author: John Burkardt
    Modified: 02 July 1999
	Modified: 02 Mar 2007 Tomasz Lis
*/
{
	unsigned long current_pointer = filein->ftell() - 2;
	unsigned long temp_pointer = filein->long_uint_read();
	unsigned long pointer = ( current_pointer + temp_pointer );
	unsigned short n2 = filein->short_uint_read();
	unsigned int icor3;
	for ( icor3 = 0; icor3 < n2; icor3++ )
    {
		vertex[icor3].tex_uv[0] = filein->float_read();
		vertex[icor3].tex_uv[1] = filein->float_read();
	}
	filein->fseek(pointer, SEEK_SET );
	return ( temp_pointer );
}
/******************************************************************************/

int tds_chunk_write(TDS_CHUNK *c, FileIO *fileout)

/******************************************************************************/
/*
Purpose:

  tds_chunk_write writes chunk of 3DS file if the size of it is known at start
  
	Author: Tomasz Lis
	Modified: 05 Mar 2007
*/
{
  if (c==NULL) return 0;
  int bytes_count=0;
  bytes_count+=fileout->short_int_write(c->chunk);
  if (bytes_count!=2)
    return 0;
  bytes_count+=fileout->long_int_write(c->size);
  if (bytes_count!=6)
    return 0;
  return bytes_count;
}
/******************************************************************************/

int tds_chunk_write_start(TDS_CHUNK *c, FileIO *fileout)

/******************************************************************************/
/*
Purpose:

  tds_chunk_write_start writes any chunk of 3DS file and stores its position
    to write its size in the futute.
  
	Author: Tomasz Lis
	Modified: 05 Mar 2007
*/
{
  if (c==NULL) return 0;
  c->size=0;
  c->cur=fileout->ftell();
  int bytes_count=0;
  bytes_count+=fileout->short_int_write(c->chunk);
  if (bytes_count!=2)
    return 0;
  bytes_count+=fileout->long_int_write(c->size);
  if (bytes_count!=6)
    return 0;
  return bytes_count;
}
/******************************************************************************/

bool tds_chunk_write_end(TDS_CHUNK *c, FileIO *fileout)

/******************************************************************************/
/*
Purpose:

  lib3ds_chunk_write_end writes chunk header again and stores right chunk size
  
	Author: Tomasz Lis
	Modified: 05 Mar 2007
*/
{
  if (c==NULL) return false;
  c->size=fileout->ftell() - c->cur;
  fileout->fseek( c->cur+2, SEEK_SET );
  if (fileout->long_int_write(c->size)!=4)
    return false;
  c->cur+=c->size;
  fileout->fseek( c->cur, SEEK_SET );
  return true;
}
/******************************************************************************/

int tds_write_name_chunk ( FileIO *fileout, const unsigned long chnk_type, const char *string )

/******************************************************************************/
{
    //Getting max string size
    int max_size;
    switch (chnk_type)
    {
    case MAT_NAME:    max_size=16; break;
    case MAT_MAPNAME: max_size=12; break;
    default:  max_size=12; break;
    };
    //Preparing name to write
    char map_name[129];
    strncpy(map_name,string,max_size);
    map_name[max_size]=0;
    //Creating chunk header
    TDS_CHUNK c_anyname;
    c_anyname.chunk=chnk_type;
    c_anyname.size=6+strlen(map_name)+1;
    if (debug)
      printf ( "      WRITE_NAME_CHUNK: writing chunk with name \"%s\" at %06X\n",
        map_name, fileout->ftell() );
    int bytes_count=0;
    bytes_count+=tds_chunk_write(&c_anyname, fileout);
    bytes_count+=tds_write_limited_string(fileout,map_name,max_size);
    return bytes_count;
}
/******************************************************************************/

unsigned long tds_write_map_chunk (FileIO *fileout,unsigned short mask_type,const Texture &txtr)

/******************************************************************************/
/*
Purpose:
  Writes a map chunk to material section.

  Author: Tomasz lis
  Modified: 15 Mar 2007
*/
{
       unsigned long bytes_count=0;
       unsigned short short_int_val;
       if (debug)
           printf ( "    MAT_TEXMAP: writing map chunk for %s at %06X\n",
            txtr.name, fileout->ftell() );
       // MAT_TEXMAP begin.
       TDS_CHUNK c_mattexmap;
       c_mattexmap.chunk=mask_type;
       tds_chunk_write_start(&c_mattexmap, fileout);
       bytes_count += 6;
         // INT_PERCENTAGE for storing the transparency level
          short_int_val = (unsigned short)0x0030;
          bytes_count += fileout->short_int_write( short_int_val );
          long            l_mat_texmap_0030 = 6 + 2;
          bytes_count += fileout->long_int_write( l_mat_texmap_0030 );
          unsigned short alpha=(unsigned short)floor(100.0*(txtr.intense)+0.5);
          bytes_count += fileout->short_int_write( alpha );
       // MAT_MAPNAME begin.
        bytes_count += tds_write_name_chunk(fileout,MAT_MAPNAME,txtr.name);
       // MAT_MAPNAME ends.
       // MAT_TEXMAP ends.
       tds_chunk_write_end(&c_mattexmap, fileout);
}
/******************************************************************************/

unsigned long IVCONV::tds_write_material_section (FileIO *fileout,unsigned int matidx)

/******************************************************************************/
{
    unsigned short  short_int_val;

    //Materials chunk header
    TDS_CHUNK c_matrlhdr;
    c_matrlhdr.chunk=0xAFFF;
    tds_chunk_write_start(&c_matrlhdr, fileout);
    stats.bytes_num += 6;

    {
   //---- MAT_NAME ----
    stats.bytes_num += tds_write_name_chunk(fileout,MAT_NAME,material[matidx].name);
    }

  { //---- MAT_AMBIENT ----
    TDS_CHUNK c_matambint;
    c_matambint.chunk=0xA010;
    c_matambint.size= 6+18;
    tds_chunk_write(&c_matambint, fileout);
    stats.bytes_num += 6;
    short_int_val = (unsigned short)0xA010;
    stats.bytes_num += tds_color_write(fileout,material[matidx].ambient);
  }
  { //---- MAT_DIFFUSE ----
    TDS_CHUNK c_matdiffuse;
    c_matdiffuse.chunk=0xA020;
    c_matdiffuse.size= 6+18;
    tds_chunk_write(&c_matdiffuse, fileout);
    stats.bytes_num += 6;
    stats.bytes_num += tds_color_write(fileout,material[matidx].rgb);
  }
  { //---- MAT_SPECULAR ----
    TDS_CHUNK c_matspecul;
    c_matspecul.chunk=0xA030;
    c_matspecul.size= 6+18;
    tds_chunk_write(&c_matspecul, fileout);
    stats.bytes_num += 6;
    stats.bytes_num += tds_color_write(fileout,material[matidx].specular);
  }
  { //---- MAT_SHININESS ----
	long            la040_0030 = 6 + 2;
	long            la040 = 6 + la040_0030;
    short_int_val = (unsigned short)0xA040;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	stats.bytes_num += fileout->long_int_write( la040 );
     // INT_PERCENTAGE for storing the alpha level
     short_int_val = (unsigned short)0x0030;
     stats.bytes_num += fileout->short_int_write( short_int_val );
     stats.bytes_num += fileout->long_int_write( la040_0030 );
     unsigned short shiness=(unsigned short)floor(100.0*material[matidx].shiness+0.5);
     stats.bytes_num += fileout->short_int_write( shiness );
  }
  { //---- MAT_TRANSPARENCY ----
	long            la050_0030 = 6 + 2;
	long            la050 = 6 + la050_0030;
    short_int_val = (unsigned short)0xA050;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	stats.bytes_num += fileout->long_int_write( la050 );
     // INT_PERCENTAGE for storing the alpha level
     short_int_val = (unsigned short)0x0030;
     stats.bytes_num += fileout->short_int_write( short_int_val );
     stats.bytes_num += fileout->long_int_write( la050_0030 );
     unsigned short alpha=(unsigned short)floor(100.0*material[matidx].alpha+0.5);
     stats.bytes_num += fileout->short_int_write( alpha );
  }
  { //Textures
    long txtr_idx;
    // MAT_TEXMAP
    txtr_idx=material[matidx].texture;
    if (txtr_idx>=0)
       stats.bytes_num+=tds_write_map_chunk(fileout,0xA200,texture[txtr_idx]);
    // MAT_OPACMAP
    txtr_idx=material[matidx].mask_texture;
    if (txtr_idx>=0)
       stats.bytes_num+=tds_write_map_chunk(fileout,0xA210,texture[txtr_idx]);
    // 	MAT_BUMPMAP
    txtr_idx=material[matidx].bump_texture;
    if (txtr_idx>=0)
       stats.bytes_num+=tds_write_map_chunk(fileout,0xA230,texture[txtr_idx]);
  }

    tds_chunk_write_end(&c_matrlhdr, fileout);
    return fileout->ftell()-c_matrlhdr.cur;
}
/******************************************************************************/

unsigned long IVCONV::tds_write_named_object (FileIO *fileout,unsigned int meshidx,array<FaceUV> &face_uv)

/******************************************************************************/
{
    const int max_meshname_size=10;
    int bytes_num=0;
	//NAMED_OBJECT begin. 
    //tag, size, name. 

    TDS_CHUNK c_namedobj;
    c_namedobj.chunk=NAMED_OBJECT;
    if (debug)
        printf ( "    MDATA: writing NAMED_OBJECT chunk of %04hX at %06X\n",
            c_namedobj.chunk, fileout->ftell() );
    bytes_num += tds_chunk_write_start(&c_namedobj, fileout);
    char mesh_name[max_meshname_size+1];
    strncpy(mesh_name,mesh[meshidx].name,max_meshname_size);
    mesh_name[max_meshname_size]='\0';
    if (debug)
        printf ( "        NAMED_OBJECT: writing mesh \"%s\"\n",
            mesh_name );
    bytes_num += tds_write_string ( fileout, mesh_name );

	//N_TRI_OBJECT begin.
    //tag, size.
    TDS_CHUNK c_ntriobj;
    c_ntriobj.chunk=0x4100;
    tds_chunk_write_start(&c_ntriobj, fileout);
    bytes_num += 6;
    if (debug)
        printf ( "        NAMED_OBJECT: writing N_TRI_OBJECT chunk of %04hX at %06X\n",
            c_ntriobj.chunk, fileout->ftell() );


    array<unsigned long> vert_idx;
    unsigned long vert_count=0;
    array<long> nvert_idx;
    unsigned long faces_count=0;
    get_mesh_vertices(vert_idx,vert_count,nvert_idx,faces_count,meshidx);

  {
	//POINT_ARRAY begin.  
    //tag, size, number of points, coordinates of points.
    TDS_CHUNK c_pointarray;
    c_pointarray.chunk=0x4110;
    c_pointarray.size= 2 + 4 + 2 + vert_count * 3 * 4;
    tds_chunk_write(&c_pointarray, fileout);
    bytes_num += 6;
    unsigned short u_short_int_val;
    if (vert_count<65536)
    {
        u_short_int_val = ( unsigned short ) vert_count;
    } else
    {
        printf ( "        POINT_ARRAY: Warning, number of points (%lu) exceeded 16-bits and was reduced!\n",vert_count);
        u_short_int_val = 65535;
    }
	bytes_num += tds_write_u_short_int ( fileout, u_short_int_val );
	unsigned long icor3;
	for ( icor3 = 0; icor3 < u_short_int_val; icor3++ )
	{
        unsigned long ver_idx_itm=vert_idx[icor3];
        unsigned int j;
        if (ver_idx_itm<vertex_num)
        {
            for ( j = 0; j < 3; ++j )
			    bytes_num += fileout->float_write( vertex[ver_idx_itm].cor3[j] );
        } else
        {
            printf ( "        POINT_ARRAY: Warning, vertex index %lu out of vertex array bounds!\n",ver_idx_itm);
            for ( j = 0; j < 3; ++j )
			    bytes_num += fileout->float_write( 0.0 );
        }
    }
	//POINT_ARRAY end.
    if (debug)
      printf ( "        POINT_ARRAY: Written %hu points coordinates\n",u_short_int_val);
  }

    //TEX_VERTS begin.
    {
        if (debug)
          printf ( "        TEX_VERTS: Writing UV coords for %lu vertices\n",vert_count);
        TDS_CHUNK c_mshmatrx;
        c_mshmatrx.chunk=0x4140;
        c_mshmatrx.size= 6 + 2 + 2*vert_count*4;
        tds_chunk_write(&c_mshmatrx, fileout);
        bytes_num += 6;
        bytes_num += fileout->short_int_write( vert_count );
        unsigned long icor3;
        for ( icor3 = 0; icor3 < vert_count; icor3++ )
        {
            unsigned long vert_idx_val=vert_idx[icor3];
            if (vert_idx_val>=vertex_num)
            {
              printf ( "        TEX_VERTS: Warning, vertex index %lu out of array bounds\n",vert_idx_val);
              bytes_num += fileout->float_write( 0.0 );
              bytes_num += fileout->float_write( 0.0 );
              continue;
            }
            bytes_num += fileout->float_write( vertex[vert_idx_val].tex_uv[0] );
            bytes_num += fileout->float_write( vertex[vert_idx_val].tex_uv[1] );
        }
    }
    //TEX_VERTS end.

  {
	//MESH_MATRIX begin.  
    if (debug)
      printf ( "        MESH_MATRIX: Writing transformation matrix\n");
    //tag, size, 4 by 3 matrix.
    TDS_CHUNK c_mshmatrx;
    c_mshmatrx.chunk=0x4160;
    c_mshmatrx.size= 2 + 4 + 4 * 12;
    tds_chunk_write(&c_mshmatrx, fileout);
    bytes_num += 6;
    unsigned int i,j;
	for ( i = 0; i < 4; ++i )
		for ( j = 0; j < 3; ++j )
        {
			float float_val = transform_matrix[i][j];
			bytes_num += fileout->float_write( float_val );
		}
	//MESH_MATRIX end.  
  }

	//FACE_ARRAY begin. 
    //tag, size, number of faces, nodes per face. 
    TDS_CHUNK c_facearray;
    c_facearray.chunk=0x4120;
    tds_chunk_write_start(&c_facearray, fileout);
    bytes_num += 6;
    if (debug)
      printf ( "        FACE_ARRAY: Writing %lu faces\n",faces_count);
    unsigned short u_short_int_val;
    if (faces_count<65536)
    {
        u_short_int_val = ( unsigned short ) faces_count;
    } else
    {
        printf ( "        FACE_ARRAY: Warning, number of faces (%lu) exceeded 16-bits and was reduced!\n",faces_count);
        u_short_int_val = 65535;
    }
	bytes_num += tds_write_u_short_int ( fileout, u_short_int_val );
	unsigned long iface;
	for ( iface = 0; iface < face_num; iface++ )
	  if (face[iface].mesh == meshidx)
      {
        unsigned int order=face[iface].order;
        if (order>3)
        {
            order=3;
            printf ( "        FACE_ARRAY: Warning, order of face %lu exceeds 3, reduced\n",iface);
        }
        unsigned int j;
        unsigned short short_int_val;
		for ( j = 0; j < order; ++j )
        {
            unsigned long vert_pos=face[iface].vertices[j];
            if (nvert_idx[vert_pos] >= 0)
                short_int_val = nvert_idx[vert_pos];
            else
                short_int_val = 0;
			bytes_num += fileout->short_int_write( short_int_val );
		}
		for ( ; j < 3; ++j )
			bytes_num += fileout->short_int_write( short_int_val );
		short_int_val = face[iface].flags;
		bytes_num += fileout->short_int_write( short_int_val );
	  }

	//SMOOTH_GROUP begin.
    //tag, size, group for each face.
    TDS_CHUNK c_smoothgrp;
    c_smoothgrp.chunk=0x4150;
    c_smoothgrp.size= 2 + 4 + faces_count*4;
    if (debug)
      printf ( "        SMOOTH_GROUP: Writing smoothing data for faces\n");
    tds_chunk_write(&c_smoothgrp, fileout);
    bytes_num += 6;
	for ( iface = 0; iface < face_num; iface++ )
	  if (face[iface].mesh == meshidx)
      {
		unsigned long long_int_val = face[iface].smooth;
		bytes_num += fileout->long_int_write( long_int_val );
      }
	//SMOOTH_GROUP end.

	//MSH_MAT_GROUP begin.
    unsigned int j,i;
    if (debug)
      printf ( "        MSH_MAT_GROUP: Writing mesh materials\n");
    for (j=0; j<material_num; j++)
    {
        char mater_name[17];
        strncpy(mater_name,material[j].name,16);
        mater_name[16]=0;
        TDS_CHUNK c_matgrp;
        c_matgrp.chunk=0x4130;
        tds_chunk_write_start(&c_matgrp, fileout);
        bytes_num += 6;
        bytes_num += tds_write_string ( fileout, mater_name );
        unsigned long msh_mat_group_num = 0;
        for (i=0; i<face_num; ++i)
          if ((face[i].mesh == meshidx) && (face_uv[i].material==j))
              msh_mat_group_num++;
        bytes_num += fileout->short_int_write( msh_mat_group_num );
        iface = 0;
        for (i=0; i<face_num; ++i)
          if (face[i].mesh == meshidx)
          {
            if (face_uv[i].material==j)
              bytes_num += fileout->short_int_write( (unsigned short)iface );
            iface++;
          }
        tds_chunk_write_end(&c_matgrp, fileout);
    }
	//MSH_MAT_GROUP end.

	//FACE_ARRAY end.
    tds_chunk_write_end(&c_facearray, fileout);

	//N_TRI_OBJECT end.
    tds_chunk_write_end(&c_ntriobj, fileout);

	//NAMED_OBJECT end.
    tds_chunk_write_end(&c_namedobj, fileout);
    return bytes_num;
}
/******************************************************************************/

unsigned long IVCONV::tds_write_free_vertices (FileIO *fileout,array<FaceUV> &face_uv)

/******************************************************************************/
{
    array<unsigned long> vert_idx;
    unsigned long vert_count;
    get_free_vertices(vert_idx,vert_count);
    if (vert_count<=0) return 0;
    char cloud_name[] = "vert_cloud";

	//NAMED_OBJECT begin. 
    //tag, size, name. 
    TDS_CHUNK c_namedobj;
    c_namedobj.chunk=0x4000;
    tds_chunk_write_start(&c_namedobj, fileout);
    stats.bytes_num += 6;
    if (debug)
        printf ( "    MDATA: writing NAMED_OBJECT chunk for %lu free vertices at %06X\n",
            vert_count,fileout->ftell() );
	stats.bytes_num += tds_write_string ( fileout, cloud_name );
    if (debug)
        printf ( "        NAMED_OBJECT: free vertices mesh named \"%s\"\n", cloud_name );

	//N_TRI_OBJECT begin.
    //tag, size.
    TDS_CHUNK c_ntriobj;
    c_ntriobj.chunk=0x4100;
    tds_chunk_write_start(&c_ntriobj, fileout);
    stats.bytes_num += 6;
    if (debug)
        printf ( "        NAMED_OBJECT: writing N_TRI_OBJECT chunk of %04hX at %06X\n",
            c_ntriobj.chunk, fileout->ftell() );

  {
	//POINT_ARRAY begin.  
    //tag, size, number of points, coordinates of points.
    TDS_CHUNK c_pointarray;
    c_pointarray.chunk=0x4110;
    c_pointarray.size= 2 + 4 + 2 + vert_count * 3 * 4;
    tds_chunk_write(&c_pointarray, fileout);
    stats.bytes_num += 6;
    unsigned short u_short_int_val;
    if (vert_count<65536)
    {
        u_short_int_val = ( unsigned short ) vert_count;
    } else
    {
        printf ( "        POINT_ARRAY: Warning, number of points (%lu) exceeded 16-bits and was reduced!\n",vert_count);
        u_short_int_val = 65535;
    }
	stats.bytes_num += tds_write_u_short_int ( fileout, u_short_int_val );
	unsigned long icor3;
	for ( icor3 = 0; icor3 < u_short_int_val; icor3++ )
	{
        unsigned long ver_idx_itm=vert_idx[icor3];
        unsigned int j;
        if (ver_idx_itm<vertex_num)
        {
            for ( j = 0; j < 3; ++j )
			    stats.bytes_num += fileout->float_write( vertex[ver_idx_itm].cor3[j] );
        } else
        {
            printf ( "        POINT_ARRAY: Warning, vertex index %lu out of vertex array bounds!\n",ver_idx_itm);
            for ( j = 0; j < 3; ++j )
			    stats.bytes_num += fileout->float_write( 0.0 );
        }
    }
	//POINT_ARRAY end.
    printf ( "        POINT_ARRAY: Written %hu points coordinates\n",u_short_int_val);
  }

    //TEX_VERTS begin.
    {
        TDS_CHUNK c_mshmatrx;
        c_mshmatrx.chunk=0x4140;
        c_mshmatrx.size= 6 + 2 + 2*vert_count*4;
        tds_chunk_write(&c_mshmatrx, fileout);
        stats.bytes_num += 6;
        stats.bytes_num += fileout->short_int_write( vert_count );
        unsigned long icor3;
        for ( icor3 = 0; icor3 < vert_count; icor3++ )
        {
            unsigned long vert_idx_val=vert_idx[icor3];
            if (vert_idx_val>=vertex_num)
            {
              printf ( "        TEX_VERTS: Warning, vertex index %lu out of array bounds\n",vert_idx_val);
              stats.bytes_num += fileout->float_write( 0.0 );
              stats.bytes_num += fileout->float_write( 0.0 );
              continue;
            }
            stats.bytes_num += fileout->float_write( vertex[vert_idx_val].tex_uv[0] );
            stats.bytes_num += fileout->float_write( vertex[vert_idx_val].tex_uv[1] );
        }
    }
    //TEX_VERTS end.

  {
	//MESH_MATRIX begin.  
    //tag, size, 4 by 3 matrix.
    TDS_CHUNK c_mshmatrx;
    c_mshmatrx.chunk=0x4160;
    c_mshmatrx.size= 2 + 4 + 4 * 12;
    tds_chunk_write(&c_mshmatrx, fileout);
    stats.bytes_num += 6;
    unsigned int i,j;
	for ( i = 0; i < 4; ++i )
		for ( j = 0; j < 3; ++j )
        {
			float float_val = transform_matrix[i][j];
			stats.bytes_num += fileout->float_write( float_val );
		}
	//MESH_MATRIX end.  
  }

	//FACE_ARRAY begin. 
    //tag, size, number of faces, nodes per face. 
    TDS_CHUNK c_facearray;
    c_facearray.chunk=0x4120;
    tds_chunk_write_start(&c_facearray, fileout);
    stats.bytes_num += 6;
    //Marking 0 faces in mesh
    unsigned short u_short_int_val = 0;
	stats.bytes_num += tds_write_u_short_int ( fileout, u_short_int_val );

	//FACE_ARRAY end.
    tds_chunk_write_end(&c_facearray, fileout);

	//N_TRI_OBJECT end.
    tds_chunk_write_end(&c_ntriobj, fileout);

	//NAMED_OBJECT end.
    tds_chunk_write_end(&c_namedobj, fileout);
}
/******************************************************************************/

bool IVCONV::tds_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:

  TDS_WRITE writes graphics information to a 3D Studio Max 3DS file.
  
	Author: John Burkardt
	Modified: 14 Oct 1998
*/
{
	long            long_int_val;
	short int           short_int_val;
	unsigned short int  u_short_int_val;
	float               float_val;
	
	stats.bytes_num = 0;

	//M3DMAGIC begin.
    //tag, size.
    TDS_CHUNK c_m3dmagic;
    c_m3dmagic.chunk=0x4d4d;
    tds_chunk_write_start(&c_m3dmagic, fileout);
    stats.bytes_num += 6;
    if (debug)
        printf ( "TDS_WRITE: starting with M3DMAGIC chunk of %04hX at offset %06X\n",
            c_m3dmagic.chunk, fileout->ftell() );
    {
      //M3D_VERSION begin.
      //tag, size, version.
      TDS_CHUNK c_m3dversion;
      c_m3dversion.chunk=0x0002;
      c_m3dversion.size=10;
      tds_chunk_write(&c_m3dversion, fileout);
      stats.bytes_num += 6;
      long_int_val = 3;
      if (debug)
          printf ( "TDS_WRITE: writing M3D_VERSION of %04lX\n", long_int_val );
      stats.bytes_num += fileout->long_int_write( long_int_val );
      //M3D_VERSION end
    }

	//MDATA begin.
    //tag, size.
    TDS_CHUNK c_mdata;
    c_mdata.chunk=MMAGIC;
    if (debug)
        printf ( "TDS_WRITE: writing MDATA chunk of %04hX at %06X\n",
            c_mdata.chunk, fileout->ftell() );
    stats.bytes_num += tds_chunk_write_start(&c_mdata, fileout);

  {
    //MESH_VERSION begin.
    //tag, size, version.
    TDS_CHUNK c_meshversion;
    c_meshversion.chunk=MESH_VERSION;
    c_meshversion.size=6+4;
    if (debug)
        printf ( "    MDATA: writing MESH_VERSION chunk of %04hX at %06X\n",
            c_meshversion.chunk, fileout->ftell() );
    stats.bytes_num += tds_chunk_write(&c_meshversion, fileout);
    long_int_val = 3;
    stats.bytes_num += fileout->long_int_write( long_int_val );
    //MESH_VERSION end.  
  }
  {
	//MASTER_SCALE begin.  
    //tag, size, scale.
    TDS_CHUNK c_masterscale;
    c_masterscale.chunk=0x0100;
    c_masterscale.size=6+4;
    stats.bytes_num += tds_chunk_write(&c_masterscale, fileout);
    if (debug)
        printf ( "    MDATA: writing MASTER_SCALE chunk of %04hX at %06X\n",
            c_masterscale.chunk, fileout->ftell() );
	float_val = 1.0;
	stats.bytes_num += fileout->float_write( float_val );
	//MASTER_SCALE end. 
  }
  {
	//MATERIALS DEFINITION begin.
    if (debug)
        printf ( "    MDATA: writing material chunks at %06X\n",
            fileout->ftell() );
    unsigned long j;
    for ( j = 0; j < material_num; j++ )
        tds_write_material_section (fileout,j);
	//MATERIALS DEFINITION end.
  }
  {
	//NAMED_OBJECT array begin. 
    if (debug)
        printf ( "    MDATA: writing meshes at %06X\n",
            fileout->ftell() );
    //Prepare structure for writing "per face" materials
    array<FaceUV> face_uv;
    unsigned long face_uv_num;
    clear_faces_uv(face_uv,0,face_uv_num);
    vertex_to_face_material(face_uv,0,face_uv_num);
    //vertex_to_face_uv(face_uv,0,face_uv_num); - not required in TDS
    unsigned long j;
    for ( j = 0; j < mesh_num; j++ )
        stats.bytes_num += tds_write_named_object (fileout,j,face_uv);
    // Vertices that do not make faces
    tds_write_free_vertices(fileout,face_uv);
	//NAMED_OBJECT array end.
  }

    if (debug)
        printf ( "TDS_WRITE: MDATA chunk ends at %06X, its length is set to %04lX\n",
            fileout->ftell(), fileout->ftell()-c_mdata.cur );
	//MDATA end.
    tds_chunk_write_end(&c_mdata, fileout);

	//KFDATA begin.
    TDS_CHUNK c_kfdata;
    c_kfdata.chunk=0xb000;
    if (debug)
        printf ( "TDS_WRITE: writing KFDATA chunk of %04hX at %06X\n",
            c_kfdata.chunk, fileout->ftell() );
    tds_chunk_write_start(&c_kfdata, fileout);
    stats.bytes_num += 6;

	long lb00a = 2 + 4 + 2 + 9 + 2 * 2;

	//KFHDR begin.  
    //tag, size, revision, filename, animlen.
	short_int_val = ( short ) 0xb00a;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	stats.bytes_num += fileout->long_int_write( lb00a );
	short_int_val = 5;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	stats.bytes_num += tds_write_string ( fileout, "MAXSCENE" );
	short_int_val = 100;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	//KFHDR end.  

	long lb008 = 2 + 4 + 2 * 4;
	//KFSEG begin.  
    //tag, size, start, end.
	short_int_val = ( short ) 0xb008;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	stats.bytes_num += fileout->long_int_write( lb008 );
	long_int_val = 0;
	stats.bytes_num += fileout->long_int_write( long_int_val );
	long_int_val = 100;
	stats.bytes_num += fileout->long_int_write( long_int_val );
	//KFSEG end.  

	//KFCURTIME begin.
    //tag, size, current_frame.
	short_int_val = ( short ) 0xb009;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	long lb009 = 2 + 4 + 4;
	stats.bytes_num += fileout->long_int_write( lb009 );
	long_int_val = 0;
	stats.bytes_num += fileout->long_int_write( long_int_val );
	//KFCURTIME end.

	//OBJECT_NODE_TAG begin.
    //tag, size.  
    TDS_CHUNK c_objnodetag;
    c_objnodetag.chunk=0xb002;
    if (debug)
        printf ( "    KFDATA: writing OBJECT_NODE_TAG chunk of %04hX at %06X\n",
            c_objnodetag.chunk, fileout->ftell() );
    tds_chunk_write_start(&c_objnodetag, fileout);
    stats.bytes_num += 6;

	//NODE_ID begin.
    //tag, size, id.
	long lb030 = 2 + 4 + 2;
	short_int_val = ( short ) 0xb030;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	stats.bytes_num += fileout->long_int_write( lb030 );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	//NODE_ID end.  
  {
	//NODE_HDR begin. 
    //tag, size, object_name, flag1, flag2, hierarchy.
    char mesh_name[17];
    strncpy(mesh_name,mesh[0].name,16);
    mesh_name[16]=0;
    TDS_CHUNK c_nodehdr;
    c_nodehdr.chunk=0xb010;
    tds_chunk_write_start(&c_nodehdr, fileout);
    stats.bytes_num += 6;
	stats.bytes_num += tds_write_string ( fileout, mesh_name );
	short_int_val = 16384;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = -1;
	stats.bytes_num += fileout->short_int_write( short_int_val );
    tds_chunk_write_end(&c_nodehdr, fileout);
	//NODE_HDR end. 
  }
	//PIVOT begin. 
    //tag, size, pivot_x, pivot_y, pivot_z.
	long lb013 = 2 + 4 + 6 * 2;
	short_int_val = ( short ) 0xb013;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	stats.bytes_num += fileout->long_int_write( lb013 );
	unsigned int i;
	for ( i = 0; i < 3; ++i ) {
		float_val = view.pivot[i];
		stats.bytes_num += fileout->float_write( float_val );
	}
	// PIVOT end. 

	// POS_TRACK_TAG begin.  
    // tag, size, flag, i1, i2, i3, i4, i5, i6, frame, l1, pos_x, pos_y, pos_z.
	long lb020 = 2 + 4 + 8 * 4;
	short_int_val = ( short ) 0xb020;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	stats.bytes_num += fileout->long_int_write( lb020 );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 1;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	long_int_val = 0;
	stats.bytes_num += fileout->long_int_write( long_int_val );
	for ( i = 0; i < 3; ++i ) {
		float_val = view.origin[i];
		stats.bytes_num += fileout->float_write( float_val );
	}
	/*
	POS_TRACK_TAG end. 
	ROT_TRACK_TAG begin. 
    tag, size, i1, i2, i3, i4, i5, i6, i7, i8, l1, rad, axis_x, axis_y, axis_z. 
	*/
	short_int_val = ( short ) 0xb021;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	long lb021 = 2 + 4 + 9 * 4;
	stats.bytes_num += fileout->long_int_write( lb021 );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 1;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	long_int_val = 0;
	stats.bytes_num += fileout->long_int_write( long_int_val );
	float_val = 0.0;
	stats.bytes_num += fileout->float_write( float_val );
	stats.bytes_num += fileout->float_write( float_val );
	stats.bytes_num += fileout->float_write( float_val );
	stats.bytes_num += fileout->float_write( float_val );
	//ROT_TRACK_TAG end. 

	//SCL_TRACK_TAG begin.  
    //tag, size, i1, i2, i3, i4, i5, i6, i7, i8, l1, scale_x, scale_y, scale_z.
	long lb022 = 2 + 4 + 32;
	short_int_val = ( short ) 0xb022;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	stats.bytes_num += fileout->long_int_write( lb022 );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 1;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	short_int_val = 0;
	stats.bytes_num += fileout->short_int_write( short_int_val );
	long_int_val = 0;
	stats.bytes_num += fileout->long_int_write( long_int_val );
	float_val = 1.0;
	stats.bytes_num += fileout->float_write( float_val );
	stats.bytes_num += fileout->float_write( float_val );
	stats.bytes_num += fileout->float_write( float_val );
	//SCL_TRACK_TAG end.

	//OBJECT_NODE_TAG end.
    tds_chunk_write_end(&c_objnodetag, fileout);

    if (debug)
        printf ( "TDS_WRITE: KFDATA chunk ends at %06X, its length is set to %04lX\n",
            fileout->ftell(), fileout->ftell()-c_kfdata.cur );
	//KFDATA end.
    tds_chunk_write_end(&c_kfdata, fileout);
	
    if (debug)
        printf ( "TDS_WRITE: main M3DMAGIC chunk ends at %06X, its length is set to %04lX\n",
            fileout->ftell(), fileout->ftell()-c_m3dmagic.cur );
	//M3DMAGIC end. 
    tds_chunk_write_end(&c_m3dmagic, fileout);

	// Report.
	printf ( "TDS_WRITE wrote %d bytes\n", stats.bytes_num );
	return true;
}
/******************************************************************************/

bool IVCONV::tds_read ( FileIO *filein )

/******************************************************************************/
/*
Purpose:

  TDS_READ reads a 3D Studio MAX binary 3DS file.
  
	Modified: 20 October 1998
	Author: John Burkardt
*/
{
	unsigned long   chunk_begin;
	unsigned long   chunk_end;
	unsigned long   chunk_length;
	unsigned long   chunk_length2;
	unsigned long   position;
	unsigned short int  temp_int;
	int                 version;
	int                 views_read;

	//Initialize.
	views_read = 0;
	
	temp_int = filein->short_uint_read();
	if ( debug )
		printf ( "TDS_READ: Read magic number %04X\n", temp_int );

	if ( temp_int != 0x4d4d )
    {
		printf ( "\n" );
		printf ( "TDS_READ - Fatal error!\n" );
        printf ( "    Incorrect magic tag - this is not a 3DS file\n" );
        return false;
    }

	//Move to 28 bytes from the beginning of the file. 
	position = 28;
	filein->fseek(( long ) position, SEEK_SET );
	version = filein->fgetc();
		
	if ( version < 3 )
    {
			printf ( "\n" );
			printf ( "TDS_READ - Fatal error!\n" );
			printf ( "  This routine can only read 3DS version 3 or later\n" );
			printf ( "  The input file is version %d\n" ,version );
			return false;
	}
		
	if ( debug )
			printf ( "TDS_READ: Version number is %d\n", version );

		// Move to 2 bytes from the beginning of the file. 
		// Set CURRENT_POINTER to the first byte of the chunk.
		// Set CHUNK_LENGTH to the number of bytes in the chunk.
		chunk_begin = 0;
		position = 2;
		filein->fseek(( long ) position, SEEK_SET );
		
		chunk_length = filein->long_uint_read();
		position = 6;
		
		chunk_end = chunk_begin + chunk_length;
		
		if ( debug )
			printf ( "TDS_READ: Main chunk begins at %06lu, ends at %06lu\n",
                     chunk_begin, chunk_end );
		
		while ( (position+2 < chunk_end) )
        {
			temp_int = filein->short_uint_read();
			position = position + 2;
			if ( debug )
				printf ( "TDS_READ: Identifying main section tag %04X at file position %06X\n",temp_int,position);
            if (filein->feof())
            {
				printf ( "TDS_READ: Error, unexpected input file end\n");
				break;
            }
			
			if ( temp_int == 0x0002 )
            {
				if ( debug )
					printf ( "TDS_READ: Read_Initial_Section:\n" );
				chunk_length2 = filein->long_uint_read();
				position = position + 4;
				position = position - 6 + chunk_length2;
				filein->fseek(( long ) position, SEEK_SET );
				if ( debug )
					printf ( "TDS_READ: Read_Initial_Section ends\n" );
			} else
			if ( temp_int == 0x3d3d )
            {
				if ( debug )
					printf ( "TDS_READ: Read_Edit_Section:\n" );
				position = position - 2;
				position = position + tds_read_edit_section ( filein, &views_read );
				if ( debug )
					printf ( "TDS_READ: Read_Edit_Section ends\n" );
			} else
			if ( temp_int == 0xb000 )
            {
				if ( debug )
					printf ( "TDS_READ: Read_Keyframe_Section:\n" );
				position = position - 2;
				position = position + tds_read_keyframe_section ( filein, &views_read );
				if ( debug )
					printf ( "TDS_READ: Read_Keyframe_Section ends\n" );
			} else
			{
				printf ( "\n" );
				printf ( "TDS_READ - Fatal error!\n" );
				printf ( "  Unexpected main section tag of %04hX, position = %06lu\n",temp_int,position );
				return false;
			}
		}
    //Seek to end of the main section - maybe there is some other data to read next
    position = chunk_begin + chunk_length;
	filein->fseek(( long ) position, SEEK_SET );
	return true;
}
/******************************************************************************/
