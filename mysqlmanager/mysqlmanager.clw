; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMySqlManagerView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mysqlmanager.h"
LastPage=0

ClassCount=11
Class1=CChildFrame
Class2=CMainFrame
Class3=CMySqlManagerApp
Class4=CAboutDlg
Class5=CMySqlManagerDoc
Class6=CMySqlManagerView
Class7=CToolSql
Class8=CToolSqlQuery
Class9=CToolSqlResults

ResourceCount=16
Resource1=IDD_TOOL_SQL_QUERY
Resource2=IDR_MYSQLMTYPE (English (U.S.))
Resource3=IDD_TOOL_SQL
Resource4=IDD_ABOUTBOX
Resource5=IDD_REGISTER_SERVER
Resource6=IDR_MYSQLMTYPE
Class10=CRegisterServer
Resource7=IDD_TOOL_SQL_RESULTS
Class11=CToolSqlStatus
Resource8=IDD_TOOL_SQL_STATUS
Resource9=IDD_TOOL_SQL_QUERY (English (U.S.))
Resource10=IDD_REGISTER_SERVER (English (U.S.))
Resource11=IDD_TOOL_SQL_STATUS (English (U.S.))
Resource12=IDD_TOOL_SQL_RESULTS (English (U.S.))
Resource13=IDR_MAINFRAME (English (U.S.))
Resource14=IDD_TOOL_SQL (English (U.S.))
Resource15=IDD_ABOUTBOX (English (U.S.))
Resource16=IDR_MAINFRAME

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=CMainFrame

[CLS:CMySqlManagerApp]
Type=0
BaseClass=CWinApp
HeaderFile=MySqlManager.h
ImplementationFile=MySqlManager.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MySqlManager.cpp
ImplementationFile=MySqlManager.cpp
LastObject=CAboutDlg

[CLS:CMySqlManagerDoc]
Type=0
BaseClass=CDocument
HeaderFile=MySqlManagerDoc.h
ImplementationFile=MySqlManagerDoc.cpp

[CLS:CMySqlManagerView]
Type=0
BaseClass=CTreeView
HeaderFile=MySqlManagerView.h
ImplementationFile=MySqlManagerView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CMySqlManagerView

[CLS:CToolSql]
Type=0
BaseClass=CDialog
HeaderFile=ToolSql.h
ImplementationFile=ToolSql.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_TIMER_SECS

[CLS:CToolSqlQuery]
Type=0
BaseClass=CDialog
HeaderFile=ToolSqlQuery.h
ImplementationFile=ToolSqlQuery.cpp
Filter=D
VirtualFilter=dWC
LastObject=CToolSqlQuery

[CLS:CToolSqlResults]
Type=0
BaseClass=CDialog
HeaderFile=ToolSqlResults.h
ImplementationFile=ToolSqlResults.cpp
LastObject=CToolSqlResults
Filter=D
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342312448

[DLG:IDD_TOOL_SQL_QUERY]
Type=1
Class=CToolSql
ControlCount=1
Control1=IDC_EDIT,edit,1352732868

[DLG:IDD_TOOL_SQL_RESULTS]
Type=1
Class=CToolSqlResults
ControlCount=1
Control1=IDC_EDIT,edit,1352728772

[DLG:IDD_TOOL_SQL]
Type=1
Class=CToolSql
ControlCount=12
Control1=IDOK,button,1073807360
Control2=IDCANCEL,button,1342242817
Control3=IDC_TAB1,SysTabControl32,1342177280
Control4=IDC_QUERY_PB,button,1342242827
Control5=IDC_FONT_PB,button,1342242827
Control6=IDC_SERVER_CB,combobox,1344340226
Control7=IDC_DATABASES_PB,button,1342242827
Control8=IDC_START_PB,button,1342242817
Control9=IDC_STOP_PB,button,1342242817
Control10=IDC_TIMER_SECS,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_CLEAR,button,1342242819

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MYSQLMTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=IDM_SQL_TOOL_QUERY
Command16=IDM_TOOLS_REGISTER_SERVER
Command17=IDM_TOOLS_SERVER_PROPERTIES
Command18=ID_VIEW_TOOLBAR
Command19=ID_VIEW_STATUS_BAR
Command20=ID_WINDOW_NEW
Command21=ID_WINDOW_CASCADE
Command22=ID_WINDOW_TILE_HORZ
Command23=ID_WINDOW_ARRANGE
Command24=ID_APP_ABOUT
CommandCount=24

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=IDM_QUERY_DATABASES
Command3=IDM_QUERY_EXEC
Command4=ID_FILE_NEW
Command5=ID_FILE_OPEN
Command6=ID_FILE_PRINT
Command7=ID_FILE_SAVE
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_NEXT_PANE
Command12=ID_PREV_PANE
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_CUT
Command16=IDM_QUERY_EXEC
Command17=ID_EDIT_UNDO
CommandCount=17

[DLG:IDD_REGISTER_SERVER]
Type=1
Class=CRegisterServer
ControlCount=22
Control1=65535,button,1342177287
Control2=65535,static,1342308352
Control3=ID_SERVER_CB,combobox,1344340290
Control4=65535,static,1342308352
Control5=ID_HOST_CB,combobox,1344340290
Control6=65535,static,1342308352
Control7=ID_PORT_CB,combobox,1344340290
Control8=65535,button,1342177287
Control9=ID_USE_STANDARD_CK,button,1342308361
Control10=ID_USE_TRUSTED_CK,button,1342177289
Control11=1060,static,1342308352
Control12=ID_USER,edit,1350762624
Control13=1061,static,1342308352
Control14=ID_PASSWORD,edit,1350631584
Control15=65535,static,1342308352
Control16=ID_DISPLAY_SERVER_STATUS_CK,button,1342242819
Control17=IDOK,button,1342242817
Control18=ID_REMOVE_PB,button,1342242816
Control19=IDCANCEL,button,1342242816
Control20=ID_HELP,button,1342242816
Control21=ID_SERVERS_PB,button,1342242816
Control22=ID_GROUPS_PB,button,1342242816

[CLS:CRegisterServer]
Type=0
HeaderFile=RegisterServer.h
ImplementationFile=RegisterServer.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=1060

[DLG:IDD_TOOL_SQL_STATUS]
Type=1
Class=CToolSqlStatus
ControlCount=1
Control1=IDC_EDIT,edit,1352728772

[CLS:CToolSqlStatus]
Type=0
HeaderFile=ToolSqlStatus.h
ImplementationFile=ToolSqlStatus.cpp
BaseClass=CDialog
Filter=D
LastObject=CToolSqlStatus
VirtualFilter=dWC

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MYSQLMTYPE (English (U.S.))]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=IDM_SQL_TOOL_QUERY
Command16=IDM_TOOLS_REGISTER_SERVER
Command17=IDM_TOOLS_SERVER_PROPERTIES
Command18=ID_VIEW_TOOLBAR
Command19=ID_VIEW_STATUS_BAR
Command20=ID_WINDOW_NEW
Command21=ID_WINDOW_CASCADE
Command22=ID_WINDOW_TILE_HORZ
Command23=ID_WINDOW_ARRANGE
Command24=ID_APP_ABOUT
CommandCount=24

[DLG:IDD_TOOL_SQL_QUERY (English (U.S.))]
Type=1
ControlCount=1
Control1=IDC_EDIT,edit,1352732868

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Command1=ID_EDIT_COPY
Command2=IDM_QUERY_DATABASES
Command3=IDM_QUERY_EXEC
Command4=ID_FILE_NEW
Command5=ID_FILE_OPEN
Command6=ID_FILE_PRINT
Command7=ID_FILE_SAVE
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_NEXT_PANE
Command12=ID_PREV_PANE
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_CUT
Command16=IDM_QUERY_EXEC
Command17=ID_EDIT_UNDO
CommandCount=17

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342312448

[DLG:IDD_TOOL_SQL (English (U.S.))]
Type=1
ControlCount=12
Control1=IDOK,button,1073807360
Control2=IDCANCEL,button,1342242817
Control3=IDC_TAB1,SysTabControl32,1342177280
Control4=IDC_QUERY_PB,button,1342242827
Control5=IDC_FONT_PB,button,1342242827
Control6=IDC_SERVER_CB,combobox,1344340226
Control7=IDC_DATABASES_PB,button,1342242827
Control8=IDC_START_PB,button,1342242817
Control9=IDC_STOP_PB,button,1342242817
Control10=IDC_TIMER_SECS,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_CLEAR,button,1342242819

[DLG:IDD_TOOL_SQL_RESULTS (English (U.S.))]
Type=1
ControlCount=1
Control1=IDC_EDIT,edit,1352728772

[DLG:IDD_REGISTER_SERVER (English (U.S.))]
Type=1
ControlCount=22
Control1=65535,button,1342177287
Control2=65535,static,1342308352
Control3=ID_SERVER_CB,combobox,1344340290
Control4=65535,static,1342308352
Control5=ID_HOST_CB,combobox,1344340290
Control6=65535,static,1342308352
Control7=ID_PORT_CB,combobox,1344340290
Control8=65535,button,1342177287
Control9=ID_USE_STANDARD_CK,button,1342308361
Control10=ID_USE_TRUSTED_CK,button,1342177289
Control11=1060,static,1342308352
Control12=ID_USER,edit,1350762624
Control13=1061,static,1342308352
Control14=ID_PASSWORD,edit,1350631584
Control15=65535,static,1342308352
Control16=ID_DISPLAY_SERVER_STATUS_CK,button,1342242819
Control17=IDOK,button,1342242817
Control18=ID_REMOVE_PB,button,1342242816
Control19=IDCANCEL,button,1342242816
Control20=ID_HELP,button,1342242816
Control21=ID_SERVERS_PB,button,1342242816
Control22=ID_GROUPS_PB,button,1342242816

[DLG:IDD_TOOL_SQL_STATUS (English (U.S.))]
Type=1
ControlCount=1
Control1=IDC_EDIT,edit,1352728772


