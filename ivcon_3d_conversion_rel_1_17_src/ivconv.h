/******************************************************************************/
// Author:  John Burkardt 

// Purpose:
//   Header file for IVCON.CPP, defining class IVCON with all 3D scene elements.

//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/
#ifndef _ivcon_ivconv
#define _ivcon_ivconv

#include "tmat.h"	// transform matrix
#include "utils.h"	// various utilities

#include <string>
#include <math.h>
#include <assert.h>

using std::string;

//Defines to enable/disable supported formats (undef to disable)
#define _ivcon_use_tds
#define _ivcon_use_asc
#define _ivcon_use_ase
#define _ivcon_use_byu
#define _ivcon_use_dxf
#define _ivcon_use_gmod
#define _ivcon_use_hrc
#define _ivcon_use_iv
#define _ivcon_use_obj
#define _ivcon_use_pov
#define _ivcon_use_smf
#define _ivcon_use_stl
#define _ivcon_use_stlb
#define _ivcon_use_swv
#define _ivcon_use_swm
#define _ivcon_use_tec
#define _ivcon_use_tri
#define _ivcon_use_trib
#define _ivcon_use_txt
#define _ivcon_use_ucd
#define _ivcon_use_vla
#define _ivcon_use_wrl
#define _ivcon_use_xgl

#define _ivcon_version "1.17 TL (19 Apr 2008)"

//This one should be defined in MATH.H, but it is not (I don't know why)
#ifndef isnan
#define isnan(x) (sizeof (x) == sizeof (float) ? __isnanf (x)	\
		  : sizeof (x) == sizeof (double) ? __isnan (x)	\
		  : __isnanl (x))
#endif

//Constants with default names
const char DEFAULT_TEXTURE_NAME[]="tex";
const char DEFAULT_MESH_NAME[]="msh";
const char DEFAULT_MATERIAL_NAME[]="mat";
const char DEFAULT_LIGHT_NAME[]="lgt";
const char DEFAULT_CAMERA_NAME[]="cam";
const char DEFAULT_ITEM_NAME[]="itm"; //we use this default name if we don't know
                                         //exacly what we're reading

//Limits of various values - to prevent hanging
//const unsigned int  LINE_MAX_LEN=256;    // this is defined in UTIL unit
//const unsigned int  FNAME_LEN=511;       // this is defined in UTIL unit
const unsigned long SHAPES_MAX=65535;    // maximal shapes(lines) count
const unsigned int  SHAPE_ORDER_MAX=32767;// maximum number of points per shape
const unsigned long VERTICES_MAX=1048575; // maximal vertices count (may be >65535)
const unsigned long FACES_MAX=1048575;    // maximal vertices count (may be >65535)
const unsigned int  ORDER_MAX=15;        // maximum number of vertices per face
const unsigned int  MATERIAL_MAX=1023;   // maximum number of materials and textures

//Environment defines visual properties of a background
struct Environment
{
	vec3 rgb;	              // background color
	vec3 ambient_light;       // default (ambient) light color and intensivity
	int texture;              // background texture
	Environment(){rgb=vec3(.0,.0,.0);ambient_light=vec3(.5,.5,.5); texture=-1; };
};

//Environment defines visual properties of a background
struct Light
{
	char  name[LINE_MAX_LEN]; // text name for light
	vec3  rgb;	              // light color
	float intensity;          // light intensity
	vec3  cor3;               // light position
	bool shadows;             //enables casting shadows
	bool directional;          //type of light: directional or isotropic
	vec3  target;             // stores light direction
	Light(){rgb=vec3(.0,.0,.0); intensity=1.0; cor3=vec3(.0,.0,.0);target=vec3(.0,.0,.0);
      directional=false;shadows=false;sprintf(name,"%s",DEFAULT_LIGHT_NAME);};
};
//Camera defines cameras and views
struct Camera
{
	char  name[LINE_MAX_LEN];  // text name for camera
	vec3  origin;
	vec3  pivot;
	Camera(){origin=vec3(.0,.0,.0); pivot=vec3(.0,.0,.0);sprintf(name,"%s",DEFAULT_CAMERA_NAME); };
};

//Material defines visual properties of a face. It may use textures
struct Material
{
	char name[LINE_MAX_LEN];  // Unique text name for material
	vec3 ambient;             // Color in ambient light
	vec3 rgb;	              // diffuse : the "real" material color
	float shiness;            // shiness level
	vec3 specular;            // Color of direct light reflection, usually bright
	float alpha;              // Object opacity (alpha channel)
	int texture;              // Index of used texture, -1 means none
	int mask_texture;         // Index of texture mask (opacity map)
	int bump_texture;         // Index of bump texture, -1 means none
	Material(float r, float g, float b, float a=0.0)
    { rgb=vec3(r,g,b); ambient=rgb; specular=rgb; shiness=0.1; alpha=a; texture=-1; mask_texture=-1; bump_texture=-1; sprintf(name,"%s",DEFAULT_MATERIAL_NAME); };
	Material(){rgb=vec3(.5,.5,.5); ambient=rgb; specular=rgb; shiness=0.1; alpha=0.0; texture=-1; mask_texture=-1; bump_texture=-1; sprintf(name,"%s",DEFAULT_MATERIAL_NAME); };
};

//Textures are external files containing bitmaps used in materials
struct Texture
{
	char name[LINE_MAX_LEN];  //texture file name
	float intense;              //texture transparency channel, or its effect intense
    Texture(){intense=1.0;sprintf(name,"%s",DEFAULT_TEXTURE_NAME); };
};

//Mesh is a bunch of faces treated as one element
struct Mesh
{
	char name[LINE_MAX_LEN];    // text name for Mesh
	bool shadows;
    Mesh(){sprintf(name,"%s",DEFAULT_MESH_NAME);shadows=true; };
};

//Face is a single polygon made of some points(vertices) in 3D space
struct Face
{
	unsigned int order;             // the number of vertices in face (ie 3=triangular face)
	array<unsigned long> vertices;	// contains indexes of points (vertices) making up the face
	unsigned int mesh;              // index of Mesh this face belong to
	vec3 normal;                    // the face normal vector; may be computed automatically
	float  area;	                // the area of face; may be computed automatically
	unsigned int flags;             // flags defining face properties
	unsigned int smooth;       // smoothness definition
    Face(){ order=0;mesh=0;normal=vec3(0.0,0.0,0.0);area=0.0;flags=0;smooth=0; };
    Face(const Face &src){ order=src.order;mesh=src.mesh;normal=vec3(src.normal);area=src.area;flags=src.flags;smooth=src.smooth;vertices=src.vertices; };
};

// Vertex is a point in 3D space
struct Vertex
{
    vec3 cor3;                      //vertex coordinates
	unsigned int material;          // the material of vertices of face
	vec2 tex_uv;                    // texture coordinates of vertex
	vec3 normal;                    // normal vector at vertices of face; may be computed automatically
	vec3 rgb;                       // colors of vertex
    Vertex(){ material=0;cor3=vec3(0.0,0.0,0.0);normal=vec3(0.0,0.0,0.0);
            rgb=vec3(0.7,0.7,0.7);tex_uv=vec2(0.0,0.0); };
    Vertex(const Vertex &src){ material=src.material;cor3=vec3(src.cor3);normal=vec3(src.normal);
            rgb=vec3(src.rgb);tex_uv=vec2(src.tex_uv); };
    bool operator==(const Vertex& __other) const
      { return (cor3==__other.cor3)     && (material==__other.material) &&
		       (tex_uv==__other.tex_uv) && (normal==__other.normal) &&
		       (rgb==__other.rgb); }
};

// Shape is a line or curve. It don't need to be closed. It is usually invisible.
struct Shape
{
	unsigned int order;             // the number of points in shape
    array<vec3> cor3;               //shape points coordinates
	unsigned int mesh;              // index of Mesh this shape belong to
	vec3 rgb;                       // color of shape
//	int	dex;		// node indices, denoting polylines, each terminated by -1.
	                        // should not be used! i just cat't remove it now
    Shape(){ order=0;mesh=0;rgb=vec3(0.9,0.9,0.9);};
};

// Structure to create list of colors, useful when reading some formats
struct ColorItm
{
	vec3 rgb;
};

// Structure to read UV mapping "per face", for conversion to "per vertex" mapping
struct FaceUV
{
	unsigned int order;             // number of vertices mapped
	unsigned int material;          // material index of the face
	array<vec2> tex_uv;             // texture coordinates associated with face
    FaceUV(){ order=0;material=0;tex_uv[0]=vec2(0.0,0.0); };
};

struct Options
{
	unsigned char line_prune;    //line prunning on faces-to-lines conversion
	unsigned char fast_mode;
	float tiny_face_area;        //areas smaller that this part of largest area will be deleted
    Options(){ line_prune=1;tiny_face_area=0.0001;fast_mode=false; };
};

struct Stats
{
	unsigned int max_order;        //vertices per face, maximum
	unsigned int text_num;         //number of text lines readed/written (when working with text file)
	unsigned int bad_num;
	unsigned int bytes_num;
	unsigned int comment_num;
	unsigned int dup_num;
    Stats(){ max_order=0;text_num=0;bad_num=0;bytes_num=0;comment_num=0;dup_num=0; };
};

// Structures for reading/writing specific formats
struct ASE_READER;
struct IV_READER;
class CVehiclesReader;
class CMapReader;

// The main converter class
class IVCONV	// c++ wrapper around ivcon.c
{
public:	// needed for library use
    // class constructor;
    IVCONV();
    // reads file of given name, clearing all previous data;
    bool read(const string& file);
    // appends file of given name to previously read data;
    bool append(const string& file);
    // writes all scene data into specified file;
    bool write(const string& file);

public:	// needed for interactive use (main.cpp)

    // returns value of protected variable face_num;
    unsigned int get_faces_count() const {return face_num;}
    // prints information about a face;
    bool face_print(unsigned long iface);
    // shows report about 3D data in current scene;
    void data_report(bool show_comments);

    // inits variables and clears any stored scene data;
    void data_init();
    // converts faces to lines (required for some formats);
    bool face_to_line(unsigned char line_prune);
    // replaces scene with a subset of faces;
    bool face_subset(unsigned int iface1,unsigned int iface2);
    // reverses normal vectors direction;
    bool reverse_normals();
    // reverses all faces direction;
    bool reverse_faces();
    // reverses face order (first becomes last);
    bool face_reverse_order();
    // divides faces to smaller ones and reduces maximum face order;
    bool reduce_face_order(unsigned int max_order);
    // scales the whole scene;
    bool scale(float x, float y, float z);
    // recomputes normals in faces and vertices;
    bool recompute_normals();
    // renames meshes into given pattern
    bool rename_meshes(char *name_pattern);

protected: // "high level"
    // inits variables;
    void init_program_data();

  // DISK FILES RELATED
    // writes scene data; gets filename from internal variable;
    bool data_write();
    // reads scene data; gets filename from internal variable;
    bool data_read();
    // returns true if a file type is binary, false if it is text format;
    bool is_binary_format(char *file_ext);
    //Sets the prefix of default mesh/texture text names, using input file name
    void make_default_name_prefix();

  // SHAPES RELATED
    // converts line vertices (returned by some format readers) to shape coords;
    void line_vertices_to_shape_coords(array<Vertex> &lnvert,unsigned long lnvert_num,unsigned long start_shape);
    // converts shape coords to line vertices; required by some format writers;
    void shape_coords_to_line_vertices(array<Vertex> &lnvert,unsigned long &lnvert_num,unsigned long start_shape);

  // FACES RELATED
    // erases elements from all arrays to make them smaller than max_num;
    bool reduce_item_numbers(unsigned long max_num);
    // computes and sets areas of faces;
    void face_area_set(bool delete_small_faces);
    // copies a face into another index;
    bool face_copy(unsigned long dest_idx,unsigned long src_idx);

  // VERTICES RELATED
    // searches for vertex identical to vert, returnes its index in vert_idx if found;
    bool find_identical_vertex(unsigned long &vert_idx,Vertex *vert);
    // analyzes mesh and return its number of faces, number of vertices and the vertex indexes array;
    bool get_mesh_vertices(array<unsigned long> &vert_idx,unsigned long &vert_count,array<long> &nvert_idx,unsigned long &faces_count,unsigned long mesh_idx);
    // analyzes all faces and returnes array of unassigned vertices;
    bool get_free_vertices(array<unsigned long> &vert_idx,unsigned long &vert_count);

  // MATERIALS RELATED
    // assigns materials to objects that do not have a valid one;
    void materials_fixups();
    // returns index of the material with given name, or -1 on error;
    int get_material_index(char *name);
    // returns index of the material with given name, or creates new material with this name;
    unsigned int get_material_index_or_create(char *name);
    // converts face materials names (returned by some format readers) to material indexes;
    void face_materials_to_indexes(array<NameItm> &face_material_names,array<FaceUV> &face_uv,unsigned long coords_num);
    // converts materials from faces to vertices;
    bool face_to_vertex_material(array<FaceUV> &face_uv,unsigned long start_face,unsigned long coords_num);
    // converts materials from vertices to faces;
    bool vertex_to_face_material(array<FaceUV> &face_uv,unsigned long start_face,unsigned long coords_num);
    // returns material used in most of face vertices;
    unsigned long get_face_major_material(unsigned long face_idx);

  // UV MAPPING RELATED
    // converts UV mapping coords from faces to vertices, multiplying vertices if needed;
    bool face_to_vertex_uv(array<FaceUV> &face_uv,unsigned long start_face,unsigned long coords_num);
    // converts UV mapping coords from vertices to faces;
    bool vertex_to_face_uv(array<FaceUV> &face_uv,unsigned long start_face,unsigned long coords_num);
    // creates empty (or clears) UV coordinates for faces; returns length of face_uv in coords_num;
    bool clear_faces_uv(array<FaceUV> &face_uv,unsigned long start_face,unsigned long &coords_num);
    // clears UV coordinates for vertices;
    bool clear_vertices_uv();

  // COLORS RELATED
    // reads colors from list of vertices "vert_idx" into separate list "colr", merging same colors;
    bool vertex_colors_to_colist(array<ColorItm> &colr,array<unsigned long> &vert_cidx,unsigned long &colr_num,array<unsigned long> vert_idx,unsigned long vert_count);

  // OTHER
    // checks if there are no errors in the scene texture; checks memory allocation, and limits for scene elements;
    void data_check();
    // shows a report on input data;
    void data_input_report();
    // merges vertices on identical positions inside faces;
    void edge_null_delete();
    // computes faces normals by averaging vertices normals;
    void face_normal_ave();
    // erases empty faces (with less than 3 vertices);
    void face_null_delete();
    // erases empty meshes (meshes with no faces pointing);
    void mesh_null_delete();
    // computes normal vectors for vertices;
    void vertex_normal_set();
    // computes and displays vertices coordinate data range;
    void vertex_cor3_range();
    //Makes sure that all objects have unique text names, and the names
    // are no longer than maxlen characters
    void force_unique_names(int maxlen);

protected:	// converters

	bool asc_read(FileIO *filein);
	bool asc_write(FileIO *fileout);
	bool ase_read(FileIO *filein);
	bool ase_write(FileIO *fileout);
	bool byu_read(FileIO *filein);
	bool byu_write(FileIO *fileout);
	bool dxf_read(FileIO *filein);
	bool dxf_write(FileIO *fileout);
	bool hrc_read(FileIO *filein);
	bool hrc_write(FileIO *fileout);
	bool iv_read(FILE *filein);
	bool iv_write(FILE *fileout);
	bool obj_read(FILE *filein);
	bool obj_write(FILE *fileout);
	bool pov_write(FileIO *fileout);
	bool smf_read(FileIO *filein);
	bool smf_write(FileIO *fileout);
	bool stla_read(FILE *filein);
	bool stla_write(FILE *fileout);
	bool stlb_read(FileIO *filein);
	bool stlb_write(FileIO *fileout);
	bool swm_read(FILE *filein);
	bool swv_read(FILE *filein);
	bool gmod_read(FILE *fileout);
	bool gmod_write(FILE *fileout);
	bool tds_read(FileIO *filein);
	bool tds_write(FileIO *fileout);
	bool tec_write(FileIO *fileout);
	bool tria_read(FileIO *filein);
	bool tria_write(FileIO *fileout);
	bool trib_read(FileIO *filein);
	bool trib_write(FileIO *fileout);
	bool txt_write(FILE *fileout);
	bool ucd_write(FILE *fileout);
	bool vla_read(FileIO *filein);
	bool vla_write(FileIO *fileout);
	bool wrl_write(FILE *filout);
	bool xgl_write(FILE *fileout);
	

protected:

	char filein_name[FNAME_LEN];
	char fileout_name[FNAME_LEN];

  // CONVERTERS ADDITIONAL FUNCTION PROTOTYPES

    // low-level ASE read function;
    bool ase_read_cfacelist_property(ASE_READER &rd);
    bool ase_read_cvertlist_property(ASE_READER &rd);
    bool ase_read_tfacelist_property(ASE_READER &rd);
    bool ase_read_tvertlist_property(ASE_READER &rd);
    bool ase_read_facelist_property(ASE_READER &rd);
    bool ase_read_vertxlist_property(ASE_READER &rd);
    bool ase_read_meshnormals_property(ASE_READER &rd);
    bool ase_read_scene_property(ASE_READER &rd);
    bool ase_read_nodetm_property(ASE_READER &rd);
    bool ase_read_mesh_property(ASE_READER &rd);
    bool ase_read_geomobject_property(ASE_READER &rd);
    // low-level ASE write function;
    unsigned long  ase_write_geomobject(FileIO *fileout, unsigned long meshidx);
    unsigned long  ase_write_material(FileIO *fileout, unsigned long matridx);
    unsigned long  ase_write_material_map(FileIO *fileout, long textr_idx);
    unsigned long  ase_write_light(FileIO *fileout, unsigned long lightidx);
    unsigned long  ase_write_camera(FileIO *fileout, unsigned long camidx);

    // low-level DXF write function;
	bool dxf_write_shape(FileIO *fileout,unsigned long shape_idx);
	bool dxf_write_face(FileIO *fileout,unsigned long face_idx);

    // low-level IV read function;
    bool iv_read_basecolor(IV_READER &rd);
    // low-level IV write function;
    bool iv_write_material(FILE *fileout, unsigned long matridx);
    bool iv_write_trmatrix(FILE *fileout);
    bool iv_write_vertices(FILE *fileout);
    bool iv_write_textureuv(FILE *fileout);
    bool iv_write_texture(FILE *fileout, long textr_idx);
    bool iv_write_lmodel_and_matbind(FILE *fileout);
    bool iv_write_vertnormals(FILE *fileout);
    bool iv_write_faceset(FILE *fileout);
    bool iv_write_lineset(FILE *fileout);
    bool iv_write_basecolor(FILE *fileout,array<vec3> rgbcolor,unsigned long color_num);

    // low-level SWV read function;
    bool swv_read_mesh(unsigned int src_mesh,CVehiclesReader *vr,array<FaceUV> &face_uv,int car_mat,unsigned long vertex_num_base,unsigned long face_num_base);
    bool swv_read_materials();
    // low-level SWM read function;
    bool swm_read_mesh(unsigned int src_mesh,CMapReader *mr,int mat_base);

    // low-level 3DS read function;
	unsigned long  tds_read_boolean(unsigned char *boolean, FILE *filein);
    unsigned long  tds_read_edit_section(FileIO *filein, int *views_read);
	unsigned long  tds_read_keyframe_section(FileIO *filein, int *views_read);
	unsigned long  tds_read_keyframe_objdes_section(FileIO *filein);
	unsigned long  tds_read_material_section(FileIO *filein);
    unsigned long  tds_read_obj_section(FileIO *filein, unsigned long mesh_idx,array<NameItm> &face_material_names);
	unsigned long  tds_read_object_section(FileIO *filein,array<NameItm> &face_material_names);
	unsigned long  tds_read_tex_verts_section(FileIO *filein);
	unsigned long  tds_read_texmap_section(FileIO *filein);
	unsigned long  tds_read_spot_section(FileIO *filein);
	unsigned long  tds_read_vp_section(FileIO *filein, int *views_read);
    // low-level 3DS write function;
    unsigned long  tds_write_material_section(FileIO *fileout,unsigned int matidx);
    unsigned long  tds_write_named_object(FileIO *fileout,unsigned int meshidx,array<FaceUV> &face_uv);	
    unsigned long  tds_write_free_vertices(FileIO *fileout,array<FaceUV> &face_uv);

    // low-level HRC read function;
    bool hrc_read_controlpoints_property(struct HRC_READER &rd);
    bool hrc_read_edges_property(struct HRC_READER &rd);
    bool hrc_read_material_property(struct HRC_READER &rd);
    bool hrc_read_mesh_property(struct HRC_READER &rd);
    bool hrc_read_model_property(struct HRC_READER &rd);
    bool hrc_read_nodes_property(struct HRC_READER &rd);
    bool hrc_read_patch_property(struct HRC_READER &rd);
    bool hrc_read_polygons_property(struct HRC_READER &rd);
    bool hrc_read_spline_property(struct HRC_READER &rd);
    bool hrc_read_taggedpoints_property(struct HRC_READER &rd);
    bool hrc_read_texture_property(struct HRC_READER &rd);
    bool hrc_read_vertices_property(struct HRC_READER &rd);
    // low-level HRC write function;
    bool hrc_write_mesh(FileIO *fileout, unsigned long mesh_idx);

    // low-level WRL write function;
    bool wrl_write_shapes(FILE *fileout);
    bool wrl_write_mesh(FILE *fileout, unsigned long mesh_idx);
    bool wrl_write_lineset(FILE *fileout);
    bool wrl_write_faceset(FILE *fileout, unsigned long mesh_idx);

protected:
    // 3D scene elements
    Environment env;             // environment options
    Camera view;                 // Default view
    array<Camera> camera;        // Cameras in scene
    unsigned long camera_num;
    array<Light> light;          // lights in scene
    unsigned long light_num;
    array<Material> material;   // materials used
    unsigned long material_num;
    array<Texture> texture;     // textures (for use in materials)
    unsigned long texture_num;

    array<Mesh> mesh;          // meshes, or objects defifnition; faces make meshes
    unsigned long mesh_num;
    array<Face> face;           // faces definition; vertices make faces
    unsigned long face_num;
    array<Vertex> vertex;      // vertices definition
    unsigned long vertex_num;
    array<Shape> shape;        // Shapes (ie lines) definitions
    unsigned long shape_num;

	float  transform_matrix[4][4]; //the current transformation matrix
    
    //Converter statistics
    Stats stats;

public:
    //Program options
    Options opt;
    // Prefix which should be added to every name taken from DEFAULT_* pool
	char default_name_prefix[LINE_MAX_LEN];
};

// some low-level utility options
extern bool debug;
/******************************************************************************/

#endif
