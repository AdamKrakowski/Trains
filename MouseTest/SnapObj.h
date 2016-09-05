#pragma once

	int  SnapStrightToEndObjects( CRail *p1,CRail *p2,CRect rc) ;
	int  SnapBendToEndObjects( CRail *p1,CRail *p2,CRect rc) ;

	int	 SnapEndToStrightObjects( CRail *p1,CRail *p2,CRect rc) ;
	int  SnapEndToBendObjects( CRail *p1,CRail *p2,CRect rc) ;

	int  SnapTwoStrightObjects( CRail *p1,CRail *p2,CRect rc) ;
	int  SnapBendToStrightObjects( CRail *p1,CRail *p2,CRect rc) ;
	int  SnapSwitchToStrightObjects( CRail *p1,CRail *p2,CRect rc) ;

	int SnapTwoObjectsWithStright( CRail *p1,CRail *p2, CStraightRail *p, CRect rc); 

	int  SnapTwoBendObjects( CRail *p1,CRail *p2,CRect rc) ;
	int  SnapStrightToBendObjects( CRail *p1,CRail *p2,CRect rc) ;
	int  SnapSwitchToBendObjects( CRail *p1,CRail *p2,CRect rc) ;

	int  SnapTwoSwitchObjects( CRail *p1,CRail *p2,CRect rc) ;
	int  SnapStrightToSwitchObjects( CRail *p1,CRail *p2,CRect rc) ;
	int  SnapBendToSwitchObjects( CRail *p1,CRail *p2,CRect rc) ;

	int  GlueTwoStrightObjects(  CRail *p1,CRail *p2,CRect rc); 
	int  GetAngleTwoStrightObjects(  CRail *p1,CRail *p2,CRect rc) ;
	int  GetAngleTwoBendObjects(  CRail *p1,CRail *p2,CRect rc) ;
	int  GetAngleStrightAndBendObjects(  CRail *p1,CRail *p2,CRect rc);
	int  GetAngleStrightAndSwitchObjects(  CRail *p1,CRail *p2,CRect rc); 
	int  GetAngleSwitchtAndBendObjects(  CRail *p1,CRail *p2,CRect rc); 
	int  GetAngleSwitchtAndSwitchObjects(  CRail *p1,CRail *p2,CRect rc) ;

	int  GlueStrightToBendObjects(  CRail *p1,CRail *p2,CRect rc); 

	int  ConnectTwoBendObjectsWithBend( CRail *p1,CRail *p2,CRail *p3,CRect rc) ;
	int  ConnectBendAndStraightObjectsWithBend( CRail *p1,CRail *p2,CRail *p3,CRect rc) ;
	int  ConnectTwoSwitchObjectsWithBend( CRail *p1,CRail *p2,CRail *p3,CRect rc) ;

	int ConnectObjectsWithBend( CRail *p1,CRail *p2,CRail *p3,CRect rc) ;

	void TurnStraight ( CRail *p , float  fangle , int endpoint );
	void MoveStraight (  CRail *p , CFPoint DestPoint , int endpoint  ) ;
	void TurnSwitch ( CRail *p , float  fangle , int endpoint );
	void MoveSwitch ( CRail *p , CFPoint DestPoint , int endpoint );
	void TurnBend ( CRail *p , float  fangle  , int endpoint  ) ;
	void MoveBend ( CRail *p , CFPoint DestPoint  ,   int endpoint  );

	int  HitSingleObject(  CRail *p ) ;