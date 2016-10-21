// WorkspaceBar.h : interface of the CWorkspaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__257022E7_0F22_4F6B_8E2F_8D7205DFF703__INCLUDED_)
#define AFX_WORKSPACEBAR_H__257022E7_0F22_4F6B_8E2F_8D7205DFF703__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Align_def.h"
#include "AlignPrj.h"
#include "OutputBar.h"

class CWorkspaceBar : public CBCGPDockingControlBar
{
public:
	CWorkspaceBar();

	void SetAlgPrj(CAlignPrj *pAlg);
	void SetOutputBar(COutputBar *pOutputBar);
	
	void Relayout();

private:
	void	InitTree();
	void	CleanTree();
	
	// Attributes
protected:
//	CALMPrjManager	*m_pALM;
	CAlignPrj   *m_pAlg;
	CTreeCtrl	m_wndTree;
	COutputBar  *m_pOutputBar;
	
	CImageList	m_workSpaceImages;
	
	HTREEITEM	m_hImgRoot;		//��Ƭ�б�
	HTREEITEM	m_hLiDRoot;		//�����б�
	HTREEITEM	m_hPrdRoot;		//��Ʒ�б�
	
	HTREEITEM	m_hPrd_DEMRoot;
	HTREEITEM	m_hPrd_DOMRoot;
	HTREEITEM	m_hPrd_TMRoot;
	

	CArray<Align_LidLine, Align_LidLine>	*m_pLidList;
	
private:
	void	CreateWorkSpaceImages();
	void	LayoutImage(CArray<Align_Image, Align_Image> *pImgList);
	void	LayoutLiDAR(CArray<Align_LidLine, Align_LidLine>	*pLidList);
//  	void	LayoutProduct(CArray<CString,CString> *pDEMList,
//  						  CArray<CString,CString> *pDOMList,
//  						  CArray<CString,CString> *pTMList);
	
	void	OutputImageInfo(HTREEITEM hItem);
	void	OutputLiDARInfo(HTREEITEM hItem);
	void	OutputProductInfo(HTREEITEM hItem);
	
	void	DisplayImage(HTREEITEM hItem);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkspaceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEBAR_H__257022E7_0F22_4F6B_8E2F_8D7205DFF703__INCLUDED_)
