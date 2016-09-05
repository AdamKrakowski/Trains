#include "stdafx.h"
#include "Math.h"

#include "MouseTest.h"
 
#include "Foundation.h"
#include "Objects.h"

#include "ComputeGA.h"
#include "Graphics.h"

#include "SnapObj.h"

#include "Debug.h"
extern CDebug mDebug;

 extern CSystem  mSystem;

//mSystem.SystemErrorMessage( 10 ) ;



CRail::CRail()
{
	mtype =0;
	mDebugData.DPoint[0]=CPoint(0,0); 
	mDebugData.DPoint[1]=CPoint(0,0); 
	mDebugData.DPoint[2]=CPoint(0,0); 
	mDebugData.DPoint[3]=CPoint(0,0); 
	mDebugData.DPoint[4]=CPoint(0,0); 
	mDebugData.DPoint[5]=CPoint(0,0); 

	mDrawData.StartPoint= CFPoint(0,0);
	mDrawData.EndPoint= CFPoint(0,0);
	mDrawData.ConnID[2] = mDrawData.ConnID[1] = mDrawData.ConnID[0] = 0;
    //SwitchOn = 0;
	mSelected =0;

	mDrawData.Turn = 0;
	mDrawData.Width = 10;
	mDrawData.mStartAngle = START_STANDARDANGLE;
	mDrawData.mSweepAngle = SWEEP_STANDARDANGLE;
	mDrawData.Radius = LEAD_STANDARDRADIUS;

	 
	mIndexes[2]=mIndexes[1]=mIndexes[0]=0;

}

// Virtual
void  CRail::DrawObject( HDC hdc ) 
{
/*  direct to derived classes */
}
// Virtual
void  CRail::MoveObject(  int  dx, int  dy)
{

}

//	 virtual 
void  CRail::TurnObject(  CPoint point, int endpoint )
{


}


void  CRail::RotateObject(   float  fangle , int endpoint)
{
//	 virtual 
}

void  CRail::DisplaceObject(  CFPoint DestPoint , int endpoint) 
{
//	 virtual 
}


CRect  CRail::GetBoundingRect() 
{
//	 virtual 
	CRect rc(0,0,0,0); return rc;
}

// Virtual

CFPoint CRail::GetPoint( int ipoint , float fref ) 
{
	CFPoint pt = CFPoint(0,0);
	return pt;
}

 
CFPoint CRail::GetPoint( int ipoint ) 
{
	CFPoint pt;
	return pt;
}
  
int CRail::GetSwitchPosition()   // virual 0 - stright 1 bend
{
return 0;
}



void CRail::Serialize(CArchive& ar) 
{


}



void   CRail::DrawTracker( HDC hdc, int state )
{

	switch (state)
	{
	case 0:
		break;

	case 1:
	case 2:
		{
			int nHandleCount = 3;
			for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
			{
				CPoint handle = GetHandle(nHandle);
				PatBlt(hdc, handle.x - 3, handle.y - 3, 7, 7, DSTINVERT);
			}
		}
		break;
	}
}

CPoint CRail::GetHandle(int nHandle)
{
 
	int x, y ;

	switch (nHandle)
	{
	default:
		 
		 break;
	case 1:
		 x = (int)mDrawData.StartPoint.x;
		 y = (int)mDrawData.StartPoint.y;
		break;

	case 2:
		x = (int)mDrawData.EndPoint.x ;
		y = (int)mDrawData.EndPoint.y ;		//y = mRect.top;
		break;

	case 3:
		x = (int)(mDrawData.StartPoint.x +mDrawData.EndPoint.x)/2 ;
		y = (int)(mDrawData.StartPoint.y +mDrawData.EndPoint.y)/2 ;
		break;
	}

	return CPoint(x, y);
}
 
void  CRail::ReCalculateObject()
{
// purely virtual
}

void CRail::ComputeBoundingRect()
{
// purely virtual

}

int CRail::GetHitHandle(CPoint cp)
{
	int i;

	for ( i= 1;i< 4;i++ )
	{
	   CPoint  pt = GetHandle(i);
	   if ( GetDistance ( pt , cp ) < 4)  return i;
	}
	return 0;
}

float  CRail::GetMainAxisLenght()
{
    return GetDistance(mDrawData.EndPoint,mDrawData.StartPoint);
}
 
void  CRail::SetObject( int mode , float radius , float fStartAngle, float fSweepAngl ) 
{
	mDrawData.Radius = radius;
	mDrawData.Turn = mode;
	mDrawData.mStartAngle = START_STANDARDANGLE;
	mDrawData.mSweepAngle = fSweepAngl;

	ReCalculateObject();
	ComputeBoundingRect();
}

void   CRail::SetPosition( CPoint mStartPoint ,CPoint mEndPoint ) 
{
	mDrawData.StartPoint.x = (float)mStartPoint.x;
	mDrawData.StartPoint.y = (float)mStartPoint.y;
	mDrawData.EndPoint.x = (float)mEndPoint.x;
	mDrawData.EndPoint.y = (float)mEndPoint.y;

	if ( mtype == ObjBendRail )
		mDrawData.EndPoint =  GetPointFmStart(  mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_BEND_MAIN_AXIS_LENGHT );

	if ( mtype == ObjRailSwitch )
		mDrawData.EndPoint =  GetPointFmStart(  mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_SWITCH_MAIN_AXIS_LENGHT );

	ReCalculateObject();
	ComputeBoundingRect();
}


void   CRail::SetPoint( int ipoint , CFPoint fpoint)
{
switch ( ipoint )
	{
		case 0:
			  mDrawData.StartPoint = fpoint;
		break;
		case 1:  
		      mDrawData.EndPoint = fpoint;
		break;
		case 2:   
			  mDrawData.CenterPoint = fpoint;
		break;
		default:
		break;
	} 

}

void	CRail::SetPosition( CFPoint mStartPoint ,CFPoint mEndPoint )  
{
	mDrawData.StartPoint = mStartPoint;
	mDrawData.EndPoint = mEndPoint;

	if ( mtype == ObjBendRail )
		mDrawData.EndPoint =  GetPointFmStart(  mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_BEND_MAIN_AXIS_LENGHT );

	if ( mtype == ObjRailSwitch )
		mDrawData.EndPoint =  GetPointFmStart(  mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_SWITCH_MAIN_AXIS_LENGHT );

	ReCalculateObject();
	ComputeBoundingRect();
}
 
void  CRail::SetStandardData(  float radius , int iwide )
{
	mDrawData.Radius = radius;
	mDrawData.Width = iwide;
	ReCalculateObject();
	ComputeBoundingRect();
}
 

void  CRail::SetDebugPoint( int ipoint , CPoint pt ) 
{
	mDebugData.DPoint[ipoint].x = pt.x ;
 	mDebugData.DPoint[ipoint].y = pt.y ;
}

void  CRail::SetDebugPoint( int ipoint , CFPoint pt ) 
{
	mDebugData.DPoint[ipoint].x = (int)pt.x ;
 	mDebugData.DPoint[ipoint].y = (int)pt.y ;
}

int   CRail::GetMode() 
{
	return mDrawData.Turn;
}

float CRail::GetFloat(int ipoint) 
{	 
	if ( ipoint == 0) return mDrawData.mStartAngle;
	else if ( ipoint == 1) return mDrawData.mSweepAngle;
	else return mDrawData.Radius;
}

void  CRail::SetRailId( WORD  mID )
{
	mDrawData.ThisID = mID ;
}

 WORD  CRail::GetRailConnId ( int index) 
{
	if ( index < 3 ) return mDrawData.ConnID[index];
	else return 0xFFFF;
}

void  CRail::CleanCommIDs() 
{
	mDrawData.ConnID[2] = mDrawData.ConnID[1] = mDrawData.ConnID[0] = 0;
}

 void  CRail::SetRailConnId( int index , WORD mID )
{
	if ( index < 3 ) mDrawData.ConnID[index] = mID;
}

WORD  CRail::GetRailId()
{
	return mDrawData.ThisID;
}

DRAWDATASTRUCT *CRail::GetDrawDataStructPtr() 
{
	return &mDrawData;
}

CRect  CRail::GetActiveRegion()
{
	return  mDrawData.mUpdRect;
}

void CRail::SetSelected(int isel)  
{
	mSelected = isel;
}


void  CRail::HandleRailInfo()
{
	DRAWDATASTRUCT *pd =  GetDrawDataStructPtr();
	CRail *p = this;

	//mDebug.CDebugPut("*Rail*:[%X] [%X] [%X] [%X]  ", (int)p  ,(int)p->GetIndex(0)  ,(int)p->GetIndex(1)  ,(int)p->GetIndex(2) );


	if (   p->mtype == ObjStraightRail)
		mDebug.CDebugPut("*Rail_Id:[%d]  ObjStraightRail ", pd->ThisID  );
	if (   p->mtype == ObjBendRail)
		mDebug.CDebugPut("*Rail_Id:[%d]  ObjBendRail ", pd->ThisID  );
	if (   p->mtype == ObjRailSwitch)
		mDebug.CDebugPut("*Rail_Id:[%d]  ObjRailSwitch ", pd->ThisID  );
	if (   p->mtype == ObjRailEnd)
		mDebug.CDebugPut("*Rail_Id:[%d]  ObjRailEnd ", pd->ThisID  );

	mDebug.CDebugPut("*Rail_Id:[%d]  Conn_Ids:[%d][%d][%d] ", pd->ThisID,pd->ConnID[0],pd->ConnID[1],pd->ConnID[2]  );
	mDebug.CDebugPut("-StarPoint:(%6.2f,%6.2f) ",  p->GetPoint(0).x,p->GetPoint(0).y   );
	mDebug.CDebugPut("-EndPoint :(%6.2f,%6.2f) ",  p->GetPoint(1).x,p->GetPoint(1).y  );
	if (   p->mtype == ObjRailSwitch)     
		mDebug.CDebugPut("-SwPoint :(%6.2f,%6.2f) ",  p->GetPoint(2).x,p->GetPoint(2).y  );

	if ( (p->mtype != ObjStraightRail)  && ( p->mtype != ObjRailEnd ) )
	{
		mDebug.CDebugPut("-Radius:%6.2f St: Angle:%6.2f ", pd->Radius ,  pd->mStartAngle );
		mDebug.CDebugPut("-Sweep: %6.2f St: Turn:%6.2f ", pd->mSweepAngle ,  (float)pd->Turn );
		mDebug.CDebugPut("-CenterPoint :(%6.2f,%6.2f) ",  p->GetPoint(GP_CENTER).x,p->GetPoint(GP_CENTER).y  );
 
	}
	
	if (   p->mtype == ObjStraightRail)
		mDebug.CDebugDsp("*Rail_Id:[%d]  ObjStraightRail ", pd->ThisID  );
	if (   p->mtype == ObjBendRail)
		mDebug.CDebugDsp("*Rail_Id:[%d]  ObjBendRail ", pd->ThisID  );
	if (   p->mtype == ObjRailSwitch)
		mDebug.CDebugDsp("*Rail_Id:[%d]  ObjRailSwitch ", pd->ThisID  );
	
	mDebug.CDebugDsp("*Rail_Id:[%d]  Conn_Ids:[%d][%d][%d] ", pd->ThisID,pd->ConnID[0],pd->ConnID[1],pd->ConnID[2]  );
	mDebug.CDebugDsp("-StarPoint:(%6.2f,%6.2f) ",  p->GetPoint(0).x,p->GetPoint(0).y   );
	mDebug.CDebugDsp("-EndPoint :(%6.2f,%6.2f) ",  p->GetPoint(1).x,p->GetPoint(1).y  );

	if ( p->mtype == ObjRailSwitch)
		mDebug.CDebugDsp("-SwPoint :(%6.2f,%6.2f) ",  p->GetPoint(2).x,p->GetPoint(2).y  );
	if ( p->mtype != ObjStraightRail)
	{
		mDebug.CDebugDsp("-Radius:%6.2f Angle:%6.2f ", pd->Radius ,  pd->mStartAngle );
		mDebug.CDebugDsp("-Sweep: %6.2f St: Turn:%6.2f ", pd->mSweepAngle ,  (float)pd->Turn );
		mDebug.CDebugDsp("-CenterPoint :(%6.2f,%6.2f) ",  p->GetPoint(GP_CENTER).x,p->GetPoint(GP_CENTER).y  );
	}
}

CRail  *CRail::GetIndex(int ep) 
{
	return Indexes[ep];
}

void   CRail::SetIndex(int ep , CRail  *pidx ) 
{
	 Indexes[ep] = pidx; 
}

int	CRail::GetIIndex(int ep) 
{
	return mIndexes[ep];
}

void   CRail::SetIndex(int ep , int ip ) 
{
	 mIndexes[ep] = ip; 
}


////////////////////////////////  BEND  //////////////////////////////////////////


CBendRail::CBendRail() 
{

}


CBendRail::CBendRail( DRAWDATASTRUCT  * pDrawData ) 
{
	mtype = ObjBendRail;
	mDrawData.StartPoint =	pDrawData->StartPoint;
	mDrawData.EndPoint =	pDrawData->EndPoint;  // conversion to FPoint
	mDrawData.Radius =		pDrawData->Radius;
	mDrawData.Turn =		pDrawData->Turn;
	mDrawData.Width =		pDrawData->Width;
	mDrawData.mSweepAngle = pDrawData->mSweepAngle;
	mDrawData.EndPoint =	GetPointFmStart( mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_BEND_MAIN_AXIS_LENGHT );
	
	ReCalculateObject();
	ComputeBoundingRect();
}

CBendRail::CBendRail(CPoint cpStartPoint ,CPoint cpEndPoint    )
{
	// the base constructor is called first
	mtype = ObjBendRail;
	mDrawData.StartPoint = cpStartPoint;
	mDrawData.EndPoint = cpEndPoint;  // conversion to FPoint
	mDrawData.EndPoint =  GetPointFmStart(  mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_BEND_MAIN_AXIS_LENGHT );
	ReCalculateObject();
}

CBendRail::CBendRail(CFPoint cpStartPoint ,CFPoint cpEndPoint    )
{
	// the base constructor is called first
	mtype = ObjBendRail;
	mDrawData.StartPoint = cpStartPoint;
 	mDrawData.EndPoint = cpEndPoint;  // conversion to FPoint
	mDrawData.EndPoint =  GetPointFmStart(  mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_BEND_MAIN_AXIS_LENGHT );
	ReCalculateObject();
}


void CBendRail::DrawObject( HDC hdc )
{

	PaintBendRail( &mDrawData, DOUBLELINE | IS_BEND | SWITCHOFF , hdc );
	//	OnDrawRect( &mDrawData.mUpdRect ,   hdc );
}

void  CBendRail::MoveObject(  int  dx, int  dy)
{
	CFPoint DestPoint ;;
	DestPoint.x  = (float)dx;
	DestPoint.y  = (float)dy;
	this->DisplaceObject(DestPoint , 3 ); 
}

void  CBendRail::TurnObject(  CPoint point, int endpoint )
{
 	if ( endpoint == 0 )  // move around start point
	{
		float a2 = 	ComputeVectorAngle( mDrawData.StartPoint , mDrawData.EndPoint   ) ; // main axis	
		float a1 = 	ComputeVectorAngle( mDrawData.StartPoint ,  point ) ; // aux axis 	
		if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return  ; 
		this->RotateObject(a1-a2 ,3 );
	}
	else
	{	 
		float a2 = 	ComputeVectorAngle( mDrawData.EndPoint  , mDrawData.StartPoint    ) ; // main axis
		float a1 = 	ComputeVectorAngle( mDrawData.EndPoint  , point ) ; // aux axis 
		if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return  ; 
		this->RotateObject(a1-a2 ,4 );
	}

}

void CBendRail::ComputeBoundingRect()
{
     ComputeBandingRectSwitch( &mDrawData  ); //  fits better  for  move/rotate
	// ComputeBandingRectBend( &mDrawData  );
}


CRect  CBendRail::GetBoundingRect() 
{
	return mDrawData.mUpdRect;
}


void CBendRail::Serialize(CArchive& ar) 
{
	if ( ar.IsStoring() != 0 )
	{
	//	ar <<  (DWORD)mCount;
		ar.Write(sizeof(DRAWDATASTRUCT),&mDrawData);
	}
	else
		ar.Read(sizeof(DRAWDATASTRUCT),&mDrawData);
}


CFPoint CBendRail::GetPoint( int ipoint , float fref )
{
CFPoint pt = CFPoint(0,0); 

pt.x = mDrawData.CenterPoint.x +  (float)mDrawData.Radius*(float)cos(  PI180*(fref) );
pt.y = mDrawData.CenterPoint.y -  (float)mDrawData.Radius*(float)sin(  PI180*(fref) );
		
return pt;
}


CFPoint CBendRail::GetPoint( int ipoint ) 
{
	CFPoint pt = CFPoint(0,0); 
	switch ( ipoint )
	{
		case 0:
			pt = mDrawData.StartPoint;
		break;
		case 1:  // bend point
			pt.x = mDrawData.CenterPoint.x +  (float)mDrawData.Radius*(float)cos(  PI180*(mDrawData.mStartAngle+mDrawData.mSweepAngle) );
			pt.y = mDrawData.CenterPoint.y -  (float)mDrawData.Radius*(float)sin(  PI180*(mDrawData.mStartAngle+mDrawData.mSweepAngle) );
		break;
		case 2:
		break;
		case GP_CENTER:  
			pt =  mDrawData.CenterPoint ;
		break;
		case 4:  // beginning of the main axis
			pt  = mDrawData.StartPoint;
		break;
		case 5:  // end of the main axis - virtual point for bend
			pt  = mDrawData.EndPoint;
		break;
		case 6:  // end of the main axis - virtual point for bend
			pt = mDrawData.ExtendedPoint;
		break;
		case 7:  // end of the main axis - virtual point for bend
			pt =  GetPointFmStart( this->GetPoint(1) , mDrawData.ExtendedPoint, -STANDARD_BEND_MAIN_AXIS_LENGHT  );      
		break;	
		case 10:
	 		pt = CFPoint ( mDebugData.DPoint[0] ) ;
		break;
		case 11:
	 		pt = CFPoint (mDebugData.DPoint[1] ) ;
		break;
		case 12:
	 		pt = CFPoint (mDebugData.DPoint[2] ) ;
		break;
		case 13:
	 		pt = CFPoint (mDebugData.DPoint[3]  );
		break;
		case 14:
	 		pt = CFPoint (mDebugData.DPoint[4] ) ;
		break;
		case 15:
	 		pt = CFPoint (mDebugData.DPoint[5]  );
		break;
		default:
		break;
	} 
	return pt;
}

void  CBendRail::ReCalculateObject()
{
	mDrawData.CenterPoint =	ComputeTwoParLines( mDrawData.StartPoint ,mDrawData.EndPoint , mDrawData.Radius ,  mDrawData.Turn  ) ;
	mDrawData.mStartAngle =	ComputeVectorAngle( mDrawData.CenterPoint , mDrawData.StartPoint ) ;
	mDrawData.ExtendedPoint = ComputeExtendedLinePoint ( this->GetPoint(3) ,this->GetPoint(0),this->GetPoint(1));
}

float  CBendRail::GetMainAxisLenght()
{
    return STANDARD_BEND_MAIN_AXIS_LENGHT;
}


void  CBendRail::RotateObject(   float  fangle , int endpoint)
{
	float dist  =  this->GetMainAxisLenght();
	CFPoint  pt;

	if ( endpoint == 0 )  // turn around start point  OK
	{
		float fStartAngle =  ComputeVectorAngle(this->GetPoint(0) , this->GetPoint(5)) ;
		pt.x = this->GetPoint(0).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(0).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( this->GetPoint(0) ,pt ); 
	}
	else if ( endpoint == 1) // turn around end point point  !!! strange but works - OK
	{
		float fStartAngle =  ComputeVectorAngle( this->GetPoint(0) , this->GetPoint(5)) ;
		pt.x = this->GetPoint(0).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(0).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( this->GetPoint(0) ,pt ); 
	}
		else if ( endpoint == 3)  // around start point  - handle virtual end point
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(0) ,this->GetPoint(5)) ;
		pt.x = this->GetPoint(0).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(0).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition(  this->GetPoint(0) ,pt );
	}
	else if ( endpoint == 4)  //  around end point  - handle start point  
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(5) ,this->GetPoint(0)) ;
		pt.x = this->GetPoint(5).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(5).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( pt, this->GetPoint(5)  ); 
	}
}

void  CBendRail::DisplaceObject(  CFPoint DestPoint , int endpoint) 
{
	CFPoint stp =  this->GetPoint(0);
	CFPoint enp =  this->GetPoint(5);
	float delta_x,delta_y;

    if (  endpoint == 0 )
	{
 		delta_x = DestPoint.x - this->GetPoint(0).x;
		delta_y = DestPoint.y - this->GetPoint(0).y;
	}
	else if (  endpoint == 1 )
	{
		delta_x = DestPoint.x - this->GetPoint(1).x;
		delta_y = DestPoint.y - this->GetPoint(1).y;
	}
	else if (  endpoint == 3)  // move by vector
	{
		delta_x = DestPoint.x  ;
		delta_y = DestPoint.y  ;
	}
	else return;

	stp.x += delta_x;
	stp.y += delta_y;
	enp.x += delta_x;
	enp.y += delta_y;
	this->SetPosition( stp ,enp ); 

}

/////////////////////////////////////  STRAIGHT ///////////////////////////////////////

CStraightRail::CStraightRail() 
{

}

CStraightRail::CStraightRail(CPoint cpStartPoint ,CPoint cpEndPoint  )
{
    // the base constructor is called first
	mtype = ObjStraightRail;
	mDrawData.StartPoint = cpStartPoint;
	mDrawData.EndPoint = cpEndPoint;

	ComputeBoundingRect();
}

CStraightRail::CStraightRail(CFPoint cpStartPoint ,CFPoint cpEndPoint  )
{
    // the base constructor is called first
	mtype = ObjStraightRail;
	mDrawData.StartPoint = cpStartPoint;
	mDrawData.EndPoint = cpEndPoint;

	ComputeBoundingRect();
}


void CStraightRail::DrawObject( HDC hdc )
{
	PaintStraightRail( &mDrawData, DOUBLELINE , hdc );
}
 
 

void  CStraightRail::MoveObject(  int  dx, int  dy)
{
	CFPoint DestPoint ;;
	DestPoint.x  = (float)dx;
	DestPoint.y  = (float)dy;
	this->DisplaceObject(DestPoint , 3 );
}

void  CStraightRail::TurnObject(  CPoint point , int endpoint )  // 0 totate around stp  1 around enp
{
	if ( endpoint == 0 )  // move around start point
	{
		float a2 = 	ComputeVectorAngle( mDrawData.StartPoint , mDrawData.EndPoint   ) ; // main axis	
		float a1 = 	ComputeVectorAngle( mDrawData.StartPoint ,  point ) ; // aux axis 	
		if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return  ; 
		this->RotateObject(a1-a2 ,3 );
	}
	else
	{	 
		float a2 = 	ComputeVectorAngle( mDrawData.EndPoint  , mDrawData.StartPoint    ) ; // main axis
		float a1 = 	ComputeVectorAngle( mDrawData.EndPoint  , point ) ; // aux axis 
		if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return  ; 
		this->RotateObject(a1-a2 ,4 );
	}
}

void CStraightRail::ComputeBoundingRect()
{
	mDrawData.mUpdRect = CRect ( (int)mDrawData.StartPoint.x , (int)mDrawData.StartPoint.y, (int)mDrawData.EndPoint.x ,(int)mDrawData.EndPoint.y );
	mDrawData.mUpdRect.NormalizeRect();
	mDrawData.mUpdRect.InflateRect( 4 + (int)mDrawData.Width);
}


CRect  CStraightRail::GetBoundingRect() 
{
	return mDrawData.mUpdRect;
}

void CStraightRail::Serialize(CArchive& ar) 
{
	if ( ar.IsStoring() != 0 )
	{
	//	ar <<  (DWORD)mCount;
	ar.Write(sizeof(DRAWDATASTRUCT),&mDrawData);
	 

	}
	else
		ar.Read(sizeof(DRAWDATASTRUCT),&mDrawData);

}

CFPoint CStraightRail::GetPoint( int ipoint ) 
{
	CFPoint pt = CFPoint(0,0);
 
	switch ( ipoint )
	{
		case 0:
 		    pt = mDrawData.StartPoint;
		break;
		case 1:
 			pt = mDrawData.EndPoint;
		break;
		case 10:
		 	pt = CFPoint (mDebugData.DPoint[0]  );
		break;
		case 4:  // beginning of the main axis
			pt  = mDrawData.StartPoint;
		break;
		case 5:  // end of the main axis - virtual point for bend
			pt  = mDrawData.EndPoint;
		break;
		case 11:
		 	pt = CFPoint (mDebugData.DPoint[1]  );
		break;
		case 12:
		 	pt = CFPoint (mDebugData.DPoint[2] ) ;
		break;
		case 13:
		 	pt = CFPoint (mDebugData.DPoint[3]) ;
		break;
		case 14:
	 		pt = CFPoint (mDebugData.DPoint[4] ) ;
		break;
		case 15:
	 		pt = CFPoint (mDebugData.DPoint[5]  );
		break;
		default:
		break;

	}
	return pt;	

}

void  CStraightRail::ReCalculateObject()
{


}

float  CStraightRail::GetMainAxisLenght()
{
    return GetDistance(mDrawData.EndPoint,mDrawData.StartPoint);
}

void  CStraightRail::RotateObject(   float  fangle , int endpoint)
{
	CFPoint pt;
	float dist  =   GetDistance( this->GetPoint(0) ,this->GetPoint(1) );	
	if ( endpoint == 0)
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(0) ,this->GetPoint(1)) ;
		pt.x = this->GetPoint(0).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(0).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition(  this->GetPoint(0) ,pt ); 
	}
	else if ( endpoint == 1)
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(1) ,this->GetPoint(0)) ;
		pt.x = this->GetPoint(1).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(1).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( pt, this->GetPoint(1)  ); 
	}
	else if ( endpoint == 3)  // around start point
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(0) ,this->GetPoint(1)) ;
		pt.x = this->GetPoint(0).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(0).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( this->GetPoint(0) ,pt );
	}
	else if ( endpoint == 4)
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(1) ,this->GetPoint(0)) ;
		pt.x = this->GetPoint(1).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(1).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( pt, this->GetPoint(1)  ); 
	}
}

void  CStraightRail::DisplaceObject(  CFPoint DestPoint , int endpoint) 
{
	CFPoint stp =  this->GetPoint(0);
	CFPoint enp =  this->GetPoint(1);
	float delta_x,delta_y;

	if (  endpoint == 0 )
	{
 		delta_x = DestPoint.x - this->GetPoint(0).x;
		delta_y = DestPoint.y - this->GetPoint(0).y;
	}
	else if (  endpoint == 1 )
	{
		delta_x = DestPoint.x - this->GetPoint(1).x;
		delta_y = DestPoint.y - this->GetPoint(1).y;
	}
	else if (  endpoint == 3)  // move by vector
	{
		delta_x = DestPoint.x  ;
		delta_y = DestPoint.y  ;
	}
	else return;
	 
	stp.x += delta_x;
	stp.y += delta_y;
	enp.x += delta_x;
	enp.y += delta_y;
	this->SetPosition( stp ,enp ); 
}



/////////////////////////////////  SWITCH  /////////////////////////////////


CRailSwitch::CRailSwitch() 
{

}


CRailSwitch::CRailSwitch( DRAWDATASTRUCT  * pDrawData ) 
{

	mtype = ObjRailSwitch;
	mDrawData.StartPoint =	pDrawData->StartPoint;
	mDrawData.EndPoint =	pDrawData->EndPoint;  // conversion to FPoint
	mDrawData.Radius =		pDrawData->Radius;
	mDrawData.Turn =		pDrawData->Turn;
	mDrawData.Width =		pDrawData->Width;
	mDrawData.mSweepAngle = pDrawData->mSweepAngle;
	mDrawData.EndPoint =	GetPointFmStart( mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_SWITCH_MAIN_AXIS_LENGHT );
	iSwitchOff = 0;
	ReCalculateObject();
	ComputeBoundingRect();
}

CRailSwitch::CRailSwitch(CPoint cpStartPoint ,CPoint cpEndPoint    )
{
	// the base constructor is called first
	mtype = ObjRailSwitch;
		iSwitchOff = 0;
	mDrawData.StartPoint = cpStartPoint;
	mDrawData.EndPoint = cpEndPoint;  // conversion to float coordinates
    // fixed lenght of the switch straight rail
	mDrawData.EndPoint =  GetPointFmStart(  mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_SWITCH_MAIN_AXIS_LENGHT );
	ReCalculateObject();
}

CRailSwitch::CRailSwitch(CFPoint cpStartPoint ,CFPoint cpEndPoint    )
{
	// the base constructor is called first
	mtype = ObjRailSwitch;
	mDrawData.StartPoint = cpStartPoint;
	mDrawData.EndPoint = cpEndPoint;  // conversion ??
		iSwitchOff = 0;
    // fixed lenght of the switch straight rail
	mDrawData.EndPoint =  GetPointFmStart(  mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_SWITCH_MAIN_AXIS_LENGHT );
	ReCalculateObject();
}

void  CRailSwitch::ToggleSwitchPosition() 
{
 if( iSwitchOff == 0 )  iSwitchOff = 1;
 else iSwitchOff = 0;
}


void CRailSwitch::DrawObject( HDC hdc )
{
 	DWORD  options = DOUBLELINE;
	if ( iSwitchOff == 1 ) options |= SWITCHOFF;

	PaintRailSwitch( &mDrawData, options , hdc );
	//OnDrawRect( &mDrawData.mUpdRect ,   hdc );
}
 
void  CRailSwitch::MoveObject(  int  dx, int  dy)
{
	CFPoint DestPoint ;;
	DestPoint.x  = (float)dx;
	DestPoint.y  = (float)dy;
	this->DisplaceObject(DestPoint , 3 );
	ReCalculateObject();

}

void  CRailSwitch::TurnObject(  CPoint point , int endpoint )
{
	if ( endpoint == 0 )  // move around start point
	{
		float a2 = 	ComputeVectorAngle( mDrawData.StartPoint , mDrawData.EndPoint   ) ; // main axis	
		float a1 = 	ComputeVectorAngle( mDrawData.StartPoint ,  point ) ; // aux axis 	
		if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return  ; 
		this->RotateObject(a1-a2 ,3 );
	}
	else
	{	 
		float a2 = 	ComputeVectorAngle( mDrawData.EndPoint  , mDrawData.StartPoint    ) ; // main axis
		float a1 = 	ComputeVectorAngle( mDrawData.EndPoint  , point ) ; // aux axis 
		if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return  ; 
		this->RotateObject(a1-a2 ,4 );
	}
		
	ReCalculateObject();
}

void CRailSwitch::ComputeBoundingRect()
{
	ComputeBandingRectSwitch( &mDrawData  );
}


CRect  CRailSwitch::GetBoundingRect() 
{
	return mDrawData.mUpdRect;
}


void CRailSwitch::Serialize(CArchive& ar) 
{
	if ( ar.IsStoring() != 0 )
	{
		ar.Write(sizeof(DRAWDATASTRUCT),&mDrawData);
		ar <<   iSwitchOff;
		//	mDebug.CDebugPut((char *)"SwitchW %d  " ,(int)iSwitchOff ); 

	}
	else
	{
		ar.Read(sizeof(DRAWDATASTRUCT),&mDrawData);
		//ar >> (DWORD)iSwitchOff;   - do not work for int changing member to DWORD
		ar >>  iSwitchOff;
		// mDebug.CDebugPut((char *)"SwitchR %d  " ,(int)iSwitchOff );

	}

}


CFPoint CRailSwitch::GetPoint( int ipoint , float fref )
{
CFPoint pt = CFPoint(0,0); 

pt.x = mDrawData.CenterPoint.x +  (float)mDrawData.Radius*(float)cos(  PI180*(fref) );
pt.y = mDrawData.CenterPoint.y -  (float)mDrawData.Radius*(float)sin(  PI180*(fref) );
		
return pt;
}


CFPoint CRailSwitch::GetPoint( int ipoint ) 
{
	
	CFPoint pt = CFPoint(0,0);
 
	switch ( ipoint )
	{
		case 0:
 		    pt = mDrawData.StartPoint;
		break;
		case 1:
 			pt = mDrawData.EndPoint;
		break;
		case 2:
			pt.x = mDrawData.CenterPoint.x + (float)mDrawData.Radius*(float)cos(  PI180*(mDrawData.mStartAngle+mDrawData.mSweepAngle) );
			pt.y = mDrawData.CenterPoint.y - (float)mDrawData.Radius*(float)sin(  PI180*(mDrawData.mStartAngle+mDrawData.mSweepAngle) );
		break;
		case 3:  
			pt = mDrawData.CenterPoint;
		break;
		case 4:  // beginning of the main axis
			pt  = mDrawData.StartPoint;
		break;
		case 5:  // end of the main axis - virtual point for bend
			pt  = mDrawData.EndPoint;
		break;
		case 6:
			pt = mDrawData.ExtendedPoint;
		break;
		case 7:  // end of the main axis - virtual point for bend
			pt =  GetPointFmStart( this->GetPoint(2) , mDrawData.ExtendedPoint, -STANDARD_SWITCH_MAIN_AXIS_LENGHT  );      
		break;	
		case 10:
		 	pt = CFPoint (mDebugData.DPoint[0]  );
		break;
		case 11:
		 	pt = CFPoint (mDebugData.DPoint[1]  );
		break;
		case 12:
		 	pt = CFPoint (mDebugData.DPoint[2] ) ;
		break;
		case 13:
		 	pt = CFPoint (mDebugData.DPoint[3]) ;
		break;
		case 14:
	 		pt = CFPoint (mDebugData.DPoint[4] ) ;
		break;
		case 15:
	 		pt = CFPoint (mDebugData.DPoint[5]  );
		break;
		default:
		break;

	}
	return pt;	
}

int CRailSwitch::GetSwitchPosition()   // 0 - stright 1 bend
{
   return iSwitchOff;
}


void  CRailSwitch::ReCalculateObject()
{
	mDrawData.CenterPoint =	ComputeTwoParLines( mDrawData.StartPoint ,mDrawData.EndPoint , mDrawData.Radius , mDrawData.Turn );
	mDrawData.mStartAngle =	ComputeVectorAngle( mDrawData.CenterPoint , mDrawData.StartPoint ) ;
	mDrawData.ExtendedPoint = ComputeExtendedLinePoint( this->GetPoint(3),this->GetPoint(0),this->GetPoint(2) );
}


float  CRailSwitch::GetMainAxisLenght()
{
    return STANDARD_SWITCH_MAIN_AXIS_LENGHT;
}


void  CRailSwitch::RotateObject(   float  fangle , int endpoint)
{
	CFPoint pt;
	float dist  =   GetDistance( this->GetPoint(0) ,this->GetPoint(1) );	
	if ( endpoint == 0)
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(0) ,this->GetPoint(1)) ;
		pt.x = this->GetPoint(0).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(0).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition(  this->GetPoint(0) ,pt ); 
	}
	else if ( endpoint == 1)
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(1) ,this->GetPoint(0)) ;
		pt.x = this->GetPoint(1).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(1).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( pt,this->GetPoint(1)  ); 
	}
	else if ( endpoint == 3)  // around start point
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(0) ,this->GetPoint(1)) ;
		pt.x = this->GetPoint(0).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(0).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition(  this->GetPoint(0) ,pt );
	}
	else if ( endpoint == 4)
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(1) ,this->GetPoint(0)) ;
		pt.x = this->GetPoint(1).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(1).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( pt, this->GetPoint(1)  ); 
	}
}

void  CRailSwitch::DisplaceObject(  CFPoint DestPoint , int endpoint) 
{
	CFPoint stp =  this->GetPoint(0);
	CFPoint enp =  this->GetPoint(1);

	float delta_x,delta_y;

	if (  endpoint == 0 )
	{
 		delta_x = DestPoint.x - this->GetPoint(0).x;
		delta_y = DestPoint.y - this->GetPoint(0).y;
	}
	else if (  endpoint == 1 )
	{
		delta_x = DestPoint.x - this->GetPoint(1).x;
		delta_y = DestPoint.y - this->GetPoint(1).y;
	}
	else if (  endpoint == 2) 
	{
		delta_x = DestPoint.x - this->GetPoint(2).x;
		delta_y = DestPoint.y - this->GetPoint(2).y;
	}
	else if (  endpoint == 3)  // move by vector
	{
		delta_x = DestPoint.x  ;
		delta_y = DestPoint.y  ;
	}
	else return;

	stp.x += delta_x;
	stp.y += delta_y;
	enp.x += delta_x;
	enp.y += delta_y;
	this->SetPosition( stp ,enp ); 
}

/////////////////////////////////////  End ///////////////////////////////////////
CEndRail::CEndRail() 
{

}

CEndRail::CEndRail(CPoint cpStartPoint ,CPoint cpEndPoint  )
{
	// the base constructor is called first
	mtype = ObjRailEnd;
	mDrawData.StartPoint	= cpStartPoint;
	mDrawData.EndPoint		= cpEndPoint;
	mDrawData.EndPoint		= GetPointFmStart(  mDrawData.StartPoint,mDrawData.EndPoint, STANDARD_STOP_MAIN_AXIS_LENGHT );	
	ComputeBoundingRect();
}

CEndRail::CEndRail(CFPoint cpStartPoint ,CFPoint cpEndPoint  )
{
	mtype = ObjRailEnd;
	mDrawData.StartPoint = cpStartPoint;
	mDrawData.EndPoint =  GetPointFmStart(  mDrawData.StartPoint, cpEndPoint , STANDARD_STOP_MAIN_AXIS_LENGHT );
	ComputeBoundingRect();
}

void CEndRail::DrawObject( HDC hdc )
{
	PaintEndRail( &mDrawData, DOUBLELINE , hdc );
	// 	OnDrawRect( &mDrawData.mUpdRect ,   hdc );
}

void  CEndRail::MoveObject(  int  dx, int  dy)
{
	CFPoint DestPoint ;;
	DestPoint.x  = (float)dx;
	DestPoint.y  = (float)dy;
	this->DisplaceObject(DestPoint , 3 );
}

void  CEndRail::TurnObject(  CPoint point , int endpoint )
{
	if ( endpoint == 0 )  // move around start point
	{
		float a2 = 	ComputeVectorAngle( mDrawData.StartPoint , mDrawData.EndPoint   ) ; // main axis	
		float a1 = 	ComputeVectorAngle( mDrawData.StartPoint ,  point ) ; // aux axis 	
		if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return  ; 
		this->RotateObject(a1-a2 ,3 );
	}
	else
	{	 
		float a2 = 	ComputeVectorAngle( mDrawData.EndPoint  , mDrawData.StartPoint    ) ; // main axis
		float a1 = 	ComputeVectorAngle( mDrawData.EndPoint  , point ) ; // aux axis 
		if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return  ; 
		this->RotateObject(a1-a2 ,4 );
	}

}

void CEndRail::ComputeBoundingRect()
{
	mDrawData.mUpdRect = CRect ( (int)mDrawData.StartPoint.x , (int)mDrawData.StartPoint.y, (int)mDrawData.EndPoint.x ,(int)mDrawData.EndPoint.y );
	mDrawData.mUpdRect.NormalizeRect();
	mDrawData.mUpdRect.InflateRect( 4 + (int)mDrawData.Width);
}

CRect  CEndRail::GetBoundingRect() 
{
	return mDrawData.mUpdRect;
}


void CEndRail::Serialize(CArchive& ar) 
{
	if ( ar.IsStoring() != 0 )
	{
		ar.Write(sizeof(DRAWDATASTRUCT),&mDrawData);
	}
	else
		ar.Read(sizeof(DRAWDATASTRUCT),&mDrawData);
}


CFPoint CEndRail::GetPoint( int ipoint ) 
{
	CFPoint pt = CFPoint(0,0);
 
	switch ( ipoint )
	{
		case 0:
 		    pt = mDrawData.StartPoint;
		break;
		case 1:
 			pt = mDrawData.EndPoint;
		break;
		case 4:  // beginning of the main axis
			pt  = mDrawData.StartPoint;
		break;
		case 5:  // end of the main axis - virtual point for bend
			pt  = mDrawData.EndPoint;
		break;
		case 10:
		 	pt = CFPoint (mDebugData.DPoint[0]  );
		break;
		case 11:
		 	pt = CFPoint (mDebugData.DPoint[1]  );
		break;
		case 12:
		 	pt = CFPoint (mDebugData.DPoint[2] ) ;
		break;
		case 13:
		 	pt = CFPoint (mDebugData.DPoint[3]) ;
		break;
		case 14:
	 		pt = CFPoint (mDebugData.DPoint[4] ) ;
		break;
		case 15:
	 		pt = CFPoint (mDebugData.DPoint[5]  );
		break;
		default:
		break;

	}
	return pt;	

}
  
float  CEndRail::GetMainAxisLenght()
{
    return STANDARD_STOP_MAIN_AXIS_LENGHT;
}

void  CEndRail::RotateObject(   float  fangle , int endpoint)
{
	CFPoint pt;
	float dist  =   GetDistance( this->GetPoint(0) ,this->GetPoint(1) );	
	if ( endpoint == 0)
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(0) ,this->GetPoint(1)) ;
		pt.x = this->GetPoint(0).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(0).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition(  this->GetPoint(0) ,pt ); 
	}
	else if ( endpoint == 1)
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(1) ,this->GetPoint(0)) ;
		pt.x = this->GetPoint(1).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(1).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( pt, this->GetPoint(1)  ); 
	}
	else if ( endpoint == 3)  // around start point
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(0) ,this->GetPoint(1)) ;
		pt.x = this->GetPoint(0).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(0).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( this->GetPoint(0) ,pt );
	}
	else if ( endpoint == 4)
	{
		float fStartAngle =  	ComputeVectorAngle( this->GetPoint(1) ,this->GetPoint(0)) ;
		pt.x = this->GetPoint(1).x +  (float)dist*(float)cos(  PI180*(fStartAngle+fangle) );
		pt.y = this->GetPoint(1).y -  (float)dist*(float)sin(  PI180*(fStartAngle+fangle) );
		this->SetPosition( pt, this->GetPoint(1)  ); 
	}

}

void  CEndRail::DisplaceObject(  CFPoint DestPoint , int endpoint) 
{
	
	CFPoint stp =  this->GetPoint(0);
	CFPoint enp =  this->GetPoint(1);
 
	float delta_x,delta_y;

	if (  endpoint == 0 )
	{
 		delta_x = DestPoint.x - this->GetPoint(0).x;
		delta_y = DestPoint.y - this->GetPoint(0).y;
	}
	else if (  endpoint == 1 )
	{
		delta_x = DestPoint.x - this->GetPoint(1).x;
		delta_y = DestPoint.y - this->GetPoint(1).y;
	}
	else if (  endpoint == 3)  // move by vector
	{
		delta_x = DestPoint.x  ;
		delta_y = DestPoint.y  ;
	}
	else return;
	 
	stp.x += delta_x;
	stp.y += delta_y;
	enp.x += delta_x;
	enp.y += delta_y;
	this->SetPosition( stp ,enp ); 

}



CSemphore::CSemphore() 
{
	mtype = ObjSemaphore;
	ComputeBoundingRect();
}

CSemphore::CSemphore( CFPoint cpStartPoint ,CFPoint cpEndPoint    )
{
	// the base constructor is called first
	mtype = ObjSemaphore;
	iColorRed = SEM_RED;;
	mDrawData.StartPoint = cpStartPoint;
	mDrawData.EndPoint = cpEndPoint;  // conversion to float coordinates
	ComputeBoundingRect();
}


void CSemphore::DrawObject( HDC hdc ) 
{
		ComputeBoundingRect();
	PaintSemaphore( &mDrawData, iColorRed , hdc );
}

CFPoint CSemphore::GetPoint( int ipoint ) 
{
	if ( ipoint == 0  ) return mDrawData.StartPoint;
	if ( ipoint == 1  ) return mDrawData.EndPoint;
	if ( ipoint == 2  ) return mDrawData.CenterPoint;
	return CFPoint(0,0);
}

void CSemphore::ComputeBoundingRect()
{
	mDrawData.mUpdRect = CRect ( (int)mDrawData.StartPoint.x , (int)mDrawData.StartPoint.y, (int)mDrawData.EndPoint.x ,(int)mDrawData.EndPoint.y );
	mDrawData.mUpdRect.NormalizeRect();
	mDrawData.mUpdRect.InflateRect( 4 + (int)mDrawData.Width  );
}

CRect  CSemphore::GetBoundingRect()
{

	return mDrawData.mUpdRect;
}

void CSemphore::Serialize(CArchive& ar) 
{
	if ( ar.IsStoring() != 0 )
	{
	// mDrawData.StartPoint.Serialize(ar);
	// mDrawData.EndPoint.Serialize(ar);
	// mDrawData.CenterPoint.Serialize(ar);
	// ar << mDrawData.ThisID;
	// ar << mDrawData.ConnID[0];
	}
	else
	{
	 //mDrawData.StartPoint.Serialize(ar);
	 //mDrawData.EndPoint.Serialize(ar);
	 //mDrawData.CenterPoint.Serialize(ar);
	 //ar >> mDrawData.ThisID;
	 //ar >> mDrawData.ConnID[0]; 
	}
}


void  CSemphore::ToggleSemPosition() 
{
 if( iColorRed == SEM_GREEN )  iColorRed = SEM_RED;
 else iColorRed = SEM_GREEN;
}

 int   CSemphore::GetSemStatus()
 {
  return  iColorRed ;
 }
