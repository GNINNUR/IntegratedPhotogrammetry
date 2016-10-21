
// PhotoOrientationView.cpp : CPhotoOrientationView ���ʵ��
//

#include "stdafx.h"
#include "PhotoOrientation.h"

#include "PhotoOrientationDoc.h"
#include "PhotoOrientationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPhotoOrientationView

IMPLEMENT_DYNCREATE(CPhotoOrientationView, CView)

BEGIN_MESSAGE_MAP(CPhotoOrientationView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPhotoOrientationView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CPhotoOrientationView ����/����

CPhotoOrientationView::CPhotoOrientationView()
{
	// TODO: �ڴ˴���ӹ������

}

CPhotoOrientationView::~CPhotoOrientationView()
{
}

BOOL CPhotoOrientationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPhotoOrientationView ����

void CPhotoOrientationView::OnDraw(CDC* /*pDC*/)
{
	CPhotoOrientationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPhotoOrientationView ��ӡ


void CPhotoOrientationView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CPhotoOrientationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPhotoOrientationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPhotoOrientationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CPhotoOrientationView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPhotoOrientationView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CPhotoOrientationView ���

#ifdef _DEBUG
void CPhotoOrientationView::AssertValid() const
{
	CView::AssertValid();
}

void CPhotoOrientationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPhotoOrientationDoc* CPhotoOrientationView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhotoOrientationDoc)));
	return (CPhotoOrientationDoc*)m_pDocument;
}
#endif //_DEBUG


// CPhotoOrientationView ��Ϣ�������
