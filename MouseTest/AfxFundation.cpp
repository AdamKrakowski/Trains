
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

 void NormalizeRect( RECT *rc)
 {

	 if ( rc->top > rc->bottom )
	 {
		 int temp = rc->top;
		 rc->top = rc->bottom;
		 rc->bottom = temp;
	 }

	 if ( rc->left > rc->right )
	 {
		 int temp = rc->left;
		 rc->left = rc->right;
		 rc->right = temp;
	 }
 }


 void InflateRect( RECT *rc , int value)
 {
	rc->top -= value;
    rc->bottom += value;
	rc->left  -= value;
	rc->right += value; 
 }


 int PointInsideTheRect( CPoint cp , CRect rc ) 
 {
	if ( (cp.x > rc.left)  &&  ( cp.x < rc.right) )
	    if ( (cp.y > rc.top)  &&  ( cp.y < rc.bottom) )
			return 1;
	return 0;
 }

 int PointInsideTheRect( CFPoint cpx , CRect rc ) 
 {
	
	 CPointF cp = CPointF(cpx);
	 if ( (cp.x > rc.left)  &&  ( cp.x < rc.right) )
	    if ( (cp.y > rc.top)  &&  ( cp.y < rc.bottom) )
			return 1;
	return 0;
 }


CRect::CRect() 
{
	thisRect.left = 0 ;
	thisRect.top = 0;
	thisRect.right = 0;
	thisRect.bottom = 0 ;
		left= 0 ;
	top = 0;
	right = 0;
	bottom = 0 ;

}

CRect::CRect( int Left, int  Top,int Right,int Bottom ) 
{
	thisRect.left = Left ;
	thisRect.top = Top;
	thisRect.right = Right;
	thisRect.bottom = Bottom ;

	left= Left ;
	top = Top;
	right = Right;
	bottom = Bottom ;
}


CRect::CRect( CPoint p1,CPoint p2)
{
	thisRect.left = p1.x ;
	thisRect.top = p1.y;
	thisRect.right = p2.x;
	thisRect.bottom = p2.y ;

	left= thisRect.left ;
	top = thisRect.top;
	right = thisRect.right;
	bottom = thisRect.bottom ;
}

CRect::CRect( RECT rc) 
{
	thisRect.left = rc.left ;
	thisRect.top = rc.top;
	thisRect.right = rc.right;
	thisRect.bottom = rc.bottom ;

	left= thisRect.left ;
	top = thisRect.top;
	right = thisRect.right;
	bottom = thisRect.bottom ;
}

RECT CRect::GetRect() 
{
 	RECT lprc ;  
 	lprc.left =  left; 
 	lprc.top =  top;
 	lprc.right =  right;
 	lprc.bottom =  bottom;
	return  lprc;
}


void CRect::NormalizeRect()
 {
		 thisRect.left = left;
	 thisRect.top = top;
	 thisRect.right = right;
	 thisRect.bottom = bottom;	 
	 
	 ::NormalizeRect( &thisRect );

		left= thisRect.left ;
	top = thisRect.top;
	right = thisRect.right;
	bottom = thisRect.bottom;

 }

 void  CRect::InflateRect( int size )
 {
	
	 thisRect.left = left;
	 thisRect.top = top;
	 thisRect.right = right;
	 thisRect.bottom = bottom;	 
	 
	 
	 ::InflateRect( &thisRect , size );

	left= thisRect.left ;
	top = thisRect.top;
	right = thisRect.right;
	bottom = thisRect.bottom;


 }


 void  CRect::UnionRect( CRect CRect1 , CRect CRect2  )
 {
	thisRect.top = CRect1.top ;	if ( CRect2.top  < CRect1.top ) thisRect.top = CRect2.top;
	thisRect.left = CRect1.left ;	if ( CRect2.left  < CRect1.left ) thisRect.left = CRect2.left;
	thisRect.bottom = CRect1.bottom ;	if ( CRect2.bottom  > CRect1.bottom ) thisRect.bottom = CRect2.bottom;
	thisRect.right = CRect1.right ;	if ( CRect2.right  > CRect1.right ) thisRect.right = CRect2.right;

	top = CRect1.top ;	if ( CRect2.top  < CRect1.top ) top = CRect2.top;
	left = CRect1.left ;	if ( CRect2.left  < CRect1.left ) left = CRect2.left;
	bottom = CRect1.bottom ;	if ( CRect2.bottom  > CRect1.bottom ) bottom = CRect2.bottom;
	right = CRect1.right ;	if ( CRect2.right  > CRect1.right ) right = CRect2.right;



 }



 //void CRect  
 //CRect   rect1(100,   0, 200, 300);
//CRect   rect2(0, 100, 300, 200);
//CRect   rect3;

//rect3.UnionRect(&rect1, &rect2);

//CRect   rectResult(0, 0, 300, 300);
//ASSERT(rectResult == rect3);



 
CPoint::CPoint()
{
	x =0;
	y =0;
}

CPoint::CPoint(int ix, int iy)
{
	x = ix;
	y = iy;
}


// afx foundation

