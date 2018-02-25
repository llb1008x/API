# Microsoft Developer Studio Project File - Name="I86_MS_start" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=I86_MS_start - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "I86_MS_start.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "I86_MS_start.mak" CFG="I86_MS_start - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "I86_MS_start - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "I86_MS_start___Win32_Debug"
# PROP BASE Intermediate_Dir "I86_MS_start___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "output\I86_MS"
# PROP Intermediate_Dir "output\I86_MS"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "FS\api" /I "FS\config\Win32" /I "FS\lbl" /I "FS\os" /I "FS\fsl\fat" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# Begin Target

# Name "I86_MS_start - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "FS_API"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FS\API\fs_info.c
# End Source File
# End Group
# Begin Group "FS_DEV_WINDRIVE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FS\DEVICE\windrive\wd_misc.c
# End Source File
# End Group
# Begin Group "FS_FSL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FS\FSL\fat\fat_data.c
# End Source File
# End Group
# Begin Group "FS_LBL"

# PROP Default_Filter ""
# End Group
# Begin Group "FS_OS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FS\OS\FS_X_win32.c
# End Source File
# End Group
# Begin Group "sample"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FS\sample\main.c
# End Source File
# End Group
# Begin Group "DEV_RAM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FS\DEVICE\ram\r_misc.c
# End Source File
# End Group
# Begin Group "FS_EVALIBRARY"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE=.\FS\lib\fs_eva.lib
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
