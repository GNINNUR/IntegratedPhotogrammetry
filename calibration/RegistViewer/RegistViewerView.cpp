// RegistViewerView.cpp : implementation of the CRegistViewerView class
//

#include "stdafx.h"
#include "RegistViewer.h"
#include "MainFrm.h"

#include "RegistViewerDoc.h"
#include "RegistViewerView.h"
#include "orsMap\orsIMapService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegistViewerView

IMPLEMENT_DYNCREATE(CRegistViewerView, CSingleImageView)

BEGIN_MESSAGE_MAP(CRegistViewerView, CSingleImageView)
	//{{AFX_MSG_MAP(CRegistViewerView)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_MeasureFlag, OnMeasureFlag)
	ON_UPDATE_COMMAND_UI(ID_MeasureFlag, OnUpdateMeasureFlag)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()

	ON_COMMAND(ID_QueryPoint, OnQueryPoint)
	ON_UPDATE_COMMAND_UI(ID_QueryPoint, OnUpdateQueryPoint)

	ON_COMMAND(ID_LAYER_SWIPE_HORIZONTAL, OnLayerSwipeHorizontal)
	ON_UPDATE_COMMAND_UI(ID_LAYER_SWIPE_HORIZONTAL, OnUpdateLayerSwipeHorizontal)
	ON_COMMAND(ID_LAYER_SWIPE_VERTICAL, OnLayerSwipeVertical)
	ON_UPDATE_COMMAND_UI(ID_LAYER_SWIPE_VERTICAL, OnUpdateLayerSwipeVertical)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CSingleImageView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CSingleImageView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CSingleImageView::OnFilePrintPreview)
	ON_COMMAND(ID_PAN, CSingleImageView::OnPan)
	ON_UPDATE_COMMAND_UI(ID_PAN, CSingleImageView::OnUpdatePan)
	ON_COMMAND(ID_COORD, CSingleImageView::OnCoord)
	ON_UPDATE_COMMAND_UI(ID_COORD, CSingleImageView::OnUpdateCoord)
	ON_COMMAND(ID_ZOOM_IN, CSingleImageView::OnZoomIn)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_IN, CSingleImageView::OnUpdateZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, CSingleImageView::OnZoomOut)
	ON_UPDATE_COMMAND_UI(ID_ZOOM_OUT, CSingleImageView::OnUpdateZoomOut)
//	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegistViewerView construction/destruction

CRegistViewerView::CRegistViewerView()
{
	// TODO: add construction code here
	SetOpState( opPAN  );
	m_bMeasureFlag=false;
	m_pCurObj=NULL;

	m_pt2DBuf=new POINT2D[10240];
	m_ptNum=0;

	m_bQueryPoint = false;
}

CRegistViewerView::~CRegistViewerView()
{
	TieObject *pTObj;
	for(int i=0; i<m_TObjList.GetSize(); i++)
	{
		pTObj=m_TObjList[i];
		if(pTObj)	delete pTObj;
	}
	m_TObjList.RemoveAll();

	if(m_pt2DBuf) delete m_pt2DBuf;	m_pt2DBuf=NULL;
	m_ptNum=0;
}

BOOL CRegistViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CSingleImageView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRegistViewerView drawing

void CRegistViewerView::OnDraw(CDC* pDC)
{
	CRegistViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSingleImageView::OnDraw( pDC );

	DrawTieObjects(pDC);
}

// void CRegistViewerView::DrawYourContents()
// {
// 	DrawTieObjects(GetDC());
// 	
// }

void CRegistViewerView::DrawTieObjects(CDC *pDC)
{
	int i;
	TieObject *pTObj;

	for(i=0; i<m_TObjList.GetSize(); i++)
	{
		pTObj=m_TObjList[i];
		if(pTObj->objType==TO_POINT)
		{
			DrawTiePoint(pDC, (TiePoint*)pTObj);
		}
		else if(pTObj->objType==TO_LINE)
		{
			DrawTieLine(pDC, (TieLine*)pTObj);
		}
		else if(pTObj->objType==TO_PATCH)
		{
			DrawTiePatch(pDC, (TiePatch*)pTObj);
		}
	}
}

void CRegistViewerView::DrawTiePoint(CDC *pDC, TiePoint *pObj)
{
	CPen pen_raw;
	CPen pen_sel;
	CPen pen_areor;
	double xs, ys;
	CString icpName;
	
	pen_raw.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	pen_sel.CreatePen(PS_SOLID, 1, RGB(0,255,0));
//	pen_areor.CreatePen(PS_SOLID, 1, RGB(255,255,0));
	
	CPen* oldpen =NULL;
	
	int selID=((CMainFrame*)GetMainFrame())->m_tieID;

	if(pObj->tieID==selID)
		oldpen=pDC->SelectObject(&pen_sel);
	else
		oldpen=pDC->SelectObject(&pen_raw);

	orsIImageGeometry *pGeo=GetDocument()->m_imgGeo.get();
	
	double xi, yi;
//	pGeo->Project(pObj->pt3D.X, pObj->pt3D.Y, pObj->pt3D.Z, &xi, &yi);

//	TRACE("xi-pObj->pt2D.x=%f\n", xi-pObj->pt2D.x);
//	TRACE("yi-pObj->pt2D.y=%f\n", yi-pObj->pt2D.y);

	xi=pObj->pt2D.x;		yi=pObj->pt2D.y;

	xs = Img2VCS_X( xi );
	ys = Img2VCS_Y( yi );
	
	pDC->MoveTo( xs-5, ys);		pDC->LineTo( xs+5, ys);
	pDC->MoveTo( xs, ys-5);		pDC->LineTo( xs, ys+5);
		
	pDC->SelectObject(oldpen);
}

void CRegistViewerView::DrawTieLine(CDC *pDC, TieLine *pObj)
{
	
}

void CRegistViewerView::DrawTiePatch(CDC *pDC, TiePatch *pObj)
{
	CPen pen_raw;
	CPen pen_sel;
	CPen pen_areor;
	double xs, ys;
	CString icpName;
	
	pen_raw.CreatePen(PS_SOLID, 8, RGB(0,0,255));
	//	pen_sel.CreatePen(PS_SOLID, 1, RGB(0,255,0));
	//	pen_areor.CreatePen(PS_SOLID, 1, RGB(255,255,0));
	
//	CPen* oldpen = pDC->SelectObject(&pen_raw);

	if(pObj->ptNum<=0)	return;

//	pen_raw.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	pen_sel.CreatePen(PS_SOLID, 8, RGB(255,0,0));
	//	pen_areor.CreatePen(PS_SOLID, 1, RGB(255,255,0));

	CPen* oldpen =NULL;

	int selID=((CMainFrame*)GetMainFrame())->m_tieID;

	if(pObj->tieID==selID)
		oldpen=pDC->SelectObject(&pen_sel);
	else
		oldpen=pDC->SelectObject(&pen_raw);

	xs = Img2VCS_X( pObj->pt2D[0].x );
	ys = Img2VCS_Y( pObj->pt2D[0].y );
	pDC->MoveTo( xs, ys);

	for(int i=1; i<pObj->ptNum; i++)
	{
		xs = Img2VCS_X( pObj->pt2D[i].x );
		ys = Img2VCS_Y( pObj->pt2D[i].y );
		pDC->LineTo( xs, ys);
	}

	xs = Img2VCS_X( pObj->pt2D[0].x );
	ys = Img2VCS_Y( pObj->pt2D[0].y );
	
	pDC->LineTo( xs, ys);
	
	pDC->SelectObject(oldpen);	
}
/////////////////////////////////////////////////////////////////////////////
// CRegistViewerView printing

BOOL CRegistViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRegistViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRegistViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRegistViewerView diagnostics

#ifdef _DEBUG
void CRegistViewerView::AssertValid() const
{
	CSingleImageView::AssertValid();
}

void CRegistViewerView::Dump(CDumpContext& dc) const
{
	CSingleImageView::Dump(dc);
}

CRegistViewerDoc* CRegistViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRegistViewerDoc)));
	return (CRegistViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegistViewerView message handlers

void CRegistViewerView::OnInitialUpdate() 
{
	CRegistViewerDoc *pDoc = GetDocument();
	
	m_pLayerCollection = pDoc->getLayerCollection();
	
	m_pImg = GetDocument()->getViewSpace();
	
	if( getMapService() )
		SetLayerCollection( pDoc->getLayerCollection() );
	
	CSingleImageView::OnInitialUpdate();
}

void CRegistViewerView::DisplayCoordinates(double X, double Y, double Z, float coef, double Xt, double Yt, double Zt  )
{
	((CMainFrame*) AfxGetMainWnd())->DisplayCoord( X, Y, Z, coef, Xt, Yt, Zt);
}

void CRegistViewerView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnMeasureFlag();
	
	CSingleImageView::OnLButtonDblClk(nFlags, point);
}

static linkMSG output_msg;
#define TMeasureWINDOW_TEXT "Object Measurement"
BOOL CALLBACK EnumWinsTB(
						 HWND hwnd,      // handle to parent window
						 LPARAM lParam  )
{
	char windowText[256];
	
	GetWindowText( hwnd, windowText, 255 );
	
	if( NULL != strstr( windowText, TMeasureWINDOW_TEXT ) )
	{
		output_msg.hLinkWindows[output_msg.nWindows] = hwnd;
		//s_msg.bWindowLinkOn[s_msg.nWindows] = 1;
		output_msg.nWindows++;
		output_msg.nWindows%=32;
	}
	
	return TRUE;
}

void CRegistViewerView::OnMeasureFlag() 
{
	if(m_bMeasureFlag)
		EndMeasure();
	else
	{
		output_msg.nWindows=0;
		EnumWindows( (WNDENUMPROC)EnumWinsTB, (LPARAM)this );
		m_bMeasureFlag=true;
		SetOpState(opCOORD);
		
		m_bCursorFixedEnabled=true;
	}
	
}

void CRegistViewerView::OnUpdateMeasureFlag(CCmdUI* pCmdUI) 
{
	if(m_bMeasureFlag)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CRegistViewerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bMeasureFlag)
	{
		CRegistViewerApp *theApp=(CRegistViewerApp*)AfxGetApp();
		CMainFrame *pMainFrm=GetMainFrame();
		double xi, yi, X, Y;
		
		switch(theApp->m_tieType)
		{
		case TO_POINT:
			
			if(m_pCurObj==NULL)
			{
				m_pCurObj=new TiePoint;
				m_TObjList.Add(m_pCurObj);
			}
			
			xi=VCS2Img_X( point.x );
			yi=VCS2Img_Y( point.y );
			
			((TiePoint*)(m_pCurObj))->pt2D.x=xi;
			((TiePoint*)(m_pCurObj))->pt2D.y=yi;

			GetImageGeometry()->IntersectWithZ(xi, yi, 0, &X, &Y);
			((TiePoint*)(m_pCurObj))->pt3D.X=X;
			((TiePoint*)(m_pCurObj))->pt3D.Y=Y;
			((TiePoint*)(m_pCurObj))->pt3D.Z=0;

			m_pCurObj->sourceID=theApp->m_sourceID;
			m_pCurObj->sourceType=theApp->m_sourceType;
			m_pCurObj->objType=TO_POINT;
			
			DrawTiePoint(GetDC(), (TiePoint*)m_pCurObj);
			break;

		case TO_LINE:
			


			break;

		case TO_PATCH:

			if(m_pCurObj==NULL)
			{
				m_pCurObj=new TiePatch;
				m_TObjList.Add(m_pCurObj);

//				((TiePatch*)m_pCurObj)->ptNum=0;
			}
			
			xi=VCS2Img_X( point.x );
			yi=VCS2Img_Y( point.y );
			
			m_pt2DBuf[m_ptNum].x=xi;
			m_pt2DBuf[m_ptNum].y=yi;
			m_ptNum++;
			
			m_pCurObj->sourceID=theApp->m_sourceID;
			m_pCurObj->sourceType=theApp->m_sourceType;
			m_pCurObj->objType=TO_PATCH;

			((TiePatch*)m_pCurObj)->ptNum=m_ptNum;
			((TiePatch*)m_pCurObj)->pt2D=m_pt2DBuf;
			
			DrawTiePatch(GetDC(), (TiePatch*)m_pCurObj);			

			break;
		}
		
	}

	if(m_bQueryPoint)
	{
		double xi, yi, dx, dy;
		double minDis=25;	//5 pixel
		TieObject  *pObj;
		int queryID;

		xi=VCS2Img_X( point.x );
		yi=VCS2Img_Y( point.y );

		for(int i=0; i<m_TObjList.GetSize(); i++)
		{
			pObj=m_TObjList.GetAt(i);
			if(pObj->objType!=TO_POINT)
				continue;

			dx=((TiePoint*)(pObj))->pt2D.x-xi;
			dy=((TiePoint*)(pObj))->pt2D.y-yi;

			if(dx*dx+dy*dy<minDis)
			{
				minDis=dx*dx+dy*dy;
				queryID=pObj->tieID;
			}

		}
		
		if(minDis<25)
		{
			char msg[32];
			sprintf(msg, "ID=%d", queryID);
			AfxMessageBox(msg);
		}
	}
		
	CSingleImageView::OnLButtonDown(nFlags, point);
}

void CRegistViewerView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bMeasureFlag)
	{
		EndMeasure();	
	}

	CSingleImageView::OnRButtonDown(nFlags, point);
}


void CRegistViewerView::EndMeasure()
{
	if(m_pCurObj==NULL) return;
	m_bMeasureFlag=false;
//	m_TObjList.Add(m_pCurObj);

	//向量测窗口传消息
	
	COPYDATASTRUCT cpd;
//	linkMSG msg;
	int i, j;
	
	cpd.dwData = 0;
	cpd.cbData = sizeof( linkMSG );
	cpd.lpData = &output_msg;
	
	//	s_msg.id = ORS_LM_MOUSEMOVE;
	output_msg.id=ORS_LM_Measure_Info;
	output_msg.tieID=((CMainFrame*)GetMainFrame())->m_tieID;
	output_msg.objType=m_pCurObj->objType;
	output_msg.sourceID=m_pCurObj->sourceID;
	output_msg.sourceType=m_pCurObj->sourceType;

	if(m_pCurObj->objType==TO_POINT)
	{
		output_msg.ptNum=1;
		output_msg.ptID=0;
		output_msg.pt2D=((TiePoint*)m_pCurObj)->pt2D;
		output_msg.pt3D=((TiePoint*)m_pCurObj)->pt3D;

		for( i=0; i < output_msg.nWindows; i++ )
		{
			// 只能用SendMessage
			//::PostMessage( m_linkWindows[i], WM_COPYDATA, (UINT)m_hWnd, (LPARAM)&cpd );
			::SendMessage( output_msg.hLinkWindows[i], WM_COPYDATA, (UINT)m_hWnd, (LPARAM)&cpd );
		}
	}
	else
	{
		//POINT2D pt2d;
		double x, y; 
		POINT3D pt3D;
		
		output_msg.ptNum=m_ptNum;

// 		if(m_pCurObj->objType==TO_LINE)
// 		{
// 			output_msg.ptNum=((TieLine*)m_pCurObj)->ptNum;
// 			pt2d=((TieLine*)m_pCurObj)->pt2D;
// 			pt3d=((TieLine*)m_pCurObj)->pt3D;
// 		}
// 		else if(m_pCurObj->objType==TO_PATCH)
// 		{
// 			output_msg.ptNum=((TiePatch*)m_pCurObj)->ptNum;
// 			pt2d=((TiePatch*)m_pCurObj)->pt2D;
// 			pt3d=((TiePatch*)m_pCurObj)->pt3D;
// 		}

		if(m_pCurObj->objType==TO_PATCH)
		{
			((TiePatch*)m_pCurObj)->ptNum=m_ptNum;
			((TiePatch*)m_pCurObj)->pt2D=new POINT2D[m_ptNum];

			for(i=0; i<m_ptNum; i++)
			{
				((TiePatch*)m_pCurObj)->pt2D[i]=m_pt2DBuf[i];
			}
		}

		for(j=0; j<output_msg.ptNum; j++)
		{
			output_msg.ptID=j;
			output_msg.pt2D=m_pt2DBuf[j];
			x=m_pt2DBuf[j].x;
			y=m_pt2DBuf[j].y;

			GetImageGeometry()->IntersectWithZ(x, y, 0, &(pt3D.X), &(pt3D.Y));
			pt3D.Z=0;
			output_msg.pt3D=pt3D;

			for( i=0; i < output_msg.nWindows; i++ )
			{
				// 只能用SendMessage
				//::PostMessage( m_linkWindows[i], WM_COPYDATA, (UINT)m_hWnd, (LPARAM)&cpd );
				::SendMessage( output_msg.hLinkWindows[i], WM_COPYDATA, (UINT)m_hWnd, (LPARAM)&cpd );
			}
		}
	}
	
	m_ptNum=0;
	m_pCurObj=NULL;		//下次new一个新对象
	m_bCursorFixedEnabled=false;
	m_opState = opPAN;
}

void CRegistViewerView::OnLayerSwipeHorizontal() 
{
	// TODO: Add your command handler code here
	m_opState = (opSTATE)opSWIPE_X;
	m_viewEventlistner = NULL;

	PrepareSwipeMemDC();
}

void CRegistViewerView::OnUpdateLayerSwipeHorizontal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_opState == (opSTATE)opSWIPE_X );

	if( NULL != m_pLayerCollection ) {
		orsArray<const orsChar *> layerNames =	m_pLayerCollection->GetLayerNames( ORS_LAYER_IMAGE );
		pCmdUI->Enable( layerNames.size() > 1 );
	}
	else
		pCmdUI->Enable( FALSE );
}

void CRegistViewerView::OnLayerSwipeVertical() 
{
	// TODO: Add your command handler code here
	m_opState = (opSTATE)opSWIPE_Y;
	m_viewEventlistner = NULL;	

	PrepareSwipeMemDC();
}

void CRegistViewerView::OnUpdateLayerSwipeVertical(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( m_opState == (opSTATE)opSWIPE_Y );

	if( NULL != m_pLayerCollection ) {
		orsArray<const orsChar *> layerNames =	m_pLayerCollection->GetLayerNames( ORS_LAYER_IMAGE );
		pCmdUI->Enable( layerNames.size() > 1 );
	}
	else
		pCmdUI->Enable( FALSE );
}

void CRegistViewerView::PrepareSwipeMemDC()
{
	//orsArray<const orsChar *> layerNames =	m_pLayerCollection->GetLayerNames( ORS_LAYER_IMAGE );

	CRect rect;
	GetClientRect( &rect );

	CClientDC dc(this);

	int i;
	for( i=0; i<3; i++ )
	{
		m_memLayers[i].SetViewWorkSpace( m_viewWorkSpace );
		m_memLayers[i].SetWinSize( &dc, rect.Width(), rect.Height() );
	}

	orsILayerGroup *layerTree = m_pLayerCollection->GetLayerTree();

	
	m_memLayers[0].PaintBuf( layerTree, m_zoom, m_scrollPos_img );

	int nLayerDrawn = 0;

	orsILayerTreeItem *topLayer;
	for( i=0; i<layerTree->GetNumOfChildren(); i++)
	{
		orsILayerTreeItem *layerTreeItem = layerTreeItem = layerTree->GetChildren( i );

		if( layerTreeItem->IsVisible() ) {
			if( 0 == nLayerDrawn ) {
				topLayer = layerTreeItem;
				topLayer->SetVisible( false );
			}
			else if( 1 == nLayerDrawn )
				m_memLayers[nLayerDrawn].PaintBuf( layerTree, m_zoom, m_scrollPos_img );

			++nLayerDrawn;
		}

		if( nLayerDrawn == 2 ) {
			topLayer->SetVisible( true );
			break;
		}
	}
}

void CRegistViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	float img_x, img_y;

	opSTATE opState = m_opState;

	bool layerSwap = false;
	switch( m_opState ) {
	case opSWIPE_X:	
		if( (nFlags & MK_CONTROL) != 0 )	
			opState = (opSTATE)opSWIPE_Y;
		else if( (nFlags & MK_SHIFT) != 0 )
			layerSwap = true;
		break;
	case opSWIPE_Y:	
		if( (nFlags & MK_CONTROL) != 0 )	
			opState = (opSTATE)opSWIPE_X;
		else if( (nFlags & MK_SHIFT) != 0 )
			layerSwap = true;
		break;
	default:
		CSingleImageView::OnMouseMove(nFlags, point);

		img_x = VCS2Img_X( point.x );
		img_y = VCS2Img_Y( point.y );

		orsIImageGeometry *pGeo=GetImageGeometry();
		double Xt=0, Yt=0, Zt=0;
		
		if(pGeo)
		{
			pGeo->IntersectWithZ(img_x, img_y, Zt, &Xt, &Yt);
		}

		DisplayCoordinates( img_x, img_y, 0, m_zoom, Xt, Yt, Zt );

		return ;
	}

	switch( opState ) {
	case opSWIPE_X:
		{
			CClientDC dc( this );
			CRect rect0, rect1;

			GetClientRect( &rect0 );

			rect1 = rect0;

			//rect0.right = point.x;
			rect1.left = point.x;

			if( !layerSwap )	{
				m_memLayers[1].PaintScreen( &m_memLayers[2], rect0 );		
				m_memLayers[0].PaintScreen( &m_memLayers[2], rect1 );
			}
			else {
				m_memLayers[0].PaintScreen( &m_memLayers[2], rect0 );		
				m_memLayers[1].PaintScreen( &m_memLayers[2], rect1 );
			}

			m_memLayers[2].PaintScreen( &dc, rect0 );
		}
		break;
	case opSWIPE_Y:
		{
			CClientDC dc( this );
			CRect rect0, rect1;

			GetClientRect( &rect0 );

			rect1 = rect0;

			//rect0.bottom = point.y;
			rect1.top = point.y;

			if( !layerSwap )	{
				m_memLayers[1].PaintScreen( &m_memLayers[2], rect0 );		
				m_memLayers[0].PaintScreen( &m_memLayers[2], rect1 );
			}
			else {
				m_memLayers[0].PaintScreen( &m_memLayers[2], rect0 );		
				m_memLayers[1].PaintScreen( &m_memLayers[2], rect1 );
			}

			m_memLayers[2].PaintScreen( &dc, rect0 );
		}
		break;
	}

	img_x = VCS2Img_X( point.x );
	img_y = VCS2Img_Y( point.y );

//	DisplayCoordinates( img_x, img_y, 0, m_zoom );
}

void CRegistViewerView::OnUpdateQueryPoint(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_TObjList.GetSize()>0);

	pCmdUI->SetCheck(m_bQueryPoint);
}

void CRegistViewerView::OnQueryPoint()
{
	m_bQueryPoint=!m_bQueryPoint;
}