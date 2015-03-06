/******************************************************************************/
// Author:  John Burkardt 
// Change: 08 Mar 2007 Tomasz Lis adjusted to new core
// Change: 31 Mar 2008 Tomasz Lis fixed and extended writing function
// Change: 02 Apr 2008 Tomasz Lis divided and fixed reading function

// A list of supported elements follows. Please modify if you'll enhance something.
// Format support note - reading:
// - 31-bit vertices support
// - 31-bit faces support
// - Meshes supported; not sure if more than one is ever used
// - Materials supported (but unfinished - not set to faces)
// - One texture per material supported
// - Texture UV supported (per-vertex)
// - No camera/view support
// - No Environment or lights support
// - Shapes/lines supported (untested)
// Format support note - writing:
// - Full 32-bit vertices support
// - Full 32-bit faces support
// - Meshes supported; not sure if they will work with Softimage tools
// - Materials supported
// - One texture per material supported
// - Texture UV supported (per-vertex)
// - No camera/view support
// - No Environment or lights support
// - Shapes/lines supported
/*
Examples:

HRCH: Softimage 4D Creative Environment v3.00


model
{
name         "cube_10x10"
scaling      1.000 1.000 1.000
rotation     0.000 0.000 0.000
translation  0.000 0.000 0.000

mesh
{
flag    ( PROCESS )
discontinuity  60.000

vertices   8
{
[0] position  -5.000  -5.000  -5.000
[1] position  -5.000  -5.000  5.000
[2] position  -5.000  5.000  -5.000
[3] position  -5.000  5.000  5.000
[4] position  5.000  -5.000  -5.000
[5] position  5.000  -5.000  5.000
[6] position  5.000  5.000  -5.000
[7] position  5.000  5.000  5.000
}

polygons   6
{
[0] nodes  4
{
[0] vertex  0
normal  -1.000  0.000  0.000
uvTexture  0.000  0.000
vertexColor 255 178 178 178
[1] vertex  1
normal  -1.000  0.000  0.000
uvTexture  0.000  0.000
vertexColor 255 178 178 178
[2] vertex  3
normal  -1.000  0.000  0.000
uvTexture  0.000  0.000
vertexColor 255 178 178 178
[3] vertex  2
normal  -1.000  0.000  0.000
uvTexture  0.000  0.000
vertexColor 255 178 178 178
}
material  0
[1] nodes  4
{
[0] vertex  1
normal  0.000  0.000  1.000
uvTexture  0.000  0.000
vertexColor 255 178 178 178
[1] vertex  5

...etc.....

[5] nodes  4
{
[0] vertex  2
normal  0.000  1.000  0.000
uvTexture  0.000  0.000
vertexColor 255 178 178 178
[1] vertex  3
normal  0.000  1.000  0.000
uvTexture  0.000  0.000
vertexColor 255 178 178 178
[2] vertex  7
normal  0.000  1.000  0.000
uvTexture  0.000  0.000
vertexColor 255 178 178 178
[3] vertex  6
normal  0.000  1.000  0.000
uvTexture  0.000  0.000
vertexColor 255 178 178 178
}
material  0
}

edges   12
{
[1] vertices  3  2
[2] vertices  2  0
[3] vertices  0  1
[4] vertices  1  3
[5] vertices  7  3
[6] vertices  1  5
[7] vertices  5  7
[8] vertices  6  7
[9] vertices  5  4
[10] vertices  4  6
[11] vertices  2  6
[12] vertices  4  0
}
}

material [0]
{
name           "kazoo"
type           PHONG
ambient        0.0  1.0  0.0
diffuse        1.0  0.0  0.0
specular       0.0  0.0  1.0
exponent      50.0
reflectivity   0.0
transparency   0.0
refracIndex    1.0
glow           0
coc            0.0
}

texture [0]
{
name          "/usr/users/foss/HOUSE/PICTURES/mellon"
glbname       "t2d1"
anim          STATIC
method        XY
repeat        1  1
scaling       1.000  1.000
offset        0.000  0.000
pixelInterp
effect        INTENSITY
blending      1.000
ambient       0.977
diffuse       1.000
specular      0.966
reflect       0.000
transp        0.000
roughness     0.000
reflMap       1.000
rotation      0.000
txtsup_rot    0.000  0.000  0.000
txtsup_trans  0.000  0.000  0.000
txtsup_scal   1.000  1.000  1.000
}
}
*/
//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#include "ivconv.h"

struct HRC_READER
{
    //current level in hierarchical file structure
    long level;
    //names of levels from top to current
    array<NameItm> llevel;
    //num of faces before the reading
    unsigned long face_num_old;
    unsigned long vertex_num_old;
	long mesh_idx;
	long face_idx;
	long shape_idx;
	long material_idx;
    //parameters for parsing line readed from file
	int   nlbrack;
	int   nrbrack;
	char  word[LINE_MAX_LEN];
    char *next;
    HRC_READER(){ level=0;llevel[0]=NameItm();
                  nlbrack=0;nrbrack=0;face_num_old=0;
                  mesh_idx=0; face_idx=-1; shape_idx=0; material_idx=0;
                  strcpy(word," ");next=word; };
};

/******************************************************************************/

int hrc_read_strword(const char *src,char *word)

/******************************************************************************/
{
    int count;
    int width;
    count = sscanf ( src, "%s%n", word, &width );
    if (count!=1)
        return 0;
    return width;
}

/******************************************************************************/

int hrc_read_float(const char *src,float *num)

/******************************************************************************/
{
    int count;
    int width;
    count = sscanf ( src, "%f%n", num, &width );
    if (count!=1)
        return 0;
    return width;
}

/******************************************************************************/

int hrc_read_vec3(const char *src,vec3 *v)

/******************************************************************************/
{
    float x,y,z;
    int width_all=0;
    int width;
    width = hrc_read_float(src+width_all,&x);
    if ( width <= 0 )
        return 0;
    width_all += width;
    width = hrc_read_float(src+width_all,&y);
    if ( width <= 0 )
        return 0;
    width_all += width;
    width = hrc_read_float(src+width_all,&z);
    if ( width <= 0 )
        return 0;
    width_all += width;
    (*v) = vec3(x,y,z);
    return width_all;
}

/******************************************************************************/

int hrc_read_vec2(const char *src,vec2 *v)

/******************************************************************************/
{
    float x,y;
    int width_all=0;
    int width;
    width = hrc_read_float(src+width_all,&x);
    if ( width <= 0 )
        return 0;
    width_all += width;
    width = hrc_read_float(src+width_all,&y);
    if ( width <= 0 )
        return 0;
    width_all += width;
    (*v) = vec2(x,y);
    return width_all;
}

/******************************************************************************/

bool IVCONV::hrc_read_controlpoints_property(struct HRC_READER &rd)

/******************************************************************************/
/*
Purpose:
  controlpoints is a part of spline block, containing point coordinates

*/
{
	int width;
    vec3 vt;
    if ( strcmp ( rd.word, "{" ) == 0 )
    {
    } else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
        rd.level = rd.nlbrack - rd.nrbrack;
    } else
    if ( rd.word[0] == '[' )
    {
    } else
    if ( stricmp( rd.word, "position" ) == 0 )
    {
        width = hrc_read_vec3(rd.next,&vt);
        if ( width <= 0 )
            return false;
        rd.next += width;
        int point_idx=shape[shape_num].order;
        if ( point_idx<SHAPE_ORDER_MAX )
        {
            shape[shape_num].cor3[point_idx]=vec3(vt);
            shape[shape_num].order++;
        } else
        {
            if (debug)
                printf ( "  CONTROLPOINTS: max shape order reached, point skipped.\n" );
        }
    } else
    {
        stats.bad_num++;
        printf ( "  CONTROLPOINTS: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false; //go to next line
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_edges_property(struct HRC_READER &rd)

/******************************************************************************/
{
	int   count;
	int   width;
	int   jval;
    if ( strcmp( rd.word, "{" ) == 0 )
    {
    } else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
        rd.level = rd.nlbrack - rd.nrbrack;
    } else
    if ( rd.word[0] == '[' )
    {
    } else
    if ( stricmp( rd.word, "vertices" ) == 0 )
    {
        count = sscanf ( rd.next, "%d%n", &jval, &width );
        rd.next += width;
        if (count<=0)
          return false;
        if ( shape_num >= SHAPES_MAX )
        {
            rd.shape_idx=SHAPES_MAX-1;
        } else
        {
            rd.shape_idx=shape_num;
            shape[rd.shape_idx]=Shape();
            shape_num++;
            shape[rd.shape_idx].mesh=rd.mesh_idx;
        }
        long vertex_idx;
        int point_idx;
        vertex_idx=jval+rd.vertex_num_old;
        point_idx=shape[shape_num].order;
        if ((vertex_idx<vertex_num)&&(point_idx<SHAPE_ORDER_MAX))
        {
            shape[shape_num].cor3[point_idx]=vec3(vertex[vertex_idx].cor3);
            shape[shape_num].order++;
        }
        count = sscanf ( rd.next, "%d%n", &jval, &width );
        rd.next += width;
        vertex_idx=jval+rd.vertex_num_old;
        point_idx=shape[shape_num].order;
        if ((vertex_idx<vertex_num)&&(point_idx<SHAPE_ORDER_MAX))
        {
            shape[shape_num].cor3[point_idx]=vec3(vertex[vertex_idx].cor3);
            shape[shape_num].order++;
        }
    } else
    {
        stats.bad_num++;
        printf ( "  EDGES: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false;
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_material_property(struct HRC_READER &rd)

/******************************************************************************/
{
	char  word2[LINE_MAX_LEN];
	int   width;
	float t;
    vec3 vt;
    if ( strcmp ( rd.word, "{" ) == 0 )
    {
        rd.material_idx=material_num;
        material[rd.material_idx]=Material();
        material_num++;
    } else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
        rd.level = rd.nlbrack - rd.nrbrack;
    } else
    if ( rd.word[0] == '[' )
    {
    } else
    if ( stricmp( rd.word, "ambient" ) == 0 )
    {
        vec3 vt;
        width = hrc_read_vec3(rd.next,&vt);
        if ( width <= 0 )
            return false;
        rd.next += width;
        material[rd.material_idx].ambient=vec3(vt);
    } else
    if ( stricmp( rd.word, "coc" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "diffuse" ) == 0 )
    {
        width = hrc_read_vec3(rd.next,&vt);
        if ( width <= 0 )
            return false;
        rd.next += width;
        material[rd.material_idx].rgb=vec3(vt);
    } else
    if ( stricmp( rd.word, "exponent" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "glow" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "name" ) == 0 )
    {
        width = hrc_read_strword(rd.next,word2);
        if ( width <= 0 )
            return false;
        rd.next += width;
        strcpy ( material[material_num-1].name, word2 );
    } else
    if ( stricmp( rd.word, "reflectivity" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "refracindex" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "specular" ) == 0 )
    {
        width = hrc_read_vec3(rd.next,&vt);
        if ( width <= 0 )
            return false;
        rd.next += width;
        material[material_num-1].specular=vec3(vt);
    } else
    if ( stricmp( rd.word, "transparency" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
        material[material_num-1].alpha = 1.0 - t;
    } else
    if ( stricmp( rd.word, "type" ) == 0 )
    {
        width = hrc_read_strword(rd.next,word2);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    {
        stats.bad_num++;
        printf ( "  MATERIAL: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false;
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_mesh_property(struct HRC_READER &rd)

/******************************************************************************/
{
	float t;
	int width;
    if ( strcmp ( rd.word, "{" ) == 0 )
    {
    } else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
        rd.level = rd.nlbrack - rd.nrbrack;
    } else
    if ( stricmp( rd.word, "discontinuity" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
        return false;
    } else
    if ( stricmp( rd.word, "edges" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "flag" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "polygons" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "vertices" ) == 0 )
    {
        rd.vertex_num_old = vertex_num;
        return false;
    } else
    {
        stats.bad_num++;
        printf ( "  MESH: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false;
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_model_property(struct HRC_READER &rd)

/******************************************************************************/
{
	char  word2[LINE_MAX_LEN];
	int   width;
    if ( strcmp ( rd.word, "{" ) == 0 )
    {
        rd.mesh_idx=mesh_num;
        mesh[rd.mesh_idx]=Mesh();
        mesh_num++;
    } else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
        rd.level = rd.nlbrack - rd.nrbrack;
    } else
    if ( stricmp( rd.word, "material" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "texture" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "mesh" ) == 0 )
    {
        rd.vertex_num_old = vertex_num;
        return false;
    } else
    if ( stricmp( rd.word, "name" ) == 0 )
    {
        width = hrc_read_strword(rd.next,word2);
        if ( width <= 0 )
            return false;
        rd.next += width;
        if (rd.mesh_idx>=0)
        {
            strncpy(mesh[rd.mesh_idx].name,word2,LINE_MAX_LEN);
            if (debug)
                printf ( "  MODEL: Mesh name \"%s\" set to mesh %d.\n", word2,rd.mesh_idx );
        } else
        {
            if (debug)
                printf ( "  MODEL: Mesh name \"%s\" defined, but no mesh.\n", word2 );
        }
    } else
    if ( stricmp( rd.word, "patch" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "rotation" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "scaling" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "spline" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "translation" ) == 0 )
    {
        return false;
    } else
    {
        stats.bad_num++;
        printf ( "  MODEL: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false;
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_nodes_property(struct HRC_READER &rd)

/******************************************************************************/
{
	int count;
	int width;
	int jval;
    vec2 vd;
    vec3 vt;
    if ( strcmp ( rd.word, "{" ) == 0 )
    {
        if (face_num>=FACES_MAX)
        {
            face_num=FACES_MAX-1;
        } else
        {
            rd.face_idx=face_num;
            face[rd.face_idx] = Face();
            face_num++;
            face[rd.face_idx].mesh=rd.mesh_idx;
        }
    } else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
        rd.level = rd.nlbrack - rd.nrbrack;
    } else
    if ( rd.word[0] == '[' )
    {
    } else
    if ( stricmp( rd.word, "normal" ) == 0 )
    {
        long ivert=face[rd.face_idx].order;
        width = hrc_read_vec3(rd.next,&vt);
        if ( width <= 0 )
            return false;
        rd.next += width;
        if (ivert>0)
        {
            unsigned long vertidx = face[face_num-1].vertices[ivert-1];
            vertex[vertidx].normal = vec3(vt);
        }
    } else
    if ( stricmp( rd.word, "uvTexture" ) == 0 )
    {
        long ivert=face[rd.face_idx].order;
        width = hrc_read_vec2(rd.next,&vd);
        if ( width <= 0 )
            return false;
        rd.next += width;
        if (ivert>0)
        {
            unsigned long vertidx = face[face_num-1].vertices[ivert-1];
            vertex[vertidx].tex_uv = vec2(vd);
        }
	} else
    if ( stricmp( rd.word, "vertex" ) == 0 )
    {
        long ivert=face[rd.face_idx].order;
        count = sscanf ( rd.next, "%d%n", &jval, &width );
        rd.next += width;
        if (ivert<ORDER_MAX)
        {
            face[face_num-1].order++;
            face[face_num-1].vertices[ivert] = jval+rd.vertex_num_old;
        }
					
    } else
    if ( stricmp( rd.word, "vertexColor" ) == 0 )
    {
        long ivert=face[rd.face_idx].order;
        int ca,cr,cg,cb;
        count = sscanf ( rd.next, "%d%n", &ca, &width );
        rd.next += width;
        count = sscanf ( rd.next, "%d%n", &cr, &width );
        rd.next += width;
        count = sscanf ( rd.next, "%d%n", &cg, &width );
        rd.next += width;
        count = sscanf ( rd.next, "%d%n", &cb, &width );
        rd.next += width;
        if (ivert>0)
        {
            unsigned long vertidx = face[face_num-1].vertices[ivert-1];
            vertex[vertidx].rgb = vec3(((float)cr)/255.0,((float)cg)/255.0,((float)cb)/255.0);
        }
    } else
    {
        stats.bad_num++;
        printf ( "  NODES: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false;
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_patch_property(struct HRC_READER &rd)

/******************************************************************************/
{
    // I don't know what to do with this yet.
    if ( strcmp ( rd.word, "{" ) == 0 )
    {
    } else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
        rd.level = rd.nlbrack - rd.nrbrack;
    } else
    if ( stricmp( rd.word, "approx_type" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "controlpoints" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "curv_u" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "curv_v" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "recmin" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "recmax" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "recursion" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "spacial" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "taggedpoints" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "ucurve" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "ustep" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "utension" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "utype" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "vclose" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "vcurve" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "viewdep" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "vpoint" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "vstep" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "vtension" ) == 0 ) {
    } else
    if ( stricmp( rd.word, "vtype" ) == 0 ) {
    } else
    {
        stats.bad_num++;
        printf ( "  PATCH: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false;
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_polygons_property(struct HRC_READER &rd)

/******************************************************************************/
{
	int   jval;
	int   ivert;
	int   width;
	int   count;
	char  word2[LINE_MAX_LEN];
	if ( strcmp ( rd.word, "{" ) == 0 )
    {
	} else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
		rd.level = rd.nlbrack - rd.nrbrack;
	} else
    if ( rd.word[0] == '[' )
    {
    } else
    if ( stricmp( rd.word, "material" ) == 0 )
    {
		count = sscanf ( rd.next, "%d%n", &jval, &width );
		rd.next += width;
					
		for ( ivert = 0; ivert < face[face_num-1].order; ivert++ )
		{
		    unsigned long vertidx = face[face_num-1].vertices[ivert];
		    vertex[vertidx].material = jval;
		}
					
	} else
    if ( stricmp( rd.word, "nodes" ) == 0 )
	{
		width = hrc_read_strword(rd.next,word2);
		if ( width <= 0 )
		    return false;
		rd.next += width;
	} else
	{
		stats.bad_num++;
		printf ( "  POLYGONS: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
		return false;
	}
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_spline_property(struct HRC_READER &rd)

/******************************************************************************/
{
	int   width;
	char  word2[LINE_MAX_LEN];
    if ( strcmp ( rd.word, "{" ) == 0 )
    {
        if ( shape_num >= SHAPES_MAX )
        {
            rd.shape_idx=SHAPES_MAX-1;
        } else
        {
            rd.shape_idx=shape_num;
            shape[rd.shape_idx]=Shape();
            shape_num++;
            shape[rd.shape_idx].mesh=rd.mesh_idx;
        }
    } else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
        rd.level = rd.nlbrack - rd.nrbrack;
    } else
    if ( stricmp( rd.word, "controlpoints" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "name" ) == 0 )
    {
        width = hrc_read_strword(rd.next,word2);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "nbKeys" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "step" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "tension" ) == 0 )
    {
        return false;
    } else
    if ( stricmp( rd.word, "type" ) == 0 )
    {
        return false;
    } else
    {
        stats.bad_num++;
        printf ( "  SPLINE: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false;
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_taggedpoints_property(struct HRC_READER &rd)

/******************************************************************************/
{
    if ( strcmp ( rd.word, "{" ) == 0 )
    {
    } else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
        rd.level = rd.nlbrack - rd.nrbrack;
    } else
    if ( rd.word[0] == '[' )
    {
    } else
    if ( stricmp( rd.word, "tagged" ) == 0 )
    {
    } else
    {
        stats.bad_num++;
        printf ( "  TAGGEDPOINTS: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false;
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_texture_property(struct HRC_READER &rd)

/******************************************************************************/
{
	char  word2[LINE_MAX_LEN];
    vec2 vd;
    vec3 vt;
    float t;
    int width;
    if ( strcmp ( rd.word, "{" ) == 0 )
    {
	    texture_num++;
    } else
    if ( strcmp ( rd.word, "}" ) == 0 )
    {
        rd.level = rd.nlbrack - rd.nrbrack;
    } else
    if ( rd.word[0] == '[' )
    {
    } else
    if ( stricmp( rd.word, "ambient" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "anim" ) == 0 )
    {
        width = hrc_read_strword(rd.next,word2);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "blending" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "diffuse" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "effect" ) == 0 )
    {
        width = hrc_read_strword(rd.next,word2);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "glbname" ) == 0 )
    {
        width = hrc_read_strword(rd.next,word2);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "method" ) == 0 )
    {
        width = hrc_read_strword(rd.next,word2);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "name" ) == 0 )
    {
        width = hrc_read_strword(rd.next,word2);
        if ( width <= 0 )
            return false;
        rd.next += width;
        strcpy ( texture[texture_num-1].name, word2 );
    } else
    if ( stricmp( rd.word, "offset" ) == 0 )
    {
        width = hrc_read_vec2(rd.next,&vd);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "pixelinterp" ) == 0 )
    {
    } else
    if ( stricmp( rd.word, "reflect" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "reflmap" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "repeat" ) == 0 )
    {
    } else
    if ( stricmp( rd.word, "rotation" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "roughness" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "scaling" ) == 0 )
    {
        width = hrc_read_vec2(rd.next,&vd);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "specular" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "transp" ) == 0 )
    {
        width = hrc_read_float(rd.next,&t);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "txtsup_rot" ) == 0 )
    {
        width = hrc_read_vec3(rd.next,&vt);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "txtsup_scal" ) == 0 )
    {
        width = hrc_read_vec3(rd.next,&vt);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    if ( stricmp( rd.word, "txtsup_trans" ) == 0 )
    {
        width = hrc_read_vec3(rd.next,&vt);
        if ( width <= 0 )
            return false;
        rd.next += width;
    } else
    {
        stats.bad_num++;
        printf ( "  TEXTURE: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false;
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_read_vertices_property(struct HRC_READER &rd)

/******************************************************************************/
{
	int width;
    vec3 vt;
	if ( strcmp ( rd.word, "{" ) == 0 )
	{
	} else
	if ( strcmp ( rd.word, "}" ) == 0 )
	{
		rd.level = rd.nlbrack - rd.nrbrack;
	} else
	if ( rd.word[0] == '[' )
	{
	} else
    if ( stricmp( rd.word, "position" ) == 0 )
    {
		width = hrc_read_vec3(rd.next,&vt);
		if ( width <= 0 )
			return false;
        rd.next += width;
        if ( vertex_num < VERTICES_MAX ) 
        {
            vertex[vertex_num]=Vertex();
            vertex[vertex_num].cor3 = vec3(vt);
            vertex_num++;
        }
    } else
    {
        stats.bad_num++;
        printf ( "  VERTICES: Bad data \"%s\" on line %d; error ignored.\n", rd.word,stats.text_num);
        return false;
    }
    return true;
}

/******************************************************************************/

bool IVCONV::hrc_write_mesh ( FileIO *fileout, unsigned long mesh_idx )

/******************************************************************************/
/*
Purpose:

HRC_WRITE_MESH writes a single mesh (object) into HRC file.

Author: John Burkardt
Modified: 31 Mar 2008 Tomasz Lis extracted from HRC_WRITE and fixed
*/
{

	fileout->fprintf ( "\n" );
	fileout->fprintf ( "model\n" );
	fileout->fprintf ( "{\n" );
	stats.text_num += 3;

	fileout->fprintf ( "  name         \"%s\"\n", mesh[mesh_idx].name );
	fileout->fprintf ( "  scaling      %.3f %.3f %.3f\n",1.000,1.000,1.000 );
	fileout->fprintf ( "  rotation     %.3f %.3f %.3f\n",0.000,0.000,0.000 );
	fileout->fprintf ( "  translation  %.3f %.3f %.3f\n",0.000,0.000,0.000 );
	stats.text_num += 4;

    //Preparing mesh to save
    array<unsigned long> vert_idx_arr;
    unsigned long vert_count;
    array<long> nvert_idx;
    unsigned long faces_count;
    get_mesh_vertices(vert_idx_arr,vert_count,nvert_idx,faces_count,mesh_idx);
	
	if ( faces_count > 0 )
    {
		fileout->fprintf ( "\n" );
		fileout->fprintf ( "  mesh\n" );
		fileout->fprintf ( "  {\n" );
		fileout->fprintf ( "    flag    ( %s )\n","PROCESS" );
		fileout->fprintf ( "    discontinuity  %.3f\n",60.000 );
		stats.text_num += 5;

		// Point coordinates.
		if ( vert_count > 0 )
        {
			fileout->fprintf( "\n" );
			fileout->fprintf( "    vertices %d\n", vert_count );
			fileout->fprintf( "    {\n" );
			stats.text_num += 3;
			
            int j;
			for ( j = 0; j < vert_count; ++j )
            {
				fileout->fprintf( "      [%d] position %f %f %f\n", j,
                  vertex[vert_idx_arr[j]].cor3[0],
                  vertex[vert_idx_arr[j]].cor3[1],
                  vertex[vert_idx_arr[j]].cor3[2] );
				stats.text_num++;
			}
			fileout->fprintf( "    }\n" );
			stats.text_num++;
		}

		// Faces.
		fileout->fprintf( "\n" );
		fileout->fprintf( "    polygons %d\n", faces_count );
		fileout->fprintf( "    {\n" );
		stats.text_num += 3;
		
        unsigned long face_idx;
        unsigned long iface;
        iface=0;
        for (face_idx=0;face_idx<face_num;face_idx++)
        {
            if (face[face_idx].mesh!=mesh_idx)
                continue;
			fileout->fprintf( "      [%d] nodes %d\n", iface, face[face_idx].order );
			fileout->fprintf( "      {\n" );
			stats.text_num += 2;

            int ivert;
			for ( ivert = 0; ivert < face[face_idx].order; ivert++ )
            {
                unsigned long vertidx = face[face_idx].vertices[ivert];
				
				fileout->fprintf( "        [%d] vertex %d\n", ivert, nvert_idx[vertidx] );
				fileout->fprintf( "            normal %f %f %f\n", 
					vertex[vertidx].normal[0], 
					vertex[vertidx].normal[1],
                    vertex[vertidx].normal[2] );
				fileout->fprintf( "            uvTexture  %f %f\n", 
					vertex[vertidx].tex_uv[0], vertex[vertidx].tex_uv[1] );
				fileout->fprintf( "            vertexColor  %d %d %d %d\n",255,178,178,178 );
				stats.text_num += 4;
			}
            fileout->fprintf( "      }\n" );
			fileout->fprintf( "      material %d\n", get_face_major_material(face_idx) );
			stats.text_num += 2;
			iface++;
		}
		fileout->fprintf( "    }\n" );
		fileout->fprintf( "  }\n" );
		stats.text_num += 2;
	}

	// IndexedLineSet
    int nseg;
    nseg = 0;
	unsigned long shape_idx;
    for (shape_idx=0;shape_idx<shape_num;shape_idx++)
    {
        if (shape[shape_idx].mesh!=mesh_idx)
            continue;

        fileout->fprintf( "\n" );
        fileout->fprintf( "  spline\n" );
        fileout->fprintf( "  {\n" );
        fileout->fprintf( "    name     \"spl%d\"\n", nseg );
        fileout->fprintf( "    type     %s\n","LINEAR" );
        fileout->fprintf( "    nbKeys   %d\n", shape[shape_idx].order );
        fileout->fprintf( "    tension  %.3f\n",0.000 );
        fileout->fprintf( "    step     %d\n",1 );
        fileout->fprintf( "\n" );
        stats.text_num += 9;
			
        fileout->fprintf( "    controlpoints\n" );
        fileout->fprintf( "    {\n" );
        stats.text_num += 2;
        unsigned long point_idx;
        for (point_idx=0;point_idx<shape[shape_idx].order;point_idx++)
        {
            vec3 coords=vec3(shape[shape_idx].cor3[point_idx]);
            fileout->fprintf( "      [%d] position %f %f %f\n", point_idx,
              coords[0],coords[1],coords[2]);
            stats.text_num++;
        }
        fileout->fprintf( "    }\n" );
        fileout->fprintf( "  }\n" );
        stats.text_num += 2;
        nseg++;
	}

	// MATERIALS
	for ( int i = 0; i < material_num; ++i )
    {
		fileout->fprintf( "  material [%d]\n", i );
		fileout->fprintf( "  {\n" );
		fileout->fprintf( "    name           \"%s\"\n", material[i].name );
		fileout->fprintf( "    type           %s\n","PHONG" );
		fileout->fprintf( "    ambient        %f %f %f\n", material[i].ambient[0],material[i].ambient[1],material[i].ambient[2]);
		fileout->fprintf( "    diffuse        %f %f %f\n", material[i].rgb[0],material[i].rgb[1],material[i].rgb[2]);
		fileout->fprintf( "    specular       %f %f %f\n", material[i].specular[0],material[i].specular[1],material[i].specular[2]);
		fileout->fprintf( "    exponent      %f\n",50.0 );
		fileout->fprintf( "    reflectivity   %f\n",0.0 );
		fileout->fprintf( "    transparency   %f\n", 1.0 - material[i].alpha );
		fileout->fprintf( "    refracIndex    %f\n",1.0 );
		fileout->fprintf( "    glow           %d\n",0 );
		fileout->fprintf( "    coc            %f\n",0.0 );
		fileout->fprintf( "  }\n" );
		stats.text_num+=14;
	}

	// TEXTURES
	for (int i = 0; i < texture_num; ++i )
    {
		fileout->fprintf( "  texture [%d]\n", i );
		fileout->fprintf( "  {\n" );
		fileout->fprintf( "    name           \"%s\"\n", texture[i].name );
		fileout->fprintf( "    glbname        \"t2d1\"\n" );
		fileout->fprintf( "    anim           %s\n","STATIC" );
		fileout->fprintf( "    method         %s\n","XY" );
		fileout->fprintf( "    repeat         %d %d\n",1,1 );
		fileout->fprintf( "    scaling        %.3f %.3f\n",1.000,1.000 );
		fileout->fprintf( "    offset         %.3f %.3f\n",0.000,0.000 );
		fileout->fprintf( "    pixelInterp\n" );
		fileout->fprintf( "    effect         %s\n","INTENSITY" );
		fileout->fprintf( "    blending       %.3f\n",1.000 );
		fileout->fprintf( "    ambient        %.3f\n",0.977 );
		fileout->fprintf( "    diffuse        %.3f\n",1.000 );
		fileout->fprintf( "    specular       %.3f\n",0.966 );
		fileout->fprintf( "    reflect        %.3f\n",0.000 );
		fileout->fprintf( "    transp         %.3f\n",0.000 );
		fileout->fprintf( "    roughness      %.3f\n",0.000 );
		fileout->fprintf( "    reflMap        %.3f\n",1.000 );
		fileout->fprintf( "    rotation       %.3f\n",0.000 );
		fileout->fprintf( "    txtsup_rot     %.3f  %.3f  %.3f\n",0.000,0.000,0.000 );
		fileout->fprintf( "    txtsup_trans   %.3f  %.3f  %.3f\n",0.000,0.000,0.000 );
		fileout->fprintf( "    txtsup_scal    %.3f  %.3f  %.3f\n",1.000,1.000,1.000 );
		fileout->fprintf( "  }\n" );
		stats.text_num += 25;
	}
	fileout->fprintf( "}\n" );
	stats.text_num++;
}

/******************************************************************************/

bool IVCONV::hrc_read ( FileIO *filein )

/******************************************************************************/
/*
Purpose:
  HRC_READ reads graphics information from a SoftImage HRC file.

Author: John Burkardt
Modified: 25 June 1999
*/
{
    struct HRC_READER rd=HRC_READER();

	int   count;
	int   width;
	int   iword;
	char  word1[LINE_MAX_LEN];
	char  word2[LINE_MAX_LEN];
	char  wordm1[LINE_MAX_LEN];
	
	strcpy ( rd.llevel[0].name, "Top" );
	rd.nlbrack = 0;
	rd.nrbrack = 0;
	rd.face_num_old = face_num;
	rd.vertex_num_old = vertex_num;
	strcpy ( rd.word, " " );
	strcpy ( wordm1, " " );

	// Read a line of text from the file.
	for ( ;; )
    {
		char input[LINE_MAX_LEN];
		if ( filein->fgets(input,LINE_MAX_LEN) == NULL )
			break;
		stats.text_num++;
		rd.next = input;
		iword = 0;

		// Read a word from the line.
		for ( ;; )
        {
			strcpy ( wordm1, rd.word );
			width = hrc_read_strword(rd.next,word2);
			if ( width <= 0 )
				break;
			rd.next += width;
			strcpy ( rd.word, word2 );
			++iword;
			if ( iword == 1 )
				strcpy ( word1, rd.word );

			// The first line of the file must be the header.
			if ( stats.text_num == 1 )
            {
				if ( stricmp( word1, "HRCH:" ) != 0 )
                {
                    printf ( "\n" );
                    printf ( "HRC_READ - Fatal error!\n" );
                    printf ( "  The input file has a bad header, or it's not HRC file.\n" );
					return false;
				} else
                {
					stats.comment_num++;
				}
				break; //stop processing the line
			}

			// If the word is a curly bracket, count it.
			if ( strcmp ( rd.word, "{" ) == 0 )
            {
				rd.nlbrack++;
				rd.level = rd.nlbrack - rd.nrbrack;
				strcpy ( rd.llevel[rd.level].name, wordm1 );
				if ( debug )
					printf ( "  New level: %s\n", rd.llevel[rd.level].name );
			} else
			if ( strcmp ( rd.word, "}" ) == 0 )
            {
				rd.nrbrack++;
				
				if ( rd.nlbrack < rd.nrbrack )
                {
                    printf ( "\n" );
                    printf ( "HRC_READ - Fatal error!\n" );
					printf ( "  Extraneous right bracket on line %d.\n", stats.text_num );
					printf ( "  Currently processing field %s\n.", rd.llevel[rd.level].name );
					return false;
				}
			}
			// CONTROLPOINTS
			if ( stricmp( rd.llevel[rd.level].name, "controlpoints" ) == 0 )
            {
                if (!hrc_read_controlpoints_property(rd))
                    break;
			} else
			// EDGES
			if ( stricmp( rd.llevel[rd.level].name, "edges" ) == 0 )
            {
                if (!hrc_read_edges_property(rd))
                    break;
			} else
			// MATERIAL
			if ( stricmp( rd.llevel[rd.level].name, "material" ) == 0 )
            {
                if (!hrc_read_material_property(rd))
                    break;
			} else
			// MESH
			if ( stricmp( rd.llevel[rd.level].name, "mesh" ) == 0 )
            {
                if (!hrc_read_mesh_property(rd))
                    break;
			} else
			// MODEL
			if ( stricmp( rd.llevel[rd.level].name, "model" ) == 0 )
            {
                if (!hrc_read_model_property(rd))
                    break;
			} else
			// NODES
			if ( stricmp( rd.llevel[rd.level].name, "nodes" ) == 0 )
            {
                if (!hrc_read_nodes_property(rd))
                    break;
			} else
			// PATCH
			if ( stricmp( rd.llevel[rd.level].name, "patch" ) == 0 )
            {
                if (!hrc_read_patch_property(rd))
                    break;
			} else
			// POLYGONS
			if ( stricmp( rd.llevel[rd.level].name, "polygons" ) == 0 )
            {
                if (!hrc_read_polygons_property(rd))
                    break;
			} else
			// SPLINE
			if ( stricmp( rd.llevel[rd.level].name, "spline" ) == 0 )
            {
                if (!hrc_read_spline_property(rd))
                    break;
			} else
			// TAGGEDPOINTS
			if ( stricmp( rd.llevel[rd.level].name, "taggedpoints" ) == 0 )
            {
                if (!hrc_read_taggedpoints_property(rd))
                    break;
			} else
			// TEXTURE
			if ( stricmp( rd.llevel[rd.level].name, "texture" ) == 0 )
            {
                if (!hrc_read_texture_property(rd))
                    break;
			} else
			// VERTICES
			if ( stricmp( rd.llevel[rd.level].name, "vertices" ) == 0 )
            {
                if (!hrc_read_vertices_property(rd))
                    break;
			} else
			// Any other word:
			{
			    //Note: an error message is not required here, as it was shown
                // when the section start was readed.	
                if (debug)
					printf ( "HRC_READ: Skipped word \"%s\" in level \"%s\"\n",
                        rd.word,rd.llevel[rd.level].name );
			}
        }  // end of words loop
    } // end of lines loop
  // End of information in file.
    return true;
}
/******************************************************************************/

bool IVCONV::hrc_write ( FileIO *fileout )

/******************************************************************************/
/*
Purpose:
  HRC_WRITE writes graphics data to an HRC SoftImage file.

Author: John Burkardt
Modified: 25 Jun 1998
*/
{
	stats.text_num = 0;
	fileout->fprintf ( "HRCH: Softimage 4D Creative Environment v3.00\n" );
	fileout->fprintf ( "\n" );
	stats.text_num += 2;
    unsigned long mesh_idx;
	for (mesh_idx=0;mesh_idx<mesh_num;mesh_idx++)
	{
        hrc_write_mesh( fileout, mesh_idx );
    }
	// Report.
	printf ( "\n" );
	printf ( "HRC_WRITE - Wrote %d text lines.\n", stats.text_num );
	return true;
}
/******************************************************************************/
