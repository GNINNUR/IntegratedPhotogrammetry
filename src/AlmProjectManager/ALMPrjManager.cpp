// CALMPrjManager.cpp: implementation of the CALMPrjManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ALMProjectManager.h"
#include "..\..\include\ALMPrjManager.h"
#include "Markup.h"
#include "..\..\include\lidBase.h"

//#include "lidBase.h"
//#include "LASFileIO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



HINSTANCE oldRcHandle;

CALMPrjManager::CALMPrjManager()
{
	m_xml = new CMarkup;
	InitALMPrjManager();
}

CALMPrjManager::~CALMPrjManager()
{
	m_strAlmFile="";
	m_ALMFileFlag="ALMPSProjectFile";
	m_strAlmFile="";	                 //�����ļ�ȫ·��
	m_strAlmFileDir="NULL";	             //�����ļ�·�� .alm�ļ����ڵ�·��
	m_strAlmName="";		             //�����ļ�·�� ��������׺ ".alm"
	
	m_nImgNum=0;                           //Ӱ������
	m_strRawImgDir="NULL";		           //ԭʼӰ��Ŀ¼
	m_nImgStripNum=0;                      //Ӱ�񺽴�����
	m_aryImgList.RemoveAll();              //Ӱ���б�
	m_nLidarStripNum=0;                      //Lidar��������
	m_strRawLidarDir="NULL";	               //ԭʼLidar�ļ�Ŀ¼
	m_aryStrMetaInf.RemoveAll();           //Ԫ������Ϣ
	m_aryLidarList.RemoveAll();              //Lidar�б�
	m_aryPOSData.RemoveAll();              //POS����
	m_aryEOEData.RemoveAll();              //�ⷽλԪ������
	m_aryGCPData.RemoveAll();              //������Ƶ�����
	
	m_nATGridNum=50;                       //����ƥ�������
	m_nDSMGridSize=20;                     //DSMƥ�������С

	m_dMinGrdElev=-100.0;
	m_dAverGrdElev=0.0;                      //����ƽ���߳� 
	m_dMaxGrdElev=100.0;

	m_strPOSFile="NULL";                   //POS�����ļ�
	m_strEOEFile="NULL";                   //��׼����ⷽλԪ���ļ�
	m_strCPtFile="NULL";                   //������Ƶ������ļ�
	m_strColorLutFile="NULL";              //Lidar����ɫ���ļ�
	m_strImgQVDir="NULL";                  //����ͼ·��
	m_strDgImageDir="NULL";                //�ȹ���ɫ��Ӱ��·��
	m_strMosaicDir="NULL";                 //ƴͼ·��
	m_strRegistDir="NULL";                 //Lidar��Ӱ����׼����·��
	m_strFltLidarDir="NULL";                 //�˲���Lidar����·��
	m_strMatchLidarDir="NULL";               //ƥ���Lidar����·��
	m_strDSMDir="NULL";                    //���ֱ���ģ������·��
	m_strDEMDir="NULL";                    //���ָ߳�ģ������·��
	m_strDOMDir="NULL";                    //����Ӱ������·��
	m_strCFilesDir="NULL";                 //���Ƶ���ļ�·��
	m_strLOGDir="NULL";                    //��־�ļ�·��
	m_strProductDir="NULL";                //��Ʒ·��
	m_aryDOMProduct.RemoveAll();           //DOM��Ʒ����
	m_aryDEMProduct.RemoveAll();           //DEM��Ʒ����
	m_aryTMProduct.RemoveAll();            //ר���Ʒ����

	if (m_xml) 
	{
		delete m_xml;
		m_xml=NULL;
	}
}

void CALMPrjManager::InitALMPrjManager()
{
	m_strAlmFile="";
	m_ALMFileFlag="ALMPSProjectFile";
	m_strAlmFile="";	                 //�����ļ�ȫ·��
	m_strAlmFileDir="NULL";	                 //�����ļ�·�� .alm�ļ����ڵ�·��
	m_strAlmName=_T("�չ���");		             //�����ļ�·�� ��������׺ ".alm"
        	
	m_nImgNum=0;                           //Ӱ������
	m_strRawImgDir="NULL";		           //ԭʼӰ��Ŀ¼
	m_nImgStripNum=0;                      //Ӱ�񺽴�����
	m_aryImgList.RemoveAll();              //Ӱ���б�
	m_nLidarStripNum=0;                      //Lidar��������
	m_strRawLidarDir="NULL";	               //ԭʼLidar�ļ�Ŀ¼
	m_aryStrMetaInf.RemoveAll();           //Ԫ������Ϣ
	m_aryLidarList.RemoveAll();              //Lidar�б�
	m_aryPOSData.RemoveAll();              //POS����
	m_aryEOEData.RemoveAll();              //�ⷽλԪ������
	m_aryGCPData.RemoveAll();              //������Ƶ�����

	m_nATGridNum=50;                       //����ƥ�������
	m_nDSMGridSize=20;                     //DSMƥ�������С
	
	m_dMinGrdElev=-100.0;
	m_dAverGrdElev=0.0;                      //����ƽ���߳� 
	m_dMaxGrdElev=100.0;

	m_flyOverlap=65;
	m_stripOverlap=30;

	m_strPOSFile="NULL";                   //POS�����ļ�
	m_strEOEFile="NULL";                   //��׼����ⷽλԪ���ļ�
	m_strCPtFile="NULL";                   //������Ƶ������ļ�
	m_strColorLutFile="NULL";              //Lidar����ɫ���ļ�
	m_strImgQVDir="NULL";                  //����ͼ·��
	m_strDgImageDir="NULL";                //�ȹ���ɫ��Ӱ��·��
	m_strMosaicDir="NULL";                 //ƴͼ·��
	m_strRegistDir="NULL";                 //Lidar��Ӱ����׼����·��
	m_strFltLidarDir="NULL";                 //�˲���Lidar����·��
	m_strMatchLidarDir="NULL";               //ƥ���Lidar����·��
	m_strDSMDir="NULL";                    //���ֱ���ģ������·��
	m_strDEMDir="NULL";                    //���ָ߳�ģ������·��
	m_strDOMDir="NULL";                    //����Ӱ������·��
	m_strCFilesDir="NULL";                 //���Ƶ���ļ�·��
	m_strLOGDir="NULL";                    //��־�ļ�·��
	m_strProductDir="NULL";                //��Ʒ·��
	m_aryDOMProduct.RemoveAll();           //DOM��Ʒ����
	m_aryDEMProduct.RemoveAll();           //DEM��Ʒ����
	m_aryTMProduct.RemoveAll();            //ר���Ʒ����

	m_bIsPrjModified=false;
}
//��ȡ�����ļ�ͷ��Ϣ��,m_strAlmFile��ֵȫ·��
bool	CALMPrjManager::bReadAlmFileHeader()
{
	while ( m_xml->FindChildElem("MetaInformation") )
	{    
		m_xml->IntoElem();
		while(m_xml->FindChildElem( "Info" ))
		{
			CString strInfo;
			strInfo = m_xml->GetChildData();
			m_aryStrMetaInf.Add(strInfo);
		}		
		m_xml->OutOfElem();	
	}	
	return true;
}
//��ȡ�����ļ�Ӱ����Ϣ
bool	CALMPrjManager::bReadAlmFileImgInfo()
{
	HINSTANCE oldRcHandle = AfxGetResourceHandle();
#ifdef _DEBUG
	AfxSetResourceHandle ( GetModuleHandle("ALMPrjManagerD.dll") );
#else
	AfxSetResourceHandle ( GetModuleHandle("ALMPrjManager.dll") );
#endif

	CString	strImagePath;
	CString	strErr;
	while ( m_xml->FindChildElem( "ImageInformation" ))
	{		
		m_xml->IntoElem();
		//Ӱ������
		m_xml->FindChildElem( "ImageCount" );
		m_nImgNum = atoi(m_xml->GetChildData());		
		//Ӱ��·��
		m_xml->FindChildElem( "ImageDirectory" );
		m_strRawImgDir = m_xml->GetChildData(); 
		if(bIsDirectoryExist(m_strRawImgDir.GetBuffer(128))==false)
		{//��������ԭʼӰ��Ŀ¼
			
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_RawImg_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strRawImgDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strRawImgDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("������ԭʼӰ��Ŀ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strRawImgDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("������ԭʼӰ��Ŀ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strRawImgDir=strNewDir;
						break;
					}
				}
			}
		}

		//Ӱ��������
		m_xml->FindChildElem( "ImageStripCount" );
		m_nImgStripNum = atoi(m_xml->GetChildData());		
		//Ӱ���б�
		while ( m_xml->FindChildElem( "ImageList" ))
		{			
			m_xml->IntoElem();	
			for (int i=0;i<m_nImgNum;i++)
			{
				iphImgINFO imglist;
				m_xml->FindChildElem( "ImageID");
				imglist.nImgID = atoi(m_xml->GetChildData());
				m_xml->FindChildElem( "ImageName");
				imglist.strImgName = m_xml->GetChildData();
				m_xml->FindChildElem( "StripID" );
				imglist.nStripID = atoi(m_xml->GetChildData());				
				m_xml->FindChildElem( "PhotoID" );
				imglist.nPhoID = atoi(m_xml->GetChildData());
				m_xml->FindChildElem( "RotateFlag" );
				imglist.nIsReversal = atoi(m_xml->GetChildData());
// 				m_xml->FindChildElem( "UseFlag" );
// 				imglist.useful = atoi(m_xml->GetChildData());

				//m_xml->FindChildElem("CameraFileName");
				CString str=imglist.strImgName;
				int pos;
				pos=str.ReverseFind('.');
				str=str.Left(pos);
				str+=".cam";
				//sprintf(imglist.camera.m_pCameraFileName, str.GetBuffer(256));
				imglist.camera.ReadCameraFile(str.GetBuffer(256));
				
				m_aryImgList.Add(imglist);

				strImagePath=/*m_strRawImgDir+"\\"+*/imglist.strImgName;
				if(bIsFileExist(strImagePath.GetBuffer(128))==false)
				{
					strErr.Format("�޷���λ%s", strImagePath);
					AfxMessageBox(strErr.GetBuffer(256), MB_OK|MB_ICONEXCLAMATION);
					CString		strNewDir;
					CString		strTmp;
					strTmp.LoadString(IDS_ALM_RawImg_Dir);
					strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
					if(m_strRawImgDir.Compare(strNewDir)!=0)
					{
						strImagePath=strNewDir+"\\"+imglist.strImgName;
						if(bIsFileExist(strImagePath.GetBuffer(128)))
						{
							m_bIsPrjModified=true;
							m_strRawImgDir=strNewDir;
						}
						else
						{
							while(1)
							{
								if(bSelectFolderDialog("������ԭʼӰ��Ŀ¼...",&strNewDir))
								{
									m_bIsPrjModified=true;
									m_strRawImgDir=strNewDir;
									break;
								}
							}
						}
					}
					else
					{
						while(1)
						{
							if(bSelectFolderDialog("������ԭʼӰ��Ŀ¼...",&strNewDir))
							{
								m_bIsPrjModified=true;
								m_strRawImgDir=strNewDir;
								break;
							}
						}
					}
				}
			}
			m_xml->OutOfElem();
		}
		
		m_xml->OutOfElem();
	}

	AfxSetResourceHandle ( oldRcHandle );

	return true;
}
//��ȡ�����ļ�Lidar��Ϣ
bool	CALMPrjManager::bReadAlmFileLidarInfo()
{
	HINSTANCE oldRcHandle = AfxGetResourceHandle();
#ifdef _DEBUG
	AfxSetResourceHandle ( GetModuleHandle("ALMPrjManagerD.dll") );
#else
	AfxSetResourceHandle ( GetModuleHandle("ALMPrjManager.dll") );
#endif

	CString	strLidarPath;
	CString strErr;
	while ( m_xml->FindChildElem( "LidarInformation" ))
	{
		
		m_xml->IntoElem();
		//Lidar·��
		m_xml->FindChildElem( "LidarDirectory" );
		m_strRawLidarDir = m_xml->GetChildData();
		if(bIsDirectoryExist(m_strRawLidarDir.GetBuffer(128))==false)
		{
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_RawLidar_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strRawLidarDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strRawLidarDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("������ԭʼLidarĿ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strRawLidarDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("������ԭʼLidarĿ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strRawLidarDir=strNewDir;
						break;
					}
				}
			}
		}
		//Lidar������
		m_xml->FindChildElem( "LidarStripCount" );
		m_nLidarStripNum = atoi(m_xml->GetChildData());
		
		//Lidar�б�
		while ( m_xml->FindChildElem( "LidarList" ))
		{
			m_xml->IntoElem();	
			for (int i=0;i<m_nLidarStripNum;i++)
			{
				LIDARINFO Lidarlist;
				m_xml->FindChildElem( "LidarName");
				Lidarlist.strLidarName=m_xml->GetChildData();			
				m_xml->FindChildElem( "StripID" );
				Lidarlist.nStripID=atoi(m_xml->GetChildData());
				m_xml->FindChildElem( "MinX" );
				Lidarlist.dfBox3D.XMin=atof(m_xml->GetChildData());
				m_xml->FindChildElem( "MinY" );
				Lidarlist.dfBox3D.YMin=atof(m_xml->GetChildData());
				m_xml->FindChildElem( "MinZ" );
				Lidarlist.dfBox3D.ZMin=atof(m_xml->GetChildData());
				m_xml->FindChildElem( "MaxX" );
				Lidarlist.dfBox3D.XMax=atof(m_xml->GetChildData());
				m_xml->FindChildElem( "MaxY" );
				Lidarlist.dfBox3D.YMax=atof(m_xml->GetChildData());
				m_xml->FindChildElem( "MaxZ" );
				Lidarlist.dfBox3D.ZMax=atof(m_xml->GetChildData());
				m_aryLidarList.Add(Lidarlist);

				strLidarPath=/*m_strRawLidarDir+"\\"+*/Lidarlist.strLidarName;
				if(bIsFileExist(strLidarPath.GetBuffer(128))==false)
				{
					strErr.Format("�޷���λ%s", strLidarPath);
					AfxMessageBox(strErr.GetBuffer(256), MB_OK|MB_ICONEXCLAMATION);

					CString		strNewDir;
					CString		strTmp;
					strTmp.LoadString(IDS_ALM_RawLidar_Dir);
					strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
					if(m_strRawLidarDir.Compare(strNewDir)!=0)
					{
						strLidarPath=strNewDir+"\\"+Lidarlist.strLidarName;
						if(bIsFileExist(strLidarPath.GetBuffer(128)))
						{
							m_bIsPrjModified=true;
							m_strRawLidarDir=strNewDir;
						}
						else
						{
							while(1)
							{
								if(bSelectFolderDialog("������ԭʼLidarĿ¼...",&strNewDir))
								{
									m_bIsPrjModified=true;
									m_strRawLidarDir=strNewDir;
									break;
								}
							}
						}
					}
					else
					{
						while(1)
						{
							if(bSelectFolderDialog("������ԭʼLidarĿ¼...",&strNewDir))
							{
								m_bIsPrjModified=true;
								m_strRawLidarDir=strNewDir;
								break;
							}
						}
					}
				}

			}
			m_xml->OutOfElem();
		}
		
		m_xml->OutOfElem();
	}

	AfxSetResourceHandle ( oldRcHandle );

	return true;
}
//��ȡ�����ļ��������
bool	CALMPrjManager::bReadAlmFileCameraParas()
{
	while ( m_xml->FindChildElem("CameraInformation"))
	{		
		m_xml->IntoElem();
		
		//�������
		m_xml->FindChildElem( "X0" );
		m_Camera.dfU0=atof(m_xml->GetChildData());
		m_xml->FindChildElem( "Y0" );
		m_Camera.dfV0=atof(m_xml->GetChildData());
		m_xml->FindChildElem( "F" );
		m_Camera.dfF=atof(m_xml->GetChildData());
		m_xml->FindChildElem( "PixelSize" );
		m_Camera.dfPixSize=atof(m_xml->GetChildData());
		m_xml->FindChildElem( "FrameWidth" );
		m_Camera.nWidth=atof(m_xml->GetChildData());
		m_xml->FindChildElem( "FrameHeight" );
		m_Camera.nHeight=atof(m_xml->GetChildData());
		
		//��������
		m_xml->FindChildElem( "DistortionType" );
		m_Camera.nDistortionNum=atoi(m_xml->GetChildData());
		//�������	
		if (m_Camera.nDistortionNum==4)
		{
			m_xml->FindChildElem( "K1" );
			m_Camera.dfK1=atof(m_xml->GetChildData());
			m_xml->FindChildElem( "K2" );
			m_Camera.dfK2=atof(m_xml->GetChildData());
			m_xml->FindChildElem( "P1" );
			m_Camera.dfP1=atof(m_xml->GetChildData());
			m_xml->FindChildElem( "P2" );
			m_Camera.dfP2=atof(m_xml->GetChildData());
			
		}
		else  if (m_Camera.nDistortionNum==3)         //[Count_A0_A1_R]
		{
			m_xml->FindChildElem( "A0" );
			m_Camera.dfA0=atof(m_xml->GetChildData());
			m_xml->FindChildElem( "A1" );
			m_Camera.dfA1=atof(m_xml->GetChildData());
			m_xml->FindChildElem( "R" );
			m_Camera.dfR=atof(m_xml->GetChildData());		
		}
		else
		{
			return false;
		}
		m_xml->OutOfElem();
	}
	return true;
}
//��ȡ�����ļ�Ӱ��ƥ�����
bool	CALMPrjManager::bReadAlmFileImgMachParas()
{
	while ( m_xml->FindChildElem("ImgMachInformation"))
	{
		m_xml->IntoElem();
		
		//����������
		m_xml->FindChildElem( "ATGridNum" );
		m_nATGridNum=atoi(m_xml->GetChildData());
		//DSM����
		m_xml->FindChildElem( "DSMGridSize" );
		m_nDSMGridSize=atoi(m_xml->GetChildData());
		
		//����ƽ���߳�
		m_xml->FindChildElem( "AverGroundElev" );
		m_dAverGrdElev=atof(m_xml->GetChildData());
		//���߳�
		m_xml->FindChildElem( "MaxGroundElev" );
		m_dMaxGrdElev=atof(m_xml->GetChildData());
		//��С�߳�
		m_xml->FindChildElem( "MinGroundElev" );
		m_dMinGrdElev=atof(m_xml->GetChildData());

		
		m_xml->OutOfElem();
	}
	return true;
}
//��ȡ�����ļ�����ļ�·����Ϣ
bool	CALMPrjManager::bReadAlmFileDirInfo()
{
	HINSTANCE oldRcHandle = AfxGetResourceHandle();
#ifdef _DEBUG
	AfxSetResourceHandle ( GetModuleHandle("ALMPrjManagerD.dll") );
#else
	AfxSetResourceHandle ( GetModuleHandle("ALMPrjManager.dll") );
#endif

	while ( m_xml->FindChildElem("FileFolderInformation"))
	{
		m_xml->IntoElem();		
		//POS����ȫ·��
		m_xml->FindChildElem( "POSDataFile"); m_strPOSFile = m_xml->GetChildData();
		//�ⷽλԪ������ȫ·��
		m_xml->FindChildElem( "ExteriorOrientationElementsFile"); m_strEOEFile = m_xml->GetChildData();
		//���Ƶ�����ȫ·��
		m_xml->FindChildElem( "ControlPointFile"); m_strCPtFile = m_xml->GetChildData();
		//Lidarɫ���ļ�ȫ·��
		m_xml->FindChildElem( "ColorLutFile"); m_strColorLutFile = m_xml->GetChildData();
		//����ͼ·��
		m_xml->FindChildElem( "ImageQuickViewDirectory"); m_strImgQVDir = m_xml->GetChildData();
		if(bIsDirectoryExist(m_strImgQVDir.GetBuffer(128))==false)
		{//��������ԭʼӰ��Ŀ¼
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_QV_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strImgQVDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strImgQVDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("����������ͼĿ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strImgQVDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("����������ͼĿ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strImgQVDir=strNewDir;
						break;
					}
				}
			}
		}
		//��ɫ��Ӱ��·��
		m_xml->FindChildElem( "DodgingImageDirectory"); m_strDgImageDir = m_xml->GetChildData();
		if(bIsDirectoryExist(m_strDgImageDir.GetBuffer(128))==false)
		{//����������ɫӰ��Ŀ¼
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_DodgImg_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strDgImageDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strDgImageDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("��������ɫ��Ӱ��Ŀ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strDgImageDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("��������ɫ��Ӱ��Ŀ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strDgImageDir=strNewDir;
						break;
					}
				}
			}
			
		}
		//ƴͼӰ��·��
		m_xml->FindChildElem( "MosaicDirectory"); m_strMosaicDir = m_xml->GetChildData();
		if(bIsDirectoryExist(m_strMosaicDir.GetBuffer(128))==false)
		{//��������ƴͼĿ¼
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_Mosaic_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strMosaicDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strMosaicDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("������ƴͼӰ��Ŀ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strMosaicDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("������ƴͼӰ��Ŀ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strMosaicDir=strNewDir;
						break;
					}
				}
			}
			
		}
		//Lidar��Ӱ����׼����·��
		m_xml->FindChildElem( "RegistDirectory"); m_strRegistDir = m_xml->GetChildData();
		if(bIsDirectoryExist(m_strRegistDir.GetBuffer(128))==false)
		{//����������׼Ŀ¼
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_Regist_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strRegistDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strRegistDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("��������׼����Ŀ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strRegistDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("��������׼����Ŀ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strRegistDir=strNewDir;
						break;
					}
				}
			}
		}
		//�˲���Lidar����·��
		m_xml->FindChildElem( "FilterLidarDirectory"); m_strFltLidarDir = m_xml->GetChildData();
		if(bIsDirectoryExist(m_strFltLidarDir.GetBuffer(128))==false)
		{//���������˲���Lidar����Ŀ¼
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_FilterLidar_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strFltLidarDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strFltLidarDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("�������˲���Lidar����Ŀ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strFltLidarDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("�������˲���Lidar����Ŀ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strFltLidarDir=strNewDir;
						break;
					}
				}
			}
		}
		//ƥ���Lidar����·��
		m_xml->FindChildElem( "MatchLidarDirectory"); m_strMatchLidarDir = m_xml->GetChildData();
		if(bIsDirectoryExist(m_strMatchLidarDir.GetBuffer(128))==false)
		{//��������ƥ���Lidar����Ŀ¼
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_MatchLidar_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strMatchLidarDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strMatchLidarDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("������ƥ���Lidar����Ŀ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strMatchLidarDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("������ƥ���Lidar����Ŀ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strMatchLidarDir=strNewDir;
						break;
					}
				}
			}
		}
		//DSM����·��
		m_xml->FindChildElem( "DSMDirectory"); m_strDSMDir = m_xml->GetChildData();	
		if(bIsDirectoryExist(m_strDSMDir.GetBuffer(128))==false)
		{//��������DSM����Ŀ¼
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_DSM_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strDSMDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strDSMDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("������DSM����Ŀ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strDSMDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("������DSM����Ŀ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strDSMDir=strNewDir;
						break;
					}
				}
			}
		}
		//DEM����·��
		m_xml->FindChildElem( "DEMDirectory"); m_strDEMDir = m_xml->GetChildData();	
		if(bIsDirectoryExist(m_strDEMDir.GetBuffer(128))==false)
		{//��������DEM����Ŀ¼
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_DEM_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strDEMDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strDEMDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("������DEM����Ŀ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strDEMDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("������DEM����Ŀ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strDEMDir=strNewDir;
						break;
					}
				}
			}
		}
		//DOM����·��
		m_xml->FindChildElem( "DOMDirectory"); m_strDOMDir = m_xml->GetChildData();	
		if(bIsDirectoryExist(m_strDOMDir.GetBuffer(128))==false)
		{//��������DOM����Ŀ¼
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_DOM_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strDOMDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strDOMDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("������DOM����Ŀ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strDOMDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("������DOM����Ŀ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strDOMDir=strNewDir;
						break;
					}
				}
			}
		}
		//��������·��
		m_xml->FindChildElem( "ControlDirectory"); m_strCFilesDir = m_xml->GetChildData();
		//��־�ļ�·��
		m_xml->FindChildElem( "LogDirectory"); m_strLOGDir = m_xml->GetChildData();	
		if(bIsDirectoryExist(m_strLOGDir.GetBuffer(128))==false)
		{//����������־�ļ�Ŀ¼
			CString		strNewDir;
			CString		strTmp;
			strTmp.LoadString(IDS_ALM_LOG_Dir);
			strNewDir=m_strAlmFileDir+"\\"+strTmp;	//����Ĭ��·��
			if(m_strLOGDir.Compare(strNewDir)!=0)
			{
				if(bIsDirectoryExist(strNewDir.GetBuffer(128)))
				{
					m_bIsPrjModified=true;
					m_strLOGDir=strNewDir;
				}
				else
				{
					while(1)
					{
						if(bSelectFolderDialog("��������־�ļ�Ŀ¼...",&strNewDir))
						{
							m_bIsPrjModified=true;
							m_strLOGDir=strNewDir;
							break;
						}
					}
				}
			}
			else
			{
				while(1)
				{
					if(bSelectFolderDialog("��������־�ļ�Ŀ¼...",&strNewDir))
					{
						m_bIsPrjModified=true;
						m_strLOGDir=strNewDir;
						break;
					}
				}
			}
			
		}
		m_xml->OutOfElem();
	}
	AfxSetResourceHandle ( oldRcHandle );
	return true;
}
//��ȡ�����ļ���ز�Ʒ·����Ϣ
bool	CALMPrjManager::bReadAlmFileProductInfo()
{
	while ( m_xml->FindChildElem("ProductInformation"))
	{	
		m_xml->IntoElem();
		
		while ( m_xml->FindChildElem("DOMProduct"))
		{			
			m_xml->IntoElem();
			while (m_xml->FindChildElem("DOM"))
			{
				CString strDOM;
				strDOM=m_xml->GetChildData();
				m_aryDOMProduct.Add(strDOM);
			}			
			m_xml->OutOfElem();
		}
		
		while ( m_xml->FindChildElem("DEMProduct"))
		{
			m_xml->IntoElem();
			while (m_xml->FindChildElem("DEM"))
			{
				CString strDEM;
				strDEM=m_xml->GetChildData();
				m_aryDEMProduct.Add(strDEM);
			}			
			m_xml->OutOfElem();
		}
		while ( m_xml->FindChildElem("ThematicProduct"))
		{
			m_xml->IntoElem();
			while (m_xml->FindChildElem("Thematic"))
			{
				CString strThematic;
				strThematic=m_xml->GetChildData();
				m_aryTMProduct.Add(strThematic);
			}			
			m_xml->OutOfElem();
		}	
		
		m_xml->OutOfElem();
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::bReadAlmFile()
//	  Function     :	��ȡ�����ļ���Ϣ
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	��Ϊm_strAlmFile��ֵȫ·��,�ٶ�ȡ�ù����ļ�
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.18
////////////////////////////////////////////////////////////////////////////////
bool	CALMPrjManager::bReadAlmFile(CString strAlmFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//��鹤��·���Ƿ����
	if(GetFileAttributes(strAlmFile) == -1) 
	{
		//ָ���ļ�������
		CString strMsg;
		strMsg.Format("%s�ļ�������,��ȷ�Ϲ���·����ȷ��!",(const char*)strAlmFile);
		AfxMessageBox(strMsg);
		return false;
	}
	
	if (!m_xml->Load(strAlmFile)) 
	{
		AfxMessageBox("�򿪹����ļ�ʧ��!");
		return false;
	}
	m_strAlmFile=strAlmFile;	
	m_strAlmFileDir=m_strAlmFile.Left(m_strAlmFile.ReverseFind('\\'));
	m_strAlmName=m_strAlmFile.Right(m_strAlmFile.GetLength()-m_strAlmFileDir.GetLength()-1);
	m_strAlmName=m_strAlmName.Left(m_strAlmName.GetLength()-4);
	//�����ļ�ͷ��Ϣ
	if (!bReadAlmFileHeader()) 
	{
		AfxMessageBox("û���ļ�ͷ��Ϣ!");
	}
	//����Ӱ����Ϣ
	if (!bReadAlmFileImgInfo()) 
	{
		AfxMessageBox("û��Ӱ����Ϣ!");
	}
	//������Lidar��Ϣ
	if (!bReadAlmFileLidarInfo()) 
	{
		AfxMessageBox("û��Lidar��Ϣ!");
	}
	//�����������
	if (!bReadAlmFileCameraParas()) 
	{
		AfxMessageBox("û�����������Ϣ!");
	}	
	//����Ӱ��ƥ�����
	if (!bReadAlmFileImgMachParas()) 
	{
		AfxMessageBox("û��Ӱ��ƥ�����!");
	}
	//�����ļ�·����Ϣ
	if (!bReadAlmFileDirInfo()) 
	{
		AfxMessageBox("û���ļ�·����Ϣ!");
	}
	//�����������
	//POS
	CFileFind fFind;
	if (fFind.FindFile(m_strPOSFile))
	{
		if (!bReadPOSData())
		{
			AfxMessageBox("����POS����ʧ��!");
		}
	}
// 	else
// 	{
// 		AfxMessageBox("����POS����ʧ��!");
// 	}

	//�����Ʒ��Ϣ
	if (!bReadAlmFileProductInfo()) 
	{
		AfxMessageBox("û���ļ�·����Ϣ!");
	}

	return TRUE;
}
//д�빤���ļ�ͷ��Ϣ��,m_strAlmFile��ֵȫ·��
bool	CALMPrjManager::bWriteAlmFileHeader()
{
	if (m_aryStrMetaInf.GetSize()<1)
	{
		return false;
	}
	
	//XML�ļ�ͷ
	m_xml->SetDoc("<?xml version=\"1.0\" encoding=\"gb2312\"?>\r\n");
	m_xml->AddElem( "ALMPSProjectFile" );
	m_xml->AddChildElem( "MetaInformation" );
	m_xml->IntoElem();
	for (int i=0;i<m_aryStrMetaInf.GetSize();i++)
	{
		CString strInfo;
		strInfo=m_aryStrMetaInf[i];
		m_xml->AddChildElem( "Info", strInfo);
	}
	m_xml->OutOfElem();
	return true;
}
//д�빤���ļ�Ӱ����Ϣ
bool	CALMPrjManager::bWriteAlmFileImgInfo()
{

	if (GetImageCount()<1)
	{
		return false;
	}

	m_xml->AddChildElem( "ImageInformation" );
	m_xml->IntoElem();
	CString strInfo;
	//Ӱ������
	strInfo.Format("%d",m_nImgNum);
	m_xml->AddChildElem( "ImageCount", strInfo);
	//Ӱ��·��
	m_xml->AddChildElem( "ImageDirectory", m_strRawImgDir);

	//Ӱ��������
	strInfo.Format("%d",m_nImgStripNum);
	m_xml->AddChildElem( "ImageStripCount", strInfo);

	//Ӱ���б�
	m_xml->AddChildElem( "ImageList" );
	m_xml->IntoElem();	
	for (int i=0;i<m_nImgNum;i++)
	{
		iphImgINFO imglist;
		imglist=m_aryImgList[i];
		strInfo.Format("%d",imglist.nImgID);
		m_xml->AddChildElem( "ImageID", strInfo);
		m_xml->AddChildElem( "ImageName", imglist.strImgName);
		strInfo.Format("%d",imglist.nStripID);
		m_xml->AddChildElem( "StripID", strInfo);
		strInfo.Format("%d",imglist.nPhoID);
		m_xml->AddChildElem( "PhotoID", strInfo);
		strInfo.Format("%d",imglist.nIsReversal);
		m_xml->AddChildElem( "RotateFlag", strInfo);
// 		strInfo.Format("%c", imglist.useful);
// 		m_xml->AddChildElem( "UseFlag", strInfo);

//		strInfo=imglist.camera.m_pCameraFileName;
//		m_xml->AddChildElem( "CameraFileName", strInfo);
		
		CString str=imglist.strImgName;
		int pos;
		pos=str.ReverseFind('.');
		str=str.Left(pos);
		str+=".cam";
		if(imglist.camera.m_bInor || imglist.camera.m_bExor)
			imglist.camera.WriteCameraFile(str.GetBuffer(256));
	}
	m_xml->OutOfElem();

	m_xml->OutOfElem();
	return true;
}
//д�빤���ļ�Lidar��Ϣ
bool	CALMPrjManager::bWriteAlmFileLidarInfo()
{
	if (GetLidarStripNum()<1)
	{
		return false;
	}

	m_xml->AddChildElem( "LidarInformation" );
	m_xml->IntoElem();
	//Lidar·��
	m_xml->AddChildElem( "LidarDirectory", m_strRawLidarDir);

	CString strInfo;
	//Lidar������
	strInfo.Format("%d",m_nLidarStripNum);
	m_xml->AddChildElem( "LidarStripCount", strInfo);

	//Lidar�б�
	m_xml->AddChildElem( "LidarList" );
	m_xml->IntoElem();	
	for (int i=0;i<m_nLidarStripNum;i++)
	{
		LIDARINFO Lidarlist;
		Lidarlist=m_aryLidarList[i];
		m_xml->AddChildElem( "LidarName", Lidarlist.strLidarName);
		strInfo.Format("%d",Lidarlist.nStripID);
		m_xml->AddChildElem( "StripID", strInfo);
		strInfo.Format("%.6lf",Lidarlist.dfBox3D.XMin);
		m_xml->AddChildElem( "MinX", strInfo);
		strInfo.Format("%.6lf",Lidarlist.dfBox3D.YMin);
		m_xml->AddChildElem( "MinY", strInfo);
		strInfo.Format("%.6lf",Lidarlist.dfBox3D.ZMin);
		m_xml->AddChildElem( "MinZ", strInfo);
		strInfo.Format("%.6lf",Lidarlist.dfBox3D.XMax);
		m_xml->AddChildElem( "MaxX", strInfo);
		strInfo.Format("%.6lf",Lidarlist.dfBox3D.YMax);
		m_xml->AddChildElem( "MaxY", strInfo);
		strInfo.Format("%.6lf",Lidarlist.dfBox3D.ZMax);
		m_xml->AddChildElem( "MaxZ", strInfo);
	}
	m_xml->OutOfElem();

	m_xml->OutOfElem();
	return true;
}
//д�빤���ļ��������
bool	CALMPrjManager::bWriteAlmFileCameraParas()
{
	m_xml->AddChildElem("CameraInformation");
	m_xml->IntoElem();

	//�������
	CString strInfo;
	strInfo.Format("%.6lf",m_Camera.dfU0);
	m_xml->AddChildElem( "X0", strInfo);
	strInfo.Format("%.6lf",m_Camera.dfV0);
	m_xml->AddChildElem( "Y0", strInfo);
	strInfo.Format("%.6lf",m_Camera.dfF);
	m_xml->AddChildElem( "F", strInfo);
	strInfo.Format("%.6lf",m_Camera.dfPixSize);
	m_xml->AddChildElem( "PixelSize", strInfo);
	strInfo.Format("%d",m_Camera.nWidth);
	m_xml->AddChildElem( "FrameWidth", strInfo);
	strInfo.Format("%d",m_Camera.nHeight);
	m_xml->AddChildElem( "FrameHeight", strInfo);

	//��������
	strInfo.Format("%d",m_Camera.nDistortionNum);
	m_xml->AddChildElem( "DistortionType", strInfo);
	//�������	
	if (m_Camera.nDistortionNum==4)
	{
		strInfo.Format("%e",m_Camera.dfK1);
		m_xml->AddChildElem( "K1", strInfo);
		strInfo.Format("%e",m_Camera.dfK2);
		m_xml->AddChildElem( "K2", strInfo);
		strInfo.Format("%e",m_Camera.dfP1);
		m_xml->AddChildElem( "P1", strInfo);
		strInfo.Format("%e",m_Camera.dfP2);
		m_xml->AddChildElem( "P2", strInfo);
		
	}
	else  if (m_Camera.nDistortionNum==3)         //[Count_A0_A1_R]
	{
		strInfo.Format("%e",m_Camera.dfA0);
		m_xml->AddChildElem( "A0", strInfo);
		strInfo.Format("%e",m_Camera.dfA1);
		m_xml->AddChildElem( "A1", strInfo);
		strInfo.Format("%e",m_Camera.dfR);
		m_xml->AddChildElem( "R", strInfo);
	}
	else
	{
		return false;
	}
	m_xml->OutOfElem();
	return true;
}
//д�빤���ļ�Ӱ��ƥ�����
bool	CALMPrjManager::bWriteAlmFileImgMachParas()
{
	m_xml->AddChildElem("ImgMachInformation");
	m_xml->IntoElem();

	//����������
	CString strInfo;
	strInfo.Format("%d",m_nATGridNum);
	m_xml->AddChildElem( "ATGridNum", strInfo);

	//DSM����
	strInfo.Format("%d",m_nDSMGridSize);
	m_xml->AddChildElem( "DSMGridSize", strInfo);

	//����ƽ���߳�
	strInfo.Format("%.6lf",m_dAverGrdElev);
	m_xml->AddChildElem( "AverGroundElev", strInfo);
	//���߳�
	strInfo.Format("%.6lf",m_dMaxGrdElev);
	m_xml->AddChildElem( "MaxGroundElev", strInfo);
	//��С�߳�
	strInfo.Format("%.6lf",m_dMinGrdElev);
	m_xml->AddChildElem( "MinGroundElev", strInfo);

	strInfo.Format("%d",m_flyOverlap);
	m_xml->AddChildElem( "FlyOverlap", strInfo);

	strInfo.Format("%d",m_stripOverlap);
	m_xml->AddChildElem( "StripOverlap", strInfo);

	m_xml->OutOfElem();
	return true;
}
//д�빤���ļ�����ļ�·����Ϣ
bool	CALMPrjManager::bWriteAlmFileDirInfo()
{
	m_xml->AddChildElem("FileFolderInformation");
	m_xml->IntoElem();
	 
	//POS����ȫ·��
	m_xml->AddChildElem( "POSDataFile", m_strPOSFile);
	//�ⷽλԪ������ȫ·��
	m_xml->AddChildElem( "ExteriorOrientationElementsFile", m_strEOEFile);
	//���Ƶ�����ȫ·��
	m_xml->AddChildElem( "ControlPointFile", m_strCPtFile);
	//Lidarɫ���ļ�ȫ·��
	m_xml->AddChildElem( "ColorLutFile", m_strColorLutFile);
	//����ͼ·��
	m_xml->AddChildElem( "ImageQuickViewDirectory", m_strImgQVDir);
	//��ɫ��Ӱ��·��
	m_xml->AddChildElem( "DodgingImageDirectory", m_strDgImageDir);
	//ƴͼӰ��·��
	m_xml->AddChildElem( "MosaicDirectory", m_strMosaicDir);
	//Lidar��Ӱ����׼����·��
	m_xml->AddChildElem( "RegistDirectory", m_strRegistDir);
	//�˲���Lidar����·��
	m_xml->AddChildElem( "FilterLidarDirectory", m_strFltLidarDir);
	//ƥ���Lidar����·��
	m_xml->AddChildElem( "MatchLidarDirectory", m_strMatchLidarDir);
	//DSM����·��
	m_xml->AddChildElem( "DSMDirectory", m_strDSMDir);	
	//DEM����·��
	m_xml->AddChildElem( "DEMDirectory", m_strDEMDir);	
	//DOM����·��
	m_xml->AddChildElem( "DOMDirectory", m_strDOMDir);	
	//��������·��
	m_xml->AddChildElem( "ControlDirectory", m_strCFilesDir);
	//��־�ļ�·��
	m_xml->AddChildElem( "LogDirectory", m_strLOGDir);

	m_xml->OutOfElem();
	return true;
}
//д�빤���ļ���ز�Ʒ·����Ϣ
bool	CALMPrjManager::bWriteAlmFileProductInfo()
{
	m_xml->AddChildElem("ProductInformation");
	m_xml->IntoElem();
	 
	m_xml->AddChildElem("DOMProduct");
	m_xml->IntoElem();
	CString strFilePath;
	int i;
	for (i=0;i<m_aryDOMProduct.GetSize();i++)
	{
		strFilePath=m_aryDOMProduct[i];
		m_xml->AddChildElem( "DOM", strFilePath);
	}
	m_xml->OutOfElem();

	m_xml->AddChildElem("DEMProduct");
	m_xml->IntoElem();
	for (i=0;i<m_aryDEMProduct.GetSize();i++)
	{
		strFilePath=m_aryDEMProduct[i];
		m_xml->AddChildElem( "DEM", strFilePath);
	}
	m_xml->OutOfElem();

	m_xml->AddChildElem("ThematicProduct");
	m_xml->IntoElem();
	for (i=0;i<m_aryTMProduct.GetSize();i++)
	{
		strFilePath=m_aryTMProduct[i];
		m_xml->AddChildElem( "Thematic", strFilePath);
	}
	m_xml->OutOfElem();

	m_xml->OutOfElem();
	return true;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::bWriteAlmFile()
//	  Function     :	д������Ϣ��m_strAlmFile�ļ���
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.19
////////////////////////////////////////////////////////////////////////////////
bool	CALMPrjManager::bWriteAlmFile(CString strAlmFile)
{
	//д���ļ�ͷ��Ϣ
	if (!bWriteAlmFileHeader()) 
	{
		AfxMessageBox("û���ļ�ͷ��Ϣ!");
	}
	//д��Ӱ����Ϣ
	if (!bWriteAlmFileImgInfo()) 
	{
		AfxMessageBox("û��Ӱ����Ϣ!");
	}
	//д����Lidar��Ϣ
	if (!bWriteAlmFileLidarInfo()) 
	{
		AfxMessageBox("û��Lidar��Ϣ!");
	}
	//д���������
	if (!bWriteAlmFileCameraParas()) 
	{
		AfxMessageBox("û�����������Ϣ!");
	}	
	//д��Ӱ��ƥ�����
	if (!bWriteAlmFileImgMachParas()) 
	{
		AfxMessageBox("û��Ӱ��ƥ�����!");
	}
	//д���ļ�·����Ϣ
	if (!bWriteAlmFileDirInfo()) 
	{
		AfxMessageBox("û���ļ�·����Ϣ!");
	}
	//д���Ʒ��Ϣ
	if (!bWriteAlmFileProductInfo()) 
	{
		AfxMessageBox("û���ļ�·����Ϣ!");
	}
	m_xml->Save(strAlmFile);

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::SetImgList()
//	  Function     :	����Ӱ���б�
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
BOOL CALMPrjManager::SetImgList(const iphImgList &aryImgList)
{
	if (aryImgList.GetSize()==0)
	{
		AfxMessageBox("û��Ӱ��!");
		return FALSE;
	}
	m_aryImgList.RemoveAll();
	m_aryImgList.Copy(aryImgList);
	m_bIsPrjModified=true;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::SetLidarList()
//	  Function     :	����Lidar�б�
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
BOOL CALMPrjManager::SetLidarList(const CArray<LIDARINFO,LIDARINFO&> &aryLidarList)
{
	if (aryLidarList.GetSize()==0)
	{
		AfxMessageBox("û��Ӱ��!");
		return FALSE;
	}
	m_aryLidarList.RemoveAll();
	m_aryLidarList.Copy(aryLidarList);
	m_bIsPrjModified=true;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::SetEoeData()
//	  Function     :	����EOE data
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
BOOL CALMPrjManager::SetEoeData(const CArray<OUTPARA,OUTPARA&> &aryEoeData)
{
	if (aryEoeData.GetSize()==0)
	{
		AfxMessageBox("û��Ӱ��!");
		return FALSE;
	}
	m_aryEOEData.RemoveAll();
	m_aryEOEData.Copy(aryEoeData);
	m_bIsPrjModified=true;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::SetAlmFile(CString strAlmFile)
//	  Function     :	���ù����ļ�
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
void CALMPrjManager::SetAlmFile(CString strAlmFile)
{
	m_strAlmFile=strAlmFile;	
	m_strAlmFileDir=m_strAlmFile.Left(m_strAlmFile.ReverseFind('\\'));
	m_strAlmName=m_strAlmFile.Right(m_strAlmFile.GetLength()-m_strAlmFileDir.GetLength()-1);
	m_strAlmName=m_strAlmName.Left(m_strAlmName.GetLength()-4);
}  
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::SetCameraPara(CAMPARA campara)
//	  Function     :	�����������
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
void CALMPrjManager::SetCameraPara(CAMPARA campara)
{
	m_Camera.dfA0=campara.dfA0;
	m_Camera.dfA1=campara.dfA1;
	m_Camera.dfF=campara.dfF;
	m_Camera.dfHeight=campara.dfHeight;
	m_Camera.dfK1=campara.dfK1;
	m_Camera.dfK2=campara.dfK2;
	m_Camera.dfP1=campara.dfP1;
	m_Camera.dfP2=campara.dfP2;
	m_Camera.dfPixSize=campara.dfPixSize;
	m_Camera.dfR=campara.dfR;
	m_Camera.dfU0=campara.dfU0;
	m_Camera.dfV0=campara.dfV0;
	m_Camera.dfWidth=campara.dfWidth;
	m_Camera.nDistortionNum=campara.nDistortionNum;
	m_Camera.nHeight=campara.nHeight;
	m_Camera.nWidth=campara.nWidth;
	m_Camera.nAttrib=campara.nAttrib;

	m_bIsPrjModified=true;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::bReadPOSData()
//	  Function     :	��POS����
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
BOOL	CALMPrjManager::bReadPOSData()
{
	if (m_strPOSFile=="NULL")
	{
		AfxMessageBox("POS�ļ�ΪNULL!");
//		return FALSE;
	}

	//Open file to read
	FILE *pFile=fopen(m_strPOSFile,"r");
	CString strInfo;
	if (pFile==NULL)
	{
		strInfo.Format("���ܴ�"+m_strPOSFile+"!");
		AfxMessageBox(strInfo);
//		return FALSE;
	}
	
	//operate
	char chInfo[PATHLEN];
	fgets(chInfo,PATHLEN,pFile);
	strInfo=CString(chInfo);
	strInfo.TrimLeft();
	strInfo.TrimRight();
	if (strInfo!="##GPSIMU data")
	{
		AfxMessageBox("����POS�ļ�!");
		return FALSE;
	}
	fgets(chInfo,PATHLEN,pFile);
	fgets(chInfo,PATHLEN,pFile);
	int nNum;
	fscanf(pFile,"%d\n",&nNum);
	m_aryPOSData.RemoveAll();
	for (int i = 0; i < nNum ; i++)
	{
		OUTPARA outpara;
		fscanf(pFile,"%s %lf %lf %lf %lf %lf %lf %lf\n",outpara.chImgName,
			&outpara.dfXs,&outpara.dfYs,&outpara.dfZs,&outpara.dfPhi,
			&outpara.dfOmg,&outpara.dfKapa,&outpara.dfGPSTime);
		m_aryPOSData.Add(outpara);
	}
	
	//close files
	fclose(pFile);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::bWritePOSData()
//	  Function     :	дPOS����
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
BOOL	CALMPrjManager::bWritePOSData()
{
	if (m_strPOSFile=="NULL")
	{
		AfxMessageBox("POS�ļ�·��ΪNULL!");
		return FALSE;
	}

	//Open file to read
	FILE *pFile=fopen(m_strPOSFile,"w");
	CString strInfo;
	if (pFile==NULL)
	{
		strInfo.Format("���ܴ�"+m_strPOSFile+"!");
		AfxMessageBox(strInfo);
		return FALSE;
	}
	
	//operate
	fprintf(pFile,"##GPSIMU data\n");
	fprintf(pFile,"##PhotoCount\n");
	fprintf(pFile,"##PhotoName Easting(metres) Northing(metres) H(metres) Phi(radian) Omega(radian) Kap(radian) GPSTime(s)\n");
	int nNum;
	nNum=m_aryPOSData.GetSize();
	fprintf(pFile,"%d\n",nNum);
	for (int i = 0; i < nNum ; i++)
	{
		OUTPARA outpara;
		outpara=m_aryPOSData[i];
		fprintf(pFile,"%20s %15.6lf %15.6lf %15.6lf %15.8lf %15.8lf %15.8lf %15.8lf\n",
			outpara.chImgName,outpara.dfXs,outpara.dfYs,outpara.dfZs,
			outpara.dfPhi,outpara.dfOmg,outpara.dfKapa,outpara.dfGPSTime);
	}
	
	//close files
	fclose(pFile);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::bReadEOEData()
//	  Function     :	���ⷽλԪ������
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
BOOL	CALMPrjManager::bReadEOEData()
{
	if (m_strEOEFile=="NULL")
	{
		AfxMessageBox("EOE�ļ�ΪNULL!");
		return FALSE;
	}

	//Open file to read
	FILE *pFile=fopen(m_strEOEFile,"r");
	CString strInfo;
	if (pFile==NULL)
	{
		strInfo.Format("���ܴ�"+m_strEOEFile+"!");
		AfxMessageBox(strInfo);
		return FALSE;
	}
	
	//operate
	char chInfo[PATHLEN];
	fgets(chInfo,PATHLEN,pFile);
	strInfo=CString(chInfo);
	strInfo.TrimLeft();
	strInfo.TrimRight();
	if (strInfo!="##Exterior Orientation Elements")
	{
		AfxMessageBox("���ǿ�������ļ�!");
		return FALSE;
	}
	fgets(chInfo,PATHLEN,pFile);
	fgets(chInfo,PATHLEN,pFile);
	int nNum;
	fscanf(pFile,"%d\n",&nNum);
	m_aryEOEData.RemoveAll();
	for (int i = 0; i < nNum ; i++)
	{
		OUTPARA outpara;
		fscanf(pFile,"%s %lf %lf %lf %lf %lf %lf %d\n",outpara.chImgName,
			&outpara.dfXs,&outpara.dfYs,&outpara.dfZs,&outpara.dfPhi,
			&outpara.dfOmg,&outpara.dfKapa,&outpara.nFlag);
		m_aryEOEData.Add(outpara);
	}
	
	//close files
	fclose(pFile);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::bWriteEOEData()
//	  Function     :	д�ⷽλԪ������
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
BOOL	CALMPrjManager::bWriteEOEData()
{
	if (m_strEOEFile=="NULL")
	{
		AfxMessageBox("��������ļ�·��ΪNULL!");
		return FALSE;
	}

	//Open file to read
	FILE *pFile=fopen(m_strEOEFile,"w");
	CString strInfo;
	if (pFile==NULL)
	{
		strInfo.Format("���ܴ�"+m_strEOEFile+"!");
		AfxMessageBox(strInfo);
		return FALSE;
	}
	
	//operate
	fprintf(pFile,"##Exterior Orientation Elements\n");
	fprintf(pFile,"##PhotoCount\n");
	fprintf(pFile,"##PhotoName Easting(metres) Northing(metres) H(metres) Phi(radian) Omega(radian) Kap(radian) Flag\n");
	int nNum;
	nNum=m_aryEOEData.GetSize();
	fprintf(pFile,"%d\n",nNum);
	for (int i = 0; i < nNum ; i++)
	{
		OUTPARA outpara;
		outpara=m_aryEOEData[i];
		fprintf(pFile,"%20s %15.6lf %15.6lf %15.6lf %15.8lf %15.8lf %15.8lf %4d\n",
			outpara.chImgName,outpara.dfXs,outpara.dfYs,outpara.dfZs,
			outpara.dfPhi,outpara.dfOmg,outpara.dfKapa,outpara.nFlag);
	}
	
	//close files
	fclose(pFile);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::bReadCPTData()
//	  Function     :	�����Ƶ�����
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
BOOL	CALMPrjManager::bReadCPTData()
{
	if (m_strCPtFile=="NULL")
	{
		AfxMessageBox("���Ƶ��ļ�·��ΪNULL!");
		return FALSE;
	}

	//Open file to read
	FILE *pFile=fopen(m_strCPtFile,"r");
	CString strInfo;
	if (pFile==NULL)
	{
		strInfo.Format("���ܴ�"+m_strCPtFile+"!");
		AfxMessageBox(strInfo);
		return FALSE;
	}
	
	//operate
	char chInfo[PATHLEN];
	fgets(chInfo,PATHLEN,pFile);
	strInfo=CString(chInfo);
	strInfo.TrimLeft();
	strInfo.TrimRight();
	if (strInfo!="##Coordinates of Ground Control Points")
	{
		AfxMessageBox("���ǿ��Ƶ��ļ�!");
		return FALSE;
	}
	fgets(chInfo,PATHLEN,pFile);
	fgets(chInfo,PATHLEN,pFile);
	int nNum;
	fscanf(pFile,"%d\n",&nNum);
	m_aryGCPData.RemoveAll();
	for (int i = 0; i < nNum ; i++)
	{
		CONTROLPOINT cpt;
		fscanf(pFile,"%d %lf %lf %lf %d %d\n",&cpt.nName,
			&cpt.dfX,&cpt.dfY,&cpt.dfZ,&cpt.nAttrib,&cpt.nGroup);
		m_aryGCPData.Add(cpt);
	}
	
	//close files
	fclose(pFile);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::bWriteCPTData()
//	  Function     :	д���Ƶ�����
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.20
////////////////////////////////////////////////////////////////////////////////
BOOL	CALMPrjManager::bWriteCPTData()
{
	if (m_strCPtFile=="NULL")
	{
		AfxMessageBox("���Ƶ��ļ�·��ΪNULL!");
		return FALSE;
	}

	//Open file to read
	FILE *pFile=fopen(m_strCPtFile,"w");
	CString strInfo;
	if (pFile==NULL)
	{
		strInfo.Format("���ܴ�"+m_strCPtFile+"!");
		AfxMessageBox(strInfo);
		return FALSE;
	}
	
	//operate
	fprintf(pFile,"##Coordinates of Ground Control Points\n");
	fprintf(pFile,"##PointCount\n");
	fprintf(pFile,"##PointName        X            Y           Z     Attrib  GroupID\n");
	int nNum;
	nNum=m_aryGCPData.GetSize();
	fprintf(pFile,"%d\n",nNum);
	for (int i = 0; i < nNum ; i++)
	{
		CONTROLPOINT cpt;
		cpt=m_aryGCPData[i];
		fprintf(pFile,"%10d %15.6lf %15.6lf %15.6lf %4d %4d\n",
			cpt.nName,cpt.dfX,cpt.dfY,cpt.dfZ,cpt.nAttrib,cpt.nGroup);
	}
	
	//close files
	fclose(pFile);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::bReadCameraPara()
//	  Function     :	���ļ���ȡ�������
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.22
////////////////////////////////////////////////////////////////////////////////
BOOL CALMPrjManager::bReadCameraPara(CString CameraFile)
{
	//Open file to read
	FILE *pFile=fopen(CameraFile,"r");
	if (pFile==NULL)
	{
		AfxMessageBox("���ܴ�"+CameraFile+"!");
		return FALSE;
	}
	
	//operate
	char chInfo[PATHLEN];
	CString strInfo;
	fgets(chInfo,PATHLEN,pFile);
	strInfo=CString(chInfo);
	strInfo.TrimLeft();
	strInfo.TrimRight();
	if (strInfo!="##Interior Orientation Parameters of Cameras")
	{
		AfxMessageBox("������������ļ�!");
		return FALSE;
	}
	fgets(chInfo,PATHLEN,pFile);
	fgets(chInfo,PATHLEN,pFile);
	int nNum;
	fscanf(pFile,"%d\n",&nNum);
	
	for (int i = 0; i < nNum ; i++)
	{
		fscanf(pFile,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %d\n",&m_Camera.dfU0,
			&m_Camera.dfV0,&m_Camera.dfF,&m_Camera.dfWidth,&m_Camera.dfHeight,
			&m_Camera.dfPixSize,&m_Camera.dfK1,&m_Camera.dfK2,
			&m_Camera.dfP1,&m_Camera.dfP2,&m_Camera.nAttrib);
		m_Camera.nDistortionNum=4;
		m_Camera.nWidth=int(m_Camera.dfWidth/m_Camera.dfPixSize);
		m_Camera.nHeight=int(m_Camera.dfHeight/m_Camera.dfPixSize);
	}
	
	//close files
	fclose(pFile);
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void CALMPrjManager::bWriteCameraPara()
//	  Function     :	д�����������������ļ�
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	ĿǰΪֹ��֧�ֵ������
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2008.12.22
////////////////////////////////////////////////////////////////////////////////
BOOL CALMPrjManager::bWriteCameraPara(CString CameraFile)
{
	//Open file to read
	FILE *pFile=fopen(CameraFile,"w");
	if (pFile==NULL)
	{
		AfxMessageBox("���ܴ�"+CameraFile+"!");
		return FALSE;
	}
	if (m_Camera.nDistortionNum!=4)
	{
		AfxMessageBox("���䲻��4����ģ��,�ݲ�֧��!");
		return FALSE;
	}
	
	//operate
	fprintf(pFile,"##Interior Orientation Parameters of Cameras\n");
	fprintf(pFile,"##CamaraCount\n");
	fprintf(pFile,"##    x0          y0           f          format_X     format_Y      pixelSize      k1            k2            p1            p2     Attrib\n");
	int nNum;
	nNum=1;
	fprintf(pFile,"%d\n",nNum);
	for (int i = 0; i < nNum ; i++)
	{
		if (m_Camera.nAttrib!=1111)
		{
			m_Camera.nAttrib=1111;
		}
		double dfWidth;
		dfWidth=m_Camera.nWidth*m_Camera.dfPixSize;
		if (fabs(dfWidth-m_Camera.dfWidth)>0.000001)
		{
			AfxMessageBox("������(����)������Ԫ��С��������(����)��һ��!");
			return FALSE;
		}
		double dfHeight;
		dfHeight=m_Camera.nHeight*m_Camera.dfPixSize;
		if (fabs(dfHeight-m_Camera.dfHeight)>0.000001)
		{
			AfxMessageBox("����߶�(����)������Ԫ��С������߶�(����)��һ��!");
			return FALSE;
		}
		int nWidth;
		nWidth=int(m_Camera.dfWidth/m_Camera.dfPixSize);
		if (nWidth!=m_Camera.nWidth)
		{
			AfxMessageBox("������(����)������Ԫ��С��������(����)��һ��!");
			return FALSE;
		}
		int nHeight;
		nHeight=int(m_Camera.dfHeight/m_Camera.dfPixSize);
		if (nHeight!=m_Camera.nHeight)
		{
			AfxMessageBox("����߶�(����)������Ԫ��С������߶�(����)��һ��!");
			return FALSE;
		}
		fprintf(pFile,"%12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %12.6lf %e %e %e %e %5d\n",
			m_Camera.dfU0,m_Camera.dfV0,m_Camera.dfF,m_Camera.dfWidth,
			m_Camera.dfHeight,m_Camera.dfPixSize,m_Camera.dfK1,
			m_Camera.dfK2,m_Camera.dfP1,m_Camera.dfP2,m_Camera.nAttrib);
	}
	
	//close files
	fclose(pFile);
	return TRUE;
}

bool CALMPrjManager::GetSwathBoundingBox(char *pLidarFile, BoundingBox3D *pBox)
{
 	FILE* fp;
	fp=fopen(pLidarFile,"rb");
	if(fp==0)
		return false;
	fseek(fp,179,SEEK_SET);
	if(fread(&pBox->XMax,sizeof(double),1,fp)!=1)
		return false;
	if(fread(&pBox->XMin,sizeof(double),1,fp)!=1)
		return false;
	if(fread(&pBox->YMax,sizeof(double),1,fp)!=1)
		return false;
	if(fread(&pBox->YMin,sizeof(double),1,fp)!=1)
		return false;
	if(fread(&pBox->ZMax,sizeof(double),1,fp)!=1)
		return false;
	if(fread(&pBox->ZMin,sizeof(double),1,fp)!=1)
		return false;
 	fclose(fp);
	return	true;
}

bool CALMPrjManager::IsPrjModified()
{
	return	m_bIsPrjModified;
}

bool CALMPrjManager::CreateALMPrj(char *pAlmFilePath)
{
	HINSTANCE oldRcHandle = AfxGetResourceHandle();
#ifdef _DEBUG
	AfxSetResourceHandle ( GetModuleHandle("ALMPrjManagerD.dll") );
#else
	AfxSetResourceHandle ( GetModuleHandle("ALMPrjManager.dll") );
#endif

	bool bReturn=true;
	CString	str;

	CloseALMPrjFile();
	
	SetAlmFile(pAlmFilePath);
	//����Ŀ¼�ṹ
	str.LoadString(IDS_ALM_RawImg_Dir);
	m_strRawImgDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_RawLidar_Dir);
	m_strRawLidarDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_QV_Dir);
	m_strImgQVDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_DodgImg_Dir);
	m_strDgImageDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_Mosaic_Dir);
	m_strMosaicDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_Regist_Dir);
	m_strRegistDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_FilterLidar_Dir);
	m_strFltLidarDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_MatchLidar_Dir);
	m_strMatchLidarDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_DSM_Dir);
	m_strDSMDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_DEM_Dir);
	m_strDEMDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_DOM_Dir);
	m_strDOMDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_ControlFiles_Dir);
	m_strCFilesDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_LOG_Dir);
	m_strLOGDir=m_strAlmFileDir+_T("\\")+str;

	str.LoadString(IDS_ALM_Product_Dir);
	m_strProductDir=m_strAlmFileDir+_T("\\")+str;

	//RawImages
	if(_IsDirectoryExist(m_strRawImgDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strRawImgDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strRawImgDir);
			AfxMessageBox(strInfo);
		}
	}

	//RawLIDAR
	if(_IsDirectoryExist(m_strRawLidarDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strRawLidarDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strRawLidarDir);
			AfxMessageBox(strInfo);
		}
	}

	//QV
	if(_IsDirectoryExist(m_strImgQVDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strImgQVDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strImgQVDir);
			AfxMessageBox(strInfo);
		}
	}

	//DodgingImage
	if(_IsDirectoryExist(m_strDgImageDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strDgImageDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strDgImageDir);
			AfxMessageBox(strInfo);
		}
	}

	//Mosaic
	if(_IsDirectoryExist(m_strMosaicDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strMosaicDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strMosaicDir);
			AfxMessageBox(strInfo);
		}
	}

	//Regist
	if(_IsDirectoryExist(m_strRegistDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strRegistDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strRegistDir);
			AfxMessageBox(strInfo);
		}
	}

	//FilterLas
	if(_IsDirectoryExist(m_strFltLidarDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strFltLidarDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strFltLidarDir);
			AfxMessageBox(strInfo);
		}
	}

	//MatchLas
	if(_IsDirectoryExist(m_strMatchLidarDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strMatchLidarDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strMatchLidarDir);
			AfxMessageBox(strInfo);
		}
	}

	//DSM
	if(_IsDirectoryExist(m_strDSMDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strDSMDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strDSMDir);
			AfxMessageBox(strInfo);
		}
	}

	//DEM
	if(_IsDirectoryExist(m_strDEMDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strDEMDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strDEMDir);
			AfxMessageBox(strInfo);
		}
	}

	//DOM
	if(_IsDirectoryExist(m_strDOMDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strDOMDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strDOMDir);
			AfxMessageBox(strInfo);
		}
	}

	//ControlFiles
	if(_IsDirectoryExist(m_strCFilesDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strCFilesDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strCFilesDir);
			AfxMessageBox(strInfo);
		}
	}

	//LOG
	if(_IsDirectoryExist(m_strProductDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strProductDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strProductDir);
			AfxMessageBox(strInfo);
		}
	}

	//Product
	if(_IsDirectoryExist(m_strLOGDir.GetBuffer(128))==false)
	{
		if(!CreateDirectory(m_strLOGDir,NULL))
		{
			CString strInfo;
			strInfo.Format("���ܴ��������ļ���%s",m_strLOGDir);
			AfxMessageBox(strInfo);
		}
	}

	CArray<CString,CString&> aryStrMetInfo;
//	CString	str;
	str=_T("##Integrated Photogrammetry and LiDAR Register");
	aryStrMetInfo.Add(str);
	str=_T("##V1.0");
	aryStrMetInfo.Add(str);
	str=_T("......");
	aryStrMetInfo.Add(str);

	SetMetaInfo(aryStrMetInfo);

	m_bIsPrjModified=true;
	AfxSetResourceHandle ( oldRcHandle );
	return	bReturn;
}

bool CALMPrjManager::OpenALMPrjFile(CString strAlmFile)
{
	bool	bReturn=true;

	if(strAlmFile.IsEmpty())
	{
		AfxMessageBox("�ļ�·������!", MB_OK|MB_ICONSTOP);
		bReturn=false;
		goto	FUNC_END;
	}

	if(m_strAlmFile.Compare(strAlmFile)==0)
	{//��ͬһ���ļ�
		AfxMessageBox("�����ļ��Ѵ�!", MB_OK|MB_ICONQUESTION);
		goto	FUNC_END;
	}

	bReturn=CloseALMPrjFile();
	if(bReturn==false)
		goto	FUNC_END;

	m_strAlmFile=strAlmFile;
	bReturn=bReadAlmFile(strAlmFile);

FUNC_END:
	return	bReturn;
}

bool CALMPrjManager::CloseALMPrjFile()
{
	bool	bReturn=true;
	
	if(m_bIsPrjModified)
	{
		if(AfxMessageBox("�����Ѹı䣬�Ƿ񱣴�?",MB_YESNO|MB_ICONQUESTION)==IDYES)
		{
			bReturn=SaveALMPrjFile();
			if(bReturn==false)
			{
				AfxMessageBox("���湤���ļ�ʧ��!",MB_OK);
				goto	FUNC_END;
			}
		}
	}
	
	InitALMPrjManager();

FUNC_END:
	return	bReturn;
}

bool CALMPrjManager::SaveALMPrjFile()
{
	bool	bReturn=true;
	
	bReturn=bWriteAlmFile(m_strAlmFile);
	m_bIsPrjModified=false;

	return	bReturn;
}

bool CALMPrjManager::SaveALMPrjAs(CString strAlmFile)
{
	bool	bReturn=true;
	
	bReturn=bWriteAlmFile(strAlmFile);
	if(bReturn==false)
	{
		AfxMessageBox("���湤���ļ�ʧ��!", MB_OK);
		goto	FUNC_END;
	}

	m_strAlmFile=strAlmFile;
	m_bIsPrjModified=false;

FUNC_END:
	return	bReturn;
}

BOOL CALMPrjManager::SetMetaInfo(const CArray<CString,CString&> &aryStrMetInfo)
{
	if (aryStrMetInfo.GetSize()==0)
	{
		AfxMessageBox("û���ļ�ͷ��Ϣ!");
		return FALSE;
	}
	m_aryStrMetaInf.RemoveAll();
	for (int i=0;i<aryStrMetInfo.GetSize();i++)
	{
		CString strInfo;
		strInfo=aryStrMetInfo[i];
		m_aryStrMetaInf.Add(strInfo);
	}
	m_bIsPrjModified=true;
	return TRUE;
}

void  CALMPrjManager::SetImageCount(int nImgNum)
{
	if(m_nImgNum!=nImgNum)
	{
		m_nImgNum=nImgNum;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetRawImageDir(CString strRImgDir)
{
	if(m_strRawImgDir.Compare(strRImgDir)!=0)
	{
		m_strRawImgDir=strRImgDir;
		m_bIsPrjModified=true;
	}
}
void CALMPrjManager::SetRawLidarDir(CString strRLidarDir)
{
	if(m_strRawLidarDir.Compare(strRLidarDir)!=0)
	{
		m_strRawLidarDir=strRLidarDir;
		m_bIsPrjModified=true;
	}
}
void CALMPrjManager::SetImgStripNum(int nImgStripNum)
{
	if(m_nImgStripNum!=nImgStripNum)
	{
		m_nImgStripNum=nImgStripNum;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetLidarStripNum(int nLidarStripNum)
{
	if(m_nLidarStripNum!=nLidarStripNum)
	{
		m_nLidarStripNum=nLidarStripNum;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetATGridNum(int nATGridNum)
{
	if(m_nATGridNum!=nATGridNum)
	{
		m_nATGridNum=nATGridNum;
		m_bIsPrjModified=true;
	}
} 

void CALMPrjManager::SetDSMGridSize(int nDSMGridSize)
{
	if(m_nDSMGridSize!=nDSMGridSize)
	{
		m_nDSMGridSize=nDSMGridSize;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetAverGrdElev(double dfGrdElev)
{
	if(fabs(m_dAverGrdElev-dfGrdElev)>1e-6)
	{
		m_dAverGrdElev=dfGrdElev;
		m_bIsPrjModified=true;
	}
} 

void CALMPrjManager::SetMaxGrdElev(double dfGrdElev)
{
	if(fabs(m_dMaxGrdElev-dfGrdElev)>1e-6)
	{
		m_dMaxGrdElev=dfGrdElev;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetMinGrdElev(double dfGrdElev)
{
	if(fabs(m_dMinGrdElev-dfGrdElev)>1e-6)
	{
		m_dMinGrdElev=dfGrdElev;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetFlyOverlap(short flyoverlap)
{
	if(fabs(double(m_flyOverlap-flyoverlap))>1e-6)
	{
		m_flyOverlap = flyoverlap;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetStripOverlap(short stripoverlap)
{
	if(fabs(double(m_stripOverlap-stripoverlap))>1e-6)
	{
		m_stripOverlap = stripoverlap;
		m_bIsPrjModified=true;
	}

}

void CALMPrjManager::SetPOSFile(CString strPOSFile)
{
	if(m_strPOSFile.Compare(strPOSFile)!=0)
	{
		m_strPOSFile=strPOSFile;
		m_bIsPrjModified=true;
	}
	
} 

void CALMPrjManager::SetEOEFile(CString strEOEFile)
{
	if(m_strEOEFile.Compare(strEOEFile)!=0)
	{
		m_strEOEFile=strEOEFile;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetCPtFile(CString strCPtFile)
{
	if(m_strCPtFile.Compare(strCPtFile)!=0)
	{
		m_strCPtFile=strCPtFile;
		m_bIsPrjModified=true;
	}
	
}

void CALMPrjManager::SetColorLutFile(CString strColorLutFile)
{
	if(m_strColorLutFile.Compare(strColorLutFile)!=0)
	{
		m_strColorLutFile=strColorLutFile;
		m_bIsPrjModified=true;
	}
	
}

void CALMPrjManager::SetImgQVDir(CString strImgQVDir)
{
	if(m_strImgQVDir.Compare(strImgQVDir)!=0)
	{
		m_strImgQVDir=strImgQVDir;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetDgImageDir(CString strDgImageDir)
{
	if(m_strDgImageDir.Compare(strDgImageDir)!=0)
	{
		m_strDgImageDir=strDgImageDir;
		m_bIsPrjModified=true;
	}
	
}

void CALMPrjManager::SetMosaicDir(CString strMosaicDir)
{
	if(m_strMosaicDir.Compare(strMosaicDir)!=0)
	{
		m_strMosaicDir=strMosaicDir;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetRegistDir(CString strRegistDir)
{
	if(m_strRegistDir.Compare(strRegistDir)!=0)
	{
		m_strRegistDir=strRegistDir;
		m_bIsPrjModified=true;
	}
	
}

void CALMPrjManager::SetFltLidarDir(CString strFltLidarDir)
{
	if(m_strFltLidarDir.Compare(strFltLidarDir)!=0)
	{
		m_strFltLidarDir=strFltLidarDir;
		m_bIsPrjModified=true;
	}

}

void CALMPrjManager::SetMatchLidarDir(CString strMatchLidarDir)
{
	if(m_strMatchLidarDir.Compare(strMatchLidarDir)!=0)
	{
		m_strMatchLidarDir=strMatchLidarDir;
		m_bIsPrjModified=true;
	}
}

void CALMPrjManager::SetDSMDir(CString strDSMDir)
{
	if(m_strDSMDir.Compare(strDSMDir)!=0)
	{
		m_strDSMDir=strDSMDir;
		m_bIsPrjModified=true;
	}
} 
 
void CALMPrjManager::SetDEMDir(CString strDEMDir)
{
	if(m_strDEMDir.Compare(strDEMDir)!=0)
	{
		m_strDEMDir=strDEMDir;
		m_bIsPrjModified=true;
	}
	
} 

void CALMPrjManager::SetDOMDir(CString strDOMDir)
{
	if(m_strDOMDir.Compare(strDOMDir)!=0)
	{
		m_strDOMDir=strDOMDir;
		m_bIsPrjModified=true;
	}
	
} 

void CALMPrjManager::SetCFilesDir(CString strCFilesDir)
{
	if(m_strCFilesDir.Compare(strCFilesDir)!=0)
	{
		m_strCFilesDir=strCFilesDir;
		m_bIsPrjModified=true;
	}
	
}

void CALMPrjManager::SetLOGDir(CString strLOGDir)
{
	if(m_strLOGDir.Compare(strLOGDir)!=0)
	{
		m_strLOGDir=strLOGDir;
		m_bIsPrjModified=true;
	}
	
}

void CALMPrjManager::SetProductDir(CString strProductDir)
{
	if(m_strProductDir.Compare(strProductDir)!=0)
	{
		m_strProductDir=strProductDir;
		m_bIsPrjModified=true;
	}
	
}
////////////////////////////////////////////////////////////////////////////////
//	  FunctionName :	void ALMPrjManager::
//	  Function     :	�ж�·���Ƿ����
//	  Principles   :   
//	  Input        :	()
//	  Output       :	None
//	  return       :	None	
//	  assignation  :	None
//	  Note         :	
//				        #include ".h"
//    example      :    
//    Information  :    DQY 2009.07.28
////////////////////////////////////////////////////////////////////////////////
bool  CALMPrjManager::bIsDirectoryExist(const char *sCurDir)
{
	char	sOldDir[_MAX_PATH];
	bool	bExist=true;
	
	ZeroMemory(sOldDir,_MAX_PATH);	
	GetCurrentDirectory(_MAX_PATH,sOldDir);
	
	if (strcmp(sCurDir,sOldDir)==0)	goto	EndPro;
	
	if (SetCurrentDirectory(sCurDir))
		SetCurrentDirectory(sOldDir);
	else
		bExist=false;
	
EndPro:
	return bExist;
} 

bool CALMPrjManager::bSelectFolderDialog(char* pszDlgTitle, CString *strFolder, HWND hParent)
{
	strFolder->Empty();

	LPMALLOC lpMalloc;
	if (::SHGetMalloc(&lpMalloc) != NOERROR) 
		return false;

	char szDisplayName[_MAX_PATH];
	char szBuffer[_MAX_PATH];
	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = hParent;
	browseInfo.pidlRoot = NULL; // set root at Desktop
	browseInfo.pszDisplayName = szDisplayName;
	browseInfo.lpszTitle = pszDlgTitle;
	browseInfo.ulFlags = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS;
	browseInfo.lpfn = NULL;
	browseInfo.lParam = 0;
	LPITEMIDLIST lpItemIDList;
	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
	// Get the path of the selected folder from the item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
		{
		// At this point, szBuffer contains the path the user chose.
			if (szBuffer[0] == '\0') 
				return false;
			// We have a path in szBuffer! Return it.
			*strFolder=szBuffer;
			return true;
		}
		else 
			return true; // strResult is empty
		lpMalloc->Free(lpItemIDList);
		lpMalloc->Release();
	}
	return	false;
} 


bool CALMPrjManager::bIsFileExist(const char *filename)
{
	return (_access(filename, 0) == 0); 
}

void CALMPrjManager::Copy(CALMPrjManager& __x)
{
	m_strAlmFile=__x.GetAlmFile();
	m_strAlmFileDir=__x.GetAlmFileDir();
	m_strAlmName=__x.GetAlmName();
	m_aryStrMetaInf.Copy(*(__x.GetMetaInfo()));
	m_nImgNum=__x.GetImageCount();
	m_strRawImgDir=__x.GetRawImageDir();
	m_nImgStripNum=__x.GetImgStripNum();
	m_aryImgList.Copy(*(__x.GetImgList()));
	m_nLidarStripNum=__x.GetLidarStripNum();
	m_strRawLidarDir=__x.GetRawLidarDir();
	m_aryLidarList.Copy(*(__x.GetLidarList()));
	m_aryPOSData.Copy(*(__x.GetPOSData()));
	m_aryEOEData.Copy(*(__x.GetEOEData()));
//	m_aryGCPData.Copy(*(__x.Get))
	m_Camera=*(__x.GetCameraPara());
	m_nATGridNum=__x.GetATGridNum();
	m_nDSMGridSize=__x.GetDSMGridSize();
	m_dMaxGrdElev=__x.GetMaxGrdElev();
	m_dAverGrdElev=__x.GetAverGrdElev();
	m_dMinGrdElev=__x.GetMinGrdElev();

	m_flyOverlap=__x.GetFlyOverlap();
	m_stripOverlap=__x.GetStripOverlap();

	m_strPOSFile=__x.GetPOSFile();
	m_strEOEFile=__x.GetEOEFile();
	m_strCPtFile=__x.GetCPtFile();
	m_strColorLutFile=__x.GetColorLutFile();
	m_strImgQVDir=__x.GetImgQVDir();
	m_strDgImageDir=__x.GetDgImageDir();
	m_strMosaicDir=__x.GetMosaicDir();
	m_strRegistDir=__x.GetRegistDir();
	m_strFltLidarDir=__x.GetFltLidarDir();
	m_strMatchLidarDir=__x.GetMatchLidarDir();
	m_strDSMDir=__x.GetDSMDir();
	m_strDEMDir=__x.GetDEMDir();
	m_strDOMDir=__x.GetDOMDir();
	m_strCFilesDir=__x.GetCFilesDir();
	m_strLOGDir=__x.GetLOGDir();
	m_strProductDir=__x.GetProductDir();
	m_aryDOMProduct.Copy(*(__x.GetDOMProduct()));
	m_aryDEMProduct.Copy(*(__x.GetDEMProduct()));
	m_aryTMProduct.Copy(*(__x.GetThemeProduct()));
}

