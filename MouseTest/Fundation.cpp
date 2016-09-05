
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

extern CDebug mDebug;


// afx foundation
// afx foundation


CFPoint::CFPoint() 
{
	x =0;y=0;
}

CFPoint	CFPoint::operator+(const CFPoint &second) 
{
	x += second.x;
	y += second.y;
	return CFPoint(x,y);
}

CFPoint	CFPoint::operator-(const CFPoint &second) 
{
	x -= second.x;
	y -= second.y;
	return CFPoint(x,y);
}

CFPoint::CFPoint(float ix,float iy )
{
	x = ix ;y=iy;
}
CFPoint::CFPoint(int ix,int iy )
{
	x = (float)ix ;y=(float)iy;
}

CFPoint::CFPoint( CPoint cp )
{
  x = (float)cp.x; y = (float)cp.y;

}

 
 
CPointF::CPointF( float fx , float fy) 
{
	CPoint::x = (int )roundToNearest(fx);
	CPoint::y = (int )roundToNearest(fy);
 
}

CPointF::CPointF( CFPoint fpt )
{
	CPoint::x = (int )roundToNearest(fpt.x);
	CPoint::y = (int )roundToNearest(fpt.y);
}

 
CVector::CVector() 
{
}

CVector::CVector(CPoint start,CPoint end ) 
{
	mStart = start; mEnd = end;
}

CVector::CVector(CFPoint start,CFPoint end )
{
mStart.x =(int)start.x;
mStart.y =(int)start.y;
mEnd.x = (int)end.x;
mEnd.y = (int)end.y;
}



CFVector::CFVector() 
{
}

CFVector::CFVector(CFPoint start,CFPoint end ) 
{
	mStart = start; mEnd = end;
}

CFVector::CFVector(CPoint start,CPoint end )
{
mStart.x = (float)start.x;
mStart.y = (float)start.y;
mEnd.x =  (float)end.x;
mEnd.y =  (float)end.y;
}




CSegment::CSegment() 
{
	 mStart = CFPoint(0,0);
	 mEnd = CFPoint(0,0);
}

CSegment::CSegment(CPoint start,CPoint end ) 
{
	 mStart = CFPoint(start.x,start.y) ;  mEnd = CFPoint(mEnd.x,mEnd.y);  
}

CSegment::CSegment(CFPoint start,CFPoint end )
{
mStart.x = start.x;
mStart.y = start.y;
mEnd.x =  end.x;
mEnd.y =  end.y;
}

void CSegment::DrawSegment( HDC hdc, COLORREF rf ) 
{
	HPEN pen;
	pen = CreatePen( PS_SOLID , 1 , rf );
	HPEN oldpen = (HPEN)SelectObject( hdc , pen );
	
	MoveToEx(hdc ,   (int)roundToNearest(mStart.x )     , (int)roundToNearest(mStart.y )  ,    (LPPOINT) NULL); 
	LineTo(  hdc ,  (int)roundToNearest(mEnd.x )    ,(int)roundToNearest( mEnd.y) );
 
	SelectObject( hdc , oldpen );
	DeleteObject( pen );
}


CTwoPoints::CTwoPoints() 
{
	 mPoint1 =  CFPoint(0,0); mPoint2 = mPoint1;
}

CTwoPoints::CTwoPoints( CFPoint point1, CFPoint point2   ) 
{
	mPoint1 = point1; mPoint2 = point2;
}


CCircle::CCircle() 
{

}

CCircle::CCircle( CFPoint center , float fradius ) 
{
	mCenter = center; mRadius = fradius;
}

 


CLineObject::CLineObject()
{
	ACoeff=0;
	BCoeff=0;
	CCoeff=0;
}

CLineObject::CLineObject (CPoint FirstPoint,CPoint SecondPoint)
{
	LineFromPoints( FirstPoint, SecondPoint);
}

CLineObject::CLineObject( CFPoint FirstPoint,CFPoint SecondPoint )
{
    LineFromPoints( FirstPoint, SecondPoint);
}

CLineObject::CLineObject( double dACoeff,double dBCoeff ,double dCCoeff) 
{
	ACoeff=dACoeff;
	BCoeff=dBCoeff;
	CCoeff=dCCoeff;
}

CLineObject::CLineObject( double MCoeff ,double DCoeff  )
{
	ACoeff=-MCoeff;
	BCoeff=1;
	CCoeff=-DCoeff;
}

CLineObject::CLineObject( double ACoeff,double BCoeff ,CFPoint ThroughPoint)
{
	LinePenpendicularFromLineAndPoint( ACoeff,  BCoeff ,  ThroughPoint );
}

double CLineObject::GetA()
{
	return ACoeff;
}
double CLineObject::GetB()
{
	return BCoeff;
}
double CLineObject::GetC()
{
	return CCoeff;
}

void CLineObject::LineFromPoints(CPoint FirstPoint,CPoint SecondPoint)
{	
    ACoeff = (double)(FirstPoint.y-SecondPoint.y);
    BCoeff = (double)(SecondPoint.x-FirstPoint.x);
	CCoeff = (double)(FirstPoint.y*( FirstPoint.x-SecondPoint.x)  + FirstPoint.x*( SecondPoint.y - FirstPoint.y)) ;
}

void CLineObject::LineFromPoints(CFPoint FirstPoint,CFPoint SecondPoint)
{	
    ACoeff = (double)(FirstPoint.y-SecondPoint.y);
    BCoeff = (double)(SecondPoint.x-FirstPoint.x);
	CCoeff = (double)(FirstPoint.y*( FirstPoint.x-SecondPoint.x)  + FirstPoint.x*( SecondPoint.y - FirstPoint.y)) ;
}


void CLineObject::LinePenpendicularFromLineAndPoint( double iACoeff,double iBCoeff ,CFPoint ThroughPoint)
{	
	BCoeff =   iACoeff;
	ACoeff = - iBCoeff;
	CCoeff =   -ACoeff*ThroughPoint.x - BCoeff*ThroughPoint.y;
}

void CLineObject::DrawLine( HDC hdc ,COLORREF rf  )
{	
  CFPoint fp1 = CFPoint(10,10) ;
	CFPoint fp2 = CFPoint( 2000 , 1200 ) ;
 
	if ( (int)this->GetB() == 0 )
	{
		if ( (int)this->GetA() != 0 )  
		{
			fp1.x = fp2.x = - (float)this->GetC()  /(float)this->GetA();
			fp1.y = 0;
			fp2.y = (float)1200 ;
		}
		else return ;
	}
	else
	{
		fp1.y = (float)((-this->GetA() * fp1.x - this->GetC()  ) /this->GetB());
		fp2.y = (float)((-this->GetA() * fp2.x - this->GetC()  ) /this->GetB());
	}

	CPointF pt1 = CPointF( fp1 ) ;
	CPointF pt2 = CPointF( fp2 ) ;

	HPEN pen;
	pen = CreatePen( PS_SOLID , 1 , rf);
	HPEN oldpen = (HPEN)SelectObject( hdc , pen );
	
	MoveToEx(hdc ,  pt1.x     , pt1.y,    (LPPOINT) NULL); 
	LineTo(  hdc ,  pt2.x     , pt2.y );
 
	SelectObject( hdc , oldpen );
	DeleteObject( pen );
}



CMenuGroup::CMenuGroup(){midCheck = 0;}
CMenuGroup::CMenuGroup( HMENU hmenu , UINT idFirst, UINT idLast)
{
	mhmenu = hmenu;
    midFirst=idFirst;
	midLast=idLast;
	midCheck = 0;
}

void CMenuGroup::CMenuGroupSet( HMENU hmenu , UINT idFirst, UINT idLast)
{
	mhmenu = hmenu;
    midFirst=idFirst;
	midLast=idLast;
	midCheck = 0;

}

 

int CMenuGroup::CMenuGroupCheck (UINT idCheck)
{ 
	if ( (idCheck < midFirst) || (idCheck > midLast)) return 0;
	midCheck = idCheck;
	return CheckMenuRadioItem(  mhmenu  ,midFirst, midLast , midCheck  ,MF_BYCOMMAND   );
}

int CMenuGroup::CMenuGroupUncheck() 
{
	if ( midCheck == 0 ) return 0;
	CheckMenuItem( mhmenu  , midCheck   ,MF_BYCOMMAND  | MF_UNCHECKED );
	midCheck = 0;
	return 1;
}

UINT CMenuGroup::CMenuGroupGetCurrent() 
{
	return midCheck;
}

CMenuGroupTable::CMenuGroupTable(){mCount = 0;} 
void CMenuGroupTable::CMenuGroupTableAdd( CMenuGroup *ptr )
{
   if ( mCount < MAXMENUGRP )
         List[mCount++]=ptr;
}



int CMenuGroupTable::CMenuGroupTableCheck (UINT idCheck) 
{
	int i;
	for ( i = 0 ;i < mCount; i++ )
	{
		List[i]->CMenuGroupUncheck();
		List[i]->CMenuGroupCheck(idCheck);
	}
	return 0;
}

int CMenuGroupTable::CMenuGroupTableUncheck() 
{
	int i;
	for ( i = 0 ;i < mCount; i++ )
		List[i]->CMenuGroupUncheck();
	return 0;
}

UINT CMenuGroupTable::CMenuGroupTableGetCurrent() 
{
	int i;UINT cucheck;
	for ( i = 0 ;i < mCount; i++ )
		 if ( ( cucheck = List[i]->CMenuGroupGetCurrent()) > 0 )
			 return cucheck;

	return 0;	
}


// -------------------------------------------------------------------------------
//   Temporary object class definition
// -------------------------------------------------------------------------------
 
CTempVector::CTempVector()
{
	mObjectType = ObjNothing;

	mData.StartPoint = CPoint(0,0);
	mData.EndPoint = CPoint(0,0); 
	mData.Radius = 100;
	mData.Width = 10;
	mData.mStartAngle = 0.0;
	mData.mSweepAngle = 0.0;
	mData.Turn =0;
}

void CTempVector::CTempVectorSetTmpObject(TMPOBJECT_DRAWDATASTRUCT *tmpObjS )
{
	mData.Radius = tmpObjS->Radius ;
	mData.Width =  tmpObjS->Width;
	mObjectType =  tmpObjS->mtype;
	mData.mStartAngle = tmpObjS->mStartAngle;
	mData.mSweepAngle = tmpObjS->mSweepAngle;
	mData.Turn = tmpObjS->Turn;

	mDrawVagonData.mInitialLenght = (float)tmpObjS->Lenght;
}

void CTempVector::CTempVectorUpdateStart( CPoint start )
{
	mData.StartPoint.x =(float)start.x; 
	mData.StartPoint.y =(float)start.y; 
}

void CTempVector::CTempVectorUpdateEnd( CPoint end )
{
	mData.EndPoint.x = (float)end.x; 
	mData.EndPoint.y = (float)end.y; 
}
 
void    CTempVector::CTempVectorPaintTmpObject(   HDC hdc )
{
	if ( mObjectType == ObjStraightRail )
		{
			CTempVectorPaintStright(  hdc );
		}
	if ( mObjectType == ObjBendRail )
		{
			CTempVectorPaintBend(  hdc );
		}
	if ( mObjectType == ObjRailSwitch )
		{
			CTempVectorPaintSwitch(  hdc );
		}
	if ( mObjectType == ObjRailEnd )
		{
			CTempVectorPaintEnd(  hdc );
		}

	if ( mObjectType == ObjTrain )
		{
			CTempVectorPaintTrain(  hdc );
		}
	
	if ( mObjectType ==	ObjSemaphore )
			CTempVectorPaintSemaphore(  hdc );
 
}


void CTempVector::CTempVectorPaintBend(   HDC hdc )
{
	mData.CenterPoint =	ComputeTwoParLines( mData.StartPoint ,mData.EndPoint , mData.Radius ,  mData.Turn  ) ;
	mData.mStartAngle =	ComputeVectorAngle( mData.CenterPoint , mData.StartPoint ) ;

	ComputeBandingRectBend( &mData  );
    mUnionRect = mData.mUpdRect;

	PaintBendRail( &mData, SINGLELINE| IS_BEND | SWITCHOFF  , hdc );
	OnDrawRect( &mData.mUpdRect ,   hdc ); // draw framing rect
}

void CTempVector::CTempVectorPaintStright(   HDC hdc )
{

	mData.mUpdRect = CRect ( (int)mData.StartPoint.x , (int)mData.StartPoint.y, (int)mData.EndPoint.x ,(int)mData.EndPoint.y );
	mData.mUpdRect.NormalizeRect();
	mData.mUpdRect.InflateRect( 4 + (int)mData.Width);
	mUnionRect = mData.mUpdRect;
	
	PaintStraightRail( &mData, SINGLELINE , hdc );
	OnDrawRect( &mData.mUpdRect ,   hdc );// draw framing rect
 
}

void CTempVector::CTempVectorPaintEnd(   HDC hdc )
{
	mData.EndPoint =  GetPointFmStart(  mData.StartPoint,mData.EndPoint, STANDARD_STOP_MAIN_AXIS_LENGHT );

 	mData.mUpdRect = CRect ( (int)mData.StartPoint.x , (int)mData.StartPoint.y, (int)mData.EndPoint.x ,(int)mData.EndPoint.y );
 	mData.mUpdRect.NormalizeRect();
 	mData.mUpdRect.InflateRect( 4 + (int)mData.Width);
	mUnionRect = mData.mUpdRect;
	PaintEndRail( &mData, SINGLELINE , hdc );
	OnDrawRect( &mData.mUpdRect ,   hdc );  // draw framing rect
}

//  not used !
/*
void    CTempVector::CTempVectorSetTrain( int type, int bmpId )
{

	mDrawVagonData.mResourceId =  bmpId;
	mDrawVagonData.mType = type;
	
	if ( bmpId > 0 )
	{
          mDrawVagonData.bmpHandle = LoadBitmap(hInst, MAKEINTRESOURCE(bmpId));
		
		  DeleteObject(mDrawVagonData.bmpHandle);

			//HDC MemDCExercising = CreateCompatibleDC(hdc);
			//SelectObject(MemDCExercising, bmpExercising);
			//BITMAP bm;
			//GetObject ( bmpExercising, sizeof(bm), &bm );
	}
	
	// hInst = global value
	//mDrawVagonData.bmpHandle = LoadBitmap(hInst, MAKEINTRESOURCE(bmpId));
	//mDrawData.pVagonData = &mDrawVagonData;
}
*/


void	CTempVector::CTempVectorPaintTrain( HDC hdc ) 
{
	//mData.StartPoint = GetPointFmStart(  mData.EndPoint , mData.StartPoint , STANDARD_TRAIN_MAIN_AXIS_LENGHT );

	mData.StartPoint = GetPointFmStart(  mData.EndPoint , mData.StartPoint , mDrawVagonData.mInitialLenght );
 


	 mData.mUpdRect = CRect ( (int)mData.StartPoint.x , (int)mData.StartPoint.y, (int)mData.EndPoint.x ,(int)mData.EndPoint.y );
	 mData.mUpdRect.NormalizeRect();
	 mData.mUpdRect.InflateRect( 4 + (int)mData.Width);

	 	mUnionRect = mData.mUpdRect;

	int  tmp = mData.Width ;
	mData.Width = 	mData.Width +4 ;
 
	PaintTrain( &mData, SINGLELINE , hdc );
	OnDrawRect( &mData.mUpdRect ,   hdc );  // draw framing rect

	mData.Width = tmp;
}


void CTempVector::CTempVectorPaintSwitch (   HDC hdc )
{
	// limit stright lenght at switch
	mData.EndPoint =  GetPointFmStart(  mData.StartPoint,mData.EndPoint, STANDARD_SWITCH_MAIN_AXIS_LENGHT );

	mData.CenterPoint =	ComputeTwoParLines( mData.StartPoint ,mData.EndPoint , mData.Radius ,  mData.Turn  ) ;
	mData.mStartAngle =	ComputeVectorAngle( mData.CenterPoint , mData.StartPoint ) ;
 
	ComputeBandingRectSwitch( &mData  );
 	mUnionRect = mData.mUpdRect;

	PaintRailSwitch( &mData, SINGLELINE , hdc );
	OnDrawRect( &mData.mUpdRect ,   hdc );  // draw framing rect

 
}

void	CTempVector::CTempVectorPaintSemaphore( HDC hdc )
{
	
	mData.StartPoint = GetPointFmStart(  mData.EndPoint , mData.StartPoint , (float)(mData.Width+mData.Width) );

	
	mData.mUpdRect = CRect ( (int)mData.StartPoint.x , (int)mData.StartPoint.y, (int)mData.EndPoint.x ,(int)mData.EndPoint.y );
	mData.mUpdRect.NormalizeRect();
 	mData.mUpdRect.InflateRect(  4 + (int)mData.Width    );
	mUnionRect = mData.mUpdRect;
	
	PaintSemaphore( &mData, 0 , hdc );
	OnDrawRect( &mData.mUpdRect ,   hdc );// draw framing rect

}
 
CPoint	CTempVector::CTempVectorGetPoint( int ipoint ) 
{
	if ( ipoint == 0 ) return CPointF( mData.StartPoint ); 
 	else return CPointF(mData.EndPoint);
}

CRect   CTempVector::CTempVectorGetRect() 
{

	CRect rc = mUnionRect;
	rc.InflateRect(4);
  //mDebug.CDebugPut("Inflate :%d %d %d %d  ", rc.left ,rc.top , rc.right, rc.bottom );
	return rc;
}

 
void    CTempVector::CTempVectorSetRect(CRect rc)
{

 mUnionRect = rc ;


}
 

DRAWDATASTRUCT *CTempVector::CTempVectorGetDrawData() 
{
	return &mData;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CPenStorage::CPenStorage()
{
	m_pen=NULL;
	m_oldpen=NULL;
	m_hdc = NULL;
	m_Pen_Error =0;
 

	//mDebug.CDebugPut("Create PenS");
 

    // Retrieve the bitmap color format, width, and height.  
  //  if (!GetObject(hBmp, sizeof(LOGPEN), (LPSTR)&bmp)) 
   //     errhandler("GetObject", hwnd);
 

}

CPenStorage::~CPenStorage()
{
	
	//	mDebug.CDebugPut("Destroy PenS");

	if ( m_hdc != NULL )
		if ( m_oldpen != NULL)
			SelectObject( m_hdc , m_oldpen );

	if (m_pen !=NULL )
		DeleteObject( m_pen );

}

HPEN CPenStorage::GetNewPen( int fnPenStyle, int nWidth, COLORREF crColor ) 
{
	
	if (m_pen != NULL )
	{
		if ( m_oldpen != 0 )
			 SelectObject( m_hdc , m_oldpen );
		DeleteObject( m_pen );
	}

	m_pen = CreatePen( fnPenStyle , nWidth, crColor );
	if ( m_pen == NULL ) 
	{
		m_Pen_Error = 1;
		SystemErrorMessage( 1 );
	}
	else
	{
		HPEN tmppen = (HPEN)SelectObject( m_hdc , m_pen );
		
		if ( m_oldpen == 0 )  m_oldpen = tmppen ;
	}

	return m_pen;
}


HPEN  CPenStorage::GetPen(int fnPenStyle, int nWidth, COLORREF crColor)
{
	return GetNewPen(  fnPenStyle,   nWidth,   crColor ) ;
}

HPEN CPenStorage::GetDottedPen(COLORREF crColor)
{
	return GetNewPen( PS_DOT , 1 ,   crColor ) ;
}


HPEN CPenStorage::GetSolidPen( int nWidth, COLORREF crColor)
{
	return GetNewPen( PS_SOLID , nWidth ,   crColor ) ;
}

HPEN CPenStorage::GetSlippersPen() 
{
	return GetNewPen( PS_DOT , 1 ,  GetBraunColor() ) ;
}

void CPenStorage::SetDC( HDC   hdc ) 
{
	m_hdc = hdc;

	//m_Pen_Handle = GetStockObject( BLACK_PEN );
}

CSystemState::CSystemState()
{
	system_error =0;
}

void CSystemState::SystemErrorMessage( int i_err )
{
	system_error = i_err;
}

int CSystemState::GetSystemError() 
{
	return system_error;
}


CArchive::CArchive()
{
	hFile = NULL;


}

CArchive::CArchive(char *filename, int bwrite ) 
{
	is_open =0;
	istore = 0; 

	if ( bwrite == 0 )
	hFile  = CreateFile(   (LPCTSTR) filename , GENERIC_READ , FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_ARCHIVE, NULL);
	else
	{
	hFile  = CreateFile(   (LPCTSTR) filename , GENERIC_WRITE,FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_ARCHIVE, NULL);
     istore =1; 
	}
	if ( hFile  == INVALID_HANDLE_VALUE ) { hFile = NULL; return  ;}

	is_open = 1;
	return  ;

}

CArchive::~CArchive()
{

 
	if ( is_open == 0 ) return;
	is_open =0;
	if ( hFile ) CloseHandle( hFile);  

}

int CArchive::CloseArchive()
{
 
	if ( is_open == 0 ) return 0;
	is_open =0;
	if ( hFile ) CloseHandle( hFile);  
	return 1;
}

void CArchive::SetFilename(char *filename)
{
	 
}

int	CArchive::Read(  int count, void *p)
{
	 DWORD lpNumberOfBytesRead= 0;

	if ( hFile )
    ReadFile(
      hFile,
    (LPVOID)p,
    count,
    &lpNumberOfBytesRead,
   NULL
    );

	return lpNumberOfBytesRead;
}

int CArchive::Write(  int count, void *p)
{

		 DWORD lpNumberOfBytesWritten=0;

	  if ( hFile )
    WriteFile(
      hFile,
    (LPCVOID)p,
    count,
    &lpNumberOfBytesWritten,
   NULL
    );
	return lpNumberOfBytesWritten;
}

int CArchive::OpenArchiveToRead()
{

	return 0;
}

int CArchive::OpenArchiveToWrite()
{

	return 0;
}

int CArchive::IsStoring() 
{
	return istore; 

}

void CArchive::operator>>(const  DWORD &ivalue)
{

 DWORD lpNumberOfBytesRead;
 

if ( hFile )
    ReadFile(
      hFile,
    (LPVOID)&ivalue,
    4,
    &lpNumberOfBytesRead,
   NULL
    );


}

void CArchive::operator>>(const  BYTE &bvalue)
{
 
	DWORD lpNumberOfBytesRead;

 //mDebug.CDebugPut((char *)"Restoring  "  );

// if ( hFile == NULL )
// mDebug.CDebugPut((char *)"Null  "  );


 //char b[4];

if ( hFile )
    ReadFile(
      hFile,
    (LPVOID)&bvalue,
    1,
    &lpNumberOfBytesRead,
   NULL
    );

 //mDebug.CDebugPut((char *)"b =%d " ,(int)b[0] );

}

void CArchive::operator>>(const  WORD &wvalue)
{

		DWORD lpNumberOfBytesRead;
 

if ( hFile )
    ReadFile(
      hFile,
    (LPVOID)&wvalue,
    2,
    &lpNumberOfBytesRead,
   NULL
    );


 

}

void CArchive::operator<<(const  DWORD &ivalue)
{

	 DWORD lpNumberOfBytesWritten;

	  if ( hFile )
    WriteFile(
      hFile,
    (LPCVOID)&ivalue,
    4,
    &lpNumberOfBytesWritten,
   NULL
    );

}

void CArchive::operator<<(const  BYTE &bvalue)
{

	  DWORD lpNumberOfBytesWritten;

	 // 	mDebug.CDebugPut((char *)"Saving Operator1:%d",(int)bvalue );

	  if ( hFile )
    WriteFile(
      hFile,
    (LPCVOID)&bvalue,
    1,
    &lpNumberOfBytesWritten,
   NULL
    );


	//mDebug.CDebugPut((char *)"Saving Operator1:%d",(int)bvalue );


}

void CArchive::operator<<(const  WORD &wvalue)
{
		DWORD lpNumberOfBytesWritten;

	  if ( hFile )
    WriteFile(
      hFile,
    (LPCVOID)&wvalue,
   2,
    &lpNumberOfBytesWritten,
   NULL
    );


}
