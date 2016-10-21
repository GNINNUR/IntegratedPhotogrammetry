// AlignPrj.h: interface for the CAlignPrj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALIGNPRJ_H__839F298B_521B_4C73_B8FD_BDE37180777E__INCLUDED_)
#define AFX_ALIGNPRJ_H__839F298B_521B_4C73_B8FD_BDE37180777E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Align_def.h"

#ifdef _CALIB_UTILITY_
#define  _CalibUtility_Dll_  __declspec(dllexport)
#else
#define  _CalibUtility_Dll_  __declspec(dllimport)	
#endif

// #ifndef _CALIB_UTILITY_
// #ifdef _DEBUG
// #pragma comment(lib,"CalibUtility.lib")
// #else
// #pragma comment(lib,"CalibUtility.lib")
// #endif
// #endif

typedef struct
{
	char file_signature[4];	//ALNF
	int lineNum;
	int imgNum;
	int tieObjNum;
//	int gcpNum;	//ʵ����Ƶ�
	unsigned int offsetToLidLine;
	unsigned int offsetToImage;
	unsigned int offsetToTieObj;
//	unsigned int offsetToGCP;
	int reserved;

} AlignPrj_Header;

//  modified  by zj 2010.12.4   �����ļ�����Ӱ���ļ���atn�ļ�����ͬһ��Ŀ¼��
//  2010.12.6  zj  �������ӵ��GCP��־���ļ��ṹ���ı�
//  2011.8.9   zj	���ӵ������˳�����ſ��Ƶ㣬�������ӵ�

class _CalibUtility_Dll_ CAlignPrj  
{
public:
	CAlignPrj();
	virtual ~CAlignPrj();

//	bool IsOpen() { return m_bIsOpen; };

	void Close();
	bool Open(char *pszPrjName);
	bool Save();
	bool SaveAs();

	CArray<Align_LidLine, Align_LidLine> *GetLidList();
	CArray<Align_Image, Align_Image> *GetImgList();
	CArray<TieObjChain*, TieObjChain*> *GetTieObjList();
//	CArray<TieObj_GCP*, TieObj_GCP*>  *GetGCPList();

	//��ȡӰ���ʱ���־
	bool loadImageTimeStamp(const char* pszFileName);

	void ReleaseTieList();

	void ImportLidLine(Align_LidLine lidline);
	
	CString& GetFilePath() {return m_strPrjName;}; 

	CString GetLiDARName(int lidID);
	CString GetImgName(int imgID);

	
protected:
	
	bool ReadHeader(FILE *fp);
	bool ReadAlignLidLineInfo(FILE *fp);
	bool ReadAlignImageInfo(FILE *fp);
	bool ReadTieObjects(FILE *fp);

	bool WriteHeader(FILE *fp);
	bool WriteAlignLidLineInfo(FILE *fp);
	bool WriteAlignImageInfo(FILE *fp);
	bool WriteTieObjects(FILE *fp);

	

public:
//	bool	m_bIsOpen;
	AlignPrj_Header	m_header;
	CString  m_strPrjName;
	CString  m_strDir; 

	CArray<Align_LidLine, Align_LidLine>	m_LidList;	
	CArray<Align_Image, Align_Image>  m_ImgList;

	CArray<TieObjChain*, TieObjChain*>  m_TieList;
//	CArray<TieObj_GCP*, TieObj_GCP*>   m_GCPList;
};

#endif // !defined(AFX_ALIGNPRJ_H__839F298B_521B_4C73_B8FD_BDE37180777E__INCLUDED_)
