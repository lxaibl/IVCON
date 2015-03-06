/******************************************************************************/
// ivcon.cpp  22 Dec 2002
// ivcon.c  24 May 2001
// http://www.math.iastate.edu/burkardt/g_src/ivcon/ivcon.html

//Purpose:
//    IVCON converts various 3D graphics files.

//Acknowledgements:
//    Coding, comments, and advice were supplied by a number of collaborators.
//    John F Flanagan made some corrections to the 3D Studio Max routines.
//    Zik Saleeba (zik@zikzak.net) enhanced the DXF routines, and added the
//    Golgotha GMOD routines.
//    Thanks to Susan M. Fisher, University of North Carolina,
//    Department of Computer Science, for pointing out a coding error
//    in FACE_NULL_DELETE that was overwriting all the data!

//    Author:   John Burkardt
//    Modified: 04 July 2000
//    Modified: 23 Dec 2002 Ph. Guglielmetti www.DynaBits.com moved to C++
//    Modified: 02 Mar 2007 Tomasz Lis further changes to use c++ language features

/* TODO:
 - A method to reduce all arrays given size: reduce_item_numbers(unsigned long max_num)
   should be rewritten to divide faces to obtain less than max_num vertices
   in every face.
 - File format libraries should not read from FILE directly, but ALL use
   the FileIO structure for reading.
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

#undef ERROR	// defined by windows
const bool ERROR = false;
const bool SUCCESS = true;
bool debug;
bool byte_swap;

/******************************************************************************/

IVCONV::IVCONV()
{
	init_program_data();
}
/******************************************************************************/

bool IVCONV::read(const string& file)
{
	data_init();
	strncpy(filein_name, file.c_str(), FNAME_LEN);
	make_default_name_prefix();
	return data_read();
}
/******************************************************************************/

bool IVCONV::append(const string& file)
{
	strncpy(filein_name, file.c_str(), FNAME_LEN);
	return data_read();
}
/******************************************************************************/

bool IVCONV::write(const string& file)
{
	strncpy(fileout_name, file.c_str(), FNAME_LEN);
	return data_write();
}
/******************************************************************************/

/**
 Purpose:

 CERTEX_COR3_RANGE computes the coordinate minima and maxima.

 Author:   John Burkardt
 Modified: 31 Aug 1998
 Modified: 02 Mar 2007 Tomasz Lis
 */
void IVCONV::vertex_cor3_range()
{
	float xave;
	float xmax;
	float xmin;
	xave = vertex[0].cor3[0];
	xmax = vertex[0].cor3[0];
	xmin = vertex[0].cor3[0];

	float yave;
	float ymax;
	float ymin;
	yave = vertex[0].cor3[1];
	ymax = vertex[0].cor3[1];
	ymin = vertex[0].cor3[1];

	float zave;
	float zmax;
	float zmin;
	zave = vertex[0].cor3[2];
	zmax = vertex[0].cor3[2];
	zmin = vertex[0].cor3[2];

	printf("\n");
	printf("VERTEX_COR3_RANGE - Data range:\n");
	printf("\n");

	for (unsigned long i = 1; i < vertex_num; i++)
	{
		xave += vertex[i].cor3[0];
		if (vertex[i].cor3[0] < xmin)
			xmin = vertex[i].cor3[0];
		if (vertex[i].cor3[0] > xmax)
			xmax = vertex[i].cor3[0];

		yave += vertex[i].cor3[1];
		if (vertex[i].cor3[1] < ymin)
			ymin = vertex[i].cor3[1];
		if (vertex[i].cor3[1] > ymax)
			ymax = vertex[i].cor3[1];

		zave += vertex[i].cor3[2];
		if (vertex[i].cor3[2] < zmin)
			zmin = vertex[i].cor3[2];
		if (vertex[i].cor3[2] > zmax)
			zmax = vertex[i].cor3[2];
	}

	xave = xave / vertex_num;
	yave = yave / vertex_num;
	zave = zave / vertex_num;

	printf("   Minimum     Average     Maximum    Range\n");
	printf("\n");
	printf("X % 10f % 10f % 10f % 10f\n", xmin, xave, xmax, xmax - xmin);
	printf("Y % 10f % 10f % 10f % 10f\n", ymin, yave, ymax, ymax - ymin);
	printf("Z % 10f % 10f % 10f % 10f\n", zmin, zave, zmax, zmax - zmin);

}
/******************************************************************************/

/*
 Purpose:

 DATA_CHECK checks the input data.

 Author: John Burkardt
 Modified: 18 May 1999
 Modified: 02 Mar 2007 Tomasz Lis
 */
void IVCONV::data_check()
{
	unsigned int iface;
	unsigned int ivert;
	unsigned int nfix;

	printf("\n");
	printf("DATA_CHECK:\n");

	//Checking memory allocation
	nfix = 0;
	if (mesh.size() < mesh_num)
	{
		mesh_num = mesh.size();
		nfix++;
	}
	if (face.size() < face_num)
	{
		face_num = face.size();
		nfix++;
	}
	if (vertex.size() < vertex_num)
	{
		vertex_num = vertex.size();
		nfix++;
	}
	if (shape.size() < shape_num)
	{
		shape_num = shape.size();
		nfix++;
	}
	if (camera.size() < camera_num)
	{
		camera_num = camera.size();
		nfix++;
	}
	if (light.size() < light_num)
	{
		light_num = light.size();
		nfix++;
	}
	if (material.size() < material_num)
	{
		material_num = material.size();
		nfix++;
	}
	if (texture.size() < texture_num)
	{
		texture_num = texture.size();
		nfix++;
	}
	if (nfix > 0)
	{
		printf("  DATA_CHECK - Warning!\n");
		printf("    Found %u structures with memory allocation errors!\n",
				nfix);
		printf("    Structures truncated.\n");
	}

	//Checking mesh
	nfix = 0;
	if (mesh_num < 1)
	{
		mesh_num = 1;
		mesh[0] = Mesh();
		sprintf(mesh[0].name, "%s%03u", DEFAULT_MESH_NAME, 0);
	}

	for (iface = 0; iface < face_num; iface++)
		if (face[iface].mesh > mesh_num)
		{
			face[iface].mesh = mesh_num - 1;
			nfix++;
		}
	if (nfix > 0)
	{
		printf("  DATA_CHECK - Fixed %u faces to belong to valid meshes.\n",
				nfix);
	}

	nfix = 0;
	for (iface = 0; iface < face_num; iface++)
		if (face[iface].order > ORDER_MAX)
		{
			face[iface].order = ORDER_MAX;
			nfix++;
		}
	if (nfix > 0)
	{
		printf("  DATA_CHECK - Warning!\n");
		printf("    Truncated %u faces using more than %lu vertices per face.\n",
				nfix, ORDER_MAX);
	}

	if (vertex_num > VERTICES_MAX)
	{
		printf("  DATA_CHECK - Warning!\n");
		printf("    Total number of vertices was %lu, which exceeds internal limit of %lu vertices.\n",
				vertex_num, VERTICES_MAX);
		vertex_num = VERTICES_MAX;
		printf("    Vertices array truncated. This may cause errors in faces.\n");
	}

	if (face_num > FACES_MAX)
	{
		printf("  DATA_CHECK - Warning!\n");
		printf("    Total number of faces was %lu, which exceeds internal limit of %lu faces.\n",
				face_num, FACES_MAX);
		face_num = FACES_MAX;
		printf("    Faces array truncated. Some of faces lost.\n");
	}

	nfix = 0;
	for (iface = 0; iface < face_num; iface++)
		for (ivert = 0; ivert < face[iface].order; ivert++)
		{
			unsigned long vertidx = face[iface].vertices[ivert];
			//If the vertex does not exist, let's create it
			if (vertidx >= vertex_num)
			{
				vertex[vertex_num] = Vertex();
				face[iface].vertices[ivert] = vertex_num;
				vertex_num++;
				nfix++;
			}
		}
	if (nfix > 0)
	{
		printf("  DATA_CHECK - Warning!\n");
		printf("    Corrected %u faces using non-existing vertices.\n", nfix);
	}

	nfix = 0;
	for (int i = 0; i < material_num; ++i)
		if (strcmp(material[i].name, "") == 0)
		{
			sprintf(material[i].name, "%s%03u", DEFAULT_MATERIAL_NAME, i);
			nfix++;
		}
	for (int i = 0; i < texture_num; ++i)
		if (strcmp(texture[i].name, "") == 0)
		{
			sprintf(texture[i].name, "%s%03u", DEFAULT_TEXTURE_NAME, i);
			nfix++;
		}
	if (nfix > 0)
	{
		printf("  DATA_CHECK - Warning!\n");
		printf("    Corrected %u empty material/texture names.\n", nfix);
	}

	printf("  Data checked.\n");

	return;
}

/*
 Purpose:

 find_identical_vertex Searches for vertex identical to vert,
 returnes its index in vert_idx if found. May be slow if called
 many times.

 Created: 04 Mar 2007 Tomasz Lis
 */
bool IVCONV::find_identical_vertex(unsigned long &vert_idx, Vertex *vert)
{
	unsigned long i = 0;
	array<Vertex>::const_iterator p;
	array<Vertex>::const_iterator end = vertex.begin() + vertex_num;
	if (end > vertex.end())
		end = vertex.end();
	for (p = vertex.begin(); p < end; p++)
	{
		if ((*p) == (*vert))
		{
			vert_idx = i;
			return true;
		}
		i++;
	}
	return false;
}

/*
 Purpose:

 GET_MESH_VERTICES analyzes mesh and return its number of faces, number of vertices
 and the vertex indexes arrays. The vert_idx array has size of vert_count and
 contains indexes of vertices in main ivcon VERTEX array. The nvert_idx array
 has size of VERTEX_NUM and contains indexes of vertices in vert_idx array,
 or -1 if a vertex is not inside vert_idx.

 Created: 04 Mar 2007 Tomasz Lis
 */
bool IVCONV::get_mesh_vertices(array<unsigned long> &vert_idx,
		unsigned long &vert_count, array<long> &nvert_idx,
		unsigned long &faces_count, unsigned long mesh_idx)
{
	vert_count = 0;
	faces_count = 0;
	if (mesh_idx >= mesh_num)
	{
		printf("    GET_MESH_VERTICES: Warning, requested mesh %lu is out of range!\n",
				mesh_idx);
		return false;
	}
	if (debug)
		printf("    GET_MESH_VERTICES: Preparing list of vertices in mesh %lu\n",
				mesh_idx);
	{ //counting vertices for memory preallocation
	  // allocates too much memory, but who cares!
		unsigned long face_idx;
		for (face_idx = 0; face_idx < face_num; face_idx++)
			if (face[face_idx].mesh == mesh_idx)
				vert_count += face[face_idx].order;
		if (debug)
			printf("      GET_MESH_VERTICES: Preallocating memory for %lu elements\n",
					vert_count);
		vert_idx.reserve(vert_count);
		nvert_idx.reserve(vertex_num);
		unsigned long ivert;
		for (ivert = 0; ivert < vertex_num; ivert++)
			nvert_idx[ivert] = -1;
	}
	// now really sweeping through faces and creating vertices list
	vert_count = 0;
	unsigned long face_idx;
	for (face_idx = 0; face_idx < face_num; face_idx++)
		if (face[face_idx].mesh == mesh_idx)
		{
			faces_count++;
			unsigned int ivert;
			for (ivert = 0; ivert < face[face_idx].order; ivert++)
			{
				unsigned long new_idx = face[face_idx].vertices[ivert];
				//check if not already added
				if (nvert_idx[new_idx] < 0)
				{
					vert_idx[vert_count] = new_idx;
					nvert_idx[new_idx] = vert_count;
					vert_count++;
				}
			}
		}
	if (debug)
		printf("    GET_MESH_VERTICES: Created list of %lu vertices\n",
				vert_count);
	return true;
}

/*
 Purpose:
 get_free_vertices analyzes all faces and returnes array of unassigned vertices

 Created: 04 Mar 2007 Tomasz Lis
 */
bool IVCONV::get_free_vertices(array<unsigned long> &vert_idx,
		unsigned long &vert_count)
{
	array<char> vert_in_use;
	{
		unsigned long vertidx;
		for (vertidx = 0; vertidx < vertex_num; vertidx++)
			vert_in_use[vertidx] = false;
	}
	unsigned long face_idx;
	unsigned int ivert;
	for (face_idx = 0; face_idx < face_num; face_idx++)
		for (ivert = 0; ivert < face[face_idx].order; ivert++)
		{
			unsigned long vertidx = face[face_idx].vertices[ivert];
			if (vertidx < vertex_num)
				vert_in_use[vertidx] = true;
		}
	{
		vert_count = 0;
		unsigned long vertidx;
		for (vertidx = 0; vertidx < vertex_num; vertidx++)
			if (!vert_in_use[vertidx])
			{
				vert_idx[vert_count] = vertidx;
				vert_count++;
			}
	}
	return SUCCESS;
}

/*
 Purpose:
 make_default_name_prefix uses filein_name to set a value of default_name_prefix,
 which is used when creating default text names of meshes,textures
 and similar objects.

 Created: 18 Dec 2007 Tomasz Lis
 */
void IVCONV::make_default_name_prefix()
{
	char *pos_s = strrchr(filein_name, '\\');
	char *pos_e = strrchr(filein_name, '.');
	if (pos_s == NULL)
		pos_s = filein_name;
	if ((pos_e == NULL) || (pos_e - pos_s <= 0))
		pos_e = filein_name + strlen(filein_name);
	if (pos_e - pos_s <= 0)
		pos_s = filein_name;
	int npre_len = pos_e - pos_s;
	int idx;
	if (npre_len <= 0)
	{
		sprintf(default_name_prefix, "%02d", rand() % 100);
		return;
	}
	if (npre_len <= 4)
	{
		for (idx = 0; idx < npre_len; idx++)
			default_name_prefix[idx] = tolower(pos_s[idx]);
		default_name_prefix[npre_len] = 0;
		return;
	}
	default_name_prefix[0] = tolower(pos_s[0]);
	pos_e -= 3;
	for (idx = 0; idx < 3; idx++)
		default_name_prefix[idx + 1] = tolower(pos_e[idx]);
	default_name_prefix[4] = 0;
}

/*
 Purpose:
 force_unique_names makes sure that all objects have unique text names,
 and the names are no longer than maxlen characters.

 Created: 18 Dec 2007 Tomasz Lis
 */
void IVCONV::force_unique_names(int maxlen)
{
	//"default name" - how much space should there be for text name
	// and for item number
	int def_name_numlen = 4;
	if (maxlen < 4)
		def_name_numlen = maxlen;
	int def_name_textlen = maxlen - def_name_numlen;
	array<NameItm> namelist;
	unsigned long namelist_len = 0;
	{
		for (int imesh = 0; imesh < mesh_num; imesh++)
		{
			char *name = mesh[imesh].name;
			//If name too long - truncate
			if (strlen(name) > maxlen)
				name[maxlen] = '\0';

			if (nameitem_arr_pos(namelist, name) >= 0)
			{
				//So the name is a duplicate - that's bad...
				// Try adding number at end, and repeat this until we have unique name
				int num = 0;
				do
				{
					num++;
					sprintf(name + def_name_textlen, "%0*d", def_name_numlen,
							num);
				} while (nameitem_arr_pos(namelist, name) >= 0);
				if (debug)
					printf("      FORCE_UNIQUE_NAMES: Mesh %d renamed to \"%s\"\n",
							imesh, name);
			}
			//If we have unique name - add to list and proceed to next one
			namelist[namelist_len] = NameItm();
			strcpy(namelist[namelist_len].name, name);
			namelist_len++;
		}
	}
}

/*
 Purpose:
 CLEAR_FACES_UV Creates empty (or clears) UV coordinates for faces.
 It also copies "order" values from face to face_uv.
 Returns length of face_uv in coords_num.

 Created: 04 Mar 2007 Tomasz Lis
 */
bool IVCONV::clear_faces_uv(array<FaceUV> &face_uv, unsigned long start_face,
		unsigned long &coords_num)
{
	unsigned long face_idx;
	if (face_num > start_face)
		coords_num = face_num - start_face;
	else
		coords_num = 0;
	if (debug)
		printf("    CLEAR_FACES_UV: preparing UV structure for %lu faces starting at %lu\n",
				coords_num, start_face);
	//Pre-reserving array
	face_uv.reserve(coords_num);
	coords_num = 0;
	for (face_idx = start_face; face_idx < face_num; face_idx++)
	{
		unsigned int ivert;
		face_uv[coords_num] = FaceUV();
		face_uv[coords_num].order = face[face_idx].order;
		for (ivert = 0; ivert < face[face_idx].order; ivert++)
			face_uv[coords_num].tex_uv[ivert] = vec2(0.0, 0.0);
		coords_num++;
	}
	return SUCCESS;
}

/*
 Purpose:
 clear_vertices_uv Clears UV coordinates for all vertices

 Created: 04 Mar 2007 Tomasz Lis
 */
bool IVCONV::clear_vertices_uv()
{
	unsigned long vertidx;
	for (vertidx = 0; vertidx < vertex_num; vertidx++)
		vertex[vertidx].tex_uv = vec2(0.0, 0.0);
	return SUCCESS;
}

/*
Purpose:
  FACE_TO_VERTEX_UV Converts UV mapping coords AND material indexes
   from faces to vertices, multiplying vertices if needed

	Created: 04 Mar 2007 Tomasz Lis
	Modified: 05 Mar 2007 Tomasz Lis vertex copying when UV differs
*/
bool IVCONV::face_to_vertex_uv(array<FaceUV> &face_uv, unsigned long start_face,
		unsigned long coords_num)
{
	if (debug)
		printf("    FACE_TO_VERTEX_UV: converting UV for faces %lu to %lu\n",
				start_face, start_face + coords_num - 1);
	unsigned long vertex_num_base = vertex_num;
	//array for marking if a vertice has UV mapping set
	array<char> vert_has_uv;
	{
		unsigned long vertidx;
		for (vertidx = 0; vertidx < vertex_num; vertidx++)
			vert_has_uv[vertidx] = false;
	}
	// A message to show because find_identical_vertex() is slow
	if ((coords_num > 4096) && (!opt.fast_mode))
		printf("    FACE_TO_VERTEX_UV: Reducing UV may take some time, wait...\n");
	for (unsigned long iface = 0; iface < coords_num; iface++)
	{
		unsigned int order = face_uv[iface].order;
		if (start_face + iface < face_num)
		{
			if ((debug) && (order != face[start_face + iface].order))
				printf("    FACE_TO_VERTEX_UV: Warning, face_uv order (%u) differs from face order (%u)!\n",
						order, face[start_face + iface].order);
			if (order > face[start_face + iface].order)
				order = face[start_face + iface].order;
			for (unsigned int ivert = 0; ivert < order; ivert++)
			{
				unsigned long vertidx = face[start_face + iface].vertices[ivert];
				Vertex *vert = &vertex[vertidx];
				vec2 new_uv = face_uv[iface].tex_uv[ivert];
				unsigned long new_matr = face_uv[iface].material;
				//ok, we have a vertex; if it has no mapping - just set it.
				if (!vert_has_uv[vertidx])
				{
					vert->tex_uv = new_uv;
					vert->material = new_matr;
					vert_has_uv[vertidx] = true;
					continue;
				}
				//it has mapping, maybe identical with the one we want?
				if ((vert->tex_uv == new_uv) && (vert->material == new_matr))
					continue;
				//so we know we can't use this vertex anymore. Let's search for same one
				Vertex vert2 = *vert;
				vert2.tex_uv = new_uv;
				vert2.material = new_matr;
				if (!opt.fast_mode)
				{
					unsigned long vertidx2;
					if (find_identical_vertex(vertidx2, &vert2))
					{
						face[start_face + iface].vertices[ivert] = vertidx2;
						vert_has_uv[vertidx2] = true;
						continue;
					}
				}
				//there is no proper vertex - we have to clone the one we have
				vertex[vertex_num] = Vertex(vert2);
				vert_has_uv[vertex_num] = true;
				face[start_face + iface].vertices[ivert] = vertex_num;
				//if (debug)
				//    printf("    FACE_TO_VERTEX_UV: Vertex %lu cloned into %lu to set proper UV mapping\n",vertidx,vertex_num);
				vertex_num++;
			}
		}
		else
		{
			if (debug)
				printf("    FACE_TO_VERTEX_UV: Warning, face index %lu out of bounds!\n",
						start_face + iface);
		}
	}
	if (debug)
		printf("    FACE_TO_VERTEX_UV: To set proper UV mapping, %lu vertices has been cloned\n",
				vertex_num - vertex_num_base);
	return SUCCESS;
}

/**
 Purpose:
 VERTEX_TO_FACE_UV Converts UV mapping coords from vertices to faces

 Warning:
 face_uv must be at least coords_num entries long. All "order" variables
 in face_uv structure must be set identical to those from face structure,
 starting at start_face item.

 Created: 04 Mar 2007 Tomasz Lis
 */
bool IVCONV::vertex_to_face_uv(array<FaceUV> &face_uv, unsigned long start_face,
		unsigned long coords_num)
{
	if (debug)
		printf("    VERTEX_TO_FACE_UV: converting UV for faces %lu to %lu\n",
				start_face, start_face + coords_num - 1);
	for (unsigned long iface = 0; iface < coords_num; iface++)
	{
		unsigned int order = face_uv[iface].order;
		if (start_face + iface < face_num)
		{
			if ((debug) && (order != face[start_face + iface].order))
				printf("    VERTEX_TO_FACE_UV: Warning, face_uv order (%u) differs from face order (%u)!\n",
						order, face[start_face + iface].order);
			if (order > face[start_face + iface].order)
				order = face[start_face + iface].order;
			for (unsigned int ivert = 0; ivert < order; ivert++)
			{
				unsigned long vertidx = face[start_face + iface].vertices[ivert];
				face_uv[iface].tex_uv[ivert] = vertex[vertidx].tex_uv;
			}
		}
		else
		{
			if (debug)
				printf("    VERTEX_TO_FACE_UV: Warning, face index %lu out of bounds!\n",
						start_face + iface);
		}
	}
	return SUCCESS;
}

/*
Purpose:

  materials_fixups checks if materials and textures are properly set,
   also creates default material if the scene don't have any

	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::materials_fixups()
{

	// If there are no materials at all, define one.
	// PhG : no, it will be defined only when exporting in a format that supports materials
	// TL : NO! Very bad idea. We cannot have errors in data structure, no matter which format we choose

	if (material_num < 1)
	{
		material_num = 1;
		material[0] = Material(0.7f, 0.7f, 0.7f);
		sprintf(material[0].name, "%s%03u", DEFAULT_MATERIAL_NAME, 0);
	}

	//If a material has wrong texture number, remove it
	for (unsigned int imat = 0; imat < material_num; imat++)
		if ((material[imat].texture < 0)
				|| (material[imat].texture >= texture_num))
			material[imat].texture = -1;
	for (unsigned int imat = 0; imat < material_num; imat++)
		if ((material[imat].mask_texture < 0)
				|| (material[imat].mask_texture >= texture_num))
			material[imat].mask_texture = -1;
	for (unsigned int imat = 0; imat < material_num; imat++)
		if ((material[imat].bump_texture < 0)
				|| (material[imat].bump_texture >= texture_num))
			material[imat].bump_texture = -1;

	//If a vertex has not been assigned a material, set it to material 0.
	for (unsigned long vertidx = 0; vertidx < vertex_num; vertidx++)
	{
		if ((vertex[vertidx].material < 0)
				|| (vertex[vertidx].material > material_num - 1))
			vertex[vertidx].material = 0;
	}
	return;
}

/*
Purpose:

  get_material_index_or_create returns index of material with given name,
   or creates material with this name. If name is invalid, it returns 0.

	Author:   Tomasz Lis
	Modified: 02 Mar 2007 Tomasz Lis
*/
unsigned int IVCONV::get_material_index_or_create(char *name)
{
	if ((name == NULL) || (strlen(name) < 1))
		return 0;
	for (unsigned int idx = 0; idx < material_num; idx++)
		if (strncmp(name, material[idx].name, LINE_MAX_LEN) == 0)
			return idx;
	unsigned int idx = material_num;
	material_num++;
	strncpy(material[idx].name, name, LINE_MAX_LEN);
	return idx;
}

/*
Purpose:

  get_material_index returns index of material with given name,
   or returns -1 if material doesn't exist or name is invalid.

	Author:   Tomasz Lis
	Modified: 02 Mar 2007 Tomasz Lis
*/
int IVCONV::get_material_index(char *name)
{
	if ((name == NULL) || (strlen(name) < 1))
		return -1;
	for (int idx = 0; idx < material_num; idx++)
		if (strncmp(name, material[idx].name, LINE_MAX_LEN) == 0)
			return idx;
	return -1;
}

/*
Purpose:

  get_face_major_material returns material used in most of face vertices

	Author:   Tomasz Lis
	Modified: 02 Mar 2007 Tomasz Lis
*/
unsigned long IVCONV::get_face_major_material(unsigned long face_idx)
{
	if (face_idx >= face_num)
		return 0;
	array<unsigned int> uses_count;
	unsigned long mat_idx;
	for (mat_idx = 0; mat_idx < material_num; mat_idx++)
		uses_count[mat_idx] = 0;
	unsigned int ivert;
	for (ivert = 0; ivert < face[face_idx].order; ivert++)
	{
		unsigned long vertidx = face[face_idx].vertices[ivert];
		if (vertidx >= vertex_num)
			continue;
		mat_idx = vertex[vertidx].material;
		if (mat_idx >= material_num)
			continue;
		uses_count[mat_idx]++;
	}
	unsigned long mat_max = 0;
	for (mat_idx = 0; mat_idx < material_num; mat_idx++)
		if (uses_count[mat_idx] > uses_count[mat_max])
			mat_max = mat_idx;
	return mat_max;
}

/*
Purpose:

  face_materials_to_indexes helps assigning materials to faces in some formats.
    it reads face_material_names array and sets material indexes inside faces_uv
    to proper values. face_material_names should have face_num elements.

	Author:   Tomasz Lis
	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::face_materials_to_indexes(array<NameItm> &face_material_names,
		array<FaceUV> &face_uv, unsigned long coords_num)
{
	for (unsigned int idx = 0; idx < coords_num; idx++)
	{
		int mat_idx = get_material_index_or_create(face_material_names[idx].name);
		face_uv[idx].material = mat_idx;
	}
}

/*
Purpose:

  line_vertices_to_shape_coords converts line vertices (returned by some
   format readers) to shape coords.

	Author:   Tomasz Lis
	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::line_vertices_to_shape_coords(array<Vertex> &lnvert,
		unsigned long lnvert_num, unsigned long start_shape)
{
	unsigned long vertidx = 0;
	for (unsigned long shpidx = start_shape; shpidx < shape_num; shpidx++)
	{
		for (unsigned int ptidx = 0; ptidx < shape[shpidx].order; ptidx++)
		{
			if (vertidx >= lnvert_num)
				return;
			shape[shpidx].cor3[ptidx] = lnvert[vertidx].cor3;
			vertidx++;
		}
	}
}

/*
Purpose:

  vertex_colors_to_colist reads colors from list of vertices "vert_idx"
   into separate list "colr", merging same colors. This is useful
   when writing to some formats which include color list and color indexes.
   The indexes are returned in vert_cidx, which has vert_count entries.

	Author:   Tomasz Lis
	Modified: 14 Mar 2007 Tomasz Lis
*/
bool IVCONV::vertex_colors_to_colist(array<ColorItm> &colr,
		array<unsigned long> &vert_cidx, unsigned long &colr_num,
		array<unsigned long> vert_idx, unsigned long vert_count)
{
	vert_cidx.reserve(vert_count);
	unsigned long ivert;
	for (ivert = 0; ivert < vert_count; ivert++)
	{
		unsigned long v_idx = vert_idx[ivert];
		vec3 ncolor = vec3(vertex[v_idx].rgb);
		unsigned long colr_idx;
		long ncol_idx = -1;
		for (colr_idx = 0; colr_idx < colr_num; colr_idx++)
		{
			if ((colr[colr_idx].rgb) == ncolor)
			{
				ncol_idx = colr_idx;
				break;
			}
		}
		if (ncol_idx < 0)
		{
			ncol_idx = colr_num;
			colr[ncol_idx] = ColorItm();
			colr[ncol_idx].rgb = vec3(ncolor);
			colr_num++;
		}
		vert_cidx[ivert] = ncol_idx;
	}	//end for (ivert...)
	return SUCCESS;
}

/*
Purpose:

  shape_coords_to_line_vertices converts shape coords to line vertices
   (required by some format writers).

	Author:   Tomasz Lis
	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::shape_coords_to_line_vertices(array<Vertex> &lnvert,
		unsigned long &lnvert_num, unsigned long start_shape)
{
	lnvert_num = 0;
	for (unsigned long shpidx = start_shape; shpidx < shape_num; shpidx++)
	{
		for (unsigned int ptidx = 0; ptidx < shape[shpidx].order; ptidx++)
		{
			lnvert[lnvert_num].cor3 = shape[shpidx].cor3[ptidx];
			lnvert_num++;
		}
	}
}

/*
Purpose:

  DATA_INIT initializes (or clears) the internal graphics data.

	Author: John Burkardt
	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::data_init()
{
	if (debug)
	{
		printf("\n");
		printf("  DATA_INIT: ");
	}
	env = Environment();             // environment options
	view = Camera();                 // Default view
	camera_num = 0;
	camera.clear();
	light_num = 0;
	light.clear();
	material_num = 0;
	material.clear();
	texture_num = 0;
	texture.clear();
	mesh_num = 0;
	mesh.clear();
	face_num = 0;
	face.clear();
	vertex_num = 0;
	vertex.clear();
	shape_num = 0;
	shape.clear();

	tmat_init(transform_matrix); //the current transformation matrix
	stats = Stats();

	if (debug)
		printf("Graphics data initialized.\n");
	return;
}
/******************************************************************************/

/*
Purpose:

  DATA_READ reads a file into internal graphics data.

	Author:   John Burkardt
	Modified: 26 Sep 1999
	Modified: 02 Mar 2007 Tomasz Lis
*/
bool IVCONV::data_read()
{
	char *filein_type;
	bool ok;
	//Retrieve the input file type.
	filein_type = file_ext(filein_name);

	if (filein_type == NULL)
	{
		printf("\n");
		printf("DATA_READ - Fatal error!\n");
		printf("  Could not determine the type of '%s'.\n", filein_name);
		return ERROR;
	}
	else if (debug)
	{
		printf("\n");
		printf("DATA_READ: Input file has type %s.\n", filein_type);
	}
	//Initialize some data.
	stats = Stats();

	FileIO *filein = NULL;
	//Open the file.
	if (is_binary_format(filein_type))
		filein = new FileIO(filein_name, "rb");
	else
		filein = new FileIO(filein_name, "r");

	if (!filein->is_opened())
	{
		printf("\n");
		printf("DATA_READ - Fatal error!\n");
		printf("  Could not open the input file \"%s\"!\n", filein_name);
		return ERROR;
	}

	//Read the information in the file.
#ifdef _ivcon_use_tds
	if (leqi(filein_type, "3DS"))
	{
		ok = tds_read(filein);
	}
	else
#endif
#ifdef _ivcon_use_asc
	if (leqi(filein_type, "ASC"))
	{
		ok = asc_read(filein);
	}
	else
#endif
#ifdef _ivcon_use_ase
	if (leqi(filein_type, "ASE"))
	{
		ok = ase_read(filein);
	}
	else
#endif
#ifdef _ivcon_use_byu
	if (leqi(filein_type, "BYU"))
	{
		ok = byu_read(filein);
	}
	else
#endif
	/*
	 #ifdef _ivcon_use_cob
	 if ( leqi ( filein_type, "COB" )  )
	 {
	 ok = cob_read ( filein );
	 } else
	 #endif
	 */
#ifdef _ivcon_use_dxf
	if (leqi(filein_type, "DXF"))
	{
		ok = dxf_read(filein);
	}
	else
#endif
#ifdef _ivcon_use_gmod
	if (leqi(filein_type, "GMOD"))
	{
		ok = gmod_read(filein->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_hrc
	if (leqi(filein_type, "HRC"))
	{
		ok = hrc_read(filein);
	}
	else
#endif
#ifdef _ivcon_use_iv
	if (leqi(filein_type, "IV"))
	{
		ok = iv_read(filein->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_wrl
	if (leqi(filein_type, "WRL"))
	{
		ok = iv_read(filein->fileptr);	// reads also vrml
	}
	else
#endif
#ifdef _ivcon_use_obj
	if (leqi(filein_type, "OBJ"))
	{
		ok = obj_read(filein->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_smf
	if (leqi(filein_type, "SMF"))
	{
		ok = smf_read(filein);
	}
	else
#endif
#ifdef _ivcon_use_stl
	if (leqi(filein_type, "STL") || leqi(filein_type, "STLA"))
	{
		ok = stla_read(filein->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_stlb
	if (leqi(filein_type, "STLB"))
	{
		ok = stlb_read(filein);
	}
	else
#endif
#ifdef _ivcon_use_swv
	if (leqi(filein_type, "SWV"))
	{
		ok = swv_read(filein->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_swm
	if (leqi(filein_type, "SWM"))
	{
		ok = swm_read(filein->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_tri
	if (leqi(filein_type, "TRI") || leqi(filein_type, "TRIA"))
	{
		ok = tria_read(filein);
	}
	else
#endif
#ifdef _ivcon_use_trib
	if (leqi(filein_type, "TRIB"))
	{
		ok = trib_read(filein);
	}
	else
#endif
#ifdef _ivcon_use_vla
	if (leqi(filein_type, "VLA"))
	{
		ok = vla_read(filein);
	}
	else
#endif
	{
		printf("\n");
		printf("DATA_READ - Fatal error!\n");
		printf(
				"  Unacceptable input file type - cannot recognize by extension.\n");
		return ERROR;
	}

	//Close the file
	delete filein;

	if (debug)
		printf("DATA_READ: Finished reading the data file.\n");

	//Restore the transformation matrix.
	tmat_init(transform_matrix);

	data_input_report();

	//Warn about any errors that occurred during reading.
	if (!ok)
	{
		printf("\n");
		printf("DATA_READ - Fatal error!\n");
		printf("  An error occurred while reading the input file.\n");
	}

	//Check the data.
	//You MUST wait until after this check before doing other computations,
	//since vertex_num and other variables could be much larger than the legal
	//maximums, until corrected by DATA_CHECK.
	data_check();

	materials_fixups();

	//Delete edges of zero length.
	edge_null_delete();

	//Compute the area of each face.
	face_area_set(true);

	//Delete faces with zero area.
	face_null_delete();

	// Delete empty meshes
	mesh_null_delete();

	//Recompute zero face-vertex normals from vertex positions.
	vertex_normal_set();

	//Recompute zero face normals by averaging face-vertex normals.
	face_normal_ave();

	//Report on the vertex coordinate range.
	vertex_cor3_range();

	return ok;
}
/**********************************************************************/

/*
Purpose:

  DATA_REPORT gives a summary of the 3D data stored in memory

	Author: John Burkardt
	Modified: 24 May 1999
	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::data_report(bool show_comments)
{
	if (show_comments)
	{
		printf("\n");
		printf("DATA_REPORT - The 3D data stored in memory contains:\n");
		printf("\n");
	}
	//Compute some stats
	stats.max_order = face[0].order;
	for (unsigned long iface = 0; iface < face_num; iface++)
		if (face[iface].order > stats.max_order)
			stats.max_order = face[iface].order;
	if (show_comments)
		printf("    Camera defines a point of view on scene.\n");
	printf("  Cameras                    %d\n", camera_num);
	printf("  Lights                     %d\n", light_num);
	if (show_comments)
		printf("    Meshes are whole objects in 3D world.\n");
	printf("  Meshes (Objects)           %d\n", mesh_num);
	if (show_comments)
		printf("    Meshes are made of Faces.\n");
	printf("  Faces                      %d\n", face_num);
	if (show_comments)
		printf("    Faces are made of Vertices.\n");
	printf("  Vertices per face, maximum %d\n", stats.max_order);
	printf("  Total vertices number      %d\n", vertex_num);
	if (show_comments)
		printf("    Lines are usually invisible helpers.\n");
	printf("  Line items                 %d\n", shape_num);
	if (show_comments)
		printf("    Materials controls the mesh look.\n");
	printf("  Materials                  %d\n", material_num);
	printf("  Textures                   %d\n", texture_num);
	return;
}
/**********************************************************************/

/*
Purpose:

  DATA_INPUT_REPORT gives a summary of the contents of the data file.

	Author: John Burkardt
	Modified: 24 May 1999
	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::data_input_report()
{
	printf ( "\n" );
	printf ( "DATA_INPUT_REPORT - The input file contains:\n" );
	printf ( "\n" );
	printf ( "  Bad data items             %d\n", stats.bad_num );
	printf ( "  Text lines                 %d\n", stats.text_num );
	printf ( "  Data bytes (binary data)   %d\n", stats.bytes_num );
//	printf ( "  Colors                     %d\n", color_num );
	printf ( "  Comments                   %d\n", stats.comment_num );
	printf ( "  Duplicate points           %d\n", stats.dup_num );
//	printf ( "  Groups                     %d\n", group_num );
    data_report(false);
	return;
}
/******************************************************************************/

/*
Purpose:

  DATA_WRITE writes the internal graphics data to a file.

	Author: John Burkardt
	Modified: 22 May 1999
*/
bool IVCONV::data_write()
{
	char *fileout_type;
	int line_num_save;
	int result;

	result = SUCCESS;
	//Retrieve the output file type.
	fileout_type = file_ext(fileout_name);

	if (fileout_type == NULL)
	{
		printf("\n");
		printf("DATA_WRITE - Fatal error!\n");
		printf("  Could not determine the output file type.\n");
		return ERROR;
	}

	FileIO *fileout = NULL;
	//Open the output file.
	if (is_binary_format(fileout_type))
		fileout = new FileIO(fileout_name, "wb");
	else
		fileout = new FileIO(fileout_name, "w");

	if (!fileout->is_opened())
	{
		printf("\n");
		printf("DATA_WRITE - Fatal error!\n");
		printf("  Could not open the output file \"%s\"!\n", fileout_name);
		return ERROR;
	}

	//Write the output file.
#ifdef _ivcon_use_tds
	if (leqi(fileout_type, "3DS"))
	{
		reduce_face_order(3);
		reduce_item_numbers(65535);
		force_unique_names(10);
		face_area_set(false);
		result = tds_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_asc
	if (leqi(fileout_type, "ASC"))
	{
		result = asc_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_ase
	if (leqi(fileout_type, "ASE"))
	{
		result = ase_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_byu
	if (leqi(fileout_type, "BYU"))
	{
		result = byu_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_cob
	if (leqi(fileout_type, "COB"))
	{
		result = cob_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_dxf
	if (leqi(fileout_type, "DXF"))
	{
		reduce_face_order(10);
		face_area_set(false);
		result = dxf_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_gmod
	if (leqi(fileout_type, "GMOD"))
	{
		reduce_face_order(4);
		reduce_item_numbers(65530);
		face_area_set(false);
		result = gmod_write(fileout->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_hrc
	if (leqi(fileout_type, "HRC"))
	{
		result = hrc_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_iv
	if (leqi(fileout_type, "IV"))
	{
		result = iv_write(fileout->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_obj
	if (leqi(fileout_type, "OBJ"))
	{
		result = obj_write(fileout->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_pov
	if (leqi(fileout_type, "POV"))
	{
		result = pov_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_smf
	if (leqi(fileout_type, "SMF"))
	{
		result = smf_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_stl
	if (leqi(fileout_type, "STL") || leqi(fileout_type, "STLA"))
	{
		result = stla_write(fileout->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_stlb
	if (leqi(fileout_type, "STLB"))
	{
		result = stlb_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_tec
	if (leqi(fileout_type, "TEC"))
	{
		reduce_face_order(3);
		face_area_set(false);
		result = tec_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_tri
	if (leqi(fileout_type, "TRI") || leqi(fileout_type, "TRIA"))
	{
		result = tria_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_trib
	if (leqi(fileout_type, "TRIB"))
	{
		result = trib_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_txt
	if (leqi(fileout_type, "TXT"))
	{
		result = txt_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_ucd
	if (leqi(fileout_type, "UCD"))
	{
		result = ucd_write(fileout->fileptr);
	}
	else
#endif
#ifdef _ivcon_use_vla
	if (leqi(fileout_type, "VLA"))
	{
		line_num_save = shape_num;
		if (face_num > 0)
		{
			printf("\n");
			printf("DATA_WRITE - Note:\n");
			printf("  Face information will be converted to\n");
			printf("  line information for output to a VLA file.\n");
			face_to_line(opt.line_prune);// pruning depending on current options
		}
		result = vla_write(fileout);
		shape_num = line_num_save;
	}
	else
#endif
#ifdef _ivcon_use_wrl
	if (leqi(fileout_type, "WRL"))
	{
		result = wrl_write(fileout);
	}
	else
#endif
#ifdef _ivcon_use_xgl
	if (leqi(fileout_type, "XGL"))
	{
		result = xgl_write(fileout);
	}
	else
#endif
	{

		result = ERROR;
		printf("\n");
		printf("DATA_WRITE - Fatal error!\n");
		printf("  Unacceptable output file type \"%s\".\n", fileout_type);

	}

	//Close the output file.
	delete fileout;

	if (result == ERROR)
		return ERROR;
	else
		return SUCCESS;
}
/******************************************************************************/

/*
Purpose:

  EDGE_NULL_DELETE deletes face edges with zero length.

	Author: John Burkardt
	Modified: 16 July 1999
	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::edge_null_delete()
{
	printf("\n");
	printf("EDGE_NULL_DELETE:\n");

	array<unsigned long> face2;

	unsigned long edge_num = 0;
	unsigned int edge_num_del = 0;
	//Consider each face
	for (unsigned long iface = 0; iface < face_num; iface++)
	{
		//Consider each pair of consecutive vertices,
		// and create new vertices array
		unsigned int face_order2 = 0;
		for (unsigned int ivert = 0; ivert < face[iface].order; ivert++)
		{
			edge_num++;
			unsigned int jvert = ivert + 1;
			if (jvert >= face[iface].order)
				jvert = 0;

			unsigned long inode = face[iface].vertices[ivert];
			unsigned long jnode = face[iface].vertices[jvert];

			float x = vertex[inode].cor3[0] - vertex[jnode].cor3[0];
			float y = vertex[inode].cor3[1] - vertex[jnode].cor3[1];
			float z = vertex[inode].cor3[2] - vertex[jnode].cor3[2];

			float distsq = x * x + y * y + z * z;

			if (distsq != 0.0)
			{
				face2[face_order2] = inode;
				face_order2++;
			}
			else
			{
				edge_num_del++;
			}
		}
		//Copy the new vertices array into the face
		face[iface].order = face_order2;
		unsigned int ivert;
		for (ivert = 0; ivert < face[iface].order; ivert++)
		{
			face[iface].vertices[ivert] = face2[ivert];
		}
	}

	printf("  There are a total of %lu edges.\n", edge_num);
	if ((edge_num_del > 0) || (debug))
		printf("  Of these, %u were of zero length, and deleted.\n", edge_num_del);

	return;
}
/**********************************************************************/

/*
Purpose:
  FACE_AREA_SET computes the area of all faces, also marks for deletion
    tiny faces, with area smaller that given part of maximal area.
Formula:
  The area is the sum of the areas of the triangles formed by
  node N with consecutive pairs of nodes.
Reference:
  Adrian Bowyer and John Woodwark,
  A Programmer's Geometry,
  Butterworths, 1983.

  Author:  John Burkardt
  Modified: 17 Jul 1999
  Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::face_area_set(bool delete_small_faces)
{
	printf("\n");
	printf("FACE_AREA_SET:\n");

	for (unsigned int iface = 0; iface < face_num; iface++)
	{
		face[iface].area = 0.0;
		if (face[iface].order < 3)
			continue;

		for (unsigned int i = 0; i < face[iface].order - 2; ++i)
		{
			unsigned long i1 = face[iface].vertices[i];
			unsigned long i2 = face[iface].vertices[i + 1];
			unsigned long i3 = face[iface].vertices[i + 2];
			if ((i1 >= vertex_num) || (i2 >= vertex_num) || (i3 >= vertex_num))
				continue;

			float x1 = vertex[i1].cor3[0];
			float y1 = vertex[i1].cor3[1];
			float z1 = vertex[i1].cor3[2];

			float x2 = vertex[i2].cor3[0];
			float y2 = vertex[i2].cor3[1];
			float z2 = vertex[i2].cor3[2];

			float x3 = vertex[i3].cor3[0];
			float y3 = vertex[i3].cor3[1];
			float z3 = vertex[i3].cor3[2];

			//Find the projection of (P3-P1) onto (P2-P1).
			float dot = (x2 - x1) * (x3 - x1) + (y2 - y1) * (y3 - y1)
					+ (z2 - z1) * (z3 - z1);
			float base = sqrt(
					(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)
							+ (z2 - z1) * (z2 - z1));

			//The height of the triangle is the length of (P3-P1) after its
			//projection onto (P2-P1) has been subtracted.
			float height;
			float alpha;
			if (base == 0.0)
			{
				height = 0.0;
			}
			else
			{
				alpha = dot / (base * base);
				float x = x3 - x1 - alpha * (x2 - x1);
				float y = y3 - y1 - alpha * (y2 - y1);
				float z = z3 - z1 - alpha * (z2 - z1);
				height = sqrt(x * x + y * y + z * z);
			}

			float area_tri = 0.5 * base * height;
			face[iface].area += area_tri;

		} //end for ( i = ...)
	} //end for ( iface = ...)

	// Selecting max face area
	float area_max = face[0].area;
	for (unsigned int iface = 1; iface < face_num; iface++)
	{
		float iarea = face[iface].area;
		if (area_max < iarea)
			area_max = iarea;
	}
	// Selecting min and min but nonzero area
	float area_min = area_max;
	float area_min_nz = area_max;
	unsigned int zfaces = 0;
	for (unsigned int iface = 0; iface < face_num; iface++)
	{
		float iarea = face[iface].area;
		if ((area_min > iarea))
			area_min = iarea;
		if ((area_min_nz > iarea) && (iarea > 0.0))
			area_min_nz = iarea;
		if (iarea <= 0.0)
			zfaces++;
	}
	printf("  Minimum nonzero face area is %f\n", area_min_nz);
	printf("  Maximum face area is %f\n", area_max);
	if ((zfaces > 0) || (debug))
		printf("  Found %lu zero-area faces\n", zfaces);
	if (delete_small_faces)
	{
		float tol = area_max * opt.tiny_face_area;
		if (area_min < tol)
		{
			unsigned int face_num_del = 0;
			for (unsigned int iface = 0; iface < face_num; iface++)
				if (face[iface].area < tol)
				{
					face[iface].order = 0;
					face_num_del++;
				}
			printf("  Marked %u tiny faces for deletion.\n", face_num_del);
		}
	}
	return;
}
/******************************************************************************/

/*
Purpose:

  FACE_NORMAL_AVE sets face normals as average of face vertex normals.

	Author: John Burkardt
	Modified: 09 October 1998
	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::face_normal_ave()
{
	if (face_num <= 0)
		return;

	printf("\n");
	printf("FACE_NORMAL_AVE:");

	unsigned int nfix = 0;

	for (unsigned int iface = 0; iface < face_num; iface++)
	{
		//Check the norm of the current normal vector.
		float x = face[iface].normal[0];
		float y = face[iface].normal[1];
		float z = face[iface].normal[2];
		float norm = (float) sqrt(x * x + y * y + z * z);

		if (norm == 0.0)
		{
			nfix++;
			face[iface].normal = vec3(0.0, 0.0, 0.0);

			for (unsigned int ivert = 0; ivert < face[iface].order; ivert++)
				for (int i = 0; i < 3; ++i)
				{
					unsigned long vertidx = face[iface].vertices[ivert];
					face[iface].normal[i] += vertex[vertidx].normal[i];
				}

			x = face[iface].normal[0];
			y = face[iface].normal[1];
			z = face[iface].normal[2];
			norm = (float) sqrt(x * x + y * y + z * z);

			if (norm == 0.0)
			{
				float val = (float) (1.0 / sqrt(3.0));
				face[iface].normal = vec3(val, val, val);
			}
			else
			{
				for (int i = 0; i < 3; ++i)
					face[iface].normal[i] = face[iface].normal[i] / norm;
			}
		}
	}

	if (nfix > 0)
	{
		printf(" Recomputed %d face normals\n", nfix);
		printf("  by averaging face vertex normals.\n");
	}
	else
		printf(" Face normals OK.\n");
	return;
}
/**********************************************************************/

/*
Purpose:

  FACE_NULL_DELETE deletes faces of order less than 3.

	Comments:

	  Thanks to Susan M. Fisher, University of North Carolina,
	  Department of Computer Science, for pointing out a coding error
	  in FACE_NULL_DELETE that was overwriting all the data!

	Author: John Burkardt
	Modified: 28 February 2007
	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::face_null_delete()
{
	printf("\n");
	printf("FACE_NULL_DELETE:\n");

	//FACE_NUM2 is the number of faces we'll keep.
	int face_num2 = 0;
	//Check every face.
	for (unsigned int iface = 0; iface < face_num; iface++)
	{
		//Keep it only if it has order 3 or more.
		if (face[iface].order >= 3)
		{
			//We don't have to slide data down in the array until
			//NUMFACE2 and IFACE get out of synch, that is, after
			//we've discarded at least one face.
			if (face_num2 != iface)
			{
				face_copy(face_num2, iface);
			}
			//Update the count only after we've used the un-incremented value
			//as a pointer.
			face_num2++;

		}
	} //end for (iface=....)

	printf("  There are a total of %d faces.\n", face_num);
	if ((face_num != face_num2) || (debug))
		printf("  Of these, %d passed the order test and %d was deleted.\n",
				face_num2, face_num - face_num2);

	face_num = face_num2;
	return;
}
/******************************************************************************/

/*
Purpose:

  FACE_PRINT prints out information about a face.

	Author: John Burkardt
	Modified: 31 August 1998
	Modified: 02 Mar 2007 Tomasz Lis
*/
bool IVCONV::face_print(unsigned long iface)
{
	if (iface < 0 || iface > face_num - 1)
	{
		printf("\n");
		printf("FACE_PRINT - Fatal error!\n");
		printf("  Face indices must be between 1 and %lu\n", face_num);
		printf("  But your requested value was %lu\n", iface);
		return false;
	}

	printf("\n");
	printf("FACE_PRINT\n");
	printf("  Information about face %lu\n", iface);
	printf("\n");
	printf("  Number of vertices is %u\n", face[iface].order);
	printf("\n");
	printf("  Vertex list:\n");
	printf("    Vertex #, Node #, Material #, X, Y, Z:\n");
	printf("\n");
	for (unsigned int ivert = 0; ivert < face[iface].order; ivert++)
	{
		unsigned long j = face[iface].vertices[ivert];
		unsigned int k = vertex[j].material;
		printf(" %d %d %d %f %f %f\n", ivert, j, k, vertex[j].cor3[0],
				vertex[j].cor3[1], vertex[j].cor3[2]);
	}

	printf("\n");
	printf("  Face normal vector:\n");
	printf("\n");
	printf(" %f %f %f\n", face[iface].normal[0], face[iface].normal[1],
			face[iface].normal[2]);

	printf("\n");
	printf("  Vertex face normals:\n");
	printf("\n");
	for (unsigned int ivert = 0; ivert < face[iface].order; ivert++)
	{
		unsigned long vertidx = face[iface].vertices[ivert];
		printf(" %d %f %f %f\n", ivert, vertex[vertidx].normal[0],
				vertex[vertidx].normal[1], vertex[vertidx].normal[2]);
	}

	return SUCCESS;

}
/**********************************************************************/

/*
Purpose:

  FACE_REVERSE_ORDER reverses the order of the vertices in each face.

	Discussion:

	  Reversing the order of the vertices requires that the normal vectors
	  be reversed as well, so this routine will automatically reverse
	  the normals associated with vertices and faces.

	Author: John Burkardt
	Modified: 28 June 1999
	Modified: 02 Mar 2007 Tomasz Lis
*/
bool IVCONV::face_reverse_order()
{
	bool Result;
	//Reversing vertices order inside faces
	Result = reverse_faces();

	//Reversing normals in vertices and faces
	Result &= reverse_normals();

	printf("\n");
	printf("FACE_REVERSE_ORDER\n");
	printf("  Each list of nodes defining a face\n");
	printf("  has been reversed; related information,\n");
	printf("  including normal vectors, was also updated.\n");

	return Result;
}

/******************************************************************************/

/*
Purpose:

  FACE_SUBSET selects a subset of the current faces as the new scene.

	Warning:
	  The original graphic object is overwritten by the new one.

	Author: John Burkardt
	Modified: 12 Oct 1998
	Modified: 02 Mar 2007 Tomasz Lis
*/
bool IVCONV::face_subset(unsigned int iface1, unsigned int iface2)
{
	if ((iface1 < 0) || (iface1 > face_num - 1))
		return ERROR;
	if ((iface2 < iface1) || (iface2 > face_num - 1))
		return ERROR;

	//Deleting all shapes - the selection can be made only from faces
	shape_num = 0;

	unsigned int inc = iface1;
	//"Slide" the data for the saved faces down the face arrays.
	for (unsigned int iface = 0; iface < iface2 + 1 - iface1; iface++)
		face[iface] = face[iface + inc];
	//Now reset the number of faces.
	face_num = iface2 + 1 - iface1;

	//Now, for each point I, set LIST(I) = J if point I is the J-th
	//point we are going to save, and 0 otherwise.  Then J will be
	//the new label of point I.
	array<long> list;
	for (unsigned long vertidx = 0; vertidx < vertex_num; vertidx++)
		list[vertidx] = -1;

	unsigned long vertex_num2 = 0;
	for (unsigned int iface = 0; iface < face_num; iface++)
		for (unsigned int ivert = 0; ivert < face[iface].order; ivert++)
		{
			unsigned long vertidx = face[iface].vertices[ivert];
			if (list[vertidx] == -1)
			{
				vertex_num2++;
				list[vertidx] = vertex_num2;
			}
		}

	//Now make the nonzero list entries rise in order, so that
	//we can compress the VERTEX data in a minute.
	vertex_num2 = 0;
	for (unsigned long ivert = 0; ivert < vertex_num; ivert++)
		if (list[ivert] != -1)
		{
			list[ivert] = vertex_num2;
			vertex_num2++;
		}

	//Relabel the FACE array with the new node indices.
	for (unsigned int iface = 0; iface < face_num; iface++)
		for (unsigned int ivert = 0; ivert < face[iface].order; ivert++)
		{
			unsigned long vertidx = face[iface].vertices[ivert];
			face[iface].vertices[ivert] = list[vertidx];
		}

	//Rebuild the VERTEX array by sliding data down.
	for (unsigned long vertidx = 0; vertidx < vertex_num; vertidx++)
	{
		long vertkdx = list[vertidx];
		if (vertkdx != -1)
			vertex[vertkdx] = vertex[vertidx];
	}

	vertex_num = vertex_num2;
	return SUCCESS;
}
/**********************************************************************/

/*
Purpose:

FACE_TO_LINE converts face information to line information.

Discussion:

    In some cases, the graphic information represented by polygonal faces
    must be converted to a representation based solely on line segments.
    This is particularly true if a VLA file is being written.

    Author: John Burkardt
    Modified: 26 May 1999
	Modified: 02 Mar 2007 Tomasz Lis
*/
bool IVCONV::face_to_line(unsigned char line_prune)
{
	bool out_of_shapes = false;
	//Case 0:No line pruning.
	if (line_prune == 0)
	{
		for (unsigned int iface = 0; iface < face_num; iface++)
		{
			if (shape_num > SHAPES_MAX)
			{
				out_of_shapes = true;
				break;
			}
			//Convert vertices to curve coordinate points
			shape[shape_num] = Shape();
			unsigned int sh_order = 0;
			for (unsigned int ivert = 0; ivert < face[iface].order; ivert++)
			{
				unsigned long vertidx = face[iface].vertices[ivert];
				shape[shape_num].cor3[sh_order] = vertex[vertidx].cor3;
				sh_order++;
			}
			//Repeat first vertex to close the curve, and set its color
			unsigned int ivert = 0;
			{
				unsigned long vertidx = face[iface].vertices[ivert];
				shape[shape_num].cor3[sh_order] = vertex[vertidx].cor3;
				shape[shape_num].rgb = vertex[vertidx].rgb;
				sh_order++;
			}
			shape[shape_num].order = sh_order;
			shape_num++;
		}
	}
	else
	//Case 2: Simple-minded line pruning.
	//Only draw line (I,J) if I < J.
	{
		for (unsigned int iface = 0; iface < face_num; iface++)
			//Convert vertices to curve coordinate points
			for (unsigned int ivert = 0; ivert < face[iface].order; ivert++)
			{
				if (shape_num > SHAPES_MAX)
				{
					out_of_shapes = true;
					break;
				}
				unsigned int jvert;
				if (ivert + 1 < face[iface].order)
					jvert = ivert + 1;
				else
					jvert = 0;
				unsigned long vertidx = face[iface].vertices[ivert];
				unsigned long vertjdx = face[iface].vertices[jvert];
				if (vertidx < vertjdx)
				{
					shape[shape_num] = Shape();
					unsigned int sh_order = 0;
					shape[shape_num].cor3[sh_order] = vertex[vertidx].cor3;
					shape[shape_num].rgb = vertex[vertidx].rgb;
					sh_order++;
					shape[shape_num].cor3[sh_order] = vertex[vertjdx].cor3;
					sh_order++;
					shape[shape_num].order = sh_order;
					shape_num++;
				}
			}
	}
	//Deleting all faces
	face_num = 0;
	//Warn if maximum exceeded
	if (out_of_shapes)
	{
		printf("\n");
		printf("FACE_TO_LINE - Warning:\n");
		printf("  Some face information was lost, because maximum\n");
		printf("  number of shapes (%lu) was exceeded.\n", SHAPES_MAX);
		return false;
	}
	printf("  Generated %lu shapes. All faces erased.\n", shape_num);
	return SUCCESS;
}
/**********************************************************************/

/*
Purpose:
  FACE_TO_VERTEX_MAT extends face material definitions to vertices.

	Discussion:
	  Assuming material indices are defined for all the faces, this
	  routine assigns to each vertex of a face the material of that face.

    Warning:
      face_uv must be at least coords_num entries long. All "order" variables
      in face_uv structure must be set identical to those from face structure,
      starting at start_face item.
      FACES_UV can be initialized by call of clear_faces_uv

	Author:   John Burkardt
	Modified: 22 May 1999
	Modified: 02 Mar 2007 Tomasz Lis
*/
bool IVCONV::face_to_vertex_material(array<FaceUV> &face_uv,
		unsigned long start_face, unsigned long coords_num)
{
	if (debug)
		printf("    FACE_TO_VERTEX_MAT: converting material for faces %lu to %lu\n",
				start_face, start_face + coords_num - 1);
	for (unsigned long iface = 0; iface < coords_num; iface++)
	{
		unsigned int order = face_uv[iface].order;
		if (start_face + iface < face_num)
		{
			if ((debug) && (order != face[start_face + iface].order))
				printf("    FACE_TO_VERTEX_MAT: Warning, face_uv order (%u) differs from face order (%u)!\n",
						order, face[start_face + iface].order);
			if (order > face[start_face + iface].order)
				order = face[start_face + iface].order;
			for (unsigned int ivert = 0; ivert < order; ivert++)
			{
				unsigned long vertidx = face[start_face + iface].vertices[ivert];
				vertex[vertidx].material = face_uv[iface].material;
			}
		}
		else
		{
			if (debug)
				printf("    FACE_TO_VERTEX_MAT: Warning, face index %lu out of bounds!\n", start_face + iface);
		}
	}
	return SUCCESS;
}
/**********************************************************************/

/*
Purpose:

  MESH_NULL_DELETE deletes meshes that have no faces

	Author: Tomasz Lis
	Modified: 12 Mar 2007
*/
void IVCONV::mesh_null_delete()
{
	printf("\n");
	printf("MESH_NULL_DELETE:\n");
	if (mesh_num < 1)
	{
		mesh_num = 1;
		mesh[0] = Mesh();
		sprintf(mesh[0].name, "%s%03u", DEFAULT_MESH_NAME, 0);
		printf("  No meshes found, default mesh created\n");
	}
	//Creating array to count faces in meshes
	array<unsigned long> mesh_facenum;
	{
		for (unsigned long i = 0; i < mesh_num; i++)
			mesh_facenum[i] = 0;
	}
	{	//counting faces in meshes
		unsigned long face_fix_count = 0;
		for (unsigned long i = 0; i < face_num; i++)
		{
			unsigned long mesh_idx = face[i].mesh;
			if (mesh_idx < mesh_num)
			{
				mesh_facenum[mesh_idx]++;
			}
			else
			{
				face[i].mesh = 0;
				mesh_facenum[0]++;
				face_fix_count++;
			}
		}
		if (face_fix_count > 0)
			printf("  Fixed %lu bad mesh indexes inside faces\n",
					face_fix_count);
	}
	{
		for (unsigned long i = 0; i < mesh_num; i++)
			printf("  Mesh %lu, named \"%s\", contains %lu faces\n", i,
					mesh[i].name, mesh_facenum[i]);
	}
	//Creating array of new mesh indexes
	//and shortening MESH array
	unsigned long empty_count = 0;
	unsigned long new_mesh_num = 0;
	array<unsigned long> new_mesh;
	{
		new_mesh[0] = new_mesh_num;
		for (unsigned long i = 1; i < mesh_num; i++)
		{
			if (mesh_facenum[i] > 0)
			{
				new_mesh_num++;
				if (new_mesh_num != i)
					mesh[new_mesh_num] = mesh[i];
			}
			else
			{
				empty_count++;
			}
			new_mesh[i] = new_mesh_num;
		}
		new_mesh_num++;
	}
	{  //Updating mesh indices in faces
		for (unsigned long i = 0; i < face_num; i++)
		{
			unsigned long mesh_pos = face[i].mesh;
			face[i].mesh = new_mesh[mesh_pos];
		}
	}
	mesh_num = new_mesh_num;

	if ((empty_count > 0) || (debug))
		printf("  Deleted %lu empty meshes\n", empty_count);
	return;
}
/******************************************************************************/

/*
Purpose:

rename_meshes() renames meshes into given pattern.

    Author:    Tomasz Lis
    Modified:  17 Jul 2007 Tomasz Lis
*/
bool IVCONV::rename_meshes(char *name_pattern)
{
	if (debug)
	{
		printf("\n");
		printf("rename_meshes: starting the rename process.\n");
	}
	char npattern[LINE_MAX_LEN];
	char *pos = strstr(name_pattern, "*");
	int part_len;
	if (pos >= name_pattern)
		part_len = (int) (pos - name_pattern);
	else
		part_len = strlen(npattern);
	strncpy(npattern, name_pattern, part_len);
	strncpy(npattern + part_len, "%s", 2);
	strncpy(npattern + part_len + 2, name_pattern + part_len + 1,
			LINE_MAX_LEN - part_len - 2);
	char nname[LINE_MAX_LEN];
	for (unsigned long i = 0; i < mesh_num; i++)
	{
		strncpy(nname, mesh[i].name, LINE_MAX_LEN);
		sprintf(nname, npattern, mesh[i].name);
		if (debug)
			printf("    Renaming \"%s\" to \"%s\".\n", mesh[i].name, nname);
		strncpy(mesh[i].name, nname, LINE_MAX_LEN);
	}
	printf("rename_meshes: Renamed %d meshes.\n", mesh_num);
	return true;
}
/******************************************************************************/

/*
 Purpose:

 INIT_PROGRAM_DATA initializes (clears) the internal program data.

 Author:    John Burkardt
 Modified:  26 May 1999
 Modified:  02 Mar 2007 Tomasz Lis
 */
void IVCONV::init_program_data()
{
	if (debug)
	{
		printf("\n");
		printf("INIT_PROGRAM_DATA: initializing.\n");
	}
	opt = Options();
	byte_swap = false;
	debug = 0;
	strcpy(filein_name, "");
	strcpy(fileout_name, "");
	strcpy(default_name_prefix, "");
	data_init();

	return;
}
/******************************************************************************/

/*
Purpose:

  VERTEX_NORMAL_SET recomputes the face vertex normal vectors.

	Author: John Burkardt
	Modified: 12 October 1998
	Modified: 02 Mar 2007 Tomasz Lis
*/
void IVCONV::vertex_normal_set()
{
	if (face_num <= 0)
		return;

	printf("\n");
	printf("VERTEX_NORMAL_SET:\n");

	int nfix = 0;
	//Consider each face.
	for (unsigned int iface = 0; iface < face_num; iface++)
		for (unsigned int ivert = 0; ivert < face[iface].order; ivert++)
		{
			unsigned long vertidx = face[iface].vertices[ivert];
			float norm = 0.0;
			for (int i = 0; i < 3; ++i)
			{
				float temp = vertex[vertidx].normal[i];
				norm += temp * temp;
			}
			norm = (float) sqrt(norm);
			if (norm == 0.0)
			{
				nfix++;
				unsigned long i0 = face[iface].vertices[ivert];
				float x0 = vertex[i0].cor3[0];
				float y0 = vertex[i0].cor3[1];
				float z0 = vertex[i0].cor3[2];

				unsigned int jp1 = ivert + 1;
				if (jp1 >= face[iface].order)
					jp1 = jp1 - face[iface].order;
				unsigned long i1 = face[iface].vertices[jp1];
				float x1 = vertex[i1].cor3[0];
				float y1 = vertex[i1].cor3[1];
				float z1 = vertex[i1].cor3[2];

				unsigned int jp2 = ivert + 2;
				if (jp2 >= face[iface].order)
					jp2 = jp2 - face[iface].order;
				unsigned long i2 = face[iface].vertices[jp2];
				float x2 = vertex[i2].cor3[0];
				float y2 = vertex[i2].cor3[1];
				float z2 = vertex[i2].cor3[2];

				float xc = (y1 - y0) * (z2 - z0) - (z1 - z0) * (y2 - y0);
				float yc = (z1 - z0) * (x2 - x0) - (x1 - x0) * (z2 - z0);
				float zc = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);

				norm = (float) sqrt(xc * xc + yc * yc + zc * zc);

				if (norm == 0.0)
				{
					xc = (float) 1.0 / sqrt(3.0);
					yc = (float) 1.0 / sqrt(3.0);
					zc = (float) 1.0 / sqrt(3.0);
				}
				else
				{
					xc = xc / norm;
					yc = yc / norm;
					zc = zc / norm;
				}

				vertex[vertidx].normal[0] = xc;
				vertex[vertidx].normal[1] = yc;
				vertex[vertidx].normal[2] = zc;
			}
		}

	if (nfix > 0)
		printf("  Recomputed %d face vertex normals.\n", nfix);
	else
		printf("  Face vertex normals are OK.\n");
	return;
}
/**********************************************************************/

/*
Purpose:

  VERTEX_TO_FACE_MATERIAL extends vertex material definitions to faces.

    Warning:
      face_uv must be at least coords_num entries long. All "order" variables
      in face_uv structure must be set identical to those from face structure,
      starting at start_face item.
      FACES_UV can be initialized by call of clear_faces_uv

	Discussion:
	  Assuming material indices are defined for all the vertices, this
	  routine assigns to each face the material associated with most
	  of its vertices.

	Author: John Burkardt
	Modified: 22 May 1999
	Modified: 02 Mar 2007 Tomasz Lis
*/
bool IVCONV::vertex_to_face_material(array<FaceUV> &face_uv,
		unsigned long start_face, unsigned long coords_num)
{
	if (debug)
		printf("    VERTEX_TO_FACE_MAT: converting material for faces %lu to %lu\n",
				start_face, start_face + coords_num - 1);
	for (unsigned long iface = 0; iface < coords_num; iface++)
	{
		unsigned int order = face_uv[iface].order;
		if (start_face + iface < face_num)
		{
			if ((debug) && (order != face[start_face + iface].order))
				printf("    VERTEX_TO_FACE_MAT: Warning, face_uv order (%u) differs from face order (%u)!\n",
						order, face[start_face + iface].order);
			if (order > face[start_face + iface].order)
				order = face[start_face + iface].order;
			face_uv[iface].material = get_face_major_material(
					start_face + iface);
		}
		else
		{
			if (debug)
				printf("    VERTEX_TO_FACE_MAT: Warning, face index %lu out of face array bounds!\n",
						start_face + iface);
		}
	}
	if (debug)
		printf("    VERTEX_TO_FACE_MAT: face materials set\n");
	return SUCCESS;
}
/**********************************************************************/

bool IVCONV::is_binary_format(char *file_ext)
{
	if (leqi(file_ext, "3DS") ||
		leqi(file_ext, "GMOD") ||
        leqi(file_ext, "STLB") ||
		leqi(file_ext, "SWV")  ||
		leqi(file_ext, "SWM")  ||
		leqi(file_ext, "TRIB"))
		return true;
    return false;
}

/**********************************************************************/

/*
Purpose:

  reverse_normals reverses normal vectors in vertices and faces

	Modified: 02 Mar 2007 Tomasz Lis
*/
bool IVCONV::reverse_normals()
{
	for (unsigned long icor3 = 0; icor3 < vertex_num; icor3++)
		for (int i = 0; i < 3; ++i)
			vertex[icor3].normal[i] = -vertex[icor3].normal[i];

	for (unsigned long iface = 0; iface < face_num; iface++)
		for (int i = 0; i < 3; ++i)
			face[iface].normal[i] = -face[iface].normal[i];

	return SUCCESS;
}
/**********************************************************************/

/*
Purpose:
    Copies a face into another index without checking if indexes are inside
    range of face_num.
*/
bool IVCONV::face_copy(unsigned long dest_idx, unsigned long src_idx)
{
	face[dest_idx] = Face(face[src_idx]);
}
/**********************************************************************/

/*
Purpose:
  REDUCE_FACE_ORDER divides faces to smaller ones and reduces
   maximum face order

Discussion:
  It is easier to write the code than to explain how it works....
   This is a most simple way possible. It would be better if we
   choose the dividing line more wisely, but usually we're reducing
   from order 4 to order 3, and in this case method is no matter.
Note:
  After REDUCE_FACE_ORDER, faces area parameter should be recomputed.

	Created: 04 Mar 2007 Tomasz Lis
*/
bool IVCONV::reduce_face_order(unsigned int max_order)
{
	if (max_order < 3)
	{
		printf("REDUCE_FACE_ORDER: Warning, requested maximal face order %u is too small\n", max_order);
		return false;
	}
	printf("\n");
	printf("REDUCE_FACE_ORDER: sweeping through faces...\n");
	unsigned long face_idx;
	unsigned long face_num_base = face_num;
	bool show_face_info = (debug && (face_num < 4096));
	for (face_idx = 0; face_idx < face_num_base; face_idx++)
	{
		while (face[face_idx].order > max_order)
		{
			if (show_face_info)
				printf("REDUCE_FACE_ORDER: Dividing face %lu\n", face_idx);
			//Zero the face area - it will change after dividing
			face[face_idx].area = 0.0f;
			//Dividing face into two subfaces
			//Note: face[face_num] is a new face, face[face_idx] is the old one
			face_copy(face_num, face_idx);
			face[face_num].order = max_order;
			unsigned int src_order = face[face_idx].order;
			//First vertex will be same on both faces
			face[face_num].vertices[0] = face[face_idx].vertices[0];
			//The rest will be copied from end of old vertex
			for (int i = 1; i < max_order; i++)
			{
				int j = src_order + i - max_order;
				face[face_num].vertices[i] = face[face_idx].vertices[j];
			}
			//The order of source vertex is reduced by (max_order-2).
			src_order = face[face_idx].order - max_order + 2;
			face[face_idx].order = src_order;
			face_num++;
		}
	}
	printf("REDUCE_FACE_ORDER: order reduced to %u, %lu new faces created.\n",
			max_order, face_num - face_num_base);
	return SUCCESS;
}
/**********************************************************************/

/*
Purpose:

  reverse_faces reverses vertices arrays in faces

	Modified: 02 Mar 2007 Tomasz Lis
*/
bool IVCONV::reverse_faces()
{
	for (unsigned int iface = 0; iface < face_num; iface++)
	{
		unsigned int m = face[iface].order;
		for (unsigned int ivert = 0; ivert < m / 2; ivert++)
		{
			unsigned int jvert = m - ivert - 1;
			unsigned long vertidx = face[iface].vertices[ivert];
			face[iface].vertices[ivert] = face[iface].vertices[jvert];
			face[iface].vertices[jvert] = vertidx;

			//TL: maybe I don't understand what this function should do, but
			// I think if we'll revert vertices order and vertices data,
			// then nothing will be reverted. So I disabled this:
			//Vertex vertx = vertex[vertidx];
			//vertex[vertidx]=vertex[vertjdx];
			//vertex[vertjdx]=vertx;
		}
	}
	return SUCCESS;
}
/**********************************************************************/

/*
 Purpose:

 recompute_normals recomputes normal vectors in vertices and faces

 Modified: 02 Mar 2007 Tomasz Lis
 */
bool IVCONV::recompute_normals()
{
	for (unsigned int iface = 0; iface < face_num; iface++)
	{
		face[iface].normal = vec3(0.0, 0.0, 0.0);
	}

	for (unsigned int iface = 0; iface < face_num; iface++)
	{
		for (unsigned int ivert = 0; ivert < face[iface].order; ivert++)
		{
			unsigned long vertidx = face[iface].vertices[ivert];
			vertex[vertidx].normal = vec3(0.0, 0.0, 0.0);
		}
	}

	vertex_normal_set();
	face_normal_ave();
	return SUCCESS;
}

/*
Purpose:

  scale the scene (vertices, face normals and shapes)

	Modified: 02 Mar 2007 Tomasz Lis
*/
bool IVCONV::scale(float x, float y, float z)
{
	//Scale vertices
	for (unsigned long vertidx = 0; vertidx < vertex_num; vertidx++)
	{
		vertex[vertidx].cor3[0] = x * vertex[vertidx].cor3[0];
		vertex[vertidx].cor3[1] = y * vertex[vertidx].cor3[1];
		vertex[vertidx].cor3[2] = z * vertex[vertidx].cor3[2];
	}

	//Normals has changed - zero them
	for (unsigned int iface = 0; iface < face_num; iface++)
		face[iface].normal = vec3(0.0, 0.0, 0.0);
	for (unsigned long vertidx = 0; vertidx < vertex_num; vertidx++)
		vertex[vertidx].normal = vec3(0.0, 0.0, 0.0);

	//Scale shapes
	for (unsigned long shpidx = 0; shpidx < shape_num; shpidx++)
	{
		for (unsigned int coridx = 0; coridx < shape[shpidx].order; coridx++)
		{
			shape[shpidx].cor3[0][coridx] *= x;
			shape[shpidx].cor3[1][coridx] *= y;
			shape[shpidx].cor3[2][coridx] *= z;
		}
	}

	//recompute the area of each face
	face_area_set(false);
	//recompute normal vectors
	vertex_normal_set();
	face_normal_ave();
	vertex_cor3_range();
	return SUCCESS;
}

/******************************************************************************/

/*
Purpose:

  REDUCE_ITEM_NUMBERS divides the monolithic object into acceptably small pieces.

	Note:

	  Many format allows an unsigned short int for the number of
	  points, and number of faces in an object.  This limits such quantities
	  to 65535, or even less if some numbers are marked "special".
      If we have at least one object with more items than that, we need
      to cut come items from scene.

	Author: John Burkardt
	Modified: 14 Oct 1998
	Modified: 04 Mar 2007 Tomasz Lis
*/
bool IVCONV::reduce_item_numbers(unsigned long max_num)
{
	//TODO - for now there's only simplest way, should be extended.
	if (camera_num > max_num)
	{
		camera_num = max_num;
		if (debug)
			printf("    REDUCE_ITEM_NUMBERS: Warning, cameras count truncated to %lu\n", camera_num);
	}
	if (light_num > max_num)
	{
		light_num = max_num;
		if (debug)
			printf("    REDUCE_ITEM_NUMBERS: Warning, lights count truncated to %lu\n", light_num);
	}
	if (material_num > max_num)
	{
		material_num = max_num;
		if (debug)
			printf("    REDUCE_ITEM_NUMBERS: Warning, materials count truncated to %lu\n", material_num);
	}
	if (texture_num > max_num)
	{
		texture_num = max_num;
		if (debug)
			printf("    REDUCE_ITEM_NUMBERS: Warning, textures count truncated to %lu\n", texture_num);
	}
	if (mesh_num > max_num)
	{
		mesh_num = max_num;
		if (debug)
			printf("    REDUCE_ITEM_NUMBERS: Warning, meshes count truncated to %lu\n", mesh_num);
	}
	if (face_num > max_num)
	{
		face_num = max_num;
		if (debug)
			printf("    REDUCE_ITEM_NUMBERS: Warning, faces count truncated to %lu\n", face_num);
	}
	//Disabled, because in 3DS format the total number of vertives
	// may exceed 65535, if only every mesh has less than 65535
//  if (vertex_num>max_num)   vertex_num=max_num;
	if (shape_num > max_num)
	{
		shape_num = max_num;
		if (debug)
			printf("    REDUCE_ITEM_NUMBERS: Warning, shapes count truncated to %lu\n", shape_num);
	}

	return SUCCESS;
}
/******************************************************************************/
