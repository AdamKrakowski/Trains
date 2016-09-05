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
 

///////////////////////////////////////////////////////////////////////////
//
//			V	A	G	O	N
//
//
///////////////////////////////////////////////////////////////////////////


CTrainVagon::CTrainVagon ()   
{

}

CTrainVagon::~CTrainVagon ()
{
	//if( mDrawData.pVagonData != NULL)
	//	if( mDrawVagonData.bmpHandle != NULL)
	//		DeleteObject(mDrawVagonData.bmpHandle);
}


CTrainVagon::CTrainVagon (CFPoint mStartPoint , CFPoint mEndPoint , TMPOBJECT_DRAWDATASTRUCT *p   ) 
{
	mDrawData.Width = 12; /// !!!!!!
	mDrawData.StartPoint	= mStartPoint;
	mDrawData.EndPoint		= mEndPoint;
	mDrawData.Turn = p->Turn;
	mInternalLoopError=0;
 
//	mDrawVagonData.mResourceId = p->iResBitmap;  // do we need this
 
	///////mDrawVagonData.bmpHandle = LoadBitmap(hInst, MAKEINTRESOURCE(mDrawVagonData.mResourceId));
	



	mDrawVagonData.bmpHandle = (HBITMAP)p->bmHandle;
	mDrawData.pVagonData	=	&mDrawVagonData;
	
	
	 mDrawData.EndPoint		=   GetPointFmStart(    mStartPoint ,  mEndPoint, (float)p->Lenght   );

 


 
	BITMAP bm;
	GetObject ( mDrawVagonData.bmpHandle, sizeof(bm), &bm );
	mDrawData.EndPoint =    GetPointFmStart(    mStartPoint ,  mEndPoint, (float)bm.bmHeight   );
	 

	 //mDebug.CDebugDsp((char *)"Load BMP... %d  %d   ",(int)bm.bmHeight, (int)GetDistance( mDrawData.StartPoint ,mDrawData.EndPoint )  );
	 


	mDrawVagonData.mType = p->tType;
	mDrawVagonData.mHeadHandle=0;
	mDrawVagonData.mTailHandle=0;


	mDrawVagonData.mInitialLenght = GetDistance(mDrawData.StartPoint ,mDrawData.EndPoint);
	
	mDrawVagonData.mCorrection=(float)0;

	mSpeed=0;

	mCorrTable  = 0;

}

int  CTrainVagon::IsBmpHandle()  
{
	if ( mDrawVagonData.bmpHandle != NULL)  return 1;

	return 0;
}


void   CTrainVagon::SetVagonId( WORD mID)  
{

	mDrawData.ThisID = mID ;

}

WORD     CTrainVagon::GetVagonId()  
{
	return  mDrawData.ThisID ;
}

void   CTrainVagon::SetPosition( CFPoint mStartPoint ,CFPoint mEndPoint ) 
{
	mDrawData.StartPoint	= mStartPoint;
	mDrawData.EndPoint		= mEndPoint;
}

void   CTrainVagon::SetHeadPosition(  CFPoint mPoint ) 
{
	mDrawData.StartPoint	= mPoint;
 
}

void   CTrainVagon::SetTailPosition(  CFPoint mPoint ) 
{
 
	mDrawData.EndPoint		= mPoint;
}
/*
void    CTrainVagon::SetRailTailSwitch( int id , int ibend )
{
    
	if ( mRailTailId[1] != id)
	{
		mRailTailId[1] = mRailTailId[0];
		mRailTailBendStatus[1] = mRailTailBendStatus[0];

		mRailTailId[0] = id;
		mRailTailBendStatus[0] = ibend;
	}
	else
		mRailTailBendStatus[1] = ibend;
		
}

int    CTrainVagon::GetRailTailSwitch( int id   )
{
   if ( id == mRailTailId[1] ) return mRailTailBendStatus[1];
   if ( id == mRailTailId[0] ) return mRailTailBendStatus[0];	
   return -1;
}
*/

/*
void	CTrainVagon::ClrRailTailSwitch()  
{
	mRailTailId[0] = 0;
	mRailTailId[1] = 0;
}
*/
 
void   CTrainVagon::SetLenght( CFPoint mStartPoint , CFPoint mEndPoint )
{
	mDrawData.Radius = GetDistance(  mStartPoint ,  mEndPoint ) ;

	// mDebug.CDebugDsp("Initial lenght  : %d   " ,   (int)mDrawData.Radius     );

}

float  CTrainVagon::GetLenght() 
{

	return 	mDrawVagonData.mInitialLenght  ;
}


void CTrainVagon::AddCurrentDif( float dif )
{
	mDifTable[1] = mDifTable[0];
	mDifTable[0] = dif;
}

 
	// float mCorrTable[2];


float CTrainVagon::GetCurrentCorr()
{
	float fdif = mDifTable[1] - mDifTable[0];
	float fcor = abs(mDifTable[0]);

	if ( fcor < 0.02 ) fcor = (float)0.02;
	if ( fcor > 0.50 ) fcor = (float)0.50;

	if ( mDifTable[0] > 0.04 )  // need correction
	{
		if (  mCorrTable == 0 )  mCorrTable = (float)0.02 ; // always start with small 
		else  
		{
		 if (  mCorrTable >  0 )
		 {
			 if ( fdif > 0 )  mCorrTable = (float)fcor ;  // keep going  mDifTable[1] older  mDifTable[0]  newer
			 else 	mCorrTable = (float)-fcor ; // reverse 
		 }
		 else
		 {
			 if ( fdif  >   0 )  mCorrTable = (float)-fcor ;  // ok dir
			 else 	mCorrTable = (float)fcor ; 
		 }
		}

	}
	else
	{

		mCorrTable = 0;
	}


	return  mCorrTable;
}

 

//	(0)	<------- -1		rail dir		1------->		(1)
//					[(0)		(1)]
//					<--------------| 1 -initially
//					|--------------> -1

void     CTrainVagon::SetDirection(  int dir )
{
	// if dir = -1   then  GetPoint(0) is the direction endpoint
	mDrawVagonData.mRailHeadDir = dir;
	mDrawVagonData.mRailTailDir = dir;
	mDrawVagonData.mVagonDir = 1;  // always the same direction from start
}

void     CTrainVagon::ChangeDirection()
{
    mDrawVagonData.mVagonDir *= -1;     // change to direction vagon moves - reference to the vagon
    mDrawVagonData.mRailHeadDir *= -1;  // change to direction vagon moves - reference to the segment
	mDrawVagonData.mRailTailDir *= -1;  // change to direction vagon moves - reference to the segment
}
 
void     CTrainVagon::SetLocation( CRail *p,float relp )
{
 	mDrawVagonData.mHeadPointer = p;  // head and tail on the same segment    
	mDrawVagonData.mTailPointer = p; 
	mDrawVagonData.mRailHeadDir =  1;  // start towards 0
	mDrawVagonData.mRailTailDir =  1;  // the same
	//mDrawVagonData.mCurrHeadPos = relp;

	//mDrawData.StartPoint = GetPointFmStart( mDrawVagonData.mHeadPointer->GetPoint(0) , mDrawVagonData.mHeadPointer->GetPoint(1) , mDrawVagonData.mCurrHeadPos );
	
	mDrawData.StartPoint =
		GetPointFmStart( mDrawVagonData.mHeadPointer->GetPoint(0) , mDrawVagonData.mHeadPointer->GetPoint(1) , relp) ;
	
		
	
	//mDrawData.EndPoint = GetPointFmStart( mDrawVagonData.mHeadPointer->GetPoint(0) , mDrawVagonData.mHeadPointer->GetPoint(1) , mDrawVagonData.mCurrHeadPos+ mDrawVagonData.mRailTailDir*GetLenght() );
}

 CRect	CTrainVagon::GetBoundingRect()
 {
	 // mDebug.CDebugPut("Lenght: %d  %d" ,  (int)GetDistance( GetPoint(0),GetPoint(1)),(int)GetDistance( mDrawData.StartPoint ,mDrawData.EndPoint)   );
	 ComputeBandingRectTrain( &mDrawData  );
	 return mDrawData.mUpdRect;
 }

 //   moving forward in (0) dir or backward to (1)
int      CTrainVagon::GetVagonDir()
{
	return mDrawVagonData.mVagonDir;
}
 int     CTrainVagon::GetVagonHeadDir()
 {
	 return mDrawVagonData.mRailHeadDir;

 }

int      CTrainVagon::GetVagonTailDir()
{
	return mDrawVagonData.mRailTailDir;
}

//void     CTrainVagon::SetVagonDir(int adir) 
//{
//	mDrawVagonData.mVagonDir = adir;
//}

void     CTrainVagon::SetVagonHeadDir(int adir) 
{
	mDrawVagonData.mRailHeadDir = adir;
}
void     CTrainVagon::SetVagonTailDir(int adir) 
{
	mDrawVagonData.mRailTailDir = adir;
}

//  rails storage
#include "ObjectStore.h"
extern CList  mList;

//    move vagon if status - started
//
//
//
//
//

void     CTrainVagon::StartTheLoco( int ispeed ) 
{
	mSpeed = ispeed;
	mSpeedLoad = ispeed;
}
void     CTrainVagon::StopTheLoco() 
{
	mSpeed = 0;
}
void     CTrainVagon::SetSpeed(int ispeed)
{
	mSpeed = ispeed;
	mSpeedLoad = ispeed;
}

int		CTrainVagon::GetSpeed()
{
	return mSpeed;
}


int  CTrainVagon::Move(int delta ,int istep)
{
 	
	if ( mInternalLoopError != 0 ) 
	{		
		mDebug.CDebugPut("Move:Loop Error !!!! ID:[%d] Error: %d " ,mDrawData.ThisID , mInternalLoopError );
		mSystem.SystemErrorMessage( 100 ) ;
		mInternalLoopError = 0;
		return 0;
	}

	if ( istep ==1) 
	{
		MoveI(  delta);
		return 1;
	}

	if ( mSpeed == 0 ) return 0;

	if ( mSpeedLoad <= 0 ) return 0;

	mSpeedLoad -=1;

	if ( mSpeedLoad == 0 )
	{
		MoveI(  delta);
		mSpeedLoad = mSpeed; // reload
		return 1;
	}

	return 0;
}


CRail    *CTrainVagon::GetCurrentHeadPointer()
{

	return mDrawVagonData.mHeadPointer;

}

void	CTrainVagon::SetCurrentHeadPointer(CRail *p)
{
	mDrawVagonData.mHeadPointer = p;
}

CRail    *CTrainVagon::GetCurrentTailPointer()
{

	return mDrawVagonData.mTailPointer;

}

void	CTrainVagon::SetCurrentTailPointer(CRail *p)
{
	mDrawVagonData.mTailPointer = p;
}
 

void	CTrainVagon::SetCurrentEopPointer( int ep,  CRail *p)
{
	if ( ep > 1 ) return ;
	if ( ep == 0 ) mDrawVagonData.mHeadPointer = p;
	else mDrawVagonData.mTailPointer = p;
}


 
CRail	*CTrainVagon::GetCurrentEopPointer( int ep ) 
{
    if ( ep > 1 ) NULL ;
	if ( ep == 0 ) return mDrawVagonData.mHeadPointer ;
	else	 return mDrawVagonData.mTailPointer;
}



void     CTrainVagon::SetPosition( int ep,  CFPoint mPoint ) 
{
	if ( ep > 1 ) return ;
	if ( ep == 0 ) mDrawData.StartPoint = mPoint ;
	else	mDrawData.EndPoint = mPoint ;
}	 
CFPoint  CTrainVagon::GetPosition( int ep  )  
{
    if ( ep > 1 ) mDrawData.StartPoint ;
	if ( ep == 0 ) return mDrawData.StartPoint ;
	else	 return mDrawData.EndPoint;
}
void     CTrainVagon::SetSwitchPosition( int ep, int onoff )
{
	if ( ep < 2 ) mDrawVagonData.OnTheBend[ep] = onoff;
}
int      CTrainVagon::GetSwitchPosition( int ep  ) 
{
	if ( ep < 2 ) return mDrawVagonData.OnTheBend[ep];
	else return 0;
}


int      CTrainVagon::GetHandle(int iz)
{
	if ( iz == 1 ) return mDrawVagonData.mTailHandle;
	return mDrawVagonData.mHeadHandle;
}

void     CTrainVagon::SetHandle( int iz, WORD  id) 
{
	if ( iz == 0 )   mDrawVagonData.mHeadHandle = id;
	if ( iz == 1 )   mDrawVagonData.mTailHandle = id;
}


int  CTrainVagon::GetVagonType()
{
	return  mDrawVagonData.mType;
}

void  CTrainVagon::DrawObject( HDC hdc ) 
{
	PaintTrain( &mDrawData, SINGLELINE , hdc );

	//CRect rc =  GetBoundingRect();
	//  OnDrawRect( &rc  ,   hdc );
}


CFPoint CTrainVagon::GetPoint( int ipoint ) 
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
		 
		default:
		break;

	}
	return pt;	

}

int CompTempMoveOEp( OTHERVAGONEND_DRAWDATASTRUCT *oes );

void   CTrainVagon::MoveI(int delta)
{
	int idelta;  CFPoint mHeadPoint,mTailPoint;
	oes = &soes;
 
	oes->error =0;
	oes->endrail_reached = 0; 

	idelta = delta*mDrawVagonData.mRailHeadDir;
	oes->iShiftDistance = (abs)((float)idelta);
	oes->ep = 1; if ( idelta < 0  ) oes->ep = 0;
	oes->Curr_Point = GetPoint(0); // mDrawVagonData.StartPoint;
 	oes->iDirEp = mDrawVagonData.mRailHeadDir;
	oes->pCurrSegment = mDrawVagonData.mHeadPointer;
	
	oes->onbend =  mDrawVagonData.OnTheBend[0];
	oes->onbend =	oes->onbendHead;

	int iresult = ::CompTempMoveOEp (oes );

	if (  oes->error != 0) { StopTheLoco(); mInternalLoopError =1; return ;}

	mHeadPoint = oes->New_Point;
	mDrawVagonData.mRailHeadDir = oes->iDirEp;
	mDrawVagonData.mHeadPointer = oes->pNewSegment;
	
	mDrawVagonData.OnTheBend[0] = oes->onbend;
	oes->onbendHead = oes->onbend;

	float d,l,corr;
 
	{
	idelta = delta*mDrawVagonData.mRailTailDir;
	oes->iShiftDistance = (abs)((float)idelta);
	corr =  GetCurrentCorr();
 	oes->iShiftDistance += corr;
	oes->ep = 1; if ( idelta < 0  ) oes->ep = 0;
	oes->Curr_Point = GetPoint(1); // mDrawVagonData.StartPoint;
	oes->iDirEp = mDrawVagonData.mRailTailDir;
	oes->pCurrSegment = mDrawVagonData.mTailPointer;
	
	oes->onbend =  mDrawVagonData.OnTheBend[1];
	oes->onbend =	oes->onbendTail;
	iresult = ::CompTempMoveOEp (oes );

	if (  oes->error != 0) { StopTheLoco(); mInternalLoopError =1; return ;}

	mTailPoint = oes->New_Point;
	mDrawVagonData.mRailTailDir = oes->iDirEp;
	mDrawVagonData.mTailPointer = oes->pNewSegment;
	
	oes->onbendTail = oes->onbend;
	mDrawVagonData.OnTheBend[1] = oes->onbend;

	d =  GetDistance(mHeadPoint , mTailPoint );
	l =  GetLenght() ;
	AddCurrentDif( abs(d-l) );

	if ( abs(d-l) > 10 )
		{
			mSystem.SystemErrorMessage( 200 ) ;
			mInternalLoopError = 1;
		}
	}

	 
	 //imarker+=1;
	 
//	if ( abs(d-l) > 0.04 )
//	{
//   mDebug.CDebugPut("Distance d1:%6.2f Cor:%6.2f  Dif:%4.2f ",  d ,  corr , d-l );
  // mDebug.CDebugPut("Marker:%d HID:%d  TID:%d ",  imarker, mDrawVagonData.mHeadPointer->GetRailId() , mDrawVagonData.mTailPointer->GetRailId() );
//	}
	 
 
	SetPosition( mHeadPoint , mTailPoint) ;

	if ( oes->endrail_reached == 1 ) StopTheLoco();

}
