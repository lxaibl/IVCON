/******************************************************************************/
// Author:  John Burkardt 

// Purpose:
//   Header file for UTILS.CPP, defining some usefull classes and methods

//Copying and copyrights:
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
/******************************************************************************/

#ifndef _ivcon_utils
#define _ivcon_utils

#include <stdio.h>
#include <vector>
#include <valarray>

//Limits of various values - to prevent hanging
const unsigned int  LINE_MAX_LEN=256;    // maximal text line length (ie name)
//Size allocated for storing file paths
const unsigned int  FNAME_LEN=511;       // maximum length for file name, longer path to allow network...

/*template <typename T>
T max(const T& a, const T& b) {return a>b?a:b;}
*/
class vec2:public std::valarray<float>
{
typedef std::valarray<float> _parent;
public:
	vec2():_parent(0.0,2) {};	
	vec2(const float* buf):_parent(buf,2) {};
	vec2(const float x, const float y):_parent(2) 
		{ operator[](0)=x; operator[](1)=y;}			
    bool operator==(const vec2& __other) const
      { return (operator[](0)==__other[0]) && (operator[](1)==__other[1]); }
};

class vec3:public std::valarray<float>
{
typedef std::valarray<float> _parent;
public:
	vec3():_parent(0.0,3) {};	
	vec3(const float* buf):_parent(buf,3) {};
	vec3(const float x, const float y, const float z):_parent(3) 
		{ operator[](0)=x; operator[](1)=y; operator[](2)=z;}			
    bool operator==(const vec3& __other) const
      { return (operator[](0)==__other[0]) && (operator[](1)==__other[1]) &&
               (operator[](2)==__other[2]); }
};


class vec4:public std::valarray<float>
{
typedef std::valarray<float> _parent;
public:
	vec4():_parent(0.0,4) {};	
	vec4(const float* buf):_parent(buf,4) {};
	vec4(const float x, const float y, const float z, const float t):_parent(4) 
		{ operator[](0)=x; operator[](1)=y; operator[](2)=z; operator[](3)=t;}			
};

template <typename T>	// a vector with automatic growth
class array : public std::vector<T>
{
typedef std::vector<T> _parent;
public:
	
    array(const size_t size=0) : _parent(size) {};
	T& operator[](size_t i)
	{
		if (i >= array::size()) 
		{
			if (i >= array::capacity())
				array::reserve(i*2);
			array::resize(i+1);
		}
		return _parent::operator[](i);
	}
	const T& operator[](size_t i) const
	{
		return _parent::operator[](i);
	}
};

// FILES RELATED
extern bool byte_swap;
char *file_ext ( char *file_name );
float float_reverse_bytes ( float x );

class FileIO
{
public: 
	FileIO(const char *nfname,const char *naccmode);
	virtual ~FileIO();
	bool is_binary();
	bool is_opened();
	bool is_big_endian();
	void set_big_endian(bool val);
	long int ftell();
	int fseek(long int offset, int origin);
	bool feof();
    // writing routines
    int fputc(int character);
    int fprintf(const char * format, ... );
    int float_write(float float_val);
    int long_int_write(long int_val);
    int long_uint_write(unsigned long int_val);
    int short_int_write(short int int_val);
    int short_uint_write(unsigned short int_val);
    // reading routines
    int fgetc();
    char *fgets(char *str,int num);
    float float_read();
    long long_int_read();
    unsigned long long_uint_read();
    short int short_int_read();
    unsigned short short_uint_read();
protected:
    char fname[FNAME_LEN];
    bool binary;
    bool big_endian;
    unsigned long lines_read;
    unsigned long lines_wrote;
    unsigned long bytes_read;
    unsigned long bytes_wrote;
    long int f_last_pos;
public: //temporary solution
    FILE *fileptr;
};

// Structure to create list of names, useful when reading some formats
struct NameItm
{
	char name[LINE_MAX_LEN];
};

float rgb_to_hue ( float r, float g, float b );

int rcol_find ( const array<vec3>& a, int n, vec3 ) ;

int char_index_last ( char* string, char c );
bool char_pad ( unsigned int *char_index, unsigned int *null_index, char *string, unsigned long STRING_MAX );

bool leqi ( char* string1, char* string2 );

int ivec_max ( unsigned int n, const array<unsigned int>& a );
long get_long_pos ( unsigned long val, const array<unsigned long>& a,unsigned long n );

int nameitem_arr_pos(array<NameItm> namearr,char *name);

#endif
