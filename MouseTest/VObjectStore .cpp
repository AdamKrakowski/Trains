
#include "stdafx.h"
#include "Windowsx.h"
#include "Math.h"
#include <stdio.h>

#include "MouseTest.h"

#include "Foundation.h"

#include "ObjectStore.h"
#include "SnapObj.h"

#include "ComputeGA.h"

#include "Graphics.h"

#include "Debug.h"

extern CDebug mDebug;
extern CList  mList;
extern CSystem  mSystem;

//mSystem.SystemErrorMessage( 10 ) ;

#define N_DEBUG_MODE_VAGONS



float   GetDistanceIndep(   CFPoint StartPoint , CFPoint EndPoint , CRail  *p1 , CRail  *p2 ) 
{
	if ( p1 == p2 )
	{
		if ( p1->mtype == ObjStraightRail ) return GetDistance(  StartPoint ,   EndPoint);
		else 
			if ( p1->mtype  == ObjBendRail)
			{
				DRAWDATASTRUCT *pdd  = p1->GetDrawDataStructPtr();
				return GetDistance(  StartPoint ,   EndPoint, pdd );
			}
			else if ( p1->mtype  == ObjRailSwitch)
			{
				if ( p1->GetSwitchPosition() == 1 )
				{
					DRAWDATASTRUCT *pdd  = p1->GetDrawDataStructPtr();
					return GetDistance(  StartPoint ,   EndPoint , pdd );
				}
				else return GetDistance(  StartPoint ,   EndPoint);

			}

	}
	else
	{
		//  more complex tbd



	}


	return 0;
}

int  GoSwitchRail( OTHERVAGONEND_DRAWDATASTRUCT *oes  )
{
	CRail *pr;

	float fLinRest = oes->iShiftDistanceRemainder;	if (fLinRest == 0) fLinRest+=(float)0.01;
	pr = mList.GetByEndPoint( oes->pCurrSegment , oes->ep);	if ( pr == NULL ) return 1;

	oes->pNewSegment = pr;

	if ( pr->mtype == ObjStraightRail || pr->mtype == ObjRailEnd ) // next is the straight
	{
		if ( pr->mode2 == 0 ) oes->New_Point = GetPointFmStart( pr->GetPoint(0) , pr->GetPoint(1) , fLinRest );
		else				  oes->New_Point = GetPointFmStart( pr->GetPoint(1) , pr->GetPoint(0) , fLinRest );
		
		if (   pr->mtype == ObjRailEnd ) oes->endrail_reached = 1;

	}

	if ( pr->mtype == ObjBendRail ) // next is the bend
	{
		if ( pr->mode2 == 0 ) oes->New_Point  = GetPointFmStart( pr->GetPoint(0), pr->GetPoint(1), fLinRest , pr->GetDrawDataStructPtr());
		else				  oes->New_Point  = GetPointFmStart( pr->GetPoint(1), pr->GetPoint(0), fLinRest , pr->GetDrawDataStructPtr());
	}

	if ( pr->mtype == ObjRailSwitch ) // next is the switch
	{		
		if ( pr->mode2 == 2 ) oes->onbend = 1;  // we are on the bend
		else if (  pr->mode2 == 1 ) oes->onbend = 0; // we are on the stright
		else
			{  // We need to check if We bend or not
				if ( pr->GetSwitchPosition() == 1 ) oes->onbend = 1; // bend
				else  oes->onbend = 0;// stright
			}

		if ( oes->onbend == 0 ) // next is the stright
			{
			if ( pr->mode2 == 0 ) oes->New_Point = GetPointFmStart( pr->GetPoint(0) , pr->GetPoint(1) ,  fLinRest );
			else				  oes->New_Point = GetPointFmStart( pr->GetPoint(1) , pr->GetPoint(0) ,  fLinRest );
			}
		else
			{
			if ( pr->mode2 == 0 ) oes->New_Point = GetPointFmStart(pr->GetPoint(0),pr->GetPoint(2),fLinRest,pr->GetDrawDataStructPtr());
			else				  oes->New_Point = GetPointFmStart(pr->GetPoint(2),pr->GetPoint(0),fLinRest,pr->GetDrawDataStructPtr());
			}
	}

	 if ( pr->mode2 == 0 ) 	oes->iDirEp =  1;	else 	oes->iDirEp = -1; 
	return 0;
}


//--------------------------------
// input : oes->ep   [0,1] -	the direction to go
//		 : oes->pCurrSegment  (oes->pslave_segment_tmp)
//		 : oes->Curr_Point	(oes->pslave_pointOther)
//		 : oes->iShiftDistance
// output:
//		 : oes->New_Point (oes->pslave_point_tmp)
//		 : oes->pNewSegment (oes->pslave_segment_tmp)
//


int  GoRailBend( OTHERVAGONEND_DRAWDATASTRUCT *oes  )
{
	CRail *pr = oes->pCurrSegment;	int ob=1; if ( pr->mtype == ObjRailSwitch ) ob=2;
	int oth_ep = GetTheOtherPointIndex(oes->ep, ob );

	float fSegLenght = GetDistance( pr->GetPoint(0) , pr->GetPoint(ob) , pr->GetDrawDataStructPtr() ); //// INDP
	float fSegRelPos = GetDistance( pr->GetPoint(oth_ep ) , oes->Curr_Point ,  pr->GetDrawDataStructPtr() );// INDP

	if (  (fSegRelPos + oes->iShiftDistance ) <=  fSegLenght ) // INDP
	{
        int ths_ep =   ( oes->ep != 0  ) + ( ( ob ==2 ) && ( oes->ep !=0 ))  ;
		oes->New_Point  = GetPointFmStart( pr->GetPoint(oth_ep) , pr->GetPoint(ths_ep) , fSegRelPos + oes->iShiftDistance , pr->GetDrawDataStructPtr() );
		oes->pNewSegment = pr; // default segment = current
	}
	else
	{ // next track
		oes->iShiftDistanceRemainder  = abs( fSegLenght - (fSegRelPos + oes->iShiftDistance)); 
		if ( oes->ep != 0 ) oes->ep  = ob;
		return GoSwitchRail(  oes  );
	}	 
	return 0;
}


//--------------------------------
// input : oes->ep   [0,1] -	the direction to go
//		 : oes->pCurrSegment  (oes->pslave_segment_tmp)
//		 : oes->Curr_Point	(oes->pslave_pointOther)
//		 : oes->iShiftDistance
// output:
//		 : oes->New_Point (oes->pslave_point_tmp)
//		 : oes->pNewSegment (oes->pslave_segment_tmp)
//

int  GoRailStraight( OTHERVAGONEND_DRAWDATASTRUCT *oes  )
{	
	CRail *pr = oes->pCurrSegment;
	int  oth_ep = GetTheOtherPointIndex(oes->ep);

	float fSegLenght = GetDistance( pr->GetPoint(0) , pr->GetPoint(1) );  
	float fSegRelPos = GetDistance( pr->GetPoint(oth_ep) , oes->Curr_Point );

	if (  (fSegRelPos + oes->iShiftDistance ) <=  fSegLenght ) //  old track
	{
		oes->New_Point  = GetPointFmStart( pr->GetPoint(oth_ep) , pr->GetPoint(oes->ep) , fSegRelPos + oes->iShiftDistance );// other,oes->ep
		oes->pNewSegment = pr;
	}
	else
	{	// new track
		oes->iShiftDistanceRemainder  = abs( fSegLenght - (fSegRelPos + oes->iShiftDistance));  // whats left to take  -linear		
		return GoSwitchRail(  oes  );
	}
	return 0;  
}



//--------------------------------
// input : oes->ep   [0,1] -	the direction to go
//		 : oes->Curr_Point
//		 : oes->pCurrSegment
//		 : oes->Curr_Point
//		 : oes->iShiftDistance
// output:
//		 : oes->New_Point
//		 : oes->pNewSegment
//
//

int CompTempMoveOEp( OTHERVAGONEND_DRAWDATASTRUCT *oes )
{	
	CRail *pr = oes->pCurrSegment ;  // oes->pslave->GetCurrentEopPointer( GetTheOtherPointIndex( oes->epslave) );

	if ( pr->mtype == ObjStraightRail || pr->mtype == ObjRailEnd )   // + switch ! On the bend  check  for other point only
		GoRailStraight(  oes  );

	if ( pr->mtype == ObjBendRail )   //  + switch  on the bend check  for other point only
		GoRailBend(  oes  );

	if ( pr->mtype == ObjRailSwitch ) // next is the switch
	{
		if ( oes->onbend == 1)	GoRailBend(  oes  );
		else	GoRailStraight(  oes  );
	}

	return 0;
}



///////////////////////////////////////////////////////////////
///    LIST class
///////////////////////////////////////////////////////////////



CVagonList::CVagonList()
{
	mCount = 0;
	mID = 0;
 
	 

//	pCurSeleted = NULL;
}


//   if  archive contains  the file spec then  verify othervise load brand new
//HMODULE WINAPI GetModuleHandle( _In_opt_  LPCTSTR lpModuleName)
/*
//typedef struct _WIN32_FIND_DATA {
  DWORD    dwFileAttributes;
  FILETIME ftCreationTime;
  FILETIME ftLastAccessTime;
  FILETIME ftLastWriteTime;
  DWORD    nFileSizeHigh;
  DWORD    nFileSizeLow;
  DWORD    dwReserved0;
  DWORD    dwReserved1;
  TCHAR    cFileName[MAX_PATH];
  TCHAR    cAlternateFileName[14];
} WIN32_FIND_DATA, *PWIN32_FIND_DATA, *LPWIN32_FIND_DATA

*/
 

void CVagonList::AddToTheEnd( CTrainVagon *p) 
{
	if ( mCount < MX_VAGONS )  mVagonPointerTable[mCount++] = p;
	mID +=1 ;
	p->SetVagonId( mID) ;
}


int CVagonList:: ComputeSetNearestPoint (CTrainVagon * pv ,CRail *p ) 
{
	// tbd to set on bend rail later - no switch line
	// tbd if set axis on two stright segments 
	if ( p->mtype == ObjStraightRail )
	{
 
		// find point on the line closest to headpointof the car
		CFPoint Cfp = GetPointAtTheLine (  p->GetPoint(0),  p->GetPoint(1) , pv->GetPoint(0) );  // cfp related point
		// what is the stright segment distance 
		float flen = GetDistance( p->GetPoint(0),  p->GetPoint(1));
		// is close enough betwean two ends !!! what if two  lines are paraller and close - error
        if (  GetDistance ( Cfp , p->GetPoint(0) ) < flen )
			if (  GetDistance ( Cfp , p->GetPoint(1) ) < flen )
				if ( GetDistance ( Cfp , pv->GetPoint(0) ) < 32 )  // arbitrary + cfp related
		{
			// get the point of the other end - set the tail of the vagon
			CFPoint mEndPoint;
			int dir ;
			
			if ( GetDistance ( pv->GetPoint(0), p->GetPoint(0) ) < GetDistance ( pv->GetPoint(1), p->GetPoint(0) )   )
			{
				mEndPoint = GetPointFmStart(Cfp,p->GetPoint(1),GetDistance( pv->GetPoint(0), pv->GetPoint(1)));	
				dir =-1;  // towards 0-endpoint
			}
			else
			{
				mEndPoint = GetPointFmStart(Cfp,p->GetPoint(0),GetDistance( pv->GetPoint(0), pv->GetPoint(1)));	
				dir = 1;  // towards 1-endpoint
			}

			pv->SetPosition( Cfp ,  mEndPoint) ;

			pv->SetLenght(Cfp ,  mEndPoint) ;

  //mDebug.CDebugPut("Lenght2: %d   " ,   (int)GetDistance(  Cfp ,  mEndPoint)     );


		 	pv->SetLocation( p ,  GetDistance( p->GetPoint(0) , Cfp  )  );

			pv->SetDirection(dir);

			// set train to beg &  end accordingly    d(alpha) = dt* 180/(PI*R)
			return 1;
		}
	}

	return 0;
}
 
int CVagonList::CTrainVagonSnap(CTrainVagon *pv, CList  *pl)
{
	int i;
	int count = pl->GetCount();

	for ( i =0; i < count ;i++)
	{
		 CRail *p  = pl->GetAt( i ) ;
		 if (   ComputeSetNearestPoint ( pv , p ) )  return 1;
	}
    return 0;
}
 

 CTrainVagon *CVagonList::GetAt( int index) 
{
	CTrainVagon *p;
	p = mVagonPointerTable[index];
	return p;
}


 CTrainVagon *CVagonList::GetAtCoordinates( CPoint pt ) 
 {
	CTrainVagon *p;
	int i;
	for ( i =0;i <mCount; i++)
	{
		p = mVagonPointerTable[i];
		CRect rc = p->GetBoundingRect();
		if ( PointInsideTheRect( pt , rc ) )
		 	return p;
	}
	return NULL;
 }

 CTrainVagon *CVagonList::GetAtId( WORD  id ) 
{
	CTrainVagon *p;
	int i;
 

	for ( i =0; i < mCount ;i++)
	{
	  p = mVagonPointerTable[i];
	  if ( p->GetVagonId() == id )
		  return p;
	}

	return NULL;
}


void CVagonList::VagonsInfo( ) 
{
	CTrainVagon *p;
	int i;
 
	for ( i =0; i < mCount ;i++)
	{

		p = mVagonPointerTable[i];

		mDebug.CDebugPut("VG_ID:[%d] Handles[%d][%d] ", p->GetVagonId(), p->GetHandle(0),p->GetHandle(1) );
	}
}




 int  CVagonList::GetCount() 
{
   return mCount;
}


int  CVagonList::RemoveAtTheEnd()
{
	CTrainVagon *p ;
	if ( mCount == 0 ) return 0;
	p =	mVagonPointerTable[mCount-1] ;
	mCount -=1 ;
//	DRAWDATASTRUCT *dt = p->GetDrawDataStructPtr();
//	Detach(dt->ThisID); 
	delete p;
	return 0;
}




void  CVagonList::LocoControl( int option , int id)
{

	int icount;

	if ( ( icount =  GetCount()) > 0 )
		{
			int i;
			for ( i =0;i < icount;i++ )
				{
						 CTrainVagon *p =  GetAt(i);

						if ( p->GetVagonType() == 0) continue;

						if ( p->GetVagonId() == id)
						{
							switch ( option )
							{
							case	OPTION_CHANGE_DIR:		p->ChangeDirection(); break;
							case	OPTION_SPEED_FAST:		p->SetSpeed(1);		break;
							case	OPTION_SPEED_SLOW:		p->SetSpeed(2);		break;
							case	OPTION_TRAIN_START:		p->StartTheLoco(2); break;
							case	OPTION_TRAIN_STOP:		p->StopTheLoco();	break;
							default : break;
							}
						}

				}

		}
}

 




// filter 0 - no filter  else only ID

void  CVagonList::MoveAllTrains( HWND mhWnd,int istep ,int filter)
{
	int icount;

	if ( ( icount = GetCount()) > 0 )
	{
					int i;

					for ( i =0;i < icount;i++ )
					{
						  CTrainVagon *p =  GetAt(i);
						  COLISSION_DRAWDATASTRUCT CCds;

						 if ( p->GetVagonType() == 0) continue;
						 if ( filter > 0 ) if ( p->GetVagonId() != filter ) continue;
			
						 CRect rc0 = p->GetBoundingRect();


						  if ( p->Move( 2 ,istep ) == 0) continue;  //  move only engine vagon - move linked  vagons -  give the  direction  (vektor move of the ending = beg)
						
						 
						 
						 CRect rc1 = p->GetBoundingRect();
						 
						 CRect rcc;
						 rcc.UnionRect(rc0,rc1);
						 RECT   r = rcc.GetRect();
					//	 InvalidateRect( mhWnd , &r , FALSE );

						   MoveVagons(p);

						 //mDebug.CDebugPut("Move vagons " );	

						 int c= CollisionCheck(p , &CCds );
						 if ( c == 1)
						 {
							 // glue vagons
							 int Id1 = CCds.pVagonFix->GetVagonId( );
							 int Id2 = CCds.pVagonFloat->GetVagonId( );

							 CCds.pVagonFix->SetHandle(CCds.epFix,Id2 );
							 CCds.pVagonFloat->SetHandle(CCds.epFloat,Id1);

							 //mDebug.CDebugPut("CollisionCheck: %d %d ", Id1,Id2 );	
						 }

					    InvalidateSet( p ,  mhWnd  ) ;
 

					}



				}


}

//				dir in reference to rail
//      RaildT	SemDir	Tdir	Res
//		0->1	0->1	0->1	1
//		0->1	0->1	1->0	0
//		0->1	1->0	0->1	0
//		0->1	1->0	1->0	1
//
//		1->0	0->1	0->1	1
//		1->0	0->1	1->0	0
//		1->0	1->0	0->1	0
//		1->0	1->0	1->0	1
//




 void  CVagonList::CheckSemStopAll (CList  *pCntList ) 
 {
	int icount;

	if ( ( icount = GetCount()) > 0 )
	{
		int i;
		for ( i =0;i < icount;i++ )
		{
			  CTrainVagon *p =  GetAt(i);
			  if ( p->GetVagonType() == 0) continue;
 

			  if ( IsLocoPullTheVagons( p) == 1 ) 
				{
					int j,jcount =pCntList->GetCount() ;
					for ( j =0;j < jcount;j++ )
					{
						CSemphore *rp = (CSemphore *)pCntList->GetAt(j);
						CRail *r = p->GetCurrentHeadPointer();
						
	// mDebug.CDebugPut("check stop %d %d ", rp->GetRailConnId(0) ,r->GetRailId()    );					
						
						if ( rp->GetRailConnId(0) == r->GetRailId() )
						{  // head is on the right rail
							// compute distance

 //mDebug.CDebugPut("Distance %d ", GetDistance( p->GetPoint(0) , rp->GetPoint(0) )    );	
							// midle of the vagon
							CFPoint middle = GetCenterPoint( p->GetPoint(0) , p->GetPoint(1)  );
							  //        pss->SetPoint(2,cppt);

							if (GetDistance( middle , rp->GetPoint(2) ) < 50 )
								if ( rp->GetSemStatus() ==  SEM_RED ) 
								{
									//must be driving directions in relation to semphore
									int dir = p->GetVagonHeadDir();

									CFPoint desP;

									if ( dir == 1)  desP = r->GetPoint(1);    // to 1
									else desP = r->GetPoint(0);  

									//float a = GetAbsAngleOfTwoVectors (  middle ,  rp->GetPoint(2), rp->GetPoint(0) ,rp->GetPoint(1));
								 float a = GetAbsAngleOfTwoVectors (  middle ,  desP , rp->GetPoint(0) ,rp->GetPoint(1));

// mDebug.CDebugPut("Angle %6.2f  %6.2f ", a , a   );	

									if ( a > 90 )
											p->StopTheLoco();
								}

						}

					}
				  
				}
			  else 
			  {
				   
				  //  get last vagon
				 CTrainVagon *pe =   GetLastVagon(  p ) ;

				//  mDebug.CDebugPut("push LV:%X " , (int)pe  );

				  if ( pe != NULL )
				  {
 
					int j,jcount =pCntList->GetCount() ;
					for ( j =0;j < jcount;j++ )
					{
						CSemphore *rp = (CSemphore *)pCntList->GetAt(j);
						CRail *r = pe->GetCurrentHeadPointer();
						
	// mDebug.CDebugPut("check stop %d %d ", rp->GetRailConnId(0) ,r->GetRailId()    );					
						
						if ( rp->GetRailConnId(0) == r->GetRailId() )
						{  // head is on the right rail
							// compute distance

 //mDebug.CDebugPut("Distance %d ", (int)GetDistance( pe->GetPoint(0) , rp->GetPoint(0) )    );	
//mDebug.CDebugPut("PointGet %6.2f %6.2f  " ,   rp->GetPoint(2 ).x ,  rp->GetPoint(2 ).y );
							// midle of the vagon
							CFPoint middle = GetCenterPoint( pe->GetPoint(0) , pe->GetPoint(1)  );

							if (GetDistance( middle , rp->GetPoint(2) ) < 50 )
								if ( rp->GetSemStatus() ==  SEM_RED ) 
								{

								int dir = pe->GetVagonHeadDir();

									CFPoint desP;

									if ( dir == 1)  desP = r->GetPoint(1);    // to 1
									else desP = r->GetPoint(0);  

									//float a = GetAbsAngleOfTwoVectors (  middle ,  rp->GetPoint(2), rp->GetPoint(0) ,rp->GetPoint(1));
								 float a = GetAbsAngleOfTwoVectors (  middle ,  desP , rp->GetPoint(0) ,rp->GetPoint(1));
 //mDebug.CDebugPut("Angle %6.2f  %6.2f ", a , a   );	

								if ( a > 90 )   p->StopTheLoco();

								}

						}

					}



				  }




			  }
				  
 


		}
	}

 }

 int	CVagonList::IsTheTrainMoving( CTrainVagon *pv )  
{

	if ( pv->GetVagonType() != 0 )
		if ( pv->GetSpeed() == 0)  return 0;

	CTrainVagon *pv1;
	int handle = pv->GetVagonId();
	int ep =0;
	int hnd = pv->GetHandle(0);
	// go to 0
	//
	//			mDebug.CDebugPut("IsTheTrainMoving handle:%d ",pv->GetVagonId());

	while( 1)
	{ 
		if ( handle == 0 ) break;
		pv1 =   GetAtId( hnd) ;

		if ( pv1 == NULL ) break;

		if ( pv1->GetVagonType() != 0 ) return pv1->GetSpeed();
		//  next ep

		if ( pv1->GetHandle(0) == handle )   ep = 1 ;  
		if ( pv1->GetHandle(1) == handle )   ep = 0 ;  
		
		if ( pv1->GetHandle(ep) == 0 ) break;

		handle = pv1->GetVagonId();
		hnd = pv1->GetHandle(ep);
	}

 

 
	handle = pv->GetVagonId();
	ep =1;
	hnd = pv->GetHandle(1);
	// go to 1

	while( 1)
	{ 
		if ( handle == 0 ) break;
		pv1 =   GetAtId( hnd) ;

		
		if ( pv1 == NULL ) break;

		if ( pv1->GetVagonType() != 0 ) return pv1->GetSpeed();

		//  next ep
		if ( pv1->GetHandle(0) == handle )   ep = 1 ;  
		if ( pv1->GetHandle(1) == handle )   ep = 0 ;  
		
		if ( pv1->GetHandle(ep) == 0 ) break;

		handle = pv1->GetVagonId();

			hnd = pv1->GetHandle(ep);

	}

	return 0;
}

// recursive check

int     CVagonList::CollisionCheckEp( int endpoint ,  CTrainVagon *px , COLISSION_DRAWDATASTRUCT *pcd )
{	
	// mDebug.CDebugPut("CollisionEP:(%d) %d %d ", px->GetVagonId() , Hhead , Htail );
	 pcd->MxLoop -=1;
	 if ( pcd->MxLoop == 0 ) 
		 {
		mDebug.CDebugPut("CollisionEP: Error Loop "   );
		mSystem.SystemErrorMessage( 20 ) ;
			 return 0;
	 }

	 int oldId = px->GetVagonId();  //  this ID
	 int nID = px->GetHandle(endpoint); 
	 if ( nID == 0 ) 
		 {
		mDebug.CDebugPut("CollisionEP: Error nID "   );
			mSystem.SystemErrorMessage( 21 ) ;
			 return 0; // error
	     }

	 CTrainVagon *p ;
     p = GetAtId( nID); 
	 if ( p == NULL ) 
		 {
		   mDebug.CDebugPut("CollisionEP: Error GetVagon "   );
		   	mSystem.SystemErrorMessage( 22 ) ;
			 return 0; //error
	 }
	 //mDebug.CDebugPut("CollisionEP new hnd:ID (%d) %d %d ", p->GetVagonId() , p->GetHandle(0) , p->GetHandle(1) );

     if (( p->GetHandle(0) != oldId  )  && ( p->GetHandle(1) != oldId )) 
	 {
		 	mDebug.CDebugPut("CollisionEP: Error no handle "   );
				mSystem.SystemErrorMessage( 23 ) ;
		 return 0; // error
	 }

	 pcd->pVagonFix = p;

	 if (  p->GetHandle(0) == oldId  )
	 {
		 if ( p->GetHandle(1) != 0 ) return CollisionCheckEp( 1 ,  p ,  pcd );
		 else pcd->epFix = 1;
	 }
	 else
	 {
		 if ( p->GetHandle(0) != 0 ) return CollisionCheckEp( 0 , p , pcd );
		 else pcd->epFix = 0;
	 }
 
	return 1;  
}


 
int Collide( CTrainVagon *pv  , CTrainVagon *pvfix ,int ep  )
{

	CRail *ph;
	if ( ep == 0 ) ph = pvfix->GetCurrentHeadPointer();
	else ph = pvfix->GetCurrentTailPointer();

	if ( pv->GetCurrentHeadPointer() != ph )
		if ( pv->GetCurrentTailPointer() != ph )
			return 0;

	CFPoint TestPoint = pvfix->GetPoint(ep);
    CFPoint StartPoint= pv->GetPoint(0);
	CFPoint EndPoint= pv->GetPoint(1);

	float d0  = GetDistance(StartPoint , EndPoint );
	float d1  = GetDistance(StartPoint , TestPoint );
	float d2  = GetDistance(EndPoint   , TestPoint);


	//mDebug.CDebugDsp("colide  : %d   " ,    (int)GetDistance(StartPoint, EndPoint ) ,(int)d1,(int)d2   );

	 
	if ( (d1  <= d0) && (d2 <= d0 )) 
	{
	    if ( d2 < 6 ) return 2;
		else return 1;
	}
	 
	return 0;
}
 


int     CVagonList::CollisionCheck(CTrainVagon *p, COLISSION_DRAWDATASTRUCT *pcd ) 
{

	//  find the last vagon if moving backward & verify collision
//   loco is pulling ?  
//   loco is pushing ?
//   [0] <  (-1)     (1) > [1]  - rail dir
//   head {0}{1} tail    <<<<<<     vagon dir VD (1)       
//	 head {0}{1} tail    >>>>>>     vagon dir (-1)
//
//   is pulling or pushing -> 
//           <-  (0====1)            <- (      )(0====1)     <- (0====1)(      )
//  	if VD==1   if (is vagon attached to head ) -->  push  othervise  pull - if  attached to the end
//          ->   (0====1)             -> (      )(0====1)      -> (0====1)(      )
//		if VD==-1  if (is vagon attached to head ) -->  pull  othervise  push - if  attached to the end
//      what if  both side ???????
//      if collicions stop the train
//

	int ipull =0,ipush=0;  // not needed here but for debug

	int VD = p->GetVagonDir();
	int Hhead = p->GetHandle(0);
	int Htail = p->GetHandle(1);

	 int er = 1;  // OK

	pcd->pVagonFix = p; // default no vagons
	pcd->MxLoop = 8;  // looping sentinel  !!!! max vagons

	if ( VD ==  1 ) 
	{
		pcd->epFix = 0;  // default no vagon attached
		if ( Hhead != 0 )
			{
				ipush=1;     //     <--- (     )[(0)------1] //   colision at vagon
				er  = CollisionCheckEp( 0,  p , pcd );
			}
		if ( Htail != 0 ) 	ipull=1;	//     <---  [(0)------1](     )
	}

	if (  VD ==  -1 ) 
	{
		pcd->epFix = 1; // default no vagon attached
		if ( Hhead != 0 ) 	ipull=1;	//      ---> (     )[0------(1)]  colission at  loco
		if ( Htail != 0 ) 
			{
				ipush=1;		// ---> [0------(1)](     ) 	// colision at  vagon
				er =CollisionCheckEp( 1 ,  p , pcd );
			}
	}		

	//  select endpoint to take care  ( if ep is not embraced in betwean wheeels of any train or hit EP 
 
	if ( er == 0)
	{  mDebug.CDebugPut("Error...  "  ); 	mSystem.SystemErrorMessage( 26 ) ; return 0; }

	// here we should have the fix vagon handle and ep not connected
	//	mDebug.CDebugPut("CollisionCheck:Id (%d) Ep %d   ", pcd->pVagonFix->GetVagonId() , pcd->epFix );

	int i,j;
	CTrainVagon *pv  ;
	CFPoint CPstart,Cpend;

	for ( i =0; i < mCount ;i++)
	{
		pv =	mVagonPointerTable[i] ;
		if ( pcd->pVagonFix == pv) continue;

		CPstart = pv->GetPoint(0);
		Cpend = pv->GetPoint(1);

		//mDebug.CDebugPut("CollisionC?:Id (%d) ( %d,%d)   ", pv->GetVagonId() , (int)CPstart.x , (int)CPstart.y );

	   // if ( (j= Collide( CPstart,Cpend, pcd->pVagonFix->GetPoint( pcd->epFix ))) > 0 ) 
   	    if ( (j= Collide( pv , pcd->pVagonFix , pcd->epFix )) > 0 )     
		{
			pcd->pVagonFloat = pv;
			pcd->epFloat = j-1; 

			///  protection
			// if ( pcd->pVagonFloat->GetVagonId() == 1) return 0;

			// info
			mDebug.CDebugPut("CollisionWith:Id (%d) Ep %d - fixed %d  ", pcd->pVagonFloat->GetVagonId() , pcd->epFloat , pcd->pVagonFloat->GetVagonId());

			return 1;
		}
	}

	return 0;
}

 

CRail *CVagonList::FindRailByLocation ( CFPoint cp )
{
	CRail *p;
    int i;

	for ( i =0; i < mList.GetCount() ; i++)
	{
		p = mList.GetAt(i);

		if ( p->mtype == ObjStraightRail )
			if ( IsOnTheStrightLine( p->GetPoint(0)   ,p->GetPoint(1) , cp))  return p;

		if ( p->mtype == ObjRailSwitch)
			if ( IsOnTheStrightLine( p->GetPoint(0)   ,p->GetPoint(1) , cp)) { p->mode1 =0; return p;}

		if ( p->mtype == ObjBendRail)
			if ( IsOnTheBendLine( p->GetPoint(0) ,p->GetPoint(1),cp,p->GetFloat(GET_FLOAT_RADIUS),p->GetPoint( GP_CENTER ))) return p;   
		
		if ( p->mtype == ObjRailSwitch)
			if ( IsOnTheBendLine( p->GetPoint(0) ,p->GetPoint(2),cp,p->GetFloat(GET_FLOAT_RADIUS),p->GetPoint( GP_CENTER ))) { p->mode1 =1; return p;}
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////////  
//	Entry:
//  mp_oe_data.pmaster	=	 master vagon pointer;
//	mp_oe_data.epmaster =	 master vagon endpoint;
//  mp_oe_data.pslave   =	 slave vagon pointer;
//  mp_oe_data.epslave  =	 slave vagon endpoint connected to the master;
//---------------------------------------------------------------------------
//  return:  0 no errors
//  call:  ShiftVagonNextRail  ( current rail pointer , heading to :oes->ep )
//
//  slaves needs to be update always  - slave become master for the next vagon
// 
//   wich point  is determine where is the  vagon - wich side to the rail EPs
//    rail[0]                          [1]
//       [       ](     )(      )			tpp.mPoint2
//		(       )(      )[       ]			tpp.mPoint1
//



int CVagonList::ShiftVagon( OTHERVAGONEND_DRAWDATASTRUCT *oes )
{ 
 
//  Save the lenght of the slave
	oes->SlaveLen = oes->pslave->GetLenght();  
    //  Find the current master rail pointer
	//  oes->pmaster->GetCurrentEopPointer( oes->epmaster ) ;
	//  Get master endpoint  connected to slave endpoint
	CFPoint pointThis = oes->pmaster->GetPoint( oes->epmaster );  
	// Create vestor from slave endpoint to master endpoint
	CVector v1 = CVector( oes->pslave->GetPoint(oes->epslave), oes->pmaster->GetPoint(oes->epmaster) );
	//  Compute shift - master endpoint slave endpoint  - size of vector
	oes->iShiftDistance = GetDistance( pointThis , oes->pslave->GetPoint(oes->epslave));
    //	Set postion of the slave endpoint to fix at master endpoint - shift one ep
	oes->pslave->SetPosition( oes->epslave ,pointThis  );
	//  Set pointer to the rail of the slave endpoint to fix at master endpoint - shift one ep
	oes->pslave->SetCurrentEopPointer(  oes->epslave  ,  oes->pmaster->GetCurrentEopPointer( oes->epmaster ) );
	// copy on the switch position - regardless it is switch or not 
	oes->pslave->SetSwitchPosition( oes->epslave , oes->pmaster->GetSwitchPosition(oes->epmaster)  );
    //  Temp:  get slave ep oposite to master ep.
	int oep = GetTheOtherPointIndex( oes->epslave );
    //  Temp:  get slave rail pointer oposite to master ep  
	CRail *ps_other  = oes->pslave->GetCurrentEopPointer(oep);
	//  Create vector for bend,stright,switch stricht  - exclude switch bend
	CVector v2 = CVector( ps_other->GetPoint( 0  ), ps_other->GetPoint(1 ) ); 
	// Verify switch bend and modify vector if needed
	if (  ps_other->mtype == ObjRailSwitch )
	{
		oes->onbend =	oes->pslave->GetSwitchPosition(oep) ;		 
		if ( oes->onbend  ==  1 ) 
			v2 = CVector( ps_other->GetPoint( 0  ), ps_other->GetPoint(2) );		 
	}
	//  compute vectors angle and determine move of the oposite slave ep
	if (    GetAbsAngleOfTwoVectors ( v1 , v2  )  < 90 ) oes->ep = 1;
	else oes->ep = 0;

	// used to pass to CompTempMoveOEp  coordinates of the slave other points
	//  CompTempMoveOEp  is local function to the class

	oes->pCurrSegment = oes->pslave->GetCurrentEopPointer(oep );
	oes->Curr_Point	= oes->pslave->GetPoint(oep);

	float corr =  oes->pslave->GetCurrentCorr();
 	oes->iShiftDistance += corr; // add correction
	// if switch get postion on the switch of the ather end
	oes->onbend =	oes->pslave->GetSwitchPosition(oep) ;
	// this is private to CVagonList class
	int iresult = ::CompTempMoveOEp (oes );   // move by  oes->iShiftDistance in current direction  +  or - 

	oes->pslave->SetSwitchPosition( oep ,  oes->onbend );
	//  update the slave other end position
	oes->pslave->SetPosition( oep, oes->New_Point);
	//  update the slave other end segment  
	oes->pslave->SetCurrentEopPointer( oep,oes->pNewSegment );

	// adjustments
	float d =  GetDistance( oes->pslave->GetPoint(0), oes->pslave->GetPoint(1) );
	float l =  oes->SlaveLen;
	oes->pslave->AddCurrentDif( abs(d-l) );
	if ( abs(d-l) > 10 ) mSystem.SystemErrorMessage( 200 ) ;

	return iresult; // no errors
}


//////////////////////////////////////////////////////////////////////////////  
//	Entry:
//  int endpoint		=	 lead vagon endpoint;
//	CTrainVagon *pmaster =	 lead vagon pointer;
//                           contains actual position on the rail (head & tail ) head is related to index 0  while tail to 1
//
//  int ipush   =	 1  lead vagon is pusshing - lead vagon 0 is pulling
//	(0)	<------- -1		rail dir		1------->		(1)
//					[(0)		(1)]
//					<--------------| 1 -initially
//	
//---------------------------------------------------------------------------
//	recursive move
//  move  next vagon attached to the endpoint to the endpoint location
//  find the other point in direction of other endpoint

int  CVagonList::MoveOneAttachedVagon(  int endpoint , CTrainVagon *pmaster ,int ipush )  // master endpoint
{
	int oldId	= pmaster->GetVagonId();  //  save master vagon ID
	int nID		= pmaster->GetHandle(endpoint);	 // get connected slave ID

	if ( nID == 0 ) 
		{ mDebug.CDebugPut("MoveOneAttachedVagon:!no handle to slave found "  ); 	mSystem.SystemErrorMessage( 40 ) ; return -2;}

	mp_oe_data.pmaster	=	pmaster;  // pointer to vagon master
	mp_oe_data.epmaster =	endpoint; // which endp point of the master is attached to slave
	// get connected slave
    mp_oe_data.pslave = GetAtId( nID );  //  get pointer to slave

	if ( mp_oe_data.pslave == NULL )
		{ mDebug.CDebugPut("MoveOneAttachedVagon:!no slave found "  );	mSystem.SystemErrorMessage( 41 ) ;	return -3; }

    if (( mp_oe_data.pslave->GetHandle(0) != oldId  )  && ( mp_oe_data.pslave->GetHandle(1) != oldId )) 
		{ mDebug.CDebugPut("MoveOneAttachedVagon:!handle to master found "  );	mSystem.SystemErrorMessage( 42 ) ; return -4;  }

	if (  mp_oe_data.pslave->GetHandle(0) == oldId  ) // new is fixed by ep=0
	 {
		 	mp_oe_data.epslave = 0;
			ShiftVagon( &mp_oe_data );
			if ( mp_oe_data.pslave->GetHandle(1) != 0  ) //// recurence for  more vagons
			 	return MoveOneAttachedVagon(  1 ,  mp_oe_data.pslave ,ipush  ); // nake slave being master for the next slave ep =1
	 }
	else // new is fixed by ep=1
	 {
			mp_oe_data.epslave = 1;	
			ShiftVagon( &mp_oe_data );
			if ( mp_oe_data.pslave->GetHandle(0) != 0  ) // // recurence for  more vagons   
			 	return MoveOneAttachedVagon(  0 ,  mp_oe_data.pslave ,ipush  ); // nake slave being master for the next slave ep=0
	 }

	return 0; //ok
}


int   CVagonList::IsLocoPullTheVagons(CTrainVagon *p) 
{
	int ipull =0,ipush=0; // can be removed later
	int VD = p->GetVagonDir();
	int Hhead = p->GetHandle(0);
	int Htail = p->GetHandle(1);

	if ( ( Hhead == 0 ) && ( Htail == 0 )) return 1;

	if ( VD ==  1 ) 
	{
		if ( Hhead != 0 ) return 0;
		if ( Htail != 0 ) return 1;
	}

	if (  VD ==  -1 ) 
	{
		if ( Hhead != 0 ) 	return 1;
		if ( Htail != 0 ) return 0;
	}		

	return 0;
}



CTrainVagon *CVagonList::GetLastVagon(CTrainVagon *p) 
{
	int ipull =0,ipush=0; // can be removed later
	int VD = p->GetVagonDir();
	int Hhead = p->GetHandle(0);
	int Htail = p->GetHandle(1);

	if ( ( Hhead == 0 ) && ( Htail == 0 )) return p ;

	if ( Hhead != 0 ) 
	{
		int nID = Hhead;
		int oldID = p->GetVagonId();
 
		while(1)
		{

			CTrainVagon *ps = GetAtId( nID ); 
			if (  ps->GetHandle(0) == oldID  ) // new is fixed by ep=0
			{
				if ( ps->GetHandle(1) == 0 ) return ps;
				nID = ps->GetHandle(1);
			}
			else
			{
				if ( ps->GetHandle(0) == 0 ) return ps;
				nID = ps->GetHandle(0);
			}
			oldID = ps->GetVagonId();
		}

	}

	if ( Htail != 0 ) 
	{
		int nID = Htail;
		int oldID = p->GetVagonId();
 
		while( 1 )
		{

			CTrainVagon *ps = GetAtId( nID ); 
			if (  ps->GetHandle(0) == oldID  ) // new is fixed by ep=0
			{
				if ( ps->GetHandle(1) == 0 ) return ps;
				nID = ps->GetHandle(1);
			}
			else
			{
				if ( ps->GetHandle(0) == 0 ) return ps;
				nID = ps->GetHandle(0);
			}
			oldID = ps->GetVagonId();
		}
	}
	 		
	return NULL;
}



void  CVagonList::MoveVagons(CTrainVagon *p) 
{

	int ipull =0,ipush=0; // can be removed later

	int VD = p->GetVagonDir();
	int Hhead = p->GetHandle(0);
	int Htail = p->GetHandle(1);
	 


	if ( ( Hhead == 0 ) && ( Htail == 0 )) return;  //  nothing attached


	mp_oe_data.endrail_reached = 0;
	mp_oe_data.error = 0;

	if ( VD ==  1 ) 
	{
		if ( Hhead != 0 )
			{
				ipush=1;    
				MoveOneAttachedVagon( 0 , p , 1 /*push*/ ) ; //	<---   (     )(     )(     )[(0)------1]  
			}
		if ( Htail != 0 ) 
			{
				ipull=1;	 
				MoveOneAttachedVagon( 1 , p ,0 ) ;//	<---   [(0)------1](     )(     )(     )
		    }
	}

	if (  VD ==  -1 ) 
	{
		if ( Hhead != 0 ) 	
			{
				ipull=1;
				MoveOneAttachedVagon( 0 , p ,0 ) ;	//     (      )(      )(     )[(0------(1)]    --->
			}
		if ( Htail != 0 ) 
			{
				ipush=1;
			 	MoveOneAttachedVagon( 1 , p , 1 /*push*/) ;//		[0------(1)](     )(     )(     ) 	 --->
			}
	}		


	if ( mp_oe_data.endrail_reached != 0 )  // signaling end rail
	{
		p->StopTheLoco();

	}

	if (	mp_oe_data.error == 1 ) p->StopTheLoco();

	//if ( trigger ==1 ) mDebug.CDebugPut("[EXIT] "  );
    
     /*   // debug only
	if ( ipull == 1){ mDebug.CDebugPut("Pull vagon:%d %d", Hhead, Htail );}
	if ( ipush == 1){ mDebug.CDebugPut("Push vagon:%d %d", Hhead, Htail );}
	*/
}

 void	DebugSave(  CRect rcret );
CRect CVagonList::GetBoundingRectForTrain(CTrainVagon *p   ) 
{
	CRect rc(0,0,0,0);

	return rc;

}


void CVagonList::InvalidateCar(  int endpoint , CTrainVagon *pmaster ,  HWND nhWnd  )
{
	int oldId	= pmaster->GetVagonId();  //  save master vagon ID
	int nID		= pmaster->GetHandle(endpoint);	 // get connected slave ID

	if ( nID == 0 ) 
		{
			mDebug.CDebugPut("InvalidateCar:!no handle to slave found "  );
				mSystem.SystemErrorMessage( 43 ) ;
			return  ; // error
		}

	CTrainVagon *pslave = GetAtId( nID );
	if ( pslave == NULL ) return;

	CRect rc = pslave->GetBoundingRect();
	RECT   r = rc.GetRect();			 
	InvalidateRect( nhWnd ,  &r , FALSE );
	
	if ( pslave->GetHandle(0) == oldId )
		if ( pslave->GetHandle(1) == 0 ) return;
		else InvalidateCar( 1 ,  pslave ,   nhWnd  );

	if ( pslave->GetHandle(1) == oldId )
		if ( pslave->GetHandle(0) == 0 ) return;
		else InvalidateCar( 0 ,  pslave ,   nhWnd  );

	return;
}


CRect CVagonList::InvalidateCar(  int endpoint , CTrainVagon *pmaster ,  CRect rcin )
{
	int oldId	= pmaster->GetVagonId();  //  save master vagon ID
	int nID		= pmaster->GetHandle(endpoint);	 // get connected slave ID

	if ( nID == 0 ) 
		{
			mDebug.CDebugPut("InvalidateCar:!no handle to slave found "  );
			mSystem.SystemErrorMessage( 44 ) ;
			return rcin ; // error
		}

	CTrainVagon *pslave = GetAtId( nID );
	if ( pslave == NULL ) return rcin;

	CRect rc = pslave->GetBoundingRect();
	CRect ur;
	ur.UnionRect(rc,rcin);

	ur.NormalizeRect();
	
	if ( pslave->GetHandle(0) == oldId )
		if ( pslave->GetHandle(1) == 0 ) return ur;
		else return InvalidateCar( 1 ,  pslave ,   ur  );

	if ( pslave->GetHandle(1) == oldId )
		if ( pslave->GetHandle(0) == 0 ) return ur;
		else return  InvalidateCar( 0 ,  pslave ,   ur );

	return ur;
}



void CVagonList::InvalidateSet(CTrainVagon *pmaster ,  HWND nhWnd  ) 
{
	CRect rcret;

	int Hhead = pmaster->GetHandle(0);
	int Htail = pmaster->GetHandle(1);

	CRect rc = pmaster->GetBoundingRect();
	if ( ( Hhead == 0 ) && ( Htail == 0 ))
	{			
	 	// DebugSave(rc  );
		rc.InflateRect(4);
		RECT   r = rc.GetRect();
		InvalidateRect( nhWnd ,  &r , FALSE );	
		return;  //  no vagons
	}

 

	if ( Hhead != 0 )  rcret = InvalidateCar( 0 ,  pmaster ,   rc  );			  
	if ( Htail != 0 )  rcret = InvalidateCar( 1 ,  pmaster ,   rc  );

	  // DebugSave(rcret );
	 rcret.InflateRect(4);
	 RECT   r = rcret.GetRect();	
	 InvalidateRect( nhWnd ,  &r , FALSE );
 

}

 
