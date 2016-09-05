
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


#define N_DEBUG_MODE_VAGONS


extern CList  mList;

/////////////////////////////////////////////////////////////////////////////////////


CList::CList()
{
	mCount = 0;
	mID = 0;
	pCurSeleted = NULL;
	stdRadius = 100;
	stdWide = 10;
}


void CList::SetAppParameters( float bendRadius , int wideTrack )
{
	stdRadius = bendRadius;
	stdWide = wideTrack;
}

void CList::Serialize(CArchive& ar)
{
	int i;
	if ( ar.IsStoring() != 0 )
	{
		ar <<  (DWORD)mCount;
		ar <<  (WORD)mID;
		for ( i =0;i <mCount; i++)
		{
				CRail *p;
				p = mRailPointerTable[i];

				if( p->mtype == ObjBendRail )
				{
					CBendRail *p1 = (CBendRail *)mRailPointerTable[i];
					ar <<  (WORD)ObjBendRail;
					p1->Serialize( ar );
				}
				if( p->mtype == ObjStraightRail )
				{
					CStraightRail *p1 = (CStraightRail *)mRailPointerTable[i];
					ar <<  (WORD)ObjStraightRail;
					p1->Serialize( ar );
				}
				if( p->mtype == ObjRailSwitch )
				{
					CRailSwitch *p1 = (CRailSwitch *)mRailPointerTable[i];
					ar <<  (WORD)ObjRailSwitch;
					p1->Serialize( ar );
				}
				if( p->mtype == ObjRailEnd )
				{
					CEndRail *p1 = (CEndRail *)mRailPointerTable[i];
					ar <<  (WORD)ObjRailEnd;
					p1->Serialize( ar );
				}


		}

	}
	else
	{
	
		DWORD  count=0;

		ar >>  (DWORD)count;
		ar >>  (WORD)mID;

		// mDebug.CDebugPut((char *)"Read %d %d" ,(int)count,(int)mID);

		 mCount =0;

	    WORD  wType; 
		for ( i =0;i < (int)count; i++)
		{
				ar >>  (WORD)wType;
		
					// mDebug.CDebugPut((char *)"Read: %d  " ,(int)wType );
				
				if( wType == ObjBendRail )
				{
					CBendRail *p1 = new CBendRail();
					p1->mtype = ObjBendRail;
					p1->Serialize( ar );
					AddToTheEndStd(  p1)  ;
				}
				if( wType == ObjStraightRail )
				{
					CStraightRail *p1 =  new CStraightRail();
					p1->mtype = ObjStraightRail;
					p1->Serialize( ar );
					AddToTheEndStd(  p1)  ;
				}
				if( wType == ObjRailSwitch )
				{
					CRailSwitch *p1 =  new CRailSwitch() ;
					p1->mtype = ObjRailSwitch;
					p1->Serialize( ar );
					AddToTheEndStd(  p1)  ;
				}
				if( wType == ObjRailEnd )
				{
					CEndRail *p1 = new CEndRail() ;
 					p1->mtype = ObjRailEnd;
					p1->Serialize( ar );
					AddToTheEndStd(  p1)  ;

				}


		}




	}


}


void CList::AddToTheEndStd( CRail *p)
{

if ( mCount < MX_RAIL )  mRailPointerTable[mCount++] = p;
//p->SetStandardData(stdRadius,stdWide);

}


 void CList::AddToTheEnd( CRail *p) 
{
	if ( mCount < MX_RAIL )  mRailPointerTable[mCount++] = p;
	mID +=1 ;
	p->SetRailId( mID) ;
	p->SetStandardData(stdRadius,stdWide);
}

//  to do remove all connectors assosiated with this segment

void CList::Detach( WORD mLastID ) 
{
	DRAWDATASTRUCT *dt;
	CRail *p;

	int i;
	for ( i =0;i <mCount; i++)
	{
		p = mRailPointerTable[i];
		dt = p->GetDrawDataStructPtr();
		if ( dt->ConnID[0] == mLastID ) dt->ConnID[0] = 0;
		if ( dt->ConnID[1] == mLastID ) dt->ConnID[1] = 0;
		if ( dt->ConnID[2] == mLastID ) dt->ConnID[2] = 0;
	}

}

int  CList::RemoveAtTheEnd() 
{
	CRail *p ;
	if ( mCount == 0 ) return 0;
	p =	mRailPointerTable[mCount-1] ;
	mCount -=1 ;
	DRAWDATASTRUCT *dt = p->GetDrawDataStructPtr();
	Detach(dt->ThisID); 
	delete p;
	return 0;
}


int  CList::DeleteObject(CRail *pd) 
{
	CRail *p;
	int i,j=-1;

	if ( pd->GetRailConnId(0) != 0) return 2;
	if ( pd->GetRailConnId(1) != 0) return 3;
	if ( pd->mtype == ObjRailSwitch)
		if ( pd->GetRailConnId(2) != 0) return 4;

	for ( i =0; i <mCount; i++)
	{
		p = mRailPointerTable[i];
		if ( p == pd )
		{
			// detach
			j = i;
			delete p;
			break;
		}
	}


	if ( j >=0 )
	{
	mCount -=1 ;
	for ( i = j;  i < mCount; i++)
		mRailPointerTable[i] = mRailPointerTable[i+1];
	return 1;
	}

  return 0;
}



CRail *CList::GetAtCoordinates( CPoint pt ) 
{
	CRail *p;
	int i;
	for ( i =0;i <mCount; i++)
	{
		p = mRailPointerTable[i];
		DRAWDATASTRUCT *dt = p->GetDrawDataStructPtr();
		if ( PointInsideTheRect( pt , dt->mUpdRect ) )
			return p;
	}
	return NULL;
}


	// CRail  *Indexes[3]; //  fast indexing
void   CList::Index() 
{
 
	int i,j;
	int count = mList.GetCount();
 
	for ( i =0; i < count ;i++)
	{
		 CRail *p  = mList.GetAt( i ) ;
		 WORD ThisId = p->GetRailId();

		for ( j =0; j < count ;j++)
			if ( i != j )
			{
				CRail *pp  = mList.GetAt( j ) ;
				if ( pp->GetRailConnId(0) == ThisId ) pp->SetIndex(0 , p ) ;//   
				if ( pp->GetRailConnId(1) == ThisId ) pp->SetIndex(1 , p ) ;//		 
				if ( pp->GetRailConnId(2) == ThisId ) pp->SetIndex(2 , p ) ;//		 

				if ( pp->GetRailConnId(0) == ThisId ) pp->SetIndex(0, i );
				if ( pp->GetRailConnId(1) == ThisId ) pp->SetIndex(1, i );
				if ( pp->GetRailConnId(2) == ThisId ) pp->SetIndex(2, i );
			}
	}
}

CRail *CList::GetByEndPoint(  CRail *current , int  endpoint   ) 
{

// 	WORD cId = current->GetRailConnId(endpoint);
	WORD ThisId = current->GetRailId();
//	int i;
//	int count = mList.GetCount();
//	CRail *p = current->GetIndex( endpoint );

	//  alte   : 
	int i = current->GetIIndex( endpoint );
    CRail *p  = mList.GetAt( i ) ;

//	mDebug.CDebugPut("*Rail*:[%X] [%X] [%X] [%X]  ", (int)current  ,(int)current->GetIndex(0)  ,(int)current->GetIndex(1)  ,(int)current->GetIndex(2) );

	if ( p->GetRailConnId(0) == ThisId ) p->mode2 = 0;
	if ( p->GetRailConnId(1) == ThisId ) p->mode2 = 1;
	if ( p->GetRailConnId(2) == ThisId ) p->mode2 = 2;
	
//	mDebug.CDebugPut("*Rail*:[%X] mode:[%d]   ", (int)p  ,p->mode2   );

	return p;

 /*
	for ( i =0; i < count ;i++)
	{
		 CRail *p  = mList.GetAt( i ) ;
		 if ( p->GetRailId() == cId )  
		 {
			if ( p->GetRailConnId(0) == ThisId ) p->mode2 = 0;
			if ( p->GetRailConnId(1) == ThisId ) p->mode2 = 1;
			if ( p->GetRailConnId(2) == ThisId ) p->mode2 = 2;
			return p;
		 }
	}
   return NULL;
   */
}

float CList::GetMaxVariation(int *ii)
{
  float fmax = 0;
  int icheck =0;

  CRail *p,*pi;
  int i,j ;
  for ( i =0;i < mCount; i++)
	{
		p = mRailPointerTable[i];

		for ( j =0;j < mCount; j++)
			if ( i != j )
			{
				pi = mRailPointerTable[j];
			 
				if ( p->GetRailConnId(0) == pi->GetRailId() )
				{

					if ( pi->GetRailConnId(0) == p->GetRailId())
					{  // compute point0  &  point(0)
						icheck+=1;
						float d = GetDistance( p->GetPoint(0) ,  pi->GetPoint(0) );
						if ( d > fmax ) fmax = d;

					}
					if ( pi->GetRailConnId(1) == p->GetRailId())
					{  // compute point0  &  point(1)
						icheck+=1;
						float d = GetDistance( p->GetPoint(0) ,  pi->GetPoint(1) );
						if ( d > fmax ) fmax = d;
					}
					if ( pi->mtype == ObjRailSwitch)
						if ( pi->GetRailConnId(2) == p->GetRailId())
						{  // compute point0  &  point(1)
						icheck+=1;
						float d = GetDistance( p->GetPoint(0) ,  pi->GetPoint(2) );
						if ( d > fmax ) fmax = d;
						}

				}

				if ( p->GetRailConnId(1) == pi->GetRailId() )
				{
 
					if ( pi->GetRailConnId(0) == p->GetRailId())
					{  // compute point1  &  point(0)
						icheck+=1;
						float d = GetDistance( p->GetPoint(1) ,  pi->GetPoint(0) );
						if ( d > fmax ) fmax = d;


					}
					if ( pi->GetRailConnId(1) == p->GetRailId())
					{  // compute point1  &  point(1)
						icheck+=1;
						float d = GetDistance( p->GetPoint(1) ,  pi->GetPoint(1) );
						if ( d > fmax ) fmax = d;
					}

					if ( pi->mtype == ObjRailSwitch)
						if ( pi->GetRailConnId(2) == p->GetRailId())
						{  // compute point0  &  point(1)
						icheck+=1;
						float d = GetDistance( p->GetPoint(1) ,  pi->GetPoint(2) );
						if ( d > fmax ) fmax = d;
						}

				}

                if ( p->mtype == ObjRailSwitch)
					if ( p->GetRailConnId(2) == pi->GetRailId() )
					{

						if ( pi->GetRailConnId(0) == p->GetRailId())
						{  // compute point1  &  point(0)
						icheck+=1;
						float d = GetDistance( p->GetPoint(2) ,  pi->GetPoint(0) );
						if ( d > fmax ) fmax = d;


						}

						if ( pi->GetRailConnId(1) == p->GetRailId())
						{  // compute point1  &  point(1)
						icheck+=1;
						float d = GetDistance( p->GetPoint(2) ,  pi->GetPoint(1) );
						if ( d > fmax ) fmax = d;
						}
						
						if ( pi->mtype == ObjRailSwitch)
							if ( pi->GetRailConnId(2) == p->GetRailId())
							{  // compute point0  &  point(1)
							icheck+=1;
							float d = GetDistance( p->GetPoint(2) ,  pi->GetPoint(2) );
							if ( d > fmax ) fmax = d;
							}


					}

			}

 
	}

  *ii = icheck;



  return fmax;
}

 CRail *CList::GetAt( int index) 
{
	CRail *p;
	p = mRailPointerTable[index];
	return p;
}

 int  CList::GetCount() 
{
   return mCount;
}

void  CList::SetActiveObjPtr(CRail *p) 
{
	pCurSeleted = p;
}

 CRail *CList::GetActiveObjPtr() 
{
	return pCurSeleted ;
}

 void CList::CheckPointToSnap( CRail *p1,CRail *p2, int pointindex1,int pointindex2)
 {
	
	 // mDebug.CDebugPut("Check objects(0) ID to swap- %d %d ",p1->GetRailId()  , p1->GetRailId() );		
	 // mDebug.CDebugPut("Check objects(1) IDX to swap- %d %d ",pointindex1   , pointindex2  );

	 if ( GetDistance ( p2->GetPoint(pointindex2), p1->GetPoint(pointindex1)  ) < 10 ) // asuming match
		 {
			 CRect rc;
			 rc.top = (int)p1->GetPoint(pointindex1).y;
			 rc.left = (int)p1->GetPoint(pointindex1).x;
			 rc.bottom = (int)p2->GetPoint(pointindex2).y;
			 rc.right = (int)p2->GetPoint(pointindex2).x;
			 rc.NormalizeRect();
			 rc.InflateRect(2);

		// mDebug.CDebugPut("  match %d %d ",pointindex1  , pointindex2 );	
		// mDebug.CDebugPut((char *)"  march   = %d   %d", (int)p1->mtype , (int)p2->mtype);

			 SnapTwoObjects( p1,p2,rc);
		 }
 }


 void CList::AutoSnap(CRail *p2)
{
	
	int i;CRail *p1;
	for ( i =0;i < mCount  ; i++)
	{
		p1 = mRailPointerTable[i];

		if ( p1 == p2 ) continue;

		CheckPointToSnap( p1, p2, 0,0);
		CheckPointToSnap( p1, p2, 1,0);
		CheckPointToSnap( p1, p2, 0,1);
		CheckPointToSnap( p1, p2,1,1);

		if ( p2->mtype == ObjRailSwitch )
              {
                CheckPointToSnap( p1, p2,1,2);
                CheckPointToSnap( p1, p2,0,2);
                if ( p1->mtype == ObjRailSwitch )
                     CheckPointToSnap( p1, p2,2,2);
				//continue;
              }


		if ( p1->mtype == ObjRailSwitch )
		{
			CheckPointToSnap( p1, p2,2,0);
			CheckPointToSnap( p1, p2,2,1);
			//if ( p2->mtype == ObjRailSwitch )
			//	CheckPointToSnap( p1, p2,2,2);
		}
					 
	}
}


 
int CList::CheckConnectedPointToSnap( CRail *p1,CRail *p2, int pointindex1,int pointindex2)
 {
	if ( GetDistance ( p2->GetPoint(pointindex2), p1->GetPoint(pointindex1)  ) < 1  ) // asuming match
		 {
				WORD  wID1		= p1->GetRailId();
				WORD  wID2		= p2->GetRailId();
				p1->SetRailConnId( pointindex1 , wID2 );
				p2->SetRailConnId( pointindex2 , wID1 );

				 	// mDebug.CDebugPut("Glue connected objects - match %d %d ",pointindex1  , pointindex2 );		
				return 0;
		 }
	return 1;
 }




int CList::GlueConnectedObjects( CRail * p1,CRail *p2,CRect rc) 
{
	int success=1;

		// mDebug.CDebugPut("Glue connected objects - checking  "  );	



		if ( CheckConnectedPointToSnap( p1, p2, 0,0) == 0 ) success=0;
		if (CheckConnectedPointToSnap( p1, p2, 1,0)== 0 ) success=0;
		if (CheckConnectedPointToSnap( p1, p2, 0,1)== 0 ) success=0;
		if (CheckConnectedPointToSnap( p1, p2,1,1)== 0 ) success=0;
		if ( p1->mtype == ObjRailSwitch )
		{
			if (CheckConnectedPointToSnap( p1, p2,2,0)== 0 ) success=0;
			if (CheckConnectedPointToSnap( p1, p2,2,1)== 0 ) success=0;
			if (CheckConnectedPointToSnap( p1, p2,2,2)== 0 ) success=0;
		}
 
	return success;
}


int  CList::ProcessObjects( CRect rc , int mode) 
{
 int i ;
	 int ipoint=0;
	 int iPos[8];
	 int iEp[8];

	 // looking for first match 

	 rc.NormalizeRect();

	 for ( i =0;i < mCount;i++ )
	{
		CRail *p = mRailPointerTable[i ];
		//   if first point inside rc
		//   if second point inside rc
		CFPoint cp;
		cp = p->GetPoint( 0 ) ;
		//mDebug.CDebugPut("0:Points:%d %d {%d,%d} ",i, 0,(int)cp.x,(int)cp.y );
		if ( PointInsideTheRect( cp , rc ) )
		 {
			// mDebug.CDebugPut("In: {%d,%d} " , (int)cp.x,(int)cp.y );
			 if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 0;  ipoint+=1 ; 
			 }
		 }
		cp = p->GetPoint( 1 ) ;
		//mDebug.CDebugPut("1:Points:%d %d {%d,%d} ",i, 1,(int)cp.x,(int)cp.y );
		if ( PointInsideTheRect( cp , rc ) )
		{
			//mDebug.CDebugPut("In: {%d,%d} ",(int)cp.x,(int)cp.y );
			if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 1;  ipoint+=1 ; 
			 }
		}
		cp = p->GetPoint( 2 ) ;
		//mDebug.CDebugPut("2:Points:%d %d {%d,%d} ",i, 2,(int)cp.x,(int)cp.y );
		if ( PointInsideTheRect( cp , rc ) )
		{
			//mDebug.CDebugPut("In: {%d,%d} ",(int)cp.x,(int)cp.y );
			if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 2;  ipoint+=1 ; 
			 }
		}

		if ( ipoint >= 7 ) break;
	}   


	if ( ipoint == 1 )  
	{
        CRail *p1 = mRailPointerTable[iPos[0]];
		return  HitSingleObject(  p1 );
	}

	if ( ipoint == 2 )  
	 {
		// mDebug.CDebugPut("Two points "   );

		 CRail *p1 = mRailPointerTable[iPos[0]];
		 CRail *p2 = mRailPointerTable[iPos[1]];
		// trivial check for endpoints marked as selected
		 //  if  connected together display  the link status

		 int  id1 = p1->GetRailId();
		 int  id2 = p2->GetRailId();

		 int  ic1 =-1;
		 int  ic2 =-1;

		 if ( p2->GetRailConnId(0) == id1 )
		 {
			 if ( p1->GetRailConnId(0) ==  id2 ){ ic1 = 0; ic2=0;}
			 if ( p1->GetRailConnId(1) ==  id2 ){ ic1 = 1; ic2=0;}
			 if ( p1->GetRailConnId(2) ==  id2 ){ ic1 = 2; ic2=0;}
		 }
		 else if ( p2->GetRailConnId(1) == id1 )
		 {
			 if ( p1->GetRailConnId(0) ==  id2 ){ic1 = 0; ic2=1;}
			 if ( p1->GetRailConnId(1) ==  id2 ){ic1 = 1; ic2=1;}
			 if ( p1->GetRailConnId(2) ==  id2 ){ic1 = 2; ic2=1;}
		 }
		 else if ( p2->GetRailConnId(2) == id1 )
		 {
			 if ( p1->GetRailConnId(0) ==  id2 ){ic1 = 0; ic2=2;}
			 if ( p1->GetRailConnId(1) ==  id2 ){ic1 = 1; ic2=2;}
			 if ( p1->GetRailConnId(2) ==  id2 ){ic1 = 2; ic2=2;}
		 }


		 if ( ic1 >= 0)
		 {
			// mDebug.CDebugPut("Connected  %d<-->%d",ic1,ic2);
			// mDebug.CDebugPut("Connected point 1:(%6.2f,%6.2f)", p1->GetPoint(ic1).x , p1->GetPoint(ic1).y)  ;
			// mDebug.CDebugPut("Connected point 2:(%6.2f,%6.2f)", p2->GetPoint(ic2).x , p2->GetPoint(ic2).y)  ;
			 // fix the connection
			if ( p1->mtype == ObjStraightRail )  p1->SetPoint( ic1 , p2->GetPoint(ic2));
			if ( p2->mtype == ObjStraightRail )  p2->SetPoint( ic2 , p1->GetPoint(ic1));
		 }
		 return 0;
	 }


 return 1;   // error
}

int	CList::SnapObjects(CRect  rc1 ,CRect rc2 )  
{
 //mDebug.CDebugPut("Snapping");
	 int i ;
	 int ipoint=0;
	 int iPos[8];
	 int iEp[8];

	 // looking for first match 

	// rc.NormalizeRect();

	for ( i =0;i < mCount;i++ )
	{
		CRail *p = mRailPointerTable[i ];
		//   if first point inside rc
		//   if second point inside rc
		CFPoint cp;
		cp = p->GetPoint( 0 ) ;
		//mDebug.CDebugPut("0:Points:%d %d {%d,%d} ",i, 0,(int)cp.x,(int)cp.y );
		if ( PointInsideTheRect( cp , rc1 ) )
		 {
			// mDebug.CDebugPut("In: {%d,%d} " , (int)cp.x,(int)cp.y );
			 if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 0;  ipoint+=1 ; 
			 }
		 }


		if ( PointInsideTheRect( cp , rc2 ) )
		 {
			// mDebug.CDebugPut("In: {%d,%d} " , (int)cp.x,(int)cp.y );
			 if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 0;  ipoint+=1 ; 
			 }
		 }


		cp = p->GetPoint( 1 ) ;
		//mDebug.CDebugPut("1:Points:%d %d {%d,%d} ",i, 1,(int)cp.x,(int)cp.y );
		if ( PointInsideTheRect( cp , rc1 ) )
		{
			//mDebug.CDebugPut("In: {%d,%d} ",(int)cp.x,(int)cp.y );
			if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 1;  ipoint+=1 ; 
			 }
		}
		if ( PointInsideTheRect( cp , rc2 ) )
		{
			//mDebug.CDebugPut("In: {%d,%d} ",(int)cp.x,(int)cp.y );
			if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 1;  ipoint+=1 ; 
			 }
		}


		cp = p->GetPoint( 2 ) ;
		//mDebug.CDebugPut("2:Points:%d %d {%d,%d} ",i, 2,(int)cp.x,(int)cp.y );
		if ( PointInsideTheRect( cp , rc1 ) )
		{
			mDebug.CDebugPut("In: {%d,%d} ",(int)cp.x,(int)cp.y );
			if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 2;  ipoint+=1 ; 
			 }
		}
		if ( PointInsideTheRect( cp , rc2 ) )
		{
			mDebug.CDebugPut("In: {%d,%d} ",(int)cp.x,(int)cp.y );
			if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 2;  ipoint+=1 ; 
			 }
		}


		if ( ipoint >= 7 ) break;
	}   


if ( ipoint == 2 )  
	 {

mDebug.CDebugPut("SANP2O: "  );

		 CRect rc = rc1;
		 rc.UnionRect(rc2,rc1);

	 CRail *p1 = mRailPointerTable[iPos[0]];
	CRail *p2 = mRailPointerTable[iPos[1]];
		// trivial check for endpoints marked as selected
		if (p1->GetRailConnId(iEp[0]) != 0) return 6;  // already connected simple error-  
		if (p2->GetRailConnId(iEp[1]) != 0) return 6;  // already connected simple error-  
		// check which rail is free to move - priority on the older
		if (   (p2->GetRailConnId(0) != 0) || (p2->GetRailConnId(1)  != 0) || (p2->GetRailConnId(2) != 0) )
		{
			p1 = mRailPointerTable[iPos[1]];  // swap and test
			p2 = mRailPointerTable[iPos[0]];
			if (   (p2->GetRailConnId(0) != 0) || (p2->GetRailConnId(1)  != 0) || (p2->GetRailConnId(2) != 0) )
			{
				// need to revert changes
				CRail *p1 = mRailPointerTable[iPos[0]];
				CRail *p2 = mRailPointerTable[iPos[1]];	
				return GlueTwoFixedObjects(p1,p2,rc);
			}
		}
		// OK to go on this path
		return SnapTwoObjects( p1,p2,rc);

}




return 0;
}

 int  CList::SnapObjects( CRect rc)
 {
	 //mDebug.CDebugPut("Snapping");
	 int i ;
	 int ipoint=0;
	 int iPos[8];
	 int iEp[8];

	 // looking for first match 

	 rc.NormalizeRect();

	for ( i =0;i < mCount;i++ )
	{
		CRail *p = mRailPointerTable[i ];
		//   if first point inside rc
		//   if second point inside rc
		CFPoint cp;
		cp = p->GetPoint( 0 ) ;
		//mDebug.CDebugPut("0:Points:%d %d {%d,%d} ",i, 0,(int)cp.x,(int)cp.y );
		if ( PointInsideTheRect( cp , rc ) )
		 {
			// mDebug.CDebugPut("In: {%d,%d} " , (int)cp.x,(int)cp.y );
			 if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 0;  ipoint+=1 ; 
			 }
		 }
		cp = p->GetPoint( 1 ) ;
		//mDebug.CDebugPut("1:Points:%d %d {%d,%d} ",i, 1,(int)cp.x,(int)cp.y );
		if ( PointInsideTheRect( cp , rc ) )
		{
			//mDebug.CDebugPut("In: {%d,%d} ",(int)cp.x,(int)cp.y );
			if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 1;  ipoint+=1 ; 
			 }
		}
		cp = p->GetPoint( 2 ) ;
		//mDebug.CDebugPut("2:Points:%d %d {%d,%d} ",i, 2,(int)cp.x,(int)cp.y );
		if ( PointInsideTheRect( cp , rc ) )
		{
			//mDebug.CDebugPut("In: {%d,%d} ",(int)cp.x,(int)cp.y );
			if ( ipoint < 8 )
			 {
				iPos[ipoint]= i;  iEp[ipoint]= 2;  ipoint+=1 ; 
			 }
		}

		if ( ipoint >= 7 ) break;
	}   


	if ( ipoint == 1 )  
	{
        CRail *p1 = mRailPointerTable[iPos[0]];
		return  HitSingleObject(  p1 );
	}

	if ( ipoint == 2 )  
	 {
		// mDebug.CDebugPut("Two points "   );


		 CRail *p1 = mRailPointerTable[iPos[0]];
		 CRail *p2 = mRailPointerTable[iPos[1]];
		// trivial check for endpoints marked as selected
		if (p1->GetRailConnId(iEp[0]) != 0) return 6;  // already connected simple error-  
		if (p2->GetRailConnId(iEp[1]) != 0) return 6;  // already connected simple error-  
		// check which rail is free to move - priority on the older
		if (   (p2->GetRailConnId(0) != 0) || (p2->GetRailConnId(1)  != 0) || (p2->GetRailConnId(2) != 0) )
		{
			p1 = mRailPointerTable[iPos[1]];  // swap and test
			p2 = mRailPointerTable[iPos[0]];
			if (   (p2->GetRailConnId(0) != 0) || (p2->GetRailConnId(1)  != 0) || (p2->GetRailConnId(2) != 0) )
			{
				// need to revert changes
				CRail *p1 = mRailPointerTable[iPos[0]];
				CRail *p2 = mRailPointerTable[iPos[1]];	
				return GlueTwoFixedObjects(p1,p2,rc);
			}
		}
		// OK to go on this path
		return SnapTwoObjects( p1,p2,rc);
	 }

	 return 1;   // error
 } 
 
 //  sanp  of one free object  to another fixed or free

int CList::SnapTwoObjects(CRail * p1,CRail *p2,CRect rc) 
{

	// mDebug.CDebugPut((char *)"   ->    = %d   %d", (int)p1->mtype , (int)p2->mtype);


		if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjStraightRail ))
		 {

			// mDebug.CDebugPut((char *)" S -> S  = %d   %d", (int)p1->mtype , (int)p2->mtype);

			 return SnapTwoStrightObjects(  p1, p2,  rc) ;
		 }

		if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjBendRail ))
		 {
			 return SnapBendToStrightObjects(  p1, p2, rc) ;
		 }

		 if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjRailSwitch ))
		 {

			//  mDebug.CDebugPut((char *)" S -> SW  = %d   %d", (int)p1->mtype , (int)p2->mtype);

			 return SnapSwitchToStrightObjects(  p1, p2,rc) ;
		 }

		 if (( p1->mtype == ObjBendRail ) && ( p2->mtype == ObjStraightRail ))
		 {
			return  SnapStrightToBendObjects(  p1, p2,  rc);
		 }

		 if (( p1->mtype == ObjBendRail ) && ( p2->mtype == ObjRailSwitch ))
		 {
			return  SnapSwitchToBendObjects(  p1, p2,  rc);
		 }
 
		 if (( p1->mtype == ObjBendRail ) && ( p2->mtype == ObjBendRail ))
		 {
			return  SnapTwoBendObjects(  p1, p2,  rc);
		 }

		 if (( p1->mtype == ObjRailSwitch ) && ( p2->mtype == ObjStraightRail ))
		 {
			
			 // mDebug.CDebugPut((char *)" SW -> S  = %d   %d", (int)p1->mtype , (int)p2->mtype);

			 return  SnapStrightToSwitchObjects(  p1, p2,  rc);
		 }

		 if (( p1->mtype == ObjRailSwitch ) && ( p2->mtype == ObjBendRail ))
		 {
			
			 //mDebug.CDebugPut((char *)" SW -> B = %d   %d", (int)p1->mtype , (int)p2->mtype);

			 return  SnapBendToSwitchObjects(  p1, p2,  rc);
		 }

		 if (( p1->mtype == ObjRailSwitch ) && ( p2->mtype == ObjRailSwitch ))
		 {
			return  SnapTwoSwitchObjects(  p1, p2,  rc);
		 }

		if (( p1->mtype == ObjRailEnd ) && ( p2->mtype == ObjStraightRail ))
		 {
			return  SnapStrightToEndObjects(  p1, p2,  rc);
		 }

		if (( p1->mtype == ObjRailEnd ) && ( p2->mtype == ObjBendRail ))
		 {
			return  SnapBendToEndObjects(  p1, p2,  rc);
		 }	
	 
		if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjRailEnd ))
		 {
			return  SnapEndToStrightObjects(  p1, p2,  rc);
		 }

		if (( p1->mtype == ObjBendRail ) && ( p2->mtype == ObjRailEnd ))
		 {
			return  SnapEndToBendObjects(  p1, p2,  rc);
		 }

		 return 2; //error
}

// -----------------------------------------------------------------------------------------------
// We are here because :
// two objects are selected - both are fixed to the other objects
// option one - extend straight line to connect  - if at least one straight exists  and angles < 2
// option two - use bend to connect two object - if at least one straight can be extended / shorten  
// option three - if endpoint matches  then simply adjust close endpoints
// ------------------------------------------------------------------------------------------------

int CList::GlueTwoFixedObjects(CRail * p1,CRail *p2,CRect rc)
{
	int iret=1;

	// what if straight angles OK ?
	if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjStraightRail ))  //  ====== ======
		{
		//if ( GlueTwoStrightObjects(  p1, p2,  rc) != 0 )   // if angles match ( < 2 deg - connect
		//		return GlueTwoFixedObjectsWithBend( p1, p2, rc);  // else  add bend and connect

			if ( GlueConnectedObjects(  p1, p2,  rc) != 0 )
				 if ( GlueTwoFixedObjectsWithStright( p1, p2, rc))   
			        return GlueTwoFixedObjectsWithBend( p1, p2, rc);  

		}

	if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjRailSwitch ))   // ===== ==//=
		{
			if ( GlueConnectedObjects(  p1, p2,  rc) != 0 )    // if any of the pair points  are in distance less then 1 pixel then it glues automatically 
				 if ( GlueTwoFixedObjectsWithStright( p1, p2, rc))  //  if the angles  match then glue with stright 
			        return GlueTwoFixedObjectsWithBend( p1, p2, rc);   // othervise use bend to connect
		}

	if (( p1->mtype == ObjRailSwitch ) && ( p2->mtype == ObjStraightRail ))   //  ==//=  =====
		{
			CRail * p= p1; p1 = p2; p2 = p;
			if ( GlueConnectedObjects(  p1, p2,  rc) != 0 )
				 if ( GlueTwoFixedObjectsWithStright( p1, p2, rc))   
			        return GlueTwoFixedObjectsWithBend( p1, p2, rc);   
		}

	if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjBendRail ))
		{
			if ( GlueStrightToBendObjects(  p1, p2,  rc) != 0 )   //  try to extend straight to connect - if  angle < 2 deg
			    return GlueTwoFixedObjectsWithBend( p1, p2, rc);    //  otherwice use bend
		}

	if (( p1->mtype == ObjBendRail ) && ( p2->mtype == ObjStraightRail ))
		{
			CRail * p= p1; p1 = p2; p2 = p;								// as above
			if ( GlueStrightToBendObjects(  p1, p2,  rc) != 0 ) 
			    return GlueTwoFixedObjectsWithBend( p1, p2, rc);    
		}


	if (( p1->mtype == ObjBendRail ) && ( p2->mtype == ObjBendRail ))
		{   
			if ( GlueConnectedObjects(  p1, p2,  rc) != 0 ) // if any of the pair points  are in distance less then 1 pixel then it glues automatically 
				 if ( GlueTwoFixedObjectsWithStright( p1, p2, rc))   // try with stright
			        return GlueTwoFixedObjectsWithBend( p1, p2, rc);    // try with bend
		}


	if (( p1->mtype == ObjRailSwitch ) && ( p2->mtype == ObjRailSwitch ))
		{   
			if ( GlueConnectedObjects(  p1, p2,  rc) != 0 )
				 if ( GlueTwoFixedObjectsWithStright( p1, p2, rc))   
			        return GlueTwoFixedObjectsWithBend( p1, p2, rc);  
		}

	if (( p1->mtype == ObjRailSwitch ) && ( p2->mtype == ObjBendRail ))
		{  

			if ( GlueConnectedObjects(  p1, p2,  rc) != 0 )
				 if ( GlueTwoFixedObjectsWithStright( p1, p2, rc))   
			        return GlueTwoFixedObjectsWithBend( p1, p2, rc);   
	     }

	if (( p1->mtype == ObjBendRail ) && ( p2->mtype == ObjRailSwitch ))
		{  
			CRail * p= p1; p1 = p2; p2 = p;
			if ( GlueConnectedObjects(  p1, p2,  rc) != 0 )
				 if ( GlueTwoFixedObjectsWithStright( p1, p2, rc))   
			        return GlueTwoFixedObjectsWithBend( p1, p2, rc);   
	     }

	return 8;
}

int CList::GlueTwoFixedObjectsWithBend(CRail * p1,CRail *p2,CRect rc)
{
    int bn;

	//mDebug.CDebugPut("GlueTwoFixedObjectsWithBend !!!!!!!!!!!! "  );		
	
	if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjStraightRail )   )
	{		 
		bn = GetAngleTwoStrightObjects(  p1 , p2 , rc) ;

		if ( bn == 2) return 2;

		CFPoint stp1	= p1->GetPoint( 0 ) ;
		CFPoint enp2	= p2->GetPoint( 0 ) ;

		if ( PointInsideTheRect( p1->GetPoint( 1 ) , rc ) ) stp1	= p1->GetPoint( 1 ) ;
		if ( PointInsideTheRect( p2->GetPoint( 1 ) , rc ) ) enp2	= p2->GetPoint( 1 ) ;

		CBendRail *p =  new CBendRail( stp1   ,enp2 );
		if (   bn == 0  ) p->SetObject( 0 , 100 , 0.0 , 45.0 ) ;  // left
		else  p->SetObject( 1 , 100 , 0.0 , -45.0 ) ;  // right
		AddToTheEnd( p);

		bn = ConnectObjectsWithBend(  p1, p2 ,p ,rc) ;
		if ( bn != 0 )
		{
			RemoveAtTheEnd();		
			mDebug.CDebugErrorMessage( bn );
		}
		return bn;
	}

	if (( p1->mtype == ObjBendRail ) && ( p2->mtype == ObjBendRail )   )
	{		 
		bn = GetAngleTwoBendObjects(  p1 , p2 , rc) ;

		if ( bn == 2) return 2;
		CFPoint stp1	= p1->GetPoint( 0 ) ;
		CFPoint enp2	= p2->GetPoint( 0 ) ;

		if ( PointInsideTheRect( p1->GetPoint( 1 ) , rc ) ) stp1	= p1->GetPoint( 1 ) ;
		if ( PointInsideTheRect( p2->GetPoint( 1 ) , rc ) ) enp2	= p2->GetPoint( 1 ) ;

		CBendRail *p =  new CBendRail( stp1   ,enp2 );
		if (   bn == 0  ) p->SetObject( 0 , 100 , 0.0 , 45.0 ) ;  // left
		else  p->SetObject( 1 , 100 , 0.0 , -45.0 ) ;  // right
		AddToTheEnd( p);

		bn = ConnectObjectsWithBend(  p1, p2 ,p ,rc) ;
		if ( bn != 0 )
		{
			RemoveAtTheEnd();		
			mDebug.CDebugErrorMessage( bn );
		}
        return bn;
	}

	if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjBendRail )   )
	{
		bn = GetAngleStrightAndBendObjects(  p1 , p2 , rc) ;

		if ( bn == 2) return 3;

		CFPoint stp1	= p1->GetPoint( 0 ) ;
		CFPoint enp2	= p2->GetPoint( 0 ) ;

		if ( PointInsideTheRect( p1->GetPoint( 1 ) , rc ) ) stp1	= p1->GetPoint( 1 ) ;
		if ( PointInsideTheRect( p2->GetPoint( 1 ) , rc ) ) enp2	= p2->GetPoint( 1 ) ;

		CBendRail *p =  new CBendRail( stp1   ,enp2 );
		if (   bn == 0  ) p->SetObject( 0 , 100 , 0.0 , 45.0 ) ;  // left
		else  p->SetObject( 1 , 100 , 0.0 , -45.0 ) ;  // right
		AddToTheEnd( p);

		bn = ConnectObjectsWithBend(  p1, p2 ,p ,rc) ;
		if ( bn != 0 )
		{
			RemoveAtTheEnd();		
			mDebug.CDebugErrorMessage( bn );
		}
		return bn;
	}


	if (( p1->mtype == ObjRailSwitch ) && ( p2->mtype == ObjRailSwitch )   )
	{		 
		bn = GetAngleSwitchtAndSwitchObjects(  p1 , p2 , rc) ;

		if ( bn == 2) return 2;

		//if ( PointInsideTheRect( p1->GetPoint( 2 ) , rc ) )  return 4;  // do not serve bend endpoint now
		//if ( PointInsideTheRect( p2->GetPoint( 2 ) , rc ) )  return 4;  // do not serve bend endpoint now

		CFPoint stp1	= p1->GetPoint( 0 ) ;
		CFPoint enp2	= p2->GetPoint( 0 ) ;

		if ( PointInsideTheRect( p1->GetPoint( 1 ) , rc ) ) stp1	= p1->GetPoint( 1 ) ;
		if ( PointInsideTheRect( p2->GetPoint( 1 ) , rc ) ) enp2	= p2->GetPoint( 1 ) ;

		CBendRail *p =  new CBendRail( stp1   ,enp2 );
		if (   bn == 0  ) p->SetObject( 0 , 100 , 0.0 , 45.0 ) ;  // left
		else  p->SetObject( 1 , 100 , 0.0 , -45.0 ) ;  // right
		AddToTheEnd( p);

		bn = ConnectObjectsWithBend(  p1, p2 ,p ,rc) ;
		if ( bn != 0 )
		{
			RemoveAtTheEnd();		
			mDebug.CDebugErrorMessage( bn );
		}
		return bn;
	}


	if (( p1->mtype == ObjRailSwitch ) && ( p2->mtype == ObjBendRail )   )
	{	
		bn = GetAngleSwitchtAndBendObjects(  p1 , p2 , rc) ;

		if ( bn == 2) return 3;

		CFPoint stp1	= p1->GetPoint( 0 ) ;
		CFPoint enp2	= p2->GetPoint( 0 ) ;

		if ( PointInsideTheRect( p1->GetPoint( 1 ) , rc ) ) stp1	= p1->GetPoint( 1 ) ;
		if ( PointInsideTheRect( p2->GetPoint( 1 ) , rc ) ) enp2	= p2->GetPoint( 1 ) ;

		CBendRail *p =  new CBendRail( stp1   ,enp2 );
		if (   bn == 0  ) p->SetObject( 0 , 100 , 0.0 , 45.0 ) ;  // left
		else  p->SetObject( 1 , 100 , 0.0 , -45.0 ) ;  // right
		AddToTheEnd( p);

		bn = ConnectObjectsWithBend(  p1, p2 ,p ,rc) ;
		if ( bn != 0 )
		{
			RemoveAtTheEnd();		
			mDebug.CDebugErrorMessage( bn );
		}
		return bn;
	}


	if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjRailSwitch )   )
	{	
		bn = GetAngleStrightAndSwitchObjects(  p1 , p2 , rc) ;

	//	mDebug.CDebugPut("ObjStraightRail->ObjRailSwitch %d " ,bn );	

		if ( bn == 2) return 3;


		CFPoint stp1	= p1->GetPoint( 0 ) ;
		CFPoint enp2	= p2->GetPoint( 0 ) ;

		if ( PointInsideTheRect( p1->GetPoint( 1 ) , rc ) ) stp1	= p1->GetPoint( 1 ) ;
		if ( PointInsideTheRect( p2->GetPoint( 1 ) , rc ) ) enp2	= p2->GetPoint( 1 ) ;

		CBendRail *p =  new CBendRail( stp1   ,enp2 );
		if (   bn == 0  ) p->SetObject( 0 , 100 , 0.0 , 45.0 ) ;  // left
		else  p->SetObject( 1 , 100 , 0.0 , -45.0 ) ;  // right
		AddToTheEnd( p);

		bn = ConnectObjectsWithBend(  p1, p2 ,p ,rc) ;
  		if ( bn != 0 )
		{
			RemoveAtTheEnd();		
			mDebug.CDebugErrorMessage( bn );
		}
		return bn;
	}


	return 1;
}


int CList::GlueTwoFixedObjectsWithStright( CRail * p1,CRail *p2,CRect rc)
{

	int bn;

	//mDebug.CDebugPut("GlueTwoFixedObjectsWithStraingt !!!!!!!!!!!! "  );		
	
	if (( p1->mtype == ObjBendRail ) && ( p2->mtype == ObjBendRail )   )
	{		 
		bn = GetAngleTwoBendObjects(  p1 , p2 , rc) ;
//mDebug.CDebugPut("GlueTwoFixedObjectsWithStraingt ObjBendRail->ObjBendRail [%d] ",bn  );	
		
	if ( bn != 2 ) return 2;

	//	if ( GetDistanceBtwTwoLines( p1->GetPoint(0) ,p1->GetPoint(1) , 
	//		p2->GetPoint(0) ,p2->GetPoint(1) ) > 4 ) return 3;


		CStraightRail *p =  new CStraightRail(p1->GetPoint( 0 ) ,p2->GetPoint( 0 ));
		AddToTheEnd( p);
		return SnapTwoObjectsWithStright(  p1, p2,  p,  rc); // set the straight connection accordingly
	}


	if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjBendRail )   )
	{		 
		bn = GetAngleStrightAndBendObjects(  p1 , p2 , rc) ;
//mDebug.CDebugPut("GlueTwoFixedObjectsWithStraingt ObjStraightRail->ObjBendRail [%d] ",bn  );	

		if ( bn != 2 ) return 2;

		if ( p1->mtype == ObjRailSwitch) 
			if ( PointInsideTheRect( p1->GetPoint( 2 ) , rc ) )  return 4;// do not serve bend endpoint now

		CStraightRail *p =  new CStraightRail(p1->GetPoint( 0 ) ,p2->GetPoint( 0 ));
		AddToTheEnd( p);
		return SnapTwoObjectsWithStright(  p1, p2,  p,  rc); // set the straight connection accordingly
	}


	if (( p1->mtype == ObjRailSwitch ) && ( p2->mtype == ObjBendRail )   )
	{		 
		bn = GetAngleSwitchtAndBendObjects(  p1 , p2 , rc) ;

	//	mDebug.CDebugPut("GlueTwoFixedObjectsWithStraingt ObjRailSwitch->ObjBendRail [%d] ",bn  );	

		if ( bn != 2 ) return 2;

		CStraightRail *p =  new CStraightRail(p1->GetPoint( 0 ) ,p2->GetPoint( 0 ));
		AddToTheEnd( p);
		return SnapTwoObjectsWithStright(  p1, p2,  p,  rc); // set the straight connection accordingly
	}


 
	if (( p1->mtype == ObjRailSwitch ) && ( p2->mtype == ObjRailSwitch )   )
	{		 
		bn = GetAngleTwoStrightObjects(  p1 , p2 , rc) ;

		if ( bn != 2 ) return 2;

	 //	if ( GetDistanceBtwTwoLines( p1->GetPoint(0) ,p1->GetPoint(1) , 
	 //		p2->GetPoint(0) ,p2->GetPoint(1) ) > 4 ) return 3;

		if ( PointInsideTheRect( p1->GetPoint( 2 ) , rc ) )  return 4; // do not serve bend endpoint now

		if ( PointInsideTheRect( p2->GetPoint( 2 ) , rc ) )  return 4; // do not serve bend endpoint now

		CStraightRail *p =  new CStraightRail(p1->GetPoint( 0 ) ,p2->GetPoint( 0 ));
		AddToTheEnd( p);
		return SnapTwoObjectsWithStright(  p1, p2,  p,  rc); // set the straight connection accordingly
	} 


	if (( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjStraightRail )  )
	{		 
		bn = GetAngleTwoStrightObjects(  p1 , p2 , rc) ;

		if ( bn != 2 ) return 2;

	 	//if ( GetDistanceBtwTwoLines( p1->GetPoint(0) ,p1->GetPoint(1) , 
	 	//	p2->GetPoint(0) ,p2->GetPoint(1) ) > 4 ) return 3;

		CStraightRail *p =  new CStraightRail(p1->GetPoint( 0 ) ,p2->GetPoint( 0 ));
		AddToTheEnd( p);
		return SnapTwoObjectsWithStright(  p1, p2,  p,  rc); // set the straight connection accordingly
	} 


	if ( ( p1->mtype == ObjStraightRail ) && ( p2->mtype == ObjRailSwitch )  )
	{		 
		bn = GetAngleStrightAndSwitchObjects(  p1 , p2 , rc) ;

		if ( bn != 2 ) return 2;

	 	if ( GetDistanceBtwTwoLines( p1->GetPoint(0) ,p1->GetPoint(1) , 
	 		p2->GetPoint(0) ,p2->GetPoint(1) ) > 4 ) return 3;

		if ( PointInsideTheRect( p2->GetPoint( 2 ) , rc ) )  return 4; // do not serve bend endpoint now

		CStraightRail *p =  new CStraightRail(p1->GetPoint( 0 ) ,p2->GetPoint( 0 ));
		AddToTheEnd( p);
		return SnapTwoObjectsWithStright(  p1, p2,  p,  rc); // set the straight connection accordingly
	} 


	return 1;
}




 
CTempDrawObjectStore::CTempDrawObjectStore ()
{
	 rc1 = CRect(0,0,0,0);
	 rc2 = CRect(0,0,0,0);
	 pt1 = CPoint(0,0);
	 pt2 = CPoint(0,0);
}

void CTempDrawObjectStore::ClrRect1() 
{
rc1.top  = 0;
}
void CTempDrawObjectStore::ClrRect2()
{
rc2.top  = 0;
}

void CTempDrawObjectStore::SetPoint1( CPoint pt )
{
	pt1 = pt;
}
void CTempDrawObjectStore::SetPoint2( CPoint pt )
{
	pt2 = pt;
}

void CTempDrawObjectStore::SetPoint1( CFPoint pt )
{
	pt1.x = (int)roundToNearest(pt.x);  
	pt1.y = (int)roundToNearest(pt.y);  
}
void CTempDrawObjectStore::SetPoint2( CFPoint pt )
{
	pt2.x = (int)roundToNearest(pt.x);  
	pt2.y = (int)roundToNearest(pt.y);  
}


void CTempDrawObjectStore::SetRect1( CRect rc )
{
	rc1 = rc;
}
void CTempDrawObjectStore::SetRect2( CRect rc )
{
	rc2 = rc;
}
void CTempDrawObjectStore::DrawTempObjects( HDC hdc)
{
	if ( pt1.x != 0 )
		DrawPointCross( pt1 ,  hdc , GetRedColor() ,   3);
 	if ( pt2.x != 0 )
		DrawPointCross( pt1 ,  hdc , GetRedColor() ,   3);
	if ( rc1.top != 0 )
		OnDrawRect( &rc1  ,   hdc );
	if ( rc2.top != 0 )
		OnDrawRect( &rc2  ,   hdc );
}
