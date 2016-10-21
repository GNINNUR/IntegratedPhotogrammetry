#ifndef _LIDAR_ADJUSTMENT_DEFINE_INCLUDE_H__
#define _LIDAR_ADJUSTMENT_DEFINE_INCLUDE_H__

#include "orsLidarGeometry/LidGeo_def.h"
#include <vector>

//Lidar����ƽ��ģ��
enum orsLidSA_TYPE
{
	LidSA_3DSimilarityTrans = 0,		//3D���Ʊ任
	LidSA_POSDrift,						//POSƯ��	
	LidSA_OriAnchor,					//�����ģ��
};


//����ƽ���в��õ����ӵ�ṹ
struct LidAdj_keyPt
{
	int tieID;				//���ӵ�ID����0��ʼ, ˳�������ͬ�����ӵ��tieID��ͬ
//	int stripID;			//����ID, ��0��ʼ���
	double  vX, vY, vZ;		//�۲�ֵ
	double  eX, eY, eZ;		//����ֵ (��ֵ)

	std::vector<LidPt_SurvInfo> *pLidPoints;	//ʵ�ʼ��������	
	char	VP_type;		//0: undefined;  1: horizontal and vertical control point;   2: horizontal control point;   3: vertical control point
	char	reliability;		//0: ���ɿ���1: �ɿ�

	LidAdj_keyPt()
	{
		pLidPoints=NULL;
		reliability=1;
	}

	~LidAdj_keyPt()
	{
		if(pLidPoints)	delete pLidPoints;	pLidPoints=NULL;
	}
};

#endif