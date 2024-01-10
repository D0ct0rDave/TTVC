# Microsoft Developer Studio Project File - Name="TTVC_MFC2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TTVC_MFC2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TTVC_MFC2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TTVC_MFC2.mak" CFG="TTVC_MFC2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TTVC_MFC2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TTVC_MFC2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TTVC_MFC2 - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "./."
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I ".\\" /I "PascalCnv" /I "Misc" /I "Controls" /I "SongStructs" /D "NDEBUG" /D "MFCS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_MIDI" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib wsock32.lib /nologo /subsystem:windows /machine:I386 /out:"TTVC.exe"

!ELSEIF  "$(CFG)" == "TTVC_MFC2 - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "./."
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I ".\\" /I "PascalCnv" /I "Misc" /I "Controls" /I "SongStructs" /D "_DEBUG" /D "MFCS" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_MIDI" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"D:\Sound\Apps\TT\TTVC.exe" /pdbtype:sept
# SUBTRACT LINK32 /map

!ENDIF 

# Begin Target

# Name "TTVC_MFC2 - Win32 Release"
# Name "TTVC_MFC2 - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\TTVC_MFC2.ico
# End Source File
# Begin Source File

SOURCE=.\res\TTVC_MFC2.rc2
# End Source File
# End Group
# Begin Group "GUIS"

# PROP Default_Filter ""
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GUIS\Main\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\GUIS\Main\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\GUIS\Main\mdiclient.cpp
# End Source File
# Begin Source File

SOURCE=.\GUIS\Main\mdiclient.h
# End Source File
# End Group
# Begin Group "PattEd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GUIS\PattEd\PattEd.cpp
# End Source File
# Begin Source File

SOURCE=.\GUIS\PattEd\PattEd.h
# End Source File
# End Group
# Begin Group "BEd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GUIS\BEd\BEd.cpp
# End Source File
# Begin Source File

SOURCE=.\GUIS\BEd\BEd.h
# End Source File
# End Group
# Begin Group "BRep"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GUIS\BRep\BRep.cpp
# End Source File
# Begin Source File

SOURCE=.\GUIS\BRep\BRep.h
# End Source File
# End Group
# Begin Group "TrackProperties"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GUIS\TrackProperties\TrackProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\GUIS\TrackProperties\Trackproperties.h
# End Source File
# End Group
# Begin Group "BSeq"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GUIS\BSeq\BSeq.cpp
# End Source File
# Begin Source File

SOURCE=.\GUIS\BSeq\BSeq.h
# End Source File
# End Group
# Begin Group "InputBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GUIS\InputBox\InputBox.cpp
# End Source File
# Begin Source File

SOURCE=.\GUIS\InputBox\InputBox.h
# End Source File
# End Group
# End Group
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Controls\BMPResource.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\BMPResource.h
# End Source File
# Begin Source File

SOURCE=.\Controls\BtnMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\BtnMgr.h
# End Source File
# Begin Source File

SOURCE=.\Controls\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls\Label.h
# End Source File
# End Group
# Begin Group "PascalCnv"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PascalCnv\PascalCnv.h
# End Source File
# Begin Source File

SOURCE=.\PascalCnv\PascalIO.cpp
# End Source File
# Begin Source File

SOURCE=.\PascalCnv\PascalIO.h
# End Source File
# Begin Source File

SOURCE=.\PascalCnv\PascalString.cpp
# End Source File
# Begin Source File

SOURCE=.\PascalCnv\PascalString.h
# End Source File
# Begin Source File

SOURCE=.\PascalCnv\PascalTypes.h
# End Source File
# End Group
# Begin Group "Player"

# PROP Default_Filter ""
# Begin Group "MIDIInClient"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PLAYER\MIDIInClient\CNETClient.cpp
# End Source File
# Begin Source File

SOURCE=.\PLAYER\MIDIInClient\CNETClient.h
# End Source File
# Begin Source File

SOURCE=.\PLAYER\MIDIInClient\CNETServer.cpp
# End Source File
# Begin Source File

SOURCE=.\PLAYER\MIDIInClient\CNETServer.h
# End Source File
# Begin Source File

SOURCE=.\PLAYER\MIDIInClient\CNETSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\PLAYER\MIDIInClient\CNETSystem.h
# End Source File
# Begin Source File

SOURCE=.\PLAYER\MIDIInClient\MIDIInClient.cpp
# End Source File
# Begin Source File

SOURCE=.\PLAYER\MIDIInClient\MIDIInClient.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\PLAYER\OMidiIn.cpp
# End Source File
# Begin Source File

SOURCE=.\PLAYER\OMidiIn.h
# End Source File
# Begin Source File

SOURCE=.\PLAYER\OMidiOut.cpp
# End Source File
# Begin Source File

SOURCE=.\PLAYER\OMidiOut.h
# End Source File
# Begin Source File

SOURCE=.\PLAYER\tt98Player.cpp
# End Source File
# Begin Source File

SOURCE=.\PLAYER\tt98Player.h
# End Source File
# End Group
# Begin Group "SongStructs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SongStructs\bloques.cpp
# End Source File
# Begin Source File

SOURCE=.\SongStructs\bloques.h
# End Source File
# Begin Source File

SOURCE=.\SongStructs\cancion.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SongStructs\consts2.cpp
# End Source File
# Begin Source File

SOURCE=.\SongStructs\consts2.h
# End Source File
# Begin Source File

SOURCE=.\SongStructs\CrtError.cpp
# End Source File
# Begin Source File

SOURCE=.\SongStructs\CrtError.h
# End Source File
# Begin Source File

SOURCE=.\SongStructs\FICHEROS.cpp
# End Source File
# Begin Source File

SOURCE=.\SongStructs\FICHEROS.h
# End Source File
# Begin Source File

SOURCE=.\SongStructs\InfoPistas.cpp
# End Source File
# Begin Source File

SOURCE=.\SongStructs\InfoPistas.h
# End Source File
# Begin Source File

SOURCE=.\SongStructs\Inter.cpp
# End Source File
# Begin Source File

SOURCE=.\SongStructs\Inter.h
# End Source File
# Begin Source File

SOURCE=.\SongStructs\PATTERNS.CPP
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SongStructs\SECUEN.CPP
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SongStructs\TABLAINS.CPP
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\SongStructs\Tefectos.cpp
# End Source File
# Begin Source File

SOURCE=.\SongStructs\Tefectos.h
# End Source File
# Begin Source File

SOURCE=.\SongStructs\tipos.h
# End Source File
# Begin Source File

SOURCE=.\SongStructs\TRACKS.CPP
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Misc\Config.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\Config.h
# End Source File
# Begin Source File

SOURCE=.\Misc\LangStrs.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\LangStrs.h
# End Source File
# Begin Source File

SOURCE=.\Misc\MiscIns.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\MiscIns.h
# End Source File
# Begin Source File

SOURCE=.\Misc\Msgs.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\Msgs.h
# End Source File
# Begin Source File

SOURCE=.\Misc\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\Parser.h
# End Source File
# Begin Source File

SOURCE=.\Misc\Pistas22.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\Pistas22.h
# End Source File
# Begin Source File

SOURCE=.\Misc\RList.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\RList.h
# End Source File
# Begin Source File

SOURCE=.\Misc\Strlist.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\Strlist.h
# End Source File
# Begin Source File

SOURCE=.\Misc\TckAMid.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\TckAMid.h
# End Source File
# Begin Source File

SOURCE=.\Misc\Variables.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\Variables.h
# End Source File
# Begin Source File

SOURCE=.\Misc\WinVars.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc\WinVars.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TTVC_MFC2.cpp
# End Source File
# Begin Source File

SOURCE=.\TTVC_MFC2.h
# End Source File
# Begin Source File

SOURCE=.\TTVC_MFC2.rc
# End Source File
# End Target
# End Project
