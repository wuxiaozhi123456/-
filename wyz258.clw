; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFftBatterFilterDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "wyz258.h"
LastPage=0

ClassCount=9
Class1=CWyz258App
Class2=CWyz258Doc
Class3=CWyz258View
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_DIALOG2
Class5=CAboutDlg
Resource2=IDD_DIALOG3
Class6=CHistogramDlg
Resource3=IDD_DIALOG1
Class7=CLineTransDlg
Resource4=IDR_MAINFRAME
Class8=CFftFilterDlg
Resource5=IDD_ABOUTBOX
Class9=CFftBatterFilterDlg
Resource6=IDD_DIALOG4

[CLS:CWyz258App]
Type=0
HeaderFile=wyz258.h
ImplementationFile=wyz258.cpp
Filter=N

[CLS:CWyz258Doc]
Type=0
HeaderFile=wyz258Doc.h
ImplementationFile=wyz258Doc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CWyz258Doc

[CLS:CWyz258View]
Type=0
HeaderFile=wyz258View.h
ImplementationFile=wyz258View.cpp
Filter=C
LastObject=CWyz258View
BaseClass=CScrollView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_RSET
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=wyz258.cpp
ImplementationFile=wyz258.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_GRAY
Command10=ID_HISTOGRAMDLG
Command11=ID_LINETRANS
Command12=ID_EQUALIZE
Command13=ID_FT
Command14=ID_IFT
Command15=ID_FFT
Command16=ID_IFFT
Command17=ID_RSET
Command18=ID_MEAN
Command19=ID_MEDIAN
Command20=ID_GRADIENT
Command21=ID_LAPLACIAN
Command22=ID_IDEA
Command23=ID_BUTTER
Command24=ID_GAUSS
CommandCount=24

[DLG:IDD_DIALOG1]
Type=1
Class=CHistogramDlg
ControlCount=0

[CLS:CHistogramDlg]
Type=0
HeaderFile=HistogramDlg.h
ImplementationFile=HistogramDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CHistogramDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG2]
Type=1
Class=CLineTransDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_GRADIENT,edit,1350631552
Control5=IDC_INTERCEPT,edit,1350631552

[CLS:CLineTransDlg]
Type=0
HeaderFile=LineTransDlg.h
ImplementationFile=LineTransDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_DIALOG3]
Type=1
Class=CFftFilterDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT1,edit,1350631552

[CLS:CFftFilterDlg]
Type=0
HeaderFile=FftFilterDlg.h
ImplementationFile=FftFilterDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CFftFilterDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG4]
Type=1
Class=CFftBatterFilterDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_EDIT2,edit,1350631552

[CLS:CFftBatterFilterDlg]
Type=0
HeaderFile=FftBatterFilterDlg.h
ImplementationFile=FftBatterFilterDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CFftBatterFilterDlg
VirtualFilter=dWC

