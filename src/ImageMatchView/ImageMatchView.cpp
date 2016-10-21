// ImageMatchView.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ImageMatchView.h"

#include "MainFrm.h"

#include "ImageMatchViewView.h"

#include "orsBase\orsUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageMatchViewApp

BEGIN_MESSAGE_MAP(CImageMatchViewApp, CWinApp)
	//{{AFX_MSG_MAP(CImageMatchViewApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_Prj_OPEN, OnPrjOPEN)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageMatchViewApp construction

orsIPlatform *g_pPlatform = NULL;

orsIPlatform *getPlatform()
{
	return g_pPlatform;
}


CImageMatchViewApp::CImageMatchViewApp()
{
	ors_string errorinfo;
	g_pPlatform = orsInitialize(errorinfo, true );
	
	//	FastDspSetPlatform( g_pPlatform );
	
	m_pBlockManager=NULL;
}

CImageMatchViewApp::~CImageMatchViewApp()
{
	BCGCBProCleanUp();
	
	orsUninitialize();
	
	if(m_pBlockManager)
	{
		delete m_pBlockManager;
		m_pBlockManager=NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CImageMatchViewApp object

CImageMatchViewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CImageMatchViewApp initialization

BOOL CImageMatchViewApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	//CSingleDocTemplate* pDocTemplate;
	m_pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CImageMatchViewDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CImageMatchViewView));
	AddDocTemplate(m_pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CImageMatchViewApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CImageMatchViewApp message handlers

CImageMatchViewDoc *CImageMatchViewApp::GetCurDocument()
{
	CDocument  *pDoc = NULL;
	POSITION	nPos = NULL;
	
	nPos = m_pDocTemplate->GetFirstDocPosition();
	while (NULL != nPos)
	{
		pDoc = m_pDocTemplate->GetNextDoc(nPos);
		if(pDoc->IsKindOf(RUNTIME_CLASS(CImageMatchViewDoc)))
			break;
	}
	if (!pDoc) {
		OnFileNew();
		pDoc = GetTailDocument();
	}
	
	return (CImageMatchViewDoc *)(pDoc);
}

CImageMatchViewDoc *CImageMatchViewApp::GetTailDocument()
{
	CDocument  *pDoc = NULL;
	POSITION	nPos = NULL;
	CString		strTitle = _T("");
	
	nPos = m_pDocTemplate->GetFirstDocPosition();
	if (NULL != nPos)
	{
		pDoc = m_pDocTemplate->GetNextDoc(nPos);
		if (NULL == nPos) {
			return (CImageMatchViewDoc *)(pDoc);
		}
		pDoc = m_pDocTemplate->GetNextDoc(nPos);
	}
	
	return (CImageMatchViewDoc *)(pDoc);
}


void CImageMatchViewApp::SetCurDocTitle()
{
	char	chTitle[128] = "";
	CString	strTmp;
	CImageMatchViewDoc  *pDoc = GetCurDocument();
	
	memset(chTitle, 0, sizeof(char)*128);
	if (NULL == m_pBlockManager) 
	{
		strTmp.LoadString(IDS_NullBlock);
	}
	else
	{
		strTmp = m_pBlockManager->GetAlmName();
	}
	strcpy(chTitle, strTmp.GetBuffer(128));
	pDoc->SetTitle(chTitle);
}


void CImageMatchViewApp::OnPrjOPEN() 
{
	CFileDialog dlg( TRUE, "blk", NULL,OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR, _T("Block File (*.blk)|*.blk|All Files(*.*)|*.*||"));
	
	if(dlg.DoModal()!=IDOK)
		return;
	
	if(m_pBlockManager==0)
	{
		m_pBlockManager=new CALMPrjManager;
	}
	
	m_pBlockManager->OpenALMPrjFile(dlg.GetPathName());
	
}
