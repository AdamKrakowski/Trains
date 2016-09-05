

//#include "Objects.h"


#define MAX_ENGINEES  12
#define MAX_VAGONS    24

 


class CCollection
{
 public:
	  CCollection();
	 ~CCollection();
	  int   CCollection::LoadEnginees() ;
	  int   CCollection::LoadVagons(); 
      HANDLE GetVagonHandle(int ipos);
      HANDLE GetEngineHandle(int ipos);
	  SIZE   GetVagonSize( int ipos);
	  SIZE   GetEngineSize( int ipos);

private:
		HANDLE bmpHandleE[MAX_ENGINEES+1];
		HANDLE bmpHandleV[MAX_VAGONS+1];
		int    ibmpESizeX[MAX_ENGINEES+1 ];
		int    ibmpESizeY[MAX_ENGINEES+1 ];
		int    ibmpVSizeX[MAX_VAGONS+1 ];
		int    ibmpVSizeY[MAX_VAGONS+1 ];
		int mEnginees;
		int mVagons;

};