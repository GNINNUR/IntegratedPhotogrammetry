// BlockSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BlockSetting.h"
#include "BlockSettingDlg.h"
#include "..\..\include\lidBase.h"

//#include "orsGuiBase/orsIProcessMsgBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockSettingDlg dialog


CBlockSettingDlg::CBlockSettingDlg(CALMPrjManager *pBlockInfo, int nTab, CWnd* pParent /*=NULL*/)
	: CDialog(CBlockSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockSettingDlg)
	m_pBlockInfo = pBlockInfo;
	m_newBlockInfo.Copy(*pBlockInfo);
	m_nTab = nTab;
	//}}AFX_DATA_INIT
}


void CBlockSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockSettingDlg)
	DDX_Control(pDX, IDC_TAB, m_wndTabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CBlockSettingDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockSettingDlg message handlers

BOOL CBlockSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	strTemp;

	m_wndPageImg.m_pBlockInfo = &m_newBlockInfo;
	m_wndPageLidar.m_pBlockInfo = &m_newBlockInfo;
	m_wndPageModelSpecific.m_pBlockInfo = &m_newBlockInfo;
	
	strTemp.LoadString(IDS_PageName_Image);
	m_wndPageImg.Create(CImgSettingPage::IDD, &m_wndTabCtrl);
	m_wndTabCtrl.AddTab(&m_wndPageImg,strTemp.GetBuffer(128));

	strTemp.LoadString(IDS_PageName_Lidar);
	m_wndPageLidar.Create(CLidarSettingPage::IDD, &m_wndTabCtrl);
	m_wndTabCtrl.AddTab(&m_wndPageLidar, strTemp.GetBuffer(128));

// 	strTemp.LoadString(IDS_PageName_CoordSys);
// 	m_wndPageCoordSys.Create(CCoordSysPage::IDD, &m_wndTabCtrl);
// 	m_wndTabCtrl.AddTab(&m_wndPageCoordSys, strTemp.GetBuffer(128));

// 	strTemp.LoadString(IDS_PageName_Projection);
// 	m_wndPageProjection.Create(CProjectionPage::IDD, &m_wndTabCtrl);
// 	m_wndTabCtrl.AddTab(&m_wndPageProjection, strTemp.GetBuffer(128));

	strTemp.LoadString(IDS_PageName_ModelSpecific);
	m_wndPageModelSpecific.Create(CModelSpecificPage::IDD, &m_wndTabCtrl);
	m_wndTabCtrl.AddTab(&m_wndPageModelSpecific, strTemp.GetBuffer(128));


	m_wndTabCtrl.SetCurFocus(m_nTab);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBlockSettingDlg::OnOK() 
{//IDOKʱ�����õ���ϢӦ���ڹ���
	
	UpdateData(TRUE);

	CString	strExistingFileName;
	CString strNewFileName;
	CString	fileName;
	CString strNewDir;
	int	i, num;

//	ref_ptr<orsIProcessMsgBar> process = ORS_CREATE_OBJECT( orsIProcessMsgBar, "ors.process.bar.mfc");

	if(m_wndPageImg.m_bCopyImg==TRUE)
	{//��ԴӰ�񿽱�������Ŀ¼
		iphImgList	*imgList;

		imgList=m_newBlockInfo.GetImgList();
		num=imgList->GetSize();
		strNewDir=m_pBlockInfo->GetRawImageDir();
//		process->InitProgressBar("����Ӱ��...", "wait...", num);
		for(i=0; i<num; i++)
		{
			strExistingFileName=(*imgList)[i].strImgName;
			fileName=_ExtractFileFullName(strExistingFileName);
			strNewFileName=strNewDir+_T("\\")+fileName;

			CopyFile(strExistingFileName, strNewFileName, TRUE);	//�����ǵ����ڵ��ļ�
			(*imgList)[i].strImgName=strNewFileName;	//�޸��ļ���Ϊ����Ŀ¼�µ�Ӱ��
			
			(*imgList)[i].camera.CalRotateMatrix();		//������ת����
//			process->SetPos(i+1);
		}
		
		AfxMessageBox("Ӱ�񿽱����!", MB_OK);
	}
	else
	{
		iphImgList	*imgList;
		
		imgList=m_newBlockInfo.GetImgList();
		num=imgList->GetSize();
		//strNewDir=m_pBlockInfo->GetRawImageDir();
		//		process->InitProgressBar("����Ӱ��...", "wait...", num);
		for(i=0; i<num; i++)
		{
// 			strExistingFileName=(*imgList)[i].strImgName;
// 			fileName=_ExtractFileFullName(strExistingFileName);
// 			strNewFileName=strNewDir+_T("\\")+fileName;
// 			
// 			CopyFile(strExistingFileName, strNewFileName, TRUE);	//�����ǵ����ڵ��ļ�
//			(*imgList)[i].strImgName=strNewFileName;	//�޸��ļ���Ϊ����Ŀ¼�µ�Ӱ��
			
			(*imgList)[i].camera.CalRotateMatrix();		//������ת����
			//			process->SetPos(i+1);
		}
	}


	if(m_wndPageLidar.m_bCopyLIDAR==TRUE)
	{//��ԭʼ���ƿ���������Ŀ¼
		iphLIDARList	*LidarList;
				
		LidarList=m_newBlockInfo.GetLidarList();
		num=LidarList->GetSize();
		strNewDir=m_pBlockInfo->GetRawLidarDir();
		for(i=0; i<num; i++)
		{
			strExistingFileName=(*LidarList)[i].strLidarName;
			fileName=_ExtractFileFullName(strExistingFileName);
			strNewFileName=strNewDir+_T("\\")+fileName;
			
			CopyFile(strExistingFileName, strNewFileName, TRUE);	//�����ǵ����ڵ��ļ�
			(*LidarList)[i].strLidarName=strNewFileName;	//�޸��ļ���Ϊ����Ŀ¼�µ�Ӱ��
			//			process->SetPos(i+1);
		}
		
		AfxMessageBox("las�������!", MB_OK);
	}
	
	m_pBlockInfo->Copy(m_newBlockInfo);

	m_pBlockInfo->SetMaxGrdElev(m_wndPageModelSpecific.m_dMaxHeight);
	m_pBlockInfo->SetMinGrdElev(m_wndPageModelSpecific.m_dMinHeight);
	m_pBlockInfo->SetAverGrdElev((m_wndPageModelSpecific.m_dMaxHeight+m_wndPageModelSpecific.m_dMinHeight)/2);

	m_pBlockInfo->SetFlyOverlap(m_wndPageModelSpecific.m_flyoverlap);
	m_pBlockInfo->SetStripOverlap(m_wndPageModelSpecific.m_stripoverlap);


	CDialog::OnOK();
}
