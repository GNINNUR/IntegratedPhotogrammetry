#ifndef _ANN_Key_Match_H__
#define _ANN_Key_Match_H__

#ifdef _ImageMatchAT_
#define  _Image_Match_  __declspec(dllexport)
#else
#define  _Image_Match_  __declspec(dllimport)	
#endif

#include <vector>

#include "Calib_Camera.h"
#include "orsBase/orsTypedef.h"
#include "orsBase/orsString.h"
#include "orsBase/orsArray.h"
class orsMatchLinks;

//�Թ�ѧӰ��
//�������������  zj2014.12.7
//�޸Ĳο�alignment���void CMainFrame::OnMatchlidimg()
bool _Image_Match_ KeyMatch_pairwise_ransac(const char *imgNameList, const char *atTiePtsFileName, 
	_iphCamera	*pCamera, const char *imgKeyDir, int zoom=1, int minOverlap=3);

bool _Image_Match_ OutputMatchLinks(orsMatchLinks *mtLinkMap, orsArray<orsString> *vStripFiles, 
									std::vector<int> &imgIDVec, int minOverlap, const char *tieFileName);

#endif