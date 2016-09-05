
#include "Objects.h"


#define MX_RAIL   500
 

  class CList
{
 public:
	 CList();
	 void SetAppParameters( float bendRadius , int wideTrack );
	 void AddToTheEnd( CRail *p);
	 void AddToTheEndStd( CRail *p);
	 int  RemoveAtTheEnd();
	 int  DeleteObject(CRail *p);
	 CRail *GetAt( int);
	 CRail *GetAtCoordinates( CPoint pt );
	 CRail *GetByEndPoint(  CRail *current , int  endpoint   ) ;
	 int  GetCount();
	 int  SnapObjects( CRect rc);
	 int	SnapObjects(CRect  rc1 ,CRect rc2 ) ;
	 int  ProcessObjects( CRect rc , int mode);
	 void Detach(WORD mLastID);
	 void SetActiveObjPtr(CRail *p);
	 CRail *GetActiveObjPtr() ;
	 void AutoSnap(CRail *p);
	 int SnapTwoObjects(CRail * p1,CRail *p2, CRect rc);
	 int GlueTwoFixedObjects(CRail * p1,CRail *p2,CRect rc);
	 int GlueTwoFixedObjectsWithBend(CRail * p1,CRail *p2,CRect rc);
	 int GlueTwoFixedObjectsWithStright( CRail * p1,CRail *p2,CRect rc);
	 int GlueConnectedObjects( CRail * p1,CRail *p2,CRect rc);
	 void Serialize(CArchive& ar);
	 	 void   Index();

	 float GetMaxVariation(int *);
private:
	 void CheckPointToSnap( CRail *p1,CRail *p2, int pointindex1,int pointindex2);
	 int CheckConnectedPointToSnap( CRail *p1,CRail *p2, int pointindex1,int pointindex2);
 

private:
	int  mCount;
	WORD mID;
	CRail *mRailPointerTable[MX_RAIL];
	CRail *pCurSeleted;
	float stdRadius;
	int   stdWide;
};

 
  #define MX_VAGONS   500

 



 class CVagonList
{
 public:
	 CVagonList();
	 int	GetCount();
	 void	AddToTheEnd( CTrainVagon *p);
	 void   VagonsInfo( ); 
	 CTrainVagon *GetAt( int);
	 CTrainVagon *GetAtCoordinates( CPoint pt );
	 CTrainVagon *GetAtId( WORD  id ) ;
	 int	CTrainVagonSnap(CTrainVagon *pv , CList  *pl );
	 int	RemoveAtTheEnd(); 
	 void	LocoControl( int option , int id);
	 void   MoveAllTrains( HWND mhWnd,int istep,int filter);
	 void	CheckSemStopAll ( CList  *pCntList) ;
	 int	IsTheTrainMoving( CTrainVagon *p) ;
	 int    IsLocoPullTheVagons(CTrainVagon *p) ;
	 CTrainVagon *CVagonList::GetLastVagon(CTrainVagon *p) ;
	 int	CollisionCheck(    CTrainVagon *p, COLISSION_DRAWDATASTRUCT *pcd );
	 void   MoveVagons(CTrainVagon *p) ;
	 int	MoveOneAttachedVagon(  int endpoint , CTrainVagon *px ,int ipush);
	 CRect	GetBoundingRectForTrain(CTrainVagon *p);
	 void	InvalidateSet(CTrainVagon *p , HWND nhWnd); 

private:
	int	ComputeSetNearestPoint (CTrainVagon * pv ,CRail * p ) ;
	int CollisionCheckEp(  int endpoint , CTrainVagon *px  ,COLISSION_DRAWDATASTRUCT *pcd );
	int ShiftVagon( OTHERVAGONEND_DRAWDATASTRUCT *); 
	CRail *FindRailByLocation ( CFPoint cp );
	void InvalidateCar( int ep , CTrainVagon *p ,  HWND nhWnd  );
	CRect InvalidateSet(CTrainVagon *pmaster ,  CRect rcin  );
	CRect InvalidateCar(  int endpoint , CTrainVagon *pmaster ,  CRect rcin );
	
private:
	int  mCount;
	WORD mID;
	CTrainVagon *mVagonPointerTable[MX_VAGONS];
	OTHERVAGONEND_DRAWDATASTRUCT  mp_oe_data;

};


 