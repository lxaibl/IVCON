/******************************************************************************/
// Author:  Zik Saleeba (zik@zikzak.net)
// Change: 02 Mar 2007 Tomasz Lis adjusted to new IVCONV class, minor fixes

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - Max vertices 65535
// - Max faces 65535, max face order 4
// - No meshes support
// - Max materials/textures 65535
// - Restricted materials support - no colors, no alpha, only one texture
// - Texture UV supported (per face)
// - No camera/view support
// - No Environment or lights support
// - No shapes/lines support
// Format support note - writing:
// - Max vertices 65535
// - Max faces 65535, max face order 4
// - No meshes support
// - Max materials/textures 65535
// - Restricted materials support - no colors, no alpha, only one texture
// - Texture UV supported (per face)
// - No camera/view support
// - No Environment or lights support
// - No shapes/lines support

/*
golgotha GMOD file format:


FILE HEADER
  w32     magic number           f9 fa 63 1e
  w32     number of sections
  [ number of sections
  w32     section id
  w32     section offset
  ]

  TEXTURE NAME SECTION - section id = 0x13 (19)
    w16     number of faces
    [ number of faces
    w16     texture name length
    [ texture name length
    w8      texture name character
    ]
    ]

  MODEL QUADS SECTION - section id = 0x12 (18)
    w16     number of faces
    [ number of faces
    [ four vertices
    w16     vertex index
    float   xpos (0.0-1.0)
    float   ypos (0.0-1.0)
    ]
    float   scale
    w16     flags
    float   xnormal     (normal should be normalised)
    float   ynormal
    float   znormal
    ]

  VERTEX ARRAY SECTION - section id = 0x14 (20)
    w16     number of vertices
    w16     number of animations
    w16     length of animation name
    [ length of animation name
    w8      animation name character
    ]
    w16     number of frames in animation
    [ number of frames in animation
    [ number of vertices
    float   xpos
    float   ypos
    float   zpos
    float   xnormal
    float   ynormal
    float   znormal
    ]
    ]
*/

//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

//const int GMOD_MAX_SECTIONS=32;
const int GMOD_UNUSED_VERTEX=65535;
const int G1_SECTION_MODEL_QUADS=18;
const int G1_SECTION_MODEL_TEXTURE_NAMES=19;
const int G1_SECTION_MODEL_VERT_ANIMATION=20;

/******************************************************************************/

bool gmod_arch_check ( )

/******************************************************************************/
/*
Purpose:

  GMOD_ARCH_CHECK inquires into some features of the computer architecture.
  
	Author: Zik Saleeba (zik@zikzak.net)
	Modified: 19 May 1999
*/
{
	static unsigned char one[4];
	
	int temp = sizeof ( float );
	if ( temp != 4 )
		return false;

	*(float *)one = 1.0;
	
	if (one[0] == 0 && one[1] == 0 && one[2] == 128 && one[3] == 63) {
		// little endian IEEE floats
		return true;
	}
	
	if (one[0] == 63 && one[1] == 128 && one[2] == 0 && one[3] == 0) {
		// big endian IEEE floats
		return true;
	}
	
	return false;
}
/******************************************************************************/

float gmod_read_float ( FILE *filein )

/******************************************************************************/
/*
Purpose:

  GMOD_READ_FLOAT reads a float from a Golgotha GMOD file.
  
	Author: Zik Saleeba (zik@zikzak.net)
	Modified: 19 May 1999
*/
{
	int endian = 1;
	unsigned char *out_pos;
	int i;
	float Val;
	
	if (*(char *)&endian == 1) {
		// we're little-endian, which is native for GMOD floats
		fread(&Val, sizeof(Val), 1, filein);
	}
	else {
		// we're big-endian, flip'em 
		out_pos = (unsigned char *)&Val;
		for ( i = sizeof(Val)-1; i >= 0; i-- )
			*(out_pos+i) = fgetc(filein);
	}
	
	return Val;
}
/******************************************************************************/

unsigned short gmod_read_w16 ( FILE *filein )

/******************************************************************************/
/*
Purpose:

  GMOD_READ_W16 reads a 16 bit word from a Golgotha GMOD file.
  
	Author: Zik Saleeba (zik@zikzak.net)
	Modified: 19 May 1999
*/
{
	unsigned char Byte1 = fgetc ( filein );
	unsigned char Byte2 = fgetc ( filein );
	return Byte1 | (((unsigned short)Byte2) << 8);
}
/******************************************************************************/

unsigned long gmod_read_w32 ( FILE *filein )

/******************************************************************************/
/*
Purpose:

  GMOD_READ_W32 reads a 32 bit word from a Golgotha GMOD file.
  
	Author: Zik Saleeba (zik@zikzak.net)
	Modified: 19 May 1999
*/
{
	unsigned char Byte1 = fgetc(filein);
	unsigned char Byte2 = fgetc(filein);
	unsigned char Byte3 = fgetc(filein);
	unsigned char Byte4 = fgetc(filein);
	
	return ((unsigned long)Byte1) | 
		(((unsigned long)Byte2) << 8) | 
		(((unsigned long)Byte3) << 16) | 
		(((unsigned long)Byte4) << 24);
}

/******************************************************************************/

void gmod_write_float ( float Val, FILE *fileout )

/******************************************************************************/
/*
Purpose:

  GMOD_WRITE_FLOAT writes a float to a Golgotha GMOD file.
  
	Author: Zik Saleeba (zik@zikzak.net)
	Modified: 19 May 1999
*/
{
	int endian = 1;
	unsigned char *out_pos;
	int i;
	
	if (*(char *)&endian == 1)
    {
		// we're little-endian, which is native for GMOD floats
		fwrite ( &Val, sizeof(Val), 1, fileout );
	} else
    {
		// we're big-endian, flip 'em
		out_pos = (unsigned char *)&Val;
		for ( i = sizeof(Val)-1; i >= 0; i-- )
			fputc(*(out_pos+i), fileout);
	}
}
/******************************************************************************/

void gmod_write_w16 ( unsigned short Val, FILE *fileout )

/******************************************************************************/
/*
Purpose:

  GMOD_WRITE_W16 writes a 16 bit word to a Golgotha GMOD file.
  
	Modified: 13 September 2000
	Author: Zik Saleeba (zik@zikzak.net)
*/
{
	unsigned char OutByte[2];
	
	OutByte[0] = (unsigned char)(Val & 0xff);
	OutByte[1] = (unsigned char)(Val >> 8);
	
	fwrite ( OutByte, sizeof(unsigned char), 2, fileout );
}
/******************************************************************************/

void gmod_write_w32 ( unsigned long Val, FILE *fileout )

/******************************************************************************/
/*
Purpose:

  GMOD_WRITE writes a 32 bit word to a Golgotha GMOD file.
  
	Modified: 19 May 1999
	Author: Zik Saleeba (zik@zikzak.net)
*/
{
	unsigned char OutByte[4];
	
	OutByte[0] = (unsigned char)(Val & 0xff);
	OutByte[1] = (unsigned char)((Val >> 8) & 0xff);
	OutByte[2] = (unsigned char)((Val >> 16) & 0xff);
	OutByte[3] = (unsigned char)((Val >> 24) & 0xff);
	
	fwrite ( OutByte, sizeof(unsigned char), 4, fileout );
}

/******************************************************************************/

bool IVCONV::gmod_read ( FILE *filein )

/******************************************************************************/
/*
Purpose:

  GMOD_READ reads a golgotha GMOD file.
  
	Author: Zik Saleeba (zik@zikzak.net)
	Modified: 19 May 1999
	Modified: 04 Mar 2007 Tomasz Lis different way of reading UV mapings
*/
{
	// check if we can handle this architecture
	if (!gmod_arch_check())
    {
		printf("GMOD_READ - This architecture not supported.\n");
		return false;
	}
	// read the file header 
	unsigned char MagicNumber[4];
	// read the magic number
	fread(MagicNumber, 1, 4, filein);
    if ( debug )
      printf ( "GMOD_READ: File magic number is %02X%02X%02X%02X\n",(int)MagicNumber[0],(int)MagicNumber[1],(int)MagicNumber[2],(int)MagicNumber[3] );
	if (MagicNumber[0] != 0xf9 || MagicNumber[1] != 0xfa || 
		MagicNumber[2] != 0x63 || MagicNumber[3] != 0x1e)
    {
		printf("\n");
		printf("GMOD_READ: Fatal error!\n");
		printf("    Bad magic number on GMOD file, probably wrong file type.\n");
		return false;
	}
	unsigned long section_num = gmod_read_w32(filein);
    if ( debug )
      printf ( "GMOD_READ: The file claims to have %lu sections\n", section_num);
/*	if (NumSections >= GMOD_MAX_SECTIONS)
    {
		printf("GMOD_READ - Too many sections (%ld) in GMOD file - please increase static limit GMOD_MAX_SECTIONS\n", NumSections);
		return false;
	}*/
	
	//Read the sections.
	unsigned long section_idx;
	array<unsigned long> section_id;
	array<unsigned long> section_offs;
	for ( section_idx = 0; section_idx < section_num; section_idx++ )
    {
		section_id[section_idx] = gmod_read_w32(filein);
		section_offs[section_idx] = gmod_read_w32(filein);
	}
    if ( debug )
      printf ( "GMOD_READ: Sections list ends at %06X\n", ftell(filein));

    //Store current counts, to allow merging 3D data
	unsigned long face_num_base=face_num;
	unsigned long vertex_num_base=vertex_num;
	unsigned long material_num_base=material_num;
	unsigned int max_veridx = 0;

    //Prepare structure for reading "per face" UV mapping
    array<FaceUV> face_uv;

	//Read each successive section.
	for ( section_idx = 0; section_idx < section_num; section_idx++ )
    {
      if ( debug )
          printf ( "GMOD_READ: processing section %3lu: ID=%04lX, Offs=%06lX\n",
                  section_idx, section_id[section_idx], section_offs[section_idx]);
      //Go to the start of the section.
      fseek ( filein, section_offs[section_idx], SEEK_SET );
	  //What type of section is it?
	  switch (section_id[section_idx])
      {
	  //Model section.
	  case G1_SECTION_MODEL_QUADS:
        {
            //Get the number of faces.
			unsigned long face_num_inc = gmod_read_w16 ( filein );
            if ( debug )
                printf ( "    G1_SECTION_MODEL_QUADS contains %lu faces\n",
                        face_num_inc);

			//Get the information on each face.
			unsigned long face_count;
			for ( face_count = 0; face_count < face_num_inc; face_count++ )
            {
                face[face_num]=Face();
                face_uv[face_count]=FaceUV();
				unsigned int ivert = 0;
                int vertex_count;
				for ( vertex_count = 0; vertex_count < 4; vertex_count++ )
                {
					// read the vertex index
					unsigned long veridx = gmod_read_w16(filein);
					float coord_u = gmod_read_float(filein);
					float coord_v = gmod_read_float(filein);
					if (veridx != GMOD_UNUSED_VERTEX)
                    {
					    face[face_num].vertices[ivert] = vertex_num_base+veridx;
                        face_uv[face_count].tex_uv[ivert]=vec2(coord_u,coord_v);
						if (max_veridx < veridx)
							max_veridx = veridx;
                        ivert++;
					}
					// read the texture position
				}
				
				// scale and flags
                float Scale;
				fread(&Scale, sizeof(Scale), 1, filein);
                unsigned short Flags = gmod_read_w16(filein);
				//if ( debug )
				//	printf ( "    G1_SECTION_MODEL_QUADS face %lu flags = %u\n",
                //            face_num, (unsigned int)Flags );
				
				// normal vector
				face[face_num].normal[0] = gmod_read_float(filein);
				face[face_num].normal[1] = gmod_read_float(filein);
				face[face_num].normal[2] = gmod_read_float(filein);
				
				// the order is the number of used vertices
				face[face_num].order = ivert;
				face_uv[face_count].order = ivert;
                face_num++;
			}
			if ( debug )
				printf ( "    G1_SECTION_MODEL_QUADS ends at file offset %06X\n",
                        ftell(filein) );
         };break;
			
	  //Texture name section.
	  case G1_SECTION_MODEL_TEXTURE_NAMES:
        {
			// get the number of textures
			unsigned int texture_num_inc = gmod_read_w16(filein);
            if ( debug )
                printf ( "    G1_SECTION_MODEL_TEXTURE_NAMES contains %lu textures for faces\n",
                        texture_num_inc);
			
			unsigned long texture_count;
			for (texture_count = 0; texture_count<texture_num_inc; texture_count++)
            {
               	char file_name[LINE_MAX_LEN];  //texture file name
				// read the texture name
				unsigned short TextureNameLen = gmod_read_w16(filein);
				if (TextureNameLen>LINE_MAX_LEN)
                {
                    if ( debug )
                        printf ( "    G1_SECTION_MODEL_TEXTURE_NAMES warning: texture length %hu reduced\n",
                            TextureNameLen);
                    TextureNameLen=LINE_MAX_LEN;
                }
				fread ( file_name, sizeof(char), TextureNameLen, filein);
				file_name[TextureNameLen] = '\0';
				// Add the new material only if it does not exist already
				int material_idx = get_material_index(file_name);
				if (strlen(file_name)<1) material_idx=0;
                if (material_idx<0)
                    {
                    texture[texture_num]=Texture();
                    strncpy(texture[texture_num].name,file_name,LINE_MAX_LEN);
                    if ( debug )
                        printf ( "    G1_SECTION_MODEL_TEXTURE_NAMES found texture %s\n",
                            texture[texture_num].name);
                    //Create a material for this texture
                      material[material_num]=Material();
                      strncpy(material[material_num].name,file_name,LINE_MAX_LEN);
                      material[material_num].texture=texture_num;
                      material_idx=material_num;
                      material_num++;
                    texture_num++;
                    }
                //Set the right face to this material index
                if (texture_count < face_num-face_num_base)
                    face_uv[texture_count].material=material_num_base+material_idx;
			}
			if ( debug )
				printf ( "    G1_SECTION_MODEL_TEXTURE_NAMES ends at file offset %06X\n",
                        ftell(filein) );
		};break;
			
			
	  // vertex section
			
	  case G1_SECTION_MODEL_VERT_ANIMATION:
        {
			// get the number of vertices
			unsigned long vertex_num_inc = gmod_read_w16(filein);
            if ( debug )
                printf ( "    G1_SECTION_MODEL_VERT_ANIMATION contains %lu vertices\n",
                        vertex_num_inc);

			//Get the number of animations.
			unsigned short NumAnimations = gmod_read_w16(filein);
			if (NumAnimations > 1) {
				printf ( "GMOD_READ - Fatal error!\n" );
				printf ( "  GMOD files can only handle one animation.\n" );
				printf ( "  This file contains %d.\n", NumAnimations );
				return false;
			}
			
			// read the animation name
			unsigned short AnimationNameLen = gmod_read_w16(filein);
			char anim_name[LINE_MAX_LEN];
			if (AnimationNameLen>0)
                fread ( anim_name, sizeof(char), AnimationNameLen, filein);
			anim_name[AnimationNameLen] = '\0';
			
			// get the number of frames of animation
			unsigned short NumFrames = gmod_read_w16(filein);
			if (NumFrames > 1)
				printf("GMOD_READ - Too many frames of animation (%d) in GMOD file - will only use 1.\n", NumFrames);
			
			// go through all the vertices, reading each one
            unsigned long vertex_count;
			for (vertex_count = 0; vertex_count < vertex_num_inc; vertex_count++)
            {
                vertex[vertex_num]=Vertex();
				// read the vertex
				vertex[vertex_num].cor3[0] = gmod_read_float(filein);
				vertex[vertex_num].cor3[1] = gmod_read_float(filein);
				vertex[vertex_num].cor3[2] = gmod_read_float(filein);
				// read the normal
				vertex[vertex_num].normal[0] = gmod_read_float(filein);
				vertex[vertex_num].normal[1] = gmod_read_float(filein);
				vertex[vertex_num].normal[2] = gmod_read_float(filein);
                /*if ( debug )
                {
                    printf ( "    G1_SECTION_MODEL_VERT_ANIMATION vertex %4lu pos: %06lX",
                           vertex_count,(unsigned long)ftell(filein));
                    printf ( " coords: %f %f %f\n",vertex[vertex_num].cor3[0],vertex[vertex_num].cor3[1],vertex[vertex_num].cor3[2]);
                }*/
                vertex_num++;
			}
			if ( debug )
				printf ( "    G1_SECTION_MODEL_VERT_ANIMATION ends at file offset %06X\n",
                        ftell(filein) );
		};break;
			
		default:
			continue;
    }
  }
  
  //Convert UV mapping to "per vertex"
    face_to_vertex_material(face_uv,face_num_base,face_num-face_num_base);
    face_to_vertex_uv(face_uv,face_num_base,face_num-face_num_base);

  //Check for sanity.
  if ( max_veridx >= vertex_num ) {
	  printf ( "GMOD_READ - Maximum coordinate index (%lu)\n", max_veridx );
	  printf ( "  exceeded number of coordinates (%lu) in GMOD file.\n", vertex_num );
	  return false;
  }
  
  return true;
}
/******************************************************************************/

bool IVCONV::gmod_write ( FILE *fileout )
	
/******************************************************************************/
/*
    Purpose:
	  GMOD_WRITE writes a Golgotha GMOD file.
	  
	Author: Zik Saleeba (zik@zikzak.net)
	Modified: 19 May 1999
	Modified: 04 Mar 2007 Tomasz Lis different way of writing UV mapings
*/
{
	static unsigned char MagicNumber[4] = { 0xf9, 0xfa, 0x63, 0x1e };

	//Check if we can handle this architecture.
	if ( !gmod_arch_check() ) {
		printf("GMOD_WRITE - This architecture not supported.\n");
		return false;
	}

    //Prepare structure for writing "per face" UV mapping
    array<FaceUV> face_uv;
    unsigned long face_uv_num;
    clear_faces_uv(face_uv,0,face_uv_num);
    vertex_to_face_material(face_uv,0,face_uv_num);
    vertex_to_face_uv(face_uv,0,face_uv_num);
    	
	//Write the file header.

	//Write the magic number.
	fwrite ( MagicNumber, sizeof(char), 4, fileout );

	unsigned long SectionHeaderPos;
	unsigned long TextureNameSectionPos;
	unsigned long ModelSectionPos;
	unsigned long VertexSectionPos;
	
	//Write the number of sections.
	unsigned long NumSections = 3;
	gmod_write_w32 ( NumSections, fileout ); 

	//Write a dummy section header which we'll overwrite later.
	SectionHeaderPos = ftell ( fileout );
	unsigned long SectionCount;
	for (SectionCount = 0; SectionCount < NumSections; SectionCount++) {
		gmod_write_w32 ( 0, fileout );
		gmod_write_w32 ( 0, fileout );
	}  

	//Texture name section.

	//Take note of where we are in the file.
	TextureNameSectionPos = ftell ( fileout );
	//Write the number of textures.
	gmod_write_w16 ( ( unsigned short ) face_num, fileout );

	//Write the texture names.
	unsigned long face_idx;
	for ( face_idx = 0; face_idx < face_num; face_idx++ )
    {
		unsigned long material_idx=face_uv[face_idx].material;
		long texture_idx=material[material_idx].texture;
		char *txtr_ptr="";
		if (texture_idx>0) txtr_ptr=texture[texture_idx].name;
		gmod_write_w16 ( ( unsigned short ) strlen ( txtr_ptr ), 
			fileout );
		fwrite ( txtr_ptr, strlen ( txtr_ptr ), 1, fileout );
	}

	//Model section.

	//Take note of where we are in the file.
	ModelSectionPos = ftell(fileout);
	
	//Write the number of faces.
	gmod_write_w16 ( ( unsigned short ) face_num, fileout );
	
	//Write the information on each face.
	unsigned long FaceCount;
	for ( FaceCount = 0; FaceCount < face_num; FaceCount++ )
    {
        unsigned int order=((face[FaceCount].order < 4) ? face[FaceCount].order : 4);
        int VertexCount;
		for (VertexCount = 0; VertexCount < order; VertexCount++)
        {
            unsigned long veridx = face[FaceCount].vertices[VertexCount];
            //Write the vertex index.
			gmod_write_w16 ( (unsigned short)veridx, fileout );
			
			//Write the texture position.
//			gmod_write_float ( face_uv[FaceCount].tex_uv[0], fileout );
//			gmod_write_float ( face_uv[FaceCount].tex_uv[1], fileout );
            // the other way - involves vertices:
			gmod_write_float ( vertex[veridx].tex_uv[0], fileout );
			gmod_write_float ( vertex[veridx].tex_uv[1], fileout );
		}
		
		//Write any extra vertices which are unused.
		for ( ; VertexCount < 4; VertexCount++ )
        {
		    //Write the vertex index.
			gmod_write_w16 ( GMOD_UNUSED_VERTEX, fileout );
			//Write the texture position for non-existing vertices
			gmod_write_float ( 0.0, fileout );
			gmod_write_float ( 0.0, fileout );
		}
		
		//Scale and flags.
        float Min[3];
        float Max[3];
		
		//Find the bounding box.
        int DimensionCount;
		for ( DimensionCount = 0; DimensionCount < 3; DimensionCount++ )
        {
            int CorNumber;
			CorNumber = face[FaceCount].vertices[0];
			Min[DimensionCount] = vertex[CorNumber].cor3[DimensionCount];
			Max[DimensionCount] = vertex[CorNumber].cor3[DimensionCount];
			
			for (VertexCount = 1; VertexCount < ((face[FaceCount].order < 4) ? face[FaceCount].order : 4); VertexCount++) {
				
				CorNumber = face[FaceCount].vertices[VertexCount];
				
				if (Min[DimensionCount] > vertex[CorNumber].cor3[DimensionCount])
					Min[DimensionCount] = vertex[CorNumber].cor3[DimensionCount];
				
				if (Max[DimensionCount] < vertex[CorNumber].cor3[DimensionCount])
					Max[DimensionCount] = vertex[CorNumber].cor3[DimensionCount];
			}
		}
		
		//The scale is the "width" of the face for mipmapping - 
		//I just take the maximum bounding box dimension.
		float MaxWidth = Max[0] - Min[0];
		for ( DimensionCount = 1; DimensionCount < 3; DimensionCount++ ) {
			
			if ( MaxWidth < Max[DimensionCount] - Min[DimensionCount] )
				MaxWidth = Max[DimensionCount] - Min[DimensionCount];
		}
		
        float Scale;  
		Scale = MaxWidth;
		fwrite ( &Scale, sizeof(Scale), 1, fileout );
		
		//Flags are just nothing.
		gmod_write_w16 ( 0, fileout );
		//Normal vector.
		gmod_write_float ( face[FaceCount].normal[0], fileout );
		gmod_write_float ( face[FaceCount].normal[1], fileout );
		gmod_write_float ( face[FaceCount].normal[2], fileout );
	}
	
	//Vertex section.
	
	//Take note of where we are in the file.
	VertexSectionPos = ftell ( fileout );
	
	//Write the number of vertices.
	gmod_write_w16 ( ( unsigned short ) vertex_num, fileout );
	
	//Write the number of animations.
	gmod_write_w16 ( 1, fileout );
	
	//Write the animation name. 
	gmod_write_w16 ( 0, fileout );
	
	//Write the number of frames of animation.
	gmod_write_w16 ( 1, fileout );
	
	//Go through all the vertices, writing each one.
    unsigned long vertex_idx;
	for ( vertex_idx = 0; vertex_idx < vertex_num; vertex_idx++ )
    {
        //Write the vertex.
		gmod_write_float ( vertex[vertex_idx].cor3[0], fileout );
		gmod_write_float ( vertex[vertex_idx].cor3[1], fileout );
		gmod_write_float ( vertex[vertex_idx].cor3[2], fileout );
		//Write the normal.
		gmod_write_float ( vertex[vertex_idx].normal[0], fileout );
		gmod_write_float ( vertex[vertex_idx].normal[1], fileout );
		gmod_write_float ( vertex[vertex_idx].normal[2], fileout );
	}

	//Now rewrite the section header.
	
	//Go back to the section header.
	fseek ( fileout, ( long ) SectionHeaderPos, SEEK_SET );
	if ( debug )
		printf ( "    Writing section header at file offset %06X\n",
          ftell(fileout) );
	
	//Write the texture name section header.
	gmod_write_w32 ( G1_SECTION_MODEL_TEXTURE_NAMES, fileout );
	gmod_write_w32 ( TextureNameSectionPos, fileout );
	if ( debug )
		printf ( "    Writing G1_SECTION_MODEL_TEXTURE_NAMES offset of %06lX\n",
          TextureNameSectionPos );
	
	//Write the model section header.
	gmod_write_w32 ( G1_SECTION_MODEL_QUADS, fileout );
	gmod_write_w32 ( ModelSectionPos, fileout );
	if ( debug )
		printf ( "    Writing G1_SECTION_MODEL_QUADS offset of %06lX\n",
          ModelSectionPos );
	
	//Write the vertex section header.
	gmod_write_w32 ( G1_SECTION_MODEL_VERT_ANIMATION, fileout );
	gmod_write_w32 ( VertexSectionPos, fileout );
	if ( debug )
		printf ( "    Writing G1_SECTION_MODEL_VERT_ANIMATION offset of %06lX\n",
          VertexSectionPos );
	
	return true;
}

/******************************************************************************/
