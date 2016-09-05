
#pragma once


//  mode for painting

#define  SINGLELINE  0x0001
#define  DOUBLELINE  0x0002

#define  DRAWLINES   0x0004

//#define  DRAWBEND2   0x0008 
#define  IS_BEND     0x0010

#define  SWITCHOFF  0x0008 

void SystemErrorMessage( int i_err );
int  GetSystemError();
 void NormalizeRect( RECT *rc);
 void InflateRect( RECT *rc , int value);
 

 extern HINSTANCE hInst;	

 //  AfcFundation -------

 #include "AfcFoundation.h"

 //  AfcFundation -------|


 class CSystemState
{
public:
	CSystemState();
	virtual void SystemErrorMessage( int i_err );
	virtual int GetSystemError();
	int system_error;
};

 //  system class
class CSystem : public CSystemState
{
	public:
	void SystemErrorMessage( int i_err );
	int GetSystemError();
};


class CFPoint 
{

public:
	CFPoint::CFPoint();
	CFPoint::CFPoint(float ix,float iy);
	CFPoint::CFPoint(int ix,int iy);
    CFPoint::CFPoint(CPoint cp);
	CFPoint operator+(const CFPoint &second);
	CFPoint operator-(const CFPoint &second);
	float x;
	float y;
};

class CPointF  : public CPoint
{
public:
	CPointF( float x , float y);
	CPointF( CFPoint fpt );

};


class CVector 
{
public:
	CVector();
	CVector(CPoint start,CPoint End );
	CVector(CFPoint start,CFPoint End );
	CPoint mStart;
	CPoint mEnd;
};

class CFVector 
{
public:
	CFVector();
	CFVector(CPoint start,CPoint End );
	CFVector(CFPoint start,CFPoint End );
	CFPoint mStart;
	CFPoint mEnd;
};


class CSegment 
{
public:
	CSegment();
	CSegment(CPoint start,CPoint End );
	CSegment(CFPoint start,CFPoint End );
	void CSegment::DrawSegment( HDC hdc, COLORREF rf ); 
	CFPoint mStart;
	CFPoint mEnd;
};

 
class CCircle 
{
public:
	CCircle();
	CCircle( CFPoint center , float fradius  );
	CFPoint mCenter;
	float  mRadius;
};

  

 

class CTwoPoints
{
public:
	CTwoPoints();
	CTwoPoints(  CFPoint point1, CFPoint point2  );
	CFPoint mPoint1;
	CFPoint mPoint2;
	int  result;
};

 
 


class CLineObject
{
public:
	CLineObject();
	CLineObject( CPoint FirstPoint,CPoint SecondPoint );
	CLineObject( CFPoint FirstPoint,CFPoint SecondPoint );
	CLineObject( double ACoeff,double BCoeff ,CFPoint ThroughPoint);
	CLineObject::CLineObject( double dACoeff,double dBCoeff ,double dCCoeff) ;
	CLineObject::CLineObject( double MCoeff ,double DCoeff  );
	void LineFromPoints(CPoint FirstPoint,CPoint SecondPoint);
	void LineFromPoints(CFPoint FirstPoint,CFPoint SecondPoint);
	void LinePenpendicularFromLineAndPoint( double ACoeff,double BCoeff ,CFPoint ThroughPoint);
	void   DrawLine( HDC hdc ,COLORREF rf );
	double GetA();
	double GetB();
	double GetC();
private:
	double ACoeff;
	double BCoeff;
	double CCoeff;
};

 // the global structures follows

class CRail;

typedef struct {
	CFPoint StartPoint;     // mouse hit location
	CFPoint EndPoint;       // mouse move to/release location
	int mResourceId;
	HBITMAP bmpHandle;
	int mType;
	SIZE sz;

 	CRail	*mHeadPointer;     //  pointer to the element where head  cars are on [0]
 	CRail	*mTailPointer;     //  pointer to the element where trail cars are on [1]
	 
	int     OnTheBend[2];

	// float  mCurrHeadPos;
	// float  mCurrHeadCount;   // count     0 -->  45   or  45---> 0

	int    mRailHeadDir;         // direction of the head axis  (-1)  - fm 1to0  (1)  from 0 --> 1
	int    mRailTailDir;         // direction of the trail axis  !!!!!! OBSOLITE  mDrawData.Turn !!!!!!!
	int    mVagonDir;            // is  vagon moving forward or bacward   <-  H====T  ->  mDrawData show s the same
	WORD   mHeadHandle;    // if <> 0   the connected to other vagon
	WORD   mTailHandle;    // if <> 0   the connected to other vagon
 
	// int    mOtherEndpoint;       // endpoint connected to the current endpoint
	// int    mOnTheSwitchBend;     // 1 on the bend switch
    // int    mOnTheSwitchBendTail;

	float   mInitialLenght;
	float   mCorrection;

	// int   mInternalLoopError;
	// int   mInternalLoops;
}   DRAWVAGONDATASTRUCT;


typedef struct {
	CFPoint StartPoint;     // mouse hit location
	CFPoint EndPoint;       // mouse move to/release location
	CFPoint CenterPoint;
	CFPoint ExtendedPoint;
	float   Radius;
	int    Width;
	int    Turn;
	float  mStartAngle;
	float  mSweepAngle;
	CRect  mUpdRect;
	WORD   ThisID;
	WORD   ConnID[3]; 
	DRAWVAGONDATASTRUCT *pVagonData;
}   DRAWDATASTRUCT;

 
typedef struct {
	int     mtype;
	float   Radius;
	int     Width;
	int     Lenght;
	int     iResBitmap;
	WORD    Turn;
	WORD    tType;
	float  mStartAngle;
	float  mSweepAngle;
	HANDLE  bmHandle;
}   TMPOBJECT_DRAWDATASTRUCT;


class CTrainVagon;

typedef struct {
	int     MxLoop;
	int     epFix;
	int     epFloat;
	CTrainVagon *pVagonFix;
	CTrainVagon *pVagonFloat;
}   COLISSION_DRAWDATASTRUCT;



typedef struct {
	CPoint DPoint[6]; 
}   DEBUGDRAWDATASTRUCT;



   int PointInsideTheRect( CPoint cp , CRect rc ) ;
   int PointInsideTheRect( CFPoint cp , CRect rc ) ;


 class CMenuGroup {
 public:
	CMenuGroup();
	CMenuGroup( HMENU hmenu , UINT idFirst, UINT idLast);
	void CMenuGroupSet( HMENU hmenu , UINT idFirst, UINT idLast);
    int CMenuGroupCheck (UINT idCheck);
	int CMenuGroupUncheck();
	UINT CMenuGroupGetCurrent();
	HMENU mhmenu;
    UINT  midFirst;
	UINT midLast;
	UINT midCheck;
 };

#define MAXMENUGRP 10
 class CMenuGroupTable {
 public:
	CMenuGroupTable();
	void CMenuGroupTableAdd( CMenuGroup *ptr );
    int CMenuGroupTableCheck (UINT idCheck);
	int CMenuGroupTableUncheck();
	UINT CMenuGroupTableGetCurrent();
    CMenuGroup *List[MAXMENUGRP];
	int  mCount;
 };





class CTempVector {
public: 
	CTempVector();

	void    CTempVectorSetTmpObject(TMPOBJECT_DRAWDATASTRUCT *tmpObjS );
	void	CTempVectorUpdateStart( CPoint start );
	void	CTempVectorUpdateEnd( CPoint end );
	void    CTempVectorPaintTmpObject(   HDC hdc );
	CPoint	CTempVectorGetPoint( int ipoint );
	CRect   CTempVectorGetRect();
	void    CTempVectorSetRect(CRect rc);
	DRAWDATASTRUCT *CTempVectorGetDrawData();
 
private:
	DRAWDATASTRUCT mData;
	DRAWVAGONDATASTRUCT mDrawVagonData;
	int		mObjectType;
	CRect   mUnionRect;
	void	CTempVectorPaintBend(   HDC hdc );
	void	CTempVectorPaintStright(   HDC hdc );
	void	CTempVectorPaintSwitch (   HDC hdc );
	void	CTempVectorPaintEnd(   HDC hdc );
	void	CTempVectorPaintTrain( HDC hdc );
	void	CTempVectorPaintSemaphore( HDC hdc );
};


 class CPenStorage
{
public:
	CPenStorage();
	~CPenStorage();
	HPEN GetPen(int fnPenStyle, int nWidth, COLORREF crColor);
	HPEN GetDottedPen(COLORREF crColor);
	HPEN GetSolidPen( int nWidth, COLORREF crColor);
	HPEN GetSlippersPen(); 
	void SetDC( HDC   hdc );
	HPEN m_pen;
	HPEN m_oldpen;
	HDC  m_hdc;
	int  m_Pen_Error;
private:
	HPEN GetNewPen( int fnPenStyle, int nWidth, COLORREF crColor );
}; 

 



//CArchive::CArchive(CFile* pFile, UINT nMode, int nBufSize, void* lpBuf) 

 

/*

Header: afx.h

CFPoint	CFPoint::operator+(const CFPoint &second) 
{
	x += second.x;
	y += second.y;
	return CFPoint(x,y);
}

 CArchive::operator <<
Stores objects and primitive types to the archive.

CArchive::operator >>
Loads objects and primitive types from the archive



void CArchive::operator>>(const  int &ivalue);
void CArchive::operator>>(const  BYTE &bvalue);
void CArchive::operator>>(const  WORD &wvalue);
void CArchive::operator>>(const  *char &cvalue);

store to the archive---

void CArchive::WriteString( 
   LPCTSTR lpsz  
);

void CArchive::Write( 
   const void* lpBuf, 
   UINT nMax  
)


CArchive& operator <<( 
   BYTE by  
); 
CArchive& operator <<( 
   WORD w  
); 
CArchive& operator <<( 
   LONG l  
); 
CArchive& operator <<( 
   DWORD dw  
); 
CArchive& operator <<( 
   float f  
); 
CArchive& operator <<( 
   double d  
); 
CArchive& operator <<( 
   int i  
); 
CArchive& operator <<( 
   short w  
); 
CArchive& operator <<( 
   char ch  
); 
CArchive& operator<<( 
   wchar_t ch  
); 
CArchive& operator <<( 
   unsigned u  
); 
CArchive& operator <<( 
   bool b  
); 
CArchive& operator<<( 
   ULONGLONG dwdw  
); 
CArchive& operator<<( 
   LONGLONG dwdw  
);

*/



