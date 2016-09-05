




int  WM_LBUTTONUP_Handle_FuntionSelObject( CPoint mpoint ) ;
int  WM_LBUTTONUP_Handle_FuntionMoveObject( CPoint mpoint ) ;
int  WM_LBUTTONUP_Handle_FuntionRotObjectStp( CPoint mpoint ); 
int  WM_LBUTTONUP_Handle_FuntionRotObjectEnp( CPoint mpoint ); 
int  WM_LBUTTONUP_Handle_FunctionAddStright( CPoint mpoint ) ;
int  WM_LBUTTONUP_Handle_FunctionAddBend( CPoint mpoint ) ;
int  WM_LBUTTONUP_Handle_FunctionAddSwitch( CPoint mpoint ); 
int  WM_LBUTTONUP_Handle_FunctionAddEnd( CPoint mpoint ); 
int  WM_LBUTTONUP_Handle_FunctionAddVagon( CPoint mpoint );
int  WM_MOUSEMOVE_Handle_FuntionMoveObject( CPoint mpoint ) ;
int  WM_MOUSEMOVE_Handle_FuntionRotObjectStp( CPoint mpoint ) ;
int  WM_MOUSEMOVE_Handle_FuntionRotObjectEnp( CPoint mpoint ); 
int  WM_MOUSEMOVE_Handle_FunctionTracker( CPoint mpoint ) ;
int  WM_MOUSEMOVE_Handle_FunctionSaveRect( CPoint mpoint ) ;
int  WM_LBUTTONUP_Handle_FunctionAddSemaphore( CPoint mpoint );
int  WM_MOUSEMOVE_Handle_FunctionAddSemaphore( CPoint mpoint ); 


void HandleReferenceLineStore( int lineno );
void DeactivateObject();
void HandleReferenceLineStore( int lineno );
int  InvalidateCurrentObjectState(  CRail *p  );

// upon release
int CheckMouseHit( CPoint mpoint , int mode );

RECT GetLastCRect();
CFPoint  GetLastPoint();
 
					 
