
// mapView.cpp : CmapView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "map.h"
#endif

#include "mapDoc.h"
#include "mapView.h"
#include <fstream>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif


// CmapView

IMPLEMENT_DYNCREATE(CmapView, CView)

BEGIN_MESSAGE_MAP(CmapView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CmapView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_DRAW_LINE, &CmapView::OnDrawLine)
	ON_COMMAND(ID_READ_DEM, &CmapView::OnReadDem)
	ON_COMMAND(ID_DRAW_SECTION, &CmapView::OnDrawSection)
END_MESSAGE_MAP()

// CmapView 构造/析构

CmapView::CmapView()
	: m_bPress(false)
{
	// TODO: 在此处添加构造代码
	m_bPress = false;
	m_pEle = NULL;
	m_px = NULL;
	m_py = NULL;
	m_bDrawDem = false;
	m_bDrawSection = false;
}

CmapView::~CmapView()
{
	if(m_pEle)
	{
		delete[] m_pEle;
		m_pEle = NULL;
	}
	if (m_px)
	{
		delete[] m_px;
		m_px = NULL;
	}
	if (m_py)
	{
		delete[] m_py;
		m_py = NULL;
	}
}

BOOL CmapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CmapView 绘制

void CmapView::OnDraw(CDC* pDC)
{
	CmapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if(m_bPress)
	{
		pDC->MoveTo(0,0);
		pDC->LineTo(1000,1700);
		m_bPress = false;
	}	
	if (m_bDrawDem)
	{
		DrawDem(pDC);
		m_bDrawDem = false;		
	}
	if (m_bDrawSection)
	{
		SetPoint(myDlg.a, myDlg.b, myDlg.c, myDlg.d);
		GetPoint(pDC);
		m_bDrawDem = false;
		m_bDrawSection = false;
	}
}


// CmapView 打印


void CmapView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CmapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CmapView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CmapView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CmapView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CmapView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CmapView 诊断

#ifdef _DEBUG
void CmapView::AssertValid() const
{
	CView::AssertValid();
}

void CmapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmapDoc* CmapView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmapDoc)));
	return (CmapDoc*)m_pDocument;
}
#endif //_DEBUG


// CmapView 消息处理程序


void CmapView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CRect rc;
	GetClientRect(&rc);
	int nWidth = 1000;
	int nHeight = 1800;
	pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetViewportOrg(rc.left, rc.top);
	pDC->SetWindowOrg(0, 0);
	pDC->SetViewportExt(rc.Width(), rc.Height());
	pDC->SetWindowExt(nWidth, nHeight);
	
	CView::OnPrepareDC(pDC, pInfo);
}


int CmapView::myFuction(int a,int b,float c)
{
	int x,y;
	return 0;
}


void CmapView::OnDrawLine()
{
	// TODO: 在此添加命令处理程序代码
	m_bPress = true;
	Invalidate();
}


void CmapView::OnReadDem()
{
	// TODO: 在此添加命令处理程序代码
	OpenFile("D:\\实习dem-中国格式.dem");
	if(m_bDrawDem)
		Invalidate();
}


bool CmapView::OpenFile(char* szFile)
{
	ifstream is(szFile);
	if(is.bad())
		return false;
	char message[300];
	is.seekg(0,ios::beg);
	for(;;)
	{
		char message[200];
		is.getline(message,sizeof(message)-1,'\n');

		CString s(message);
		s.TrimLeft();
		s.TrimRight();
		int nPos = s.Find(_T(":"));
		int nLen = s.GetLength();
		if(nPos==-1)
			continue;
		CString s1 = s.Left(nPos);
		CString s2 = s.Right(nLen-nPos-1);
		if(s1.CompareNoCase(_T("DataMark"))==0)
		{
			if(s2.CompareNoCase(_T("CNSDTF-DEM"))==0)
			{
				AfxMessageBox(_T("当前DEM文件非国家空间数据交换格式！"));
				return false;
			}
		}
		else if(s1.CompareNoCase(_T("Version"))==0)
		{

		}
		else if(s1.CompareNoCase(_T("Unit"))==0)
		{

		}
		else if(s1.CompareNoCase(_T("Alpha"))==0)
		{

		}
		else if(s1.CompareNoCase(_T("Compress"))==0)
		{

		}
		else if(s1.CompareNoCase(_T("X0"))==0)
			m_fLeftTopX = _ttof(s2);
		else if(s1.CompareNoCase(_T("Y0"))==0)
			m_fLeftTopY = _ttof(s2);
		else if(s1.CompareNoCase(_T("DX"))==0)
			m_fGridX = _ttof(s2);
		else if(s1.CompareNoCase(_T("DY"))==0)
			m_fGridY = _ttof(s2);
		else if(s1.CompareNoCase(_T("Row"))==0)
			m_nRow = _ttoi(s2);
		else if(s1.CompareNoCase(_T("Col"))==0)
			m_nCol = _ttoi(s2);
		else if(s1.CompareNoCase(_T("ValueType"))==0)
		{

		}
		else if(s1.CompareNoCase(_T("Hzoom"))==0)
		{
			m_nScale = _ttoi(s2);
			break;
		}
	}
	if(fabs(m_fGridX-m_fGridY)>0.001)
	{
		AfxMessageBox(_T("系统要求DEM为正方形网格！"));
		return false;
	}
	m_pEle = new int[m_nRow*m_nCol];
	int i,j,k;
	for(i=0;i<m_nRow;i++)
	{
		LONG nRowPos = i*m_nCol;
		int nCharRowofCol;
		if(m_nCol%10==0)
			nCharRowofCol = m_nCol/10;
		else
			nCharRowofCol = m_nCol/10+1;
		int nCurZ[10];
		for(j=0;j<nCharRowofCol;j++)
		{
			is.getline(message,sizeof(message)-1,'\n');
			int nFields = sscanf(message,"%d%d%d%d%d%d%d%d%d%d",&nCurZ[0],&nCurZ[1],&nCurZ[2],&nCurZ[3],&nCurZ[4],&nCurZ[5],&nCurZ[6],&nCurZ[7],&nCurZ[8],&nCurZ[9]);
			int k;
			for(k=0;k<nFields;k++)
			{
				int nPos_of_Point;
				nPos_of_Point = nRowPos+j*10+k;
				m_pEle[nPos_of_Point] = nCurZ[k];
			}
		}
	}
	is.close();
	m_bDrawDem = true;
	return true;
}

void CmapView::DrawDem(CDC* pDC)
{
	int i, j;
	int color = RGB(0, 0, 0);
	for (i = 0; i < m_nRow; i++)
	{
		for (j = 0; j < m_nCol; j++)
		{
			int k = i*m_nCol+j;
			if (m_pEle[k] > 2000000)
			{
				color = RGB(100, 0, m_pEle[k] / 20000);
			}
			if (m_pEle[k] <= 2000000)
			{
				color = RGB(100, 0, (m_pEle[k] + 2000000) / 40000);
			}
			pDC->SetPixel(j, i, color);
		}
	}
}

void CmapView::SetPoint(int x1,int y1,int x2,int y2)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}


void CmapView::GetPoint(CDC* pDC)
{
	int dx, dy, n, k;
	float xinc, yinc, x, y;
	dx = x2 - x1;
	dy = y2 - y1;
	if (abs(dx) > abs(dy))
		n = abs(dx);
	else
		n = abs(dy);
	xinc = (float)dx / n;
	yinc = (float)dy / n;
	x = x1;
	y = y1;
	m_px = new int[m_nCol];
	m_py = new int[m_nRow];
	for (k = 0; k < n; k++)
	{
		m_px[k] = round(x);
		m_py[k] = round(y);
		pDC->SetPixel(round(x), round(y), RGB(0, 255, 0));
		x += xinc;
		y += yinc;
	}
	int crColor = RGB(0, 0, 200);
	int nPenstyle = 0;
	int nWidth = 1;
	CPen pen(nPenstyle, nWidth, crColor);
	CPen* oldpen = pDC->SelectObject(&pen);
	for (k = 0; k < n; k++)
	{
		pDC->MoveTo(m_px[k], 1800);
		pDC->LineTo(m_px[k], 1800-m_pEle[k]/10000);
	}
	pDC->SelectObject(oldpen);
}


void CmapView::OnDrawSection()
{
	INT_PTR nRes; 
	nRes = myDlg.DoModal();
	if (IDCANCEL == nRes)
		return;
	m_bDrawDem = true;
	m_bDrawSection = true;
	Invalidate();
}
