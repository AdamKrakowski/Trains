


#pragma once

//  Public colours definition

COLORREF GetBlackColor();
COLORREF GetGrayColor();
COLORREF GetRedColor();
COLORREF GetBlueColor();

COLORREF GetBlueDarkColor();
COLORREF GetRedDarkColor();
COLORREF GetWhiteColor();
COLORREF GetBraunColor();

// Public to draw cross marks (+, x) for CPOint or extended CFPoint (float)

void DrawPointPlus( CPoint cp , HDC hdc , COLORREF clr , int weight);
void DrawPointCross( CPoint cp , HDC hdc , COLORREF clr , int weight);

void DrawPointPlus( CFPoint cp , HDC hdc , COLORREF clr , int weight) ;
void DrawPointCross( CFPoint cp , HDC hdc , COLORREF clr , int weight); 
 
//  Draw line specified by A,B,C cooeficient Ax+By+C=0;
void  DrawTmpSegment ( CTwoPoints mTwoPoint   , HDC hdc );


void	DrawLine ( CLineObject mLine   , HDC hdc );
void	DrawLine ( CLineObject mLine   , HDC hdc , COLORREF rf  );

// Draw rectangle

void OnDrawRect( CRect  *srect , HDC hdc );
void DrawRectTilted( CPoint p1, CPoint p2, CPoint p3, CPoint p4  , HDC hdc );

//  to draw basic  primitives
void PaintBendRail( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc );
void PaintStraightRail( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc );
void PaintRailSwitch( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc );
void PaintEndRail( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc  );

void	PaintTrain( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc  );
void PaintTrainBitmap( CFPoint StartPoint,CFPoint EndPoint,HBITMAP bmpExercising , HDC hdc );
void PaintSemaphore( DRAWDATASTRUCT *p, DWORD mode ,  HDC hdc );

void ComputeBandingRectBend( DRAWDATASTRUCT *p  );
void ComputeBandingRectSwitch( DRAWDATASTRUCT *p  );
void ComputeBandingRectTrain( DRAWDATASTRUCT *p  );