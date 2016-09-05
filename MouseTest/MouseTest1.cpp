// MouseTest1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MouseTest.h"


#include "Windowsx.h"
#include "Math.h"
#include <stdio.h>
 
#include "Foundation.h"
#include "ComputeGA.h"
#include "ObjectStore.h"
#include "Graphics.h"

#include "Debug.h"
HWND FindApplicationWinHandle( char *CsTitle );
 

#define MAX_LOADSTRING 100

//  Aplication specific constant

#define APP_RADIUS   100
#define APP_WIDE  10

int    MouseState=0; 
int    RefLineToggle =0 ;   // move later to fundation

int enable_refLine[4]={0,0,0,0};
CLineObject mRefLine[4] ;
 

CDebug mDebug;

CTempVector mCTempVector ;

RECT  up_rect_old;
RECT  up_rect_new;

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
	  FuntionNone } WtoFunMode;

int mFunctionMode = FuntionNone ;


void OnDraw( HWND hWnd,HDC hdc );
void OnDrawTmpLine( HWND hWnd,HDC hdc );
void OnDrawBgn( HWND hWnd , HDC hdc );
void OnDrawTmpOutBox( HWND hWnd , HDC hdc );
void OnDrawTmpTrain( HWND hWnd , HDC hdc );
void OnDrawRect( RECT  *srect , HDC hdc );
void DrawShiftedSection( POINT SPoint ,POINT EPoint , int disp,HDC hdc );
void PaintTrain(HWND hWnd , HDC hdc);
//void CalculateTrainPosition ();
//void CalculateNextTrainPosition ();

int  mTimer =0;
int  mCounter=0;

// train
int TotalLenght=0;
int CurrentDistance =0;

CMenuGroup  mGroupAdd ;
CMenuGroup  mGroupMod ;
CMenuGroup  mGroupTol ;
CMenuGroup  mGroupSel ;
 
CMenuGroupTable  mGroupMenuTab;

CList  mList;

RECT  client_rect;




// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

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

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MOUSETEST1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MOUSETEST1));

up_rect_old.top = 0;
		up_rect_old.left = 0;
		up_rect_new.top = 0;
		up_rect_new.left = 0;


		{
			char path[MAX_PATH];
			GetModuleFileName( NULL ,path ,MAX_PATH);
			StrippedDir( path);
			strcat_s(path, MAX_PATH-20 ,"Debug.txt");
			mDebug.CDebugOpen( path);
			 
		}

		mDebug.CDebugPut("Log is started : ");
	 


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
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MOUSETEST1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MOUSETEST1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

mGroupAdd.CMenuGroupSet( hmenu , IDM_ADD_STRAIGHT, IDM_ADD_CROSS_RIGHT);
mGroupMenuTab.CMenuGroupTableAdd(&mGroupAdd);
mGroupMod.CMenuGroupSet( hmenu , IDM_MODIFY_MOVE, IDM_MODIFY_SNAP_RIGHT);
mGroupMenuTab.CMenuGroupTableAdd(&mGroupMod); 
mGroupSel.CMenuGroupSet( hmenu , IDM_SELECT_OBJECT, IDM_SELECT_NONE);
mGroupMenuTab.CMenuGroupTableAdd(&mGroupSel); 

		mCTempVector.CTempVectorSetRadius(APP_RADIUS);
		mCTempVector.CTempVectorSetWide(APP_WIDE);


	mDebug.CDebugOpenRemoteDisplay(hWnd);

	mDebug.CDebugDsp("Connected to MouseTest App");

	return TRUE;
}

// globally accessed Windows handle
HWND mhWnd = NULL;


int  InvalidateCurrentObjectState(  CRail *p  )
{
	CRect  rc =  mCTempVector.CTempVectorGetRect() ;
	rc.NormalizeRect();
	RECT lprc ;
	lprc.left = rc.left; 
	lprc.top = rc.top;
	lprc.right = rc.right;
	lprc.bottom = rc.bottom;
	InflateRect ( &lprc,2);
	InvalidateRect( mhWnd , &lprc , FALSE ); // to be detailed
	return 0;
}



int  WM_LBUTTONUP_Handle_FuntionSelObject( CPoint mpoint ) 
{

	CPoint	pt = mCTempVector.CTempVectorGetPoint(0 ) ;
	if ( GetDistance( pt, mpoint ) < 3 )
		{
			CRail *p  = mList.GetAtCoordinates( pt );
			if ( p != NULL )
				{
					p->SetSelected(1);
					mList.SetActiveObjPtr(p);

                    mFunctionMode = FunctionSelHandle;
					CRect  rc = p->GetActiveRegion();
					mCTempVector.CTempVectorSetRect( rc);

					InvalidateCurrentObjectState(  p  );
					return 0;
				}

			}
	return 1;
}


int  WM_LBUTTONUP_Handle_FuntionMoveObject( CPoint mpoint ) 
{
	 
	CRail *p =  mList.GetActiveObjPtr() ;

	p->SetSelected(0);
	mFunctionMode =  FuntionSelObject;
	
	mList.Detach( p->GetDrawDataStructPtr()->ThisID ); 
	p->CleanCommIDs();

	CRect  rc = p->GetActiveRegion();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );

	mList.AutoSnap(p);
 
	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );
	return 0;
}

int  WM_LBUTTONUP_Handle_FuntionRotObjectStp( CPoint mpoint ) 
{
	CRail *p =  mList.GetActiveObjPtr() ;
	p->SetSelected(0);
	mFunctionMode =  FuntionSelObject;

	mList.Detach( p->GetDrawDataStructPtr()->ThisID );
	p->CleanCommIDs();

	CRect  rc = p->GetActiveRegion();
	mCTempVector.CTempVectorSetRect( rc);

	InvalidateCurrentObjectState(  p  );
	return 0;
}

int  WM_LBUTTONUP_Handle_FuntionRotObjectEnp( CPoint mpoint ) 
{
	CRail *p =  mList.GetActiveObjPtr() ;
	p->SetSelected(0);
	mFunctionMode =  FuntionSelObject;

	mList.Detach( p->GetDrawDataStructPtr()->ThisID );
	p->CleanCommIDs();

	CRect  rc = p->GetActiveRegion();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );
	return 0;
}

int  WM_LBUTTONUP_Handle_FunctionAddStright( CPoint mpoint ) 
{
	
	if ( GetDistance( mCTempVector.CTempVectorGetPoint(0)  , mpoint) <  4 )  
		{
               InvalidateCurrentObjectState(  NULL );	
			   return 1;
		}
	CStraightRail *p =  new CStraightRail(   mCTempVector.CTempVectorGetPoint(0)  , mpoint );
	mList.AddToTheEnd( p);

	CRect   rc = p->GetBoundingRect();   ///  THIS should be after each setposition called inside the object. - function only needs to retrieve ready data
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	 
	
	mList.AutoSnap(p);
	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );		 

	return 0;
}	 

int  WM_LBUTTONUP_Handle_FunctionAddBend( CPoint mpoint ) 
{
	if ( GetDistance( mCTempVector.CTempVectorGetPoint(0)  , mpoint) < 4 )  
		{
               InvalidateCurrentObjectState(  NULL );	
			   return 1;
		}
	DRAWDATASTRUCT *pd = mCTempVector.CTempVectorGetDrawData(); 

	mCTempVector.CTempVectorUpdateEnd( mpoint );

	CBendRail *p =  new CBendRail(  pd  );
	mList.AddToTheEnd( p);

	CRect   rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	mList.AutoSnap(p);

	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	return 0; 
}

int  WM_LBUTTONUP_Handle_FunctionAddSwitch( CPoint mpoint ) 
{
	if ( GetDistance( mCTempVector.CTempVectorGetPoint(0)  , mpoint) <  4 )  
		{
               InvalidateCurrentObjectState(  NULL );	
			   return 1;
		}
	DRAWDATASTRUCT *pd = mCTempVector.CTempVectorGetDrawData(); 

	mCTempVector.CTempVectorUpdateEnd( mpoint );

	CRailSwitch *p =  new CRailSwitch(  pd  );		 
	mList.AddToTheEnd( p);

	CRect   rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	
	
	mList.AutoSnap(p);

 	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	return 0;
}


int  WM_LBUTTONUP_Handle_FunctionAddEnd( CPoint mpoint ) 
{
	
	if ( GetDistance( mCTempVector.CTempVectorGetPoint(0)  , mpoint) <  4 )  
		{
			InvalidateCurrentObjectState(  NULL );	
			return 1;
		}
	 
	CEndRail *p =  new CEndRail(   mCTempVector.CTempVectorGetPoint(0)  , mpoint );
	mList.AddToTheEnd( p);

	CRect   rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );

	mList.AutoSnap(p);

 	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );

	return 0;
}


int  WM_MOUSEMOVE_Handle_FuntionMoveObject( CPoint mpoint ) 
{
	CRail *p =  mList.GetActiveObjPtr() ;
	CPoint pt = mCTempVector.CTempVectorGetPoint(1);  // but not the first

	
	CRect  rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );
	 						
	mCTempVector.CTempVectorSetRect( rc);
	p->MoveObject( mpoint.x-pt.x,mpoint.y-pt.y );  // only move free  object!!!

	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	mCTempVector.CTempVectorUpdateEnd( mpoint );
 	return 0;
}

int  WM_MOUSEMOVE_Handle_FuntionRotObjectStp( CPoint mpoint ) 
{
	CRail *p =  mList.GetActiveObjPtr() ;
	
	CRect  rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );
	
	p->TurnObject( mpoint  ,1 );  // only move free  object!!!

	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	mCTempVector.CTempVectorUpdateEnd( mpoint );
	return 0;
}


int  WM_MOUSEMOVE_Handle_FuntionRotObjectEnp( CPoint mpoint ) 
{
	CRail *p =  mList.GetActiveObjPtr() ;
	
	CRect  rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );
	
	p->TurnObject( mpoint  ,0 );  // only move free  object!!!

	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	mCTempVector.CTempVectorUpdateEnd( mpoint );
	return 0;
}


int  WM_MOUSEMOVE_Handle_FunctionTracker( CPoint mpoint ) 
{
	RECT lprc ;
 
		CPoint Cp1 = mCTempVector.CTempVectorGetPoint(0 );
		CPoint Cp2 = mCTempVector.CTempVectorGetPoint( 1);	
	    
		lprc.left = Cp1.x;
		lprc.top = Cp1.y;
		lprc.right = Cp2.x;
		lprc.bottom = Cp2.y;

		NormalizeRect(&lprc); 
 
	InflateRect ( &lprc,2);
	InvalidateRect( mhWnd , &lprc , FALSE ); // to be detailed

	mCTempVector.CTempVectorUpdateEnd( mpoint );
	return 0;
}

void HandleReferenceLineStore( int lineno )
{

	if ( enable_refLine[lineno]==0) 
		{
			CRail *p =  mList.GetActiveObjPtr() ;
			if ( p != NULL)
				{
					if ( RefLineToggle == 1 )
						mRefLine[lineno]  =  CLineObject ( p->GetPoint( 4  ), p->GetPoint( 5 )); 

					if ( RefLineToggle == 2 )
					{
						 if ( p->mtype == ObjBendRail )
								mRefLine[lineno]   =   CLineObject ( p->GetPoint( 1  ), p->GetPoint( 6 ));   // blue 
	 					
						 if ( p->mtype == ObjRailSwitch )
					 			mRefLine[lineno]  =  CLineObject ( p->GetPoint( 2  ), p->GetPoint( 6 ));   // blue 
					}
					enable_refLine[lineno]=1;
				}

		}
		else enable_refLine[lineno]=0;
}	  


void DeactivateObject()
{
	  // must invalidate sel handle  if not hit handle or  the same object
		CRail *p =  mList.GetActiveObjPtr() ;
		if ( p != NULL)
				{
			    p->SetSelected(0);
			 	p->mSelected = 0 ;
				mList.SetActiveObjPtr( NULL) ;
				mFunctionMode = FuntionSelObject;

				CRect  rc = p->GetBoundingRect();
				mCTempVector.CTempVectorSetRect( rc);
				InvalidateCurrentObjectState(  p  );	

				 }

}

void  HandleRailInfo( CRail *p )
{
	DRAWDATASTRUCT *pd = p->GetDrawDataStructPtr();

	mDebug.CDebugPut("*Rail_Id:[%d]  Conn_Ids:[%d][%d][%d] ", pd->ThisID,pd->ConnID[0],pd->ConnID[1],pd->ConnID[2]  );
	mDebug.CDebugPut("-StarPoint:(%6.2f,%6.2f) ", pd->StartPoint.x, pd->StartPoint.y );
	mDebug.CDebugPut("-EndPoint :(%6.2f,%6.2f) ", pd->EndPoint.x, pd->EndPoint.y );
	mDebug.CDebugPut("-Radius:%6.2f Angle:%6.2f ", pd->Radius ,  pd->mStartAngle );

	mDebug.CDebugDsp("*Rail_Id:[%d]  Conn_Ids:[%d][%d][%d] ", pd->ThisID,pd->ConnID[0],pd->ConnID[1],pd->ConnID[2]  );
	mDebug.CDebugDsp("-StarPoint:(%6.2f,%6.2f) ", pd->StartPoint.x, pd->StartPoint.y );
	mDebug.CDebugDsp("-EndPoint :(%6.2f,%6.2f) ", pd->EndPoint.x, pd->EndPoint.y );
	mDebug.CDebugDsp("-Radius:%6.2f Angle:%6.2f ", pd->Radius ,  pd->mStartAngle );

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
	mhWnd = hWnd;



	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
				case IDM_DO_DRAW_ARCH:
			mGroupMenuTab.CMenuGroupTableUncheck();
//			mDrawOption  = DrawTmpArch;

			break;
		case IDM_TOOLS_UNDO:
			// should return rect to invalidate 
			mList.RemoveAtTheEnd();
			InvalidateRect( hWnd , NULL , TRUE );
			break;
		case IDM_TOOLS_TGL_LINE:
			// should return rect to invalidate 
			 
			RefLineToggle += 1;
			RefLineToggle %= 3;

			InvalidateRect( hWnd , NULL , TRUE );
			break;

		case IDM_TOOLS_STORE_LINE1:
			// should return rect to invalidate 
			 HandleReferenceLineStore( 0 );
			// get bounding rect
			InvalidateRect( hWnd , NULL , TRUE );
			break;

		case IDM_TOOLS_STORE_LINE2:
			// should return rect to invalidate 
						 HandleReferenceLineStore( 1 ); 
			InvalidateRect( hWnd , NULL , TRUE );
			break;

		case IDM_TOOLS_STORE_LINE3:
			// should return rect to invalidate 
			 			 HandleReferenceLineStore( 2 );
			InvalidateRect( hWnd , NULL , TRUE );
			break;

		case IDM_TOOLS_STORE_LINE4:
			// should return rect to invalidate 
				 HandleReferenceLineStore( 3 );		 
			InvalidateRect( hWnd , NULL , TRUE );
			break;

		case IDM_ADD_ARCH:
 
			 
			break;
		case IDM_DO_DRAWALL:
			 
			break;
		case IDM_BOX:
//			mDrawOption  = DrawOutBox;
		    mGroupMenuTab.CMenuGroupTableUncheck();
			InvalidateRect( hWnd , NULL , TRUE );
			break;
		case IDM_TRAIN:
			break;

		case IDM_ADD_STRAIGHT:	

			mGroupMenuTab.CMenuGroupTableCheck(IDM_ADD_STRAIGHT);
			DeactivateObject();
			mFunctionMode = FunctionAddStright;

		break;

		case IDM_ADD_BEND_LEFT:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_ADD_BEND_LEFT);
			DeactivateObject();
			mFunctionMode = FunctionAddBendL;
			mCTempVector.CTempVectorSetBendAngle( 0.0 , 45.0 ); 
			mCTempVector.CTempVectorSetMode(0) ;

		break;
		case IDM_ADD_BEND_RIGHT:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_ADD_BEND_RIGHT);
			DeactivateObject();
			mFunctionMode = FunctionAddBendR;
			mCTempVector.CTempVectorSetMode(1) ;
			mCTempVector.CTempVectorSetBendAngle( 0.0 , -45.0 ); 

		break;
		case IDM_ADD_CROSS_LEFT:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_ADD_CROSS_LEFT);
			DeactivateObject();
			mFunctionMode = FunctionAddSwitchL;
			mCTempVector.CTempVectorSetBendAngle( 0.0 , 45.0 ); 
			mCTempVector.CTempVectorSetMode(0) ;

		break;
		case IDM_ADD_CROSS_RIGHT:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_ADD_CROSS_RIGHT);
			DeactivateObject();
			mFunctionMode = FunctionAddSwitchR;
			mCTempVector.CTempVectorSetMode(1) ;
			mCTempVector.CTempVectorSetBendAngle( 0.0 , -45.0 ); 
		break;
		
		case IDM_ADD_STOP:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_ADD_STOP);
			DeactivateObject();
			mFunctionMode = FunctionAddEnd;
		break;

		case IDM_MODIFY_MOVE:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_MODIFY_MOVE);
			mFunctionMode = FuntionNone;
		break;
		case IDM_MODIFY_RESIZE:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_MODIFY_RESIZE);
			mFunctionMode = FuntionNone;

						{
			CRail *p =  mList.GetActiveObjPtr() ;
			if ( p!= NULL)
				{
				HandleRailInfo(  p );
				}
			}
	      DeactivateObject();


		break;
		case IDM_MODIFY_DETACH:

		 mGroupMenuTab.CMenuGroupTableCheck (IDM_MODIFY_DETACH);
		mFunctionMode = FuntionNone;
		break;
		case IDM_MODIFY_DELETE:
			  
		mGroupMenuTab.CMenuGroupTableCheck (IDM_MODIFY_DELETE);

								{
			CRail *p =  mList.GetActiveObjPtr() ;
			if ( p!= NULL)
				{
				  
					 DeactivateObject();

					mList.DeleteObject(  p );
				}
			}
	      DeactivateObject();




		//mFunctionMode = FuntionNone;
		break;
		case IDM_MODIFY_SNAP_LEFT:
// snapp glue
			mGroupMenuTab.CMenuGroupTableCheck (IDM_MODIFY_SNAP_LEFT);

						//mFunctionMode = FuntionSnap ;
			{
					CPoint Cp1 = mCTempVector.CTempVectorGetPoint( 0);
					CPoint Cp2 = mCTempVector.CTempVectorGetPoint( 1);
					CRect rc ;

					rc.left = Cp1.x;
					rc.top = Cp1.y;
					rc.right = Cp2.x;
					rc.bottom = Cp2.y;

					rc.NormalizeRect();

					mList.SnapObjects(  rc) ;
			}

			InvalidateRect( hWnd , NULL , FALSE ); // to be detailed
					 
		break;
		case IDM_MODIFY_SNAP_RIGHT:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_MODIFY_SNAP_RIGHT);
			mFunctionMode = FuntionSnap ;
		break;

		case IDM_SELECT_OBJECT:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_SELECT_OBJECT);
			DeactivateObject();
			mFunctionMode = FuntionSelObject;	
 
		break;
		case IDM_SELECT_REGION:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_SELECT_REGION);
			DeactivateObject();
			mFunctionMode =FunctionTracker;	
 
		break;
		case IDM_SELECT_NONE:

			mGroupMenuTab.CMenuGroupTableCheck (IDM_SELECT_NONE);
			mFunctionMode = FuntionNone;
 
		break;


				default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		GetClientRect( hWnd , &client_rect );

		OnDrawBgn( hWnd, hdc );
		OnDraw(  hWnd, hdc );
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		MouseState=1; 
		mpoint = CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		mCTempVector.CTempVectorUpdateStart( mpoint );
		mCTempVector.CTempVectorUpdateEnd( mpoint );
		
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

			 // [1]
		}
	 
		DeactivateObject();
		 
	 
		break;
	case WM_LBUTTONUP:
		MouseState=0; 		
		mpoint = CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		mCTempVector.CTempVectorUpdateEnd( mpoint );	 
		if ( mFunctionMode == FuntionSelObject )    { WM_LBUTTONUP_Handle_FuntionSelObject( mpoint ) ;RefLineToggle =0    ;return 0; }
		if ( mFunctionMode == FuntionMoveObject )   { WM_LBUTTONUP_Handle_FuntionMoveObject( mpoint )  ;return 0; }
		if ( mFunctionMode == FuntionRotObjectStp ) { WM_LBUTTONUP_Handle_FuntionRotObjectStp( mpoint );return 0; }
 		if ( mFunctionMode == FuntionRotObjectEnp ) { WM_LBUTTONUP_Handle_FuntionRotObjectEnp( mpoint );return 0; }		 
		if ( mFunctionMode == FunctionAddStright )  { WM_LBUTTONUP_Handle_FunctionAddStright( mpoint ) ;return 0; }
		if (( mFunctionMode == FunctionAddBendR )||( mFunctionMode == FunctionAddBendL )) { WM_LBUTTONUP_Handle_FunctionAddBend(mpoint);return 0;} 
		if (( mFunctionMode == FunctionAddSwitchR )||( mFunctionMode == FunctionAddSwitchL )){ WM_LBUTTONUP_Handle_FunctionAddSwitch( mpoint);return 0;}
		if ( mFunctionMode == FunctionAddEnd ) { WM_LBUTTONUP_Handle_FunctionAddEnd( mpoint ) ;return 0; }
		if ( mFunctionMode == FunctionTracker) { WM_MOUSEMOVE_Handle_FunctionTracker( mpoint ) ; mFunctionMode = FuntionNone; return 0; }
		//  InvalidateRect( hWnd , NULL , FALSE ); // for all other cases especially end of drawing temp 
		break;
	case WM_MOUSEMOVE:
		if ( MouseState==1 )
			{   
			mpoint = CPoint(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));	 
			if ( mFunctionMode == FuntionMoveObject)   { WM_MOUSEMOVE_Handle_FuntionMoveObject( mpoint )   ;return 0; }
			if ( mFunctionMode == FuntionRotObjectStp) { WM_MOUSEMOVE_Handle_FuntionRotObjectStp( mpoint ) ;return 0; }
			if ( mFunctionMode == FuntionRotObjectEnp) { WM_MOUSEMOVE_Handle_FuntionRotObjectEnp( mpoint ) ;return 0; }
			if ( mFunctionMode == FunctionTracker) { WM_MOUSEMOVE_Handle_FunctionTracker( mpoint ) ;return 0; }
			mCTempVector.CTempVectorUpdateEnd( mpoint );// invalidate temporary object	
			InvalidateCurrentObjectState(  NULL  );	
			//InvalidateRect( hWnd , NULL , FALSE );
			}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		mCounter+=1;
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
    hBrush = CreateSolidBrush(RGB( 0 , 240, 0));
    FillRgn(hdc, bgRgn, hBrush);

	DeleteObject( hBrush );
	GetStockObject(WHITE_BRUSH);

}


void OnDraw( HWND hWnd , HDC hdc )
{


	//UINT mItem = mGroupMenuTab.CMenuGroupTableGetCurrent();

	int  icount =0;

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
	 				//DrawLine (mLineL2  ,   hdc , GetBlueColor() );
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
		  
 			
			  cp = CPointF (p->GetPoint( 0 )) ;  DrawPointPlus( cp ,  hdc , GetBlackColor() , 1);
		     cp = CPointF (p->GetPoint( 1 ) );  DrawPointPlus( cp ,  hdc , GetBlueColor()  , 1);
		     cp = CPointF (p->GetPoint( 2 )) ;  if ( cp.x != 0 ) DrawPointPlus( cp ,  hdc , GetBlackColor() , 1);

		 
		   cp = CPointF (p->GetPoint( 10 ));  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetWhiteColor() ,   1 );
			 cp = CPointF (p->GetPoint( 11 ));  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetRedColor() ,   3);
			 cp = CPointF (p->GetPoint( 12 ));  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetBlueColor()   ,   1 );
			 cp = CPointF (p->GetPoint( 13)) ;  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetBlackColor()   ,   3 );

			 cp = CPointF (p->GetPoint( 14 ));  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetWhiteColor() ,   1 );
			 cp = CPointF (p->GetPoint( 15 ));  if ( cp.x != 0 )DrawPointCross( cp ,  hdc , GetRedColor() ,   3);

			 if ( p->mtype > 0 )
			 {
 
				CLineObject mLineL1 =  CLineObject ( p->GetPoint( 10 ),  p->GetPoint( 11 ));   // red
			
				//CLineObject mLineL2 =  CLineObject ( p->GetPoint( 10  ), p->GetPoint( 12 ));   // blue 
				//CLineObject mLineL3 =  CLineObject ( p->GetPoint( 10  ), p->GetPoint( 13 ));   // blue 

				if ( (p->GetPoint( 12 ).x > 10 ) & (p->GetPoint( 13 ).x > 10 ) )
				{
				
					CLineObject mLineL4 =  CLineObject ( p->GetPoint( 12  ), p->GetPoint( 13 ));   // blue 
	 				DrawLine (mLineL4  ,   hdc , GetWhiteColor() );

				}



				if ( (p->GetPoint( 14 ).x > 10 ) & (p->GetPoint( 15 ).x > 10 ) )
				{
				
					CLineObject mLineL4 =  CLineObject ( p->GetPoint( 14  ), p->GetPoint( 15 ));   // blue 
	 				DrawLine (mLineL4  ,   hdc , GetRedColor() );

				}
							//	CLineObject mLineL2 =  CLineObject ( p->GetPoint( 10 ),  p->GetPoint( 11 ));   // red

				//CLineObject mLineP1 =  CLineObject( mLineL2.GetA(),mLineL2.GetB(),p->GetPoint( 12 ));

		 DrawLine (mLineL1   ,   hdc );
	 //	 DrawLine (mLineL2   ,   hdc );
	//	 DrawLine (mLineL3   ,   hdc );
 

			 	//CFPoint pf =  CrossOfTwoLines ( mLineP1 , mLineL1  ) ;
			 	//cp.x = pf.x;cp.y =pf.y; 
				 // DrawPointExOrange (   cp ,   hdc );
 
			 }
			  
	 


		}   
	}


		{
	int i;
	for ( i =0 ;i < 4;i++ )
	     if ( enable_refLine[i] == 1 )
				DrawLine ( mRefLine[i]  ,   hdc , GetRedColor() );
	}



	if ( MouseState==1)
	{
	if (( mFunctionMode == FunctionAddBendR   )||(mFunctionMode == FunctionAddBendL   ))	mCTempVector.CTempVectorPaintBend( hdc );

	if (( mFunctionMode == FunctionAddSwitchR )||( mFunctionMode == FunctionAddSwitchL))	mCTempVector.CTempVectorPaintSwitch( hdc );
 
	if (  mFunctionMode == FunctionAddStright )												mCTempVector.CTempVectorPaintStright( hdc );
	
	if ( mFunctionMode == FunctionAddEnd )													mCTempVector.CTempVectorPaintEnd(hdc);
	
	}


	if (  mFunctionMode == FunctionTracker )
	{
		RECT lprc ;
 
		CPoint Cp1 = mCTempVector.CTempVectorGetPoint(0 );
		CPoint Cp2 = mCTempVector.CTempVectorGetPoint( 1);


		lprc.left = Cp1.x;
		lprc.top = Cp1.y;
		lprc.right = Cp2.x;
		lprc.bottom = Cp2.y;

		NormalizeRect(&lprc);

		DrawFocusRect( hdc, &lprc );


	}
 

 
}
 

/*
void CalculateTrainPosition ()
{

	CPoint spoint = mCTempVector.CTempVectorExtendedPoint( CurrentDistance+4  );
	CPoint epoint = mCTempVector.CTempVectorExtendedPoint( CurrentDistance-4  );

	up_rect_old.right = spoint.x;
	up_rect_old.top = spoint.y;
	up_rect_old.left = epoint.x;
	up_rect_old.bottom = epoint.y;

	NormalizeRect( &up_rect_old );
	InflateRect( &up_rect_old , 5 );

}

void CalculateNextTrainPosition ()
{

//	TotalLenght  = mCTempVector.CTempVectorDistance();
	CurrentDistance+=2;
    if (  CurrentDistance > TotalLenght )  
	        {
		     CurrentDistance = TotalLenght; 
			 mTimer =0;
//			 mDrawOption = DrawNothing;
			}//

	CPoint spoint = mCTempVector.CTempVectorExtendedPoint( CurrentDistance+4  );
	CPoint epoint = mCTempVector.CTempVectorExtendedPoint( CurrentDistance-4  );

	up_rect_new.right = spoint.x;
	up_rect_new.top = spoint.y;
	up_rect_new.left = epoint.x;
	up_rect_new.bottom = epoint.y;

	NormalizeRect( &up_rect_new );
	InflateRect( &up_rect_new , 5 );

}




void PaintTrain(HWND hWnd , HDC hdc)
{

	//	TotalLenght  = mCTempVector.CTempVectorDistance();
	 
        if (  CurrentDistance > TotalLenght )  
	        {
		     CurrentDistance = TotalLenght; 
			 KillTimer( hWnd,100); 
			 mTimer =0;
//			 mDrawOption = DrawNothing;
			}

		     // center point
		     // POINT tpoint = GetPointFmStart(  HitDown_Point_Client  , Last_Point_Client ,  CurrentDistance );

		CPoint spoint = mCTempVector.CTempVectorExtendedPoint( CurrentDistance+4  );
		CPoint epoint = mCTempVector.CTempVectorExtendedPoint( CurrentDistance-4  );
	  
		MoveToEx(hdc ,  spoint.x     ,spoint.y ,    (LPPOINT) NULL); 
		LineTo(  hdc ,  epoint.x  ,  epoint.y );
	  
		up_rect_new.right = spoint.x;
		up_rect_new.top = spoint.y;
		up_rect_new.left = epoint.x;
		up_rect_new.bottom = epoint.y;

		NormalizeRect( &up_rect_new );
		InflateRect( &up_rect_new , 5 );  
			
}

*/



/*
typedef struct tagCOPYDATASTRUCT {
  ULONG_PTR dwData;
  DWORD     cbData;
  PVOID     lpData;
} COPYDATASTRUCT, *PCOPYDATASTRUCT;

wParam  A handle to the window passing the data. 
lParam  A pointer to a COPYDATASTRUCT structure that contains the data to be passed. 

// ************ Globals ************
//
#define MYDISPLAY 1
typedef struct tagMYREC
{
   char  s1[80];
   char  s2[80];
   DWORD n;
} MYREC;
COPYDATASTRUCT MyCDS;
MYREC MyRec;
HRESULT hResult;
BOOL CALLBACK InfoDlgProc( HWND, UINT, WPARAM, LPARAM );
// ************ Code fragment ****************
// Get data from user. InfoDlgProc stores the information in MyRec.
//
   DialogBox( ghInstance, "InfoDlg", hWnd, (DLGPROC) InfoDlgProc );
//
// Copy data into structure to be passed via WM_COPYDATA.
// Also, we assume that truncation of the data is acceptable.
//
   hResult = StringCbCopy( MyRec.s1, sizeof(MyRec.s1), szFirstName );
   if (hResult != S_OK)
   		return False;
   hResult = StringCbCopy( MyRec.s2, sizeof(MyRec.s2), szLastName );
   if (hResult != S_OK)
   		return False;
   MyRec.n = nAge;
//
// Fill the COPYDATA structure
// 
   MyCDS.dwData = MYPRINT;          // function identifier
   MyCDS.cbData = sizeof( MyRec );  // size of data
   MyCDS.lpData = &MyRec;           // data structure
//
// Call function, passing data in &MyCDS
//
   hwDispatch = FindWindow( "Disp32Class", "Hidden Window" );
   if( hwDispatch != NULL )
      SendMessage( hwDispatch,
                   WM_COPYDATA,
                   (WPARAM)(HWND) hWnd,
                   (LPARAM) (LPVOID) &MyCDS );
   else
      MessageBox( hWnd, "Can't send WM_COPYDATA", "MyApp", MB_OK );

The receiving application has a hidden window which receives the information from WM_COPYDATA and displays it to the user.

Copy
 
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



*/