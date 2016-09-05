#pragma once

 


#define   LEAD_STANDARDRADIUS	100
#define   START_STANDARDANGLE  	0.0
#define   SWEEP_STANDARDANGLE  	45.0

// START_POINT = VIRTUAL_START_POINT
// BEND_POINT_SWITCH  = VIRTUAL_END_POINT
// END_POINT  <>  VIRTUAL_END_POINT

#define	START_POINT			0
#define	END_POINT			1
#define	BEND_POINT_SWITCH	2
#define	CENTER_SWITCH		3
#define	VIRTUAL_START_POINT	4
#define	VIRTUAL_END_POINT	5


#define  STANDARD_BEND_MAIN_AXIS_LENGHT      80
#define  STANDARD_SWITCH_MAIN_AXIS_LENGHT    80
#define  STANDARD_STOP_MAIN_AXIS_LENGHT		 40
#define  STANDARD_TRAIN_MAIN_AXIS_LENGHT	64  

 
#define		ObjNothing				0
#define	 	ObjStraightRail			1
#define	 	ObjBendRail				2
#define	 	ObjRailSwitch			3
#define	 	ObjRailSwitch2			4 
#define	 	ObjRailEnd				5
#define	 	ObjRailControl			6 
#define	 	ObjTrain				7
#define	 	ObjSemaphore			8
#define	 	ObjCntDis    			9
 
#define GET_FLOAT_START_ANGLE	0
#define GET_FLOAT_END_ANGLE		1
#define GET_FLOAT_SWEEP_ANGLE		1
#define GET_FLOAT_RADIUS		2
#define GP_CENTER   3

#define SEM_RED	   1
#define SEM_GREEN  0


#define  VAGON_TOHEAD_DIR		1     //  <-- [(0)     (1)]
#define  VAGON_TOBACK_DIR		-1    //  --> [(0)     (1)]
#define  VAGON_TO_0END_DIR		-1    //   (0)  <---   (1) on current segment
#define  VAGON_TO_1END_DIR		1     //   (0)  --->   (1) on current segment

#define OPTION_CHANGE_DIR	1
#define OPTION_SPEED_FAST	2
#define OPTION_SPEED_SLOW	3
#define OPTION_TRAIN_START  4
#define OPTION_TRAIN_STOP	5





 typedef struct {
	float iShiftDistance ;   // distance of the connected endpoint
	float iShiftDistanceRemainder ;
	float SlaveLen;
	CTrainVagon *pmaster;
	CTrainVagon *pslave;
	int epmaster;
	int epslave ;
	CRail *pCurrSegment; 
	CRail *pNewSegment; 
	CFPoint Curr_Point; 
	CFPoint New_Point; 
	int     iDirEp;
	int    onbendHead;
	int    onbendTail;
	int    error;
	int    endrail_reached;
	int		ep;
	int		onbend;
}   OTHERVAGONEND_DRAWDATASTRUCT;


class CRail   
	{

 public:
	 CRail();

	 virtual void  DrawObject( HDC hdc );
	 virtual CFPoint GetPoint( int ipoint );
	 virtual CFPoint GetPoint( int ipoint , float fref );
	 virtual float  GetMainAxisLenght();
	 virtual void  MoveObject( int dx,int dy);
	 virtual void  TurnObject( CPoint point, int endpoint );
	 virtual void  ReCalculateObject();
	 virtual void  RotateObject(   float  fangle , int endpoint);
	 virtual void  DisplaceObject(  CFPoint DestPoint , int endpoint);
	 virtual 	 CRect  GetBoundingRect();
	 virtual void ComputeBoundingRect();
	 virtual int GetSwitchPosition();  // 0 - stright 1 bend
	 virtual void Serialize(CArchive& ar);

	 void   DrawTracker( HDC hdc, int state );
	 CPoint CRail::GetHandle(int nHandle);

	 void	SetObject( int mode , float radius , float fStartAngle, float fSweepAngl );
	 void	SetStandardData(  float radius , int iwide );
	 void	SetPosition( CPoint mStartPoint ,CPoint mEndPoint ); 
	 void	SetPosition( CFPoint mStartPoint ,CFPoint mEndPoint ); 
	 void   SetPoint( int index , CFPoint fpoint);

	 int	GetMode();
	 float	GetFloat(int ipoint);
 
	 void	SetDebugPoint( int ipoint , CPoint pt );
	 void	SetDebugPoint( int ipoint , CFPoint pt );
 
	 void  HandleRailInfo();

	 void  SetRailId( WORD  mID );
	 WORD  GetRailId();
	 void  CleanCommIDs();
	 WORD  GetRailConnId ( int index);
	 void  SetRailConnId( int index , WORD mID );

     DRAWDATASTRUCT *GetDrawDataStructPtr();
 
	 int GetHitHandle(CPoint cp);

	 void SetSelected(int isel) ;

	 CRect  GetActiveRegion();

	 void   SetIndex(int ep , CRail  *pidx );
	 CRail  *GetIndex(int ep);

 	 void   SetIndex(int ep , int ip );
	 int    GetIIndex(int ep);


	public:
	  BYTE   mtype,mSelected,mode1,mode2;
 private:
	 DEBUGDRAWDATASTRUCT mDebugData;
	 DRAWDATASTRUCT  mDrawData;

 	 CRail  *Indexes[3]; //  fast indexing
	 int     mIndexes[3];

	 friend class CBendRail ;
	 friend class CStraightRail ;
 	 friend class CRailSwitch ;
	 friend class CEndRail;
	 friend class CSemphore;
	};


 class CBendRail : public CRail {
 public:
	CBendRail();
	 CBendRail( CPoint mStartPoint ,CPoint mEndPoint   );
	CBendRail( CFPoint mStartPoint ,CFPoint mEndPoint   );
	CBendRail( DRAWDATASTRUCT  * pDrawData );
 public:
	void  DrawObject( HDC hdc );
	void  MoveObject(  int  dx, int  dy);
	void  TurnObject( CPoint point , int endpoint );
	CFPoint GetPoint( int ipoint , float fref );
	CFPoint GetPoint( int ipoint );
	float  GetMainAxisLenght();
	void  ReCalculateObject();
	void  RotateObject(   float  fangle , int endpoint);
	void  DisplaceObject(  CFPoint DestPoint , int endpoint);
	CRect  GetBoundingRect();
	void ComputeBoundingRect();
	void Serialize(CArchive& ar);
 private:
 
	 
 };
 

  class CStraightRail : public CRail  {
 public:
	 CStraightRail();
	 CStraightRail( CPoint mStartPoint ,CPoint mEndPoint );
	 CStraightRail( CFPoint mStartPoint ,CFPoint mEndPoint );
 public:
	void   DrawObject( HDC hdc );
	void  MoveObject(  int  dx, int  dy);
	void  TurnObject( CPoint point, int endpoint );
	CFPoint GetPoint( int ipoint );
	float  GetMainAxisLenght();
	void  ReCalculateObject();
	void  RotateObject(   float  fangle , int endpoint);
	void  DisplaceObject(  CFPoint DestPoint , int endpoint);
	CRect  GetBoundingRect();
	void ComputeBoundingRect();
		void Serialize(CArchive& ar);
private:
	  
 
 };


 class CRailSwitch : public CRail  {
 public:
	CRailSwitch();
	CRailSwitch( CPoint mStartPoint ,CPoint mEndPoint   );
	CRailSwitch( CFPoint mStartPoint ,CFPoint mEndPoint   );
	CRailSwitch( DRAWDATASTRUCT  * pDrawData );
 public:
	void  DrawObject( HDC hdc );
	void  MoveObject(  int  dx, int  dy);
	void  TurnObject(  CPoint point , int endpoint );
	CFPoint GetPoint( int ipoint );
	CFPoint GetPoint( int ipoint , float fref );
	int   GetSwitchPosition();  // 0 - stright 1 bend
	float  GetMainAxisLenght();
	void  ReCalculateObject();
	void  RotateObject(   float  fangle , int endpoint);
	void  DisplaceObject(  CFPoint DestPoint , int endpoint);
	CRect  GetBoundingRect();
	void ComputeBoundingRect();
	void  ToggleSwitchPosition();
		void Serialize(CArchive& ar);
 private:
	 DWORD  iSwitchOff ;
	 	  
  };


class CEndRail : public CRail  {
 public:
	CEndRail();
	CEndRail( CPoint mStartPoint ,CPoint mEndPoint );
	CEndRail( CFPoint mStartPoint ,CFPoint mEndPoint );
 public:
	void   DrawObject( HDC hdc );
	void  MoveObject(  int  dx, int  dy);
	void  TurnObject(  CPoint point, int endpoint );
	CFPoint GetPoint( int ipoint );
	float  GetMainAxisLenght();
	void  RotateObject(   float  fangle , int endpoint);
	void  DisplaceObject(  CFPoint DestPoint , int endpoint);
	CRect  GetBoundingRect();
	void ComputeBoundingRect();
		void Serialize(CArchive& ar);
private:
 
 };

 class CSemphore : public CRail  {
 public:
	CSemphore();
	CSemphore( CFPoint mStartPoint ,CFPoint mEndPoint   );
 public:
	void  DrawObject( HDC hdc );
	CFPoint GetPoint( int ipoint );
 	CRect GetBoundingRect();
 	void  ComputeBoundingRect();
	void  ToggleSemPosition();
	void  Serialize(CArchive& ar);
	int   GetSemStatus();

 private:
	 DWORD  iColorRed ;
	 	  
  };



class CTrainVagon   
{

 public:
	 CTrainVagon ();
	 ~CTrainVagon ();
	 CTrainVagon (CFPoint mStartPoint ,CFPoint mEndPoint, TMPOBJECT_DRAWDATASTRUCT *p   );

	 int      IsBmpHandle();  

	 virtual  void  DrawObject( HDC hdc );
	 void     SetLenght(CFPoint mStartPoint ,CFPoint mEndPoint);
	 void     SetPosition( CFPoint mStartPoint ,CFPoint mEndPoint);
	 void	  SetHeadPosition(  CFPoint mPoint ) ;
     void	  SetTailPosition(  CFPoint mPoint ) ;
	
	 void	  SetCurrentEopPointer( int ep,  CRail *p);
	 CRail	  *GetCurrentEopPointer( int ep );

	 void     SetPosition( int ep,  CFPoint mPoint ) ;
	 CFPoint  GetPosition( int ep  ) ;
	 void     SetSwitchPosition( int ep, int onoff ) ;
	 int      GetSwitchPosition( int ep  ) ;

	 void     SetLocation( CRail *mCurrentPointer,float relposition);
	 void     SetDirection( int dir);
	 void     SetVagonId( WORD mID) ;
	 WORD     GetVagonId() ;
	 int      GetVagonDir();
	 int      GetVagonHeadDir();
	 int      GetVagonTailDir();
	 void     SetVagonHeadDir(int adir);
	 void     SetVagonTailDir(int adir);
	 int      GetHandle(int iz);
	 void     SetHandle( int iz, WORD  id); 
	 void     ChangeDirection();
	 int      Move(int delta,int istep);
	 void     MoveI(int delta);

	 void     StartTheLoco( int speed );
	 void     StopTheLoco();
	 void     SetSpeed(int speed);
	 int	  GetSpeed();

	 CFPoint  GetPoint( int ipoint ) ;
	 float    GetLenght();
	 CRect	  GetBoundingRect();
	 int      GetVagonType();
	 CRail    *GetCurrentHeadPointer();
	 void	  SetCurrentHeadPointer(CRail *);
	 CRail    *GetCurrentTailPointer();
	 void	  SetCurrentTailPointer(CRail *);

	void	CTrainVagon::AddCurrentDif( float dif);
	float	CTrainVagon::GetCurrentCorr( );

 private:
	 DRAWDATASTRUCT  mDrawData;
	 DRAWVAGONDATASTRUCT mDrawVagonData;

	 OTHERVAGONEND_DRAWDATASTRUCT  soes;
	 OTHERVAGONEND_DRAWDATASTRUCT  *oes;

	 int   mInternalLoopError;
	 int   mSpeed;  // 0 stop
	 int   mSpeedLoad;
	 float mDifTable[2];
	 float mCorrTable;
};
 

class CTempDrawObjectStore   
{
 public:
	CTempDrawObjectStore ();
	void SetPoint1( CPoint pt );
	void SetPoint2( CPoint pt );
	void SetRect1( CRect rc );
	void SetRect2(	CRect rc );
	void SetPoint1( CFPoint pt );
	void SetPoint2( CFPoint pt );
	void ClrRect1();
	void ClrRect2();

	void DrawTempObjects( HDC hdc);

private:
	CRect rc1;
	CRect rc2;
	CPoint pt1;
	CPoint pt2;
};