
#include "stdafx.h"
#include "WBButton.h"
#include ".\wbbutton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HRGN CWBButton::BitmapRegion(HBITMAP hBitmap,COLORREF cTransparentColor,BOOL bIsTransparent)
{
	// We create an empty region
	HRGN		hRegion=NULL;
	
	// If the passed bitmap is NULL, go away!
	if(!hBitmap)
		return hRegion;
	
	// We create a memory context for working with the bitmap
	// The memory context is compatible with the display context (screen)
	HDC			hMemDC=CreateCompatibleDC(NULL);
	
	// If no context is created, go away, too!
	if(!hMemDC) 
		return hRegion;
		
	// Computation of the bitmap size
	BITMAP		bmBitmap;
	
	GetObject(hBitmap, sizeof(bmBitmap), &bmBitmap);
	
	// In order to make the space for the region, we
	// create a bitmap with 32bit depth color and with the
	// size of the loaded bitmap!
	BITMAPINFOHEADER RGB32BITSBITMAPINFO=
	{ 
		sizeof(BITMAPINFOHEADER), 
		bmBitmap.bmWidth, 
		bmBitmap.bmHeight, 
		1,32,BI_RGB,0,0,0,0,0 
	};
	
	// Here is the pointer to the bitmap data
	VOID		*pBits;
	
	// With the previous information, we create the new bitmap!
	HBITMAP		hNewBitmap;
	hNewBitmap=CreateDIBSection(hMemDC,
								(BITMAPINFO *)&RGB32BITSBITMAPINFO,
								DIB_RGB_COLORS,&pBits,NULL,0);

	// If the creation process succeded...
	if(hNewBitmap)	
	{
		// We select the bitmap onto the created memory context
		// and then we store the previosly selected bitmap on this context!
		HBITMAP		hPrevBmp=(HBITMAP) SelectObject(hMemDC,hNewBitmap);
		
		// We create another device context compatible with the first!
		HDC			hDC=CreateCompatibleDC(hMemDC);
		
		// If success...
		if(hDC) 
		{		
			// We compute the number of bytes per row that the bitmap contains, rounding to 32 bit-multiples
			BITMAP		bmNewBitmap;
			
			GetObject(hNewBitmap,sizeof(bmNewBitmap),&bmNewBitmap);
				
			while(bmNewBitmap.bmWidthBytes % 4) 
				bmNewBitmap.bmWidthBytes++;
			
			// Copy of the original bitmap on the memory context!
			HBITMAP		hPrevBmpOrg=(HBITMAP) SelectObject(hDC,hBitmap);
			BitBlt(hMemDC,0,0,bmBitmap.bmWidth,bmBitmap.bmHeight,hDC,0,0,SRCCOPY);

			// In order to optimize the code, we don't call the GDI each time we
			// find a transparent pixel. We use a RGN_DATA structure were we store
			// consecutive rectangles, until we have a large amount of them and then we crete
			// the composed region with ExtCreateRgn(), combining it with the main region.
			// Then we begin again initializing the RGN_DATA structure and doing another
			// iteration, until the entire bitmap is analyzed.

			// Also, in order to not saturate the Windows API with calls for reserving
			// memory, we wait until NUMRECT rectangles are stores in order to claim
			// for another NUMRECT memory space!
            #define NUMRECT	100			
			DWORD maxRect = NUMRECT;
			
			// We create the memory data
			HANDLE hData=GlobalAlloc(GMEM_MOVEABLE,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect));
			RGNDATA *pData=(RGNDATA*) GlobalLock(hData);
			pData->rdh.dwSize=sizeof(RGNDATAHEADER);
			pData->rdh.iType=RDH_RECTANGLES;
			pData->rdh.nCount=pData->rdh.nRgnSize=0;
			SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);
			
			// We study each pixel on the bitmap...
			BYTE *Pixeles=(BYTE*) bmNewBitmap.bmBits+(bmNewBitmap.bmHeight-1)*bmNewBitmap.bmWidthBytes;
			
			// Main loop
			for(int Row=0;Row<bmBitmap.bmHeight;Row++) 
			{
				// Horizontal loop
				for(int Column=0;Column<bmBitmap.bmWidth;Column++)
				{		
					// We optimized searching for adyacent transparent pixels!
					int Xo=Column;
					LONG *Pixel=(LONG*) Pixeles+Column;

					while(Column<bmBitmap.bmWidth) 
					{
						BOOL bInRange=FALSE;

						// If the color is that indicated as transparent...
						if(	GetRValue(*Pixel)==GetRValue(cTransparentColor) &&
							GetGValue(*Pixel)==GetGValue(cTransparentColor) &&
							GetBValue(*Pixel)==GetBValue(cTransparentColor) )
							bInRange=TRUE;

						if((bIsTransparent) && (bInRange)) 
							break;

						if((!bIsTransparent) && (!bInRange)) 
							break;

						Pixel++;
						Column++;
					} // while (Column < bm.bmWidth) 		
					
					if(Column>Xo) 
					{
						// We add the rectangle (Xo,Row),(Column,Row+1) to the region

						// If the number of rectangles is greater then NUMRECT, we claim
						// another pack of NUMRECT memory places!
						if (pData->rdh.nCount>=maxRect)
						{
							GlobalUnlock(hData);
							maxRect+=NUMRECT;
							hData=GlobalReAlloc(hData,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect),GMEM_MOVEABLE);
							pData=(RGNDATA *)GlobalLock(hData);					
						}		
						
						RECT *pRect=(RECT*) &pData->Buffer;
						SetRect(&pRect[pData->rdh.nCount],Xo,Row,Column,Row+1);		
								
						if(Xo<pData->rdh.rcBound.left) 
							pData->rdh.rcBound.left=Xo;

						if(Row<pData->rdh.rcBound.top) 
							pData->rdh.rcBound.top=Row;

						if(Column>pData->rdh.rcBound.right) 
							pData->rdh.rcBound.right=Column;
						
						if(Row+1>pData->rdh.rcBound.bottom) 
							pData->rdh.rcBound.bottom=Row+1;
		
						pData->rdh.nCount++;																	

						// In Win95/08 there is a limitation on the maximum number of
						// rectangles a RGN_DATA can store (aprox. 4500), so we call
						// the API for a creation and combination with the main region
						// each 2000 rectangles. This is a good optimization, because
						// instead of calling the routines for combining for each new
						// rectangle found, we call them every 2000 rectangles!!!
						if(pData->rdh.nCount==2000)
						{						
							HRGN hNewRegion=ExtCreateRegion(NULL,sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRect),pData);
							if (hNewRegion) {
								// Si ya existe la regi髇 principal,sumamos la nueva,
								// si no,entonces de momento la principal coincide con
								// la nueva regi髇.
								if (hRegion) {
									CombineRgn(hRegion,hRegion,hNewRegion,RGN_OR);									
									DeleteObject(hNewRegion);
								} else
									hRegion=hNewRegion;
								
								
							}
							// Volvemos a comenzar la suma de rect醤gulos
							pData->rdh.nCount=0;
							SetRect(&pData->rdh.rcBound,MAXLONG,MAXLONG,0,0);
						}			
					
					} // if (Column > Xo)
				} // for (int  Column ...)

				// Nueva Row. Lo del negativo se debe a que el bitmap est?invertido
				// verticalmente.
				Pixeles -= bmNewBitmap.bmWidthBytes;
			
			} // for (int Row...)			

			// Una vez finalizado el proceso,procedemos a la fusi髇 de la
			// regi髇 remanente desde la 鷏tima fusi髇 hasta el final			
			HRGN hNewRegion=ExtCreateRegion(NULL,sizeof(RGNDATAHEADER)+(sizeof(RECT)*maxRect),pData);

			if(hNewRegion) 
			{
				// If the main region does already exist, we add the new one,
				if(hRegion)
				{														
					CombineRgn(hRegion,hRegion,hNewRegion,RGN_OR);
					DeleteObject(hNewRegion);
				}
				else
					// if not, we consider the new one to be the main region at first!
					hRegion=hNewRegion;				
			}						
			
			// We free the allocated memory and the rest of used ressources
			GlobalFree(hData);
			SelectObject(hDC,hPrevBmpOrg);
			DeleteDC(hDC);
			
		}// if (hDC) 

		SelectObject(hMemDC,hPrevBmp);
		DeleteDC(hMemDC);
	} //if (hNewBitmap)	

	return hRegion;
}

/////////////////////////////////////////////////////////////////////////////
// CWBButton


CWBButton::CWBButton()
{
    m_TopHeight = 8;
    m_BottomHeight = 8;
    m_LeftWidth = 8;
    m_RightWidth = 17;
    m_State = notInited;
//    m_pFnt = new CAutoFont("宋体");   // MS Sans Serif");
    m_BkColor = RGB(255,0,255);
   	m_bOver = m_bTracking = FALSE;
    m_RcId       = 0;       // Resource ID
    m_NumofPics  = 0;
	m_mode=false;testflg=false;
	m_bRgn=false;
	m_lpButtonText.Empty();

	m_nBtnID = 0;

	m_nStarDrawPos = 0;
}

CWBButton::CWBButton(int nID)
{
    m_TopHeight = 8;
    m_BottomHeight = 8;
    m_LeftWidth = 8;
    m_RightWidth = 17;
    m_State = notInited;
    m_BkColor = RGB(255,0,255);
   	m_bOver = m_bTracking = FALSE;
    m_RcId       = 0;       // Resource ID
    m_NumofPics  = 0;
	m_mode=false;testflg=false;
	m_bRgn=false;

	m_nBtnID = nID;

	m_nStarDrawPos = 0;
}


CWBButton::~CWBButton()
{
    NormalBitmap.DeleteObject();
    SelectBitmap.DeleteObject();
    FocusBitmap.DeleteObject();
    DisableBitmap.DeleteObject();
	MoveBitmap.DeleteObject();
	MaskBitmap.DeleteObject();
//    SAFE_DELETE(m_pFnt);
}


BEGIN_MESSAGE_MAP(CWBButton, CButton)
	//{{AFX_MSG_MAP(CWBButton)
	ON_WM_KEYDOWN()
    ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//在对按钮初始化时，自动为按钮属性添加自画属性(OWNERDRAW)
void CWBButton::PreSubclassWindow() 
{
    SetButtonStyle(GetButtonStyle()|BS_OWNERDRAW );
	m_toolTip.Create( this , TTS_ALWAYSTIP) ;
    //m_toolTip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_toolTip.Activate( true ) ;
	m_toolTip.AddTool( this, m_strToolTip) ;
	m_toolTip.SetDelayTime(100);
	m_nBtnID = GetDlgCtrlID();
}

void CWBButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{

    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rc;
	bool IsDisable = false;	

    GetClientRect(rc);


	CMyMemDC2 dc(pDC, rc);
	UINT state = lpDrawItemStruct->itemState;


    if (!m_mode)
	{
		if (state & ODS_FOCUS)
		{
			if(!testflg)	DrawBitmap( &dc, focus );else DrawBitmap(&dc,flgnormal);
			if(state & ODS_SELECTED)
			{if(!testflg){DrawBitmap( &dc, select );rc.left += 5;}else DrawBitmap(&dc,flgselect);}
		}
		else if (state & ODS_DISABLED)
		{IsDisable = true;DrawBitmap( &dc, disable );}
		else{if(!testflg) DrawBitmap( &dc, normal );else DrawBitmap(&dc,flgnormal);}

		if(m_bOver && !(state & ODS_SELECTED))
		{if(!testflg) DrawBitmap(&dc,move); else DrawBitmap(&dc,flgmove);}
	}
	else
	{
		if(state & ODS_DISABLED) DrawBitmap(&dc,disable);//
		else if(m_bOver && !testflg) DrawBitmap(&dc,move);//
		else if(!m_bOver && testflg) DrawBitmap(&dc,select);
		else if(!m_bOver && !testflg) DrawBitmap(&dc,normal);
		else DrawBitmap(&dc,focus);
	}
}


bool CWBButton::LoadBitmaps
(
     UINT id, int count ,    
     int TopHeight, int BottomHeight, int LeftWidth, int RightWidth,
	 int height, int width
)
{
    m_TopHeight = TopHeight;
    m_BottomHeight = BottomHeight;
    m_LeftWidth = LeftWidth;
    m_RightWidth = RightWidth;
	m_height=height; m_width=width;

    m_RcId       = id;       // Resource ID
    m_NumofPics  = count;  


    CBitmap bmp;
    if( !bmp.LoadBitmap(id) ) return false;

	if( !InitBitmap( bmp, NormalBitmap, 0, count ) ) return false;
    if( !InitBitmap( bmp, SelectBitmap,	1, count ) ) return false;
    if( !InitBitmap( bmp, FocusBitmap,	2, count ) ) return false;
    if( !InitBitmap( bmp, DisableBitmap,3, count ) ) return false;
    if( !InitBitmap( bmp, MoveBitmap, 4, count ) ) return false;
	if( !InitBitmap( bmp, FlgNormalBitmap,  5, count ) ) return false;
	if( !InitBitmap( bmp, FlgSelectBitmap,  6, count ) ) return false;
	if( !InitBitmap( bmp, FlgMove,  7, count ) ) return false;

 //   if( !InitBitmap( bmp, NormalBitmap, 0, count ) ) return false;
 //   if( !InitBitmap( bmp, SelectBitmap, 1, count ) ) return false;
 //   if( !InitBitmap( bmp, DisableBitmap,2, count ) ) return false;
 //   if( !InitBitmap( bmp, FocusBitmap,  3, count ) ) return false;
 //   if( !InitBitmap( bmp, MoveBitmap,  4, count ) ) return false;
	//if( !InitBitmap( bmp, FlgNormalBitmap,  5, count ) ) return false;
	//if( !InitBitmap( bmp, FlgSelectBitmap,  6, count ) ) return false;
	//if( !InitBitmap( bmp, FlgMove,  7, count ) ) return false;

    bmp.DeleteObject();

    return true;
}

bool CWBButton::LoadBitmaps
(
     LPCTSTR pBitMapFile, int count ,    
     int TopHeight, int BottomHeight, int LeftWidth, int RightWidth,
	 int height, int width
)
{
	m_TopHeight = TopHeight;
    m_BottomHeight = BottomHeight;
    m_LeftWidth = LeftWidth;
    m_RightWidth = RightWidth;
	m_height=height; m_width=width;

    //m_RcId       = id;       // Resource ID
    m_NumofPics  = count;  

	CString strPath;

	strPath = _T("");

	CBitmap bmp;

	CImage imgTemp;
	imgTemp.Load(pBitMapFile);
	if ( bmp.m_hObject ) bmp.Detach();
	bmp.Attach(imgTemp.Detach());

/*	if( !InitBitmap( bmp, NormalBitmap, 0, count ) ) return false;
    if( !InitBitmap( bmp, FocusBitmap,	1, count ) ) return false;
    if( !InitBitmap( bmp, MoveBitmap,	2, count ) ) return false;
    if( !InitBitmap( bmp, DisableBitmap,3, count ) ) return false;
    if( !InitBitmap( bmp, SelectBitmap, 4, count ) ) return false;
	if( !InitBitmap( bmp, FlgNormalBitmap,  5, count ) ) return false;
	if( !InitBitmap( bmp, FlgSelectBitmap,  6, count ) ) return false;
	if( !InitBitmap( bmp, FlgMove,  7, count ) ) return false;
*/
	if( !InitBitmap( bmp, NormalBitmap, 0, count ) ) return false;
	if( !InitBitmap( bmp, SelectBitmap, 1, count ) ) return false;
	if( !InitBitmap( bmp, FocusBitmap,2, count ) ) return false;
	if( !InitBitmap( bmp, DisableBitmap,  3, count ) ) return false;
	if( !InitBitmap( bmp, MoveBitmap,  4, count ) ) return false;
	if( !InitBitmap( bmp, FlgNormalBitmap,  5, count ) ) return false;
	if( !InitBitmap( bmp, FlgSelectBitmap,  6, count ) ) return false;
	if( !InitBitmap( bmp, FlgMove,  7, count ) ) return false;

    bmp.DeleteObject();

    return true;
	
}

bool CWBButton::InitBitmap( CBitmap & src, CBitmap & dist, int index, int count )
{
    CDC * pDC = GetDC();



    CDC memDC; 
    memDC.CreateCompatibleDC(pDC);

    CDC srcDC;
    srcDC.CreateCompatibleDC(pDC);

    CBitmap* pOldBitmap1;
    pOldBitmap1 = srcDC.SelectObject(&src);

    BITMAP bmpinfo;
    src.GetBitmap(&bmpinfo);
    int bmpWidth = bmpinfo.bmWidth / count;
    int bmpHeight = bmpinfo.bmHeight;
    int orix = bmpWidth * index;

    CRect rc;
    GetClientRect(rc);

    CBitmap* pOldBitmap2;
    dist.DeleteObject();
    dist.CreateCompatibleBitmap(pDC,m_height,m_width);//rc.Width(),rc.Height());
    pOldBitmap2 = memDC.SelectObject(&dist);

    // lefttop,leftbottom,righttop,rightbottom
    if( !memDC.BitBlt(0,0,m_height, m_width, &srcDC,orix,0,SRCCOPY) ) return false;
    

    if( !memDC.BitBlt(0,m_width - m_BottomHeight,m_LeftWidth, m_BottomHeight, //rc.bottom
                &srcDC,orix,bmpHeight - m_BottomHeight,SRCCOPY) ) return false;


    if( !memDC.BitBlt(m_height - m_RightWidth, 0 ,m_RightWidth, m_TopHeight, //rc.right
                 &srcDC,orix + bmpWidth - m_RightWidth,0,SRCCOPY) ) return false;


    if( !memDC.BitBlt(rc.right - m_RightWidth,rc.bottom - m_BottomHeight,m_RightWidth,m_BottomHeight,
                 &srcDC,orix + bmpWidth - m_RightWidth,bmpHeight - m_BottomHeight,SRCCOPY) ) return false;

    // entireimage
    memDC.StretchBlt(m_LeftWidth,
                     m_TopHeight,
                     m_height  - ( m_LeftWidth + m_RightWidth ) ,//rc.Width()
                     m_width -  ( m_TopHeight + m_BottomHeight) ,//rc.Height()
                     &srcDC,
                     orix + m_LeftWidth,
                     m_TopHeight,
                     bmpWidth - ( m_LeftWidth + m_RightWidth ) ,
                     bmpHeight - ( m_TopHeight + m_BottomHeight ) ,SRCCOPY);

    
    // topbar,bottombar( Stretch )
    memDC.StretchBlt(m_LeftWidth,0, rc.Width() - (m_LeftWidth + m_RightWidth), m_TopHeight,
                     &srcDC,orix + m_LeftWidth, 0, bmpWidth - ( m_LeftWidth + m_RightWidth) , m_TopHeight,SRCCOPY);

    memDC.StretchBlt(m_LeftWidth, rc.bottom - m_BottomHeight, rc.Width() - ( m_LeftWidth + m_RightWidth), m_BottomHeight,
                     &srcDC,orix + m_LeftWidth,bmpHeight - m_BottomHeight,bmpWidth - ( m_LeftWidth + m_RightWidth) , m_BottomHeight,SRCCOPY );


    // sidebar ( STretch? )
    memDC.StretchBlt(0,m_TopHeight,m_LeftWidth,rc.bottom - m_TopHeight - m_BottomHeight ,
                     &srcDC, orix,m_TopHeight, m_LeftWidth, bmpHeight - ( m_TopHeight + m_BottomHeight ) ,SRCCOPY);

    memDC.StretchBlt(rc.right - m_RightWidth ,m_TopHeight,m_RightWidth,rc.bottom - m_TopHeight - m_BottomHeight ,
                     &srcDC, orix +  bmpWidth - m_RightWidth,m_TopHeight, m_RightWidth, bmpHeight - m_TopHeight - m_BottomHeight ,SRCCOPY);

    srcDC.SelectObject(pOldBitmap1);
    memDC.SelectObject(pOldBitmap2);

    ReleaseDC(pDC);
  //ReleaseDC(&srcDC);
  //ReleaseDC(&memDC);
	memDC.DeleteDC();
	srcDC.DeleteDC();

     m_State = FileLoaded;
    
    return true;

}

void CWBButton::DrawBitmap( CDC * pDC, int mode )
{
    if( m_State < FileLoaded ) return;

    CRect rc;
    GetClientRect(rc);

	COLORREF crOldBack = pDC->SetBkColor(RGB(0,255,0));
	COLORREF crOldText = pDC->SetTextColor(RGB(0,0,0));
	CDC dcImage, dcTrans;

	if(m_bRgn)
	{
//		m_rgn=BitmapRegion((HBITMAP)NormalBitmap.m_hObject,RGB(0,255,0));
//		CRgn rgn;rgn.Attach(m_rgn);
//		pDC->SelectClipRgn(&rgn);
//		rgn.DeleteObject();
	}
	// Create two memory dcs for the image and the mask
	dcImage.CreateCompatibleDC(pDC);
	dcTrans.CreateCompatibleDC(pDC);
	// Select the image into the appropriate dc
    CBitmap* pOldBitmapImage;
    switch(mode)
    {
    case normal:
	   pOldBitmapImage  = dcImage.SelectObject(&NormalBitmap);
       break;
    case select:
	   pOldBitmapImage  = dcImage.SelectObject(&SelectBitmap);
       break;
    case focus:
	   pOldBitmapImage  = dcImage.SelectObject(&FocusBitmap);
       break;
    case disable:
	   pOldBitmapImage  = dcImage.SelectObject(&DisableBitmap);
       break;
	case move:
	   pOldBitmapImage  = dcImage.SelectObject(&MoveBitmap);
       break;
	case flgnormal:
		pOldBitmapImage = dcImage.SelectObject(&FlgNormalBitmap);
		break;
	case flgselect:
		pOldBitmapImage = dcImage.SelectObject(&FlgSelectBitmap);
		break;
	case flgmove:
		pOldBitmapImage = dcImage.SelectObject(&FlgMove);
		break;
    default:
        return;
    }
      
	// Create the mask bitmap
	CBitmap bitmapTrans;
	int nWidth = rc.Width();
	int nHeight = rc.Height();
	bitmapTrans.CreateBitmap(nWidth, nHeight, 1, 1, NULL);

    // Select the mask bitmap into the appropriate dc
	CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

	// Build mask based on transparent colour
//	dcImage.SetBkColor(m_BkColor);
	dcImage.SetBkColor(RGB(0,255,0));
	dcTrans.BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	pDC->BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);
	pDC->BitBlt(0, 0, nWidth, nHeight, &dcTrans, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);

	// Restore settings
	dcImage.SelectObject(pOldBitmapImage);
	dcTrans.SelectObject(pOldBitmapTrans);
	pDC->SetBkColor(crOldBack);
	pDC->SetTextColor(crOldText);

	/*
	*	按钮上面添加字
	*/
	DrawBtnText(pDC, mode);
	
	dcImage.DeleteDC();
	dcTrans.DeleteDC();
	bitmapTrans.DeleteObject();
}

void CWBButton::DrawBtnText(CDC * pDC, int mode)
{
	if (m_lpButtonText.IsEmpty())
		return;

	CRect   rectClient;
	GetClientRect(rectClient);
	int nWidth = rectClient.Width();

	// button文本字体
	CFont font;

	//font.CreateFont(
	//	6,                        // nHeight
	//	0,                        // nWidth
	//	0,                        // nEscapement
	//	0,                        // nOrientation
	//	FW_NORMAL,                // nWeight
	//	FALSE,                    // bItalic
	//	FALSE,                    // bUnderline
	//	0,                        // cStrikeOut
	//	ANSI_CHARSET,              // nCharSet
	//	OUT_DEFAULT_PRECIS,        // nOutPrecision
	//	CLIP_DEFAULT_PRECIS,      // nClipPrecision
	//	DEFAULT_QUALITY,          // nQuality
	//	DEFAULT_PITCH | FF_SWISS,
	//	_T("Arial")                    // nPitchAndFamily   Arial
	//	);

	CFont* pOldFont = pDC->SelectObject(GetFont());

	CSize size = pDC->GetTextExtent(m_lpButtonText);

	rectClient.left   = (rectClient.Width() - size.cx)/2;
	rectClient.top    = (rectClient.Height() - size.cy)/2;
	rectClient.right  = rectClient.left + size.cx;
	rectClient.bottom = rectClient.top + size.cy;

	switch(mode)
	{
	case select:
		rectClient.top    = rectClient.top + 1;
		rectClient.left   = rectClient.left + 1;
		rectClient.right  = rectClient.left + size.cx;
		rectClient.bottom = rectClient.top + size.cy;
		break ;
	default:
		break ;
	}

	if (0 < m_nStarDrawPos && m_nStarDrawPos < nWidth)
	{
		rectClient.left  = m_nStarDrawPos;
		rectClient.right = nWidth;
	}

	// 文本透明
	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText(m_lpButtonText, rectClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(pOldFont);
}

void CWBButton::SetWindowText(LPCTSTR lpszString)
{
	m_lpButtonText = lpszString;
	return ;
}

void CWBButton::SetTextStarDrawPos(int nPos)	// 设置文字开始画的位置 [2/15/2012 Jesse]
{
	m_nStarDrawPos = nPos;
}

BOOL CWBButton::OnEraseBkgnd(CDC* pDC) 
{
    return TRUE;
}

void CWBButton::SetButtonDef( int TopHeight, int BottomHeight, int LeftWidth, int RightWidth )
{ 
    m_TopHeight = TopHeight;
    m_BottomHeight = BottomHeight;
    m_LeftWidth = LeftWidth;
    m_RightWidth = RightWidth;

    if( m_RcId != 0 && m_NumofPics != 0 )
    {
       LoadBitmaps(m_RcId,m_NumofPics,TopHeight,BottomHeight,LeftWidth,RightWidth,45,26);
    }

}

//DEL void CWBButton::SetTextFont( CFont & fnt )
//DEL { 
//DEL 
//DEL  //   LOGFONT lf;
//DEL  //   fnt.GetLogFont(&lf);
//DEL //    SAFE_DELETE(m_pFnt);
//DEL   //  m_pFnt = new CAutoFont(lf);
//DEL }

//DEL void CWBButton::SetFontColor( COLORREF color )
//DEL {
//DEL //    m_pFnt->SetFontColor(color);
//DEL  //   UpdateWindow();
//DEL }


void CWBButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetSafeHwnd()!=GetFocus()->GetSafeHwnd() && m_bRgn)
		SetFocus();
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

    if (IsWindow(m_toolTip.m_hWnd))
    {
        m_toolTip.UpdateTipText( m_strToolTip , this);
    }


    CWnd *pWnd = GetActiveWindow();
    CWnd *pParent = GetOwner();

    if ((GetCapture() != this) && 
        pWnd != NULL && 
        pParent != NULL)
    {
        SetCapture();
        Invalidate();
    }
    else
    {
        CRect rc;
        GetClientRect(&rc);
        if (!rc.PtInRect(point))
        {
            // Redraw only if mouse goes out
            //if (m_MouseOnButton == TRUE)
            {
                //m_MouseOnButton = FALSE;
                Invalidate();
            }
            // If user is NOT pressing left button then release capture!
            if (!(nFlags & MK_LBUTTON)) ReleaseCapture();
        }
    }

	CButton::OnMouseMove(nFlags, point);
}
void CWBButton::OnOK()
{
//	DrawBitmap(
}
void CWBButton::OnCancel()
{

}
LRESULT CWBButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver = FALSE;
	m_bTracking = FALSE;
	InvalidateRect(NULL, FALSE);
	return TRUE;
}

LRESULT CWBButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bOver = TRUE;

	InvalidateRect(NULL);
	return TRUE;
}

void CWBButton::SetMod(bool mode)
{
	m_mode=mode;
}



bool CWBButton::GetCheck()
{
	return testflg;
}

void CWBButton::SetCheck()
{
	if(m_mode)
	{
		CDC * dc=GetDC();
		if(!testflg) {testflg=true;  DrawBitmap(dc,focus);}
		else		 {testflg=false; DrawBitmap(dc,move);}
		ReleaseDC(dc);
	}
//	else
//	{
//		if(!testflg) {testflg=true;  DrawBitmap(GetDC(),flgmove)
//		else
//	}
}
void CWBButton::SetCheck(bool flg)
{
	testflg=flg;
	Invalidate();
//	CDC *dc=GetDC();
//	DrawBitmap(dc,normal);
//	ReleaseDC(dc);
}

void CWBButton::ChangMod()
{
	if(!m_mode) {testflg=!testflg; Invalidate();}
}

void CWBButton::ChangMod(bool bFlag)
{
	if(!m_mode) {testflg=bFlag; Invalidate();}
}

bool CWBButton::GetMod()
{
	return testflg;
}

void CWBButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	return;
	if (nChar==VK_RETURN || nChar==VK_SPACE)
		return;
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CWBButton::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	if(m_bRgn)
	{
		SetWindowRgn(NULL, FALSE);
		m_rgn=BitmapRegion((HBITMAP)MaskBitmap.m_hObject,RGB(0,255,0));
		SetWindowRgn(m_rgn,TRUE);
	}
}

void CWBButton::SetButtonRgn(UINT ID)
{
	m_bRgn=true;
	m_ID=ID;
	MaskBitmap.LoadBitmap(ID);
	m_rgn=BitmapRegion((HBITMAP)MaskBitmap.m_hObject,RGB(0,255,0));
	PostMessage(WM_SIZE);
}

void CWBButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	GetParent()->SendMessage(MSG_TO_PARENT_CLICKED_DOWN, m_nBtnID);

	CButton::OnLButtonDown(nFlags, point);
}

void CWBButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rc;
	GetClientRect(&rc);

	if ( rc.PtInRect(point) )
		GetParent()->SendMessage(MSG_TO_PARENT_CLICKED_UP, m_nBtnID);

	CButton::OnLButtonUp(nFlags, point);
}

void CWBButton::SetButtonID(int nID)
{
	m_nBtnID = nID;
}

BOOL CWBButton::PreTranslateMessage(MSG* pMsg)
{
	m_toolTip.RelayEvent(pMsg );

	return CButton::PreTranslateMessage(pMsg);
}

void CWBButton::SetToolTip(CString strToolTip)
{
	m_strToolTip = strToolTip;
	if( m_hWnd )
	{
		if( m_toolTip.m_hWnd )
			m_toolTip.UpdateTipText( strToolTip, this );

	}
}