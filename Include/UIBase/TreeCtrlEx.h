///////////////////////////////////////////////////////////////////////////////
// 
// CTreeCtrlEx - Multiple selection tree control for MFC
// 
// Bendik Engebretsen (c) 1997-1999
// bendik@techsoft.no
// http://www.techsoft.no/bendik/
//
//

#ifndef __TREECTRLEX_H
#define __TREECTRLEX_H

#define TVGN_EX_ALL			0x000F

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlEx window

class CTreeCtrlEx : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlEx)

// Construction
public:
	CTreeCtrlEx() : m_bSelectPending(FALSE), m_hClickedItem(NULL), m_hFirstSelectedItem(NULL), m_bSelectionComplete(TRUE), m_bEditLabelPending(FALSE) {}
	BOOL Create(DWORD dwStyle, DWORD dwExStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

// Attributes
public:
	UINT GetSelectedCount() const;
	HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode);
	HTREEITEM GetFirstSelectedItem();
	HTREEITEM GetNextSelectedItem(HTREEITEM hItem);
	HTREEITEM GetPrevSelectedItem(HTREEITEM hItem);
	HTREEITEM ItemFromData(DWORD dwData, HTREEITEM hStartAtItem=NULL) const;

	BOOL SelectItemEx(HTREEITEM hItem, BOOL bSelect=TRUE);

	BOOL SelectItems(HTREEITEM hFromItem, HTREEITEM hToItem);
	void ClearSelection(BOOL bMultiOnly=FALSE);


	void     SetTreeItemImage(CString strbmp,int cx,int cy,int nGrow,COLORREF colormask,int nPointSize);
	void     SetTreeItemMulImage(int iImageCnt = 0);
	void	 SetTreeItemDepartImage(BOOL btrue = FALSE);

protected:
	void SelectMultiple( HTREEITEM hClickedItem, UINT nFlags, CPoint point );

	void     GetListImage(CImageList &ImageList,CBitmap &Bitmap ,int nImageIdx);

private:
	BOOL		m_bSelectPending;
	CPoint		m_ptClick;
	HTREEITEM	m_hClickedItem;
	HTREEITEM	m_hFirstSelectedItem;
	BOOL		m_bSelectionComplete;
	BOOL		m_bEditLabelPending;
	UINT		m_idTimer;
	int			m_iSetItemMulImageCnt;
	BOOL     m_bSetDepartImgFlg;
	int      m_icx;
	int      m_icy;
	int      m_iFontSize;
	CImageList m_ilTree;
	CBitmap m_Bitmap;

	COLORREF m_ColcorMask;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeCtrlEx() {}

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlEx)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


HTREEITEM GetTreeItemFromData(CTreeCtrl& treeCtrl, DWORD dwData, HTREEITEM hStartAtItem=NULL);

#endif
