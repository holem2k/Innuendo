# Microsoft Developer Studio Generated NMAKE File, Based on Innuendo.dsp
!IF "$(CFG)" == ""
CFG=Innuendo - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Innuendo - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Innuendo - Win32 Release" && "$(CFG)" != "Innuendo - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Innuendo.mak" CFG="Innuendo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Innuendo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Innuendo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Innuendo - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Innuendo.exe"


CLEAN :
	-@erase "$(INTDIR)\Assert!.obj"
	-@erase "$(INTDIR)\Innuendo.obj"
	-@erase "$(INTDIR)\Innuendo.pch"
	-@erase "$(INTDIR)\Polygon.obj"
	-@erase "$(INTDIR)\Screen.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\Vertex.obj"
	-@erase "$(INTDIR)\View.obj"
	-@erase "$(INTDIR)\Win.obj"
	-@erase "$(OUTDIR)\Innuendo.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Innuendo.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Innuendo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Innuendo.pdb" /machine:I386 /out:"$(OUTDIR)\Innuendo.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Assert!.obj" \
	"$(INTDIR)\Innuendo.obj" \
	"$(INTDIR)\Polygon.obj" \
	"$(INTDIR)\Screen.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Vertex.obj" \
	"$(INTDIR)\View.obj" \
	"$(INTDIR)\Win.obj"

"$(OUTDIR)\Innuendo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Innuendo - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Innuendo.exe"


CLEAN :
	-@erase "$(INTDIR)\Assert!.obj"
	-@erase "$(INTDIR)\Innuendo.obj"
	-@erase "$(INTDIR)\Innuendo.pch"
	-@erase "$(INTDIR)\Polygon.obj"
	-@erase "$(INTDIR)\Screen.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\Vertex.obj"
	-@erase "$(INTDIR)\View.obj"
	-@erase "$(INTDIR)\Win.obj"
	-@erase "$(OUTDIR)\Innuendo.exe"
	-@erase "$(OUTDIR)\Innuendo.ilk"
	-@erase "$(OUTDIR)\Innuendo.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G4 /MLd /W3 /Gm /Gi /Zi /Od /Ob1 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Innuendo.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Innuendo.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Innuendo.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Innuendo.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Assert!.obj" \
	"$(INTDIR)\Innuendo.obj" \
	"$(INTDIR)\Polygon.obj" \
	"$(INTDIR)\Screen.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Vertex.obj" \
	"$(INTDIR)\View.obj" \
	"$(INTDIR)\Win.obj"

"$(OUTDIR)\Innuendo.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Innuendo.dep")
!INCLUDE "Innuendo.dep"
!ELSE 
!MESSAGE Warning: cannot find "Innuendo.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Innuendo - Win32 Release" || "$(CFG)" == "Innuendo - Win32 Debug"
SOURCE=".\Assert!.cpp"

"$(INTDIR)\Assert!.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Innuendo.pch"


SOURCE=.\Innuendo.cpp

"$(INTDIR)\Innuendo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Innuendo.pch"


SOURCE=.\Polygon.cpp

"$(INTDIR)\Polygon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Innuendo.pch"


SOURCE=.\Screen.cpp

"$(INTDIR)\Screen.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Innuendo.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Innuendo - Win32 Release"

CPP_SWITCHES=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Innuendo.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Innuendo.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Innuendo - Win32 Debug"

CPP_SWITCHES=/nologo /G4 /MLd /W3 /Gm /Gi /Zi /Od /Ob1 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Innuendo.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Innuendo.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Vertex.cpp

"$(INTDIR)\Vertex.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Innuendo.pch"


SOURCE=.\View.cpp

"$(INTDIR)\View.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Innuendo.pch"


SOURCE=.\Win.cpp

"$(INTDIR)\Win.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Innuendo.pch"



!ENDIF 

