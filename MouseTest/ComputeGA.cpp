
#include "stdafx.h"
#include "MouseTest.h"

#include "Windowsx.h"
#include "Math.h"
#include <stdio.h>

#include "ComputeGA.h"


#include "Debug.h"
 

extern CDebug mDebug;


 


double roundToNearest(double num) {
    return (num > 0.0) ? floor(num + 0.5) : ceil(num - 0.5);
}


int GetTheOtherPointIndex( int index  )
{
	if ( index == 0 ) return 1;
	else return 0;
}

// int GetTheOtherPointIndex( 0 ,  iret)

int GetTheOtherPointIndex( int index , int iret)
{
	if ( index == 0 ) return iret;
	else return 0;
}


int GetLowestValue( int *table, int size )
{
  int highest= 100000;
  int i;

  for ( i=0;i < size;i++)
	  if ( table[i] < highest ) highest =  table[i] ;

  return highest;
}

int GetHighestValue( int *table, int size )
{
  int lowest = -100000;
  int i;

  for ( i=0;i < size;i++)
	  if ( table[i] > lowest )   lowest = table[i] ;


  return lowest;
}


int IsPoinInsideSegment(CFPoint StartPoint , CFPoint EndPoint ,CFPoint ThePoint)
{
	float d = GetDistance(StartPoint,EndPoint);
	if ( (GetDistance(StartPoint,ThePoint)  < d ) && (GetDistance(EndPoint,ThePoint)  < d )) return 1;
	return 0;
}


int IsOnTheStrightLine( CFPoint StartPoint ,CFPoint EndPoint, CFPoint TestPoint) 
{
	float d0  = GetDistance(StartPoint , EndPoint );
	float d1  = GetDistance(StartPoint , TestPoint );
	float d2  = GetDistance(EndPoint   , TestPoint);
	if (   abs((d1+d2) - d0)  <  2.0 ) return 1;
	return 0;
}


int IsOnTheBendLine( CFPoint StartPoint ,CFPoint EndPoint, CFPoint TestPoint , float radius ,CFPoint CenterPoint)
{
	float d0  = GetDistance(StartPoint , EndPoint );
	float d1  = GetDistance(StartPoint , TestPoint );
	float d2  = GetDistance(EndPoint   , TestPoint);

	if (( d1 > d0) || (d2 > d0)) return 0;

	if ( abs(GetDistance(StartPoint , CenterPoint ) - radius ) > 2 ) return 0;

	return 1;
}

CTwoPoints  ComputeIntersectionOfTwoCircles(CCircle  c1 , CCircle c2  )
{
	CTwoPoints  tpc( CFPoint(-1,-1),CFPoint(-1,-1) );
	tpc.result = 0;

	float D = sqrt( (c1.mCenter.x-c2.mCenter.x )*(c1.mCenter.x-c2.mCenter.x ) + (c1.mCenter.y-c2.mCenter.y  )*(c1.mCenter.y-c2.mCenter.y) );

	if (  ( c1.mRadius+c2.mRadius)  < D  ) 
		{
			tpc.result = -1;
			return tpc;
		}

	if (  abs( c1.mRadius-c2.mRadius)  > D  )
		{
			tpc.result = -2;	
			return tpc;
		}

	if (  ( abs(c1.mRadius+c2.mRadius)-D) < 0.000000001  ) 
		{
			tpc.mPoint1.x = (c2.mCenter.x - c1.mCenter.x)/2;
			tpc.mPoint1.y = (c2.mCenter.y - c1.mCenter.y)/2;
			tpc.result = 1;
			return tpc;
		}

	double d1 = (double)(D+c1.mRadius+c2.mRadius);
	double d2 = (double)(D+c1.mRadius-c2.mRadius);
	double d3 = (double)(D-c1.mRadius+c2.mRadius);
	double d4 = (double)(-D+c1.mRadius+c2.mRadius);
	float delta =  (float)0.25*(float)sqrt(d1*d2*d3*d4);

	tpc.mPoint1.x =(float)0.5*(c1.mCenter.x+c2.mCenter.x )
			 + ((c2.mCenter.x-c1.mCenter.x )*(c1.mRadius*c1.mRadius-c2.mRadius*c2.mRadius ))/((float)2.0*D*D)
			 + (float)2.0*delta*( c1.mCenter.y-c2.mCenter.y )/( D*D );

	tpc.mPoint2.x  = (float)0.5*(c1.mCenter.x+c2.mCenter.x )
			 + ((c2.mCenter.x-c1.mCenter.x )*(c1.mRadius*c1.mRadius-c2.mRadius*c2.mRadius ))/((float)2.0*D*D)
			 - (float)2.0*delta*( c1.mCenter.y-c2.mCenter.y )/( D*D );

	tpc.mPoint1.y  = (float)0.5*(c1.mCenter.y+c2.mCenter.y )
			 + ((c2.mCenter.y-c1.mCenter.y )*(c1.mRadius*c1.mRadius-c2.mRadius*c2.mRadius ))/((float)2.0*D*D)
			 - (float)2.0*delta*( c1.mCenter.x-c2.mCenter.x )/( D*D );
 
	tpc.mPoint2.y  = (float)0.5*(c1.mCenter.y+c2.mCenter.y )
			 + ((c2.mCenter.y-c1.mCenter.y )*(c1.mRadius*c1.mRadius-c2.mRadius*c2.mRadius ))/((float)2.0*D*D)
			 + (float)2.0*delta*( c1.mCenter.x-c2.mCenter.x )/( D*D );

	tpc.result = 2;
	return tpc;
}


 

CTwoPoints  ComputeIntersectionOfCircleAndLine(CCircle  c1 , CLineObject l1 )
{
	CTwoPoints  tpc( CFPoint(-1,-1),CFPoint(-1,-1) );
	tpc.result = -1;
	
	if ( (int)l1.GetB() != (int)0)
		{
		float m =  (float)(-l1.GetA()/l1.GetB());
		float d =  (float)(-l1.GetC()/l1.GetB());

		float D = c1.mRadius*c1.mRadius*(1 + m*m) -  (c1.mCenter.y- m*c1.mCenter.x - d)* (c1.mCenter.y- m*c1.mCenter.x - d);

		// if D == 0  the one point
		tpc.mPoint2.x  =  ( c1.mCenter.x + c1.mCenter.y*m - d*m + sqrt(D))/(1+m*m );
		tpc.mPoint1.x  =  ( c1.mCenter.x + c1.mCenter.y*m - d*m - sqrt(D))/(1+m*m );

		tpc.mPoint1.y  =  ( d + c1.mCenter.x*m + c1.mCenter.y*m*m - m*sqrt(D))/(1+m*m );
		tpc.mPoint2.y  =  ( d + c1.mCenter.x*m + c1.mCenter.y*m*m  + m*sqrt(D))/(1+m*m );
 
		tpc.result = 2;
		if ( D == 0.0) tpc.result = 1;
	} 
	else if ( (int)l1.GetA() != (int)0)
	{
		float fA = 1 ;
		float fB = -2*c1.mCenter.y;

		float fC =	(float)(l1.GetC()*l1.GetC()/( l1.GetA()*l1.GetA()  ) 
				+ 2.0*c1.mCenter.x*l1.GetC()/l1.GetA() 
				+ c1.mCenter.x*c1.mCenter.x 
				+ c1.mCenter.y*c1.mCenter.y 
				- c1.mRadius*c1.mRadius);

		float delta =  fB*fB - 4*fA*fC;
	    if ( delta < 0 ) 
			{
				tpc.result = 0;
				return tpc ;
			}
		else
		{
			float sdelta = sqrt(delta);
			tpc.mPoint1.y =  ( -fB - sdelta)/(2*1);
			tpc.mPoint2.y =  ( -fB  + sdelta)/(2*1);
			tpc.mPoint1.x  =  (float)(-l1.GetC()/l1.GetA()) ;
			tpc.mPoint2.x  =  (float)(-l1.GetC()/l1.GetA()) ;
			tpc.result = 2;
			if ( delta == 0.0) tpc.result = 1;

		}
	}

	return tpc;
}

//
//  find point which is in given segment  -1,-1  if not
//  circle center  creates range
//  Start & End refers to reference segment
//  need to consider direction to EndPoint


CTwoPoints  ComputeIntersectionOfCircleAndSegment( CFPoint CenterPoint , float Radius , CFPoint StartPoint,CFPoint EndPoint  )
{
	CTwoPoints  tpc( CFPoint(-1,-1),CFPoint(-1,-1) );

	CLineObject l1 = CLineObject( StartPoint ,  EndPoint );
	CCircle  c1 = CCircle( CenterPoint,Radius);

	tpc = ComputeIntersectionOfCircleAndLine(   c1 ,  l1 );

	return tpc;

}

//   d = | Ax0 + By0 + C |/sqrt( A*A +B*B )

 float	GetDistanceLineToPoint( CFPoint  mstartpoint , CFPoint EndPoint  ,  CFPoint  mPoint   ) 
 {
	double d=0;

	CLineObject l1 =  CLineObject(  mstartpoint , EndPoint );

	d = abs( l1.GetA()*mPoint.x + l1.GetB()*mPoint.y + l1.GetC() )/sqrt( l1.GetA()*l1.GetA() + l1.GetB()*l1.GetB() );

	return (float)d;
 }
 

float   GetDistanceBtwTwoLines( CFPoint StartPoint1 , CFPoint EndPoint1 , CFPoint StartPoint2 , CFPoint EndPoint2 ) 
{   
 	CLineObject  mLineL1;
	CLineObject  mLineLP;
	CLineObject  mLineL2;

	mLineL1 = CLineObject(  StartPoint1 ,   EndPoint1  );
	mLineL2 = CLineObject(  StartPoint2 ,   EndPoint2 );

	mLineLP = CLineObject( mLineL1.GetA() , mLineL1.GetB(), StartPoint1 );
    CFPoint CrossPoint  =  CrossOfTwoLines (  mLineLP , mLineL2 ) ;
    return GetDistance( StartPoint1 , CrossPoint  ) ;
}

 
float   GetDistance( CFPoint StartPoint , CFPoint EndPoint  ) 
{
	float  dx = EndPoint.x-StartPoint.x;
	float  dy = EndPoint.y-StartPoint.y;
	float  d2 = dx*dx+dy*dy;
	float fl = sqrt((float)d2);
    return fl;
}
 


CFPoint   GetCenterPoint( CFPoint StartPoint , CFPoint EndPoint  )
{
	CFPoint  pt;
	pt.x = (( StartPoint.x )+( EndPoint.x ))/2;
	pt.y = (( StartPoint.y )+( EndPoint.y ))/2;
	return pt;
}


CFPoint GetPointAtTheLine (  CFPoint StartPoint ,CFPoint EndPoint , CFPoint ExtPoint )
{
	CFPoint  pt;

	CLineObject  mLineL;
	mLineL.LineFromPoints(   StartPoint ,    EndPoint );

	CLineObject mLineP =  CLineObject( mLineL.GetA() , mLineL.GetB(), ExtPoint );

	pt =  CrossOfTwoLines ( mLineL , mLineP ) ;

	return pt;
}
 


CFPoint   CrossOfTwoLines ( CLineObject  mLineMath1 , CLineObject  mLineMath2 ) 
{
	CFPoint ptf = CFPoint(0,0);
	double fw = mLineMath1.GetA()*mLineMath2.GetB()-mLineMath2.GetA()*mLineMath1.GetB();
	if ( (int)fw != 0  )  // lines are ! paraller
	{
		double fwx,fwy;
		fwx = -mLineMath1.GetC()*mLineMath2.GetB() - (-mLineMath2.GetC()  )*mLineMath1.GetB();
		fwy = -mLineMath2.GetC()*mLineMath1.GetA() - (-mLineMath1.GetC()  )*mLineMath2.GetA();
		ptf.x = (float) (fwx/fw ) ;
 		ptf.y = (float) (fwy/fw ) ;
	}
	return ptf;
}

CFPoint fCrossOfTwoVectors ( CVector vFirst ,CVector vSecond )
{
	CFPoint ptf = CFPoint(0,0);

	CLineObject  mLineMath1;
	CLineObject  mLineMath2;

	mLineMath1.LineFromPoints(   vFirst.mStart ,    vFirst.mEnd);
	mLineMath2.LineFromPoints(   vSecond.mStart ,   vSecond.mEnd);

	double fw = mLineMath1.GetA()*mLineMath2.GetB()-mLineMath2.GetA()*mLineMath1.GetB();
	if ( (int)fw != 0  )  //  if zero  -- vectors are paraller
	{
		double fwx,fwy;
		fwx = -mLineMath1.GetC()*mLineMath2.GetB() - (-mLineMath2.GetC()  )*mLineMath1.GetB();
		fwy = -mLineMath2.GetC()*mLineMath1.GetA() - (-mLineMath1.GetC()  )*mLineMath2.GetA();
		ptf.x = (float) (fwx/fw ) ;
 		ptf.y = (float) (fwy/fw ) ;
	}
	return ptf;
}


CFPoint fCrossOfTwoVectors ( CFVector vFirst ,CFVector vSecond )
{
	CFPoint ptf = CFPoint(0,0);

	CLineObject  mLineMath1;
	CLineObject  mLineMath2;

	mLineMath1.LineFromPoints(   vFirst.mStart ,    vFirst.mEnd);
	mLineMath2.LineFromPoints(   vSecond.mStart ,   vSecond.mEnd);

	double fw = mLineMath1.GetA()*mLineMath2.GetB()-mLineMath2.GetA()*mLineMath1.GetB();
	if ( (int)fw != 0  )  //  if zero  -- vectors are paraller
	{
		double fwx,fwy;
		fwx = -mLineMath1.GetC()*mLineMath2.GetB() - (-mLineMath2.GetC()  )*mLineMath1.GetB();
		fwy = -mLineMath2.GetC()*mLineMath1.GetA() - (-mLineMath1.GetC()  )*mLineMath2.GetA();
		ptf.x = (float) (fwx/fw ) ;
 		ptf.y = (float) (fwy/fw ) ;
	}
	return ptf;
}



/*    Obsolite funtion repalaced by ext
CFPoint ComputePointFromLinePointOrigin ( CLineObject mLine , CFPoint  origin , float displacement ) 
{

	CFPoint pt = CFPoint(0,0) ;

	if ( (int)mLine.GetB() != 0  )
	{
		pt.x  = origin.x+displacement;
		pt.y = (float)( ( - mLine.GetA()*( origin.x + displacement ) - mLine.GetC()  )/mLine.GetB()) ;

	}
	else if  ( (int)mLine.GetA() != 0  )
	{
		pt.y  = origin.y+displacement;
		pt.x = (float)( ( - mLine.GetB()*( origin.y + displacement ) - mLine.GetC()  )/mLine.GetA()) ;
	}
	return pt;
}
*/
// fixed  distance 
///  compute both  crossing point equally placed on both side of the center point on the line
//

CFPoint   ComputePointFromLinePointOriginExt ( CLineObject mLine  , CFPoint  CenterPoint , float  fRadius , int idir )
{
	CFPoint cf= CFPoint(0,0);
	float fA,fB,fC,delta;
	float lA=(float)mLine.GetA(),lB=(float)mLine.GetB(),lC=(float)mLine.GetC();
	float lA2 = lA*lA,lB2 = lB*lB,lC2 = lC*lC;;

	if ( (int)lA == 0  )
	{
	   if ( (int)lB != 0  )	
	   {
		fC = -fRadius*fRadius + CenterPoint.y*CenterPoint.y + CenterPoint.x*CenterPoint.x + (float)2.0*CenterPoint.y*lC/lB + lC2/lB2;
		fB = (float)2.0* ( lC*lA/lB2 + lA*CenterPoint.y/lB - CenterPoint.x ); 
		fA = lA2/lB2 +1; 
		delta = fB*fB - (float)4.0*fA*fC; if ( delta <= 0 ) return CFPoint(0,0);
		if ( idir == 0) { cf.x = (-fB - sqrt ( delta))/(2*fA) ; cf.y = -(lC + lA*cf.x)/lB; }
		else { cf.x = (-fB +sqrt ( delta))/(2*fA) ; cf.y = -( lC + lA*cf.x)/lB; }
	   }
	   else   return cf;
	}
	else
	{
		fC =  -fRadius*fRadius +CenterPoint.y*CenterPoint.y + CenterPoint.x*CenterPoint.x + (float)2.0*CenterPoint.x*lC/lA+lC2/lA2;
		fB =  (float)2.0* ( lC*lB/lA2 + lB*CenterPoint.x/lA - CenterPoint.y) ; 
		fA = lB2/lA2 +1; 
		delta = fB*fB - (float)4.0*fA*fC;  if ( delta <= 0 ) return cf;
		if ( idir == 0)	{ cf.y = (-fB - sqrt ( delta))/(2*fA)  ;cf.x = -( lC + lB*cf.y)/lA; }
		else	{ cf.y = (-fB +sqrt ( delta))/(2*fA) ;	cf.x = -( lC + lB*cf.y)/lA; }
	}
	return cf;
}


float  GetAbsAngleOfTwoVectors ( float a1, float a2 )
{
	float angle =  0.0;
	float angle1=0,angle2=0;

	if ( a1 > a2 )
	{
		angle1 = a1;
		angle2 = a2;
	}
	else
		if ( a2 > a1 )
		{
		angle2 = a1;
		angle1 = a2;
		}

    angle = angle1 - angle2;
	if ( angle > 180 ) angle = 360 - angle;
    return angle;
}


float  GetAbsAngleOfTwoVectors ( CVector vFirst ,CVector vSecond ) 
{
	float a1 =	ComputeVectorAngle( vFirst.mStart  ,  vFirst.mEnd ) ;
	float a2 =	ComputeVectorAngle( vSecond.mStart , vSecond.mEnd ) ;
	return  GetAbsAngleOfTwoVectors (   a1,   a2 );
}

float  GetAbsAngleOfTwoVectors ( CFPoint StartPoint1 ,CFPoint EndPoint1  ,CFPoint StartPoint2 ,CFPoint EndPoint2 )
{
	float a1 =	ComputeVectorAngle( StartPoint1 , EndPoint1 ) ;
	float a2 =	ComputeVectorAngle( StartPoint2 , EndPoint2 ) ;
	return  GetAbsAngleOfTwoVectors (   a1,   a2 );
}

 CFPoint ComputeBendExtendedPoint ( CFPoint  CenterPoint , CFPoint  MidleEndPoint , CFPoint OtherEndPoint)
 {
	CLineObject mLineL =  CLineObject ( CenterPoint , MidleEndPoint  );
	CLineObject mLineP =  CLineObject( mLineL.GetA() , mLineL.GetB(), MidleEndPoint );
	//CFPoint cf = ComputePointFromLinePointOrigin ( mLineP , MidleEndPoint , 100 );
	
	CFPoint cf = ComputePointFromLinePointOriginExt( mLineP , MidleEndPoint , 100 ,0 );


	float a1 = ComputeVectorAngle( MidleEndPoint , OtherEndPoint ) ;
	float a2 = ComputeVectorAngle( MidleEndPoint , cf  ) ;
	float aa =  GetAbsAngleOfTwoVectors (   a1,   a2 );

	if (  aa  > 90 )  
			CFPoint cf = ComputePointFromLinePointOriginExt( mLineP , MidleEndPoint , 100 ,1);

		//cf = ComputePointFromLinePointOrigin ( mLineP , MidleEndPoint , -100 );
	return cf;
 }

 //  start point is where :
//  perpendicular line is created
//  two solutions for center point are evaluated and the one with closer angle to ExtPoint is selected as center
//  Turn has only symbolic meaning for matching it with ExtPoint
//  consider passing the parameters as structure !

CFPoint ComputeCenterPoint( CFPoint   StartPoint ,CFPoint   EndPoint , CFPoint   ExtPoint , float iRadius   )  
{
	int lTurn = 0;
	CLineObject mLineL =  CLineObject ( StartPoint , EndPoint );
	CLineObject mLineP =  CLineObject( mLineL.GetA() , mLineL.GetB(), StartPoint );
	CFPoint pt  = ComputePointFromLinePointOriginExt ( mLineP  , StartPoint , iRadius , lTurn );
	if ( lTurn  == 0 ) lTurn =1; else lTurn =0; // swap it if no match in the first place
	if (  GetAbsAngleOfTwoVectors ( ComputeVectorAngle( StartPoint , ExtPoint ),ComputeVectorAngle(StartPoint,pt)) > 90 )
						pt  = ComputePointFromLinePointOriginExt ( mLineP , StartPoint , iRadius , lTurn );
	return pt;
}


CFPoint ComputeExtendedLinePoint( CFPoint CenterPoint ,CFPoint StartPoint , CFPoint EndPoint )
{
  int lTurn = 0;
 
  CLineObject mLineL =  CLineObject ( CenterPoint , EndPoint );   // create line from center point to exp point
  CLineObject mLineP =  CLineObject( mLineL.GetA() , mLineL.GetB(), EndPoint );  // perpendicular line

  CFPoint pt  = ComputePointFromLinePointOriginExt ( mLineP  ,EndPoint , 100 , lTurn );

  if ( lTurn  == 0 ) lTurn =1; else lTurn =0; // swap it if no match in the first place
  
  if (  GetAbsAngleOfTwoVectors ( ComputeVectorAngle( StartPoint , EndPoint ),ComputeVectorAngle( EndPoint ,pt)) > 90 )
						pt  = ComputePointFromLinePointOriginExt ( mLineP , EndPoint , 100 , lTurn );
 
  //p->SetDebugPoint(0, EndPoint);  
  //p->SetDebugPoint(1, pt ); 
  //p->SetDebugPoint(2, CenterPoint ); 
  return pt;
}

 
int ComputeBendToLink1( CFPoint StartPoint1 ,CFPoint EndPoint1  ,CFPoint StartPoint2 ,CFPoint EndPoint2 ,DRAWDATASTRUCT  *ps)
{
	int iresult =0;
	CFVector vFirst  =  CFVector(StartPoint1,EndPoint1 ); 
	CFVector vSecond =  CFVector(StartPoint2,EndPoint2 ); 		
	CFPoint ptf = fCrossOfTwoVectors (  vFirst ,  vSecond );

	CFPoint ptf1 =   EndPoint1   ;
	CFPoint ptf2 =  EndPoint2   ;
    float dist1 = GetDistance( ptf  , ptf1 ) ;
    float dist2 = GetDistance( ptf  , ptf2 ) ;
	float	fRadius =0;
	CFPoint fBendCenterPoint;
	CFPoint vp1,vp2;  // virtual point & real one
	vp1 = ptf1;
	vp2 = ptf2; 

	if ( dist1 > dist2 ) // select greater distance from cross
	{
			// mDebug.CDebugPut("Link1:(1)>(2) PREL need to extend 1  " );
		vp2 = ptf2; // V1-->  needs to be extended V2 to keep  
		vp1 =  GetPointFmStart(CFPoint(StartPoint1),ptf1,GetDistance(CFPoint(StartPoint1),CFPoint(EndPoint1)) +dist1-dist2 );
		// mDebug.CDebugPut("Link1:Vp1:%d,%d  vp2:%d,%d ", vp1.x, vp1.y, vp2.x, vp2.y );
		CLineObject mLineL1 =  CLineObject (StartPoint1,EndPoint1);
		CLineObject mLineP1 =  CLineObject( mLineL1.GetA(),mLineL1.GetB(),vp1);
		CLineObject mLineL2 =  CLineObject (StartPoint2,EndPoint2);
		CLineObject mLineP2 =  CLineObject( mLineL2.GetA(),mLineL2.GetB(),vp2);	
		fBendCenterPoint =  CrossOfTwoLines (  mLineP1 ,  mLineP2 );
		iresult = 1;
	}
	else
	{
				//	 mDebug.CDebugPut("Link1:(1)<(2) PREL need to extend 2  " );
		vp1 = ptf1; // V2-->  needs to be extended V1 to keep  
		vp2 =  GetPointFmStart(CFPoint(StartPoint2),ptf2,GetDistance(CFPoint( StartPoint2),CFPoint(EndPoint2))+dist2 -dist1);

	// mDebug.CDebugPut("Link1:Vp1:5d,%d  vp2:%d,%d ", vp1.x, vp1.y, vp2.x, vp2.y );
		
		CLineObject mLineL1 =  CLineObject (StartPoint1,EndPoint1);
		CLineObject mLineP1 =  CLineObject( mLineL1.GetA(),mLineL1.GetB(),vp1);
		CLineObject mLineL2 =  CLineObject (StartPoint2,EndPoint2);
		CLineObject mLineP2 =  CLineObject( mLineL2.GetA(),mLineL2.GetB(),vp2);
		fBendCenterPoint =  CrossOfTwoLines (  mLineP1 ,  mLineP2 );
		iresult = 2;
	}

	fRadius = GetDistance( fBendCenterPoint ,  vp1 ) ; // caoul be also vp2 
	float angle1 = ComputeVectorAngle( fBendCenterPoint , vp1  ) ;
	float angle2 = ComputeVectorAngle( fBendCenterPoint , vp2  ) ;
	ps->mStartAngle = angle1;
	ps->mSweepAngle = angle2;
	ps->Radius =  fRadius;
	ps->CenterPoint.x = fBendCenterPoint.x;
	ps->CenterPoint.y = fBendCenterPoint.y;
	ps->StartPoint.x = vp1.x;
	ps->StartPoint.y = vp1.y;
	ps->EndPoint.x = vp2.x;
	ps->EndPoint.y = vp2.y;

	return iresult;
}

//   Shring  closer vector
//   return 0 failure  1 vector shrank 2 vector shrank
//


int ComputeBendToLink2( CFPoint StartPoint1 ,CFPoint EndPoint1  ,CFPoint StartPoint2 ,CFPoint EndPoint2 ,DRAWDATASTRUCT  *ps)
{
	int iresult=0; 


	CVector vFirst  =  CVector(StartPoint1,EndPoint1 ); 
	CVector vSecond =  CVector(StartPoint2,EndPoint2 ); 


	CFPoint ptf = fCrossOfTwoVectors (  vFirst ,  vSecond );
	CFPoint ptf1 = CFPoint( EndPoint1  );
	CFPoint ptf2 = CFPoint( EndPoint2  );

    float dist1 = GetDistance( ptf  , ptf1 ) ;
    float dist2 = GetDistance( ptf  , ptf2 ) ;
	float fRadius =0;
	CFPoint fBendCenterPoint;
	CFPoint vp1,vp2;
	vp1 = ptf1;
	vp2 = ptf2; 

	//	mDebug.CDebugPut("Computing " );
	if ( dist1 <= dist2 ) // potential error if equal ? or preinitialize vps
	{
		//mDebug.CDebugPut("(1)  < (2)  need to shorte 1  by:%6.2f %6.2f", -( dist2 - dist1) + fGetDistance( CFPoint( StartPoint1),CFPoint(EndPoint1) )  , 0.0 );
		//mDebug.CDebugPut("Radius:%d CenterPoint:{%d,%d} ", dat.Radius,dat.CenterPoint.x, dat.CenterPoint.y);
		//mDebug.CDebugPut("Angle1:%6.2f Angle2:%6.2f ", dat.mStartAngle,  dat.mSweepAngle );
		vp2 = ptf2;// V1-->  needs to be shrinked  V2 to keep 

		float fshort = -( dist2 - dist1) + GetDistance( CFPoint( StartPoint1),CFPoint(EndPoint1) );
		// cross point
		CLineObject mLineL1 =  CLineObject (StartPoint1,EndPoint1);
		CLineObject mLineL2 =  CLineObject (StartPoint2,EndPoint2);
		CFPoint fBendCrossPoint =  CrossOfTwoLines (  mLineL1 ,  mLineL2 );

	// mDebug.CDebugPut("(1)<(2) PREL need to shorter 1  by:%6.2f %6.2f", fshort    , 0.0 );

		vp1 =  GetPointFmStart( CFPoint( StartPoint1 ),CFPoint( EndPoint1 ) , fshort  );
		 
		if ( GetDistance( fBendCrossPoint , vp1 )   < GetDistance( vp1 , EndPoint1 ) )
			{
			fshort =   GetDistance( CFPoint( StartPoint1),CFPoint(EndPoint1)) - dist2 - GetDistance( fBendCrossPoint , CFPoint( EndPoint1 ) ) ;
		//	mDebug.CDebugPut("(1)<(2) MORE need to shorter 1  by:%6.2f %6.2f", fshort   , 0.0 );
			if ( fshort < 0) return 0;
			vp1 =  GetPointFmStart( CFPoint( StartPoint1 ),CFPoint( EndPoint1 ) , fshort );
			}

		CLineObject mLineP1 =  CLineObject( mLineL1.GetA(),mLineL1.GetB(),vp1);
		CLineObject mLineP2 =  CLineObject( mLineL2.GetA(),mLineL2.GetB(),vp2);	
		fBendCenterPoint =  CrossOfTwoLines (  mLineP1 ,  mLineP2 );
		iresult = 1; 
	}
	else
	{

//mDebug.CDebugPut("Case 2:(2)  > (1)  need to shorten 2  by:%6.2f %6.2f", -( dist1 - dist2) + fGetDistance( CFPoint( StartPoint2),CFPoint(EndPoint2) )  , 0.0 );

		float fshort = -(dist1 - dist2) + GetDistance( CFPoint( StartPoint2),CFPoint(EndPoint2));

		vp1 = ptf1; // V2 needs to be shrinked V1 to keep
		vp2 =  GetPointFmStart(CFPoint( StartPoint2),CFPoint(EndPoint2), fshort);

		CLineObject mLineL1 =  CLineObject (StartPoint1,EndPoint1);
		CLineObject mLineL2 =  CLineObject (StartPoint2,EndPoint2);
		CFPoint fBendCrossPoint =  CrossOfTwoLines (  mLineL1 ,  mLineL2 );

 		if ( GetDistance( fBendCrossPoint , vp2 )   < GetDistance( vp2 , EndPoint2) )
			{
			fshort =   GetDistance( CFPoint( StartPoint2),CFPoint(EndPoint2)) - dist1 - GetDistance( fBendCrossPoint , CFPoint( EndPoint2 ) ) ;
			if ( fshort < 0) return 0; // cannot exeed start point 
				//	mDebug.CDebugPut("(2)<(1) MORE need to shorter 1  by:%6.2f %6.2f", fshort   , 0.0 );
			vp2 = GetPointFmStart( CFPoint( StartPoint2 ),CFPoint( EndPoint2 ) , fshort );
			}

		CLineObject mLineP1 =  CLineObject( mLineL1.GetA(),mLineL1.GetB(),vp1);
		CLineObject mLineP2 =  CLineObject( mLineL2.GetA(),mLineL2.GetB(),vp2);
		fBendCenterPoint =  CrossOfTwoLines (  mLineP1 ,  mLineP2 );
		iresult	=	2; 
	}

	fRadius = GetDistance( fBendCenterPoint ,  vp1 ) ; // caoul be also vp2 
//mDebug.CDebugPut("fRadius  by:%6.2f %6.2f", fRadius  , 0.0 );
	float angle1 = ComputeVectorAngle( fBendCenterPoint , vp1  ) ;
	float angle2 = ComputeVectorAngle( fBendCenterPoint , vp2  ) ;
	ps->mStartAngle = angle1;
	ps->mSweepAngle = angle2;
	ps->Radius =  fRadius;
	ps->CenterPoint.x = fBendCenterPoint.x;
	ps->CenterPoint.y = fBendCenterPoint.y;
 	ps->StartPoint.x = vp1.x;
	ps->StartPoint.y = vp1.y;
 	ps->EndPoint.x	= vp2.x;
 	ps->EndPoint.y	= vp2.y;

	return iresult;
}



float  ComputeVectorAngleIn( CFPoint startPoint , CFPoint endPoint ) 
{
 if (( endPoint.x >  startPoint.x ) && ( endPoint.y  <= startPoint.y ))
  return (float)( C180PI*atan((double)((double)( startPoint.y-endPoint.y))/((double)(endPoint.x-startPoint.x)))  );
 
 if (( endPoint.x <=  startPoint.x ) && ( endPoint.y  <  startPoint.y ))
  return (float)(90.0+C180PI*atan((double)((double)(startPoint.x-endPoint.x))/((double)(startPoint.y-endPoint.y))) );

 if (( endPoint.x <  startPoint.x ) && ( endPoint.y  >= startPoint.y ))
  return (float)(180.0+C180PI*atan((double)((double)(endPoint.y-startPoint.y))/((double)(startPoint.x-endPoint.x))) );

 if (( endPoint.x >=  startPoint.x ) && ( endPoint.y  >  startPoint.y ))
  return (float)(270.0+C180PI*atan((double)((double)(endPoint.x-startPoint.x))/((double)(endPoint.y-startPoint.y))) );

 return 0.0;
}


float  ComputeVectorAngle( CFPoint startPoint , CFPoint endPoint ) 
{
  float angle = ComputeVectorAngleIn(  startPoint ,   endPoint ) ;
  if ( angle >= 360 ) angle -=360;
  return angle;
}


float ComputeVectorAngle( CPoint startPoint , CPoint endPoint ) 
{
 if (( endPoint.x >  startPoint.x ) && ( endPoint.y  <= startPoint.y ))
  return (float)( C180PI*atan((double)((double)( startPoint.y-endPoint.y))/((double)(endPoint.x-startPoint.x)))  );
 
 if (( endPoint.x <=  startPoint.x ) && ( endPoint.y  <  startPoint.y ))
  return (float)(90.0+C180PI*atan((double)((double)(startPoint.x-endPoint.x))/((double)(startPoint.y-endPoint.y))) );

 if (( endPoint.x <  startPoint.x ) && ( endPoint.y  >= startPoint.y ))
  return (float)(180.0+C180PI*atan((double)((double)(endPoint.y-startPoint.y))/((double)(startPoint.x-endPoint.x))) );

 if (( endPoint.x >=  startPoint.x ) && ( endPoint.y  >  startPoint.y ))
  return (float)(270.0+C180PI*atan((double)((double)(endPoint.x-startPoint.x))/((double)(endPoint.y-startPoint.y))) );

 return 0.0;
}


int  GetDistance( POINT StartPoint , POINT EndPoint  ) 
{
 
	int  dx = EndPoint.x-StartPoint.x;
	int  dy = EndPoint.y-StartPoint.y;
	int  d2 = dx*dx+dy*dy;
	float fl = sqrt((float)d2);
	d2 =(int)( roundToNearest (fl));
    return d2;
}

int  GetDistance( CPoint StartPoint , CPoint EndPoint  ) 
{
 
	int  dx = EndPoint.x-StartPoint.x;
	int  dy = EndPoint.y-StartPoint.y;
	int  d2 = dx*dx+dy*dy;
	float fl = sqrt((float)d2);
	d2 =(int)( roundToNearest (fl));
    return d2;
}

CPoint  GetCenterPoint( CPoint StartPoint , CPoint EndPoint  )
{
	CPoint  pt;

	pt.x = (( StartPoint.x )+( EndPoint.x )) >> 1;
	pt.y = (( StartPoint.y )+( EndPoint.y )) >> 1;

	return pt;
}

 

CPoint GetPointFmStart(  CPoint StartPoint, CPoint EndPoint, int distance )
{
	CPoint  ExtPoint;

	float	fdx = (float)(EndPoint.x-StartPoint.x);
	float	fdy = (float)(EndPoint.y-StartPoint.y);
	float	fl = sqrt((float)(fdx*fdx+fdy*fdy ) );  

	float	fx = ((float)(distance))*fdx/fl;
	float	fy = ((float)(distance))*fdy/fl;

	fx = (float)roundToNearest (fx) ;
	fy = (float)roundToNearest (fy) ;

	ExtPoint.x =  StartPoint.x +(int)fx;
	ExtPoint.y =  StartPoint.y +(int)fy;

	return ExtPoint;
}


CFPoint  GetPointFmStart(  CFPoint StartPoint,CFPoint EndPoint, float distance )
{
	CFPoint ExtPoint;
	float	fdx = EndPoint.x-StartPoint.x;
	float	fdy = EndPoint.y-StartPoint.y;
	float	fl = sqrt(fdx*fdx+fdy*fdy ) ;  
	float	fx = distance*fdx/fl;
	float	fy = distance*fdy/fl;
	ExtPoint.x =  StartPoint.x + fx;
	ExtPoint.y =  StartPoint.y + fy;
	return ExtPoint;
}

//  need to convert to angle  first then  set properly on the bend
//		----------------
//	   |				|
//   [0]				[1]
//
//
//
//

 

CFPoint GetPointFmStartBend(  CFPoint StartPoint,CFPoint EndPoint,DRAWDATASTRUCT *pdd ,float distance )
{
	CFPoint ExtPoint;

	float dti =  distance*((float)C180PI)/pdd->Radius;  // from distance to angle
    // what is our start point ? Are we going from real start or opposite ?
	int startpoint ;

	if (  (  ((int)StartPoint.x) ==  ((int)pdd->StartPoint.x) ) && (  ((int)StartPoint.y) ==  ((int)pdd->StartPoint.y) ))
		startpoint = 1; // yes
	else 
		startpoint = 0; // must be other point
 
	float angle;

	// mDebug.CDebugPut("GBFS startpoint:%d  Turn: %d  ", startpoint,  pdd->Turn  );

	if ( startpoint == 1 )
	{	
		if ( pdd->Turn == 1 ) dti*=-1;
		angle = pdd->mStartAngle + dti ;
	}
	else
	{
		if ( pdd->Turn == 0 ) dti*=-1;
		angle = pdd->mStartAngle + pdd->mSweepAngle + dti ;
	}


 //mDebug.CDebugPut("GBFS start angle:%6.2f  sweep:%6.2f   ", pdd->mStartAngle , pdd->mSweepAngle );
 //mDebug.CDebugPut("GBFS angle:%6.2f  dti:%6.2f   ", angle , dti );


	ExtPoint.x = pdd->CenterPoint.x +  (float)pdd->Radius*(float)cos(  PI180*(angle) );
	ExtPoint.y = pdd->CenterPoint.y -  (float)pdd->Radius*(float)sin(  PI180*(angle) );

	return ExtPoint;
}
 
CFPoint GetPointFmStart(  CFPoint StartPoint,CFPoint EndPoint,float distance ,DRAWDATASTRUCT *pdd )
{
	CFPoint ExtPoint(0,0);

	float dti =  distance*((float)C180PI)/pdd->Radius;  // from distance to angle
    // what is our start point ? Are we going from real start or opposite ?
	int startpoint ;

	if (  (  ((int)StartPoint.x) ==  ((int)pdd->StartPoint.x) ) && (  ((int)StartPoint.y) ==  ((int)pdd->StartPoint.y) ))
		startpoint = 1; // yes
	else 
		startpoint = 0; // must be other point
 
	float angle;

	// mDebug.CDebugPut("GBFS startpoint:%d  Turn: %d  ", startpoint,  pdd->Turn  );

	if ( startpoint == 1 )
	{	
		if ( pdd->Turn == 1 ) dti*=-1;
		angle = pdd->mStartAngle + dti ;
	}
	else
	{
		if ( pdd->Turn == 0 ) dti*=-1;
		angle = pdd->mStartAngle + pdd->mSweepAngle + dti ;
	}


 //mDebug.CDebugPut("GBFS start angle:%6.2f  sweep:%6.2f   ", pdd->mStartAngle , pdd->mSweepAngle );
 //mDebug.CDebugPut("GBFS angle:%6.2f  dti:%6.2f   ", angle , dti );


	ExtPoint.x = pdd->CenterPoint.x +  (float)pdd->Radius*(float)cos(  PI180*(angle) );
	ExtPoint.y = pdd->CenterPoint.y -  (float)pdd->Radius*(float)sin(  PI180*(angle) );

   return ExtPoint;
}

/*
float  GetDistanceAngle( CFPoint  mstartpoint , DRAWDATASTRUCT *pdd , CFPoint  mpoint ) 
{

	float a1 =	ComputeVectorAngle( pdd->CenterPoint , mstartpoint ) ;
	float a2 =	ComputeVectorAngle( pdd->CenterPoint , mpoint ) ;
	return abs( a1-a2);

}
*/

float	GetDistance( CFPoint  mStartpoint , CFPoint mEndPoint  , DRAWDATASTRUCT *pdd  ) 
{
	// convert to abgle 
	
	float a =  GetAbsAngleOfTwoVectors ( pdd->CenterPoint  ,mStartpoint   ,pdd->CenterPoint  ,mEndPoint );
	 
//	float a1 =	ComputeVectorAngle( pdd->CenterPoint , mStartpoint ) ;
//	float a2 =	ComputeVectorAngle( pdd->CenterPoint , mEndPoint ) ;
/*
	mDebug.CDebugPut("GetDistance:-------------------- "  );
	mDebug.CDebugPut("GetDistance: stp %6.2f , %6.2f   ", mStartpoint.x, mStartpoint.y  );
	mDebug.CDebugPut("GetDistance: enp %6.2f , %6.2f   ", mEndPoint.x, mEndPoint.y  );	
	mDebug.CDebugPut("GetDistance: a1:%6.2f a2:%6.2f   ", a1 , a2  );
	mDebug.CDebugPut("GetDistance: abs:%6.2f  :%6.2f   ", a , (float)0  );
	mDebug.CDebugPut("GetDistance:-------------------- "  );
	 */
	float perimeter = ((float)PI180)*pdd->Radius*a;  // Pi/180
	return perimeter;
}


float LtoAngle(float distance , DRAWDATASTRUCT *pdd  ) 
{
	return  distance*((float)C180PI)/pdd->Radius;   
}
// create perpendicular section at pPoint lengh of lenght
//           *        * S,E  
//           |
//        X--+--X    + PPoint disp
//           | 
//           |
//           |
//           *




/*
RECT GetPerpendicularSectionAt(CPoint SPoint,CPoint EPoint,CPoint PPoint,int disp)
{
	
	
	 
	RECT  rect;

	int  dx = EPoint.x-SPoint.x;
	int  dy = EPoint.y-SPoint.y;
	float fl = sqrt((float)(dx*dx+dy*dy));
	float fdisp =(float) disp;

 	int dxx = (int )( roundToNearest (fdisp * ((float)dy)/fl));
    int dyy = (int )( roundToNearest (fdisp * ((float)dx)/fl));

	rect.left	= PPoint.x + dxx;
	rect.top	= PPoint.y - dyy;
	rect.right	= PPoint.x - dxx;
	rect.bottom = PPoint.y + dyy;

	return rect;
}
*/


CSegment ComputeSectionPependicularAtPoint( CFPoint StartPoint ,CFPoint EndPoint ,float distance,float lenght)
{
	CSegment mSection= CSegment();
	// get the extednded point
	CFPoint ExtPoint;
	if ( distance != 0 ) // the distance cannot be zero
	{
	 ExtPoint = GetPointFmStart(    StartPoint,  EndPoint,   distance );
	// use old obsolite function
	CFPoint Pt1 = ComputeTwoParLines( ExtPoint ,StartPoint , lenght , 0);
	CFPoint Pt2 = ComputeTwoParLines( ExtPoint ,StartPoint , lenght , 1);
	mSection= CSegment(Pt1,Pt2);
	}
	return mSection;
}
 
CSegment ComputeSectionPependicularAtPointExt( CFPoint StartPoint ,CFPoint EndPoint ,float offset,float lenght)
{
	CSegment mSection= CSegment();

	CLineObject   mLine = CLineObject (  StartPoint ,  EndPoint); // generate the line 
 
	CFPoint ExtPoint;
	if ( offset != 0 )
		ExtPoint = GetPointFmStart(  StartPoint,  EndPoint,   offset );
	 else 
		ExtPoint = StartPoint;

	CLineObject   mLineP =   CLineObject( mLine.GetA() ,mLine.GetB()  ,ExtPoint)  ;        // perpendicular at the point

	CFPoint Pt1 =  ComputePointFromLinePointOriginExt (  mLineP  , ExtPoint, lenght   , 0 );
	CFPoint Pt2 =  ComputePointFromLinePointOriginExt (  mLineP  , ExtPoint, lenght   , 1 );
 
	mSection= CSegment(Pt1,Pt2);

	return mSection;
}




CFPoint  ComputeTwoParLines( CFPoint SPoint ,CFPoint EPoint , int disp , int point_index ) 
{
	float  dx = EPoint.x-SPoint.x;
	float  dy = EPoint.y-SPoint.y;

	float fl = sqrt((float)(dx*dx+dy*dy));
	float fdisp =(float) disp;

	float dxx =    (fdisp * ((float)dy)/fl);
    float dyy =    (fdisp * ((float)dx)/fl);
	
	CFPoint SPoint0; 
	CFPoint SPoint1;
	CFPoint EPoint0; 
	CFPoint EPoint1;

	if ( point_index == 0 )
	{
	SPoint0.x = SPoint.x + dxx;
	SPoint0.y = SPoint.y - dyy;
	return SPoint0;
	}
	else if ( point_index == 1 )
	{
	SPoint1.x = SPoint.x - dxx;
	SPoint1.y = SPoint.y + dyy;
	return SPoint1;
	}
	else if ( point_index == 2 )
	{
	EPoint0.x = EPoint.x + dxx;
	EPoint0.y = EPoint.y - dyy;
	return EPoint0;
	}
 	else if ( point_index == 3 )
	{
	EPoint1.x = EPoint.x - dxx;
	EPoint1.y = EPoint.y + dyy;
	return EPoint1;
	}

	EPoint1.x = 0;
	EPoint1.y = 0;
    return EPoint1;
}

CFPoint  ComputeTwoParLines( CFPoint SPoint ,CFPoint EPoint , float disp , int point_index ) 
{
	float  dx = EPoint.x-SPoint.x;
	float  dy = EPoint.y-SPoint.y;

	float fl = sqrt((float)(dx*dx+dy*dy));
	float fdisp =(float) disp;

	float dxx =    (fdisp * ((float)dy)/fl);
    float dyy =    (fdisp * ((float)dx)/fl);
	
	CFPoint SPoint0; 
	CFPoint SPoint1;
	CFPoint EPoint0; 
	CFPoint EPoint1;

	if ( point_index == 0 )
	{
	SPoint0.x = SPoint.x + dxx;
	SPoint0.y = SPoint.y - dyy;
	return SPoint0;
	}
	else if ( point_index == 1 )
	{
	SPoint1.x = SPoint.x - dxx;
	SPoint1.y = SPoint.y + dyy;
	return SPoint1;
	}
	else if ( point_index == 2 )
	{
	EPoint0.x = EPoint.x + dxx;
	EPoint0.y = EPoint.y - dyy;
	return EPoint0;
	}
 	else if ( point_index == 3 )
	{
	EPoint1.x = EPoint.x - dxx;
	EPoint1.y = EPoint.y + dyy;
	return EPoint1;
	}

	EPoint1.x = 0;
	EPoint1.y = 0;
    return EPoint1;
}


CPoint ComputeTwoParLines( CPoint SPoint ,CPoint EPoint , int disp , int point_index ) 
{

	int  dx = EPoint.x-SPoint.x;
	int  dy = EPoint.y-SPoint.y;

	float fl = sqrt((float)(dx*dx+dy*dy));
	float fdisp =(float) disp;

 
	int dxx = (int )roundToNearest(   (fdisp * ((float)dy)/fl));
    int dyy = (int )roundToNearest(   (fdisp * ((float)dx)/fl));
	
	CPoint SPoint0; 
	CPoint SPoint1;
	CPoint EPoint0; 
	CPoint EPoint1;

	if ( point_index == 0 )
	{
	SPoint0.x = SPoint.x + dxx;
	SPoint0.y = SPoint.y - dyy;
	return SPoint0;
	}
	else if ( point_index == 1 )
	{
	SPoint1.x = SPoint.x - dxx;
	SPoint1.y = SPoint.y + dyy;
	return SPoint1;
	}
	else if ( point_index == 2 )
	{
	EPoint0.x = EPoint.x + dxx;
	EPoint0.y = EPoint.y - dyy;
	return EPoint0;
	}
 	else if ( point_index == 3 )
	{
	EPoint1.x = EPoint.x - dxx;
	EPoint1.y = EPoint.y + dyy;
	return EPoint1;
	}

	EPoint1.x = 0;
	EPoint1.y = 0;
    return EPoint1;
}
 

CTwoPoints  ComputeParSectionOfTheLenght( CFPoint p0, CFPoint p1, float d , float d1)
{
	CTwoPoints cp2;

	CFPoint pex =  GetPointFmStart(  p0 , p1 , d );
	
	CLineObject mLine = CLineObject ( p0 , pex);
	CLineObject   mLineP =   CLineObject( mLine.GetA() ,mLine.GetB() ,pex )  ; 
	CCircle  c1 = CCircle( pex , d1 );
	cp2 = ComputeIntersectionOfCircleAndLine(   c1 , mLineP );

	return cp2;
}