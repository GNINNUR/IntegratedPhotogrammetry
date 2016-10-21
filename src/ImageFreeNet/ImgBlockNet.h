// ImgBlockNet.h: interface for the CImgBlockNet class.
//	create in 2010.4.16		ZJ		Ӱ���������ṹ
//  4.19         ������(������)�ļ���׺ .ibn		
//	4.19		����ʽ������
//				(1) GPS/IMU ���ȽϺ�	ֱ��������������
//				(2) GPS �Ϻ�	��Զ���õ���ʼ�Ľ�Ԫ�أ�����������
//				(3) ������Ƭ	�Զ���Զ��򣬾��Զ��򣬹���������
//
//	4.19	����uselessӰ��Ľӿڣ�����BlockViewer��ViewDSM�ж�δ����Ӧ�޸ģ��Ժ��ٽ�������
//	4.20	sift�����ļ���׺: .sift.txt		.sift.bin
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGFREENET_H__4F34F2BD_19DD_4D3E_BE92_635C7CB03906__INCLUDED_)
#define AFX_IMGFREENET_H__4F34F2BD_19DD_4D3E_BE92_635C7CB03906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdgDataType.h"
#include "orsBase\orsIPlatform.h"
#include "StdAfx.h"

class CALMPrjManager;
class CTINClass;

class _Image_FreeNet_ CImgBlockNet  
{
public:
	CImgBlockNet();
	virtual ~CImgBlockNet();
	
	void	SetBlockPrjInfo(CALMPrjManager *pBlockPrj);//���ù�����Ϣ
	
	bool	CreateBlockNet_POS();	//���ù����е�POS��Ϣ������������ȷ����Ƭ֮������˹�ϵ

//	bool	RebuildFreeNet();	//�ؽ��������������е�m_prjData�����ؽ�

	bool	WriteFile_ibn(const char *pImgNetName);	//д��Ӱ�����ļ�

	bool	ReadFile_ibn(const char *pImgNetName);	//��ȡӰ�����ļ�

	bool	OutputExtOrFile_aop();	//���ⷽλԪ�ص���aop�ļ�
	bool	OutputIntOrFile_iop();	//���ڶ����������iop�ļ�

	bool	LoadExtOrFile_aop();
	bool	LoadIntOrFile_iop();





	bool	DetectSIFT(char *option);	//���������е�Ӱ����SIFT����
	bool	ANNMatch_SIFT(float ratio = 0.6, bool pairwise = false);//Approximate k-NN ƥ�� sift������
	bool	ExtimateFMatrix();	//���öԼ����ι���F����
	
	
	

protected:
	void  ConstructImageTopo();
	float EstimateGroundSizeOfImages( const int numOfNbGiven );



private:
	CALMPrjManager	*m_pBlockPrj;

	bool		*m_pbOrientation;	//��Ƭ�����־
	bool		*m_ImgOverlapM;		//Ӱ���ص�����	�Գ���

private:	
	//image topology
	CTINClass		*m_pImgNet;
	CEnvelope		m_envelope;
	blockINFO		m_prjData;	//���ں���Ӱ���Զ��ؽ�������������Ƭ����

private:
	CString		m_strANNMatchFile;

};

void _Image_FreeNet_ FreeNetSetPlatform(orsIPlatform *pPlatform);

#endif // !defined(AFX_IMGFREENET_H__4F34F2BD_19DD_4D3E_BE92_635C7CB03906__INCLUDED_)
