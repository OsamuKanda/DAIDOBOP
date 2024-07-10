# Microsoft Developer Studio Generated NMAKE File, Format Version 40001
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=HCOPYDLL - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの HCOPYDLL - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "HCOPYDLL - Win32 Release" && "$(CFG)" !=\
 "HCOPYDLL - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "HCOPYDLL.mak" CFG="HCOPYDLL - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "HCOPYDLL - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "HCOPYDLL - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "HCOPYDLL - Win32 Debug"
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HCOPYDLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "HCOPYDL0"
# PROP BASE Intermediate_Dir "HCOPYDL0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\HCOPYDLL.dll"

CLEAN : 
	-@erase ".\Release\HCOPYDLL.dll"
	-@erase ".\Release\PRINT.OBJ"
	-@erase ".\Release\DIB.OBJ"
	-@erase ".\Release\CAPTURE.OBJ"
	-@erase ".\Release\ERRORS.OBJ"
	-@erase ".\Release\PALETTE.OBJ"
	-@erase ".\Release\HCOPYDLL.OBJ"
	-@erase ".\Release\HCOPYDLL.lib"
	-@erase ".\Release\HCOPYDLL.exp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /W3 /vd0 /GX /Ox /Ot /Oa /Og /Oi /Ob2 /Gf /Gy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /G5 /ML /W3 /vd0 /GX /Ox /Ot /Oa /Og /Oi /Ob2 /Gf /Gy /D\
 "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/HCOPYDLL.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:4.0 /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:4.0\
 /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)/HCOPYDLL.pdb"\
 /machine:I386 /out:"$(OUTDIR)/HCOPYDLL.dll" /implib:"$(OUTDIR)/HCOPYDLL.lib" 
LINK32_OBJS= \
	"$(INTDIR)/PRINT.OBJ" \
	"$(INTDIR)/DIB.OBJ" \
	"$(INTDIR)/CAPTURE.OBJ" \
	"$(INTDIR)/ERRORS.OBJ" \
	"$(INTDIR)/PALETTE.OBJ" \
	"$(INTDIR)/HCOPYDLL.OBJ"

"$(OUTDIR)\HCOPYDLL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "HCOPYDLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\HCOPYDLL.dll"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\HCOPYDLL.dll"
	-@erase ".\Debug\DIB.OBJ"
	-@erase ".\Debug\ERRORS.OBJ"
	-@erase ".\Debug\HCOPYDLL.OBJ"
	-@erase ".\Debug\PRINT.OBJ"
	-@erase ".\Debug\CAPTURE.OBJ"
	-@erase ".\Debug\PALETTE.OBJ"
	-@erase ".\Debug\HCOPYDLL.ilk"
	-@erase ".\Debug\HCOPYDLL.lib"
	-@erase ".\Debug\HCOPYDLL.exp"
	-@erase ".\Debug\HCOPYDLL.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/HCOPYDLL.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/HCOPYDLL.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:4.0 /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /version:4.0\
 /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)/HCOPYDLL.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/HCOPYDLL.dll" /implib:"$(OUTDIR)/HCOPYDLL.lib" 
LINK32_OBJS= \
	"$(INTDIR)/DIB.OBJ" \
	"$(INTDIR)/ERRORS.OBJ" \
	"$(INTDIR)/HCOPYDLL.OBJ" \
	"$(INTDIR)/PRINT.OBJ" \
	"$(INTDIR)/CAPTURE.OBJ" \
	"$(INTDIR)/PALETTE.OBJ"

"$(OUTDIR)\HCOPYDLL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "HCOPYDLL - Win32 Release"
# Name "HCOPYDLL - Win32 Debug"

!IF  "$(CFG)" == "HCOPYDLL - Win32 Release"

!ELSEIF  "$(CFG)" == "HCOPYDLL - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\PRINT.C
DEP_CPP_PRINT=\
	".\dib.h"\
	".\print.h"\
	

"$(INTDIR)\PRINT.OBJ" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PALETTE.C
DEP_CPP_PALET=\
	".\errors.h"\
	".\palette.h"\
	

"$(INTDIR)\PALETTE.OBJ" : $(SOURCE) $(DEP_CPP_PALET) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HCOPYDLL.C
DEP_CPP_HCOPY=\
	".\..\hcopy\hcopy.h"\
	".\print.h"\
	".\capture.h"\
	

"$(INTDIR)\HCOPYDLL.OBJ" : $(SOURCE) $(DEP_CPP_HCOPY) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ERRORS.C
DEP_CPP_ERROR=\
	".\errors.h"\
	

"$(INTDIR)\ERRORS.OBJ" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DIB.C
DEP_CPP_DIB_C=\
	".\errors.h"\
	".\palette.h"\
	".\dib.h"\
	

"$(INTDIR)\DIB.OBJ" : $(SOURCE) $(DEP_CPP_DIB_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CAPTURE.C
DEP_CPP_CAPTU=\
	".\capture.h"\
	".\palette.h"\
	".\dib.h"\
	

"$(INTDIR)\CAPTURE.OBJ" : $(SOURCE) $(DEP_CPP_CAPTU) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
