
#if !defined(AFX_WBBUTTONPNG_H__518122CF_358F_11D4_8F4F_00402656D980__INCLUDED_)
#define AFX_WBBUTTONPNG_H__518122CF_358F_11D4_8F4F_00402656D980__INCLUDED_

#include "WBButton.h"

#include <atlimage.h>
using namespace ATL; 



// This class implements a Window Blinds like button
class CWBButtonPng : public CWBButton
{

public:
	CWBButtonPng();
	CWBButtonPng(int nID);
	virtual ~CWBButtonPng();


	//{{AFX_VIRTUAL(CWBButtonPng)
public:
	//}}AFX_VIRTUAL

	virtual void DrawBitmap( CDC * pDC, int mode );
	void PaintBk(CDC* pDC);

	virtual bool LoadBitmaps(LPCTSTR lpBitMapFile, 
							 int count ,    
							 int TopHeight, 
							 int BottomHeight, 
							 int LeftWidth, 
							 int RightWidth,
							 int height, 
							 int width, 
							 LPCTSTR lpImageUnderBK = NULL);

private:
	CImage			m_ImageUnderBK;	// µ×²ã±³¾°Í¼Æ¬
	CImage			m_ImageBK;		// ±³¾°Í¼Æ¬
	CDC				m_dcBk;
	CBitmap			m_bmpBk;
	CBitmap*		m_pbmpOldBk;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++

#endif // !defined(AFX_WBBUTTONPNG_H__518122CF_358F_11D4_8F4F_00402656D980__INCLUDED_)
