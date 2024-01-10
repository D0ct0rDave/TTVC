; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TTVC_MFC2.h"
LastPage=0

ClassCount=10
Class1=CTTVC_MFC2App
Class3=CMainFrame
Class4=CChildFrame

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDD_BSEQ
Class2=PattEd
Class5=CAboutDlg
Class6=CMDIClient
Resource3=IDD_TRACKPROPS
Resource4=IDR_MAINFRAME_MENU
Class7=TrackProperties
Resource5=IDR_MAINFRAME_ACCEL
Class8=CBSeq
Class9=CGridListView
Class10=CInputBox
Resource6=IDD_BREP
Resource7=IDD_INPUTBOX

[CLS:CTTVC_MFC2App]
Type=0
HeaderFile=TTVC_MFC2.h
ImplementationFile=TTVC_MFC2.cpp
Filter=N
LastObject=CTTVC_MFC2App
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CMainFrame]
Type=0
HeaderFile=guis\main\mainfrm.h
ImplementationFile=guis\main\mainfrm.cpp
BaseClass=CMDIFrameWnd
LastObject=CMainFrame
Filter=T
VirtualFilter=fWC

[CLS:CChildFrame]
Type=0
HeaderFile=guis\patted\childfrm.h
ImplementationFile=guis\patted\childfrm.cpp
BaseClass=CMDIChildWnd
Filter=M
LastObject=CChildFrame

[CLS:CAboutDlg]
Type=0
HeaderFile=TTVC_MFC2.cpp
ImplementationFile=TTVC_MFC2.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:CMDIClient]
Type=0
HeaderFile=guis\main\mdiclient.h
ImplementationFile=guis\main\mdiclient.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC
LastObject=CMDIClient

[DLG:IDD_BREP]
Type=1
Class=?
ControlCount=0

[CLS:PattEd]
Type=0
HeaderFile=guis\patted\childview.h
ImplementationFile=guis\patted\childview.cpp
BaseClass=CWnd 
LastObject=ID_APP_ABOUT

[DLG:IDD_TRACKPROPS]
Type=1
Class=TrackProperties
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=1000,edit,1350631552
Control4=1001,combobox,1344339971
Control5=1002,combobox,1344339971
Control6=1003,combobox,1344339971
Control7=1004,button,1342242819
Control8=1005,button,1342242819
Control9=1006,button,1342242816
Control10=1007,button,1342242816
Control11=1009,combobox,1344339971

[CLS:TrackProperties]
Type=0
HeaderFile=guis\trackproperties\trackproperties.h
ImplementationFile=guis\trackproperties\trackproperties.cpp
BaseClass=CDialog
LastObject=1000
Filter=D
VirtualFilter=dWC

[DLG:IDD_BSEQ]
Type=1
Class=CBSeq
ControlCount=3
Control1=IDC_LIST_SEQ,listbox,1344275729
Control2=IDC_ST_TITLE,static,1342308608
Control3=IDC_ST_AUTOR,static,1342308608

[CLS:CBSeq]
Type=0
HeaderFile=guis\bseq\bseq.h
ImplementationFile=guis\bseq\bseq.cpp
BaseClass=CWnd
Filter=D
VirtualFilter=dWC
LastObject=CBSeq

[CLS:CGridListView]
Type=0
HeaderFile=GridListView.h
ImplementationFile=GridListView.cpp
BaseClass=CListView
Filter=C
LastObject=CGridListView

[CLS:CInputBox]
Type=0
HeaderFile=guis\inputbox\inputbox.h
ImplementationFile=guis\inputbox\inputbox.cpp
BaseClass=CDialog
LastObject=CInputBox
Filter=D
VirtualFilter=dWC

[DLG:IDD_INPUTBOX]
Type=1
Class=CInputBox
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_EDIT2,edit,1342179328

[MNU:IDR_MAINFRAME_MENU]
Type=1
Class=CMainFrame
Command1=ID_ARCHIVO_NUEVO
Command2=ID_ARCHIVO_ABRIR
Command3=ID_ARCHIVO_GUARDARTEMA
Command4=ID_ARCHIVO_GUARDARTEMACOMO
Command5=ID_ARCHIVO_EXPORTARAMIDI
Command6=ID_RECENTFILE_00
Command7=ID_RECENTFILE_01
Command8=ID_RECENTFILE_02
Command9=ID_RECENTFILE_03
Command10=ID_APP_EXIT
Command11=ID_EDICIN_PATRN_COPIAR
Command12=ID_EDICIN_PATRN_CORTAR
Command13=ID_EDICIN_PATRN_PEGAR
Command14=ID_EDICIN_PATRN_BORRAR
Command15=ID_EDICIN_PATRN_CARGAR
Command16=ID_EDICIN_PATRN_SALVAR
Command17=ID_EDICIN_PISTA_COPIAR
Command18=ID_EDICIN_PISTA_CORTAR
Command19=ID_EDICIN_PISTA_PEGAR
Command20=ID_EDICIN_PISTA_BORRAR
Command21=ID_EDICIN_PISTA_PROCESSARVOLUMEN
Command22=ID_EDICIN_PISTA_PROCESAREFECTO
Command23=ID_EDICIN_PISTA_CARGAR
Command24=ID_EDICIN_PISTA_SALVAR
Command25=ID_EDICIN_BLOQUE_COPIAR
Command26=ID_EDICIN_BLOQUE_CORTAR
Command27=ID_EDICIN_BLOQUE_PEGAR
Command28=ID_EDICIN_BLOQUE_BORRAR
Command29=ID_EDICIN_BLOQUE_CARGAR
Command30=ID_EDICIN_BLOQUE_SALVAR
Command31=ID_EDICIN_TABLASDEEFECTOS
Command32=ID_EDICIN_TABLASDEINSTRUMENTOS
Command33=ID_EDICIN_OPCIONES_ENTRADAMIDI
Command34=ID_VER_BARRADEREPRODUCCION
Command35=ID_VER_BARRADESECUENCIA
Command36=ID_VER_BARRADEEDICIN
Command37=ID_VIEW_STATUS_BAR
Command38=ID_APP_ABOUT
CommandCount=38

[ACL:IDR_MAINFRAME_ACCEL]
Type=1
Class=CMainFrame
Command1=ID_VER_BARRADEREPRODUCCION
Command2=ID_VER_BARRADESECUENCIA
Command3=ID_VER_BARRADEEDICIN
Command4=ID_VIEW_STATUS_BAR
Command5=ID_ENABLE_EDITION
Command6=ID_EDICIN_TABLASDEEFECTOS
Command7=ID_EDICIN_PISTA_PROCESAREFECTO
Command8=ID_EDICIN_TABLASDEINSTRUMENTOS
Command9=ID_ARCHIVO_NUEVO
Command10=ID_ARCHIVO_ABRIR
Command11=ID_APP_EXIT
Command12=ID_FILE_SAVE
Command13=ID_FILE_SAVE_AS
Command14=ID_EDICIN_PISTA_PROCESSARVOLUMEN
Command15=ID_BPM_INC
Command16=ID_EDICIN_BLOQUE_COPIAR
Command17=ID_EDICIN_PATRN_COPIAR
Command18=ID_EDICIN_PISTA_COPIAR
Command19=ID_EDICIN_BLOQUE_CORTAR
Command20=ID_EDICIN_PATRN_CORTAR
Command21=ID_EDICIN_PISTA_CORTAR
Command22=ID_EDICIN_BLOQUE_PEGAR
Command23=ID_EDICIN_PATRN_PEGAR
Command24=ID_EDICIN_PISTA_PEGAR
Command25=ID_EDICIN_BLOQUE_BORRAR
Command26=ID_EDICIN_PATRN_BORRAR
Command27=ID_EDICIN_PISTA_BORRAR
Command28=ID_BPM_DEC
CommandCount=28

