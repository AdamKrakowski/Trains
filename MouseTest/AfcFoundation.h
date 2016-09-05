
#pragma once

#include "stdafx.h"

 //  AfcFundation -------


 class CPoint {
 public:
	 CPoint();
	 CPoint(int x,int y);
	 int x,y;
 };


 class CRect  {
 public:
	 CRect();
	 CRect( int Left, int  Top,int Right,int Bottom );
	 CRect( CPoint p1 , CPoint p2);
	 CRect( RECT rc);
	 RECT  thisRect;
 

 public:
	 void NormalizeRect();
	 void InflateRect( int value);
	 void UnionRect(CRect CRect1,CRect CRect2  );
	 int left,top,right,bottom;
	 RECT GetRect();

 };
 

 


class CArchive
{
public:
	CArchive();
	CArchive(char *filename, int bwrite );
	CArchive::~CArchive();
	void SetFilename(char *filename);
	int OpenArchiveToRead();
	int OpenArchiveToWrite();
	int CloseArchive();
	int Write( int count , void *p);
	int Read(  int count, void *p);

	void CArchive::operator>>(const  DWORD &ivalue);
	void CArchive::operator>>(const  BYTE &bvalue);
	void CArchive::operator>>(const  WORD &wvalue);

	void CArchive::operator<<(const  DWORD &ivalue);
	void CArchive::operator<<(const  BYTE &bvalue);
	void CArchive::operator<<(const  WORD &wvalue);

	virtual int IsStoring();
private:
 
	int system_error;
	int is_open;
	int istore;

	char FileName[MAX_PATH];
	int  mError;
	int  mFilename;
	HANDLE hFile;
	HWND   mHwnd;

};


 //  AfcFundation -------|
