// Label.cpp : implementation file   
//   

#include "stdafx.h"   
#include "Lable.h"   

#ifdef _DEBUG   
#define new DEBUG_NEW   
#undef THIS_FILE   
static char THIS_FILE[] = __FILE__;   
#endif   

/////////////////////////////////////////////////////////////////////////////   
// CLabel   

CLabel::CLabel(void)   
{   
	//初始化文本颜色和背景颜色   
	m_crText = GetSysColor(COLOR_WINDOWTEXT);   
	m_crBkground = GetSysColor(COLOR_3DFACE);  
	m_crOldText = m_crText;

	//初始化字体结构   
	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);   

	//初始化链接   
	m_bLink = FALSE;   

	//初始化闪烁类型、是否闪烁   
	m_Type = CLabel::None;   
	m_bFlash=FALSE;   

	//初始化光标句柄   
	m_hCursor=NULL;   

	//初始化启动山所标志   
	m_bStartTimerFlag=FALSE;   
}   

//拷贝构造函数   
CLabel::CLabel(const CLabel & p)   
{   
	//设置字体   
	m_lf=p.m_lf;   

	//设置是否链接   
	m_bLink=p.m_bLink;   

	//设置闪烁类型   
	m_Type=p.m_Type;   

	//设置背景色和文本色   
	m_crBkground=p.m_crBkground;   
	m_crText=p.m_crText; 
	m_crOldText = p.m_crOldText;

	//设置光标   
	if(p.m_hCursor != NULL)   
	{   
		m_hCursor=CopyCursor(p.m_hCursor);   
	}   

	//定时期启动标志和闪烁标志   
	m_bStartTimerFlag=FALSE;   
	m_bFlash=FALSE;    
}   

//赋值构造函数   
CLabel& CLabel::operator=(const CLabel& p)   
{   
	if(this != &p)   
	{   
		//设置字体   
		m_lf=p.m_lf;   

		//设置是否链接   
		m_bLink=p.m_bLink;   

		//设置闪烁类型   
		m_Type=p.m_Type;   

		//设置背景色和文本色   
		m_crBkground=p.m_crBkground;   
		m_crText=p.m_crText;
		m_crOldText = p.m_crOldText;

		//设置光标   
		if(m_hCursor != NULL)   
		{   
			DestroyCursor(m_hCursor);   
			m_hCursor=NULL;   
		}   
		if(p.m_hCursor != NULL)   
		{   
			m_hCursor=CopyCursor(p.m_hCursor);   
		}   

		//定时期启动标志和闪烁标志   
		m_bStartTimerFlag=FALSE;   
		m_bFlash=FALSE;    
	}   

	return *this;   
}   

//析构函数   
CLabel::~CLabel(void)   
{   
	//销毁光标   
	if(m_hCursor != NULL)   
	{   
		DestroyCursor(m_hCursor);   
		m_hCursor=NULL;   
	}   

	//销毁字体   
	m_textfont.DeleteObject();   
}   

//设置文本   
void CLabel::SetText(IN const CString& strText)   
{   
	//设置显示文本   
	SetWindowText(strText);   

	//重绘窗口(经过测试，Invalidate所耗时间远小于RedrawWindow   
	Invalidate();   
}   

//设置文本颜色   
void CLabel::SetTextColor(IN COLORREF crText)   
{   
	//设置文本颜色   
	m_crText = crText; 
	m_crOldText = m_crText;

	//重绘窗口(经过测试，Invalidate所耗时间远小于RedrawWindow   
	Invalidate();   
}   

//设置是否加粗显示   
BOOL CLabel::SetFontBold(IN BOOL bBold)   
{      
	BOOL returnvalue=FALSE;   

	//设置是否加粗   
	if(bBold == TRUE)   
	{   
		m_lf.lfWeight=FW_BOLD;   
	}   
	else   
	{   
		m_lf.lfWeight=FW_NORMAL;   
	}   

	//更新字体   
	if(CreateFont()==TRUE)   
	{   
		//重绘窗口(经过测试，Invalidate所耗时间远小于RedrawWindow   
		Invalidate();   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//设置是否下划线显示   
BOOL CLabel::SetFontUnderline(IN BOOL bSet)   
{      
	BOOL returnvalue=FALSE;   

	//设置是否下划线   
	m_lf.lfUnderline = bSet;   

	//更新字体   
	if(CreateFont()==TRUE)   
	{   
		//重绘窗口(经过测试，Invalidate所耗时间远小于RedrawWindow   
		Invalidate();   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//设置是否斜体显示   
BOOL CLabel::SetFontItalic(IN BOOL bSet)   
{   
	BOOL returnvalue=FALSE;   

	//设置是否斜体   
	m_lf.lfItalic = bSet;   

	//更新字体   
	if(CreateFont()==TRUE)   
	{   
		//重绘窗口(经过测试，Invalidate所耗时间远小于RedrawWindow   
		Invalidate();   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//设置字号   
BOOL CLabel::SetFontSize(IN long nSize)   
{   
	BOOL returnvalue=FALSE;   

	//设置新字号   
	if(nSize >=0 )   
	{   
		m_lf.lfHeight = nSize;   
	}   
	else   
	{   
		m_lf.lfHeight = -nSize;   
	}   

	//更新字体   
	if(CreateFont()==TRUE)   
	{   
		//重绘窗口(经过测试，Invalidate所耗时间远小于RedrawWindow   
		Invalidate();   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//获取是否加粗   
BOOL CLabel::IsFontBold(void)   
{   
	BOOL returnvalue=FALSE;   

	//判断字体权重是否大于等于700   
	if(m_lf.lfWeight >= 700)   
	{   
		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//获取是否斜体   
BOOL CLabel::IsFontItalic(void)   
{   
	BOOL returnvalue=FALSE;   

	//判断字体斜显属性是否为真   
	if(m_lf.lfItalic != 0)   
	{   
		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//获取是否下划线   
BOOL CLabel::IsFontUnderline(void)   
{   
	BOOL returnvalue=FALSE;   

	//判断字体下划线属性是否为真   
	if(m_lf.lfUnderline != 0)   
	{   
		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//获取字号   
long     CLabel::GetFontSize(void)   
{   
	long returnvalue=0;   

	//获取字体的绝对值   
	returnvalue=abs(m_lf.lfWidth);   

	return returnvalue;   
}   

//获取字体名   
CString CLabel::GetFontName(void)   
{   
	CString tempstring("");   

	tempstring=m_lf.lfFaceName;   

	return tempstring;   
}   

// 获取字体
LOGFONT CLabel::GetLogFont(void) 
{
	return m_lf;
}

//设置背景色   
void CLabel::SetBkColor(IN COLORREF crBkgnd)   
{   
	//设置背景色   
	m_crBkground=crBkgnd;   

	//重绘窗口(经过测试，Invalidate所耗时间远小于RedrawWindow   
	Invalidate();   
}   

//获取背景色   
COLORREF CLabel::GetBkColor(void)   
{   
	return m_crBkground;   
}   

//获取前景色   
COLORREF CLabel::GetTextColor(void)   
{   
	return m_crText;   
}   

//设置字体名   
BOOL CLabel::SetFontName(IN const CString& strFont)   
{      
	BOOL returnvalue=FALSE;   

	//设置新字体   
	_tcscpy(m_lf.lfFaceName,strFont);   

	//更新字体   
	if(CreateFont()==TRUE)   
	{   
		//重绘窗口(经过测试，Invalidate所耗时间远小于RedrawWindow   
		Invalidate();   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   


BEGIN_MESSAGE_MAP(CLabel, CStatic)   
	//{{AFX_MSG_MAP(CLabel)   
	ON_WM_TIMER()   
//	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()   
	ON_WM_ERASEBKGND()   
	ON_WM_PAINT()  
	ON_WM_DESTROY()   
	//}}AFX_MSG_MAP   
END_MESSAGE_MAP()   

/////////////////////////////////////////////////////////////////////////////   
// CLabel message handlers   

//WM_TIMER响应函数   
void CLabel::OnTimer(IN UINT nIDEvent)    
{   
	//修改闪烁标志   
	m_bFlash = !m_bFlash;   

	//重绘窗口(经过测试，Invalidate所耗时间远小于RedrawWindow   
	Invalidate();   

	//  CStatic::OnTimer(nIDEvent);   
}   

//设置是否以链接文本显示   
void CLabel::SetLink(BOOL bLink, COLORREF crLinkText /*= RGB(0,0,255)*/)   
{   
	HINSTANCE hInstance=AfxGetInstanceHandle();   

	//修改链接标志   
	m_bLink=bLink;   

	if(bLink == TRUE)   
	{   
		//保存原文本颜色
		m_crOldText = m_crText;

		//设置文本颜色   
		m_crText=crLinkText;   

		//设置下划线   
		m_lf.lfUnderline=TRUE;   

		//设置新光标   
		m_hCursor=::LoadCursor(NULL, IDC_HAND);
		
		//修改NOTIFY风格，该风格可激活WM_SETCURSOR   
		ModifyStyle(0,SS_NOTIFY);   
	}   
	else   
	{   
		//设置文本颜色为原文本色   
		m_crText=m_crOldText;   

		//设置下划线   
		m_lf.lfUnderline=FALSE;   

		//销毁光标   
		if(m_hCursor != NULL)   
		{   
			DestroyCursor(m_hCursor);   
			m_hCursor=NULL;   
		}   

		//修改NOTIFY风格，该风格可激活WM_SETCURSOR   
		ModifyStyle(SS_NOTIFY,0);   
	}   

	if(CreateFont()==TRUE)   
	{   
		//重绘窗口(经过测试，Invalidate所耗时间远小于RedrawWindow   
		Invalidate();   
	} 
}   

//是否链接   
BOOL    CLabel::IsLink(void)   
{   
	return m_bLink;   
}   

//WM_LBUTTONDOWN消息相应函数   
//void CLabel::OnLButtonDown(IN UINT nFlags, IN CPoint point)    
//{   
//	const int   nTextLength=1024;   
//	TCHAR text[nTextLength];   
//	int length=0;   
//
//	//判断是否有链接标志   
//	if(m_bLink == TRUE)   
//	{   
//		//获取窗口文本   
//		length=GetWindowText(text,nTextLength-1);   
//		text[min(length,nTextLength-1)]='\0';   
//		
//		//执行链接   
//		ShellExecute(NULL,_T("open"),L"http://192.168.0.160:8080/ecim/search/viewuser.jsp?sessionid=300&flag=true",NULL,NULL,SW_SHOWMAXIMIZED);   
//	}   
//}   

//WM_SETCURSOR消息相应函数   
BOOL CLabel::OnSetCursor(IN CWnd* pWnd,IN UINT nHitTest,IN  UINT message)    
{   
	BOOL returnvalue=FALSE;   

	if(m_hCursor != NULL)   
	{   
		//设置新光标   
		::SetCursor(m_hCursor);   

		returnvalue=TRUE;   
	}   
	else   
	{   
		if(CStatic::OnSetCursor(pWnd, nHitTest, message) != 0)   
		{   
			returnvalue=TRUE;   
		}   
	}   

	return returnvalue;   
}   

//WM_ERASEBKGND消息相应函数   
BOOL CLabel::OnEraseBkgnd(IN CDC* pDC)    
{   
	// TODO: Add your message handler code here and/or call default   
	return TRUE;   
}   

//WM_PAINT消息相应函数   
void CLabel::OnPaint(void)    
{   
	CPaintDC dc(this); // device context for painting   

	// TODO: Add your message handler code here   

	// Do not call CStatic::OnPaint() for painting messages   
	CRect clientrect(0,0,0,0);//客户区RECT    
	
	//获取客户区尺寸   
	GetClientRect(&clientrect);   

	//绘制背景   
	DrawBackground(dc,clientrect);   
	//绘制文本   
	DrawText(dc,clientrect);   
}   

//设置闪烁类型   
BOOL CLabel::SetFlashType(IN int iFlashType)   
{   
	BOOL returnvalue=FALSE;   

	//验证iFlashType的有效性   
	if(iFlashType == CLabel::None    
		|| iFlashType == CLabel::Text   
		|| iFlashType == CLabel::Background)   
	{   
		m_Type = iFlashType;   
		returnvalue=TRUE;   
	}   
	else   
	{   
		m_Type = ErrorType;   
	}   

	return returnvalue;   
}   

//获取闪烁类型   
int CLabel::GetFlashType(void)   
{   
	return m_Type;   
}   

//启动闪烁   
BOOL CLabel::StartFlash(IN UINT iIntervalTime)   
{   
	BOOL returnvalue=FALSE;   
	UINT_PTR bSetTimer;   

	//首先停止定时器   
	StopFlash();   

	//启动定时器   
	bSetTimer=SetTimer(IDT_TIMER1,iIntervalTime,NULL);   
	if(bSetTimer != 0)   
	{   
		m_bStartTimerFlag=TRUE;   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//关闭闪烁   
void CLabel::StopFlash(void)   
{   
	if(m_bStartTimerFlag == TRUE)   
	{   
		KillTimer(IDT_TIMER1);   

		m_bStartTimerFlag=FALSE;   
	}   
}   

//设置光标   
BOOL CLabel::SetCursor(IN UINT nIDCursor)   
{   
	HCURSOR hCursor=NULL;   
	BOOL    returnvalue=FALSE;   
	HINSTANCE hInstance=AfxGetInstanceHandle();   


	//装载光标资源   
	hCursor=::LoadCursor(NULL, MAKEINTRESOURCE(nIDCursor));   

	if(hCursor != NULL)   
	{   
		//如过当前光标有效，则先销毁   
		if(m_hCursor != NULL)   
		{   
			DestroyCursor(m_hCursor);   
		}   
		//设置新光标   
		m_hCursor=hCursor;   

		ModifyStyle(0,SS_NOTIFY);   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//WM_DESTROY响应函数   
void CLabel::OnDestroy(void)    
{   
	//销毁定时器   
	if(m_bStartTimerFlag == TRUE)   
	{   
		KillTimer(IDT_TIMER1);   

		m_bStartTimerFlag=FALSE;   
	}   

	CStatic::OnDestroy();   

	// TODO: Add your message handler code here   
}   

//设置光标（支持动态光标）   
BOOL CLabel::SetCursor(IN LPCTSTR lpCursorFileName)   
{   
	HCURSOR hCursor=NULL;   
	BOOL    returnvalue=FALSE;   

	//装载光标资源   
	hCursor=::LoadCursorFromFile(lpCursorFileName);   

	if(hCursor != NULL)   
	{   
		//如过当前光标有效，则先销毁以前光标   
		if(m_hCursor != NULL)   
		{   
			DestroyCursor(m_hCursor);   
		}   
		//设置新光标   
		m_hCursor=hCursor;   

		ModifyStyle(0,SS_NOTIFY);   

		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//创建字体   
BOOL CLabel::CreateFont(void)   
{   
	BOOL bCreateFont=FALSE;   
	BOOL returnvalue=FALSE;   

	//首先释放   
	m_textfont.DeleteObject();   

	//创建字体   
	bCreateFont=m_textfont.CreateFontIndirect(&m_lf);   

	if(bCreateFont == TRUE)   
	{   
		returnvalue=TRUE;   
	}   

	return returnvalue;   
}   

//绘制文本   
void CLabel::DrawText(CDC &dc, CRect &clientrect)   
{   
	CString strText("");   
	UINT nTextFlags=DT_EDITCONTROL | DT_WORDBREAK | DT_END_ELLIPSIS |  DT_NOPREFIX;   
	UINT maskchar=0x03;   
	DWORD windowstyle=GetStyle();   

	
	//获取文本绘制标志   
	maskchar &= windowstyle;   
	if(maskchar == SS_CENTER)   
	{   
		nTextFlags |= DT_CENTER;   
	}   
	else if(maskchar == SS_LEFT) 
	{   
		nTextFlags |= DT_LEFT;   
	}   
	else
	{   
		nTextFlags |= DT_RIGHT;   
	} 

	if ( windowstyle & SS_LEFTNOWORDWRAP )
	{
		nTextFlags |= DT_VCENTER | DT_SINGLELINE | DT_LEFT; 
	}

	//设置为透明   
	dc.SetBkMode(TRANSPARENT);   

	//绘制文本   
	if(m_Type == CLabel::Text && !m_bFlash)   
	{   
	}   
	else   
	{   
		//设置文本颜色   
		if(windowstyle & WS_DISABLED)   
		{   
			dc.SetTextColor(GetSysColor(COLOR_INACTIVECAPTIONTEXT));   
		}   
		else   
		{   
			dc.SetTextColor(m_crText);   
		}   

		//选入字体   
		dc.SelectObject(m_textfont);   

		//获取显示文本   
		GetWindowText(strText);   
		dc.DrawText(strText,&clientrect,nTextFlags);   
	}   
}   

//绘制背景   
void CLabel::DrawBackground(CDC &dc, CRect &clientrect)   
{     
	//CBrush bkbrush(RGB(0,0,0));//画刷   

	////绘制背景   
	//bkbrush.DeleteObject();   
	//if(m_Type == CLabel::Background && !m_bFlash)   
	//{   
	//	bkbrush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));   
	//}   
	//else   
	//{   
	//	bkbrush.CreateSolidBrush(m_crBkground);   
	//}   
	//dc.FillRect(&clientrect,&bkbrush);   
	//bkbrush.DeleteObject();   

	CBrush *pbkbrush = NULL;
	
	if ( m_Type == CLabel::Background && !m_bFlash )
	{
		pbkbrush = new CBrush;
		pbkbrush->CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	}
	else if ( m_Type == CLabel::None )
	{
		pbkbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	}
	else
	{
		pbkbrush = new CBrush;
		pbkbrush->CreateSolidBrush(m_crBkground);
	}
	dc.FillRect(&clientrect,pbkbrush);
	pbkbrush->DeleteObject();
	pbkbrush = NULL;
}
