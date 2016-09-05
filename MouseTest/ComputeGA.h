#pragma once

#include "Foundation.h"

double roundToNearest(double num);
int GetLowestValue( int *table, int size );
int GetHighestValue( int *table, int size );
int GetTheOtherPointIndex( int index , int iret);
int GetTheOtherPointIndex( int index  );
float LtoAngle(float shiftlinear, DRAWDATASTRUCT *pdd  );
// frist two defines segment  the third is the point to check - assuming ThePoint is on the segment 
int IsPoinInsideSegment(CFPoint StartPoint , CFPoint EndPoint ,CFPoint ThePoint); 

CFPoint GetCenterPoint( CFPoint StartPoint , CFPoint EndPoint  );
//    >------------x-----------+
//   start        end         result ( distance from start to result point - could be negative )
CFPoint GetPointFmStart(  CFPoint StartPoint,CFPoint EndPoint, float distance );
//    >------------x-----------+
//   start        end         result ( distance from start to result point - could be negative )
CFPoint GetPointFmStartBend(  CFPoint StartPoint,CFPoint EndPoint, DRAWDATASTRUCT *pdd ,float distance );
CFPoint GetPointFmStart(  CFPoint StartPoint,CFPoint EndPoint,float distance ,DRAWDATASTRUCT *pdd );
float	GetDistance( CFPoint StartPoint , CFPoint EndPoint  );
float	GetDistance( CFPoint  mstartpoint , CFPoint EndPoint  , DRAWDATASTRUCT *pdd  ) ;
float	GetDistanceLineToPoint( CFPoint  mstartpoint , CFPoint EndPoint  ,  CFPoint  mPoint   ) ;
float	GetDistanceAngle( CFPoint  mstartpoint , DRAWDATASTRUCT *pdd , CFPoint  mpoint ) ;
float   GetDistanceBtwTwoLines( CFPoint StartPoint1 , CFPoint EndPoint1 , CFPoint StartPoint2 , CFPoint EndPoint2 ) ;
CFPoint ComputeTwoParLines( CFPoint SPoint ,CFPoint EPoint , float disp , int point_index ); 
void DrawTwoParLines( CPoint SPoint ,CPoint EPoint , int disp, HDC hdc ) ;
// verified up to 20140718
float	ComputeVectorAngle( CFPoint startPoint , CFPoint endPoint ) ;
// verified up to 20140718
CFPoint fCrossOfTwoVectors ( CVector vFirst ,CVector vSecond );
// Basic math
CFPoint   CrossOfTwoLines ( CLineObject  mLineMath1 , CLineObject  mLineMath2 ) ;
// verified up to 20140718
int ComputeBendToLink2( CFPoint StartPoint1 ,CFPoint EndPoint1  ,CFPoint StartPoint2 ,CFPoint EndPoint2 ,DRAWDATASTRUCT  *ps);
// verified up to 20140718
int ComputeBendToLink1( CFPoint StartPoint1 ,CFPoint EndPoint1  ,CFPoint StartPoint2 ,CFPoint EndPoint2 ,DRAWDATASTRUCT  *ps);
//int ComputeBendToLink1( CPoint StartPoint1 ,CPoint EndPoint1  ,CPoint StartPoint2 ,CPoint EndPoint2 ,DRAWDATASTRUCT  *ps);
// could be obsolited
//CFPoint ComputePointFromLinePointOrigin ( CLineObject mLine , CFPoint  origin , float displacement ) ;
// verified 20140718 
CFPoint ComputePointFromLinePointOriginExt ( CLineObject mLine  , CFPoint  CenterPoint , float  fRadius , int idir );
// verified up to 20140718
float  GetAbsAngleOfTwoVectors ( float a1, float a2 );
float  GetAbsAngleOfTwoVectors ( CVector vFirst ,CVector vSecond );
float  GetAbsAngleOfTwoVectors ( CFPoint StartPoint1 ,CFPoint EndPoint1  ,CFPoint StartPoint2 ,CFPoint EndPoint2 );
// obsolite as we cen get it directly from GetPoint - index 5
CFPoint ComputeBendExtendedPoint ( CFPoint  CenterPoint , CFPoint  MidleEndPoint , CFPoint OtherEndPoint);
// verified 20140718
CFPoint ComputeCenterPoint( CFPoint StartPoint ,CFPoint EndPoint , CFPoint ExtPoint, float iRadius  )  ;
// verified 20140718
CFPoint ComputeExtendedLinePoint( CFPoint CenterPoint ,CFPoint StartPoint , CFPoint EndPoint );
//  new 20140724					*
//									|
//    >------------x----------------+
//									|
//									*
//   StartPoint        EndPoint         result ( distance from start to result point - could be negative but zero )
CSegment ComputeSectionPependicularAtPoint( CFPoint StartPoint ,CFPoint EndPoint ,float distance,float lenght);
//  Same as above but also  allows 0 - offset
CSegment ComputeSectionPependicularAtPointExt( CFPoint StartPoint ,CFPoint EndPoint ,float offset ,float lenght); 
//  Get  point on the line closest to the ExtPoint
//						x ExtPoint
//	StartPoint			|
//     x----------------X-----------x EndPoint
//						| X = point returned
//						|
CFPoint GetPointAtTheLine (  CFPoint StartPoint ,CFPoint EndPoint , CFPoint ExtPoint );

CTwoPoints  ComputeIntersectionOfTwoCircles(CCircle  c1 , CCircle c2  );
CTwoPoints  ComputeIntersectionOfCircleAndLine(CCircle  c1 , CLineObject l1 );
CTwoPoints  ComputeIntersectionOfCircleAndSegment(CCircle  c1 , CFPoint StartPoint,CFPoint EndPoint  );
CTwoPoints  ComputeIntersectionOfCircleAndSegment( CFPoint CenterPoint , float Radius , CFPoint StartPoint,CFPoint EndPoint  );
CTwoPoints  ComputeIntersectionOfCircleAndCircleSegment( CCircle  c1 , CCircle  c2 , CFPoint StartPoint,CFPoint EndPoint  );
 
CTwoPoints  ComputeParSectionOfTheLenght( CFPoint p0, CFPoint p1, float d , float d1);


//  check whether test point is on the line betwean StartPoint & EndPoint  - 1 of yes
int IsOnTheStrightLine( CFPoint StartPoint ,CFPoint EndPoint, CFPoint TestPoint);
int IsOnTheBendLine( CFPoint StartPoint ,CFPoint EndPoint, CFPoint TestPoint , float radius ,CFPoint CenterPoint);


double roundToNearest(double num);  

#define PI  3.14159265358979323846
#define PI180  0.0174532925199433
#define C180PI 57.2957795 
