
IVCON 3D Graphics File Converter
==================================

0. Short overview
----------------------------------

IVCON is an executable C++ program that can read and write a small variety
 of 3D graphics file formats, converting from one to the other.

This is extended version of John Burkardt's great, but buggy tool.
It Supports many 3D formats, and a lot of 3D scene elements.


1. Supported formats
----------------------------------

The supported formats include:
* 3DS AutoCAD 3D Studio Max binary files;
* ASE AutoCAD ASCII export files;
* BYU Movie.BYU surface geometry files;
* DXF AutoCAD DXF files;
* GMOD Golgotha GMOD files;
* HRC SoftImage hierarchical files;
* IV SGI Inventor files;
* OBJ WaveFront Advanced Visualizer ASCII files;
* OFF GEOMVIEW OFF files;
* POV Persistence of Vision files (output only);
* SMF Michael Garland's format for his QSLIM program;
* STL/STLA ASCII Stereolithography files;
* STLB binary Stereolithography files;
* SWV Syndicate Wars vehicles primveh (input only);
* SWM Renamed Syndicate Wars MAD maps (input only);
* TEC TECPLOT files (output only);
* TRI/TRIA a simple ASCII triangle format requested by Greg Hood;
* TRIB a simple binary triangle format requested by Greg Hood;
* TXT a text dump (output only);
* UCD Advanced Visual Systems (AVS) Unstructured Cell Data (output only);
* VLA Evans and Sutherland Digistar II VLA files for planetariums;
* WRL WRL/VRML (Virtual Reality Modeling Language) files (output only).
* XGL the XGL format, based on the XML language and OpenGl graphics (output only).


2. Supported scene elements
----------------------------------

A single file format may support only limited elements of a scene, but
 the main program allows all formats to support:

* Unlimited (32-bit, or 64-bit depending on system architecture) number
  of vertices (vertices are points in 3D space);
* Unlimited (system-dependent) number of faces (face is a single closed area
  made of some vertices);
* Unlimited (system-dependent) number of meshes/object (a mesh, or a object,
  is a set of faces making one element; mesh usually closes a volume);
* Unlimited number of shapes/lines (lines are usually helpers in 3D scene);
* Unlimited number of materials; diffuse, ambient and specular color
  in every material; alpha (transparency) is also supported;
* Unlimited number of textures; alpha level for every texture;
* Texture mapping (so-called UV mapping - defines how to place
  textures on meshes);
* Three textures in every material: diffuse, diffuse alpha map and bump;
* One default place of view and unlimited cameras
* Environment parameters - color and texture; unlimited lights;
* Program allows to merge scenes from different files;
  automatically fixes errors in data;
* Basic data processing - face order reduction, small faces deletion;


3. Usage of the program
----------------------------------

The program can be used in command line form, as in:
    ivcon input.iv output.stl 
 or can be used in an interactive mode, in which input and output files
 can be read or converted in any order.

Graphics file formats are poorly documented; hence this program was written
 primarily by examining typical files and struggling to interpret them.
Particularly in the case of Inventor and DXF files, the program is not able
 to handle all the subtleties of the file format, and may mangle data,
 or even crash.

4. Changelog
----------------------------------
1.17
 - Fixed SMF format support
1.16
 - Fixed HRC, TEC, POV and VLA format support
 - Shapes can now belong to a mesh
1.15
 - Added rename meshes commands
 - Added reducing mesh name sizes and making the names unique
1.14
 - Rewritten the INTERACT commands routines
 - Added reducing face order command
 - Added mesh renaming command
1.13
 - Started unification of file reading routines
 - Corrected BYU format
 - Corrected DXF format
 - Minor corrections in ASC and ASE
1.12
 - Some speedups in listing mesh vertices
 - ASE export can support face order up to 26
 - Extended and fixed WRL writing
 - Fixed re-setting faces area after reducing faces order
 - Added support of Syndicate Wars maps SWM
1.11
 - Fixed some core errors (better memory allocation)
 - Added empty meshes deletion
 - Fixed some errors in IV and 3DS formats
1.09
 - Added support of Syndicate Wars vehicles SWV
 - Fixed IV support


5. Support
----------------------------------

For technical support, or if you wish to report a bug, please go to
the project's page at SourceForge and use the "Tracker" menu:

https://sourceforge.net/tracker/?group_id=191003

If you're submitting a file open problem, you will probably be asked
to send the problematic file via mail. To make it faster, you can
upload the file to any free hosting site, and provide the link in your
bug report.


6. Authors
----------------------------------

Core author is John Burkardt. Helpful corrections to the 3DS reader
 were supplied by John F Flanagan.
The GMOD reader and writer were written and supplied by Zik Saleeba.
Improvements to the DXF routines to handle polygon oriented files were
 supplied by Zik Saleeba.
The 3DS reader is based on a program by Martin van Velsen, with the help
 of Gert van der Spoel. 
Divided into small files, corrections and improvements, normals computation
 by Philippe Guglielmetti.
Rewritten, vierified, corrected many bugs, improved 3DS filter and main
 class by Tomasz Lis.


7. Reference
----------------------------------
1. Adrian Bowyer and John Woodwark,
   A Programmer's Geometry,
   Butterworths, 1983.
2. James Foley, Andries van Dam, Steven Feiner, John Hughes,
   Computer Graphics, Principles and Practice,
   Addison Wesley, Second Edition, 1990.
3. 3D Systems, Inc,
   Stereolithography Interface Specification,
   October 1989.

8. More documentation
----------------------------------
You can find more information on IVCon HTTP page:

http://ivcon-tl.sourceforge.net/

It is also available in offline version, inside the samples package.

<EOF>
