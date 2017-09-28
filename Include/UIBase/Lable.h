/************************************************************  
类名： CLabel 
功能描述：CStatic扩展:支持字体、字号、加粗、斜体、下划线设置; 
		  支持静态光标和动态光标；支持超链接形式显示；支持文本 
		  闪烁和背景闪烁，支持闪烁间隔设置； 
作者：刘斌 
版本：1.0.0 
完成日期：2005/06/12 
修改记录：										 
修改人 felix     修改时间      新版本号      修改内容			 
************************************************************/ 
#if !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_) 
#define AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_ 
 
#if _MSC_VER >= 1000 
#pragma once 
#endif // _MSC_VER >= 1000 
// Label.h : header file 
 
class CLabel : public CStatic 
{ 
public: 
	enum FlashType {None, Text, Background, ErrorType };//闪烁类型 
	enum TIMER{IDT_TIMER1=0x1001};//定时器 
 
	CLabel(void); 
	CLabel(const CLabel& p);//拷贝构造函数 
	CLabel& operator=(const CLabel& p);//赋值构造函数 
 
	virtual ~CLabel(void); 
 
	 
	//设置光标,第一个为某个资源的ID号，第二个为光标文件名(第二个支持动画光标) 
	BOOL SetCursor(IN UINT nIDCursor); 
	BOOL SetCursor(IN LPCTSTR lpCursorFileName); 
 
	//以指定时间间隔启动闪烁；停止闪烁、设置闪烁类型(闪烁文本还是背景) 
	BOOL StartFlash(IN UINT iIntervalTime); 
	void StopFlash(void); 
	virtual BOOL SetFlashType(IN int iFlashType); 
 
	//获取闪烁类型 
	int	GetFlashType(void); 
 
	//设置背景色、前景色 
	void SetBkColor(IN COLORREF crBkgnd); 
	void SetTextColor(IN COLORREF crText); 
 
	//获取背景色、前景色 
	COLORREF GetBkColor(void); 
	COLORREF GetTextColor(void); 
 
	//设置显示文本 
	void SetText(IN const CString& strText); 
 
	//设置是否加粗、下划线、斜体、字体名称、字号 
	BOOL SetFontBold(IN BOOL bBold); 
	BOOL SetFontName(IN const CString& strFont); 
	BOOL SetFontUnderline(IN BOOL bSet); 
	BOOL SetFontItalic(IN BOOL bSet); 
	BOOL SetFontSize(IN long nSize); 
 
	//获取是否加粗、下划线、斜体、字号、字体名 
	BOOL IsFontBold(void); 
	BOOL IsFontItalic(void); 
	BOOL IsFontUnderline(void); 
	long GetFontSize(void); 
	CString GetFontName(void); 
	LOGFONT GetLogFont(void); /*Add:Felix 12/27/2011*/
 
	//是否以链接文本显示 
	void SetLink(BOOL bLink, COLORREF crLinkText = RGB(0,0,255)); 
 
	//是否链接 
	BOOL	IsLink(void); 
	 
 
private: 
	//绘制背景和文本 
	void DrawBackground(CDC& dc,CRect& clientrect); 
	void DrawText(CDC& dc,CRect& clientrect); 
 
	BOOL CreateFont(void);//创建字体 
 
	CFont m_textfont;//字体 
 
	COLORREF    m_crOldText;//原文本色
	COLORREF	m_crText;//文本色 
	COLORREF	m_crBkground;//背景色 
 
	LOGFONT		m_lf;//字体结构 
 
	BOOL		m_bLink;//是否是链接 
 
	int			m_Type;//闪烁类型 
	BOOL		m_bFlash;//是否闪烁 
	 
	HCURSOR		m_hCursor;//光标句柄 
 
	BOOL		m_bStartTimerFlag;//启动定时器标志 
 
	//{{AFX_VIRTUAL(CLabel) 
	//}}AFX_VIRTUAL 
protected: 
	//{{AFX_MSG(CLabel) 
	afx_msg void OnTimer(UINT nIDEvent); 
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); 
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message); 
	afx_msg BOOL OnEraseBkgnd(CDC* pDC); 
	afx_msg void OnPaint(void); 
	afx_msg void OnDestroy(void); 
	//}}AFX_MSG 
 
	DECLARE_MESSAGE_MAP() 
}; 
 
///////////////////////////////////////////////////////////////////////////// 
 
//{{AFX_INSERT_LOCATION}} 
// Microsoft Developer Studio will insert additional declarations immediately before the previous line. 
 
#endif // !defined(AFX_LABEL_H__A4EABEC5_2E8C_11D1_B79F_00805F9ECE10__INCLUDED_) 