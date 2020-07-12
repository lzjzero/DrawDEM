
// mapView.h : CmapView 类的接口
//

#pragma once
#include "CMyDialog.h"

class CmapView : public CView
{
protected: // 仅从序列化创建
	CmapView();
	DECLARE_DYNCREATE(CmapView)

// 特性
public:
	CmapDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CmapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	float m_fLeftTopX;
	float m_fLeftTopY;
	float m_fGridX;
	float m_fGridY;
	int   m_nRow;
	int   m_nCol;
	int   m_nScale;
	int*  m_pEle;
	int   x1, y1, x2, y2;
	int*  m_px;
	int*  m_py;

	int myFuction(int a,int b,float c);
	bool m_bDrawDem;
	bool m_bPress;
	afx_msg void OnDrawLine();
	afx_msg void OnReadDem();
	bool OpenFile(char* szFile);
	void DrawDem(CDC* pDC);
	void SetPoint(int x1, int y1, int x2, int y2);
	void GetPoint(CDC* pDC);
	afx_msg void OnDrawSection();
	bool m_bDrawSection;
	CMyDialog myDlg;
	afx_msg void OnCalV();
};

#ifndef _DEBUG  // mapView.cpp 中的调试版本
inline CmapDoc* CmapView::GetDocument() const
   { return reinterpret_cast<CmapDoc*>(m_pDocument); }
#endif
