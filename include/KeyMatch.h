// KeyMatch.h: interface for the CKeyMatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYMATCH_H__885DCD96_C1AF_46AD_937A_26B38B080113__INCLUDED_)
#define AFX_KEYMATCH_H__885DCD96_C1AF_46AD_937A_26B38B080113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "orsGuiBase/orsIProcessMsgBar.h"
#include <vector>
#include "IPhBaseDef.h"
#include "bundler\keys2a.h"
//#include "\openrs\IntegratedPhotogrammetry\external\SiftGPU\SiftGPU\src\SIFTGPU.h"

#ifdef _IMAGE_MATCH_
#define  _Image_Match_  __declspec(dllexport)
#else
#define  _Image_Match_  __declspec(dllimport)
#endif

using namespace std;

int ReadKeyFile(vector<KEYPOINT> *kpList, vector<unsigned char> *desc,  unsigned char **keys, 
				keypt_t **info = NULL);


//ɾ���ظ�ƥ���
void _Image_Match_ DeleteDuplicates(char *pszMatches, char *pszNoDuplicates);


class _Image_Match_ CKeyMatch  
{
public:
	CKeyMatch();
	virtual ~CKeyMatch();
	//����ƥ��ʱ�ı�������
	void	SetMatchRatio(double ratio) { m_ratio = ratio; };
	
	//����ƥ�䣬�������Ż�  Approximate Nearest Neighbor 
	bool	ANNMatch_pairwise(char *pszKeyList, char *pszMatches, orsIProcessMsgBar *process);

	//����������Ƭƥ�䣬������
	bool	ANNMatch_pairwise_strip(vector<string> *pkeyNameList, char *pszMatches, orsIProcessMsgBar *process);

	//�������ƥ��
	std::vector<KeypointMatch>  ANNMatch_pairwise(vector<KEYPOINT> *key1, vector<unsigned char> *desc1, 
		vector<KEYPOINT> *key2, vector<unsigned char> *desc2);

	//fast Approximate Nearest Neighbors	zj 2010.5.4
	bool	flann_pairwise(char *pszKeyList, char *pszMatches, orsIProcessMsgBar *process);


	bool	Match_globaloptimize(char *pszKeyList, char *pszMatches, orsIProcessMsgBar *process);

	std::vector<KEYPOINTMATCH> Match_ann();

	//�����ϵ������ƥ���
	std::vector<KeypointMatch> Match_pairwise_NCC(const char *pKeyFile1, const char *pKeyFile2, double NCC_thresh=0.6, int cand=2);
	
	//��ratio����ƥ���
	std::vector<KeypointMatch> Match_pairwise_ratio(const char *pKeyFile1, const char *pKeyFile2, double ratio_thresh=0.6);

	
protected:
	bool	m_bHasPOS;		//�Ƿ�����POSԼ��ƥ��
	
private:
	double  m_ratio;			//��������
	
};

//int _Image_Match_ ReadSIFTKeyFile(const char *filename, unsigned char **keys, keypt_t **info = NULL);

#ifndef _IMAGE_MATCH_
#ifdef _DEBUG
#pragma comment(lib,"ImageMatchD.lib")
#else
#pragma comment(lib,"ImageMatch.lib")
#endif
#endif

#endif // !defined(AFX_KEYMATCH_H__885DCD96_C1AF_46AD_937A_26B38B080113__INCLUDED_)
