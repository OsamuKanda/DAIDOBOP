# Microsoft Developer Studio Generated NMAKE File, Based on HCOPY.dsp
!IF "$(CFG)" == ""
CFG=HCOPY - Win32 Release
!MESSAGE No configuration specified. Defaulting to HCOPY - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "HCOPY - Win32 Release" && "$(CFG)" != "HCOPY - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HCOPY.mak" CFG="HCOPY - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HCOPY - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HCOPY - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "HCOPY - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\HCOPY.exe"

!ELSE 

ALL : "$(OUTDIR)\HCOPY.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\HCOPY.OBJ"
	-@erase "$(INTDIR)\HCOPY.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\HCOPY.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G6 /ML /W3 /vd0 /GX /Ox /Ot /Oa /Ow /Oi /Ob2 /Gf /Gy /D\
 "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\HCOPY.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HCOPY.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 ..\HCOPYDLL\Release\HCOPYDLL.LIB /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\HCOPY.pdb" /machine:I386 /out:"$(OUTDIR)\HCOPY.exe" 
LINK32_OBJS= \
	"$(INTDIR)\HCOPY.OBJ" \
	"$(INTDIR)\HCOPY.res"

"$(OUTDIR)\HCOPY.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "HCOPY - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\HCOPY.exe"

!ELSE 

ALL : "$(OUTDIR)\HCOPY.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\HCOPY.OBJ"
	-@erase "$(INTDIR)\HCOPY.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\HCOPY.exe"
	-@erase "$(OUTDIR)\HCOPY.ilk"
	-@erase "$(OUTDIR)\HCOPY.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G6 /MLd /W3 /Gm /vd0 /GR /GX /Zi /Od /Gf /Gy /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\HCOPY.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\HCOPY.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 ..\HCOPYDLL\Debug\HCOPYDLL.LIB /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\HCOPY.pdb" /debug /machine:I386 /out:"$(OUTDIR)\HCOPY.exe" 
LINK32_OBJS= \
	"$(INTDIR)\HCOPY.OBJ" \
	"$(INTDIR)\HCOPY.res"

"$(OUTDIR)\HCOPY.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "HCOPY - Win32 Release" || "$(CFG)" == "HCOPY - Win32 Debug"
SOURCE=.\HCOPY.C
DEP_CPP_HCOPY=\
	".\hcopy.h"\
	

"$(INTDIR)\HCOPY.OBJ" : $(SOURCE) $(DEP_CPP_HCOPY) "$(INTDIR)"


SOURCE=.\HCOPY.RC
DEP_RSC_HCOPY_=\
	".\HCOPY.ICO"\
	

"$(INTDIR)\HCOPY.res" : $(SOURCE) $(DEP_RSC_HCOPY_) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

