

#include "stdafx.h"
#include "Windowsx.h"
//#include "Math.h"
//#include <stdio.h>

#include "VObjCollection.h"

#include "Debug.h"
extern CDebug mDebug;


CCollection::CCollection()
{
 int i;	 for (i=0;i<MAX_ENGINEES;i++ )  bmpHandleE[i] = bmpHandleV[i] = NULL; 
}

CCollection::~CCollection()
{
 int i;	 
 for (i=0;i<MAX_ENGINEES;i++ )
	{ 
	if ( bmpHandleE[i] != NULL ) DeleteObject( bmpHandleE[i] );
	if ( bmpHandleV[i] != NULL ) DeleteObject( bmpHandleV[i] );
	}
}


int    CCollection::LoadEnginees() 
{
	int count = 0;
	// create path
	char path[MAX_PATH];

	GetModuleFileName( NULL ,path ,MAX_PATH);
	StrippedDir( path);
	strcat_s(path, MAX_PATH-20 ,"Enginees\\Eng*.bmp");
	// mDebug.CDebugPut( path  );

	WIN32_FIND_DATA  fdat;
	HANDLE h =  FindFirstFile  ( (LPCTSTR)path ,  &fdat);

	if ( h !=  INVALID_HANDLE_VALUE  )
	{
		while ( 1)
		{ 
			StrippedDir( path);
			strcat_s(path, MAX_PATH-20 ,fdat.cFileName);
			//mDebug.CDebugPut( path );
			bmpHandleE[count] = LoadImage( 0, (LPCTSTR)path  , IMAGE_BITMAP , 0, 0, LR_LOADFROMFILE);

			BITMAP bm;
			GetObject ( bmpHandleE[count], sizeof(bm), &bm );
			ibmpESizeX[count] = bm.bmWidth;
			ibmpESizeY[count] = bm.bmHeight;

            count+=1;
			if ( count >= MAX_ENGINEES)  break;
            if (  FindNextFile ( h , &fdat) == FALSE) break;
		}
	}
 
	mEnginees = count;
	return  count;
}

int    CCollection::LoadVagons() 
{
	int count = 0;
	// create path
	char path[MAX_PATH];
	GetModuleFileName( NULL ,path ,MAX_PATH);
	StrippedDir( path);
	strcat_s(path, MAX_PATH-20 ,"Vagons\\Vag*.bmp");
	// mDebug.CDebugPut( path  );

	WIN32_FIND_DATA  fdat;
	HANDLE h =  FindFirstFile  ( (LPCTSTR)path ,  &fdat);

	if ( h !=  INVALID_HANDLE_VALUE  )
	{
		while ( 1)
		{ 
			StrippedDir( path);
			strcat_s(path, MAX_PATH-20 ,fdat.cFileName);
			//mDebug.CDebugPut( path );
			bmpHandleV[count] = LoadImage( 0, (LPCTSTR)path  , IMAGE_BITMAP , 0, 0, LR_LOADFROMFILE);
           
			BITMAP bm;
			GetObject ( bmpHandleV[count], sizeof(bm), &bm );
			ibmpVSizeX[count] = bm.bmWidth;
			ibmpVSizeY[count] = bm.bmHeight;

			count+=1;
			if ( count >= MAX_VAGONS)  break;
            if (  FindNextFile ( h , &fdat) == FALSE) break;
		}
	}
 	mVagons = count;
	return  count;

}


 HANDLE CCollection::GetVagonHandle(int ipos) 
 {
	 return bmpHandleV[ipos];
 }



 HANDLE CCollection::GetEngineHandle(int ipos) 
 {
	 return bmpHandleE[ipos];
 }

 SIZE   CCollection::GetVagonSize( int ipos)
 {
	 SIZE  rs;
	 rs.cx = ibmpVSizeX[ipos] ;
	 rs.cy = ibmpVSizeY[ipos] ;
	return rs;
 }


SIZE   CCollection::GetEngineSize( int ipos) 
{
	 SIZE  rs;
	 rs.cx = ibmpESizeX[ipos] ;
	 rs.cy = ibmpESizeY[ipos] ;
	return rs;
}