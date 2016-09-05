#include "stdafx.h"
#include "Windowsx.h"
#include "Math.h"
#include <stdio.h>

#include "MouseTest.h"
 
#include "Foundation.h"
#include "Objects.h"
#include "ComputeGA.h"

#include "Graphics.h"

#include "Debug.h"
extern CDebug mDebug;
 
 

int  GetAngleTwoStrightObjects(  CRail *p1,CRail *p2,CRect rc) 
 {
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;

	float a1 ,a2 ;
	if ( PointInsideTheRect( stp1 , rc ) )				 //   [0]-------->(1)
	{
		a1 = 	ComputeVectorAngle( enp1 , stp1   ) ;	 // main axis from right to left   [0]<<-----(1)
		if ( PointInsideTheRect( stp2 , rc ) )			 //  (1) <--------[0]
			 a2 = ComputeVectorAngle( stp2 , enp2  ) ;   // aux axis from right  to left   <<-----
		else                                             //  (0)-------->[1]
			 a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis from right  to left   <<-----
	}
	else												//   (0)-------->[1]
	{
		a1 = 	ComputeVectorAngle( stp1 , enp1   ) ;	// main axis from left to writh [0] ----->>(1)
		if ( PointInsideTheRect( stp2 , rc ))			//  [0]-------->(1)
			a2 = ComputeVectorAngle( stp2 , enp2   ) ;  // aux axis from left to right  --------->>
		else											//  [1]-------->(0)
			a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis from left to right  --------->>
	}

	if ( a2 >= a1 )
	{
		if ( (a2-a1) <  2) return 2;
		if ( (a2-a1) < 180 ) return 0;
		else return 1 ;  // 360 =0 fixed
	}
	else
	{
		if ( (a1-a2) <  2) return 2;
		if ( (a1-a2) < 180 ) return 1;
		else return 0 ;
	}
 
 }
 
//  p1 is stright p2 is switch

int  GetAngleStrightAndSwitchObjects(  CRail *p1,CRail *p2,CRect rc) 
 {
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;

	float a1 ,a2 ;
	if ( PointInsideTheRect( stp1 , rc ) )				 //   [0]-------->(1)
	{
		a1 = 	ComputeVectorAngle( enp1 , stp1   ) ;	 // main axis from right to left   [0]<<-----(1)
		if ( PointInsideTheRect( stp2 , rc ) )			 //  (1) <--------[0]
			 a2 = ComputeVectorAngle( stp2 , enp2  ) ;   // aux axis from right  to left   <<-----
		else   if ( PointInsideTheRect( enp2 , rc ) )	 //  (0)-------->[1]
			 a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis from right  to left   <<-----
		else
			{
				a2 = ComputeVectorAngle( p2->GetPoint( 6 )  , p2->GetPoint( 2 ) ) ; 

			//p2->SetDebugPoint(0, enp1 ); //  
			//p2->SetDebugPoint(1, stp1 ); // 
			//p2->SetDebugPoint(2, p2->GetPoint(6) ); //  
			//p2->SetDebugPoint(3, p2->GetPoint(2) ); // 

		}
	}
	else												//   (0)-------->[1]
	{
		a1 = 	ComputeVectorAngle( stp1 , enp1   ) ;	// main axis from left to writh [0] ----->>(1)
		if ( PointInsideTheRect( stp2 , rc ))			//  [0]-------->(1)
			a2 = ComputeVectorAngle( stp2 , enp2   ) ;  // aux axis from left to right  --------->>
		else	if ( PointInsideTheRect( enp2 , rc ) )	//  [1]-------->(0)
			a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis from left to right  --------->>
		else
		{
			a2 = ComputeVectorAngle( p2->GetPoint( 6 )  , p2->GetPoint( 2 ) ) ; 
			
			//p2->SetDebugPoint(0, stp1 ); //  
			//p2->SetDebugPoint(1, enp1 ); // 
			//p2->SetDebugPoint(2, p2->GetPoint(6) ); //  
			//p2->SetDebugPoint(3, p2->GetPoint(2) ); // 

		}
	}
 
	//mDebug.CDebugPut("ObjStraightRail->ObjRailSwitch %6.2f %6.2f " ,a1,a2 );	

	if ( a2 >= a1 )
	{
		if ( (a2-a1) <  2) return 2;
		if ( (a2-a1) < 180 ) return 0;
		else return 1 ;  // 360 =0 fixed
	}
	else
	{
		if ( (a1-a2) <  2) return 2;
		if ( (a1-a2) < 180 ) return 1;
		else return 0 ;
	}
 
 }


///   [SW]  ---  [B]

int  GetAngleSwitchtAndBendObjects(  CRail *p1,CRail *p2,CRect rc) 
 {
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;

	float a1 ,a2 ;
		
	if ( PointInsideTheRect( stp1 , rc ) )				 //   [0]-------->(1)
	{
		a1 = 	ComputeVectorAngle( enp1 , stp1   ) ;				// main axis from right to left   [0]<<-----(1)
		if ( PointInsideTheRect( stp2 , rc ) )						//  (1) <--------[0]
			 a2 = ComputeVectorAngle( stp2 , p2->GetPoint(5) ) ;	// aux axis from right  to left   <<-----
		else														//  (0)-------->[1]
			 a2 = ComputeVectorAngle(  p2->GetPoint(6) ,enp2  ) ;	// aux axis from right  to left   <<-----
	}
	else if ( PointInsideTheRect( enp1 , rc ) )											 
	{
		a1 = 	ComputeVectorAngle( stp1 , enp1   ) ;	// main axis from left to writh [0] ----->>(1)
		if ( PointInsideTheRect( stp2 , rc ))						//  [0]-------->(1)
			a2 = ComputeVectorAngle( stp2 ,  p2->GetPoint(5)  ) ;	// aux axis from left to right  --------->>
		else														//  [1]-------->(0)
			a2 = ComputeVectorAngle( p2->GetPoint(6), enp2  ) ;		// aux axis from left to right  --------->>
	}
 	else 
	{
		a1 = 	ComputeVectorAngle( p1->GetPoint( 2 )  , p1->GetPoint( 6 )    ) ;	// main axis from left to writh [0] ----->>(1)
		if ( PointInsideTheRect( stp2 , rc )) //  [0]-------->(1)
			a2 = ComputeVectorAngle( stp2 ,  p2->GetPoint(5)  ) ;	// aux axis from left to right  --------->>
		else	//  [1]-------->(0)
			a2 = ComputeVectorAngle( p2->GetPoint(6), enp2  ) ;		// aux axis from left to right  --------->>
	} 
	
	
		/*
			p2->SetDebugPoint(0, p1->GetPoint( 6 ) ); //  
			p2->SetDebugPoint(1, p1->GetPoint( 2 ) ); // 
			p2->SetDebugPoint(2, p2->GetPoint(6) ); //  
			 p2->SetDebugPoint(3, enp2 ); // 
			 */

	//mDebug.CDebugPut("ObjStraightRail->ObjRailSwitch %6.2f %6.2f " ,a1,a2 );	

	if ( a2 >= a1 )
	{
		if ( (a2-a1) <  2) return 2;
		if ( (a2-a1) < 180 ) return 0;
		else return 1 ;  // 360 =0 fixed
	}
	else
	{
		if ( (a1-a2) <  2) return 2;
		if ( (a1-a2) < 180 ) return 1;
		else return 0 ;
	}
 
 }

///   [SW]  ---  [SW]

int  GetAngleSwitchtAndSwitchObjects(  CRail *p1,CRail *p2,CRect rc) 
 {
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;

	float a1=0 ,a2=0 ;
		
	if ( PointInsideTheRect( stp1 , rc ) )				 //   [0]-------->(1)
	{
		a1 = 	ComputeVectorAngle( enp1 , stp1   ) ;				// main axis from right to left   [0]<<-----(1)
		if ( PointInsideTheRect( stp2 , rc ) )						//  (1) <--------[0]
			 a2 = ComputeVectorAngle( stp2 , p2->GetPoint(5) ) ;	// aux axis from right  to left   <<-----
		else if ( PointInsideTheRect( enp2 , rc ) )															//  (0)-------->[1]
			 a2 = ComputeVectorAngle(  p2->GetPoint(6) ,enp2  ) ;	// aux axis from right  to left   <<-----
		else if ( PointInsideTheRect( p2->GetPoint( 2 ) , rc ) )	
			 a2 = ComputeVectorAngle(  p2->GetPoint(6) ,p2->GetPoint(2) ) ;
	}
	else if ( PointInsideTheRect( enp1 , rc ) )											 
	{
		a1 = 	ComputeVectorAngle( stp1 , enp1   ) ;	// main axis from left to writh [0] ----->>(1)
		if ( PointInsideTheRect( stp2 , rc ))						//  [0]-------->(1)
			a2 = ComputeVectorAngle( stp2 ,  p2->GetPoint(5)  ) ;	// aux axis from left to right  --------->>
		else if ( PointInsideTheRect( enp2 , rc ) )															//  [1]-------->(0)
			a2 = ComputeVectorAngle( p2->GetPoint(6), enp2  ) ;		// aux axis from left to right  --------->>
		else if ( PointInsideTheRect( p2->GetPoint( 2 ) , rc ) )	
			a2 = ComputeVectorAngle(  p2->GetPoint(6) ,p2->GetPoint(2) ) ;
	}
 	else 
	{
		a1 = 	ComputeVectorAngle( p1->GetPoint( 2 )  , p1->GetPoint( 6 )    ) ;	// main axis from left to writh [0] ----->>(1)
		if ( PointInsideTheRect( stp2 , rc )) //  [0]-------->(1)
			a2 = ComputeVectorAngle( stp2 ,  p2->GetPoint(5)  ) ;	// aux axis from left to right  --------->>
		else if ( PointInsideTheRect( enp2 , rc ) )															//  [1]-------->(0)
			a2 = ComputeVectorAngle( p2->GetPoint(6), enp2  ) ;		// aux axis from left to right  --------->>
		else if ( PointInsideTheRect( p2->GetPoint( 2 ) , rc ) )	
			a2 = ComputeVectorAngle(  p2->GetPoint(6) ,p2->GetPoint(2) ) ;
	} 
	
	
		/*
			p2->SetDebugPoint(0, p1->GetPoint( 6 ) ); //  
			p2->SetDebugPoint(1, p1->GetPoint( 2 ) ); // 
			p2->SetDebugPoint(2, p2->GetPoint(6) ); //  
			 p2->SetDebugPoint(3, enp2 ); // 
			 */

	//mDebug.CDebugPut("ObjStraightRail->ObjRailSwitch %6.2f %6.2f " ,a1,a2 );	

	if ( a2 >= a1 )
	{
		if ( (a2-a1) <  2) return 2;
		if ( (a2-a1) < 180 ) return 0;
		else return 1 ;  // 360 =0 fixed
	}
	else
	{
		if ( (a1-a2) <  2) return 2;
		if ( (a1-a2) < 180 ) return 1;
		else return 0 ;
	}
 
 }


int  GetAngleTwoBendObjects(  CRail *p1,CRail *p2,CRect rc) 
 {
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;

	float a1 ,a2 ;
	if ( PointInsideTheRect( stp1 , rc ) )							//   [0]-------->(1)
	{
		a1 = 	ComputeVectorAngle( p1->GetPoint(5) , stp1 ) ;		// main axis from right to left   [0]<<-----(1)		
		if ( PointInsideTheRect( stp2 , rc ) )						//  (1) <--------[0]
			 a2 = ComputeVectorAngle( stp2 , p2->GetPoint(5) ) ;	// aux axis from right  to left   <<-----
		else														//  (0)-------->[1]
			 a2 = ComputeVectorAngle(  p2->GetPoint(6) ,enp2  ) ;	// aux axis from right  to left   <<-----
	}
	else															//   (0)-------->[1]
	{
		a1 = 	ComputeVectorAngle( enp1 ,  p1->GetPoint(6)   ) ;	// main axis from left to writh [0] ----->>(1)
		if ( PointInsideTheRect( stp2 , rc ))						//  [0]-------->(1)
			a2 = ComputeVectorAngle( stp2 ,  p2->GetPoint(5)  ) ;	// aux axis from left to right  --------->>
		else														//  [1]-------->(0)
			a2 = ComputeVectorAngle( p2->GetPoint(6), enp2  ) ;		// aux axis from left to right  --------->>
	}

	if ( a2> a1 )
	{
		if ( (a2-a1) <  2) return 2;
		if ( (a2-a1) < 180 ) return 0;
		else return 1 ;
	}
	else
	{
		if ( (a1-a2) <  2) return 2;
		if ( (a1-a2) < 180 ) return 1;
		else return 0 ;
	}
 }


 //   assuming first is stright ( switch)
 
int  GetAngleStrightAndBendObjects(  CRail *p1,CRail *p2,CRect rc) 
 {
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;

	float a1 ,a2 ;
	if ( PointInsideTheRect( stp1 , rc ) )							//   [0]-------->(1)
	{
		a1 = 	ComputeVectorAngle( enp1 , stp1   ) ;				// main axis from right to left   [0]<<-----(1)
		if ( PointInsideTheRect( stp2 , rc ) )						//  (1) <--------[0]
			 a2 = ComputeVectorAngle( stp2 , p2->GetPoint(5) ) ;	// aux axis from right  to left   <<-----
		else														//  (0)-------->[1]
			 a2 = ComputeVectorAngle( p2->GetPoint(6) ,enp2 ) ;	// aux axis from right  to left   <<-----
	}
	else															//   (0)-------->[1]
	{
		a1 = 	ComputeVectorAngle( stp1 , enp1   ) ;				// main axis from left to writh [0] ----->>(1)
		if ( PointInsideTheRect( stp2 , rc ))						//  [0]-------->(1)
			a2 = ComputeVectorAngle( stp2 ,  p2->GetPoint(5)  ) ;	// aux axis from left to right  --------->>
		else														//  [1]-------->(0)
			a2 = ComputeVectorAngle( p2->GetPoint(6), enp2  ) ;		// aux axis from left to right  --------->>
	}

	if ( a2> a1 )
	{
		if ( (a2-a1) <  2) return 2;
		if ( (a2-a1) < 180 ) return 0;
		else return 1 ;
	}
	else
	{
		if ( (a1-a2) <  2) return 2;
		if ( (a1-a2) < 180 ) return 1;
		else return 0 ;
	}
 }

 

//  ---------------------------
//
//		snapping 'EndObject'
//
//  ---------------------------

int  SnapBendToEndObjects( CRail *p1,CRail *p2,CRect rc)  
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();
	
	if ( PointInsideTheRect( stp1 , rc ) )
	{
		return 1; // no snap to dead end
	}
	else // PointInsideTheRect( enp1 , rc )
	{
		float a1 = 	ComputeVectorAngle( stp1 , enp1   ) ; // main axis
		if ( PointInsideTheRect( stp2 , rc ))
		{
			float a2 = 	ComputeVectorAngle( stp2 , p2->GetPoint(5)) ; // aux axis 
		 	if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 2; 
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,0 );
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
			float a2 = ComputeVectorAngle(  p2->GetPoint(6) , enp2 ) ;  // aux axis 
		 	if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 3; 
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(enp1 ,1 );
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}
	return 0;
}

int SnapStrightToEndObjects( CRail *p1,CRail *p2,CRect rc) 
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();
	
	if ( PointInsideTheRect( stp1 , rc ) )
	{
		return 1;
	}
	else // PointInsideTheRect( enp1 , rc )
	{
		float a1 = 	ComputeVectorAngle( stp1 , enp1   ) ; // main axis
		if ( PointInsideTheRect( stp2 , rc ))
		{
			float a2 = 	ComputeVectorAngle( stp2 , enp2   ) ; // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 2; 
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,0 );
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
			float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 3; 	
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(enp1 ,1 );
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );	
		}
	}
	return 0;
}


int SnapEndToStrightObjects( CRail *p1,CRail *p2,CRect rc) 
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();
	
	if ( PointInsideTheRect( stp1 , rc ) )
	{
		float a1 = 	ComputeVectorAngle(  enp1 , stp1   ) ; // main axis	 
		if ( PointInsideTheRect( stp2 , rc ) )
		{
				return 1;
		}
		else
		{
			float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(stp1 ,1 );
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}
	else if ( PointInsideTheRect( enp1 , rc ))
	{
		float a1 = 	ComputeVectorAngle( stp1 , enp1   ) ; // main axis
		if ( PointInsideTheRect( stp2 , rc ))
		{
			return 1;
		}
		else
		{
			float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 1; 
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(enp1 ,1 );
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );	
		}
	}
	return 0;
}

// new  2014.07.30  verified & checked

int SnapEndToBendObjects( CRail *p1,CRail *p2,CRect rc) 
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();
	
	if ( PointInsideTheRect( stp1 , rc ) )  // main axis of bend   [X]-------->
	{
		float a1 = 	ComputeVectorAngle(   p1->GetPoint(5) ,stp1  ) ; // aux    <=========
		if ( PointInsideTheRect( stp2 , rc ) )  //  <------[x]
		{
				return 1;
		}
		else     //  ----->[x]
		{
			float a2 = 	ComputeVectorAngle( enp2 , stp2   ) ; // aux axis 
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(stp1 ,1 );
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}
	else // PointInsideTheRect( enp1 , rc )  main axis of bend   -------->[X]
	{
		float a1 = 	ComputeVectorAngle( enp1 , p1->GetPoint(6) ) ; // main axis	 =======>
		if ( PointInsideTheRect( stp2 , rc ))
		{
			return 1;
		}
		else
		{
			float a2 = 	ComputeVectorAngle(  enp2 , stp2 ) ; // aux axis of the bend 	
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;		
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,1 );
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}
	return 0;
}

void  SetStright( CRail *p1,CRail *p2, CStraightRail *p, int endpoint1,int endpoint2)
{
		WORD  wID =	p->GetRailId();
		p->SetPosition( p1->GetPoint( endpoint1) ,p2->GetPoint( endpoint2 ) );  
		p1->SetRailConnId( endpoint1 , wID  );
		p2->SetRailConnId( endpoint2 , wID  );
		p->SetRailConnId ( 0 ,p1->GetRailId() );
 		p->SetRailConnId ( 1 ,p2->GetRailId() );
}

 int  SnapTwoObjectsWithStright( CRail *p1,CRail *p2, CStraightRail *p, CRect rc) 
{	
	if ( PointInsideTheRect( p1->GetPoint( 0 ) , rc ) )     
		{
			if ( PointInsideTheRect( p2->GetPoint( 0 ), rc ) )              
				SetStright( p1 , p2, p, 0 ,0 );
			else if ( PointInsideTheRect( p2->GetPoint( 1 ), rc ) )  											 
				SetStright( p1 , p2, p, 0 ,1 );
			else {
				if ( PointInsideTheRect( p2->GetPoint( 2 ), rc ) ) 
					if ( p2->mtype == ObjRailSwitch )
						SetStright( p1 , p2, p, 0 ,2 );	
				}
		}
		else if ( PointInsideTheRect( p1->GetPoint( 1 ) , rc ) ) 									 
		{
			if ( PointInsideTheRect( p2->GetPoint( 0 ) , rc ))			 
	 			SetStright( p1 , p2, p, 1 ,0 );
			else if ( PointInsideTheRect( p2->GetPoint( 1 ), rc ) )  									 
	 			SetStright( p1 , p2, p, 1 ,1 );
			else {
				if ( p2->mtype == ObjRailSwitch )
					if ( PointInsideTheRect( p2->GetPoint( 2 ), rc ) )  
						SetStright( p1 , p2, p, 1 ,2 );
				}
		}
		else { 
			if ( p1->mtype == ObjRailSwitch )
				if ( PointInsideTheRect( p1->GetPoint( 2 ) , rc ) ) 									 
				{
					if ( PointInsideTheRect( p2->GetPoint( 0 ) , rc ))			 
						SetStright( p1 , p2, p, 2 ,0 );
					else if ( PointInsideTheRect( p2->GetPoint( 1 ), rc ) )  									 
						SetStright( p1 , p2, p, 2 ,1 );
					else { 
						if ( p2->mtype == ObjRailSwitch )
							if ( PointInsideTheRect( p2->GetPoint( 2 ), rc ) )  
								SetStright( p1 , p2, p, 2 ,2 );
					}
			}
		}
	return 0;
}

//  ---------------------------
//
//		snapping to  'StraightObject'
//
//  ---------------------------
//  -------------------------------------------------------------
//  [STRIGHT] <->  STRIGHT  // Completed & tested
//		2014.07.15 - checked ID & optimized &   bug1 fixed  
//		2014.07.15 - documented 
//      2014.07.21 - second  method added
//      2014.07.22 - tested second  method added

 int  SnapTwoStrightObjects( CRail *p1,CRail *p2,CRect rc) 
 {
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();
	
	if ( PointInsideTheRect( stp1 , rc ) )    //    [X]--------->
	{
		float a1 = 	ComputeVectorAngle( enp1 , stp1   ) ; // main axis  <<========
		if ( PointInsideTheRect( stp2 , rc ) )
		{
			float a2 = 	ComputeVectorAngle( stp2 , enp2  ) ; // aux axis <<=======
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(stp1 ,0 );
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}		
		else  //   enp2
		{
			float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis  <<=======
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 	
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(stp1 ,1 );
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}
	else // PointInsideTheRect( enp1 , rc )             --------->[X]
	{	
		float a1 = 	ComputeVectorAngle( stp1 , enp1   ) ; // main axis  =========>>
		if ( PointInsideTheRect( stp2 , rc ))
		{
			float a2 = 	ComputeVectorAngle( stp2 , enp2   ) ; // aux axis  =======>
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 1; 
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,0 );
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
			float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis ======>
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 1; 
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(enp1 ,1 );
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );	
		}
	}
	return 0;
 }

//   

 int  GlueTwoStrightObjects(  CRail *p1,CRail *p2,CRect rc) 
 {
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();

	if ( PointInsideTheRect( stp1 , rc ) )     //      >>  [X]--------->
	{
		float a1 = 	ComputeVectorAngle( enp1 , stp1   ) ; // main axis from end to begin     <=======
		if ( PointInsideTheRect( stp2 , rc ) )               // <---------[x]
		{
			float a2 = 	ComputeVectorAngle( stp2 , enp2  ) ; // aux axis   <=======
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 2 ) return 1; 
			p1->SetPosition( stp2 ,enp1 ); // extend first stright  stp1 = stp2
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{													// aux axis  --------->[x]
			float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  //           <========
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 2 ) return 1; 
			p1->SetPosition( enp2 ,enp1 ); // extend first stright  stp1 = enp2
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}
	else									//      --------->[X]
	{
		float a1 = 	ComputeVectorAngle( stp1 , enp1   ) ; //   main axis from end to begin     ========>
		if ( PointInsideTheRect( stp2 , rc ))			//   [X]--------->
		{
			float a2 = 	ComputeVectorAngle( stp2 , enp2   ) ; // aux axis  //           =========>
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 2 )  return 1; 
			p1->SetPosition( stp1 , stp2 );// extend first stright  enp1 = stp2
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else										//    [x]<--------- 
		{
			float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis   =========>
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 2 )  return 1; 
			p1->SetPosition( stp1 , enp2 );// extend first stright  enp1 = enp2
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );	
		}
	}
	return 0;
 }


//		[STRIGHT] <<->  BEND   
//		2014 07 11 - initally completed & tested
//		2014.07.15 - optimized , IDs  compliance verified  
//		2014.07.15 - documented 
// 		2014.07.22 - new method tested 

 int  SnapBendToStrightObjects( CRail *p1,CRail *p2,CRect rc) 
 {
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();
 
	if ( PointInsideTheRect( stp1 , rc ) )
	{
		float a1 = 	ComputeVectorAngle( enp1 , stp1   ) ; // main axis   <----------
		if ( PointInsideTheRect( stp2 , rc ))
		{
			float a2 = 	ComputeVectorAngle( stp2 , p2->GetPoint(5)) ; // aux axis   <--------
		 	if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(stp1 ,0 );
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
			float a2 = ComputeVectorAngle( p2->GetPoint(6), enp2  ) ;  // aux axis  <--------
		 	if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 	
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(stp1 ,1 );
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 1 , wID1 );	
		}
	}
	else // PointInsideTheRect( enp1 , rc )
	{
		float a1 = 	ComputeVectorAngle( stp1 , enp1   ) ; // main axis	
		if ( PointInsideTheRect( stp2 , rc ))
		{
			float a2 = 	ComputeVectorAngle( stp2 , p2->GetPoint(5)) ; // aux axis 
		 	if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,0 );
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
			float a2 = ComputeVectorAngle(  p2->GetPoint(6) , enp2 ) ;  // aux axis 
		 	if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 	
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(enp1 ,1 );
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}	 
	return 0;
 }


// assuming p1 poimts to straights
int  GlueStrightToBendObjects(  CRail *p1,CRail *p2,CRect rc)
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();

	if ( PointInsideTheRect( stp1 , rc ) )
	{
		float a1 = 	ComputeVectorAngle( enp1 , stp1   ) ; // main axis from left to wright
		if ( PointInsideTheRect( stp2 , rc ) )
		{   // case checked
			float a2 = 	ComputeVectorAngle( stp2 , p2->GetPoint(5) ) ; // aux axis from left to wright
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 2 ) return 1; 
			p1->SetPosition( stp2 ,enp1 ); // extend first stright  stp1 = stp2
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{    // case checked
			float a2 = ComputeVectorAngle(   p2->GetPoint(6),  enp2   ) ;  //aux axis from left to wright
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 2 ) return 1; 
			p1->SetPosition( enp2 ,enp1 ); 	// extend first stright  stp1 = enp2
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}
	else
	{
		float a1 = 	ComputeVectorAngle( stp1 , enp1   ) ; // main axis from left to wright
		if ( PointInsideTheRect( stp2 , rc ))
		{  // case checked
			float a2 = 	ComputeVectorAngle( stp2 , p2->GetPoint(5) ) ; // aux axis from left to wright 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 2 )  return 1; 
			p1->SetPosition( stp1 , stp2 );// extend first stright  enp1 = stp2
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{   // checked
			float a2 = ComputeVectorAngle( p2->GetPoint(6)  , enp2  ) ;  // aux axis from left to wright 	
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 2 )  return 1; 
			p1->SetPosition( stp1 , enp2 ); // extend first stright  enp1 = enp2
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );	
		}
	}
	return 0;
}

//  [STRIGHT] <<->  SWITCH  
//		2014 07 11 - initally completed & tested partially w/o bend
//		2014.07.15 - optimized , IDs  compliance verified  
//		2014.07.16 - documented 
//		2014.07.18 - exp switch snap added 

 int  SnapSwitchToStrightObjects( CRail *p1,CRail *p2,CRect rc) 
 {
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	CFPoint enp3	= p2->GetPoint( 2 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();

	if ( PointInsideTheRect( stp1 , rc ) )
		{
			if ( ( PointInsideTheRect( enp3 , rc ) ) ) // extra switch end point
			{
				float a1 = 	ComputeVectorAngle( enp1, stp1 ) ; //reverse vector
				float a2 = 	ComputeVectorAngle( p2->GetPoint(6) , p2->GetPoint(2) ) ;  
				if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1;
				p2->RotateObject( a1-a2 ,0);
				p2->DisplaceObject(stp1 ,2 );
				p1->SetRailConnId( 0 , wID2 );
				p2->SetRailConnId( 2 , wID1 );
			}
			else
			{
				float a1 = 	ComputeVectorAngle( enp1 , stp1   ) ; // main axis
				if ( PointInsideTheRect( stp2 , rc ) )
				{
					float a2 = 	ComputeVectorAngle( stp2 , enp2  ) ; // aux axis 
					if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 	
					p2->RotateObject(a1-a2 ,0);
					p2->DisplaceObject(stp1 ,0);
					p1->SetRailConnId( 0 , wID2 );
					p2->SetRailConnId( 0 , wID1 );
				}		
				else
				{
					float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis 
					if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 			
					p2->RotateObject(a1-a2 ,1);
					p2->DisplaceObject(stp1 ,1);
					p1->SetRailConnId( 0 , wID2 );
					p2->SetRailConnId( 1 , wID1 );
				}	
			}
		}
	else
		{
			if ( ( PointInsideTheRect( enp3 , rc ) ) ) // extra switch end point
			{
				float a1 = 	ComputeVectorAngle( stp1 , enp1 ) ;
				float a2 = 	ComputeVectorAngle( p2->GetPoint(6) , p2->GetPoint(2) ) ;    
				if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;	
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(enp1 ,2);
				p1->SetRailConnId( 1 , wID2 );
				p2->SetRailConnId( 2 , wID1 );
			}
			else
			{
				float a1 = 	ComputeVectorAngle( stp1 , enp1   ) ; // main axis
				if ( PointInsideTheRect( stp2 , rc ))
				{
					float a2 = 	ComputeVectorAngle( stp2 , enp2   ) ; // aux axis 
					if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 1; 	
					p2->RotateObject(a1-a2 ,0);
					p2->DisplaceObject(enp1 ,0);				
					p1->SetRailConnId( 1 , wID2 );
					p2->SetRailConnId( 0 , wID1 );
				}
				else
				{
					float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis 
					if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 1; 				
					p2->RotateObject(a1-a2 ,1);
					p2->DisplaceObject(enp1 ,1);	
					p1->SetRailConnId( 1 , wID2 );
					p2->SetRailConnId( 1 , wID1 );	
				}
			}
		}
	return 0;
 }
 

//  [BEND] <->  BEND   
//		2014 07 11 - initally completed & tested partially w/o bend
//		2014.07.15 -  optimized , IDs  compliance verified  
//		2014.07.16   -  documented , snap taken
//		2014.07.22  - second method added
//      

 int  SnapTwoBendObjects( CRail *p1,CRail *p2,CRect rc) 
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();
 
	if ( PointInsideTheRect( stp1 , rc ) )
	{ 		
		if ( PointInsideTheRect( stp2 , rc ) )   ///   swapped
		{
			float a1 = 	ComputeVectorAngle( p1->GetPoint(5) ,stp1  ) ; // main axis
			float a2 = 	ComputeVectorAngle( stp2, p2->GetPoint(5)  ) ; // main axis
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(stp1 ,0);
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
			float a1 = 	ComputeVectorAngle( p1->GetPoint(5) ,stp1  ) ; // main axis
			float a2 = 	ComputeVectorAngle( p2->GetPoint(6) , enp2  ) ; // aux axis 
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(stp1 ,1);	
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 1 , wID1 ); 
		}
	}
	else
	{
		if ( PointInsideTheRect( stp2 , rc ) )
		{
			float a1 = 	ComputeVectorAngle( enp1 , p1->GetPoint(6)) ; // main axis
			float a2 = 	ComputeVectorAngle( stp2 , p2->GetPoint(5)) ; // aux axis 
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;		
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,0);
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
			float a1 = 	ComputeVectorAngle( enp1 , p1->GetPoint(6) ) ; // main axis
			float a2 = 	ComputeVectorAngle( p2->GetPoint( 6 ), enp2 ) ; // aux axis 	
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,1);
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}

	return 0;
}

//
//      [BEND] <<-> STRIGHT  // finished & tested
//		2014 07 11 - initally completed & tested partially w/o bend
//		2014.07.15 - optimized , IDs  compliance verified  
//		2014.07.16   -  documented / verified / snaps taken
//		2014.07.22   -  new method added

int  SnapStrightToBendObjects( CRail *p1,CRail *p2,CRect rc)
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();
 
	if ( PointInsideTheRect( stp1 , rc ) )
	{ 		
		if ( PointInsideTheRect( stp2 , rc ) )
		{
			float a1 = 	ComputeVectorAngle( p1->GetPoint(5) ,stp1  ) ; // main axis
			float a2 = 	ComputeVectorAngle( stp2, enp2  ) ; // main axis
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(stp1 ,0);
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
 			float a1 = 	ComputeVectorAngle( p1->GetPoint(5) ,stp1  ) ; // main axis
			float a2 = 	ComputeVectorAngle( enp2 , stp2  ) ; // aux axis 		
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(stp1 ,1);
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}
	else
	{
		if ( PointInsideTheRect( stp2 , rc ) )
		{
			float a1 = 	ComputeVectorAngle( enp1 , p1->GetPoint(6) ) ; // main axis
			float a2 = 	ComputeVectorAngle( stp2 , enp2) ; // aux axis 
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;	 	
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,0);
		 	p1->SetRailConnId( 1 , wID2 );
		 	p2->SetRailConnId( 0 , wID1 );
		}
		else
		{		
			float a1 = 	ComputeVectorAngle( enp1 , p1->GetPoint(6) ) ; // main axis
			float a2 = 	ComputeVectorAngle(  enp2 , stp2 ) ; // aux axis of the bend 	
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;	 
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,1);
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}
	return 0;
}

//
//      [BEND] <-> SWITCH
//		2014.07.16 - initiated
//		2014.07.16 - optimized , IDs  compliance verified  - documented 
//		2014.07.16 - snapswitch added

int  SnapSwitchToBendObjects( CRail *p1,CRail *p2,CRect rc)  
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;
	CFPoint enp1	= p1->GetPoint( 1 ) ;
	CFPoint stp2	= p2->GetPoint( 0 ) ;
	CFPoint enp2	= p2->GetPoint( 1 ) ;
	CFPoint exp2	= p2->GetPoint( 2 ) ;
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();

	if ( PointInsideTheRect( stp1 , rc ) )
	{ 
		if (   PointInsideTheRect( exp2 , rc )   ) // extra switch end point
		{
			float a1 = 	ComputeVectorAngle( p1->GetPoint(5) ,stp1 ) ;  
			float a2 = 	ComputeVectorAngle( p2->GetPoint(6) , p2->GetPoint(2) ) ;   
			if ( GetAbsAngleOfTwoVectors ( a1, a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(stp1 ,2);
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 2 , wID1 );
		}
		else
		{
			if ( PointInsideTheRect( stp2 , rc ) )
				{
				float a1 = 	ComputeVectorAngle( p1->GetPoint(5) ,stp1  ) ; // main axis
				float a2 = 	ComputeVectorAngle( stp2, enp2  ) ; // main axis
				if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;	
				p2->RotateObject(a1-a2 ,1);
				p2->DisplaceObject(stp1 ,0);
				p1->SetRailConnId( 0 , wID2 );
				p2->SetRailConnId( 0 , wID1 );				
				}
			else
			{
				float a1 = 	ComputeVectorAngle( p1->GetPoint(5) ,stp1  ) ; // main axis
				float a2 = 	ComputeVectorAngle( enp2 , stp2  ) ; // main axis of switch
				if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;			
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(stp1 ,1);
				p1->SetRailConnId( 0 , wID2 );
				p2->SetRailConnId( 1 , wID1 );
			}
		}
	}
	else
	{
		if (   PointInsideTheRect( exp2 , rc )   ) // snapping  switch to endpoint need extended line
		{
			float a1 = 	ComputeVectorAngle( enp1 , p1->GetPoint(6) ) ;
			float a2 = 	ComputeVectorAngle( p2->GetPoint(6) , exp2 ) ; 
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,2);
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 2 , wID1 );
		}
		else
		{
			if ( PointInsideTheRect( stp2 , rc ) )
			{
				float a1 = 	ComputeVectorAngle( enp1 , p1->GetPoint(6) ) ; // main axis
				float a2 = 	ComputeVectorAngle( stp2 , enp2) ; // aux axis 
				if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;	
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(enp1 ,0);
		 		p1->SetRailConnId( 1 , wID2 );
		 		p2->SetRailConnId( 0 , wID1 );
			}
			else
			{		
				float a1 = 	ComputeVectorAngle( enp1 , p1->GetPoint(6) ) ; // main axis
				float a2 = 	ComputeVectorAngle(  enp2 , stp2 ) ; // aux axis of the bend 	
				if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;	
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(enp1 ,1);
				p1->SetRailConnId( 1 , wID2 );
				p2->SetRailConnId( 1 , wID1 );
			}
		}
	}
	return 0;
}

//   [SWITCH] <-> SWITCH
//		2014.07.16 - initiated
//		2014.07.16 - optimized , IDs  compliance verified  - documented  
//      2014.07.17 - exp  points added 

int  SnapTwoSwitchObjects( CRail *p1,CRail *p2,CRect rc)  
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;  // sw
	CFPoint enp1	= p1->GetPoint( 1 ) ;  // sw
	CFPoint exp1	= p1->GetPoint( 2 ) ;  // sw
	CFPoint stp2	= p2->GetPoint( 0 ) ;  // sw2
	CFPoint enp2	= p2->GetPoint( 1 ) ;  // sw2
	CFPoint exp2	= p2->GetPoint( 2 ) ;  // sw2
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();

	if ( PointInsideTheRect( stp1 , rc ) )
	{  
		if (   PointInsideTheRect( exp2 , rc )   ) // extra switch end point
		{   
				float a1 = 	ComputeVectorAngle( enp1, stp1 ) ; //reverse vector
				float a2 = 	ComputeVectorAngle( p2->GetPoint(6) , exp2  ) ;  
		        if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;	
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(stp1 ,2);
				p1->SetRailConnId( 0 , wID2 );
				p2->SetRailConnId( 2 , wID1 );
		}
		else
		{
			if ( PointInsideTheRect( stp2 , rc ) )
			{
				float a1 = 	ComputeVectorAngle( enp1 , stp1   ) ; // main axis
				float a2 = 	ComputeVectorAngle( stp2 , enp2  ) ; // aux axis 
				if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1;	
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(stp1 ,0);
				p1->SetRailConnId( 0 , wID2 );
				p2->SetRailConnId( 0 , wID1 );
			}
			else
			{
				float a1 = 	ComputeVectorAngle( enp1 , stp1   ) ; // main axis
				float a2 = 	ComputeVectorAngle( enp2 , stp2   ) ; // aux axis 
				if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1;	
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(stp1 ,1);
				p1->SetRailConnId( 0 , wID2 );
				p2->SetRailConnId( 1 , wID1 );
			}
		}
	}
	else if ( PointInsideTheRect( enp1 , rc ) )  // this is connection to stright
	{
		if ( PointInsideTheRect( exp2 , rc ) )
		{
			float a1 = 	ComputeVectorAngle( stp1 , enp1 ) ;
			float a2 = 	ComputeVectorAngle( p2->GetPoint(6) , exp2  ) ;    
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,2);
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 2 , wID1 );
		}
		else
		{	
			if ( PointInsideTheRect( stp2 , rc ) )
			{
			float a1 = 	ComputeVectorAngle( stp1,enp1  ) ; // main axis
			float a2 = 	ComputeVectorAngle( stp2 , enp2) ; // aux axis 
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;	 	
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,0);
		 	p1->SetRailConnId( 1 , wID2 );
		 	p2->SetRailConnId( 0 , wID1 );
			}
			else
			{		
			float a1 = 	ComputeVectorAngle( stp1,enp1  ) ; // main axis
			float a2 = 	ComputeVectorAngle(  enp2 , stp2 ) ; // aux axis of the bend 	
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;	
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,1);
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
			}
		}
	}
	else if ( PointInsideTheRect( exp1 , rc ) )  // might be bend point
	{
		if ( PointInsideTheRect( exp2 , rc ) )
		{  // only exp to exp possible
			float a1 = 	ComputeVectorAngle( exp1, p1->GetPoint(6)) ; //reverse vector
			float a2 = 	ComputeVectorAngle( p2->GetPoint(6) , exp2 ) ;  
			if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(exp1 ,2);
			p1->SetRailConnId( 2 , wID2 );
			p2->SetRailConnId( 2 , wID1 );

		}
		else
		{
			float a1 = 	ComputeVectorAngle( exp1, p1->GetPoint(6) ) ; //reverse vector
			if ( PointInsideTheRect( stp2 , rc ) ) 
			{
				float a2 = 	ComputeVectorAngle( stp2, enp2 ) ;  
				if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;	
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(exp1 ,0);
				p1->SetRailConnId( 2 , wID2 );
				p2->SetRailConnId( 0 , wID1 );
			}
			else
			{
				float a2 = 	ComputeVectorAngle( enp2, stp2  ) ; 
				if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(exp1 ,1);
				p1->SetRailConnId( 2 , wID2 );
				p2->SetRailConnId( 1 , wID1 );
			}
		}
	}
	return 0;
}

//		[SWITCH] <<-> STRIGHT
//		2014.07.15 - checked ID & optimized & bug1 fixed
//		2014.07.15 - documented  
//		2014.07.16 - snap taken for case 3

int  SnapStrightToSwitchObjects( CRail *p1,CRail *p2,CRect rc)  
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;  // sw
	CFPoint enp1	= p1->GetPoint( 1 ) ;  // sw
	CFPoint exp1	= p1->GetPoint( 2 ) ;  // sw
	CFPoint stp2	= p2->GetPoint( 0 ) ;  // straight
	CFPoint enp2	= p2->GetPoint( 1 ) ;  // straight
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();

	if ( PointInsideTheRect( stp1 , rc ) )  // start endpoint
	{ 	
		float a1 = 	ComputeVectorAngle( enp1 , stp1   ) ; // main axis
		if ( PointInsideTheRect( stp2 , rc ) )
		{
			float a2 = 	ComputeVectorAngle( stp2 , enp2  ) ; // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(stp1 ,0);
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}		
		else
		{
			float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1; 
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(stp1 ,1);
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 1 , wID1 );
		}
	}
	else if ( PointInsideTheRect( enp1 , rc ) )  // end endpoint
	{	

		float a1 = 	ComputeVectorAngle( stp1 , enp1   ) ; // main axis
		if ( PointInsideTheRect( stp2 , rc ))
		{
			float a2 = 	ComputeVectorAngle( stp2 , enp2   ) ; // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 1; 
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,0);
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
			float a2 = ComputeVectorAngle( enp2  , stp2  ) ;  // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 1; 
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(enp1 ,1);
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 1 , wID1 );	
		}
	}
	else if ( PointInsideTheRect( exp1 , rc ) )  //  bend endpoint
	{	
		float a1 = 	ComputeVectorAngle( exp1, p1->GetPoint(6) ) ; //reverse vector
		if ( PointInsideTheRect( stp2 , rc ) ) 
			{
				float a2 = 	ComputeVectorAngle( stp2, enp2 ) ;  
				if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(exp1 ,0);
				p1->SetRailConnId( 2 , wID2 );
				p2->SetRailConnId( 0 , wID1 );
			}
		else
			{
				float a2 = 	ComputeVectorAngle( enp2, stp2  ) ; 
				if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(exp1 ,1);
				p1->SetRailConnId( 2 , wID2 );
				p2->SetRailConnId( 1 , wID1 );
			}
	}
	return 0;
}

//   [SWITCH] <<-> BEND
//		2014.07.16 - initiated
//		2014.07.16 - tested optimize , snap taken ,documented
//      


int  SnapBendToSwitchObjects( CRail *p1,CRail *p2,CRect rc) 
{
	CFPoint stp1	= p1->GetPoint( 0 ) ;  // sw
	CFPoint enp1	= p1->GetPoint( 1 ) ;  // sw
	CFPoint exp1	= p1->GetPoint( 2 ) ;  // sw
	CFPoint stp2	= p2->GetPoint( 0 ) ;  // straight
	CFPoint enp2	= p2->GetPoint( 1 ) ;  // staright
	WORD  wID1		= p1->GetRailId();
	WORD  wID2		= p2->GetRailId();
 
	if ( PointInsideTheRect( stp1 , rc ) )  // this is connection to start
	{   
		float a1 = 	ComputeVectorAngle( enp1 , stp1   ) ; // main axis
		if ( PointInsideTheRect( stp2 , rc ) ) 
		{
			float a2 = 	ComputeVectorAngle( stp2 , p2->GetPoint(5)) ; // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(stp1 ,0);
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
			float a2 = ComputeVectorAngle( p2->GetPoint(6), enp2  ) ;  // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(stp1 ,1);
			p1->SetRailConnId( 0 , wID2 );
			p2->SetRailConnId( 1 , wID1 );	
		}
	}
	else if ( PointInsideTheRect( enp1 , rc ) )  // this is connection to end
	{	
		float a1 = 	ComputeVectorAngle( stp1 , enp1   ) ; // main axis
		if ( PointInsideTheRect( stp2 , rc ) ) 
		{
			float a2 = 	ComputeVectorAngle( stp2 , p2->GetPoint(5)) ; // aux axis 		
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 )  return 1;
			p2->RotateObject(a1-a2 ,0);
			p2->DisplaceObject(enp1 ,0);
			p1->SetRailConnId( 1 , wID2 );
			p2->SetRailConnId( 0 , wID1 );
		}
		else
		{
			float a2 = ComputeVectorAngle( p2->GetPoint(6), enp2  ) ;  // aux axis 
			if ( GetAbsAngleOfTwoVectors( a1 , a2 ) > 60 ) return 1;
			p2->RotateObject(a1-a2 ,1);
			p2->DisplaceObject(enp1 ,1);
			p1->SetRailConnId( 1 , wID2 );  // bug chganged to 1
			p2->SetRailConnId( 1 , wID1 );	
		}
	}
	else if ( PointInsideTheRect( exp1 , rc ) )  // might be bend point
	{
		float a1 = 	ComputeVectorAngle( exp1, p1->GetPoint(6) ) ;  
		if ( PointInsideTheRect( stp2 , rc ) ) 
			{
				float a2 = 	ComputeVectorAngle( stp2,  p2->GetPoint(5) ) ;  
				if ( GetAbsAngleOfTwoVectors (   a1,   a2 ) > 60 ) return 1;
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(exp1 ,0);
				p1->SetRailConnId( 2 , wID2 );
				p2->SetRailConnId( 0 , wID1 );
			}
		else
			{	
				float a2 = 	ComputeVectorAngle( p2->GetPoint(6) , enp2  ) ; 
				if ( GetAbsAngleOfTwoVectors ( a1, a2 ) > 60 ) return 1;	
				p2->RotateObject(a1-a2 ,0);
				p2->DisplaceObject(exp1 ,1);
				p1->SetRailConnId( 2 , wID2 );
				p2->SetRailConnId( 1 , wID1 );
			}
	}	
	return 0;
}


 int  HitSingleObject(  CRail *p ) 
  {
		if (  p->mtype == ObjStraightRail ) 
		{

			//TurnStraight (  p , 90 , 1 );  

			 CFPoint new_destination = CFPoint (100,100);
			 //MoveStraight (  p , new_destination , 1);   // move start point
		}


		if (  p->mtype == ObjBendRail ) 
		{
		    

						CFPoint fp = p->GetPoint( 6 ) ;
		   //CFPoint fp = ComputeExtendedLinePointBend(  p );

			//p->SetDebugPoint(3, p->GetPoint(3) ); //  
		   //  p->SetDebugPoint(0, p->GetPoint(1));  
		 	// p->SetDebugPoint(1, fp ); // 
			//p->SetDebugPoint(2, p->GetPoint(3) ); //  

 		 p->SetDebugPoint(0, p->GetPoint(0));  
		   	p->SetDebugPoint(1, p->GetPoint(5));  


			//	p->SetDebugPoint(2, p->GetPoint(0));  
		 	//  p->SetDebugPoint(3, p->GetPoint(5));  	

		    // turn around origin
		 //TurnBend (  p , 90 , 0 );  // turn around 0 point
        //TurnBend (  p , 45 , 1 );      // turn around 1 point
		 //UpdatePaintRailBendStruct( p->GetDrawDataStructPtr() , 0  );

 

		//CFPoint new_destination = CFPoint (100,100);

	 //	 MoveBend (  p , new_destination , 1);  // move endpoint to - separate for moving start point 
	//	 UpdatePaintRailBendStruct( p->GetDrawDataStructPtr() , 0  );

		  	p->SetDebugPoint(4, p->GetPoint(0));  
		 	p->SetDebugPoint(5, p->GetPoint(5));  	


		 

	   }

	   if (  p->mtype == ObjRailSwitch ) 
	   {

		   //  turn  the switch
		   // TurnSwitch ( p , 180 ,0);
          //CFPoint pt = ComputeExtendedLinePointSwitch( p );

 
//	CFPoint CenterPoint	= p->GetPoint(3);
//	CFPoint StartPoint	= p->GetPoint(0);
//	CFPoint EndPoint	= p->GetPoint(2) ;


		//	p->SetDebugPoint(0, p->GetPoint(2));  
		//	p->SetDebugPoint(1, pt ); 
		//	p->SetDebugPoint(2, p->GetPoint(3) ); 
		//	p->SetDebugPoint(3, p->GetPoint(0) ); 

	   }


	  return 0;
  }



 
 CFVector NormalizeStrightVector(CRail  *p ,CRect rc )
 {
	CFVector mVector;  // CPoint only need to redesign
	if ( PointInsideTheRect( p->GetPoint( 0 ) , rc ) )
	{	mVector.mStart = p->GetPoint( 1 );	mVector.mEnd = p->GetPoint( 0 );}
	else
	{	mVector.mStart = p->GetPoint( 0 ); mVector.mEnd  = p->GetPoint( 1 );  }
	return  mVector;
 }
 
 CFVector NormalizeBendVector(CRail  *p ,CRect rc )
 {
	CFVector mVector;  // CPoint only need to redesign
	if ( PointInsideTheRect( p->GetPoint( 0 ) , rc ) )
	{	mVector.mStart = p->GetPoint( 5 );	mVector.mEnd = p->GetPoint( 0 );}
	else
	{	mVector.mStart = p->GetPoint( 7 ); mVector.mEnd  = p->GetPoint( 1 );  }
	return  mVector;
 }


  CFVector NormalizeSwitchVector(CRail  *p ,CRect rc )
 {
		CFVector mVector;  // CPoint only need to redesign
	if ( PointInsideTheRect( p->GetPoint( 0 ) , rc ) )
	{	mVector.mStart = p->GetPoint( 1 );	mVector.mEnd = p->GetPoint( 0 );}
	else if ( PointInsideTheRect( p->GetPoint( 1 ) , rc ) )
	{	mVector.mStart = p->GetPoint( 0 ); mVector.mEnd  = p->GetPoint( 1 );  }
	else
	{	mVector.mStart = p->GetPoint( 7 ); mVector.mEnd  = p->GetPoint( 2 );  }
	return  mVector;
 }

 
int ConnectObjectsWithBend( CRail *p1,CRail *p2,CRail *p3,CRect rc) 
 {
	 DRAWDATASTRUCT  dat;
	 
     CFVector mVector1,mVector2;

	 if ( p1->mtype == ObjBendRail )	mVector1 =  NormalizeBendVector( p1 ,  rc );
	 else if ( p1->mtype == ObjRailSwitch )	mVector1 = NormalizeSwitchVector( p1 ,  rc );
	 else mVector1 = NormalizeStrightVector( p1 ,  rc );

	 if ( p2->mtype == ObjBendRail )	mVector2 =  NormalizeBendVector( p2 ,  rc );
	 else if ( p2->mtype == ObjRailSwitch )	mVector2 = NormalizeSwitchVector( p2 ,  rc );
	 else mVector2 = NormalizeStrightVector( p2 ,  rc );

	 mDebug.CDebugPut("ConnectObjectsWithBend: typ1:%d typ2:%d  " ,p1->mtype,p2->mtype);


	 int ivect = ComputeBendToLink1(mVector1.mStart,mVector1.mEnd ,mVector2.mStart,mVector2.mEnd ,&dat);

	 if ( (dat.Radius < 90 )  || (ivect == 0 ) ) return 1;

	if ( abs(dat.mStartAngle - dat.mSweepAngle) < 2.0 ) 
		{
	mDebug.CDebugPut("!!! < 2.0 angle " );
			return 4;
	}
	if (  (abs(dat.mStartAngle - dat.mSweepAngle) < 182.0 )  && (  abs(dat.mStartAngle - dat.mSweepAngle) > 178.0) ) 
	{
			mDebug.CDebugPut("!!! < 180.0 angle " );
		return 5;
	}

	// optimalization
	if (  (dat.mStartAngle -dat.mSweepAngle) > 0.0)  // dat.mStartAngle > dat.mSweepAngle
	{
		if (  (dat.mStartAngle -dat.mSweepAngle) <= 180.0)
			{ p3->SetObject( p3->GetMode(),dat.Radius,dat.mStartAngle, dat.mSweepAngle - dat.mStartAngle ); }  // case 1
		else //  
			{ p3->SetObject( p3->GetMode(),dat.Radius,dat.mStartAngle, dat.mSweepAngle - dat.mStartAngle + (float)360.0 );}
	}
	else  // dat.mSweepAngle > dat.mStartAngle
	{	
		if ( (dat.mSweepAngle - dat.mStartAngle) <= (float)180.0 )
			{ p3->SetObject( p3->GetMode(),dat.Radius,dat.mStartAngle, dat.mSweepAngle - dat.mStartAngle ); }
		else
			{ p3->SetObject( p3->GetMode(),dat.Radius,dat.mStartAngle, dat.mSweepAngle - dat.mStartAngle - (float)360.0 ); }
	}

	CFPoint pt;
	pt  =  GetPointFmStart( mVector1.mStart, mVector1.mEnd, GetDistance ( mVector1.mStart, mVector1.mEnd  ) + 200);
	p3->SetPosition( dat.StartPoint ,pt);

	 WORD  bID = p3->GetRailId();
	 WORD  s1ID = p1->GetRailId();
	 WORD  s2ID = p2->GetRailId();

	 if (   GetDistance( p1->GetPoint(0) , p3->GetPoint(0))  < 2 )
	 { 
		p1->SetRailConnId(0 , bID ) ;  
		p3->SetRailConnId(0, s1ID); 
		if ( p1->mtype == ObjStraightRail )  p1->SetPoint( 0 , p3->GetPoint(0));
		if ( p3->mtype == ObjStraightRail )  p3->SetPoint( 0 , p1->GetPoint(0));
	 }

	 if (   GetDistance( p1->GetPoint(0) , p3->GetPoint(1))  < 2 )
	 { 
		p1->SetRailConnId(0 , bID ) ;  
		p3->SetRailConnId(1,s1ID); 
		if ( p1->mtype == ObjStraightRail )  p1->SetPoint( 0 , p3->GetPoint(1));
		if ( p3->mtype == ObjStraightRail )  p3->SetPoint( 1 , p1->GetPoint(0));
	 }

	 if (   GetDistance( p1->GetPoint(1) , p3->GetPoint(0))  < 2 )
	 { 
		p1->SetRailConnId(1 , bID ) ;  
		p3->SetRailConnId(0,  s1ID);
		if ( p1->mtype == ObjStraightRail )  p1->SetPoint( 1 , p3->GetPoint(0));
		if ( p3->mtype == ObjStraightRail )  p3->SetPoint( 0 , p1->GetPoint(1));
	 }

	 if (   GetDistance( p1->GetPoint(1) , p3->GetPoint(1))  < 2 )
	 { 
		p1->SetRailConnId(1 , bID ) ;  
		p3->SetRailConnId(1,  s1ID); 
		if ( p1->mtype == ObjStraightRail )  p1->SetPoint( 1 , p3->GetPoint(1));
		if ( p3->mtype == ObjStraightRail )  p3->SetPoint( 1 , p1->GetPoint(1));
	 }

	if ( p1->mtype == ObjRailSwitch ) 
		 if (   GetDistance( p1->GetPoint(2) , p3->GetPoint(0))  < 2 )
			 { p1->SetRailConnId(2 , bID ) ;  p3->SetRailConnId(0,s1ID);  }
	if ( p1->mtype == ObjRailSwitch ) 
		 if (   GetDistance( p1->GetPoint(2) , p3->GetPoint(1))  < 2 )
			 { p1->SetRailConnId(2 , bID ) ;  p3->SetRailConnId(1,s1ID);  }
///////

	 if (   GetDistance( p2->GetPoint(0) , p3->GetPoint(0))  < 2 )
	 { 
		p2->SetRailConnId(0 , bID ) ;  
		p3->SetRailConnId(0,s2ID);
		if ( p2->mtype == ObjStraightRail )  p2->SetPoint( 0 , p3->GetPoint(0));
		if ( p3->mtype == ObjStraightRail )  p3->SetPoint( 0 , p2->GetPoint(0));
	 }

	 if (   GetDistance( p2->GetPoint(0) , p3->GetPoint(1))  < 2 )
	 { 
		p2->SetRailConnId(0 , bID ) ;  
		p3->SetRailConnId(1,s2ID);
		if ( p2->mtype == ObjStraightRail )  p2->SetPoint( 0 , p3->GetPoint(1));
		if ( p3->mtype == ObjStraightRail )  p3->SetPoint( 1 , p2->GetPoint(0));

	 }
 
	 if (   GetDistance( p2->GetPoint(1) , p3->GetPoint(0))  < 2 )
	 {
		p2->SetRailConnId(1 , bID ) ; 
		p3->SetRailConnId(0,s2ID); 
		if ( p2->mtype == ObjStraightRail )  p2->SetPoint( 1 , p3->GetPoint(0));
		if ( p3->mtype == ObjStraightRail )  p3->SetPoint( 0 , p2->GetPoint(1));

	 }

	 if (   GetDistance( p2->GetPoint(1) , p3->GetPoint(1))  < 2 )
	 { 
		p2->SetRailConnId(1 , bID ) ;  
		p3->SetRailConnId(1,s2ID);
		if ( p2->mtype == ObjStraightRail )  p2->SetPoint( 1 , p3->GetPoint(1));
		if ( p3->mtype == ObjStraightRail )  p3->SetPoint( 1 , p2->GetPoint(1));
	 }

	if ( p2->mtype == ObjRailSwitch ) 
		if (   GetDistance( p2->GetPoint(2) , p3->GetPoint(0))  < 2 )
		 { p2->SetRailConnId(2 , bID ) ;  p3->SetRailConnId(0,s2ID);  }

	 if ( p2->mtype == ObjRailSwitch ) 
		if (   GetDistance( p2->GetPoint(2) , p3->GetPoint(1))  < 2 )
		{ p2->SetRailConnId(2 , bID ) ;  p3->SetRailConnId(1,s2ID);  }

	 return 0;
 }