/******************************************************************************/
// Author:  John Burkardt 

// Purpose:
//   To provide input selection, argument parsing and interactive interface
//   for IVCON converter.

/* 
 IVCON changelog:
	19 April 2008	    TL fixed SMF format support
	31 March 2008	    TL fixed HRC, TEC, POV and VLA format support
	18 December 2007	TL rename commands, reducing mesh name sizes and making them unique
	17 July 2007		TL rewritten INTERACT, changed commands and parameters
	12 March 2007		TL added MESH_NULL_DELETE function
	08 March 2007		TL extended support of ASE and IV format
	01 March 2007		TL total rebuild of IVCON class, all conversion function affected
	26 August 2006		Tomasz Lis adjusted for Dev-C++ and added Syndicate Wars support
	19 March 2003		PhG tweaked IV_READ to accept also VRML wrl files
	16 March 2003		PhG corrected materials in 3DS_READ and WRL_WRITE
						ASC support added for point cloud
	23 December 2002	Philippe Guglielmetti (PhG) divided in smaller files
						PhG started conversion to C++/STL (IVCONV class)
						PhG made some variables local instead of global
						PhG changes news() to this changes file : any need to be interactive ???
						PhG added IVCONV::recompute_normals()
						PhG added bool IVCONV::reverse_normals()
	04 July 2000		Added preliminary XGL_WRITE.
	26 September 1999	After ASE_READ, call NODE_TO_VERTEX_MAT and VERTEX_TO_FACE_MATERIAL.
	27 July 1999		Corrected TMAT_ROT_VECTOR.
	17 July 1999		Added null edge and face deletion.
						Corrected a string problem in SMF_READ.
	03 July 1999		Fixed a problem with BINDING variables in SMF_READ.
	02 July 1999		Added limited texture support in 3DS/iv.
	26 June 1999		BYU_READ added.
	25 June 1999		BYU_WRITE added.
	22 June 1999		TRIB_READ added.
	16 June 1999		TRIB_WRITE Greg Hood binary triangle output routine added.
	10 June 1999		TRIA_WRITE Greg Hood ASCII triangle output routine added.
	09 June 1999		TEC_WRITE TECPLOT output routine added.
						IV_READ and IV_WRITE use TRANSFORM_MATRIX now.
	26 May 1999			LINE_PRUNE option added for VLA_WRITE.
	24 May 1999			Added << command to append new graphics data to old.
						Stuck in first draft STLB_READ/STLB_WRITE routines.
						STLA_WRITE and STLB_WRITE automatically decompose non-triangular faces before writing.
	23 May 1999			Stuck in first draft WRL_WRITE routine.
	22 May 1999			Faces converted to lines before calling VLA_WRITE.	
						Added UCD_WRITE.
						Added MATERIAL/PATCH/TAGGEDPOINTS fields in HRC_READ.
	17 May 1999			Updated SMF_WRITE, SMF_READ to match code in IVREAD.
						Added transformation matrix routines.
	16 May 1999			Zik Saleeba improved DXF support to handle polygons.
	15 April 1999		Zik Saleeba added Golgotha GMOD file format support.
	03 December 1998	Set up simple hooks in TDS_READ_MATERIAL_SECTION.
	02 December 1998	Set up simple hooks for texture map names.
	19 November 1998	IV_WRITE uses PER_VERTEX normal binding.
	18 November 1998	Added node normals.
						Finally added the -RN option.
	17 November 1998	Added face node ordering reversal option.
	20 October 1998		Added DATA_REPORT.
	19 October 1998		SMF_READ and SMF_WRITE added.
	16 October 1998		Fixing a bug in IV_READ that chokes on ]} and other	cases where brackets aren't properly spaced.
	11 October 1998		Added face subset selection option S.
	09 October 1998		Reworking normal vector treatments.
						Synchronizing IVREAD and IVCON.		
						POV_WRITE added.
	02 October 1998		IVCON reproduces BOX.3DS and CONE.3DS exactly.
	30 September 1998	IVCON compiled on the PC.
						Interactive BYTE_SWAP option added for binary files.
	25 September 1998	OBJECT_NAME made available to store object name.
	23 September 1998	3DS binary files can be written.
	15 September 1998	3DS binary files can be read.
	01 September 1998	COR3_RANGE, FACE_NORMAL_AVE added.
						Major modifications to normal vectors.
	24 August 1998		HRC_READ added.
	21 August 1998		TXT_WRITE improved.
	20 August 1998		HRC_WRITE can output lines as linear splines.
	19 August 1998		Automatic normal computation for OBJ files.
						Added normal vector computation.
						HRC_WRITE is working.
	18 August 1998		IV_READ/IV_WRITE handle BASECOLOR RGB properly now.
						Improved treatment of face materials and normals.
	17 August 1998		ORDER_MAX increased to 35.
						FACE_PRINT routine added.
						INIT_DATA routine added.
	14 August 1998		IV_READ is working.
	13 August 1998		ASE_WRITE is working.
						IV_WRITE is working.
	12 August 1998		ASE_READ is working.
	10 August 1998		DXF_WRITE is working.
						DXF_READ is working.
	27 July 1998		Interactive mode is working.
						OBJ_READ is working.
	25 July 1998		OBJ_WRITE is working.
	24 July 1998		DATA_CHECK checks the input data.
						VLA_READ is working.
						VLA_WRITE is working.
	23 July 1998		STL_WRITE is working.
	22 July 1998		STL_READ is working.
						TXT_WRITE is working.
*/

//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

IVCONV* iv;	// the converter class

/******************************************************************************/

int command_line ( char **argv )

/******************************************************************************/
{
	int   iarg;
	bool  ok;
	bool reverse_faces;
	bool reverse_normals;
	char filein_name[FNAME_LEN];	// longer path to allow network...
	char fileout_name[FNAME_LEN];	// longer path to allow network...

	//Initialize local data. 
	iarg = 0;
	ok = true;
	reverse_faces = false;
	reverse_normals = false;

	// Get the -RN option, -RF option, and the input file name. 
	iarg = iarg + 1;
	strncpy ( filein_name, argv[iarg], FNAME_LEN );
	
	if ( leqi ( filein_name, "-RN" )  ) {
		reverse_normals = true;
		printf ( "\n" );
		printf ( "COMMAND_LINE: Reverse_Normals option requested.\n" );
		iarg = iarg + 1;
		strncpy ( filein_name, argv[iarg], FNAME_LEN );
	}
	
	if ( leqi ( filein_name, "-RF" )  ) {
		reverse_faces = true;
		printf ( "\n" );
		printf ( "COMMAND_LINE: Reverse_Faces option requested.\n" );
		iarg = iarg + 1;
		strncpy ( filein_name, argv[iarg], FNAME_LEN );
	}

	//Read the input. 
	ok = iv->read (filein_name);
	
	if (!ok) {
		printf ( "\n" );
		printf ( "COMMAND_LINE - Fatal error!\n" );
		printf ( "  Failure while reading input data.\n" );
		return false;
	}

	//Reverse the normal vectors if requested.
	if ( reverse_normals  ) 
	{	
		iv->reverse_normals();
		printf ( "\n" );
		printf ( "COMMAND_LINE - Note:\n" );
		printf ( "  Reversed node, face, and vertex normals.\n" );

	}

	//Reverse the faces if requested.
	if ( reverse_faces  ) {
		
		iv->face_reverse_order ( );
		
		printf ( "\n" );
		printf ( "COMMAND_LINE - Note:\n" );
		printf ( "  Reversed the face definitions.\n" );
	}

	//Write the output file. 
	iarg = iarg + 1;
	strncpy ( fileout_name, argv[iarg], FNAME_LEN );
	
	ok = iv->write (fileout_name);
	
	if (!ok) {
		printf ( "\n" );
		printf ( "COMMAND_LINE - Fatal error!\n" );
		printf ( "  Failure while writing output data.\n" );
		return false;
	}
	return true;
}
/******************************************************************************/

void hello ()

/******************************************************************************/
/*
Purpose:

  HELLO prints an explanatory header message.
  
	Author: John Burkardt
	Modified: 04 Jul 2000
    Modified: 02 Mar 2007 Tomasz Lis
*/
{
	printf ( "\n" );
	printf ( "IVCON version %s\n",_ivcon_version );
	printf ( "\n" );
#ifdef _ivcon_use_tds
	printf ( ".3ds           3D Studio Max binary mesh;\n" );
#endif
#ifdef _ivcon_use_ase
	printf ( ".ase           AutoCAD/3D Studio ASCII Scene Export;\n" );
#endif
#ifdef _ivcon_use_asc
	printf ( ".asc           Points Cloud ASCII;\n" );
#endif
#ifdef _ivcon_use_byu
	printf ( ".byu           Movie.BYU surface geometry;\n" );
#endif
#ifdef _ivcon_use_dxf
	printf ( ".dxf           Autocad DXF;\n" );
#endif
#ifdef _ivcon_use_gmod
	printf ( ".gmod          Golgotha model;\n" );
#endif
#ifdef _ivcon_use_hrc
	printf ( ".hrc           SoftImage hierarchy;\n" );
#endif
#ifdef _ivcon_use_iv
	printf ( ".iv            SGI Open Inventor;\n" );
#endif
#ifdef _ivcon_use_obj
	printf ( ".obj           WaveFront Advanced Visualizer;\n" );
#endif
#ifdef _ivcon_use_pov
	printf ( ".pov           Persistence of Vision (output only);\n" );
#endif
#ifdef _ivcon_use_smf
	printf ( ".smf           Michael Garland's format;\n" );
#endif
#if defined(_ivcon_use_stl) || defined(_ivcon_use_stlb)
	printf ( ".stl/stla/stlb StereoLithography (ASCII & binary);\n" );
#endif
#ifdef _ivcon_use_swv
	printf ( ".swv           Syndicate Wars vehicles primveh(input);\n" );
#endif
#ifdef _ivcon_use_swm
	printf ( ".swm           Syndicate Wars map file (input only);\n" );
#endif
#ifdef _ivcon_use_tec
	printf ( ".tec           TECPLOT (output only);\n" );
#endif
#if defined(_ivcon_use_tri) || defined(_ivcon_use_trib)
	printf ( ".tri/tria/trib Greg Hood's triangle format(ASCII & binary);\n" );
#endif
#ifdef _ivcon_use_txt
	printf ( ".txt           Text dump (output only);\n" );
#endif
#ifdef _ivcon_use_ucd
	printf ( ".ucd           AVS UCD file(output only);\n" );
#endif
#ifdef _ivcon_use_vla
	printf ( ".vla           Digistar 2 Planetarium projection file;\n" );
#endif
#ifdef _ivcon_use_wrl
	printf ( ".wrl           VRML Virtual Reality Modeling Language;\n" );
#endif
#ifdef _ivcon_use_xgl
	printf ( ".xgl           XML/OpenGL format(output only);\n" );
#endif
	printf ( "\n" );
	printf ( "This software is FREE and OpenSource\n" );
	printf ( "Visit http://sourceforge.net/projects/ivcon-tl/\n" );

	return;
}
/******************************************************************************/

void help ()

/******************************************************************************/
/*
Purpose:

  HELP prints a list of the interactive commands.
  
	Author: John Burkardt
	Modified: 26 May 1999
*/
{
	printf ( "\n" );
	printf ( "Commands:\n" );
	printf ( "\n" );
	printf ( "< file    Read data from input file;\n" );
	printf ( "<< file   Append data in input file to current data;\n" );
	printf ( "> file    Write output file;\n" );
	printf ( "BS        Switch the binary file byte-swapping mode;\n" );
	printf ( "CLA       Clear all stored data;\n" );
	printf ( "CN        Recompute normal vectors;\n" );
	printf ( "DBG       Switch the debugging mode;\n" );
//	printf ( "EM/EF/EV  Erase Meshes/Faces/Vertices range;\n" );
//	printf ( "EL/EC/EM  Erase Lines/Cameras/Materials range;\n" );
	printf ( "FSTM      Switch the fast mode (reduced array searching);\n" );
	printf ( "FSS       Select face subset, discard the rest;\n" );
	printf ( "FTL       Convert face information to lines;\n" );
	printf ( "H         Print this help list;\n" );
	printf ( "I         Info, print summary of 3D data in memory;\n" );
	printf ( "IF num    Print information about one face;\n" );
	printf ( "LP        Set line prunning for FTL conversion;\n" );
	printf ( "Q         Quit;\n" );
	printf ( "RFN num   Divides faces; reduces face nodes to num;\n" );
	printf ( "RNAM name Rename meshes; changes names of all meshes;\n" );
	printf ( "RNAP      Add filename-dependent prefix to all objects;\n" );
	printf ( "RVF       Reverse the face node ordering;\n" );
	printf ( "RVN       Reverse the normal vectors;\n" );
	printf ( "TA val    Change face area considered too tiny;\n" );
	printf ( "TS x y z  Transform the data - scale;\n" );
	
	return;
}
/******************************************************************************/

int command_execute(char *input)

/******************************************************************************/
/*
Purpose:

command_execute() executes one command from the user input. Used by interact().

Return:
<-1   wrong or truncated input command
-1    exit command recognized
0     command executed successfully
>0    error when executing command

Author: John Burkardt
Modified: 16 Jun 2007 Tomasz Lis
*/
{
	char  *next;
    //Advance to the first nonspace character in INPUT. 
	for (next=input; (*next!='\0')&&(isspace(*next)); next++)
    {  }
	//Skip blank lines and comments. 
	if (*next == '\0')
      return -2;
    //Get an uppercase mnemonic of the command
    const int MAX_CMD_MNEMONIC_SIZE=6;
    char comand[MAX_CMD_MNEMONIC_SIZE];
    {
      int idx;
      for (idx=0;idx<MAX_CMD_MNEMONIC_SIZE-1;idx++)
        comand[idx]=toupper(next[idx]);
      comand[MAX_CMD_MNEMONIC_SIZE-1]=0;
    }
    //Checking what the command is:

    //Command: << FILENAME 
    //Append new data to current graphics information.
		if ( strncmp(comand,"<<",2)==0 )
        {
			next += 2;
            char filein_name[FNAME_LEN];
            filein_name[0]='\0';
			sscanf( next, "%s", filein_name );
			if (strlen(filein_name)<1)
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Cannot find filename in input data.\n" );
                return -2;
            }
			if (debug)
				printf( "INTERACT: Appending file \"%s\" to current data.\n",filein_name );
			int ok = iv->append(filein_name);
			if (!ok)
            {
				printf( "\n" );
				printf( "INTERACT - Fatal error!\n" );
				printf( "  DATA_READ failed to read input data.\n" );
                return 1;
			}
            return 0;
		}

		//Command: < FILENAME 
		if ( strncmp(comand,"<",1)==0 )
        {
			next += 1;
            char filein_name[FNAME_LEN];
            filein_name[0]='\0';
			sscanf( next, "%s", filein_name );
			if (strlen(filein_name)<1)
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Cannot find filename in input data.\n" );
                return -2;
            }
			if (debug)
				printf( "INTERACT: Clearing and reading \"%s\".\n",filein_name );
			int ok = iv->read(filein_name);
			if (!ok)
            {
				printf ( "\n" );
				printf ( "INTERACT - Fatal error!\n" );
				printf ( "  DATA_READ failed to read input data.\n" );
                return 1;
			}
            return 0;
		}

		//Command: > FILENAME 
		if ( strncmp(comand,">",1)==0 )
        {
			next += 1;
            char fileout_name[FNAME_LEN];
            fileout_name[0]='\0';
			sscanf( next, "%s", fileout_name );
			if (strlen(fileout_name)<1)
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Cannot find filename in input data.\n" );
                return -2;
            }
			if (debug)
				printf( "INTERACT: Writing \"%s\".\n",fileout_name );
			int ok = iv->write(fileout_name);
			if (!ok)
            {
				printf ( "\n" );
				printf ( "INTERACT - Fatal error!\n" );
				printf ( "  OUTPUT_DATA failed to write output data.\n" );
                return 1;
			}
            return 0;
		}

		//BS: Switch byte swapping option.
		if ( strncmp(comand,"BS",2)==0 )
        {
			next += 2;
			if ( byte_swap )
				byte_swap = false;
			else
				byte_swap = true;
            printf ( "Byte_swapping reset to %s.\n",byte_swap?"TRUE":"FALSE" );
            return 0;
		}

		//CLEAR: Clear all 3D data.
		if ( strncmp(comand,"CLA",3)==0 )
        {
			next += 3;
			iv->data_init();
			printf ( "\n" );
			printf ( "INTERACT - Note:\n" );
			printf ( "  All data structures cleared.\n" );
            return 0;
		}

		//CN: Recompute normal vectors.
		if ( strncmp(comand,"CN",2)==0 )
        {
			next += 2;
			iv->recompute_normals();
			printf ( "\n" );
			printf ( "INTERACT - Note:\n" );
			printf ( "  Normal vectors has been recomputed.\n" );
            return 0;
		}

		//DBG: Switch debug option.
		if ( strncmp(comand,"DBG",3)==0 )
        {
			next += 3;
			if (debug)
				debug = 0;
			else
				debug = 1;
            printf ( "Debug reset to %s.\n",debug?"TRUE":"FALSE" );
            return 0;
		}

		//FSTM: Switch 'fast mode'.
		if ( strncmp(comand,"FSTM",4)==0 )
        {
			next += 3;
			if (iv->opt.fast_mode)
				iv->opt.fast_mode = false;
			else
				iv->opt.fast_mode = true;
            printf ( "Fast mode reset to %s.\n",iv->opt.fast_mode?"TRUE":"FALSE" );
            return 0;
		}

		//FSS: Select a few faces, discard the rest.
		if ( strncmp(comand,"FSS",3)==0 )
        {
			next += 3;
            //Get the first and last faces to save, IFACE1 and IFACE2.
            int iface1=-1;
            int iface2=-1;
			sscanf( next, "%d %d", &iface1,&iface2 );
			if ( (iface1<0) || (iface2<0) )
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Cannot find nonnegative face bounds in input data.\n" );
                return -2;
            }
            int faces_count=iv->get_faces_count();

            if ( (iface1>faces_count-1) || (iface2>faces_count-1) )
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Face bound indices exceeds number of faces.\n" );
                return -2;
            }
            if ( iface2<iface1 )
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Ending bound must be not less than starting one.\n" );
                return -2;
            }
			printf ( "INTERACT: Saving faces between %d and %d.\n",iface1,iface2 );
			iv->face_subset(iface1,iface2);
            return 0;
		}

		//LINES: 
		//Convert face information to lines.
		if ( strncmp(comand,"FTL",3)==0 )
        {
			next += 3;
			if ( iv->get_faces_count() <= 0 )
            {
				printf ( "\n" );
				printf ( "INTERACT - Note:\n" );
				printf ( "  There were no faces to convert.\n" );
                return 0;
			}
			printf ( "\n" );
			printf ( "INTERACT - Note:\n" );
			printf ( "  Face information will be converted\n" );
			printf ( "  to line information.\n" );
			iv->face_to_line(iv->opt.line_prune);
            return 0;
		}

		//H: Help
		if ( strncmp(comand,"H",1)==0 )
        {
			next += 1;
			help();
            return 0;
		}

		//IF: Check a face. 
		if ( strncmp(comand,"IF",2)==0 )
        {
			next += 2;
            int iface=-1;
			sscanf( next, "%d", &iface );
			if (iface<0)
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Cannot find nonnegative face number in input data.\n" );
                return -2;
            }
            int faces_count=iv->get_faces_count();
			printf ( "\n" );
			printf ( "  Valid face indices are between 0 and %d.\n", iv->get_faces_count()-1 );
			if (iface>=faces_count)
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  The face index typed is too large.\n" );
                return -2;
            }
			iv->face_print ( iface );
            return 0;
		}

		//I: Print structure information. 
		if ( strncmp(comand,"I",1)==0 )
        {
			next += 1;
			iv->data_report(true);
            return 0;
		}

		//LP: Line pruning optiont
		if ( strncmp(comand,"LP",2)==0 )
        {
			next += 2;
            int new_line_prune=-1;
			sscanf( next, "%d", &new_line_prune );
			if (new_line_prune<0)
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Cannot find nonnegative pruning option in input data.\n" );
                return -2;
            }
			printf ( "\n" );
			printf ( "INTERACT - SET LINE PRUNING OPTION.\n" );
			printf ( "\n" );
			printf ( "  LINE_PRUNE = 0 means no line pruning,\n" );
			printf ( "    nonzero means line pruning.\n" );
			printf ( "  Previous value was LINE_PRUNE = %d\n", iv->opt.line_prune );
			if (new_line_prune>=11)
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  The line prune parameter is too large.\n" );
                return -2;
            }
            printf ( "  New      value is  LINE_PRUNE = %d\n", new_line_prune );
			iv->opt.line_prune=new_line_prune;
			printf ( "  New value accepted.\n" );
            return 0;
		}

		//Q: Quit
		if ( strncmp(comand,"Q",1)==0 )
        {
			next += 1;
			printf ( "\n" );
			printf ( "INTERACT - Normal end of execution.\n" );
            return -1;
		}

		//RFN: Reduce Face Nodes
		if ( strncmp(comand,"RFN",3)==0 )
        {
			next += 3;
            int max_nodes=-1;
			sscanf( next, "%d", &max_nodes );
			if (max_nodes<3)
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Cannot find large enough nodes amount in input data.\n" );
                return -2;
            }
            iv->reduce_face_order(max_nodes);
            return 0;
		}

    //Command: RNAM name
    //Renames all meshes to given name; * means old name.
		if ( strncmp(comand,"RNAM",4)==0 )
        {
			next += 4;
            char name_pattern[FNAME_LEN];
            name_pattern[0]='\0';
			sscanf( next, "%s", name_pattern );
			if (strlen(name_pattern)<1)
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Cannot find new name pattern in input data.\n" );
                return -2;
            }
			if (debug)
				printf( "INTERACT: Renaming meshes into \"%s\".\n",name_pattern );
			int ok = iv->rename_meshes(name_pattern);
			if (!ok)
            {
				printf( "\n" );
				printf( "INTERACT - Fatal error!\n" );
				printf( "  RENAME_MESHES failed to fully perform the operation.\n" );
                return 1;
			}
            return 0;
		}

    //Command: RNAM name
    //Renames all meshes to given name; * means old name.
		if ( strncmp(comand,"RNAP",4)==0 )
        {
			next += 4;
            char name_pattern[FNAME_LEN];
            sprintf(name_pattern,"%s*",iv->default_name_prefix);
			if (debug)
				printf( "INTERACT: Renaming meshes into \"%s\".\n",name_pattern );
			int ok = iv->rename_meshes(name_pattern);
			if (!ok)
            {
				printf( "\n" );
				printf( "INTERACT - Fatal error!\n" );
				printf( "  RENAME_MESHES failed to fully perform the operation.\n" );
                return 1;
			}
            return 0;
		}

		//RVF: Reverse the face node ordering. 
		if ( strncmp(comand,"RVF",3)==0 )
        {
			next += 3;
			iv->reverse_faces();
			printf ( "\n" );
			printf ( "INTERACT - Note:\n" );
			printf ( "  Reversed face node ordering.\n" );
            return 0;
		}

		//RVN: Reverse normal vectors. 
		if ( strncmp(comand,"RVN",3)==0 )
        {
			next += 3;
			iv->reverse_normals();
			printf ( "\n" );
			printf ( "INTERACT - Note:\n" );
			printf ( "  Reversed node, face and vertex normals.\n" );
            return 0;
		}

	     //TA: Change face area considered too tiny
		if ( strncmp(comand,"TA",2)==0 )
        {
			next += 2;
            float new_tiny_area=-1.f;
			sscanf( next, "%f", &new_tiny_area );
			if (new_tiny_area<0.f)
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Cannot find nonnegative floating point number in input data.\n" );
                return -2;
            }
			printf ( "\n" );
			printf ( "INTERACT - MINIMAL FACE AREA.\n" );
			printf ( "\n" );
			printf ( "  Faces smaller than (LARGEST_FACE_AREA x TINY_FACE_AREA).\n" );
			printf ( "    will be automatically erased at loading.\n" );
			printf ( "  The larger value you type, the more faces will be deleted.\n" );
			printf ( "  Previous value was TINY_FACE_AREA = %f\n",iv->opt.tiny_face_area );
			if (new_tiny_area>1.1f)
            {
				printf ( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf ( "  The new value cannot be greater than 1.0.\n" );
                return -2;
			}
			printf ( "  New      value is  TINY_FACE_AREA = %f\n",new_tiny_area );
			iv->opt.tiny_face_area=new_tiny_area;
			printf ( "  New value accepted.\n" );
            return 0;
		}

		//TS: Transform the data, scale.
		if ( strncmp(comand,"TS",2)==0 )
        {
			next += 2;
            float  x=NAN,y=NAN,z=NAN;
			sscanf( next, "%f %f %f",&x,&y,&z );
			if (isnan(x)||isnan(y)||isnan(z))
			{
				printf( "\n" );
				printf( "INTERACT - Command execution skipped!\n" );
				printf( "  Cannot find 3 proper floating point factors in input data.\n" );
                return -2;
            }
			printf ( "\n" );
			printf ( "Nodes transformation - scale.\n" );
			printf ( "Using scale factors X=%f, Y=%f, Z=%f.\n",x,y,z);
			iv->scale(x,y,z);
            return 0;
		}

		//Unknown command - warning message
		{
			printf ( "\n" );
			printf ( "INTERACT: Warning!\n" );
			printf ( "  Command was not recognized and therefore ignored.\n" );
            return -2;
		}
}
/******************************************************************************/

int interact()

/******************************************************************************/
/*
Purpose:

INTERACT carries on an interactive session with the user.

Author: John Burkardt
Modified: 22 May 1999
*/
{
	//Say hello. 
	hello ();

	//Get the next user command. 
	printf ( "\n" );
	printf ( "Enter command (H for help)\n" );
	
	char   input[LINE_MAX_LEN];
	while ( fgets( input, LINE_MAX_LEN, stdin ) != NULL )
    {
        input[LINE_MAX_LEN-1]='\0';
        int result=command_execute(input);
        if (result<-1) continue;
        if (result==-1) break;
		if (result>=0)
		{
            printf ( "\n" );
            printf ( "Enter command (H for help)\n" );
        }
		
  }
  return true;
}
/******************************************************************************/

int main ( int argc, char **argv )

/******************************************************************************/
{
	printf("initializing...");
	iv = new IVCONV();
	int result;
	//	If there are at least two command line arguments, call COMMAND_LINE.
	//	Otherwise call INTERACT and get information from the user.
	if ( argc >= 2 )
		result = command_line ( argv );
	else
		result = interact (  );
	delete iv;
	return result;
}
/******************************************************************************/
