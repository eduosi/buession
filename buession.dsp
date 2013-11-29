# Microsoft Developer Studio Project File - Name="buession" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=buession - Win32 Release_TS
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "buession.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "buession.mak" CFG="buession - Win32 Release_TS"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "buession - Win32 Release_TS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "buession - Win32 Debug_TS" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "buession - Win32 Release_TS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_TS"
# PROP BASE Intermediate_Dir "Release_TS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_TS"
# PROP Intermediate_Dir "Release_TS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\.." /I "..\..\..\Zend" /I "..\..\..\bindlib_w32" /I "..\..\..\TSRM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMPILE_DL_BUESSION" /D ZTS=1 /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\.." /I "..\..\main" /I "..\..\Zend" /I "..\..\..\bindlib_w32" /I "..\..\TSRM" /D ZEND_DEBUG=0 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BUESSION_EXPORTS" /D "COMPILE_DL_BUESSION" /D ZTS=1 /D "ZEND_WIN32" /D "PHP_WIN32" /D HAVE_BUESSION=1 /D "LIBZEND_EXPORTS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x406 /d "NDEBUG"
# ADD RSC /l 0x406 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib php5ts.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib php5ts.lib /nologo /dll /machine:I386 /out:"..\..\Release_TS/php_buession.dll" /libpath:"..\..\Release_TS" /libpath:"..\..\Release_TS_Inline"

!ELSEIF  "$(CFG)" == "buession - Win32 Debug_TS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Debug_TS"
# PROP BASE Intermediate_Dir "Debug_TS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Debug_TS"
# PROP Intermediate_Dir "Debug_TS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\.." /I "..\..\Zend" /I "..\..\..\bindlib_w32" /I "..\..\TSRM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMPILE_DL_BUESSION" /D ZTS=1 /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /O2 /I "..\.." /I "..\..\main" /I "..\..\Zend" /I "..\..\..\bindlib_w32" /I "..\..\TSRM" /D ZEND_DEBUG=1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BUESSION_EXPORTS" /D "COMPILE_DL_BUESSION" /D ZTS=1 /D "ZEND_WIN32" /D "PHP_WIN32" /D HAVE_BUESSION=1 /D "LIBZEND_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x406 /d "NDEBUG"
# ADD RSC /l 0x406 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib php5ts.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib php5ts_debug.lib /nologo /dll /machine:I386 /out:"..\..\Debug_TS/php_buession.dll" /libpath:"..\..\Debug_TS"

!ENDIF 

# Begin Target

# Name "buession - Win32 Release_TS"
# Name "buession - Win32 Debug_TS"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File
SOURCE=./buession.c
# End Source File

# Begin Source File
SOURCE=./alloc.c
# End Source File

# Begin Source File
SOURCE=./types.c
# End Source File

# Begin Source File
SOURCE=./Buession_API.c
# End Source File

# Begin Source File
SOURCE=./variable.c
# End Source File

# Begin Source File
SOURCE=./regex.h
# End Source File

# Begin Source File
SOURCE=./hash.c
# End Source File

# Begin Source File
SOURCE=./objects_API.c
# End Source File

# Begin Source File
SOURCE=./constant.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/creditcard.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/amerucan_express.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/diners_club_us.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/diners_club.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/discover.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/jcb.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/laser.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/maestro.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/mastercard.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/solo.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/uniopay.c
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/visa.c
# End Source File

# Begin Source File
SOURCE=./exception.c
# End Source File

# Begin Source File
SOURCE=./assert/assert.c
# End Source File

# Begin Source File
SOURCE=./calendar/calendar.c
# End Source File

# Begin Source File
SOURCE=./helper/helper.c
# End Source File

# Begin Source File
SOURCE=./helper/html.c
# End Source File

# Begin Source File
SOURCE=./helper/form.c
# End Source File

# Begin Source File
SOURCE=./math/math.c
# End Source File

# Begin Source File
SOURCE=./registry/registry.c
# End Source File

# Begin Source File
SOURCE=./validate/validate.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File
SOURCE=./buession.h
# End Source File

# Begin Source File
SOURCE=./alloc.h
# End Source File

# Begin Source File
SOURCE=./types.h
# End Source File

# Begin Source File
SOURCE=./Buession_API.h
# End Source File

# Begin Source File
SOURCE=./variable.h
# End Source File

# Begin Source File
SOURCE=./regex.h
# End Source File

# Begin Source File
SOURCE=./hash.h
# End Source File

# Begin Source File
SOURCE=./objects_API.h
# End Source File

# Begin Source File
SOURCE=./constant.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/creditcard.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/amerucan_express.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/diners_club_us.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/diners_club.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/discover.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/jcb.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/laser.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/maestro.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/mastercard.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/solo.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/uniopay.h
# End Source File

# Begin Source File
SOURCE=./dict/creditcard/visa.h
# End Source File

# Begin Source File
SOURCE=./dict/domain.h
# End Source File

# Begin Source File
SOURCE=./exception.h
# End Source File

# Begin Source File
SOURCE=./assert/assert.h
# End Source File

# Begin Source File
SOURCE=./calendar/calendar.h
# End Source File

# Begin Source File
SOURCE=./config/config.h
# End Source File

# Begin Source File
SOURCE=./config/cache.h
# End Source File

# Begin Source File
SOURCE=./config/simple.h
# End Source File

# Begin Source File
SOURCE=./config/json.h
# End Source File

# Begin Source File
SOURCE=./config/ini.h
# End Source File

# Begin Source File
SOURCE=./config/properties.h
# End Source File

# Begin Source File
SOURCE=./helper/helper.h
# End Source File

# Begin Source File
SOURCE=./helper/html.h
# End Source File

# Begin Source File
SOURCE=./helper/form.h
# End Source File

# Begin Source File
SOURCE=./math/math.h
# End Source File

# Begin Source File
SOURCE=./registry/registry.h
# End Source File

# Begin Source File
SOURCE=./validate/validate.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
