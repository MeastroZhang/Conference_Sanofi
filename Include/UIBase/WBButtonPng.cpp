
#include "stdafx.h"
#include "WBButtonPng.h"
#include ".\wbbuttonPng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWBButtonPng::CWBButtonPng()
: CWBButton()
{
	m_pbmpOldBk = NULL;
	m_bmpBk.DeleteObject();
	m_dcBk.DeleteDC();
}

CWBButtonPng::CWBButtonPng(int nID)
: CWBButton(nID)
{
	m_pbmpOldBk = NULL;
	m_bmpBk.DeleteObject();
	m_dcBk.DeleteDC();
}


CWBButtonPng::~CWBButtonPng()
{
	if (m_dcBk.m_hDC && m_pbmpOldBk)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	}

	m_pbmpOldBk = NULL;
	m_bmpBk.DeleteObject();
	m_dcBk.DeleteDC();
	m_ImageBK.Destroy();
	m_ImageUnderBK.Destroy();
}

void CWBButtonPng::DrawBitmap( CDC * pDC, int mode )
{
    if( m_State < FileLoaded ) return;

	if (true == m_ImageUnderBK.IsNull())
	{
		// 花指定区域的按钮背景
		PaintBk(pDC);
	}
	else
	{
		m_ImageUnderBK.Draw(pDC->GetSafeHdc(), 0, 0, m_width, m_ImageUnderBK.GetHeight(), 0, 0, 1, m_ImageUnderBK.GetHeight());
	}
	
	int cx   = m_ImageBK.GetWidth()/m_NumofPics;
	int nPos = cx*mode;
 
	BLENDFUNCTION blend;
	blend.BlendOp				= AC_SRC_OVER;
	blend.BlendFlags			= 0;
	blend.SourceConstantAlpha	= 255;
	blend.AlphaFormat			= AC_SRC_ALPHA;

	HDC hDC = m_ImageBK.GetDC();

	::AlphaBlend( pDC->m_hDC, 0, 0, m_width, m_height, hDC, nPos, 0, m_width, m_height, blend );

	m_ImageBK.ReleaseDC();

	//	按钮上面添加字
	DrawBtnText(pDC, mode);
}

bool CWBButtonPng::LoadBitmaps( LPCTSTR lpBitMapFile, 
								int count ,    
								int TopHeight, 
								int BottomHeight, 
								int LeftWidth, 
								int RightWidth,
								int height, 
								int width,
								LPCTSTR lpImageUnderBK /*= NULL*/)
{
	HRESULT rt = S_FALSE;

	m_ImageBK.Destroy();

	rt = m_ImageBK.Load(lpBitMapFile);

	if (S_OK == rt)
	{
		for(int i = 0; i < m_ImageBK.GetWidth(); ++i)
		{
			for(int j = 0; j < m_ImageBK.GetHeight(); ++j)
			{
				unsigned char* pucColor = (unsigned char*)m_ImageBK.GetPixelAddress(i , j);
				pucColor[0] = ((pucColor[0] * pucColor[3]) + 127) / 255;
				pucColor[1] = ((pucColor[1] * pucColor[3]) + 127) / 255;
				pucColor[2] = ((pucColor[2] * pucColor[3]) + 127) / 255;
			}
		}

		if (NULL != lpImageUnderBK)
		{
			rt = m_ImageUnderBK.Load(lpImageUnderBK);
			if (S_OK == rt)
			{
				for(int i = 0; i < m_ImageUnderBK.GetWidth(); ++i)
				{
					for(int j = 0; j < m_ImageUnderBK.GetHeight(); ++j)
					{
						unsigned char* pucColor = (unsigned char*)m_ImageUnderBK.GetPixelAddress(i , j);
						pucColor[0] = ((pucColor[0] * pucColor[3]) + 127) / 255;
						pucColor[1] = ((pucColor[1] * pucColor[3]) + 127) / 255;
						pucColor[2] = ((pucColor[2] * pucColor[3]) + 127) / 255;
					}
				}
			}
		}

		return CWBButton::LoadBitmaps(lpBitMapFile, count, TopHeight, BottomHeight, LeftWidth, RightWidth, height, width);
	}

	return false;
}

void CWBButtonPng::PaintBk(CDC* pDC)
{
	CClientDC clDC(GetParent());
	CRect rect;
	CRect rect1;

	GetClientRect(rect);

	GetWindowRect(rect1);

	GetParent()->ScreenToClient(rect1);

	if (m_dcBk.m_hDC == NULL&&IsTopParentActive())//modified by felix
	{
		m_dcBk.CreateCompatibleDC(&clDC);
		m_bmpBk.CreateCompatibleBitmap(&clDC, rect.Width(), rect.Height());
		m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
		m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), &clDC, rect1.left, rect1.top, SRCCOPY);
	} 

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
}