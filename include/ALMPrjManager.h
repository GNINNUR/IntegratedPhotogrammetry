// CALMPrjManager.h: interface for the CALMPrjManager class.	
//////////////////////////////////////////////////////////////////////
/*#########################################################################################################
����������
		ALM���̹�����  ����ά�����������Ϣ��


�޸��б�
		������2008.12.17
			������:	Jing ZHANG   zhangjing_whu@foxmail.com
					Quanye DU	 duquanye@163.com
		2008.12.18 �����ṹ�Ķ���
		2009.3.27  �����ļ���ΪXML��ʽ
		2009.5.28 �ͷų�#include "Markup.h" �����ƹ�������
		2009.6.5  �����ȡlas��Χ����
		2009.7.22 �޸�Ӱ���б���Ϣ����ImageID Ӱ���������� ��1��ʼ
		
		2010.3.3  ���������Ӱ��������������޸�	zj	
		2010.3.17 Ĭ��Ӱ����ͬһ��Ŀ¼�£�ֻ��¼Ӱ������Ŀ¼��������ÿ��Ӱ���¼ȫ·��

#########################################################################################################*/

#if !defined(AFX_ALMPRJMANAGER_H__A014D0BD_69B7_49CD_824C_F1E2836D83E3__INCLUDED_)
#define AFX_ALMPRJMANAGER_H__A014D0BD_69B7_49CD_824C_F1E2836D83E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ALMDefine.h"
#include "IPhBaseDef.h"



#ifdef _ALMPRJManager_
#define _ALMPrj_ __declspec(dllexport)
//#include "Markup.h"
#else
#define _ALMPrj_ __declspec(dllimport)
//#define CMarkup void
#endif

#ifndef _ALMPRJManager_
#ifdef _DEBUG
#pragma comment(lib, "ALMPrjManagerD.lib")
#pragma message("Automatically Linking With ALMPrjManagerD.dll")
#else
#pragma comment(lib, "ALMPrjManager.lib")
#pragma message("Automatically Linking With ALMPrjManager.dll")
#endif
#endif


class CMarkup;

class _ALMPrj_ CALMPrjManager  
{
public:
	CALMPrjManager();
	virtual ~CALMPrjManager();

public:
	
	bool	CreateALMPrj(char *pAlmFilePath);		//�½�����
	bool	OpenALMPrjFile(CString strAlmFile);		//�򿪹����ļ�
	bool	CloseALMPrjFile();						//�رչ����ļ�
	bool	SaveALMPrjFile();						//���湤���ļ�
	bool	SaveALMPrjAs(CString strAlmFile);		//���Ϊ

	//�����Ƿ��޸�
	bool	IsPrjModified();

			
	//�������ļ��Ľ���(pos,eoe,cpt,camera)
	BOOL	bReadPOSData();								//��POS����
	BOOL    bWritePOSData();							//дPOS����
	BOOL	bReadEOEData();								//���ⷽλԪ������
	BOOL    bWriteEOEData();							//д�ⷽλԪ������
	BOOL	bReadCPTData();								//�����Ƶ�����
	BOOL    bWriteCPTData();							//д���Ƶ�����
	BOOL    bReadCameraPara(CString CameraFile);		//���������
	BOOL    bWriteCameraPara(CString CameraFile);		//д�������


	void    SetAlmFile(CString strAlmFile);           //���ù����ļ�ȫ·��
	void    SetImageCount(int nImgNum);	              //����Ӱ������
	void    SetRawImageDir(CString strRImgDir);		  //����ԭʼӰ��Ŀ¼
	void    SetImgStripNum(int nImgStripNum);		  //����Ӱ�񺽴�����
	void	SetLidarStripNum(int nLidarStripNum);     //����Lidar��������
	void    SetRawLidarDir(CString strRLidarDir);	  //����ԭʼLidarĿ¼
	void	SetATGridNum(int nATGridNum);	          //���ÿ���ƥ������� 
	void	SetDSMGridSize(int nDSMGridSize);         //����DSMƥ�������С

	void	SetMaxGrdElev(double dfGrdElev);
	void	SetAverGrdElev(double dfGrdElev);         //���õ���ƽ���߳�   
	void	SetMinGrdElev(double dfGrdElev);

	void	SetFlyOverlap(short flyoverlap);		  //����Ӱ���ص���	
	void	SetStripOverlap(short stripoverlap);

	void	SetPOSFile(CString strPOSFile);           //����POS�����ļ�            
	void	SetEOEFile(CString strEOEFile);           //������׼����ⷽλԪ���ļ� 
	void	SetCPtFile(CString strCPtFile);           //���õ�����Ƶ������ļ�     
	void	SetColorLutFile(CString strColorLutFile); //����Lidar����ɫ���ļ�      
	void	SetImgQVDir(CString strImgQVDir);         //��������ͼ·��             
	void	SetDgImageDir(CString strDgImageDir);      //�����ȹ���ɫ��Ӱ��·��     
	void	SetMosaicDir(CString strMosaicDir);       //����ƴͼ·��               
	void	SetRegistDir(CString strRegistDir);       //����Lidar��Ӱ����׼����·��
	void	SetFltLidarDir(CString strFltLidarDir);       //�����˲���Lidar����·��      
	void	SetMatchLidarDir(CString strMatchLidarDir);   //����ƥ���Lidar����·��      
	void	SetDSMDir(CString strDSMDir);             //�������ֱ���ģ������·��   
	void	SetDEMDir(CString strDEMDir);             //�������ָ߳�ģ������·��   
	void	SetDOMDir(CString strDOMDir);             //��������Ӱ������·��       
	void	SetCFilesDir(CString strCFilesDir);       //���ÿ��Ƶ���ļ�·��       
	void	SetLOGDir(CString strLOGDir);             //������־�ļ�·��           
	void	SetProductDir(CString strProductDir);     //���ò�Ʒ·��        
	void	SetCameraPara(CAMPARA campara);			  //�����������

	BOOL	SetMetaInfo(const CArray<CString,CString&> &aryStrMetInfo);	      //����Ԫ����
	BOOL    SetImgList(const iphImgList &aryImgList); //����Ӱ���б�
	BOOL    SetLidarList(const CArray<LIDARINFO,LIDARINFO&> &aryLidarList); //����LAS�б�
    BOOL	SetEoeData(const CArray<OUTPARA,OUTPARA&> &aryEoeData); //����EOE data


	//������Ϣ��ѯ�ӿ�	(ֱ��ȡ�ڴ���Ϣ)
	CString GetAlmFile(){return m_strAlmFile;};    //��ȡ�����ļ�ȫ·��
	CString GetAlmFileDir(){return m_strAlmFileDir;};    //��ȡ�����ļ�·��
	CString GetAlmName(){return m_strAlmName;};    //��ȡ�����ļ���		
	int 	GetImageCount()  //��ȡӰ������
	{
		m_nImgNum=m_aryImgList.GetSize();
		return m_nImgNum;
	};    
	CString	GetRawImageDir(){return m_strRawImgDir;};   //��ȡԭʼӰ��Ŀ¼
	CString	GetRawLidarDir(){return m_strRawLidarDir;};		//��ȡԭʼLidarĿ¼
	int 	GetImgStripNum(){return m_nImgStripNum;};   //��ȡӰ�񺽴�����			
	int		GetLidarStripNum()  //��ȡLidar��������
	{
		m_nLidarStripNum=m_aryLidarList.GetSize();
		return m_nLidarStripNum;
	};               
	int		GetATGridNum(){return m_nATGridNum;};                   //��ȡ����ƥ������� 
	int		GetDSMGridSize(){return m_nDSMGridSize;};               //��ȡDSMƥ�������С

	double	GetMinGrdElev(){return m_dMinGrdElev;};
	double	GetAverGrdElev(){return m_dAverGrdElev;};                   //��ȡ����ƽ���߳�   
	double	GetMaxGrdElev(){return m_dMaxGrdElev;};

	short   GetFlyOverlap() {return m_flyOverlap;};
	short	GetStripOverlap() {return m_stripOverlap;};

	CString GetPOSFile(){return m_strPOSFile;};                 //��ȡPOS�����ļ�            
	CString GetEOEFile(){return m_strEOEFile;};                 //��ȡ��׼����ⷽλԪ���ļ� 
	CString GetCPtFile(){return m_strCPtFile;};                 //��ȡ������Ƶ������ļ�     
	CString GetColorLutFile(){return m_strColorLutFile;};       //��ȡLidar����ɫ���ļ�      
	CString GetImgQVDir(){return m_strImgQVDir;};               //��ȡ����ͼ·��             
	CString GetDgImageDir(){return m_strDgImageDir;};            //��ȡ�ȹ���ɫ��Ӱ��·��     
	CString GetMosaicDir(){return m_strMosaicDir;};             //��ȡƴͼ·��               
	CString GetRegistDir(){return m_strRegistDir;};             //��ȡLidar��Ӱ����׼����·��
	CString GetFltLidarDir(){return m_strFltLidarDir;};             //��ȡ�˲���Lidar����·��      
	CString GetMatchLidarDir(){return m_strMatchLidarDir;};         //��ȡƥ���Lidar����·��      
	CString GetDSMDir(){return m_strDSMDir;};                   //��ȡ���ֱ���ģ������·��   
	CString GetDEMDir(){return m_strDEMDir;};                   //��ȡ���ָ߳�ģ������·��   
	CString GetDOMDir(){return m_strDOMDir;};                   //��ȡ����Ӱ������·��       
	CString GetCFilesDir(){return m_strCFilesDir;};             //��ȡ���Ƶ���ļ�·��       
	CString GetLOGDir(){return m_strLOGDir;};                   //��ȡ��־�ļ�·��           
	CString GetProductDir(){return m_strProductDir;};           //��ȡ��Ʒ·��               
	CAMPARA* GetCameraPara(){return &m_Camera;};                //��ȡ�������

	CArray<CString,CString&>* GetMetaInfo(){return &m_aryStrMetaInf;};	 //��ȡԪ����
	iphImgList* GetImgList(){return &m_aryImgList;};     //��ȡӰ���б�
	CArray<LIDARINFO,LIDARINFO&>* GetLidarList(){return &m_aryLidarList;};     //��ȡLidar�б�
	CArray<OUTPARA,OUTPARA&>* GetPOSData(){return &m_aryPOSData;};     //��ȡPOS����
	CArray<OUTPARA,OUTPARA&>* GetEOEData(){return &m_aryEOEData;};     //��ȡ�ⷽλԪ������
	CArray<CString,CString&>* GetDEMProduct(){return &m_aryDEMProduct;};
	CArray<CString,CString&>* GetDOMProduct(){return &m_aryDOMProduct;};
	CArray<CString,CString&>* GetThemeProduct(){return &m_aryTMProduct;};

	bool	GetSwathBoundingBox(char *pLidarFile, BoundingBox3D *pBox);	   //ȡһ��Lidar�İ�Χ��
	//virtual	bool	GetPrjBoundingBox(BoundingBox3D *pBox)=0;					   //ȡ�������̰�Χ��
	//�ж�·���Ƿ����
	bool bIsDirectoryExist(const char *sCurDir);
	//ѡ���ļ���·��
	bool bSelectFolderDialog(char* pszDlgTitle, CString *strFolder, HWND hParent=NULL);
	//�ļ��Ƿ����
	bool bIsFileExist(const char *filename);

public:
	
	void Copy(CALMPrjManager& __x);

protected:	
	//�����ļ�IO
	bool	bReadAlmFile(CString strAlmFile);		//��ȡ�ù����ļ���,m_strAlmFile��ֵȫ·��
	bool	bWriteAlmFile(CString strAlmFile);		//д�빤���ļ� m_strAlmFileΪȫ·��
	void	InitALMPrjManager();					//��ʼ�����̹���
	
	//��ȡ�����ļ�ͷ��Ϣ��,m_strAlmFile��ֵȫ·��
	bool	bReadAlmFileHeader();
	//��ȡ�����ļ�Ӱ����Ϣ
	bool	bReadAlmFileImgInfo();
	//��ȡ�����ļ�Lidar��Ϣ
	bool	bReadAlmFileLidarInfo();
	//��ȡ�����ļ��������
	bool	bReadAlmFileCameraParas();
	//��ȡ�����ļ�Ӱ��ƥ�����
	bool	bReadAlmFileImgMachParas();
	//��ȡ�����ļ�����ļ�·����Ϣ
	bool	bReadAlmFileDirInfo();
    //��ȡ�����ļ���ز�Ʒ·����Ϣ
	bool	bReadAlmFileProductInfo();
	
	//д�빤���ļ�ͷ��Ϣ��,m_strAlmFile��ֵȫ·��
	bool	bWriteAlmFileHeader();
	//д�빤���ļ�Ӱ����Ϣ
	bool	bWriteAlmFileImgInfo();
	//д�빤���ļ�Lidar��Ϣ
	bool	bWriteAlmFileLidarInfo();
	//д�빤���ļ��������
	bool	bWriteAlmFileCameraParas();
	//д�빤���ļ�Ӱ��ƥ�����
	bool	bWriteAlmFileImgMachParas();
	//д�빤���ļ�����ļ�·����Ϣ
	bool	bWriteAlmFileDirInfo();
    //д�빤���ļ���ز�Ʒ·����Ϣ
	bool	bWriteAlmFileProductInfo();
	

protected:
	bool		m_bIsPrjModified;		//�����ļ��е���Ϣ�Ƿ��ѱ��޸�

private:
	//XML��д
	CMarkup* m_xml;

	CString m_ALMFileFlag;      //�ļ���ʶ "ALMPSProjectFile"
	CString	m_strAlmFile;	    //�����ļ�ȫ·��
	CString	m_strAlmFileDir;	//�����ļ�·�� .alm�ļ����ڵ�·��
	CString	m_strAlmName;		//�����ļ�·�� ��������׺ ".alm"

	CArray<CString,CString&>	m_aryStrMetaInf;	            //Ԫ������Ϣ

	int m_nImgNum;                          //Ӱ������
	CString	m_strRawImgDir;		            //ԭʼӰ��Ŀ¼
	int m_nImgStripNum;                     //Ӱ�񺽴�����
//	CArray<IMGLIST,IMGLIST> m_aryImgList;   //Ӱ���б�
	iphImgList		m_aryImgList;			//Ӱ���б�

	CArray<OUTPARA,OUTPARA&> m_aryPOSData;   //POS����
	CArray<OUTPARA,OUTPARA&> m_aryEOEData;   //�ⷽλԪ������
	CArray<CONTROLPOINT,CONTROLPOINT&> m_aryGCPData;   //������Ƶ�����
	CAMPARA m_Camera;                       //�������

	double	m_dMaxGrdElev;					//�������߳�
	double	m_dAverGrdElev;                    //����ƽ���߳�
	double	m_dMinGrdElev;					//������С�߳�

	short	m_flyOverlap;					//�����ص�
	short	m_stripOverlap;					//�����ص�


	int m_nLidarStripNum;                     //Lidar��������
	CString m_strRawLidarDir;	            	//ԭʼLidar�ļ�Ŀ¼
	CArray<LIDARINFO,LIDARINFO&> m_aryLidarList;   //Lidar�б�

	
	int m_nATGridNum;                       //����ƥ�������
	int m_nDSMGridSize;                     //DSMƥ�������С
 
	CString m_strPOSFile;                   //POS�����ļ�
	CString m_strEOEFile;                   //��׼����ⷽλԪ���ļ�
	CString m_strCPtFile;                   //������Ƶ������ļ�
	CString m_strColorLutFile;              //Lidar����ɫ���ļ�
	CString m_strImgQVDir;                  //����ͼ·��
	CString m_strDgImageDir;                //�ȹ���ɫ��Ӱ��·��
	CString m_strMosaicDir;                 //ƴͼ·��
	CString m_strRegistDir;                 //Lidar��Ӱ����׼����·��
	CString m_strFltLidarDir;                 //�˲���Lidar����·��
	CString m_strMatchLidarDir;               //ƥ���Lidar����·��
	CString m_strDSMDir;                    //���ֱ���ģ������·��
	CString m_strDEMDir;                    //���ָ߳�ģ������·��
	CString m_strDOMDir;                    //����Ӱ������·��
	CString m_strCFilesDir;                 //���Ƶ���ļ�·��
	CString m_strLOGDir;                    //��־�ļ�·��
	CString m_strProductDir;                //��Ʒ·��
	CArray<CString,CString&> m_aryDOMProduct;   //DOM��Ʒ����
	CArray<CString,CString&> m_aryDEMProduct;   //DEM��Ʒ����
	CArray<CString,CString&> m_aryTMProduct;    //ר���Ʒ����

};

typedef CArray<LIDARINFO,LIDARINFO&> iphLIDARList;

#endif // !defined(AFX_ALMPRJMANAGER_H__A014D0BD_69B7_49CD_824C_F1E2836D83E3__INCLUDED_)
