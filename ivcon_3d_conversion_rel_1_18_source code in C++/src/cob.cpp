/******************************************************************************/
// Author:  Tomasz Lis
// Created: 01 Mar 2008
// Change:  Tomasz Lis xxx

// Purpose:
//   Methods for support of Caligari trueSpace COB files

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - No read support
// Format support note - writing:
// - TODO

//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.

/******************************************************************************/

#include "ivconv.h"

typedef struct {
    unsigned short chunk;
    unsigned long size;
    unsigned long cur;
} COB_CHUNK;

/******************************************************************************/

bool IVCONV::cob_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  COB_WRITE writes graphics information to trueSpace COB file.
  
Author: Tomasz Lis
Modified: 31 May 2008
*/
{
    // Initialize
    stats.text_num = 0;
	fileout->fprintf("Caligari V%02d.%02d%c%s             \n",0,1,'A',"LH");
	stats.text_num++;


/*
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

	// POS_TRACK_TAG end. 
	// ROT_TRACK_TAG begin. 
    // tag, size, i1, i2, i3, i4, i5, i6, i7, i8, l1, rad, axis_x, axis_y, axis_z. 
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
*/
	// Report.
	printf ( "\n" );
	printf ( "COB_WRITE: Wrote %d text lines.\n", stats.text_num );
	return true;
}
/******************************************************************************/
