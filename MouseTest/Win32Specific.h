
#include "stdafx.h"
#include "MouseTest.h"

#include "Windowsx.h"



extern HWND  hwndButtonStatic;
extern HWND  hwndCombo;   

void OnDraw( HWND hWnd,HDC hdc );
void OnDrawBgn( HWND hWnd , HDC hdc );
void CreateButtons();
void CreateButtonsObj();
void CreateRailWindow();

ATOM MyRegisterRClass(HINSTANCE hInstance);

void  ShowObjWindow( int show);

void NewModeSet( int newmode );
int GetCurrentMode();
void NewModeSelectSet( int newmode );
int GetSelectMode();

void HandleLocoHit( int id );
int GetSelectedLocoId( int option );
void  DrawButtonCRail( HINSTANCE  hInst,  DRAWITEMSTRUCT *d );
void	CheckAddObject( int wmId );

void InvalidateActiveWindow();

void GetDataFileNameToRead(  HINSTANCE  hInst,  HWND hWnd  ,char  *data_out , int  lengthOfBuffer );