
#include "stdafx.h"
#include "MouseTest.h"

#include "Windowsx.h"
#include <stdio.h>
 
#include "Foundation.h"
#include "ComputeGA.h"
#include "ObjectStore.h"

//#include "Debug.h"

#include "Win32Specific.h"

#include "Debug.h"
extern CDebug mDebug;

extern CList  mCntList;
extern CList  mList;
extern CVagonList mVagonList;
extern CTempVector mCTempVector ;
extern TMPOBJECT_DRAWDATASTRUCT mtmpObjS;
extern int enable_refLine[4] ; 
extern CLineObject mRefLine[4] ; 
extern int    RefLineToggle ;   // move later to fundation

extern enum {
	  FunctionTracker,
	  FuntionSelObject,
	  FunctionSelHandle,
	  FuntionRotObjectStp,
	  FuntionMoveObject,
	  FuntionRotObjectEnp,
	  FuntionSnap,
	  FunctionBreak,
	  FunctionAddStright,
	  FunctionAddBendR,
	  FunctionAddBendL,
	  FunctionAddSwitchR,
	  FunctionAddSwitchL,
	  FunctionAddEnd,
	  FuntionAddTrain,
	  FuntionAddSemaphore,
	  FuntionNone }; // WtoFunMode;

extern int mFunctionMode  ;
extern HWND mhWnd;


RECT lastRc;
int isrect=0;
//  but region to detach

CFPoint lastPt;
int ispoint=0;

void  SetThePoint( CFPoint  lpt )  
{
	lastPt = lpt;
	ispoint=1;

	CRect rc = CRect((int)lpt.x,(int)lpt.y,(int)lpt.x,(int)lpt.y);
	RECT lprc = rc.GetRect();
	InflateRect ( &lprc,8);
	InvalidateRect( mhWnd , &lprc , FALSE ); // to be detailed

	// mDebug.CDebugPut("SetThePoint   ");

}

CFPoint  GetLastPoint()
{
	CFPoint pt(0,0);
	if ( ispoint == 1 )
	{
		pt = lastPt;
		ispoint = 0;
	}

	return pt;
}







int CheckMouseHit( CPoint mpoint , int mode )
{

	if( mode == IDM_MODE_CNTR)
		{

			//mDebug.CDebugPut("Check 0: ");

			
			// disconnect the highest priority
			//  check if  train in move first is so don't bother
			CTrainVagon *pv  = mVagonList.GetAtCoordinates( mpoint );
		 	if ( pv != NULL )
			 	if (  mVagonList.IsTheTrainMoving( pv)  == 0 )    
				{    // click should be close to the connection
					int icon=-1;
					if ( GetDistance(pv->GetPoint(0) ,mpoint ) < 8) icon = 0;
					if ( GetDistance(pv->GetPoint(1) ,mpoint ) < 8) icon = 1; 
					if ( icon >= 0)
					{
						// check this and neigboring vagon
						int handle;
						if ( (handle = pv->GetHandle( icon))  > 0 )  // vagon is connected
						{
							pv->SetHandle( icon , 0);
						//	SetThePoint( pv->GetPoint(icon));
							int id = pv->GetVagonId();
							CTrainVagon *pv1 =   mVagonList.GetAtId( handle) ; 
							if ( pv1 != NULL )
							{
								if ( pv1->GetHandle(0) == id ) 
								{ 
									pv1->SetHandle( 0 , 0);
									SetThePoint( pv1->GetPoint(0));
								}
								if ( pv1->GetHandle(1) == id )
								{
									pv1->SetHandle( 1 , 0);
									SetThePoint( pv1->GetPoint(1));
								}
							}
							// disconnected
						}
					}
				}

			
			//  loco has the priority
			//CTrainVagon *pv  = mVagonList.GetAtCoordinates( mpoint );
			if ( pv != NULL )
				{
					if ( pv->GetVagonType() != 0)
					SendMessage(mhWnd ,WM_USER,pv->GetVagonId(),1);
					return 1;
				}			
			
			
			CRail *p  = mList.GetAtCoordinates( mpoint );
			if ( p != NULL )
				{
					if ( p->mtype == ObjRailSwitch )
					{
                        CRailSwitch *pp = (CRailSwitch *)p;
						pp->ToggleSwitchPosition();

						CRect rc = pp->GetBoundingRect();
						RECT lprc = rc.GetRect();
						InflateRect ( &lprc,2);
						InvalidateRect( mhWnd , &lprc , FALSE ); // to be detailed
						return 1;
					}
				}

			p  = mCntList.GetAtCoordinates( mpoint );
			if ( p != NULL )
				{
					if ( p->mtype == ObjSemaphore )
					{
                        CSemphore *pp = (CSemphore *)p;
						pp->ToggleSemPosition();
						CRect rc = pp->GetBoundingRect();
						rc.InflateRect(2 );
						RECT lprc = rc.GetRect();
						InvalidateRect( mhWnd , &lprc , FALSE ); 
						//InvalidateObjectbyRectangle( rc );
						return 1;
					}
				}


		}

	return 0;
}




int  InvalidateCurrentObjectState(  CRail *p  )
{
	CRect  rc =  mCTempVector.CTempVectorGetRect() ;
 	RECT lprc = rc.GetRect();
	InflateRect ( &lprc,2);
	InvalidateRect( mhWnd , &lprc , FALSE ); // to be detailed
	return 0;
}



int  WM_LBUTTONUP_Handle_FuntionSelObject( CPoint mpoint ) 
{
	CPoint	pt = mCTempVector.CTempVectorGetPoint(0 ) ;
	if ( GetDistance( pt, mpoint ) < 3 )
		{
			CRail *p  = mList.GetAtCoordinates( pt );
			if ( p != NULL )
				{
					p->SetSelected(1);
					mList.SetActiveObjPtr(p);

                    mFunctionMode = FunctionSelHandle;

					CRect  rc = p->GetActiveRegion();
					mCTempVector.CTempVectorSetRect( rc);
					InvalidateCurrentObjectState(  p  );

					return 0;
				}
			}
	return 1;
}


int  WM_LBUTTONUP_Handle_FuntionMoveObject( CPoint mpoint ) 
{
	 
	CRail *p =  mList.GetActiveObjPtr() ;

	p->SetSelected(0);
	mFunctionMode =  FuntionSelObject;
	
	mList.Detach( p->GetDrawDataStructPtr()->ThisID ); 
	p->CleanCommIDs();

	CRect  rc = p->GetActiveRegion();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );

	mList.AutoSnap(p);
 
	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );

	return 0;
 
}

int  WM_LBUTTONUP_Handle_FuntionRotObjectStp( CPoint mpoint ) 
{
	CRail *p =  mList.GetActiveObjPtr() ;
	p->SetSelected(0);
	mFunctionMode =  FuntionSelObject;

	mList.Detach( p->GetDrawDataStructPtr()->ThisID );
	p->CleanCommIDs();

	CRect  rc = p->GetActiveRegion();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );

	return 0;
}

int  WM_LBUTTONUP_Handle_FuntionRotObjectEnp( CPoint mpoint ) 
{
	CRail *p =  mList.GetActiveObjPtr() ;
	p->SetSelected(0);
	mFunctionMode =  FuntionSelObject;

	mList.Detach( p->GetDrawDataStructPtr()->ThisID );
	p->CleanCommIDs();

	CRect  rc = p->GetActiveRegion();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );

	return 0;
}

int  WM_LBUTTONUP_Handle_FunctionAddStright( CPoint mpoint ) 
{
	 //SystemErrorMessage( 2);
 

	if ( GetDistance( mCTempVector.CTempVectorGetPoint(0)  , mpoint) <  4 )  
		{
               InvalidateCurrentObjectState(  NULL );	
			   return 1;
		}
	CStraightRail *p =  new CStraightRail(   mCTempVector.CTempVectorGetPoint(0)  , mpoint );
	mList.AddToTheEnd( p);

	CRect   rc = p->GetBoundingRect();   ///  THIS should be after each setposition called inside the object. - function only needs to retrieve ready data
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	 
	
	mList.AutoSnap(p);
	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );		 

	return 0;
}	 

int  WM_LBUTTONUP_Handle_FunctionAddBend( CPoint mpoint ) 
{
	
	if ( GetDistance( mCTempVector.CTempVectorGetPoint(0)  , mpoint) < 4 )  
		{
               InvalidateCurrentObjectState(  NULL );	
			   return 1;
		}
	DRAWDATASTRUCT *pd = mCTempVector.CTempVectorGetDrawData(); 

	mCTempVector.CTempVectorUpdateEnd( mpoint );

	CBendRail *p =  new CBendRail(  pd  );
	mList.AddToTheEnd( p);

	CRect   rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	mList.AutoSnap(p);

	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	return 0;
}

int  WM_LBUTTONUP_Handle_FunctionAddSwitch( CPoint mpoint ) 
{
  
	if ( GetDistance( mCTempVector.CTempVectorGetPoint(0)  , mpoint) <  4 )  
		{
               InvalidateCurrentObjectState(  NULL );	
			   return 1;
		}
	DRAWDATASTRUCT *pd = mCTempVector.CTempVectorGetDrawData(); 

	mCTempVector.CTempVectorUpdateEnd( mpoint );

	CRailSwitch *p =  new CRailSwitch(  pd  );		 
	mList.AddToTheEnd( p);

	CRect   rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	
	
	mList.AutoSnap(p);

 	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	return 0;
}


int  WM_LBUTTONUP_Handle_FunctionAddEnd( CPoint mpoint ) 
{
	
	if ( GetDistance( mCTempVector.CTempVectorGetPoint(0)  , mpoint) <  4 )  
		{
			InvalidateCurrentObjectState(  NULL );	
			return 1;
		}
	 
	CEndRail *p =  new CEndRail(   mCTempVector.CTempVectorGetPoint(0)  , mpoint );
	mList.AddToTheEnd( p);

	CRect   rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );

	mList.AutoSnap(p);

 	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );

	return 0;
}



int WM_LBUTTONUP_Handle_FunctionAddVagon( CPoint mpoint )
{

	if ( GetDistance(mpoint,mCTempVector.CTempVectorGetPoint(0) ) < 2 ) return 1;

	//  both affected
	 if ( mtmpObjS.tType != 0 )
                ComboBox_Enable( hwndCombo  ,TRUE );
 


	DRAWDATASTRUCT *pd = mCTempVector.CTempVectorGetDrawData(); 

 
		CTrainVagon *p;
	 

		p = new CTrainVagon(   mCTempVector.CTempVectorGetPoint(0)  , mpoint , &mtmpObjS    );
	

	mVagonList.AddToTheEnd( p);

		 if ( mtmpObjS.tType != 0 )
		 {
			  char pb[10];
			  sprintf_s( pb, 10, "T%d" ,   p->GetVagonId() );

			  int count = ComboBox_GetCount( hwndCombo );

			  ComboBox_InsertString(  hwndCombo  , count, pb);
			  ComboBox_SetCurSel( hwndCombo   , count );
			  ComboBox_ShowDropdown( hwndCombo   , FALSE );
			  ComboBox_SetItemData(hwndCombo  , count , p->GetVagonId() );
		 }



CRect	rc =   p->GetBoundingRect();
	rc.InflateRect(4);
	RECT   r = rc.GetRect();

	InvalidateRect( mhWnd , &r  , FALSE ); // to be detailed


	// not snapped to the rail
	if ( mVagonList.CTrainVagonSnap( p , &mList ) == 0)
	{
	   
		rc =   p->GetBoundingRect();
		mVagonList.RemoveAtTheEnd();
			rc.InflateRect(4);
	r = rc.GetRect();
	InvalidateRect( mhWnd , &r  , FALSE ); // to be detailed
	   return 1;
		
	}
		 

	rc =   p->GetBoundingRect();
	rc.InflateRect(4);
	r = rc.GetRect();
	InvalidateRect( mhWnd , &r  , FALSE ); // to be detailed

	//InvalidateRect( mhWnd , NULL , FALSE ); // to be detailed

	return 0;
}



int  WM_MOUSEMOVE_Handle_FuntionMoveObject( CPoint mpoint ) 
{
	CRail *p =  mList.GetActiveObjPtr() ;
	CPoint pt = mCTempVector.CTempVectorGetPoint(1);  // but not the first

	
	CRect  rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );
	 						
	mCTempVector.CTempVectorSetRect( rc);
	p->MoveObject( mpoint.x-pt.x,mpoint.y-pt.y );  // only move free  object!!!

	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	mCTempVector.CTempVectorUpdateEnd( mpoint );
 	return 0;
}

int  WM_MOUSEMOVE_Handle_FuntionRotObjectStp( CPoint mpoint ) 
{
	CRail *p =  mList.GetActiveObjPtr() ;
	
	CRect  rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );
	
	p->TurnObject( mpoint  ,1 );  // only move free  object!!!

	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	mCTempVector.CTempVectorUpdateEnd( mpoint );
	return 0;
}


int  WM_MOUSEMOVE_Handle_FuntionRotObjectEnp( CPoint mpoint ) 
{
	CRail *p =  mList.GetActiveObjPtr() ;
	
	CRect  rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );
	
	p->TurnObject( mpoint  ,0 );  // only move free  object!!!

	rc = p->GetBoundingRect();
	mCTempVector.CTempVectorSetRect( rc);
	InvalidateCurrentObjectState(  p  );	

	mCTempVector.CTempVectorUpdateEnd( mpoint );
	return 0;
}

 

int  WM_MOUSEMOVE_Handle_FunctionSaveRect( CPoint mpoint ) 
{
	RECT lprc ;
 

	if( isrect != 0 ) return 0;
	isrect =1;

		CPoint Cp1 = mCTempVector.CTempVectorGetPoint(0 );
		CPoint Cp2 = mCTempVector.CTempVectorGetPoint( 1);	
	    
		lprc.left = Cp1.x;
		lprc.top = Cp1.y;
		lprc.right = Cp2.x;
		lprc.bottom = Cp2.y;

		NormalizeRect(&lprc);

		lastRc = lprc;
  
	return 0;
}


RECT GetLastCRect()
{
	 
	return lastRc;

}



int  WM_MOUSEMOVE_Handle_FunctionTracker( CPoint mpoint ) 
{
	RECT lprc ;
 
		CPoint Cp1 = mCTempVector.CTempVectorGetPoint(0 );
		CPoint Cp2 = mCTempVector.CTempVectorGetPoint( 1);	
	    
		lprc.left = Cp1.x;
		lprc.top = Cp1.y;
		lprc.right = Cp2.x;
		lprc.bottom = Cp2.y;

		NormalizeRect(&lprc); 
 
	InflateRect ( &lprc,2);
	InvalidateRect( mhWnd , &lprc , FALSE ); // to be detailed

	mCTempVector.CTempVectorUpdateEnd( mpoint );
	return 0;
}

void DeactivateObject()
{
	  // must invalidate sel handle  if not hit handle or  the same object
		CRail *p =  mList.GetActiveObjPtr() ;
		if ( p != NULL)
				{
			    p->SetSelected(0);
			 	p->mSelected = 0 ;
				mList.SetActiveObjPtr( NULL) ;
				mFunctionMode = FuntionSelObject;

				CRect  rc = p->GetBoundingRect();
				mCTempVector.CTempVectorSetRect( rc);
				InvalidateCurrentObjectState(  p  );	

				 }

}

void HandleReferenceLineStore( int lineno )
{

	if ( enable_refLine[lineno]==0) 
		{
			CRail *p =  mList.GetActiveObjPtr() ;
			if ( p != NULL)
				{
					if ( RefLineToggle == 1 )
						mRefLine[lineno]  =  CLineObject ( p->GetPoint( 4  ), p->GetPoint( 5 )); 

					if ( RefLineToggle == 2 )
					{
						 if ( p->mtype == ObjBendRail )
								mRefLine[lineno]   =   CLineObject ( p->GetPoint( 1  ), p->GetPoint( 6 ));   // blue 
	 					
						 if ( p->mtype == ObjRailSwitch )
					 			mRefLine[lineno]  =  CLineObject ( p->GetPoint( 2  ), p->GetPoint( 6 ));   // blue 
					}
					enable_refLine[lineno]=1;
				}

		}
		else enable_refLine[lineno]=0;
}


		CTwoPoints mTwoPoint;
		int  mTwoPointActive;

void  SetTmpLineSegment( CTwoPoints  twop )
{
	mTwoPoint = twop;
	mTwoPointActive =1;
}


void  InvalidateTmpLineBoundingRect( )
{
	CRect	rc =   CRect( (int)mTwoPoint.mPoint1.x, (int)mTwoPoint.mPoint1.y , (int)mTwoPoint.mPoint2.x, (int)mTwoPoint.mPoint2.y );
	rc.InflateRect(4 );
	RECT lprc = rc.GetRect();
	//InvalidateRect( mhWnd , &lprc , FALSE );
	InvalidateRect( mhWnd , NULL,  FALSE );

}

void  InvalidateObjectby2Points(  CFPoint  p1,  CFPoint  p2 , int infl )
{
	CRect	rc =   CRect( (int)p1.x, (int)p1.y ,(int)p2.x, (int)p2.y );
	rc.NormalizeRect();
	RECT lprc = rc.GetRect();
	InvalidateRect( mhWnd , &lprc , FALSE );
}


int	  WM_LBUTTONUP_Handle_FunctionAddSemaphore( CPoint mpoint )
{

//	CTrainModel2DDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)	return 0;

	int i;
	int count = mList.GetCount() ;
 
	 InvalidateObjectby2Points( mCTempVector.CTempVectorGetPoint(0) , mCTempVector.CTempVectorGetPoint(1), 8 +8 );

	for (i =0 ; i < count; i++ )
	{
		CRail *p = mList.GetAt(i);
		if ( p->mtype == ObjStraightRail )
		{  
			DRAWDATASTRUCT *pd = mCTempVector.CTempVectorGetDrawData(); 		
			CFPoint pmid = GetCenterPoint( mCTempVector.CTempVectorGetPoint(0) , mCTempVector.CTempVectorGetPoint(1) );
			// line perpendicular to 0,1  crossing point mid 

			CFPoint cppt = GetPointAtTheLine (  p->GetPoint(0),p->GetPoint(1)  , pmid  );

			if (	GetDistanceLineToPoint( p->GetPoint(0),p->GetPoint(1)  , pmid  ) <  float(pd->Width << 2)    )
				if (   IsPoinInsideSegment(p->GetPoint(0),p->GetPoint(1) ,cppt ) == 1) 
				 {  
					 //
					 //    p1 ------ p2
					 //
					 // creates squere      p1 ,    p2     ,    distance from rail  , size 
				
						// to make the sem smaller need to bring pmid & cppt closer 
					 

				//	 CFPoint  pxcenter =  GetCenterPoint( cppt , pmid  );
				//	 cppt = pxcenter;

					 CTwoPoints tp = ComputeParSectionOfTheLenght( cppt , pmid , float(pd->Width << 2) -4 , float(pd->Width)-4 );
				
				float a = 
					GetAbsAngleOfTwoVectors (  mCTempVector.CTempVectorGetPoint(0) , mCTempVector.CTempVectorGetPoint(1),tp.mPoint1 ,tp.mPoint2 );
				
			 


				CSemphore *pss;

				if ( a < 90 )
				   pss =  new CSemphore(   tp.mPoint1  , tp.mPoint2 );
				else
					pss =  new CSemphore(   tp.mPoint2  , tp.mPoint1 );
				
				pss->SetPoint(2,cppt);
				pss->SetRailConnId( 0, p->GetRailId());

				mCntList.AddToTheEnd( pss );
            	CRect   rc = pss->GetBoundingRect();    
				mCTempVector.CTempVectorSetRect( rc);
				InvalidateCurrentObjectState(  pss  );
				return 1;
				}
		}
	}


	return 0;
}
 
int   WM_MOUSEMOVE_Handle_FunctionAddSemaphore( CPoint mpoint ) 
{
	int i;
	int count = mList.GetCount() ;


	// mDebug.CDebugPut("WM_MOUSEMOVE_Handle_FunctionAddSemaphore  ");

	for (i =0 ; i < count; i++ )
	{
		CRail *p = mList.GetAt(i);
		if ( p->mtype == ObjStraightRail )
		{  
			DRAWDATASTRUCT *pd = mCTempVector.CTempVectorGetDrawData(); 		
			CFPoint pmid = GetCenterPoint( mCTempVector.CTempVectorGetPoint(0) , mCTempVector.CTempVectorGetPoint(1) );
			CFPoint cppt = GetPointAtTheLine (  p->GetPoint(0),p->GetPoint(1)  , pmid  );
			if (	GetDistanceLineToPoint( p->GetPoint(0),p->GetPoint(1)  , pmid  ) <  float(pd->Width << 2)    )
			 {  
				 // is point 
				 if (   IsPoinInsideSegment(p->GetPoint(0),p->GetPoint(1) ,cppt ) == 1) 
				 {
				    SetTmpLineSegment( CTwoPoints( cppt , pmid ) );
					InvalidateTmpLineBoundingRect();
					return 1;
				 }
			 }
		}
		mTwoPointActive = 0;
		InvalidateTmpLineBoundingRect();  
	}

	return 0;
}

 

 

 

 
