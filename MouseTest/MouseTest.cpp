// MouseTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MouseTest.h"

#include "Windowsx.h"
#include "Math.h"
#include <stdio.h>

#include "VObjCollection.h"
#include "Foundation.h"
#include "ComputeGA.h"
#include "ObjectStore.h"
#include "Graphics.h"

#include "Debug.h"

#include "Win32Specific.h"

#include "MouseFcHandling.h"

#define T_MSEC 25

//  Aplication specific constant

#define APP_RADIUS   160
#define APP_WIDE	8



//  Global definition
#define MAX_LOADSTRING 100
// Global Variables:
HINSTANCE hInst;								// current instance  !!!!!!!!!! dup
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND mhWnd = NULL;

//  only mouse global variable
int    MouseState=0;

//  visual debug
int		RefLineToggle =0 ;   // move later to fundation
int		enable_refLine[4]={0,0,0,0}; 
CLineObject mRefLine[4] ;  


extern	CTwoPoints mTwoPoint;
extern	int  mTwoPointActive;


// debug objects
CDebug mDebug;
int showRect = 0; 
// end of debug 
 

enum {
	  FunctionTracker,
	  FuntionSelObject,
	  FunctionSelHandle,
	  FuntionRotObjectStp,
	  FuntionMoveObject,
	  FuntionRotObjectEnp,

	  FuntionSnap,
	  FunctionBreak,
	  FunctionAddStright,
	  FunctionAddBendR,
	  FunctionAddBendL,
	  FunctionAddSwitchR,
	  FunctionAddSwitchL,
	  FunctionAddEnd,
	  FuntionAddTrain,
	  FuntionAddSemaphore,
	  FuntionNone }; 

int mFunctionMode = FuntionNone ;

CMenuGroup  mGroupAdd ;
CMenuGroup  mGroupMod ;
CMenuGroup  mGroupTol ;
CMenuGroup  mGroupSel ;
CMenuGroupTable  mGroupMenuTab;

// application objects
CList  mList;
CList  mCntList;
CVagonList mVagonList;
CCollection mVObjCollections;
CTempDrawObjectStore mTempDrawObjs;
CTempVector mCTempVector ;
TMPOBJECT_DRAWDATASTRUCT mtmpObjS;
// global rect
RECT  client_rect;

 

void CSystem::SystemErrorMessage( int i_err ) 
{
	char etext[256];
	etext[0]=0;
	switch (i_err)
	{
		case 0:
			break;
		default :
			sprintf_s(  etext,200, "Error:%d",i_err);
		break;
	}
	SetWindowText( hwndButtonStatic , etext);
	system_error = i_err;
}

int CSystem::GetSystemError()
{
	return system_error;
}

CSystem  mSystem;

//  Global by definition
HINSTANCE hThisInstance;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	hThisInstance = hInstance;

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MOUSETEST, szWindowClass, MAX_LOADSTRING);
 	MyRegisterClass(hInstance);
	MyRegisterRClass(hInstance);

 	// open remote debug file
		{
			char path[MAX_PATH];
			GetModuleFileName( NULL ,path ,MAX_PATH);
			StrippedDir( path);
			strcat_s(path, MAX_PATH-20 ,"Debug.txt");
			mDebug.CDebugOpen( path);
			 
		}

		mDebug.CDebugPut("Log is started : ");




	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MOUSETEST));

 /*
		{
			char path[MAX_PATH];
			GetModuleFileName( NULL ,path ,MAX_PATH);
			StrippedDir( path);
			strcat_s(path, MAX_PATH-20 ,"Debug.txt");
			mDebug.CDebugOpen( path);
			 
		}

		mDebug.CDebugPut("Log is started : ");

		*/

	  mVObjCollections.LoadEnginees();
	  mVObjCollections.LoadVagons();


	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	mDebug.CDebugClose();

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//

HCURSOR hcc;
HCURSOR stdc;

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOUSETEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);


	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MOUSETEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

 
	HCURSOR stdc = wcex.hCursor;

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

	HMENU hmenu = GetMenu( hWnd   );

	mGroupAdd.CMenuGroupSet( hmenu , IDM_ADD_STRAIGHT, IDM_ADD_STOP);
	mGroupMenuTab.CMenuGroupTableAdd(&mGroupAdd);
	mGroupMod.CMenuGroupSet( hmenu , IDM_MODIFY_INFO, IDM_MODIFY_SNAP_X);
	mGroupMenuTab.CMenuGroupTableAdd(&mGroupMod); 
	mGroupSel.CMenuGroupSet( hmenu , IDM_SELECT_OBJECT, IDM_SELECT_NONE);
	mGroupMenuTab.CMenuGroupTableAdd(&mGroupSel); 

// make it global
	mhWnd = hWnd;

	mDebug.CDebugPut("InitInstance %X ",(int)mhWnd );

	GetClientRect( hWnd , &client_rect );

	// WIN32 specific
    CreateButtons();
	CreateRailWindow();

	// open ling to remote debug display if the remote app is started
	mDebug.CDebugOpenRemoteDisplay(hWnd);
	mDebug.CDebugDsp("Connected to MouseTest App");

	mList.SetAppParameters( APP_RADIUS, APP_WIDE );
	
	// support struct to add objects  
	mtmpObjS.Radius = APP_RADIUS;
	mtmpObjS.Width = APP_WIDE;
	mtmpObjS.tType = 0;  // default non loco
	mtmpObjS.iResBitmap =0;
	mtmpObjS.Turn =0;

	hcc = LoadCursor( NULL ,IDC_CROSS);

   return TRUE;
}

//  part of the fundation.h   but stored here in app

static int system_error=0;

void SystemErrorMessage( int i_err )
{
	mSystem.SystemErrorMessage( i_err);
}

int GetSystemError()
{
	return mSystem.GetSystemError();
}

/////////////////////////////////////////////////////////////////////// 
// get the id of the loco to control - Win32 specific
// control the loco 
void  LocoControl( int option)
{
	mVagonList.LocoControl(  option , GetSelectedLocoId(  option ) ); 
}

int AddLocoBM_Id[11]=
{
	IDB_BITMAP_E0,
	IDB_BITMAP_E1,
	IDB_BITMAP_E2,
	IDB_BITMAP_E3,
	IDB_BITMAP_E4,
	IDB_BITMAP_E5,
	IDB_BITMAP_E6,
	IDB_BITMAP_E7,
	IDB_BITMAP_E8,
	IDB_BITMAP_E9,
	IDB_BITMAP_E10
};

int AddVagonBM_Id[11]=
{
	IDB_BITMAP_V0,
	IDB_BITMAP_V1,
	IDB_BITMAP_V2,
	IDB_BITMAP_V3,
	IDB_BITMAP_V4,
	IDB_BITMAP_V5,
	IDB_BITMAP_V6,
	IDB_BITMAP_V7,
	IDB_BITMAP_V8,
	IDB_BITMAP_V9,
	IDB_BITMAP_V10
}; 


typedef struct 
{
	int mFunctionMode;
	int mtype;
	int mTurn;
	float mStartAngle;
	float mSweepAngle;
}   CRailAddObjSetup;


CRailAddObjSetup  sTAddObjTbl[]=
{
	{ FunctionAddStright,	ObjStraightRail	, 0 , 0.0, 0.0		},//0
	{ FunctionAddBendL	,	ObjBendRail		, 0 , 0.0, 45.0		},
	{ FunctionAddBendR	,	ObjBendRail		, 1 , 0.0, -45.0	},
	{ FunctionAddBendL	,	ObjBendRail		, 0 , 0.0, 30.0		},//3
	{ FunctionAddBendR	,	ObjBendRail		, 1 , 0.0, -30.0	},
	{ FunctionAddBendL	,	ObjBendRail		, 0 , 0.0, 22.5		},//5
	{ FunctionAddBendR	,	ObjBendRail		, 1 , 0.0, -22.5	},
	{ FunctionAddSwitchL,	ObjRailSwitch	, 0 , 0.0, 45.0		},//7
	{ FunctionAddSwitchR,	ObjRailSwitch	, 1 , 0.0, -45.0	},
	{ FunctionAddSwitchL,	ObjRailSwitch	, 0 , 0.0, 30.0		},//9
	{ FunctionAddSwitchR,	ObjRailSwitch	, 1 , 0.0, -30.0	},
	{ FunctionAddEnd,		ObjRailEnd		, 0 , 0.0, 0.0		} //11
};

 
int CreateRailObjSructure( int id )
{
	if (id > 11) return 0;
	mtmpObjS.mtype = sTAddObjTbl[id].mtype;
	mtmpObjS.Turn = sTAddObjTbl[id].mTurn;
	mtmpObjS.mStartAngle = sTAddObjTbl[id].mStartAngle;
	mtmpObjS.mSweepAngle = sTAddObjTbl[id].mSweepAngle;
	return sTAddObjTbl[id].mFunctionMode;
}

/*
 HANDLE WINAPI LoadImage(
  _In_opt_  HINSTANCE hinst,
  _In_      LPCTSTR lpszName,
  _In_      UINT uType,
  _In_      int cxDesired,
  _In_      int cyDesired,
  _In_      UINT fuLoad
)

 HANDLE WINAPI LoadImage( 0, (LPCTSTR)"Engine0.bmp" lpszName, IMAGE_BITMAP , 0, 0, LR_LOADFROMFILE);


*/


int CreateAddLocoStructure( int nbr , int iBM_id )
{
	mtmpObjS.mtype = ObjTrain;
	mtmpObjS.Turn = nbr;
	mtmpObjS.tType = 1;
	mtmpObjS.iResBitmap = iBM_id;

	mtmpObjS.bmHandle = mVObjCollections.GetEngineHandle(nbr) ;

	// works well
	/* 
	{

				char path[MAX_PATH];
			GetModuleFileName( NULL ,path ,MAX_PATH);
			StrippedDir( path);
			strcat_s(path, MAX_PATH-20 ,"Enginees\\Engine0.bmp");
			//strcat_s(path, MAX_PATH-20 ,"Vagons\\Engine0.bmp");		 


			HANDLE bmpHandle = LoadImage( 0, (LPCTSTR)path  , IMAGE_BITMAP , 0, 0, LR_LOADFROMFILE);
	
			if ( bmpHandle != NULL )
			{
			BITMAP bm;
			GetObject ( bmpHandle, sizeof(bm), &bm );
			mtmpObjS.Lenght = bm.bmHeight;

mDebug.CDebugPut("Handle : %d  Lenght:%d " ,(int)bmpHandle ,(int) mtmpObjS.Lenght );


			DeleteObject( bmpHandle);
			}
			else
mDebug.CDebugPut("Invalid Handle : %d   " ,(int)bmpHandle   );
	}
 */
	///////

	HBITMAP bmpHandle = LoadBitmap(hInst, MAKEINTRESOURCE(iBM_id));
	BITMAP bm;
	GetObject ( bmpHandle, sizeof(bm), &bm );
	mtmpObjS.Lenght = bm.bmHeight;

	DeleteObject( bmpHandle);


mCTempVector.CTempVectorSetTmpObject( &mtmpObjS );


	return FuntionAddTrain;
}

int CreateAddVagonStructure( int nbr , int iBM_id )
{
	mtmpObjS.mtype = ObjTrain;
	mtmpObjS.Turn = nbr;
	mtmpObjS.tType = 0;
	mtmpObjS.iResBitmap = iBM_id;

	mtmpObjS.bmHandle = mVObjCollections.GetVagonHandle(nbr) ;
	SIZE cs = mVObjCollections.GetVagonSize(nbr) ;

//	BITMAP bm;
//	GetObject ( mtmpObjS.bmHandle, sizeof(bm), &bm );
//	mtmpObjS.Lenght = bm.bmHeight;

	mtmpObjS.Lenght = cs.cy ;
	return FuntionAddTrain;
}


int CreateAddLocoStructure( int nbr  )
{
	return CreateAddLocoStructure( nbr , AddLocoBM_Id[nbr] );
}

int CreateAddVagonStructure( int nbr  )
{
	return CreateAddVagonStructure( nbr , AddVagonBM_Id[nbr] );
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	CPoint mpoint; 

	switch (message)
	{

	case WM_CREATE:
	 	SetTimer( hWnd , 100 , T_MSEC , NULL);
	 			break;
	case WM_ERASEBKGND:
        return true;

	case WM_DRAWITEM:
		DrawButtonCRail(   hInst, (DRAWITEMSTRUCT *)lParam );
		break;

	case WM_USER:
		 HandleLocoHit( wParam );
         return FALSE;
	 
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT: DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);break;
		case IDM_EXIT:DestroyWindow(hWnd);break;
		case IDM_MODE_EDIT:
		case IDM_MODE_ADD:
		case IDM_MODE_CNTR: 
			
			
			
			NewModeSet( wmId )	;mtmpObjS.mtype = 0;

			if ( IDM_MODE_CNTR == wmId ) 
					mList.Index(); 

			if ( IDM_MODE_ADD == wmId   )   ShowObjWindow( 1);
			else ShowObjWindow( 0);

			InvalidateActiveWindow();
			break;

		case IDM_SAVE:
			{
			mDebug.CDebugPut((char *)"Saving to:" );
			char path[MAX_PATH];
			GetModuleFileName( NULL ,path ,MAX_PATH);
			StrippedDir( path);
			strcat_s(path, MAX_PATH-20 ,"MyFile.rls");
			mDebug.CDebugPut((char *)path );
			CArchive *pa = new CArchive( (char *)path, 1);
			CArchive ar = *pa;
			mList.Serialize(  ar);
			pa->CloseArchive();
			delete pa;
			}
        break;
		case IDI_RESTORE:  
			{
			
			char path[MAX_PATH+2];
			GetDataFileNameToRead(  hInst,   hWnd  , path  , MAX_PATH );

			if ( path[0] != 0 )
			{
			mDebug.CDebugPut((char *)"Restoring fm:" );
			//char path[MAX_PATH];
			//GetModuleFileName( NULL ,path ,MAX_PATH);
			//StrippedDir( path);
			//strcat_s(path, MAX_PATH-20 ,"MyFile.rls");
			mDebug.CDebugPut((char *)path );
			CArchive *pa = new CArchive( (char *)path, 0 );	
			CArchive ar = *pa;
			mList.Serialize(  ar);
			pa->CloseArchive();
			delete pa;
			}

			}
			InvalidateActiveWindow();
		break;
		case IDM_TOOLS_UNDO:
			mList.RemoveAtTheEnd();
			InvalidateActiveWindow();
			break;
		case IDM_TOOLS_UNDOVAGON:
			mVagonList.RemoveAtTheEnd();
			InvalidateActiveWindow();
			break;

		case IDM_TOOLS_TGL_LINE:
			RefLineToggle += 1;
			RefLineToggle %= 3;
			InvalidateRect( hWnd , NULL , TRUE );
			break;
		case IDM_TOOLS_STORE_LINE1:
			HandleReferenceLineStore( 0 );
			InvalidateRect( hWnd , NULL , TRUE );
			break;
		case IDM_TOOLS_STORE_LINE2:
			HandleReferenceLineStore( 1 ); 
			InvalidateRect( hWnd , NULL , TRUE );
			break;
		case IDM_TOOLS_STORE_LINE3:
			HandleReferenceLineStore( 2 );
			InvalidateRect( hWnd , NULL , TRUE );
			break;
		case IDM_TOOLS_STORE_LINE4:
			HandleReferenceLineStore( 3 );		 
			InvalidateRect( hWnd , NULL , TRUE );
			break;

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

			CheckAddObject(  wmId );
			mGroupMenuTab.CMenuGroupTableCheck (wmId);
			DeactivateObject();
			mFunctionMode = CreateRailObjSructure( wmId -  IDM_ADD_STRAIGHT  );
			break;

		case IDM_MODIFY_INFO:
			mGroupMenuTab.CMenuGroupTableCheck (IDM_MODIFY_INFO);
			mFunctionMode = FuntionNone;
			{
				CRail *p =  mList.GetActiveObjPtr() ;
				if ( p!= NULL) p->HandleRailInfo();
			}
			break;

		case IDM_MODIFY_VINFO:
			 mVagonList.VagonsInfo();

				//mtmpObjS.mtype =  ObjSemaphore;
				//mFunctionMode = FuntionAddSemaphore;

		break;
		
		case IDM_MODIFY_DELETE:  
			mGroupMenuTab.CMenuGroupTableCheck (IDM_MODIFY_DELETE);
			mFunctionMode = FuntionNone;
			{
			CRail *p =  mList.GetActiveObjPtr() ;
			if ( p!= NULL)
				{
					DeactivateObject();
					CRect rc = p->GetBoundingRect();
					RECT  recctg = rc.GetRect();
					mList.DeleteObject(p);// remove rail
					InvalidateRect( mhWnd , &recctg , FALSE );	 
				}
			}
			break;

		case IDM_MODIFY_SNAP:
			mGroupMenuTab.CMenuGroupTableCheck (IDM_MODIFY_SNAP);

			if (  GetSelectMode() ==  IDM_SELECT_REGION ) 
			{
					CRect rc =  CRect ( mCTempVector.CTempVectorGetPoint(0) , mCTempVector.CTempVectorGetPoint(1));
					rc.NormalizeRect();
					mList.SnapObjects(  rc) ;
			}
			else
				if (  GetSelectMode() ==  IDM_SELECT_2REGIONS ) 
				{
					CRect rc1 =  CRect ( mCTempVector.CTempVectorGetPoint(0) , mCTempVector.CTempVectorGetPoint(1));
					rc1.NormalizeRect();
					CRect rc2 = CRect( GetLastCRect());
					rc2.NormalizeRect();
					mList.SnapObjects(  rc1 , rc2 ) ;
				}
			InvalidateActiveWindow();

			break;

		case IDM_SEM_ADD:
				mtmpObjS.mtype =  ObjSemaphore;
				mFunctionMode = FuntionAddSemaphore;

			break;
		case IDM_MODIFY_SNAP_X:

			 
			mGroupMenuTab.CMenuGroupTableCheck (IDM_MODIFY_SNAP_X);
			//mFunctionMode = FuntionSnap ;
			{// snap , fix , modify , display
				CRect rc =  CRect ( mCTempVector.CTempVectorGetPoint(0) , mCTempVector.CTempVectorGetPoint(1));
				rc.NormalizeRect();
				mList.ProcessObjects( rc , 0)  ;
			}
			 
			InvalidateActiveWindow();

			break;
		case IDM_SELECT_OBJECT:
			mGroupMenuTab.CMenuGroupTableCheck (IDM_SELECT_OBJECT);
			DeactivateObject();
			mFunctionMode = FuntionSelObject;
			NewModeSelectSet(   IDM_SELECT_OBJECT );
			break;
		case IDM_SELECT_REGION:
			mGroupMenuTab.CMenuGroupTableCheck (IDM_SELECT_REGION);
			DeactivateObject();
			mFunctionMode =FunctionTracker;
			NewModeSelectSet(   IDM_SELECT_REGION );
			break;
		case IDM_SELECT_2REGIONS:
			mGroupMenuTab.CMenuGroupTableCheck (IDM_SELECT_2REGIONS);
			DeactivateObject();
			mFunctionMode =FunctionTracker;
			NewModeSelectSet(   IDM_SELECT_2REGIONS );
			break;

		case IDM_SELECT_NONE:
			mGroupMenuTab.CMenuGroupTableCheck (IDM_SELECT_NONE);
			mFunctionMode = FuntionNone;
			break;

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
		mFunctionMode =  CreateAddLocoStructure( wmId -  IDM_TRAIN_ADDL0 );
		
			CheckAddObject(  wmId );
			break;

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
		mFunctionMode =  CreateAddVagonStructure( wmId -  IDM_TRAIN_ADD0 );
		
	//mDebug.CDebugPut("IDM_TRAIN_ADD..:Id (%d) %d  ", mtmpObjS.Lenght , mtmpObjS.tType );

		CheckAddObject(  wmId );
		break;

 		case IDM_TRAIN_DIR:  LocoControl(  OPTION_CHANGE_DIR);break;
		case IDM_TRAIN_STEP: mVagonList.MoveAllTrains(mhWnd,1,ComboBox_GetItemData(hwndCombo,ComboBox_GetCurSel(hwndCombo)));break;
		case IDM_TRAIN_FAST: LocoControl(  OPTION_SPEED_FAST);break;
		case IDM_TRAIN_SLOW: LocoControl(  OPTION_SPEED_SLOW);break;
		case IDM_TRAIN_START:LocoControl(  OPTION_TRAIN_START);break;
		case IDM_TRAIN_STOP: LocoControl(  OPTION_TRAIN_STOP);break;
		default: return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		KillTimer( mhWnd,100); 
		hdc = BeginPaint(hWnd, &ps);
		{
			RECT r = ps.rcPaint;
			 
			HDC hdcMem;
			HBITMAP bitmap;

		 	GetClientRect( hWnd , &client_rect );


			hdcMem = CreateCompatibleDC(hdc); 
			bitmap = CreateCompatibleBitmap(hdc,client_rect.right,client_rect.bottom);
 
			SelectObject(hdcMem, bitmap);

			OnDrawBgn( hWnd, hdcMem );
			OnDraw(  hWnd, hdcMem );
	
			// BitBlt(hdc, 0, 0, client_rect.right,client_rect.bottom , hdcMem, 0, 0, SRCCOPY);
			// -----
		 	BitBlt(hdc, r.left, r.top, r.right, r.bottom , hdcMem, r.left, r.top , SRCCOPY);
			
			DeleteObject(bitmap);
			DeleteDC(hdcMem);
		}
		/*
		{
			HDC hdcMem;
			HBITMAP bitmap;
		
			GetClientRect( hWnd , &client_rect );

			hdcMem = CreateCompatibleDC(hdc); 
			bitmap = CreateCompatibleBitmap(hdc,client_rect.right,client_rect.bottom);
			SelectObject(hdcMem, bitmap);

			OnDrawBgn( hWnd, hdcMem );
			OnDraw(  hWnd, hdcMem );
	
			BitBlt(hdc, 0, 0, client_rect.right,client_rect.bottom , hdcMem, 0, 0, SRCCOPY);
			DeleteObject(bitmap);
			DeleteDC(hdcMem);
		}
		*/
		EndPaint(hWnd, &ps);

		SetTimer( hWnd , 100 , T_MSEC , NULL);
		break;
	case WM_LBUTTONDOWN:	
		MouseState=1; 

        if ( GetCurrentMode() == IDM_MODE_CNTR) SetCursor(hcc );

		mpoint = CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		mCTempVector.CTempVectorUpdateStart( mpoint );
		mCTempVector.CTempVectorUpdateEnd( mpoint );


		//mDebug.CDebugPut("WM_LBUTTONDOWN:Id (%d) %d  ", mtmpObjS.Lenght , mtmpObjS.tType );



		mCTempVector.CTempVectorSetTmpObject( &mtmpObjS );

		if ( GetCurrentMode() == IDM_MODE_EDIT )
		{
			if (  mFunctionMode == FunctionSelHandle ) 
			{
			CRail *p =  mList.GetActiveObjPtr() ;
			if ( p != NULL)
				{
					int isel = p->GetHitHandle(mpoint);
					if ( isel == 1 ) mFunctionMode = FuntionRotObjectStp;  // rotate around Enp
					if ( isel == 3 ) mFunctionMode = FuntionMoveObject;
					if ( isel == 2 ) mFunctionMode = FuntionRotObjectEnp;  // rotate eraund Stp
					if ( isel != 0)  return 0;
				}
			}
		}

		DeactivateObject();

		break;
	case WM_LBUTTONUP:

        MouseState=0; 		
		mpoint = CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		mCTempVector.CTempVectorUpdateEnd( mpoint );	

		if ( GetCurrentMode() == IDM_MODE_CNTR )	 CheckMouseHit( mpoint , IDM_MODE_CNTR  );

		if ( GetCurrentMode() == IDM_MODE_EDIT )
		{
		if ( mFunctionMode == FuntionSelObject )    { WM_LBUTTONUP_Handle_FuntionSelObject( mpoint ) ; RefLineToggle =0 ;return 0; }
		if ( mFunctionMode == FuntionMoveObject )   { WM_LBUTTONUP_Handle_FuntionMoveObject( mpoint ) ; return 0; }
		if ( mFunctionMode == FuntionRotObjectStp ) { WM_LBUTTONUP_Handle_FuntionRotObjectStp( mpoint ) ; return 0; }
 		if ( mFunctionMode == FuntionRotObjectEnp ) { WM_LBUTTONUP_Handle_FuntionRotObjectEnp( mpoint ) ;return 0; }		 
		}

		if ( GetCurrentMode() == IDM_MODE_ADD )
		{
		if ( mFunctionMode == FunctionAddStright )  { WM_LBUTTONUP_Handle_FunctionAddStright( mpoint ); return 0; }
		if (( mFunctionMode == FunctionAddBendR ) || ( mFunctionMode == FunctionAddBendL )) { WM_LBUTTONUP_Handle_FunctionAddBend(  mpoint ) ;return 0; } 
		if (( mFunctionMode == FunctionAddSwitchR ) || ( mFunctionMode == FunctionAddSwitchL )) { WM_LBUTTONUP_Handle_FunctionAddSwitch( mpoint ) ;return 0; }
		if (  mFunctionMode == FunctionAddEnd ) { WM_LBUTTONUP_Handle_FunctionAddEnd( mpoint ) ;return 0; }
		if ( mFunctionMode == FuntionAddTrain ){ WM_LBUTTONUP_Handle_FunctionAddVagon( mpoint ); return 0; }	
		}

		if ( GetCurrentMode() == IDM_MODE_EDIT )
		{
			// here we should have higlighted region or two - this clear tracker

		    if ( mFunctionMode == FunctionTracker) { WM_MOUSEMOVE_Handle_FunctionTracker( mpoint ) ; //mFunctionMode = FuntionNone; 		
			if (  GetSelectMode() ==  IDM_SELECT_2REGIONS )  WM_MOUSEMOVE_Handle_FunctionSaveRect( mpoint ); 
			InvalidateRect( hWnd , NULL , FALSE ); 
			return 0; 
			}
		}

		if ( GetCurrentMode() == IDM_MODE_ADD )
			{
               if ( mFunctionMode == FuntionAddSemaphore )  
				    { WM_LBUTTONUP_Handle_FunctionAddSemaphore( mpoint );   }

			}


		// InvalidateRect( hWnd , NULL , FALSE ); // for all other cases especially end of drawing temp 

		break;
	case WM_MOUSEMOVE:
		if ( MouseState==1 )
		{   
			mpoint = CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));	
			if ( GetCurrentMode() == IDM_MODE_EDIT )
			{
			if ( mFunctionMode == FuntionMoveObject)	{ WM_MOUSEMOVE_Handle_FuntionMoveObject( mpoint )   ;return 0; }
			if ( mFunctionMode == FuntionRotObjectStp)	{ WM_MOUSEMOVE_Handle_FuntionRotObjectStp( mpoint ) ;return 0; }
			if ( mFunctionMode == FuntionRotObjectEnp)	{ WM_MOUSEMOVE_Handle_FuntionRotObjectEnp( mpoint ) ;return 0; }
			}

			if ( GetCurrentMode() == IDM_MODE_EDIT )
				if ( mFunctionMode == FunctionTracker)		{ WM_MOUSEMOVE_Handle_FunctionTracker( mpoint ) ;return 0; }

			if ( GetCurrentMode() == IDM_MODE_ADD )
			{
                // find the corelation with rail
				if ( mFunctionMode == FuntionAddSemaphore )  
						WM_MOUSEMOVE_Handle_FunctionAddSemaphore( mpoint ) ;


			}


			mCTempVector.CTempVectorUpdateEnd( mpoint );// invalidate temporary object	
			InvalidateCurrentObjectState(  NULL  );	
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:

		mVagonList.MoveAllTrains( mhWnd ,0,0);

		mVagonList.CheckSemStopAll ( &mCntList );
 

		if ( GetSystemError() != 0) 
						 {
		                       KillTimer( mhWnd,100);  // must be parametrtize
							   break;
						 } 

		//SetTimer( hWnd , 100 , 40, NULL);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void OnDrawBgn( HWND hWnd , HDC hdc )
{
//  create solid window background - Green
    RECT  clientRect;
	HRGN bgRgn;
    HBRUSH hBrush;

	GetClientRect(hWnd, &clientRect);
    bgRgn = CreateRectRgnIndirect(&clientRect);

		if ( bgRgn == NULL ) 
	{
		SystemErrorMessage( 2 );
		return;
	}

    hBrush = CreateSolidBrush(RGB( 0 , 240, 0));

	if ( hBrush == NULL ) 
	{
		SystemErrorMessage( 2 );
		return;
	}

    FillRgn(hdc, bgRgn, hBrush);

	DeleteObject( bgRgn );
	DeleteObject( hBrush );
	GetStockObject(WHITE_BRUSH);
}

CRect mStaticRect(0,0,0,0);

void	DebugSave(  CRect rcret )
 {
	 mStaticRect = rcret;
	 mStaticRect.NormalizeRect();
 }

void OnDraw( HWND hWnd , HDC hdc )
{
	int  icount =0;
	// draw lines
	if ( ( icount = mList.GetCount()) > 0 )
	{
		int i;
		CPoint cp;

		for ( i =0;i < icount;i++ )
		{
			 CRail *p = mList.GetAt(i);
			 p->DrawObject( hdc );

			 if ( p->mSelected != 0 )
			 {
					 p->DrawTracker(hdc, 1 );
					 
					 if ( RefLineToggle == 1 )
 					{
					CLineObject mLineL2 =  CLineObject ( p->GetPoint( 4  ), p->GetPoint( 5 ));   // blue 
	 				mLineL2.DrawLine( hdc , GetBlueColor() );
					}

					 
					if ( RefLineToggle == 2 )
					{
						if ( p->mtype == ObjBendRail )
						{
						CLineObject mLineL2 =  CLineObject ( p->GetPoint( 1  ), p->GetPoint( 6 ));   // blue 
	 					DrawLine (mLineL2  ,   hdc , GetRedColor() );
						}
					
						if ( p->mtype == ObjRailSwitch )
						{
						CLineObject mLineL2 =  CLineObject ( p->GetPoint( 2  ), p->GetPoint( 6 ));   // blue 
	 					DrawLine (mLineL2  ,   hdc , GetRedColor() );
						}
					}
					 
			 }


			 /*   // for development
			 if ( p->mtype == ObjBendRail )
			 {

			 CFPoint pp = GetPointFmStartBend( p->GetPoint( 0 ),p->GetPoint( 1 ),  p->GetDrawDataStructPtr() , 20 );
				 DrawPointCross( pp ,  hdc , GetRedColor() ,   3);

			   pp = GetPointFmStartBend( p->GetPoint( 1 ),p->GetPoint(0 ),  p->GetDrawDataStructPtr() , 20 );
				  DrawPointCross( pp ,  hdc , GetBlueColor() ,   3);
			 }
			 */

			// cp = CPointF (p->GetPoint( 0 )) ;  DrawPointPlus( cp ,  hdc , GetBlackColor() , 1);
		   //  cp = CPointF (p->GetPoint( 1 ) );  DrawPointPlus( cp ,  hdc , GetBlueColor()  , 1);
		   //  cp = CPointF (p->GetPoint( 2 )) ;  if ( cp.x != 0 ) DrawPointPlus( cp ,  hdc , GetBlueColor() , 1);
		   //  cp = CPointF (p->GetPoint( 5 )) ;  if ( cp.x != 0 ) DrawPointPlus( cp ,  hdc , GetBlueColor() , 1);
			 // debug points if used
			 cp = CPointF (p->GetPoint( 10 ));  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetWhiteColor() ,   1 );
			 cp = CPointF (p->GetPoint( 11 ));  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetRedColor() ,   3);
			 cp = CPointF (p->GetPoint( 12 ));  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetBlueColor()   ,   1 );
			 cp = CPointF (p->GetPoint( 13)) ;  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetBlackColor()   ,   3 );
			 cp = CPointF (p->GetPoint( 14 ));  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetWhiteColor() ,   1 );
			 cp = CPointF (p->GetPoint( 15 ));  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetRedColor() ,   3);

			 if ( p->mtype > 0 )
			 {
				CLineObject mLineL1 =  CLineObject ( p->GetPoint( 10 ),  p->GetPoint( 11 ));   // red
				DrawLine (mLineL1   ,   hdc );

				if ( (p->GetPoint( 12 ).x > 10 ) & (p->GetPoint( 13 ).x > 10 ) )
				{
					CLineObject mLineL2 =  CLineObject ( p->GetPoint( 12  ), p->GetPoint( 13 ));   // blue 
	 				DrawLine (mLineL2  ,   hdc , GetWhiteColor() );
				}
				if ( (p->GetPoint( 14 ).x > 10 ) & (p->GetPoint( 15 ).x > 10 ) )
				{
					CLineObject mLineL3 =  CLineObject ( p->GetPoint( 14  ), p->GetPoint( 15 ));   // blue 
	 				DrawLine (mLineL3  ,   hdc , GetRedColor() );
				}
			 }
		}   
	}

	icount = mCntList.GetCount();
	if ( icount > 0 )
	{
		for ( int i =0;i < icount;i++ )
		{
			 CRail *p = mCntList.GetAt(i);
			 p->DrawObject( hdc );
		}
	}


	if ( mTwoPointActive )
	 { 
			DrawTmpSegment ( mTwoPoint   ,   hdc );
	
	}


	// debug lines
	{
	int i;
	for ( i =0 ;i < 4;i++ )
	     if ( enable_refLine[i] == 1 )
				DrawLine ( mRefLine[i]  ,   hdc , GetRedColor() );
	}

	if ( showRect ==1 )
	 {
	 CRect rc =mCTempVector.CTempVectorGetRect() ;
	 OnDrawRect(  &rc ,   hdc );
	 }

   	if ( ( icount = mVagonList.GetCount()) > 0 )
	{
		int i;
		for ( i =0;i < icount;i++ )
		{
			 CTrainVagon *p = mVagonList.GetAt(i);
			 p->DrawObject( hdc );
		}
	}

	if ( MouseState==1)
	{
		switch (mFunctionMode)
		{
		case FunctionAddBendR:
		case FunctionAddBendL:
		case FunctionAddSwitchR:
		case FunctionAddSwitchL:
		case FunctionAddStright:
		case FunctionAddEnd:
		case FuntionAddTrain:
		case FuntionAddSemaphore:
			 mCTempVector.CTempVectorPaintTmpObject(hdc); break;
		default: break;
		}
	}



	if ( GetCurrentMode() == IDM_MODE_EDIT )
		if (  mFunctionMode == FunctionTracker )
		{
			CRect rc1 =  CRect ( mCTempVector.CTempVectorGetPoint(0) , mCTempVector.CTempVectorGetPoint(1));
			rc1.NormalizeRect(); 
			RECT lprc = rc1.GetRect();
			DrawFocusRect( hdc, &lprc );
		}

// temporary object on the screen ///////////

		mTempDrawObjs.ClrRect1();

		if ( GetSelectMode() ==  IDM_SELECT_2REGIONS ) 
			if ( GetCurrentMode() ==  IDM_MODE_EDIT ) 	
			    mTempDrawObjs.SetRect1( GetLastCRect() );

		mTempDrawObjs.SetPoint1(GetLastPoint() );
		mTempDrawObjs.DrawTempObjects(hdc);

/*
if (  GetSelectMode() ==  IDM_SELECT_2REGIONS ) 
{
	
	
	CRect rcc =CRect(GetLastCRect() );
	OnDrawRect( &rcc  ,   hdc );
}
*/
 
 
////////////////////////////////////////////////////////

	//  any  rect debug
	 OnDrawRect( &mStaticRect  ,   hdc );
 
}
 

/*
#include <mmsystem.h>

BOOL TestAudioCard()
{
 DWORD dwReturn[2];
 MCI_SYSINFO_PARMS mciInfoParms;


 mciInfoParms.wDeviceType = MCI_DEVTYPE_WAVEFORM_AUDIO;
 mciInfoParms.dwRetSize   = 4;
 mciInfoParms.lpstrReturn = (LPSTR)dwReturn;

 DWORD dret= mciSendCommand(NULL,MCI_SYSINFO,MCI_SYSINFO_QUANTITY,
				     (DWORD)(LPVOID)&mciInfoParms);
 if ( dret ) return 0;
 if (  dwReturn[0] ==  0 ) return 0;

 return (  1 );
}
*/


/*
// ************ Globals ************
//
#define MYDISPLAY 1
typedef struct tagMYREC
{
   char  s1[80];
   char  s2[80];
   DWORD n;
} MYREC;
PCOPYDATASTRUCT pMyCDS;
void WINAPI MyDisplay( LPSTR, LPSTR, DWORD );
//
// ************ Code fragment ****************
//
case WM_COPYDATA:
   pMyCDS = (PCOPYDATASTRUCT) lParam;
   switch( pMyCDS->dwData )
   {
      case MYDISPLAY:
         MyDisplay( (LPSTR) ((MYREC *)(pMyCDS->lpData))->s1,
                    (LPSTR) ((MYREC *)(pMyCDS->lpData))->s2,
                    (DWORD) ((MYREC *)(pMyCDS->lpData))->n );
   }
   break;

   bufferDC=CreateCompatibleDC(hdc);
     hammerDC=CreateCompatibleDC(hdc);
     hammer1BMP=(HBITMAP)LoadImage(NULL,"star.bmp",IMAGE_BITMAP,160,160,LR_LOADFROMFILE);
     SelectObject(hammerDC,hammer1BMP);
*/