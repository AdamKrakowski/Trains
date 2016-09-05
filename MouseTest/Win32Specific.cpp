
#include "stdafx.h"
#include "MouseTest.h"

#include "Windowsx.h"

#include "VObjCollection.h"

extern CCollection mVObjCollections;

#include "Debug.h"
extern CDebug mDebug;

void CreateButtonsObj();
void NewAddObjModeSet(  int newmode );

extern RECT  client_rect;
extern HWND mhWnd;
extern int isrect;

HWND  hwndButtonStatic;
HWND  hwndCombo; 
 
#define  B_WIDE 28
#define  LHIGH   60

 

typedef struct
{
  HWND mHWND;
  int  mXpos;
  int  mYpos;
  int  mXsize;
  int  mYsize;
  int  mId;
  int  bmup;
  int  bmdn;
  int  bmgr;
  char *text;
} BtnObjAsd;

#define B_HIGH  28

BtnObjAsd sTBL_tStandard[]=
{ 
// grouip select	
	{NULL , 360,0,58,B_HIGH, IDM_SELECT_OBJECT		,IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Sel Obj"},
	{NULL , 420,0,58,B_HIGH, IDM_SELECT_REGION		,IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Sel Rgn"},
	{NULL , 480,0,58,B_HIGH, IDM_SELECT_2REGIONS	,IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Sel 2Pt"},
// group mode
	{NULL , 550,0,58,B_HIGH, IDM_MODE_EDIT ,		IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Edit"},
	{NULL , 610,0,58,B_HIGH, IDM_MODE_ADD		 ,	IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Add"},
	{NULL , 670,0,58,B_HIGH, IDM_MODE_CNTR ,		IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Control"},
// group control
 	{NULL , 740,0,50,B_HIGH, IDM_TRAIN_STEP ,		IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Step"},
	{NULL , 790,0,50,B_HIGH, IDM_TRAIN_START ,		IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Start"},
	{NULL , 840,0,50,B_HIGH, IDM_TRAIN_FAST ,		IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Fast"},
	{NULL , 890,0,50,B_HIGH, IDM_TRAIN_SLOW ,		IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Slow"},
	{NULL , 940,0,50,B_HIGH, IDM_TRAIN_STOP ,		IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Stop"},
	{NULL , 990,0,50,B_HIGH, IDM_TRAIN_DIR ,		IDB_BITMAP_S0U   , IDB_BITMAP_S0D, IDB_BITMAP_DIS,"Dir"},
// null terminator
	{NULL,0,0,0,0,0 ,0 , 0, 0, ""}, 
};

#define B_OBJ_WIDE  56
#define BLOCOWIDE  30

BtnObjAsd sTBL_tnObjAdd[]=
{   {NULL,0,0,		B_OBJ_WIDE,B_HIGH,IDM_ADD_STRAIGHT ,IDB_BITMAP_S0U   ,	IDB_BITMAP_BDN_H, IDB_BITMAP_BUP_H,"Stright"},
	{NULL,56,0,		B_OBJ_WIDE,B_HIGH,IDM_ADD_BEND_LEFT ,IDB_BITMAP_B0UL  , IDB_BITMAP_BDN_H, IDB_BITMAP_BUP_H,"Bend L"},
	{NULL,112,0,	B_OBJ_WIDE,B_HIGH,IDM_ADD_BEND_RIGHT ,IDB_BITMAP_B0U ,	IDB_BITMAP_BDN_H, IDB_BITMAP_BUP_H, "Bend R"},
	{NULL,168,0,	B_OBJ_WIDE,B_HIGH,IDM_ADD_BEND_LEFT2,IDB_BITMAP_B1UL ,	IDB_BITMAP_BDN_H, IDB_BITMAP_BUP_H,"Bend L"},
	{NULL,224,0,	B_OBJ_WIDE,B_HIGH,IDM_ADD_BEND_RIGHT2 ,IDB_BITMAP_B1U , IDB_BITMAP_BDN_H, IDB_BITMAP_BUP_H,"Bend R"},
	{NULL,280,0,	B_OBJ_WIDE,B_HIGH,IDM_ADD_BEND_LEFT3,IDB_BITMAP_B2UL ,	IDB_BITMAP_BDN_H, IDB_BITMAP_BUP_H , "Bend L"}, 
	
	{NULL,0,30,		B_OBJ_WIDE,B_HIGH,IDM_ADD_BEND_RIGHT3,IDB_BITMAP_B2U ,	IDB_BITMAP_BDN_H, IDB_BITMAP_BUP_H , "Bend R"}, 
	{NULL,56,30,	B_OBJ_WIDE,B_HIGH,IDM_ADD_CROSS_LEFT ,IDB_BITMAP_W0UL , IDB_BITMAP_BDN_H , IDB_BITMAP_BUP_H, "Sw L"}, 
	{NULL,112,30,	B_OBJ_WIDE,B_HIGH,IDM_ADD_CROSS_RIGHT,IDB_BITMAP_W0U ,	IDB_BITMAP_BDN_H, IDB_BITMAP_BUP_H , "Sw R"}, 
	{NULL,168,30,	B_OBJ_WIDE,B_HIGH,IDM_ADD_CROSS_LEFT2 ,IDB_BITMAP_W1UL, IDB_BITMAP_BDN_H, IDB_BITMAP_BUP_H, "Sw L"}, 
	{NULL,224,30,	B_OBJ_WIDE,B_HIGH,IDM_ADD_CROSS_RIGHT2,IDB_BITMAP_W1U , IDB_BITMAP_BDN_H, IDB_BITMAP_BUP_H , "Sw R"}, 

	{NULL,280,30,	B_OBJ_WIDE,B_HIGH,IDM_ADD_STOP ,IDB_BITMAP_E0U , IDB_BITMAP_BDN_H , IDB_BITMAP_BUP_H, "End"}, 


	{NULL,0, 60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL0 ,IDB_BITMAP_E0   , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V,"L0"},
	{NULL,30,60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL1 ,IDB_BITMAP_E1  , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V,"L1"},
	{NULL,60,60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL2 ,IDB_BITMAP_E2 , IDB_BITMAP_BDN_V,   IDB_BITMAP_BUP_V, "L2"},
	{NULL,90,60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL3,IDB_BITMAP_E3 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V,"L3"},
	{NULL,120,60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL4 ,IDB_BITMAP_E4 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V,"L4"},
	{NULL,150,60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL5,IDB_BITMAP_E5 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V , "L5"}, 
	{NULL,180,60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL6,IDB_BITMAP_E6 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V , "L6"}, 
	{NULL,210,60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL7 ,IDB_BITMAP_E7 , IDB_BITMAP_BDN_V , IDB_BITMAP_BUP_V, "L7"}, 
	{NULL,240,60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL8,IDB_BITMAP_E8 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V , "L8"}, 
	{NULL,270,60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL9 ,IDB_BITMAP_E9 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "L9"}, 
	{NULL,300,60,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADDL10 ,IDB_BITMAP_E10 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "L10"}, 



	{NULL, 0,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD0 ,IDB_BITMAP_V0 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V0"}, 
	{NULL,30,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD1 ,IDB_BITMAP_V1 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V1"}, 
	{NULL,60,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD2 ,IDB_BITMAP_V2 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V2"}, 
	{NULL,90,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD3 ,IDB_BITMAP_V3 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V3"}, 
	{NULL,120,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD4 ,IDB_BITMAP_V4 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V4"}, 
	{NULL,150,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD5 ,IDB_BITMAP_V5 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V5"}, 
	{NULL,180,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD6 ,IDB_BITMAP_V6 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V6"}, 
	{NULL,210,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD7 ,IDB_BITMAP_V7 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V7"}, 
	{NULL,240,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD8 ,IDB_BITMAP_V8 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V8"}, 
	{NULL,270,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD9 ,IDB_BITMAP_V9 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V9"},  
	{NULL,300,120,	BLOCOWIDE,LHIGH,IDM_TRAIN_ADD10 ,IDB_BITMAP_V10 , IDB_BITMAP_BDN_V, IDB_BITMAP_BUP_V, "V10"}, 

	{NULL,0,0,0,0,0 ,0 , 0, 0, ""}, 
};


int mMode = 0;
int selmode=0 ;

void NewModeSelectSet( int newmode )
{
  Button_SetState( sTBL_tStandard[0].mHWND  ,   ( newmode  == IDM_SELECT_OBJECT )  ?  BST_CHECKED :  BST_UNCHECKED );
  Button_SetState( sTBL_tStandard[1].mHWND  ,   ( newmode  == IDM_SELECT_REGION )  ?  BST_CHECKED :  BST_UNCHECKED );
  Button_SetState( sTBL_tStandard[2].mHWND  ,   ( newmode  == IDM_SELECT_2REGIONS )  ?  BST_CHECKED :  BST_UNCHECKED );
  selmode = newmode;
  isrect =0;
}

int GetSelectMode()
{
	return selmode;
}

void NewModeSet( int newmode )
{
  Button_SetState( sTBL_tStandard[3].mHWND  ,	( newmode  == IDM_MODE_EDIT )  ?  BST_CHECKED :  BST_UNCHECKED );
  Button_SetState( sTBL_tStandard[4].mHWND  ,   ( newmode  == IDM_MODE_ADD	)  ?  BST_CHECKED :  BST_UNCHECKED );
  Button_SetState( sTBL_tStandard[5].mHWND  ,   ( newmode  == IDM_MODE_CNTR )  ?  BST_CHECKED :  BST_UNCHECKED );

  Button_Enable( sTBL_tStandard[6].mHWND  ,   ( newmode  == IDM_MODE_CNTR )  ?  TRUE :  FALSE );
  Button_Enable( sTBL_tStandard[7].mHWND  ,   ( newmode  == IDM_MODE_CNTR )  ?  TRUE :  FALSE );
  Button_Enable( sTBL_tStandard[8].mHWND  ,   ( newmode  == IDM_MODE_CNTR )  ?  TRUE :  FALSE );
  Button_Enable( sTBL_tStandard[9].mHWND  ,   ( newmode  == IDM_MODE_CNTR )  ?  TRUE :  FALSE );
  Button_Enable( sTBL_tStandard[10].mHWND ,   ( newmode  == IDM_MODE_CNTR )  ?  TRUE :  FALSE );
  Button_Enable( sTBL_tStandard[11].mHWND ,   ( newmode  == IDM_MODE_CNTR )  ?  TRUE :  FALSE );

  Button_Enable( sTBL_tStandard[0].mHWND  ,   ( newmode  == IDM_MODE_EDIT )  ?  TRUE :  FALSE );
  Button_Enable( sTBL_tStandard[1].mHWND ,	( newmode  == IDM_MODE_EDIT )  ?  TRUE :  FALSE );
  Button_Enable( sTBL_tStandard[2].mHWND  ,   ( newmode  == IDM_MODE_EDIT )  ?  TRUE :  FALSE );


  HMENU hmenu = GetMenu( mhWnd   );


  EnableMenuItem( hmenu , IDM_ADD_STRAIGHT ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_BEND_LEFT ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_BEND_RIGHT ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_BEND_LEFT2 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_BEND_RIGHT2 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_BEND_LEFT3 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_BEND_RIGHT3 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_CROSS_LEFT ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_CROSS_RIGHT ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_CROSS_LEFT2 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_CROSS_RIGHT2 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_ADD_STOP ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);



  EnableMenuItem( hmenu , IDM_TRAIN_ADDL0 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADDL1 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADDL2 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADDL3 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADDL4 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADDL5 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADDL6 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADDL7 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADDL8 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADDL9 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADDL10 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);

  EnableMenuItem( hmenu , IDM_TRAIN_ADD0 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADD1 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADD2 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADD3 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADD4 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADD5 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADD6 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADD7 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADD8 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADD9 ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_ADD10,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);

  EnableMenuItem( hmenu , IDM_TRAIN_DIR ,  ( newmode  == IDM_MODE_CNTR )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_STEP ,  ( newmode  == IDM_MODE_CNTR )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_START ,  ( newmode  == IDM_MODE_CNTR )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_STOP ,  ( newmode  == IDM_MODE_CNTR )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_SLOW ,  ( newmode  == IDM_MODE_CNTR )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TRAIN_FAST ,  ( newmode  == IDM_MODE_CNTR )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
 

  EnableMenuItem( hmenu , IDM_SELECT_OBJECT ,  ( newmode  == IDM_MODE_EDIT )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_SELECT_REGION ,  ( newmode  == IDM_MODE_EDIT )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_SELECT_2REGIONS ,  ( newmode  == IDM_MODE_EDIT )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
 
  EnableMenuItem( hmenu , IDM_TOOLS_UNDO ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_TOOLS_UNDOVAGON ,  ( newmode  == IDM_MODE_ADD )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);

  EnableMenuItem( hmenu , IDM_MODIFY_INFO ,  ( newmode  == IDM_MODE_EDIT )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_MODIFY_DELETE ,  ( newmode  == IDM_MODE_EDIT )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_MODIFY_SNAP ,  ( newmode  == IDM_MODE_EDIT )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);
  EnableMenuItem( hmenu , IDM_MODIFY_SNAP_X ,  ( newmode  == IDM_MODE_EDIT )  ?  MF_ENABLED :  MF_DISABLED  | MF_GRAYED);

  mMode = newmode;

  NewAddObjModeSet(   newmode );

}

void InvalidateActiveWindow()
{
	GetClientRect( mhWnd , &client_rect );
	RECT rc = client_rect;
	rc.top  = 60;
	rc.left = 60;
	InvalidateRect( mhWnd , &rc , FALSE );
}

int GetCurrentMode()
{ return mMode;}

int GetSelectedLocoId( int option )
{
	int index =  ComboBox_GetCurSel( hwndCombo);
	int id = ComboBox_GetItemData(hwndCombo  , index );
	return id;
}

void HandleLocoHit( int idd )
{
	int  count = ComboBox_GetCount(hwndCombo);
	int i;
	for ( i =0 ;i < count;i++)
	{
		int id = ComboBox_GetItemData(hwndCombo, i);
		if ( id == idd ) { ComboBox_SetCurSel(hwndCombo,i ); break;}
	}
}


  HWND mRailHWND;

LRESULT CALLBACK	WndProcR(HWND, UINT, WPARAM, LPARAM);
void  DrawButtonCRail( HINSTANCE  hInst,  DRAWITEMSTRUCT *d );

TCHAR szWindowRClass[]="RAILS";

void CreateRButtons( HWND mRailHWND);

HINSTANCE hRInstance=NULL;

ATOM MyRegisterRClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProcR;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
 	wcex.hIcon			= NULL; //LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOUSETEST));
 	wcex.hCursor		= NULL; //LoadCursor(NULL, IDC_ARROW);
	
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; // MAKEINTRESOURCE(IDC_MOUSETEST);
	wcex.lpszClassName	= szWindowRClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	//HCURSOR stdc = wcex.hCursor;

	hRInstance = hInstance;
	return RegisterClassEx(&wcex);
}

void OnDrawBgnR( HWND hWnd , HDC hdc )
{
//  create solid window background - Green
    RECT  clientRect;
	HRGN bgRgn;
    HBRUSH hBrush;

	GetClientRect(hWnd, &clientRect);
    bgRgn = CreateRectRgnIndirect(&clientRect);

		if ( bgRgn == NULL ) 
		return;
 
    hBrush = CreateSolidBrush(RGB( 0 , 240, 240));

	if ( hBrush == NULL ) 
		return;

    FillRgn(hdc, bgRgn, hBrush);

	DeleteObject( bgRgn );
	DeleteObject( hBrush );
	GetStockObject(WHITE_BRUSH);
}
 

LRESULT CALLBACK WndProcR(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//mDebug.CDebugPut("WndProcR %X ",(int)message  );
	int wmId ;
	PAINTSTRUCT ps;
	HDC hdc;
 
	switch (message)
	{
	case WM_CREATE:
	 			break;
	case WM_ERASEBKGND:
        return true;
	case WM_DRAWITEM:
		DrawButtonCRail(   hRInstance, (DRAWITEMSTRUCT *)lParam );
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		//wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ADD_STRAIGHT:	
		case IDM_ADD_BEND_LEFT:
		case IDM_ADD_BEND_RIGHT:
		case IDM_ADD_BEND_LEFT2:
		case IDM_ADD_BEND_RIGHT2:
		case IDM_ADD_BEND_LEFT3:
		case IDM_ADD_BEND_RIGHT3:
		case IDM_ADD_CROSS_LEFT:
		case IDM_ADD_CROSS_RIGHT: 
		case IDM_ADD_CROSS_LEFT2:
		case IDM_ADD_CROSS_RIGHT2:
		case IDM_ADD_STOP:
		case IDM_TRAIN_ADDL0:
		case IDM_TRAIN_ADDL1:
		case IDM_TRAIN_ADDL2:
		case IDM_TRAIN_ADDL3:
		case IDM_TRAIN_ADDL4:
		case IDM_TRAIN_ADDL5:
		case IDM_TRAIN_ADDL6:
		case IDM_TRAIN_ADDL7:
		case IDM_TRAIN_ADDL8:
		case IDM_TRAIN_ADDL9:
		case IDM_TRAIN_ADDL10:
		case IDM_TRAIN_ADD0:
		case IDM_TRAIN_ADD1:
		case IDM_TRAIN_ADD2:
		case IDM_TRAIN_ADD3:
		case IDM_TRAIN_ADD4:
		case IDM_TRAIN_ADD5:
		case IDM_TRAIN_ADD6:
		case IDM_TRAIN_ADD7:
		case IDM_TRAIN_ADD8:
		case IDM_TRAIN_ADD9:
		case IDM_TRAIN_ADD10:
		SendMessage(mhWnd ,WM_COMMAND ,wmId,0);
		SetFocus(mhWnd);
		break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	case WM_PAINT:
	 	hdc = BeginPaint(hWnd, &ps);
		{
			OnDrawBgnR(   hWnd ,   hdc );
		}
	 	EndPaint(hWnd, &ps);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void  ShowObjWindow( int show)
{

	ShowWindow( mRailHWND , show );
}

void CreateRailWindow()
{
		mRailHWND = CreateWindowEx( 
		 0,
		 szWindowRClass ,//"BUTTON",  // Predefined class; Unicode assumed 
		"Objects", //   "Stright",      // Button text 
		// WS_VISIBLE   | WS_OVERLAPPED ,  // Styles 
		 WS_OVERLAPPED ,
		//WS_CHILD | WS_VISIBLE | WS_CAPTION,
		0,0,380,204,
		 mhWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(mhWnd, GWL_HINSTANCE), 
		NULL);	

	 	CreateRButtons(mRailHWND);

//mDebug.CDebugPut("CreateRailWindow %X ",(int)mRailHWND );

}



void CreateButtons()
{
	HWND hWnd = mhWnd;
 
	int i;
	for (i=0;i<12;i++)
	{
		if (sTBL_tStandard[i].mId == 0 ) break;
		sTBL_tStandard[i].mHWND = CreateWindowEx( 
		 0,
		"BUTTON",  // Predefined class; Unicode assumed 
		sTBL_tStandard[i].text, //   "Stright",      // Button text 
		//WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		sTBL_tStandard[i].mXpos,         // x position 
		sTBL_tStandard[i].mYpos,        // y position 
		sTBL_tStandard[i].mXsize, // 56,        // Button width
		sTBL_tStandard[i].mYsize,   // B_HIGH,        // Button height
		 mhWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(mhWnd, GWL_HINSTANCE), 
		NULL);
		SetWindowLongPtr(sTBL_tStandard[i].mHWND, GWLP_ID, static_cast<LONG_PTR>(static_cast<DWORD_PTR>(sTBL_tStandard[i].mId))) ;
	}

   hwndCombo = CreateWindowEx( 
	 0,
     "COMBOBOX",  // Predefined class; Unicode assumed 
     "Combo",      // Button text 
    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_DISABLED  | CBS_DROPDOWN ,  // Styles  enbale if at leas one loco is added 
     1040,         // x position 
     2,         // y position 
    76,        // Button width
    100,        // Button height
    hWnd,     // Parent window
    NULL,       // No menu.
    (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
    NULL);

     hwndButtonStatic = CreateWindowEx( 
	 0,
     "STATIC",  // Predefined class; Unicode assumed 
     "Status:OK",      // Button text 
    WS_TABSTOP | WS_VISIBLE | WS_CHILD ,  // Styles 
     1120,         // x position 
     2,         // y position 
    96,        // Button width
    24,        // Button height
    hWnd,     // Parent window
    NULL,       // No menu.
    (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
    NULL);
 
	CreateButtonsObj();

    NewModeSet( IDM_MODE_EDIT );
	NewAddObjModeSet( IDM_MODE_EDIT) ;

   return ;
}

 
void NewAddObjModeSet(  int newmode ) 
{
 	int i=0;
	while (sTBL_tnObjAdd[i].mId != 0)
	 	Button_Enable(sTBL_tnObjAdd[i++].mHWND ,   ( newmode  == IDM_MODE_ADD )  ?  TRUE :  FALSE );
}


void	CheckAddObject( int newmode )
{
   	int i=0;
	while (sTBL_tnObjAdd[i].mId != 0)
	{
		Button_SetState( sTBL_tnObjAdd[i].mHWND ,   ( newmode  == sTBL_tnObjAdd[i].mId  )  ?  BST_CHECKED :  BST_UNCHECKED );
		i++;
	}
}

// create all ADD buttons

void CreateButtonsObj()
{ 
	int i=0,ypos =0;


	/*
	while( 1)
	{
		if (sTBL_tnObjAdd[i].mId == 0 ) break;
		sTBL_tnObjAdd[i].mHWND = CreateWindowEx( 
		 0,
		 "BUTTON",  // Predefined class; Unicode assumed 
		 sTBL_tnObjAdd[i].text, //   "Stright",      // Button text 
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		 //WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		sTBL_tnObjAdd[i].mXpos,         // x position 
		sTBL_tnObjAdd[i].mYpos,        // y position 
		sTBL_tnObjAdd[i].mXsize, // 56,        // Button width
		sTBL_tnObjAdd[i].mYsize,   //  B_HIGH,        // Button height
		mhWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(mhWnd, GWL_HINSTANCE), 
		NULL);
		SetWindowLongPtr(sTBL_tnObjAdd[i].mHWND, GWLP_ID, static_cast<LONG_PTR>(static_cast<DWORD_PTR>(sTBL_tnObjAdd[i].mId))) ;
		i++;
	}
	*/
}

void CreateRButtons(HWND mRailHWND)
{
	int i=0,ypos =0;

	while( 1)
	{
		if (sTBL_tnObjAdd[i].mId == 0 ) break;
		sTBL_tnObjAdd[i].mHWND = CreateWindowEx( 
		 0,
		 "BUTTON",  // Predefined class; Unicode assumed 
		 sTBL_tnObjAdd[i].text, //   "Stright",      // Button text 
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		 //WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		sTBL_tnObjAdd[i].mXpos,         // x position 
		sTBL_tnObjAdd[i].mYpos,        // y position 
		sTBL_tnObjAdd[i].mXsize, // 56,        // Button width
		sTBL_tnObjAdd[i].mYsize,   //  B_HIGH,        // Button height
		mRailHWND,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(mhWnd, GWL_HINSTANCE), 
		NULL);
		SetWindowLongPtr(sTBL_tnObjAdd[i].mHWND, GWLP_ID, static_cast<LONG_PTR>(static_cast<DWORD_PTR>(sTBL_tnObjAdd[i].mId))) ;
		i++;
	}

}




void  DrawButtonCRail( HINSTANCE  hInst,  DRAWITEMSTRUCT *d )
{
	HDC   MemDCButton;
	HBITMAP	bmpButton = NULL ;
	//  create compatible DC to button DC
	MemDCButton = CreateCompatibleDC(d->hDC );
    
	int	i=0;
	int  idup;
	int  idwn;
	int  idgr;
	int  ibXsize;
	int  ibYsize;
	char ct;
	int  ctn;
 
	while( 1)  
	{
		if (sTBL_tnObjAdd[i].mId == 0 ) return;
		if (sTBL_tnObjAdd[i].mHWND == d->hwndItem )
		{
			idup =sTBL_tnObjAdd[i].bmup;
			idwn =sTBL_tnObjAdd[i].bmdn;
			idgr= sTBL_tnObjAdd[i].bmgr;
			ibXsize = sTBL_tnObjAdd[i].mXsize;
			ibYsize = sTBL_tnObjAdd[i].mYsize;
			ct = sTBL_tnObjAdd[i].text[0];
			ctn = sTBL_tnObjAdd[i].text[1]-'0';

			if ( sTBL_tnObjAdd[i].text[2] >= '0' )
				ctn = 10 + sTBL_tnObjAdd[i].text[2]-'0';
 

	        break;
		}
		i++;
	}

	int grey=0;

	if ( d->itemAction ==  ODA_DRAWENTIRE  )
	{
		if (  d->itemState&ODS_DISABLED )
		{ bmpButton = LoadBitmap(hInst, MAKEINTRESOURCE(idgr)); grey=1;}
		else	if ( d->itemState&ODS_SELECTED )
				bmpButton = LoadBitmap(hInst, MAKEINTRESOURCE(idwn));   
		else
				bmpButton = LoadBitmap(hInst, MAKEINTRESOURCE(idgr));
	}
	else
	{
		if ( d->itemAction ==  ODA_SELECT  )
		{
			 if ( d->itemState&ODS_SELECTED )
			 	bmpButton = LoadBitmap(hInst, MAKEINTRESOURCE(idwn));
			 else
			 	bmpButton = LoadBitmap(hInst, MAKEINTRESOURCE(idgr));
		}
	}

	if ( bmpButton!= NULL )
	{
		SelectObject( MemDCButton, bmpButton );
		//   button  size
		BitBlt( d->hDC , d->rcItem.left ,d->rcItem.top  ,ibXsize , ibYsize ,MemDCButton ,   0,  0,  SRCCOPY);
		DeleteObject(bmpButton);

		if ( grey==0)
		{
			 if ( ct == 'L')
			 {
				bmpButton = (HBITMAP) mVObjCollections.GetEngineHandle(ctn);
				if ( bmpButton != NULL ) {
				SIZE cs =  mVObjCollections.GetEngineSize(ctn);
				SelectObject( MemDCButton, bmpButton );
				StretchBlt( d->hDC , d->rcItem.left+3,d->rcItem.top+3 , ibXsize-6 ,ibYsize-6, MemDCButton ,0,0, cs.cx,cs.cy ,SRCCOPY);
				}
			 }
			 else if ( ct == 'V')
			 {
				bmpButton =  (HBITMAP) mVObjCollections.GetVagonHandle(ctn);
				if ( bmpButton != NULL ) {
				SIZE cs =  mVObjCollections.GetVagonSize(ctn);
				SelectObject( MemDCButton, bmpButton );
				StretchBlt( d->hDC , d->rcItem.left+3,d->rcItem.top+3 , ibXsize-6 ,ibYsize-6, MemDCButton ,0,0, cs.cx,cs.cy ,SRCCOPY);
				}
			 }
			 else
			 {
				bmpButton = LoadBitmap(hInst, MAKEINTRESOURCE(idup));
				SelectObject( MemDCButton, bmpButton );
				BITMAP bm;
				GetObject ( bmpButton , sizeof(bm), &bm );
				StretchBlt( d->hDC , d->rcItem.left+3,d->rcItem.top+3 , ibXsize-6 ,ibYsize-6, MemDCButton ,0,0, bm.bmWidth,bm.bmHeight ,SRCCOPY);
				DeleteObject(bmpButton);
			 }
		}
	}
	DeleteDC(MemDCButton);
}

INT_PTR CALLBACK	GetFileName(HWND, UINT, WPARAM, LPARAM);

static char  *p_data_out; 
static int   dlen;

void GetDataFileNameToRead(  HINSTANCE  hInst,  HWND hWnd  ,char  *data_out , int  lengthOfBuffer )
{
	p_data_out = data_out; dlen = lengthOfBuffer;
	DialogBox(hInst, MAKEINTRESOURCE(IDD_GETFILENAME), hWnd, GetFileName); 

	//	 mDebug.CDebugPut("out:" );
	// mDebug.CDebugPut( p_data_out );

}

// Message handler for about box.
INT_PTR CALLBACK GetFileName(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
			HWND hList = GetDlgItem(hDlg, IDC_LISTBOX1);
			char path[MAX_PATH];
			GetModuleFileName( NULL ,path ,MAX_PATH);
			StrippedDir( path);
			strcat_s(path, MAX_PATH-20 ,"*.rls");
			WIN32_FIND_DATA  fdat;
			HANDLE h =  FindFirstFile  ( (LPCTSTR)path ,  &fdat);

			if ( h !=  INVALID_HANDLE_VALUE  )
			{
				while ( 1)
				{ 
					StrippedDir( path);
					strcat_s(path, MAX_PATH-20 ,fdat.cFileName);
					SendMessage( hList, LB_INSERTSTRING, 0 , (LPARAM)fdat.cFileName);
					if (  FindNextFile ( h , &fdat) == FALSE) break;
				}
			}
			return (INT_PTR)TRUE;
		}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			HWND hList = GetDlgItem(hDlg, IDC_LISTBOX1);

			int i = SendMessage( hList, LB_GETCURSEL , 0 , NULL);	

			if ( i >- 0 )
			{
			 //mDebug.CDebugPut( "messahe %d", i);

			 char text[32] ;
             SendMessage (hList, LB_GETTEXT, (WPARAM)i, (LPARAM)text);

			// mDebug.CDebugPut( text );
			 char path[MAX_PATH];
			GetModuleFileName( NULL ,path ,MAX_PATH);
			StrippedDir( path);
			strcat_s( path , MAX_PATH-20 , text );

		 ///mDebug.CDebugPut( path );

		    memcpy( p_data_out , path , dlen );
			}
			else
			{
				p_data_out[0]=0;
			}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
