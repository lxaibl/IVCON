# Microsoft Developer Studio Project File - Name="ivcon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ivcon - WIN32 PSEUDODEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ivcon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ivcon.mak" CFG="ivcon - WIN32 PSEUDODEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ivcon - Win32 PseudoDebug" (based on "Win32 (x86) Static Library")
!MESSAGE "ivcon - Win32 SW" (based on "Win32 (x86) Static Library")
!MESSAGE "ivcon - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ivcon - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ivcon - Win32 PseudoDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PseudoDebug"
# PROP BASE Intermediate_Dir "PseudoDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PseudoDebug"
# PROP Intermediate_Dir "PseudoDebug"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /ZI /Od /I "l:\\" /D "WIN32" /D "NDEBUG" /D "_PSEUDO_DEBUG" /D "_UNICODE" /Fr /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x100c /d "NDEBUG"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ivcon - Win32 SW"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SW"
# PROP BASE Intermediate_Dir "SW"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "SW"
# PROP Intermediate_Dir "SW"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "." /I ".." /I "c:\libs" /D "WIN32" /D "NDEBUG" /D "_PSEUDO_DEBUG" /D "_UNICODE" /D "_AFXDLL" /Fr /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GX /Zd /O2 /Ob2 /I "c:\libs" /D "WIN32" /D "NDEBUG" /D "_UNICODE" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "_AFXDLL"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ivcon - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ivcon___Win32_Debug"
# PROP BASE Intermediate_Dir "ivcon___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /ZI /Od /I "." /I ".." /I "l:\\" /D "WIN32" /D "NDEBUG" /D "_PSEUDO_DEBUG" /D "_UNICODE" /D "_AFXDLL" /Fr /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MLd /W3 /GX /ZI /Od /I "l:\\" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /Fr /YX"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_AFXDLL"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "ivcon - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ivcon___Win32_Release"
# PROP BASE Intermediate_Dir "ivcon___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /ZI /Od /I "." /I ".." /I "l:\\" /D "WIN32" /D "NDEBUG" /D "_PSEUDO_DEBUG" /D "_UNICODE" /D "_AFXDLL" /Fr /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /GX /O2 /I "l:\\" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x409 /d "_AFXDLL"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "ivcon - Win32 PseudoDebug"
# Name "ivcon - Win32 SW"
# Name "ivcon - Win32 Debug"
# Name "ivcon - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\asc.cpp
# End Source File
# Begin Source File

SOURCE=.\ase.cpp
# End Source File
# Begin Source File

SOURCE=.\byu.cpp
# End Source File
# Begin Source File

SOURCE=.\dxf.cpp
# End Source File
# Begin Source File

SOURCE=.\gmod.cpp
# End Source File
# Begin Source File

SOURCE=.\hrc.cpp
# End Source File
# Begin Source File

SOURCE=.\iv.cpp
# End Source File
# Begin Source File

SOURCE=.\ivconv.cpp
# End Source File
# Begin Source File

SOURCE=.\obj.cpp
# End Source File
# Begin Source File

SOURCE=.\pov.cpp
# End Source File
# Begin Source File

SOURCE=.\smf.cpp
# End Source File
# Begin Source File

SOURCE=.\stl.cpp
# End Source File
# Begin Source File

SOURCE=.\tds.cpp
# End Source File
# Begin Source File

SOURCE=.\tec.cpp
# End Source File
# Begin Source File

SOURCE=.\tmat.cpp
# End Source File
# Begin Source File

SOURCE=.\tri.cpp
# End Source File
# Begin Source File

SOURCE=.\txt.cpp
# End Source File
# Begin Source File

SOURCE=.\ucd.cpp
# End Source File
# Begin Source File

SOURCE=.\utils.cpp
# End Source File
# Begin Source File

SOURCE=.\vla.cpp
# End Source File
# Begin Source File

SOURCE=.\wrl.cpp
# End Source File
# Begin Source File

SOURCE=.\xgl.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\3dsexp.h
# End Source File
# Begin Source File

SOURCE=.\3dsimp.h
# End Source File
# Begin Source File

SOURCE=.\ivconv.h
# End Source File
# Begin Source File

SOURCE=.\ofile.h
# End Source File
# Begin Source File

SOURCE=.\tmat.h
# End Source File
# Begin Source File

SOURCE=.\utils.h
# End Source File
# End Group
# End Target
# End Project
