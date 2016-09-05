

#include "stdafx.h"
#include "MouseTest.h"

#include "Windowsx.h"
#include "Math.h"
#include <stdio.h>
 
#include "Foundation.h"
#include "ComputeGA.h"

#include "Debug.h"

#include "Graphics.h"
 

extern CDebug mDebug;

//extern RECT client_rect;

COLORREF GetBlackColor() 
{
	return RGB( 0 ,0,0);
}
 
COLORREF GetGrayColor()
{
	return	RGB( 127 ,127,127);
}

COLORREF GetRedColor()
{
	return	RGB( 255 ,0,0);
}

COLORREF GetBlueColor()
{
	return	RGB( 0,0,255);
} 

COLORREF GetBlueDarkColor() 
{
	return	RGB( 0,0,127);
} 

COLORREF GetRedDarkColor() 
{
	return	RGB( 127 ,0,0);
}

COLORREF GetWhiteColor()
{
	return	RGB( 255 ,255,255);
}

COLORREF GetBraunColor()
{
	return	RGB( 100 ,100,0);
}

COLORREF GetGreenColor()
{
	return	RGB( 0 ,255 ,0);
}

 
COLORREF GetDarkGreenColor()
{
	return	RGB( 0 ,127 ,0);
}


void OnDrawRect( CRect  *srect , HDC hdc )
{
	MoveToEx(hdc ,  srect->left     ,srect->top ,    (LPPOINT) NULL); 
	LineTo(  hdc ,  srect->right  ,  srect->top );
	LineTo(  hdc ,  srect->right ,  srect->bottom );
	LineTo(  hdc ,  srect->left  ,  srect->bottom );
	LineTo(  hdc ,  srect->left  ,  srect->top );
}

void DrawRectTilted( CPoint p1, CPoint p2, CPoint p3, CPoint p4  , HDC hdc )
{
	MoveToEx(hdc ,  p1.x    ,p1.y ,    (LPPOINT) NULL); 
	LineTo(  hdc ,  p2.x    ,p2.y   );
	LineTo(  hdc ,  p3.x    ,p3.y   );
	LineTo(  hdc ,  p4.x    ,p4.y   );
	LineTo(  hdc ,  p1.x    ,p1.y   );
}
// Four overridden functions  to draw point

void DrawPointPlus( CFPoint cp , HDC hdc , COLORREF clr , int weight) 
{
	CPointF  ptf = CPointF( cp  ); 
    DrawPointPlus( ptf ,   hdc ,   clr ,   weight) ;
}

void DrawPointCross( CFPoint cp , HDC hdc , COLORREF clr , int weight) 
{
	CPointF  ptf = CPointF( cp  ); 
    DrawPointCross( ptf ,   hdc ,   clr ,   weight) ;
}

void DrawPointPlus( CPoint cp , HDC hdc , COLORREF clr , int weight) 
{
	HPEN pen;
	pen = CreatePen( PS_SOLID , weight , clr );
		if ( pen == NULL ) 
	{
		SystemErrorMessage( 1 );
		return;
	}
	HPEN oldpen = (HPEN)SelectObject( hdc , pen );
	
	MoveToEx(hdc ,  cp.x-5     , cp.y ,    (LPPOINT) NULL); 
	LineTo(  hdc ,  cp.x+5   ,  cp.y );
	MoveToEx(hdc ,  cp.x      , cp.y-5 ,    (LPPOINT) NULL); 
	LineTo(  hdc ,  cp.x    ,  cp.y+5 );

	SelectObject( hdc , oldpen );
	DeleteObject( pen );
}

void DrawPointCross( CPoint cp , HDC hdc , COLORREF clr , int weight) 
{
	HPEN pen;
	pen = CreatePen( PS_SOLID , weight , clr );
		if ( pen == NULL ) 
	{
		SystemErrorMessage( 1 );
		return;
	}
	HPEN oldpen = (HPEN)SelectObject( hdc , pen );
	
	MoveToEx(hdc ,  cp.x-4     , cp.y-4 ,    (LPPOINT) NULL); 
	LineTo(  hdc ,  cp.x+4   ,  cp.y+4 );
	MoveToEx(hdc ,  cp.x-4      , cp.y+4 ,    (LPPOINT) NULL); 
	LineTo(  hdc ,  cp.x+4    ,  cp.y-4 );

	SelectObject( hdc , oldpen );
	DeleteObject( pen );
}

void  DrawTmpSegment ( CTwoPoints mTwoPoint   , HDC hdc )
{
	CPenStorage   mPenSet ;  // destruktor called outomatically
	mPenSet.SetDC( hdc);
	//if (   mPenSet.GetSolidPen(  4 , GetBraunColor() ) == NULL ) return ;
	if (   mPenSet.GetDottedPen(GetGrayColor()) == NULL ) return ;

	MoveToEx(hdc , (int) mTwoPoint.mPoint1.x    , (int)mTwoPoint.mPoint1.y ,    (LPPOINT) NULL); 
	LineTo(  hdc , (int) mTwoPoint.mPoint2.x    , (int)mTwoPoint.mPoint2.y );
}

/////////////////////////////  Lines primitives
 
void  DrawLine ( CLineObject mLine   , HDC hdc )
{
	DrawLine (   mLine   ,   hdc , GetBlueColor()  );
}


void  DrawLine ( CLineObject mLine   , HDC hdc , COLORREF rf  )
{
	CFPoint fp1 = CFPoint(10,10) ;
	CFPoint fp2 = CFPoint( 2000 , 1200 ) ;

	if ( GetSystemError() != 0) 
	{
		return;
	}
 
	if ( (int)mLine.GetB() == 0 )
	{
		if ( (int)mLine.GetA() != 0 )  
		{
			fp1.x = fp2.x = - (float)mLine.GetC()  /(float)mLine.GetA();
			fp1.y = 0;
			fp2.y = (float)1200 ;
		}
		else return ;
	}
	else
	{
		fp1.y = (float)((-mLine.GetA() * fp1.x - mLine.GetC()  ) /mLine.GetB());
		fp2.y = (float)((-mLine.GetA() * fp2.x - mLine.GetC()  ) /mLine.GetB());
	}

	CPointF pt1 = CPointF( fp1 ) ;
	CPointF pt2 = CPointF( fp2 ) ;

	CPenStorage   mPenSet ;  // destruktor called outomatically
	mPenSet.SetDC( hdc);
	if (   mPenSet.GetPen( PS_SOLID , 1 , rf ) == NULL ) return ;

	/*
	HPEN pen;
	pen = CreatePen( PS_SOLID , 1 , rf);
		if ( pen == NULL ) 
	{
		SystemErrorMessage( 1 );
		return;
	}
	HPEN oldpen = (HPEN)SelectObject( hdc , pen );
	*/

	MoveToEx(hdc ,  pt1.x     , pt1.y,    (LPPOINT) NULL); 
	LineTo(  hdc ,  pt2.x     , pt2.y );
 
	//SelectObject( hdc , oldpen );
	//DeleteObject( pen );
}

// could be problem

void DrawTwoParLinesExt( DRAWDATASTRUCT *p , DWORD mode , HDC hdc ) 
{

	float  dx = (float)(p->EndPoint.x-p->StartPoint.x);
	float  dy = (float)(p->EndPoint.y-p->StartPoint.y);

	float fl = sqrt((float)(dx*dx+dy*dy));
	float fdisp =(float) p->Width;

	float dxx =    (fdisp * ((float)dy)/fl);
    float dyy =    (fdisp * ((float)dx)/fl);

	POINT SPoint0; 
	POINT SPoint1;
	POINT EPoint0; 
	POINT EPoint1;

	SPoint0.x = (int)roundToNearest((p->StartPoint.x + dxx));
	SPoint0.y = (int)roundToNearest((p->StartPoint.y - dyy));
	SPoint1.x = (int)roundToNearest((p->StartPoint.x - dxx));
	SPoint1.y = (int)roundToNearest((p->StartPoint.y + dyy));

	EPoint0.x = (int)roundToNearest((p->EndPoint.x + dxx));
	EPoint0.y = (int)roundToNearest((p->EndPoint.y - dyy));
	EPoint1.x = (int)roundToNearest((p->EndPoint.x - dxx));
	EPoint1.y = (int)roundToNearest((p->EndPoint.y + dyy));

	// need to create CFpoints when p become modified

    if (  mode&DRAWLINES )
	{
		MoveToEx(hdc ,  SPoint0.x     , SPoint0.y ,    (LPPOINT) NULL); 
		LineTo(  hdc , EPoint0.x ,  EPoint0.y  );
		MoveToEx(hdc ,  SPoint1.x     , SPoint1.y ,    (LPPOINT) NULL); 
		LineTo(  hdc , EPoint1.x ,  EPoint1.y  );
	}

	// compute update region
 
}


void DrawBox( DRAWDATASTRUCT *p , DWORD mode , HDC hdc ) 
{

	if ( GetSystemError() != 0) 
	{
		return;
	}

	float  dx = (float)(p->EndPoint.x-p->StartPoint.x);
	float  dy = (float)(p->EndPoint.y-p->StartPoint.y);

	float fl = sqrt((float)(dx*dx+dy*dy));
	float fdisp =(float) p->Width;

	float dxx =    (fdisp * ((float)dy)/fl);
    float dyy =    (fdisp * ((float)dx)/fl);

	POINT SPoint0; 
	POINT SPoint1;
	POINT EPoint0; 
	POINT EPoint1;

	SPoint0.x = (int)roundToNearest((p->StartPoint.x + dxx));
	SPoint0.y = (int)roundToNearest((p->StartPoint.y - dyy));
	SPoint1.x = (int)roundToNearest((p->StartPoint.x - dxx));
	SPoint1.y = (int)roundToNearest((p->StartPoint.y + dyy));

	EPoint0.x = (int)roundToNearest((p->EndPoint.x + dxx));
	EPoint0.y = (int)roundToNearest((p->EndPoint.y - dyy));
	EPoint1.x = (int)roundToNearest((p->EndPoint.x - dxx));
	EPoint1.y = (int)roundToNearest((p->EndPoint.y + dyy));

	// need to create CFpoints when p become modified

  //  if (  mode&DRAWLINES )
	{
		MoveToEx(hdc ,  SPoint0.x     , SPoint0.y ,    (LPPOINT) NULL); 
		LineTo(  hdc , EPoint0.x ,  EPoint0.y  );

		MoveToEx(hdc ,  EPoint0.x     , EPoint0.y ,    (LPPOINT) NULL); 
		LineTo(  hdc , EPoint1.x ,  EPoint1.y  );

		MoveToEx(hdc ,  SPoint0.x     , SPoint0.y ,    (LPPOINT) NULL); 
		LineTo(  hdc , SPoint1.x ,  SPoint1.y  );


		MoveToEx(hdc ,  SPoint1.x     , SPoint1.y ,    (LPPOINT) NULL); 
		LineTo(  hdc , EPoint1.x ,  EPoint1.y  );
	}

}



void PaintBend( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc )
{
	CPenStorage   mPenSet ;  // destruktor called outomatically
	mPenSet.SetDC( hdc);

 
	if (   mPenSet.GetSolidPen(  4 , GetBraunColor() ) == NULL ) return ;

	// draw - railway sleepers
	//  delta need consider the radius  greater radius less delta - math to be involved
	float delta1 = 6;

	float delta  = 6*(p->mSweepAngle/45)*(p->Radius/100); 

	delta = p->mSweepAngle/delta;

	if ( delta < 0 ) delta = -delta ; 
 
	float start_angle = p->mStartAngle-(float)2.1;
	float end_angle   = p->mSweepAngle + p->mStartAngle;

	if (  start_angle < end_angle ) //  left turn-  lEFT bEND
	{
		delta+=(float)0.1;
		start_angle  = start_angle + delta -2 ; //  + 12/delta;
		while ( start_angle <  end_angle  )
		{
		CFPoint mEndBendPoint ; 
		mEndBendPoint.x = p->CenterPoint.x +  (float)p->Radius*(float)cos(  PI180*(start_angle ) );
		mEndBendPoint.y = p->CenterPoint.y -  (float)p->Radius*(float)sin(  PI180*(start_angle ) );
 		CFPoint Ext =  ComputeExtendedLinePoint(  p->CenterPoint , p->StartPoint , mEndBendPoint );
		CSegment mCs = ComputeSectionPependicularAtPointExt( mEndBendPoint , Ext , 0  , (float)(p->Width+2) );
		CFPoint p1 =  mCs.mStart; 
		CFPoint p2 =  mCs.mEnd;
		MoveToEx(hdc , (int)roundToNearest(p1.x ) ,(int)roundToNearest(p1.y) ,    (LPPOINT) NULL);
		LineTo(  hdc , (int)roundToNearest(p2.x  ) ,  (int)roundToNearest(p2.y ));
	    start_angle += delta;	
		}
	}
	else   // right tern rIGHT bEND
	{
		float start_angle = p->mStartAngle;
		float end_angle   = p->mSweepAngle + p->mStartAngle  +(float)1.6;
		while (   start_angle  > end_angle )
		{
		CFPoint mEndBendPoint ; 
		mEndBendPoint.x = p->CenterPoint.x +  (float)p->Radius*(float)cos(  PI180*(end_angle ) );
		mEndBendPoint.y = p->CenterPoint.y -  (float)p->Radius*(float)sin(  PI180*(end_angle ) );
 		CFPoint Ext =  ComputeExtendedLinePoint(  p->CenterPoint , p->StartPoint , mEndBendPoint );
		CSegment mCs = ComputeSectionPependicularAtPointExt( mEndBendPoint , Ext ,0  , (float)(p->Width+2) );
		CFPoint p1 =  mCs.mStart; 
		CFPoint p2 =  mCs.mEnd;
		MoveToEx(hdc , (int)roundToNearest(p1.x ) ,(int)roundToNearest(p1.y) ,    (LPPOINT) NULL);
		LineTo(  hdc , (int)roundToNearest(p2.x  ) ,  (int)roundToNearest(p2.y ));
	    end_angle += delta;	
		} 
	}

 
	double txcos = cos ( PI180*p->mStartAngle  );
	double tysin = sin ( PI180*p->mStartAngle  );

	if ( SWITCHOFF&mode )
	{

		if (   mPenSet.GetDottedPen( GetGrayColor() ) == NULL ) return ;
		MoveToEx(hdc,(int)roundToNearest( p->CenterPoint.x +   p->Radius*txcos)  ,(int)roundToNearest( p->CenterPoint.y -  p->Radius*tysin) ,(LPPOINT) NULL);
		AngleArc(hdc,(int)roundToNearest(p->CenterPoint.x), (int)roundToNearest(p->CenterPoint.y) , (int)roundToNearest( p->Radius),  p->mStartAngle , p->mSweepAngle );
	}


	if (   mPenSet.GetSolidPen(  mode&0x3 , GetBlackColor() ) == NULL ) return ;

	MoveToEx(hdc,(int)roundToNearest( p->CenterPoint.x +( p->Radius+p->Width)*txcos )  ,(int)roundToNearest( p->CenterPoint.y - ( p->Radius+p->Width)*tysin ) , (LPPOINT) NULL);
	AngleArc(hdc,(int)roundToNearest( p->CenterPoint.x), (int)roundToNearest(p->CenterPoint.y) , (int)roundToNearest( p->Radius+p->Width), p->mStartAngle , p->mSweepAngle );

	MoveToEx(hdc,(int)roundToNearest( p->CenterPoint.x + ( p->Radius-p->Width)*txcos ) ,(int)roundToNearest( p->CenterPoint.y - ( p->Radius-p->Width)*tysin ) ,(LPPOINT) NULL);
	AngleArc(hdc,(int)roundToNearest( p->CenterPoint.x), (int)roundToNearest(p->CenterPoint.y) ,(int)roundToNearest( p->Radius-p->Width),  p->mStartAngle ,  p->mSweepAngle );
 
	if (   mPenSet.GetSolidPen(  4 , GetBraunColor() ) == NULL ) return ; 
 

	int index = 2; //ffor switch
	if ( mode&IS_BEND ) index = 1;

 
	 if ( p->ConnID[index] == 0 ) 
	 {
		CFPoint mEndBendPoint ; 
		mEndBendPoint.x = p->CenterPoint.x +  (float)p->Radius*(float)cos(  PI180*(p->mStartAngle+p->mSweepAngle) );
		mEndBendPoint.y = p->CenterPoint.y -  (float)p->Radius*(float)sin(  PI180*(p->mStartAngle+p->mSweepAngle) );
		DrawPointCross(  mEndBendPoint ,  hdc , GetBlueColor() ,   2);
	 }
	
	 
	if ( p->ConnID[0] == 0 )  
		DrawPointCross( p->StartPoint ,  hdc , GetRedColor() ,   2);
 
}
 


void ShadowStraightRail( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc )
{
	DrawTwoParLinesExt(  p , 0 , hdc ); 
}


void PaintBendRail( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc )
{
	 ShadowStraightRail( p,   mode ,    hdc ); 
	 PaintBend(  p,   mode | IS_BEND ,    hdc ); 
}


void PaintEndRail( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc ) 
{
	CPenStorage   mPenSet ;  // destruktor called outomatically
	mPenSet.SetDC( hdc);

	float fwide = (float)p->Width+2;  // the half size of the bars  in relations to tracks wide

	// draw  railway sleeper
	if (   mPenSet.GetSolidPen(  4 , GetBraunColor() ) == NULL ) return ;
	float delta = 12;
	float fst = 14;
	float flenght = GetDistance (p->StartPoint , p->EndPoint);
	flenght -=  8;

	while ( fst < flenght)
	{
		CSegment mCs = ComputeSectionPependicularAtPoint( p->StartPoint , p->EndPoint ,  fst  , (float)fwide );
		CFPoint p1 =  mCs.mStart; 
		CFPoint p2 =  mCs.mEnd;
		MoveToEx(hdc , (int)roundToNearest(p1.x ) ,(int)roundToNearest(p1.y) ,    (LPPOINT) NULL);
		LineTo(  hdc , (int)roundToNearest(p2.x  ) ,  (int)roundToNearest(p2.y ));
		fst += delta;
	}
	
	CSegment mCs = ComputeSectionPependicularAtPoint( p->StartPoint , p->EndPoint ,  GetDistance(p->StartPoint , p->EndPoint )-3 ,(float)fwide );
	MoveToEx(hdc , (int)roundToNearest(mCs.mStart.x ) ,(int)roundToNearest(mCs.mStart.y) ,    (LPPOINT) NULL);
	LineTo(  hdc , (int)roundToNearest(mCs.mEnd.x ) ,  (int)roundToNearest(mCs.mEnd.y ));
	
// rails	
	if (   mPenSet.GetSolidPen(  mode&0x03 , GetBlackColor() ) == NULL ) return ;
	DrawTwoParLinesExt(  p , DRAWLINES , hdc ); 
//  direction
	if (   mPenSet.GetDottedPen( GetGrayColor() ) == NULL ) return ;
	MoveToEx(hdc , (int)roundToNearest(p->StartPoint.x ) ,(int)roundToNearest(p->StartPoint.y) ,    (LPPOINT) NULL);
	LineTo(  hdc , (int)roundToNearest(p->EndPoint.x ) ,  (int)roundToNearest(p->EndPoint.y ));

// draw stop bar -------------------------

	if (   mPenSet.GetSolidPen(   10 , GetBraunColor() ) == NULL ) return ; 
	CFPoint p1 =  ComputeTwoParLines( p->StartPoint ,p->EndPoint , (float)fwide , 0 ) ;
 	CFPoint p2 =  ComputeTwoParLines( p->StartPoint ,p->EndPoint , (float)fwide , 1 ) ;
	MoveToEx(hdc , (int)roundToNearest(p1.x ) ,(int)roundToNearest(p1.y) ,    (LPPOINT) NULL);
	LineTo(  hdc , (int)roundToNearest(p2.x  ) ,  (int)roundToNearest(p2.y ));
// draw connection mark
	if ( p->ConnID[1] == 0 ) 
		DrawPointCross( p->EndPoint ,  hdc , GetRedColor() ,   2);
}

char tb[24*64+8];

/*


		memset( tb,0x0, 24*64);

		memset( tb,0xFF, 4*64  );
		 tb[0+4*0] =0;
		 tb[0+4*1] =0;

		 tb[1+4*0] =0x3C;
		 tb[1+4*1] =0x3C;

		 tb[2+4*0] =0;
		 tb[2+4*1] =0;

		 tb[0+4*62] =0;
		 tb[0+4*63] =0;

		 tb[1+4*62] =0x3C;
		 tb[1+4*63] =0x3C;

		 tb[2+4*62] =0;
		 tb[2+4*63] =0;

*/

void PaintTrainBitmap( CFPoint StartPoint,CFPoint EndPoint,HBITMAP bmpExercising , HDC hdc )
{
	 HDC   MemDCExercising;

	 float angle = 	ComputeVectorAngle( StartPoint,  EndPoint  ) +90 ;

//mDebug.CDebugPut((char *)" = %d   %d", (int)angle , (int)StartPoint.x );
 
	    MemDCExercising = CreateCompatibleDC(hdc);
        SelectObject(MemDCExercising, bmpExercising);
		BITMAP bm;
		GetObject ( bmpExercising, sizeof(bm), &bm );

		HBITMAP bmpMono=NULL;

		/*
				memset( tb,0x0, 24*64);

		memset( tb,0xFF, 4*64  );
		 tb[0+4*0] =0;
		 tb[0+4*1] =0;

		 tb[1+4*0] =0x3C;
		 tb[1+4*1] =0x3C;

		 tb[2+4*0] =0;
		 tb[2+4*1] =0;

		 tb[0+4*62] =0;
		 tb[0+4*63] =0;

		 tb[1+4*62] =0x3C;
		 tb[1+4*63] =0x3C;

		 tb[2+4*62] =0;
		 tb[2+4*63] =0;
		// bmpMono = CreateBitmap(24,64,1,1, &tb);
		*/

		CRect cr;
	  	POINT  AP[4];

		//AP[0].x = StartPoint.x + (bm.bmWidth>>1);

		// find rotate point from StartPoint      
		//

		CFPoint FPoffs = ComputeTwoParLines(StartPoint,  EndPoint , (float)(bm.bmWidth>>1) , 1 );   // why one [1] is OK

		 AP[0].x = (long)roundToNearest(FPoffs.x)  ;
		 AP[0].y = (long)roundToNearest(FPoffs.y);

		cr.left  = AP[0].x  ;
		cr.right = cr.left + bm.bmWidth ;
		cr.top = AP[0].y;
		cr.bottom = AP[0].y+bm.bmHeight; 


		double dcos_angle = cos(  PI180*(angle) );
		double dsin_angle = sin(  PI180*(angle) );

		AP[1].x = (long)((float)cr.left		+	roundToNearest(bm.bmWidth*dcos_angle));
		AP[1].y = (long)((float)cr.top		-	roundToNearest(bm.bmWidth*dsin_angle)); 

		AP[2].x = (long)((float)cr.left		+	roundToNearest(bm.bmHeight*dsin_angle));
		AP[2].y = (long)((float)cr.top	    +	roundToNearest(bm.bmHeight*dcos_angle)); 

	//	AP[3].x = AP[2].x		+  	roundToNearest(bm.bmWidth*cos(  PI180*(angle) ));
	//	AP[3].y = AP[2].y	    -  	roundToNearest(bm.bmWidth*sin(  PI180*(angle) ));  

		  //  0,0  source copy OK
		PlgBlt( hdc , AP   ,MemDCExercising ,   0,  0, bm.bmWidth ,bm.bmHeight,bmpMono  , 0, 0);

		DeleteObject(bmpMono);
	    DeleteDC(MemDCExercising);
}




// rather paint vagon

void	PaintTrain( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc  )
{
	CPenStorage   mPenSet ;  // destruktor called outomatically
	mPenSet.SetDC( hdc);
	//  big red 

	 if ( p->pVagonData != NULL )
	 {
	 	PaintTrainBitmap(p->StartPoint,p->EndPoint, p->pVagonData->bmpHandle ,   hdc );
	 }
	 else
	{
		// body
		/*
		if ( p->Turn != 0 )
		{
			if (   mPenSet.GetSolidPen( 18 , GetDarkGreenColor()  ) == NULL ) return ;
		}
		else
		{
		if (   mPenSet.GetSolidPen( 20 , GetRedColor()  ) == NULL ) return ;
		}
		*/
		//MoveToEx(hdc ,  (int)p->StartPoint.x    ,  (int)p->StartPoint.y ,    (LPPOINT) NULL); 
		//LineTo(  hdc , (int)p->EndPoint.x    ,  (int)p->EndPoint.y  );

	 //  frame
		if (   mPenSet.GetSolidPen(   2 , GetBlueColor()  ) == NULL ) return ;
 		DrawBox(  p , DRAWLINES , hdc ); 
	//  head mark
		if (   mPenSet.GetSolidPen(  12 , GetBlueColor()  ) == NULL ) return ; 
		CFPoint st1 = GetPointFmStart( p->StartPoint , p->EndPoint  ,  -2 );
		CFPoint st2 = GetPointFmStart( p->StartPoint  , p->EndPoint  ,  2 );
		MoveToEx(hdc ,  (int)st1.x    ,  (int)st1.y ,    (LPPOINT) NULL); 
		LineTo(  hdc , (int)st2.x     ,  (int)st2.y );
	}
 
}

void ComputeBandingRectTrain( DRAWDATASTRUCT *p  )
{

	float  dx = (float)(p->EndPoint.x-p->StartPoint.x);
	float  dy = (float)(p->EndPoint.y-p->StartPoint.y);

	float fl = sqrt((float)(dx*dx+dy*dy));
	float fdisp =(float) p->Width;

	float dxx =    (fdisp * ((float)dy)/fl);
    float dyy =    (fdisp * ((float)dx)/fl);

	POINT SPoint0; 
	POINT SPoint1;
	POINT EPoint0; 
	POINT EPoint1;

	SPoint0.x = (int)roundToNearest((p->StartPoint.x + dxx));
	SPoint0.y = (int)roundToNearest((p->StartPoint.y - dyy));
	SPoint1.x = (int)roundToNearest((p->StartPoint.x - dxx));
	SPoint1.y = (int)roundToNearest((p->StartPoint.y + dyy));

	EPoint0.x = (int)roundToNearest((p->EndPoint.x + dxx));
	EPoint0.y = (int)roundToNearest((p->EndPoint.y - dyy));
	EPoint1.x = (int)roundToNearest((p->EndPoint.x - dxx));
	EPoint1.y = (int)roundToNearest((p->EndPoint.y + dyy));

 
	CFPoint P1 =  GetPointFmStart(  p->StartPoint, p->EndPoint , -10 );
	CFPoint P2 =  GetPointFmStart(  p->StartPoint, p->EndPoint , 10 +GetDistance(p->StartPoint, p->EndPoint));

int table[6];

	table[0] = (int)SPoint0.x;
	table[1] = (int)SPoint1.x;
	table[2] = (int)EPoint0.x;
	table[3] = (int)EPoint1.x;
	table[4] = (int)P1.x;
	table[5] = (int)P2.x;
	// find the trailing ([   ]) extension

	p->mUpdRect.left =  GetLowestValue(  table, 6 );
	p->mUpdRect.right =  GetHighestValue(  table, 6 );

	table[0] = (int)SPoint0.y;
	table[1] = (int)SPoint1.y;
	table[2] = (int)EPoint0.y;
	table[3] = (int)EPoint1.y;
	table[4] = (int)P1.y;
	table[5] = (int)P2.y;


	p->mUpdRect.top =  GetLowestValue(  table, 6 );
	p->mUpdRect.bottom =  GetHighestValue(  table, 6 );

	p->mUpdRect.NormalizeRect();
	p->mUpdRect.InflateRect( 2 );

}


void PaintStraightRail( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc )
{
	CPenStorage   mPenSet ;  // destruktor called outomatically
	mPenSet.SetDC( hdc);
	if (   mPenSet.GetSolidPen(  4 , GetBraunColor() ) == NULL ) return ;
	
	float fwide = (float)(p->Width+2);

	// draw  railway sleepers
	float delta = 12; // in pixels
	float fst = 14;
	float flenght = GetDistance (p->StartPoint , p->EndPoint)+6;
	flenght -=  8;

	while ( fst < flenght)
	{
		CSegment mCs = ComputeSectionPependicularAtPoint( p->StartPoint , p->EndPoint ,  fst  , (float)fwide );
		CFPoint p1 =  mCs.mStart; CFPoint p2 =  mCs.mEnd;
		MoveToEx(hdc , (int)roundToNearest(p1.x ) ,(int)roundToNearest(p1.y) ,    (LPPOINT) NULL);
		LineTo(  hdc , (int)roundToNearest(p2.x  ) ,  (int)roundToNearest(p2.y ));	
		fst += delta;
	}
	CSegment mCs;

// two connecting bars at each end
	mCs = ComputeSectionPependicularAtPoint( p->StartPoint , p->EndPoint , 3 ,(float)fwide);
	MoveToEx(hdc , (int)roundToNearest(mCs.mStart.x ) ,(int)roundToNearest(mCs.mStart.y) ,    (LPPOINT) NULL);
	LineTo(  hdc , (int)roundToNearest(mCs.mEnd.x ) ,  (int)roundToNearest(mCs.mEnd.y ));

	mCs = ComputeSectionPependicularAtPoint( p->StartPoint, p->EndPoint , GetDistance(p->StartPoint , p->EndPoint )-3 ,(float)fwide);
	MoveToEx(hdc , (int)roundToNearest(mCs.mStart.x ) ,(int)roundToNearest(mCs.mStart.y) ,    (LPPOINT) NULL);
	LineTo(  hdc , (int)roundToNearest(mCs.mEnd.x ) ,  (int)roundToNearest(mCs.mEnd.y ));

 // Rails
	if (   mPenSet.GetSolidPen(   mode&0x03 , GetBlackColor()) == NULL ) return ;
	DrawTwoParLinesExt(  p , DRAWLINES , hdc ); 

 // for SWITCH set direction or normal stright
	if ( (SWITCHOFF&mode) == 0)
	{
		if (   mPenSet.GetDottedPen(GetGrayColor()) == NULL ) return ;
		MoveToEx(hdc , (int)roundToNearest(p->StartPoint.x ) ,(int)roundToNearest(p->StartPoint.y) ,    (LPPOINT) NULL);
		LineTo(  hdc , (int)roundToNearest(p->EndPoint.x ) ,  (int)roundToNearest(p->EndPoint.y ));
	}
 
//  mark if connected to nothing from this endpoints
	if ( p->ConnID[0] == 0 ) 
		DrawPointCross( p->StartPoint,hdc,GetRedColor(),2);

	if ( p->ConnID[1] == 0 )  
		DrawPointCross( p->EndPoint,hdc,GetRedColor(),2); 
}
 

void PaintRailSwitch( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc )
{
	PaintStraightRail( p,   mode ,    hdc );
	PaintBend(  p,   mode ,    hdc );
}

void PaintSemaphore( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc )
{

	//mDebug.CDebugPut("PaintSemaphore  ");


	CPenStorage   mPenSet ;  // destruktor called outomatically
	mPenSet.SetDC( hdc);

	if (   mPenSet.GetSolidPen(   2 , GetBlackColor()  ) == NULL ) return ;
 	DrawBox(  p , DRAWLINES , hdc ); 

	COLORREF cr;
	if ( mode == 1)  cr = GetRedColor();
	else cr = GetGreenColor();

	if (   mPenSet.GetSolidPen(   8 , cr  ) == NULL ) return ;

	float dis = GetDistance(p->StartPoint , p->EndPoint  );

	CSegment mCs = ComputeSectionPependicularAtPoint( p->StartPoint , p->EndPoint ,  dis-3  , (float)4 );
	CFPoint p1 =  mCs.mStart; 
	CFPoint p2 =  mCs.mEnd;
	MoveToEx(hdc , (int)roundToNearest(p1.x ) ,(int)roundToNearest(p1.y) ,    (LPPOINT) NULL);
	LineTo(  hdc , (int)roundToNearest(p2.x  ) ,  (int)roundToNearest(p2.y ));

 

}

void ComputeBandingRectBend( DRAWDATASTRUCT *p  )
{

 	CSegment Cs2 = ComputeSectionPependicularAtPointExt( p->StartPoint,p->EndPoint,0,(float)p->Width); 
 
	int table[6];

	table[0] = (int)Cs2.mStart.x;
	table[1] = (int)Cs2.mEnd.x;
	table[2] = (int)(p->CenterPoint.x+((float)p->Radius+p->Width)*(float)cos(PI180*(p->mStartAngle+p->mSweepAngle)));
	table[3] = (int)(p->CenterPoint.x+((float)p->Radius-p->Width)*(float)cos(PI180*(p->mStartAngle+p->mSweepAngle )));
	table[4] = (int)(p->CenterPoint.x+((float)p->Radius+p->Width)*(float)cos(PI180*(p->mStartAngle+p->mSweepAngle/2)));
	table[5] = (int)(p->CenterPoint.x+((float)p->Radius-p->Width)*(float)cos(PI180*(p->mStartAngle+p->mSweepAngle/2 )));


	p->mUpdRect.left =  GetLowestValue(  table, 6 );
	p->mUpdRect.right =  GetHighestValue(  table, 6 );

	table[0] = (int)Cs2.mStart.y;
	table[1] = (int)Cs2.mEnd.y;
	table[2] = (int)(p->CenterPoint.y-((float)p->Radius+p->Width)*(float)sin(PI180*(p->mStartAngle+p->mSweepAngle)));
	table[3] = (int)(p->CenterPoint.y-((float)p->Radius-p->Width)*(float)sin(PI180*(p->mStartAngle+p->mSweepAngle)));
	table[4] = (int)(p->CenterPoint.y-((float)p->Radius+p->Width)*(float)sin(PI180*(p->mStartAngle+p->mSweepAngle/2)));
	table[5] = (int)(p->CenterPoint.y-((float)p->Radius-p->Width)*(float)sin(PI180*(p->mStartAngle+p->mSweepAngle/2)));


	p->mUpdRect.top =  GetLowestValue(  table, 6 );
	p->mUpdRect.bottom =  GetHighestValue(  table, 6 );

	p->mUpdRect.NormalizeRect();
	p->mUpdRect.InflateRect( 2  );
}

void ComputeBandingRectSwitch( DRAWDATASTRUCT *p  )
{
    float distance = GetDistance( p->StartPoint ,p->EndPoint  );
	CSegment Cs1=ComputeSectionPependicularAtPointExt(p->StartPoint,p->EndPoint,distance,(float)p->Width); 
	CSegment Cs2=ComputeSectionPependicularAtPointExt(p->EndPoint,p->StartPoint,distance,(float)p->Width); 
//	CSegment Cs2 = ComputeSectionPependicularAtPointExt( p->StartPoint,p->EndPoint,0,(float)p->Width); 
 
	int table[6];

	table[0] = (int)Cs2.mStart.x;
	table[1] = (int)Cs2.mEnd.x;
	table[2] = (int)(p->CenterPoint.x+((float)p->Radius+p->Width)*(float)cos(PI180*(p->mStartAngle+p->mSweepAngle)));
	table[3] = (int)(p->CenterPoint.x+((float)p->Radius-p->Width)*(float)cos(PI180*(p->mStartAngle+p->mSweepAngle )));
	table[4] = (int)Cs1.mStart.x;
	table[5] = (int)Cs1.mEnd.x;

	p->mUpdRect.left =  GetLowestValue(  table, 6 );
	p->mUpdRect.right =  GetHighestValue(  table, 6 );

	table[0] = (int)Cs2.mStart.y;
	table[1] = (int)Cs2.mEnd.y;
	table[2] = (int)(p->CenterPoint.y-((float)p->Radius+ p->Width)*(float)sin(PI180*(p->mStartAngle+p->mSweepAngle)));
	table[3] = (int)(p->CenterPoint.y-((float)p->Radius- p->Width)*(float)sin(PI180*(p->mStartAngle+p->mSweepAngle)));
	table[4] = (int)Cs1.mStart.y;
	table[5] = (int)Cs1.mEnd.y;

	p->mUpdRect.top =  GetLowestValue(  table, 6 );
	p->mUpdRect.bottom =  GetHighestValue(  table, 6 );

	p->mUpdRect.NormalizeRect();
	p->mUpdRect.InflateRect( 2  );
}