#ifndef _Feature_Match_INCLUDE__
#define _Feature_Match_INCLUDE__

#ifdef _ImageMatchAT_
#define  _Image_Match_  __declspec(dllexport)
#else
#define  _Image_Match_  __declspec(dllimport)	
#endif

#include "Calib_Camera.h"
#include "RansacEstimate/PointPair_Struct.h"
#include <vector>
#include "orsFeature2D\orsIAlg2DFeatureRegistering.h"

//����Ӱ��ƥ�䣬SIFT������ANNƥ��
bool _Image_Match_ KeyMatch_images(const char *imgName1, const char *imgName2, 
	std::vector<orsAdjPtPAIR> *ptPairVec, int &matchNum, int zoom=1);

//�����ļ�����ǿ��Ӱ���ļ�������ѧӰ���ļ���
//����ͶӰ������Ŀռ�������ƥ�䣬ƥ���������ĵ�
bool _Image_Match_ KeyMatch_Lidarimage(const char *atnName, const char *intImgName, const char *optImgName, 
	myPtPair **ptPairVec, int &maxLen, int &matchNum, int zoom=1, int minOverlap=3);

bool _Image_Match_ KeyMatch_Lidarimage_SIFT(const char *atnName, const char *intImgName, const char *optImgName, 
	myPtPair **ptPairVec, int &maxLen, int &matchNum, int zoom=1, int minOverlap=3);

#endif